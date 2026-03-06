/*
	Copyright (C) 2025
	Part of the Battle for Wesnoth Project https://www.wesnoth.org/

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY.

	See the COPYING file for more details.
*/

#define GETTEXT_DOMAIN "wesnoth-test"

#include <boost/test/unit_test.hpp>

#include "actions/undo_action.hpp"
#include "actions/attack.hpp"
#include "actions/move.hpp"
#include "config.hpp"
#include "log.hpp"
#include "map/location.hpp"
#include "tests/utils/game_config_manager_tests.hpp"

#include <memory>
#include <string>

/*
./test --report_level=detailed --log_level=all --run_test=actions_suite
*/

BOOST_AUTO_TEST_SUITE( actions_suite )

// Test swarm_blows function with various inputs
BOOST_AUTO_TEST_CASE( test_swarm_blows )
{
	// Test basic cases
	// When hp >= max_hp, should return max_blows
	BOOST_CHECK_EQUAL( swarm_blows(5, 10, 100, 100), 10 );
	BOOST_CHECK_EQUAL( swarm_blows(3, 7, 50, 50), 7 );
	BOOST_CHECK_EQUAL( swarm_blows(0, 5, 30, 30), 5 );

	// When max_blows < min_blows, special formula applies
	// min_blows - (min_blows - max_blows) * hp / max_hp
	BOOST_CHECK_EQUAL( swarm_blows(10, 5, 50, 100), 8 ); // 10 - (10-5)*50/100 = 10 - 2 = 8
	BOOST_CHECK_EQUAL( swarm_blows(8, 4, 25, 100), 7 );  // 8 - (8-4)*25/100 = 8 - 1 = 7

	// When max_blows >= min_blows, normal swarm formula
	// min_blows + (max_blows - min_blows) * hp / max_hp
	BOOST_CHECK_EQUAL( swarm_blows(5, 10, 50, 100), 7 );  // 5 + (10-5)*50/100 = 5 + 2 = 7
	BOOST_CHECK_EQUAL( swarm_blows(0, 10, 50, 100), 5 );  // 0 + (10-0)*50/100 = 5
	BOOST_CHECK_EQUAL( swarm_blows(0, 10, 0, 100), 0 );   // 0 + 10*0/100 = 0
	BOOST_CHECK_EQUAL( swarm_blows(0, 10, 100, 100), 10 );// 0 + 10*100/100 = 10

	// Edge cases
	BOOST_CHECK_EQUAL( swarm_blows(5, 10, 1, 100), 5 );   // Very low hp
	BOOST_CHECK_EQUAL( swarm_blows(5, 10, 99, 100), 9 );  // Almost full hp
}

// Test battle_context_unit_stats simple constructor (used for testing)
BOOST_AUTO_TEST_CASE( test_battle_context_unit_stats_basics )
{
	config game_config(test_utils::get_test_config());

	// Test basic construction with minimal parameters
	battle_context_unit_stats stats1(10, 3, 30, 50, 60);

	BOOST_CHECK_EQUAL( stats1.damage, 10 );
	BOOST_CHECK_EQUAL( stats1.num_blows, 3 );
	BOOST_CHECK_EQUAL( stats1.hp, 30 );
	BOOST_CHECK_EQUAL( stats1.max_hp, 50 );
	BOOST_CHECK_EQUAL( stats1.chance_to_hit, 60 );
	BOOST_CHECK_EQUAL( stats1.rounds, 1 ); // Not berserk
	BOOST_CHECK_EQUAL( stats1.swarm, false ); // No swarm
	BOOST_CHECK_EQUAL( stats1.drains, false );
	BOOST_CHECK_EQUAL( stats1.slows, false );
	BOOST_CHECK_EQUAL( stats1.is_slowed, false );
	BOOST_CHECK_EQUAL( stats1.firststrike, false );

	// Test with drain enabled
	battle_context_unit_stats stats2(15, 2, 40, 60, 70, true);

	BOOST_CHECK_EQUAL( stats2.damage, 15 );
	BOOST_CHECK_EQUAL( stats2.num_blows, 2 );
	BOOST_CHECK_EQUAL( stats2.drain_percent, 50 );
	BOOST_CHECK_EQUAL( stats2.drains, true );

	// Test with slows enabled
	battle_context_unit_stats stats3(12, 4, 25, 40, 50, false, true);

	BOOST_CHECK_EQUAL( stats3.slows, true );
	BOOST_CHECK_EQUAL( stats3.drains, false );

	// Test with slowed status
	battle_context_unit_stats stats4(20, 2, 35, 50, 60, false, false, true);

	BOOST_CHECK_EQUAL( stats4.is_slowed, true );
	// Slowed units have reduced damage
	BOOST_CHECK_EQUAL( stats4.damage, stats4.slow_damage );

	// Test with berserk (rounds = 30)
	battle_context_unit_stats stats5(8, 3, 30, 40, 50, false, false, false, true);

	BOOST_CHECK_EQUAL( stats5.rounds, 30 );

	// Test with firststrike
	battle_context_unit_stats stats6(10, 2, 30, 40, 60, false, false, false, false, true);

	BOOST_CHECK_EQUAL( stats6.firststrike, true );

	// Test with swarm
	battle_context_unit_stats stats7(10, 4, 25, 50, 60, false, false, false, false, false, true);

	BOOST_CHECK_EQUAL( stats7.swarm, true );
	BOOST_CHECK_EQUAL( stats7.swarm_min, 0 );
	BOOST_CHECK_EQUAL( stats7.swarm_max, 4 );
	// With swarm, num_blows is scaled by hp/max_hp
	BOOST_CHECK_EQUAL( stats7.num_blows, 2 ); // 4 * 25/50 = 2
}

// Test battle_context_unit_stats damage calculation
BOOST_AUTO_TEST_CASE( test_battle_context_unit_stats_damage )
{
	// Test that damage is properly clamped to max 0
	battle_context_unit_stats stats1(0, 2, 30, 50, 60);
	BOOST_CHECK_EQUAL( stats1.damage, 0 );

	// Test slow_damage calculation
	battle_context_unit_stats stats2(10, 2, 30, 50, 60);
	// slow_damage = round_damage(damage, 1, 2) which is typically damage/2
	BOOST_CHECK_EQUAL( stats2.slow_damage, 5 );
	BOOST_CHECK_EQUAL( stats2.damage, 10 );

	// Test that slowed units use slow_damage for damage
	battle_context_unit_stats stats3(10, 2, 30, 50, 60, false, false, true);
	BOOST_CHECK_EQUAL( stats3.damage, 5 );
	BOOST_CHECK_EQUAL( stats3.slow_damage, 5 );
}

// Test battle_context_unit_stats hp clamping
BOOST_AUTO_TEST_CASE( test_battle_context_unit_stats_hp_clamp )
{
	// HP should be clamped to at least 0
	battle_context_unit_stats stats1(10, 2, -5, 50, 60);
	BOOST_CHECK_EQUAL( stats1.hp, 0 );

	// HP should be clamped to at most max_hp
	battle_context_unit_stats stats2(10, 2, 100, 50, 60);
	BOOST_CHECK_EQUAL( stats2.hp, 50 );
	BOOST_CHECK_EQUAL( stats2.max_hp, 50 );

	// Normal HP value
	battle_context_unit_stats stats3(10, 2, 30, 50, 60);
	BOOST_CHECK_EQUAL( stats3.hp, 30 );
}

// Test battle_context_unit_stats calc_blows function
BOOST_AUTO_TEST_CASE( test_battle_context_unit_stats_calc_blows )
{
	// Create stats with swarm
	battle_context_unit_stats stats(10, 4, 25, 50, 60, false, false, false, false, false, true);

	// Verify swarm settings
	BOOST_CHECK_EQUAL( stats.swarm_min, 0 );
	BOOST_CHECK_EQUAL( stats.swarm_max, 4 );

	// Test calc_blows with different HP values
	BOOST_CHECK_EQUAL( stats.calc_blows(0), 0 );    // 0 + 4*0/50 = 0
	BOOST_CHECK_EQUAL( stats.calc_blows(25), 2 );   // 0 + 4*25/50 = 2
	BOOST_CHECK_EQUAL( stats.calc_blows(50), 4 );   // 0 + 4*50/50 = 4
	BOOST_CHECK_EQUAL( stats.calc_blows(12), 0 );   // 0 + 4*12/50 = 0 (integer division)
	BOOST_CHECK_EQUAL( stats.calc_blows(13), 1 );   // 0 + 4*13/50 = 1 (integer division)

	// Create stats without swarm
	battle_context_unit_stats stats2(10, 4, 25, 50, 60);
	BOOST_CHECK_EQUAL( stats2.swarm_min, 4 );
	BOOST_CHECK_EQUAL( stats2.swarm_max, 4 );
	BOOST_CHECK_EQUAL( stats2.calc_blows(25), 4 );  // Always 4 without swarm
	BOOST_CHECK_EQUAL( stats2.calc_blows(50), 4 );
}

// Test undo_action_container basic operations
BOOST_AUTO_TEST_CASE( test_undo_action_container )
{
	actions::undo_action_container container;

	// Test initial state
	BOOST_CHECK( container.empty() );
	BOOST_CHECK_EQUAL( container.steps().size(), 0 );
}

// Test map_location direction operations used in move actions
BOOST_AUTO_TEST_CASE( test_move_direction_basics )
{
	map_location loc1(5, 5);
	map_location loc2(6, 5);

	// Test basic location creation
	BOOST_CHECK_EQUAL( loc1, map_location(5, 5) );
	BOOST_CHECK( loc1 != loc2 );

	// Test direction parsing
	map_location::direction dir = map_location::parse_direction("n");
	BOOST_CHECK( dir == map_location::direction::north );

	dir = map_location::parse_direction("s");
	BOOST_CHECK( dir == map_location::direction::south );

	dir = map_location::parse_direction("ne");
	BOOST_CHECK( dir == map_location::direction::north_east );

	dir = map_location::parse_direction("sw");
	BOOST_CHECK( dir == map_location::direction::south_west );

	// Test invalid direction
	dir = map_location::parse_direction("invalid");
	BOOST_CHECK( dir == map_location::direction::indeterminate );
}

// Test get_opposite_direction function
BOOST_AUTO_TEST_CASE( test_opposite_directions )
{
	BOOST_CHECK_EQUAL(
		map_location::get_opposite_direction(map_location::direction::north),
		map_location::direction::south
	);

	BOOST_CHECK_EQUAL(
		map_location::get_opposite_direction(map_location::direction::south),
		map_location::direction::north
	);

	BOOST_CHECK_EQUAL(
		map_location::get_opposite_direction(map_location::direction::north_east),
		map_location::direction::south_west
	);

	BOOST_CHECK_EQUAL(
		map_location::get_opposite_direction(map_location::direction::south_west),
		map_location::direction::north_east
	);

	BOOST_CHECK_EQUAL(
		map_location::get_opposite_direction(map_location::direction::north_west),
		map_location::direction::south_east
	);

	BOOST_CHECK_EQUAL(
		map_location::get_opposite_direction(map_location::direction::south_east),
		map_location::direction::north_west
	);
}

// Test rotate_direction function
BOOST_AUTO_TEST_CASE( test_rotate_direction )
{
	// Rotate north clockwise
	BOOST_CHECK_EQUAL(
		map_location::rotate_direction(map_location::direction::north, 1),
		map_location::direction::north_east
	);

	// Rotate north counter-clockwise
	BOOST_CHECK_EQUAL(
		map_location::rotate_direction(map_location::direction::north, -1),
		map_location::direction::north_west
	);

	// Rotate by 3 is same as opposite
	BOOST_CHECK_EQUAL(
		map_location::rotate_direction(map_location::direction::north, 3),
		map_location::direction::south
	);

	BOOST_CHECK_EQUAL(
		map_location::rotate_direction(map_location::direction::north_east, 3),
		map_location::direction::south_west
	);
}

// Test distance_between function used in movement
BOOST_AUTO_TEST_CASE( test_distance_between )
{
	map_location loc1(0, 0);
	map_location loc2(1, 0);
	map_location loc3(2, 0);
	map_location loc4(0, 1);
	map_location loc5(5, 5);

	// Distance to self is 0
	BOOST_CHECK_EQUAL( distance_between(loc1, loc1), 0 );

	// Distance to adjacent hex is 1
	BOOST_CHECK_EQUAL( distance_between(loc1, loc2), 1 );

	// Distance to non-adjacent hexes
	BOOST_CHECK_EQUAL( distance_between(loc1, loc3), 2 );
	BOOST_CHECK_EQUAL( distance_between(loc1, loc4), 1 );
	// Distance from (0,0) to (5,5) on hex grid is 8
	BOOST_CHECK_EQUAL( distance_between(loc1, loc5), 8 );

	// Distance is symmetric
	BOOST_CHECK_EQUAL( distance_between(loc1, loc5), distance_between(loc5, loc1) );
}

// Test tiles_adjacent function
BOOST_AUTO_TEST_CASE( test_tiles_adjacent )
{
	map_location center(5, 5);

	// Get all adjacent directions
	map_location n  = center.get_direction(map_location::direction::north);
	map_location ne = center.get_direction(map_location::direction::north_east);
	map_location se = center.get_direction(map_location::direction::south_east);
	map_location s  = center.get_direction(map_location::direction::south);
	map_location sw = center.get_direction(map_location::direction::south_west);
	map_location nw = center.get_direction(map_location::direction::north_west);

	// All directions should be adjacent
	BOOST_CHECK( tiles_adjacent(center, n) );
	BOOST_CHECK( tiles_adjacent(center, ne) );
	BOOST_CHECK( tiles_adjacent(center, se) );
	BOOST_CHECK( tiles_adjacent(center, s) );
	BOOST_CHECK( tiles_adjacent(center, sw) );
	BOOST_CHECK( tiles_adjacent(center, nw) );

	// Adjacency is symmetric
	BOOST_CHECK( tiles_adjacent(n, center) );
	BOOST_CHECK( tiles_adjacent(ne, center) );

	// Non-adjacent tiles
	map_location far(10, 10);
	BOOST_CHECK( !tiles_adjacent(center, far) );

	// Same tile is not adjacent
	BOOST_CHECK( !tiles_adjacent(center, center) );
}

// Test battle_context_unit_stats with berserk rounds
BOOST_AUTO_TEST_CASE( test_battle_context_unit_stats_berserk )
{
	// Berserk attacks have rounds = 30
	battle_context_unit_stats stats(10, 3, 30, 50, 60, false, false, false, true);

	BOOST_CHECK_EQUAL( stats.rounds, 30 );
	BOOST_CHECK_EQUAL( stats.damage, 10 );
	BOOST_CHECK_EQUAL( stats.num_blows, 3 );
	BOOST_CHECK_EQUAL( stats.chance_to_hit, 60 );
}

// Test battle_context_unit_stats drain percent calculation
BOOST_AUTO_TEST_CASE( test_battle_context_unit_stats_drain_calc )
{
	// With drain enabled, drain_percent should be 50
	battle_context_unit_stats stats1(20, 2, 40, 60, 70, true);
	BOOST_CHECK_EQUAL( stats1.drain_percent, 50 );
	BOOST_CHECK_EQUAL( stats1.drain_constant, 0 );
	BOOST_CHECK_EQUAL( stats1.drains, true );

	// Without drain, drain_percent should be 0
	battle_context_unit_stats stats2(20, 2, 40, 60, 70, false);
	BOOST_CHECK_EQUAL( stats2.drain_percent, 0 );
	BOOST_CHECK_EQUAL( stats2.drain_constant, 0 );
	BOOST_CHECK_EQUAL( stats2.drains, false );
}

// Test battle_context_unit_stats poison and plague flags
BOOST_AUTO_TEST_CASE( test_battle_context_unit_stats_special_abilities )
{
	// The simple constructor doesn't support poison/plague directly,
	// but we can verify the defaults
	battle_context_unit_stats stats(10, 3, 30, 50, 60);

	BOOST_CHECK_EQUAL( stats.poisons, false );
	BOOST_CHECK_EQUAL( stats.plagues, false );
	BOOST_CHECK_EQUAL( stats.petrifies, false );
	BOOST_CHECK_EQUAL( stats.disable, false );
}

// Test battle_context_unit_stats swarm with various HP ratios
BOOST_AUTO_TEST_CASE( test_swarm_with_hp_ratios )
{
	// Swarm with 8 max blows, starting at various HP levels
	battle_context_unit_stats stats(10, 8, 32, 64, 60, false, false, false, false, false, true);

	BOOST_CHECK_EQUAL( stats.swarm_min, 0 );
	BOOST_CHECK_EQUAL( stats.swarm_max, 8 );
	BOOST_CHECK_EQUAL( stats.swarm, true );

	// At 32/64 HP (50%), blows should be 4
	BOOST_CHECK_EQUAL( stats.num_blows, 4 );

	// Test calc_blows at different HP levels
	BOOST_CHECK_EQUAL( stats.calc_blows(0), 0 );    // 0%
	BOOST_CHECK_EQUAL( stats.calc_blows(16), 2 );   // 25%
	BOOST_CHECK_EQUAL( stats.calc_blows(32), 4 );   // 50%
	BOOST_CHECK_EQUAL( stats.calc_blows(48), 6 );   // 75%
	BOOST_CHECK_EQUAL( stats.calc_blows(64), 8 );   // 100%
	BOOST_CHECK_EQUAL( stats.calc_blows(80), 8 );   // Over max HP, capped
}

// Test swarm_blows edge cases with zero values
BOOST_AUTO_TEST_CASE( test_swarm_blows_zero_cases )
{
	// Zero max_blows should return 0
	BOOST_CHECK_EQUAL( swarm_blows(0, 0, 50, 100), 0 );
	BOOST_CHECK_EQUAL( swarm_blows(0, 0, 0, 100), 0 );

	// Zero min with non-zero max
	BOOST_CHECK_EQUAL( swarm_blows(0, 5, 0, 100), 0 );
	BOOST_CHECK_EQUAL( swarm_blows(0, 5, 100, 100), 5 );

	// Zero max_hp edge case (should handle division)
	BOOST_CHECK_EQUAL( swarm_blows(5, 10, 50, 0), 10 ); // hp >= max_hp returns max_blows
}

// Test swarm_blows with max_blows < min_blows (decreasing blows)
BOOST_AUTO_TEST_CASE( test_swarm_blows_decreasing )
{
	// When max < min, blows decrease with HP loss
	// Formula: min - (min - max) * hp / max_hp
	BOOST_CHECK_EQUAL( swarm_blows(10, 5, 100, 100), 5 ); // Full HP: min - 5*1 = 5
	BOOST_CHECK_EQUAL( swarm_blows(10, 5, 75, 100), 7 );  // 75% HP: 10 - 5*0.75 = 7 (rounded down from 6.25, but formula uses int)
	BOOST_CHECK_EQUAL( swarm_blows(10, 5, 50, 100), 8 );  // 50% HP: 10 - 5*0.5 = 8 (rounded from 7.5)
	BOOST_CHECK_EQUAL( swarm_blows(10, 5, 25, 100), 9 );  // 25% HP: 10 - 5*0.25 = 9 (rounded from 8.75)
	BOOST_CHECK_EQUAL( swarm_blows(10, 5, 0, 100), 10 );  // 0% HP: min
}

// Test map_location vector operations used in movement
BOOST_AUTO_TEST_CASE( test_map_location_vector_operations )
{
	std::vector<map_location> route;
	route.push_back(map_location(0, 0));
	route.push_back(map_location(1, 0));
	route.push_back(map_location(2, 0));
	route.push_back(map_location(3, 0));

	BOOST_CHECK_EQUAL( route.size(), 4 );
	BOOST_CHECK_EQUAL( route.front(), map_location(0, 0) );
	BOOST_CHECK_EQUAL( route.back(), map_location(3, 0) );

	// Test iterator distance
	auto dist = std::distance(route.begin(), route.end());
	BOOST_CHECK_EQUAL( dist, 4 );
}

// Test map_location write and read operations
BOOST_AUTO_TEST_CASE( test_map_location_config_io )
{
	config cfg;
	map_location loc(5, 10);

	// Write location to config
	cfg["x"] = loc.wml_x();
	cfg["y"] = loc.wml_y();

	// Read it back (wml_loc constructor already handles 1-based to 0-based conversion)
	map_location read_loc(cfg["x"], cfg["y"], wml_loc());

	BOOST_CHECK_EQUAL( read_loc, loc );
}

// Test map_location all directions are distinct
BOOST_AUTO_TEST_CASE( test_map_location_all_directions_distinct )
{
	map_location center(5, 5);
	std::set<map_location> adjacent;

	adjacent.insert(center.get_direction(map_location::direction::north));
	adjacent.insert(center.get_direction(map_location::direction::north_east));
	adjacent.insert(center.get_direction(map_location::direction::south_east));
	adjacent.insert(center.get_direction(map_location::direction::south));
	adjacent.insert(center.get_direction(map_location::direction::south_west));
	adjacent.insert(center.get_direction(map_location::direction::north_west));

	// All 6 directions should produce distinct locations
	BOOST_CHECK_EQUAL( adjacent.size(), 6 );

	// None should be equal to center
	for(const auto& loc : adjacent) {
		BOOST_CHECK( loc != center );
	}
}

// Test map_location null/invalid location handling
BOOST_AUTO_TEST_CASE( test_map_location_validity )
{
	map_location valid(5, 5);
	map_location invalid;

	BOOST_CHECK( valid.valid() );
	BOOST_CHECK( !invalid.valid() );

	// Test that null_location constant works
	BOOST_CHECK( !map_location::null_location().valid() );
}

// Test battle_context_unit_stats experience values
BOOST_AUTO_TEST_CASE( test_battle_context_unit_stats_experience )
{
	battle_context_unit_stats stats(10, 3, 30, 50, 60);

	// Default experience values from simple constructor
	BOOST_CHECK_EQUAL( stats.experience, 0 );
	BOOST_CHECK_EQUAL( stats.max_experience, 1000000 );
	BOOST_CHECK_EQUAL( stats.level, 1 );
}

// Test battle_context_unit_stats leadership bonus
BOOST_AUTO_TEST_CASE( test_battle_context_unit_stats_leadership )
{
	battle_context_unit_stats stats(10, 3, 30, 50, 60);

	// Default leadership bonus is 0
	BOOST_CHECK_EQUAL( stats.leadership_bonus, 0 );
}

// Test battle_context_unit_stats max_hp clamping
BOOST_AUTO_TEST_CASE( test_battle_context_unit_stats_max_hp_clamp )
{
	// max_hp should be at least 1
	battle_context_unit_stats stats(10, 3, 30, 0, 60);
	BOOST_CHECK_EQUAL( stats.max_hp, 1 );
}

// Test undo_action_container factory registration
BOOST_AUTO_TEST_CASE( test_undo_action_container_factories )
{
	// The factory map should be accessible
	auto& factories = actions::undo_action_container::get_factories();
	BOOST_CHECK( !factories.empty() );

	// Should have factory for "event" type
	BOOST_CHECK( factories.find("event") != factories.end() );
}

// Test undo_action_container add and empty
BOOST_AUTO_TEST_CASE( test_undo_action_container_add )
{
	actions::undo_action_container container;
	BOOST_CHECK( container.empty() );

	// The container supports adding steps, but we can't easily create
	// concrete undo_action instances without a full game context
	BOOST_CHECK_EQUAL( container.steps().size(), 0 );
}

// Test map_location parity for hex grid
BOOST_AUTO_TEST_CASE( test_map_location_hex_parity )
{
	// In a hex grid, adjacent cells have different parity
	// Even row: (0,0), odd row: (0,1)
	map_location even_row(5, 0);
	map_location odd_row(5, 1);

	// Both should be valid locations
	BOOST_CHECK( even_row.valid() );
	BOOST_CHECK( odd_row.valid() );

	// NE direction from even row goes to different y than from odd row
	map_location ne_from_even = even_row.get_direction(map_location::direction::north_east);
	map_location ne_from_odd = odd_row.get_direction(map_location::direction::north_east);

	// Both NEs should be adjacent to their origin
	BOOST_CHECK( tiles_adjacent(even_row, ne_from_even) );
	BOOST_CHECK( tiles_adjacent(odd_row, ne_from_odd) );
}

// Test distance between hexes is symmetric
BOOST_AUTO_TEST_CASE( test_distance_symmetry )
{
	std::vector<map_location> test_locs = {
		map_location(0, 0),
		map_location(5, 5),
		map_location(10, 3),
		map_location(7, 12),
		map_location(15, 15)
	};

	for(size_t i = 0; i < test_locs.size(); ++i) {
		for(size_t j = i + 1; j < test_locs.size(); ++j) {
			BOOST_CHECK_EQUAL(
				distance_between(test_locs[i], test_locs[j]),
				distance_between(test_locs[j], test_locs[i])
			);
		}
	}
}

// Test map_location direction round-trip
BOOST_AUTO_TEST_CASE( test_direction_round_trip )
{
	map_location center(10, 10);

	// For each direction, applying opposite twice should get back to original
	std::vector<map_location::direction> dirs = {
		map_location::direction::north,
		map_location::direction::north_east,
		map_location::direction::south_east,
		map_location::direction::south,
		map_location::direction::south_west,
		map_location::direction::north_west
	};

	for(auto dir : dirs) {
		map_location neighbor = center.get_direction(dir);
		map_location::direction back_dir = map_location::get_opposite_direction(dir);
		map_location back = neighbor.get_direction(back_dir);

		BOOST_CHECK_EQUAL( center, back );
	}
}

// Test rotate_direction wraps around correctly
BOOST_AUTO_TEST_CASE( test_rotate_direction_wrap )
{
	// Rotating north by 6 should return to north (6 * 60 degrees = 360)
	BOOST_CHECK_EQUAL(
		map_location::rotate_direction(map_location::direction::north, 6),
		map_location::direction::north
	);

	// Rotating by -6 should also return to north
	BOOST_CHECK_EQUAL(
		map_location::rotate_direction(map_location::direction::north, -6),
		map_location::direction::north
	);

	// Rotating by 12 is also full circle
	BOOST_CHECK_EQUAL(
		map_location::rotate_direction(map_location::direction::north, 12),
		map_location::direction::north
	);
}

// Test swarm with very low HP
BOOST_AUTO_TEST_CASE( test_swarm_very_low_hp )
{
	// Unit with swarm at 1 HP out of 100
	unsigned blows = swarm_blows(0, 10, 1, 100);
	BOOST_CHECK_EQUAL( blows, 0 ); // 0 + 10 * 1/100 = 0 (integer division)

	// Unit with swarm at 10 HP out of 100
	blows = swarm_blows(0, 10, 10, 100);
	BOOST_CHECK_EQUAL( blows, 1 ); // 0 + 10 * 10/100 = 1
}

// Test battle_context_unit_stats slow_damage with odd damage values
BOOST_AUTO_TEST_CASE( test_slow_damage_odd_values )
{
	// Damage 9 slowed should be 5 (rounded from 4.5)
	battle_context_unit_stats stats1(9, 2, 30, 50, 60, false, false, true);
	BOOST_CHECK_EQUAL( stats1.damage, 5 );
	BOOST_CHECK_EQUAL( stats1.slow_damage, 5 );

	// Damage 11 slowed should be 6 (rounded from 5.5)
	battle_context_unit_stats stats2(11, 2, 30, 50, 60, false, false, true);
	BOOST_CHECK_EQUAL( stats2.damage, 6 );
	BOOST_CHECK_EQUAL( stats2.slow_damage, 6 );

	// Damage 7 slowed should be 4 (rounded from 3.5)
	battle_context_unit_stats stats3(7, 2, 30, 50, 60, false, false, true);
	BOOST_CHECK_EQUAL( stats3.damage, 4 );
	BOOST_CHECK_EQUAL( stats3.slow_damage, 4 );
}

// Test battle_context_unit_stats with multiple specials enabled
BOOST_AUTO_TEST_CASE( test_battle_context_unit_stats_combined_specials )
{
	// Drain + slows + swarm
	battle_context_unit_stats stats(15, 6, 30, 60, 70, true, true, false, false, false, true);

	BOOST_CHECK_EQUAL( stats.drains, true );
	BOOST_CHECK_EQUAL( stats.slows, true );
	BOOST_CHECK_EQUAL( stats.swarm, true );
	BOOST_CHECK_EQUAL( stats.drain_percent, 50 );
	BOOST_CHECK_EQUAL( stats.swarm_min, 0 );
	BOOST_CHECK_EQUAL( stats.swarm_max, 6 );
	// At 30/60 HP, swarm blows = 0 + 6 * 30/60 = 3
	BOOST_CHECK_EQUAL( stats.num_blows, 3 );
}

// Test map_location comparison operators
BOOST_AUTO_TEST_CASE( test_map_location_comparison )
{
	map_location a(5, 5);
	map_location b(5, 5);
	map_location c(6, 5);
	map_location d(5, 6);

	// Equality
	BOOST_CHECK( a == b );
	BOOST_CHECK( !(a == c) );
	BOOST_CHECK( !(a == d) );

	// Inequality
	BOOST_CHECK( !(a != b) );
	BOOST_CHECK( a != c );
	BOOST_CHECK( a != d );
}

// Test that tiles_adjacent is symmetric
BOOST_AUTO_TEST_CASE( test_tiles_adjacent_symmetric )
{
	std::vector<map_location> test_locs = {
		map_location(10, 10),
		map_location(11, 10),
		map_location(10, 11),
		map_location(12, 10),
		map_location(13, 15)
	};

	for(size_t i = 0; i < test_locs.size(); ++i) {
		for(size_t j = i + 1; j < test_locs.size(); ++j) {
			BOOST_CHECK_EQUAL(
				tiles_adjacent(test_locs[i], test_locs[j]),
				tiles_adjacent(test_locs[j], test_locs[i])
			);
		}
	}
}

// Test battle_context_unit_stats with high damage values
BOOST_AUTO_TEST_CASE( test_battle_context_unit_stats_high_damage )
{
	// High damage attack
	battle_context_unit_stats stats(100, 1, 30, 50, 90);
	BOOST_CHECK_EQUAL( stats.damage, 100 );
	BOOST_CHECK_EQUAL( stats.num_blows, 1 );
	BOOST_CHECK_EQUAL( stats.chance_to_hit, 90 );

	// Verify slow_damage calculation for high values
	BOOST_CHECK_EQUAL( stats.slow_damage, 50 ); // 100 / 2
}

// Test swarm_blows at exact HP thresholds
BOOST_AUTO_TEST_CASE( test_swarm_blows_thresholds )
{
	// With 10 max blows and 100 max HP, each blow needs 10 HP
	unsigned blows;

	// Exactly at threshold for 1 blow
	blows = swarm_blows(0, 10, 10, 100);
	BOOST_CHECK_EQUAL( blows, 1 );

	// Just below threshold for 2 blows
	blows = swarm_blows(0, 10, 19, 100);
	BOOST_CHECK_EQUAL( blows, 1 );

	// Exactly at threshold for 2 blows
	blows = swarm_blows(0, 10, 20, 100);
	BOOST_CHECK_EQUAL( blows, 2 );

	// Just below threshold for 3 blows
	blows = swarm_blows(0, 10, 29, 100);
	BOOST_CHECK_EQUAL( blows, 2 );
}

BOOST_AUTO_TEST_SUITE_END()
