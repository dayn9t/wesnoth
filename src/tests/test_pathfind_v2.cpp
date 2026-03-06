/*
	Copyright (C) 2025
	by Battle for Wesnoth Project https://www.wesnoth.org/

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

#include "pathfind/pathfind.hpp"
#include "pathfind/teleport.hpp"
#include "map/location.hpp"
#include "config.hpp"

#include <cmath>
#include <limits>

BOOST_AUTO_TEST_SUITE( test_pathfind_v2 )

// ============================================================================
// Cost Calculator Tests
// ============================================================================

// Mock cost calculator for testing
class mock_cost_calculator : public pathfind::cost_calculator
{
public:
	mock_cost_calculator(double fixed_cost = 1.0)
		: fixed_cost_(fixed_cost)
		, call_count_(0)
	{}

	virtual double cost(const map_location& loc, const double so_far) const override
	{
		++call_count_;
		// Return different costs based on location for testing
		if (loc.x < 0 || loc.y < 0) {
			return getNoPathValue();  // Impassable
		}
		return fixed_cost_;
	}

	int get_call_count() const { return call_count_; }
	void reset_call_count() { call_count_ = 0; }

private:
	double fixed_cost_;
	mutable int call_count_;
};

// Test cost_calculator no-path value is consistent
BOOST_AUTO_TEST_CASE( test_cost_calculator_no_path_consistency )
{
	double no_path = pathfind::cost_calculator::getNoPathValue();

	// Should be a specific sentinel value
	BOOST_CHECK_EQUAL(no_path, 42424242.0);

	// Should be positive
	BOOST_CHECK(no_path > 0);

	// Should be greater than any reasonable movement cost
	BOOST_CHECK(no_path > 1000.0);

	// Should be finite
	BOOST_CHECK(std::isfinite(no_path));
}

// Test mock cost calculator basic behavior
BOOST_AUTO_TEST_CASE( test_mock_cost_calculator_basic )
{
	mock_cost_calculator calc(2.5);

	map_location valid_loc(5, 5);
	map_location invalid_loc(-1, -1);

	BOOST_CHECK_EQUAL(calc.cost(valid_loc, 0.0), 2.5);
	BOOST_CHECK_EQUAL(calc.cost(valid_loc, 10.0), 2.5);  // Should not depend on so_far

	// Invalid locations should return no-path
	BOOST_CHECK_EQUAL(calc.cost(invalid_loc, 0.0), pathfind::cost_calculator::getNoPathValue());
}

// Test cost calculator call counting
BOOST_AUTO_TEST_CASE( test_cost_calculator_call_counting )
{
	mock_cost_calculator calc(1.0);

	BOOST_CHECK_EQUAL(calc.get_call_count(), 0);

	map_location loc1(1, 1);
	map_location loc2(2, 2);
	map_location loc3(3, 3);

	calc.cost(loc1, 0.0);
	BOOST_CHECK_EQUAL(calc.get_call_count(), 1);

	calc.cost(loc2, 0.0);
	calc.cost(loc3, 0.0);
	BOOST_CHECK_EQUAL(calc.get_call_count(), 3);

	calc.reset_call_count();
	BOOST_CHECK_EQUAL(calc.get_call_count(), 0);
}

// Test cost calculator with various fixed costs
BOOST_AUTO_TEST_CASE( test_cost_calculator_various_costs )
{
	// Very cheap terrain
	mock_cost_calculator cheap_calc(0.1);
	BOOST_CHECK_EQUAL(cheap_calc.cost(map_location(0, 0), 0.0), 0.1);

	// Expensive terrain
	mock_cost_calculator expensive_calc(100.0);
	BOOST_CHECK_EQUAL(expensive_calc.cost(map_location(0, 0), 0.0), 100.0);

	// Zero cost (edge case)
	mock_cost_calculator zero_calc(0.0);
	BOOST_CHECK_EQUAL(zero_calc.cost(map_location(0, 0), 0.0), 0.0);
}

// ============================================================================
// Plain Route Tests
// ============================================================================

// Test plain_route default state
BOOST_AUTO_TEST_CASE( test_plain_route_default_state )
{
	pathfind::plain_route route;

	BOOST_CHECK(route.steps.empty());
	BOOST_CHECK_EQUAL(route.move_cost, 0);
}

// Test plain_route with various step counts
BOOST_AUTO_TEST_CASE( test_plain_route_various_steps )
{
	pathfind::plain_route route;

	// Single step
	route.steps.push_back(map_location(0, 0));
	route.move_cost = 0;
	BOOST_CHECK_EQUAL(route.steps.size(), 1);

	// Multiple steps forming a path
	route.steps.clear();
	for (int i = 0; i < 10; ++i) {
		route.steps.push_back(map_location(i, i));
	}
	route.move_cost = 10;

	BOOST_CHECK_EQUAL(route.steps.size(), 10);
	BOOST_CHECK_EQUAL(route.move_cost, 10);

	// Verify path continuity
	for (size_t i = 1; i < route.steps.size(); ++i) {
		int dist = distance_between(route.steps[i-1], route.steps[i]);
		BOOST_CHECK(dist >= 1);  // Adjacent or further (teleport could be further)
	}
}

// Test plain_route copy semantics
BOOST_AUTO_TEST_CASE( test_plain_route_copy )
{
	pathfind::plain_route original;
	original.steps.push_back(map_location(5, 5));
	original.steps.push_back(map_location(6, 5));
	original.move_cost = 2;

	// Copy by value
	pathfind::plain_route copied = original;
	BOOST_CHECK_EQUAL(copied.steps.size(), 2);
	BOOST_CHECK_EQUAL(copied.move_cost, 2);
	BOOST_CHECK(copied.steps[0] == original.steps[0]);

	// Modify original, check copy is independent
	original.steps.clear();
	original.move_cost = 0;

	BOOST_CHECK_EQUAL(copied.steps.size(), 2);
	BOOST_CHECK_EQUAL(copied.move_cost, 2);
}

// Test plain_route with large move costs
BOOST_AUTO_TEST_CASE( test_plain_route_large_costs )
{
	pathfind::plain_route route;

	route.steps.push_back(map_location(0, 0));
	route.move_cost = INT_MAX;

	BOOST_CHECK_EQUAL(route.move_cost, INT_MAX);

	// Assign no-path value
	route.move_cost = static_cast<int>(pathfind::cost_calculator::getNoPathValue());
	BOOST_CHECK_EQUAL(route.move_cost, static_cast<int>(42424242.0));
}

// ============================================================================
// Marked Route Tests
// ============================================================================

// Test marked_route default constructor
BOOST_AUTO_TEST_CASE( test_marked_route_default )
{
	pathfind::marked_route route;

	BOOST_CHECK(route.steps.empty());
	BOOST_CHECK_EQUAL(route.move_cost, 0);
	BOOST_CHECK(route.marks.empty());
}

// Test marked_route mark default values
BOOST_AUTO_TEST_CASE( test_marked_route_mark_defaults )
{
	pathfind::marked_route::mark m;

	BOOST_CHECK_EQUAL(m.turns, 0);
	BOOST_CHECK_EQUAL(m.zoc, false);
	BOOST_CHECK_EQUAL(m.capture, false);
	BOOST_CHECK_EQUAL(m.invisible, false);
}

// Test marked_route mark with all parameters
BOOST_AUTO_TEST_CASE( test_marked_route_mark_all_params )
{
	pathfind::marked_route::mark m(3, true, true, false);

	BOOST_CHECK_EQUAL(m.turns, 3);
	BOOST_CHECK_EQUAL(m.zoc, true);
	BOOST_CHECK_EQUAL(m.capture, true);
	BOOST_CHECK_EQUAL(m.invisible, false);
}

// Test marked_route mark equality with various combinations
BOOST_AUTO_TEST_CASE( test_marked_route_mark_equality_combinations )
{
	pathfind::marked_route::mark base(2, true, false, true);

	// Same values
	pathfind::marked_route::mark same(2, true, false, true);
	BOOST_CHECK(base == same);

	// Different turns
	pathfind::marked_route::mark diff_turns(3, true, false, true);
	BOOST_CHECK(!(base == diff_turns));

	// Different zoc
	pathfind::marked_route::mark diff_zoc(2, false, false, true);
	BOOST_CHECK(!(base == diff_zoc));

	// Different capture
	pathfind::marked_route::mark diff_capture(2, true, true, true);
	BOOST_CHECK(!(base == diff_capture));

	// Different invisible
	pathfind::marked_route::mark diff_invis(2, true, false, false);
	BOOST_CHECK(!(base == diff_invis));
}

// Test marked_route marks map operations
BOOST_AUTO_TEST_CASE( test_marked_route_marks_operations )
{
	pathfind::marked_route route;

	map_location loc1(5, 5);
	map_location loc2(6, 5);
	map_location loc3(7, 5);

	// Add marks
	route.marks[loc1] = pathfind::marked_route::mark(1, false, false, false);
	route.marks[loc2] = pathfind::marked_route::mark(2, true, true, false);
	route.marks[loc3] = pathfind::marked_route::mark(3, false, true, true);

	BOOST_CHECK_EQUAL(route.marks.size(), 3);

	// Find marks
	auto it = route.marks.find(loc2);
	BOOST_CHECK(it != route.marks.end());
	BOOST_CHECK_EQUAL(it->second.turns, 2);
	BOOST_CHECK_EQUAL(it->second.zoc, true);

	// Non-existent mark
	auto not_found = route.marks.find(map_location(100, 100));
	BOOST_CHECK(not_found == route.marks.end());

	// Erase mark
	route.marks.erase(loc1);
	BOOST_CHECK_EQUAL(route.marks.size(), 2);
	BOOST_CHECK(route.marks.find(loc1) == route.marks.end());
}

// ============================================================================
// Paths Tests
// ============================================================================

// Test paths::dest_vect with many locations
BOOST_AUTO_TEST_CASE( test_paths_dest_vect_many_locations )
{
	pathfind::paths::dest_vect destinations;

	// Insert many locations
	for (int i = 0; i < 100; ++i) {
		destinations.insert(map_location(i % 20, i / 20));
	}

	BOOST_CHECK_EQUAL(destinations.size(), 100);

	// Test contains for all inserted
	for (int i = 0; i < 100; ++i) {
		BOOST_CHECK(destinations.contains(map_location(i % 20, i / 20)));
	}

	// Test contains for non-existent
	BOOST_CHECK(!destinations.contains(map_location(100, 100)));
}

// Test paths::dest_vect insert idempotence
BOOST_AUTO_TEST_CASE( test_paths_dest_vect_insert_idempotence )
{
	pathfind::paths::dest_vect destinations;

	map_location loc(5, 5);

	destinations.insert(loc);
	BOOST_CHECK_EQUAL(destinations.size(), 1);

	// Insert same location again - should not duplicate
	destinations.insert(loc);
	BOOST_CHECK_EQUAL(destinations.size(), 1);

	// Insert different location
	destinations.insert(map_location(6, 5));
	BOOST_CHECK_EQUAL(destinations.size(), 2);
}

// Test paths::step structure
BOOST_AUTO_TEST_CASE( test_paths_step_structure )
{
	pathfind::paths::step s;

	s.curr = map_location(10, 10);
	s.prev = map_location(9, 10);
	s.move_left = 5;

	BOOST_CHECK(s.curr == map_location(10, 10));
	BOOST_CHECK(s.prev == map_location(9, 10));
	BOOST_CHECK_EQUAL(s.move_left, 5);

	// Test with negative move_left (could indicate a turn boundary)
	s.move_left = -3;
	BOOST_CHECK_EQUAL(s.move_left, -3);

	// Test with large move_left
	s.move_left = INT_MAX;
	BOOST_CHECK_EQUAL(s.move_left, INT_MAX);
}

// Test paths::dest_vect get_path with simple path
BOOST_AUTO_TEST_CASE( test_paths_dest_vect_get_path_simple )
{
	pathfind::paths::dest_vect destinations;

	// Create a simple path: (0,0) -> (1,0) -> (2,0) -> (3,0)
	// Each step has prev pointing to previous location
	pathfind::paths::step s1;
	s1.curr = map_location(0, 0);
	s1.prev = map_location();  // No previous (start)
	s1.move_left = 10;

	pathfind::paths::step s2;
	s2.curr = map_location(1, 0);
	s2.prev = map_location(0, 0);
	s2.move_left = 9;

	pathfind::paths::step s3;
	s3.curr = map_location(2, 0);
	s3.prev = map_location(1, 0);
	s3.move_left = 8;

	pathfind::paths::step s4;
	s4.curr = map_location(3, 0);
	s4.prev = map_location(2, 0);
	s4.move_left = 7;

	// Push in order for binary search
	destinations.push_back(s1);
	destinations.push_back(s2);
	destinations.push_back(s3);
	destinations.push_back(s4);

	BOOST_CHECK_EQUAL(destinations.size(), 4);

	// Find destination
	auto it = destinations.find(map_location(3, 0));
	BOOST_CHECK(it != destinations.end());

	// Get path to destination
	std::vector<map_location> path = destinations.get_path(it);
	// Path should trace back through previous locations
	BOOST_CHECK(path.size() >= 1);
	BOOST_CHECK(path[0] == map_location(0, 0));  // Start
}

// ============================================================================
// VACANT_TILE_TYPE Tests
// ============================================================================

// Test VACANT_TILE_TYPE enumeration values
BOOST_AUTO_TEST_CASE( test_vacant_tile_type_values )
{
	// VACANT_CASTLE and VACANT_ANY should be different
	BOOST_CHECK_NE(pathfind::VACANT_CASTLE, pathfind::VACANT_ANY);

	// They should be comparable
	int castle_val = pathfind::VACANT_CASTLE;
	int any_val = pathfind::VACANT_ANY;

	BOOST_CHECK(castle_val != any_val);
}

// ============================================================================
// Teleport Map Tests
// ============================================================================

// Test teleport_map default construction
BOOST_AUTO_TEST_CASE( test_teleport_map_default )
{
	pathfind::teleport_map tm;

	BOOST_CHECK(tm.empty());

	// Get adjacents of any location should return empty set
	const auto& adj = tm.get_adjacents(map_location(5, 5));
	BOOST_CHECK(adj.empty());

	// Sources and targets should be empty
	BOOST_CHECK(tm.get_sources().empty());
	BOOST_CHECK(tm.get_targets().empty());
}

// Test teleport_map empty set behavior
BOOST_AUTO_TEST_CASE( test_teleport_map_empty_set )
{
	pathfind::teleport_map tm;

	// All locations should have empty adjacent sets
	for (int x = 0; x < 10; ++x) {
		for (int y = 0; y < 10; ++y) {
			const auto& adj = tm.get_adjacents(map_location(x, y));
			BOOST_CHECK(adj.empty());
		}
	}
}

// ============================================================================
// Boundary Condition Tests
// ============================================================================

// Test distance_between with same location
BOOST_AUTO_TEST_CASE( test_distance_same_location )
{
	map_location loc(10, 10);
	BOOST_CHECK_EQUAL(distance_between(loc, loc), 0);

	// Also test with zero location
	BOOST_CHECK_EQUAL(distance_between(map_location::ZERO(), map_location::ZERO()), 0);
}

// Test distance_between maximum values
BOOST_AUTO_TEST_CASE( test_distance_maximum_coordinates )
{
	// Test with large positive coordinates
	map_location far1(1000, 1000);
	map_location far2(2000, 2000);

	std::size_t dist = distance_between(far1, far2);
	BOOST_CHECK(dist > 0);
	BOOST_CHECK(dist < 2000);  // Should be less than Manhattan distance

	// Test with large negative coordinates
	map_location far3(-1000, -1000);
	map_location far4(-2000, -2000);

	dist = distance_between(far3, far4);
	BOOST_CHECK(dist > 0);
}

// Test distance_between asymmetric coordinates
BOOST_AUTO_TEST_CASE( test_distance_asymmetric )
{
	map_location loc1(10, 0);
	map_location loc2(0, 10);

	std::size_t dist12 = distance_between(loc1, loc2);
	std::size_t dist21 = distance_between(loc2, loc1);

	// Distance should be symmetric
	BOOST_CHECK_EQUAL(dist12, dist21);
}

// Test map_location validity with various border sizes
BOOST_AUTO_TEST_CASE( test_location_validity_borders )
{
	// Location at edge with various border sizes
	map_location edge(0, 0);

	BOOST_CHECK(edge.valid(10, 10, 0));   // No border needed
	BOOST_CHECK(edge.valid(10, 10, 5));   // Large border allowed

	// Location outside normal bounds but within border
	map_location outside(-2, -2);

	BOOST_CHECK(!outside.valid(10, 10, 0));   // No border
	BOOST_CHECK(!outside.valid(10, 10, 1));   // Small border
	BOOST_CHECK(outside.valid(10, 10, 5));    // Large border
}

// Test direction operations boundary cases
BOOST_AUTO_TEST_CASE( test_direction_boundary_values )
{
	using direction = map_location::direction;

	// Test all 6 directions
	std::vector<direction> all_dirs = {
		direction::north,
		direction::north_east,
		direction::south_east,
		direction::south,
		direction::south_west,
		direction::north_west
	};

	for (const auto& dir : all_dirs) {
		// Full rotation should return to original
		BOOST_CHECK_EQUAL(map_location::rotate_direction(dir, 6), dir);
		BOOST_CHECK_EQUAL(map_location::rotate_direction(dir, -6), dir);
		BOOST_CHECK_EQUAL(map_location::rotate_direction(dir, 12), dir);

		// Opposite of opposite should be original
		direction opposite = map_location::get_opposite_direction(dir);
		direction back = map_location::get_opposite_direction(opposite);
		BOOST_CHECK_EQUAL(back, dir);

		// Write and compare
		std::string written = map_location::write_direction(dir);
		BOOST_CHECK(!written.empty());
	}
}

// Test get_adjacent_tiles at various map positions
BOOST_AUTO_TEST_CASE( test_adjacent_tiles_various_positions )
{
	std::vector<map_location> test_positions = {
		map_location(0, 0),
		map_location(1, 1),
		map_location(10, 10),
		map_location(100, 100),
		map_location(-10, -10)  // Negative coordinates
	};

	for (const auto& pos : test_positions) {
		auto adjacent = get_adjacent_tiles(pos);

		// Should always have exactly 6 neighbors in hex
		BOOST_CHECK_EQUAL(adjacent.size(), 6);

		// All neighbors should be unique
		std::set<map_location> unique_check(adjacent.begin(), adjacent.end());
		BOOST_CHECK_EQUAL(unique_check.size(), 6);

		// All neighbors should be different from center
		for (const auto& adj : adjacent) {
			BOOST_CHECK(adj != pos);
		}

		// All neighbors should have distance 1 from center
		for (const auto& adj : adjacent) {
			BOOST_CHECK_EQUAL(distance_between(pos, adj), 1);
		}
	}
}

// Test tiles_adjacent with various distances
BOOST_AUTO_TEST_CASE( test_tiles_adjacent_various )
{
	map_location center(10, 10);
	auto adjacent = get_adjacent_tiles(center);

	// Adjacent tiles
	for (const auto& adj : adjacent) {
		BOOST_CHECK(tiles_adjacent(center, adj));
		BOOST_CHECK(tiles_adjacent(adj, center));  // Symmetric
	}

	// Same tile is not adjacent
	BOOST_CHECK(!tiles_adjacent(center, center));

	// Far tiles are not adjacent
	map_location far(20, 20);
	BOOST_CHECK(!tiles_adjacent(center, far));

	// Distance-2 tiles are not adjacent
	map_location dist2(12, 10);
	BOOST_CHECK_EQUAL(distance_between(center, dist2), 2);
	BOOST_CHECK(!tiles_adjacent(center, dist2));
}

// ============================================================================
// Edge Case Tests
// ============================================================================

// Test plain_route with duplicate steps
BOOST_AUTO_TEST_CASE( test_plain_route_duplicate_steps )
{
	pathfind::plain_route route;

	// Add same location multiple times (shouldn't normally happen but test behavior)
	route.steps.push_back(map_location(5, 5));
	route.steps.push_back(map_location(5, 5));
	route.steps.push_back(map_location(5, 5));
	route.move_cost = 3;

	BOOST_CHECK_EQUAL(route.steps.size(), 3);

	// All steps should be at same location
	for (const auto& step : route.steps) {
		BOOST_CHECK(step == map_location(5, 5));
	}
}

// Test marked_route mark with extreme values
BOOST_AUTO_TEST_CASE( test_mark_extreme_values )
{
	// Maximum turns
	pathfind::marked_route::mark max_turns(INT_MAX, false, false, false);
	BOOST_CHECK_EQUAL(max_turns.turns, INT_MAX);

	// Negative turns (edge case, shouldn't happen but test)
	pathfind::marked_route::mark neg_turns(-1, false, false, false);
	BOOST_CHECK_EQUAL(neg_turns.turns, -1);

	// All flags true
	pathfind::marked_route::mark all_true(10, true, true, true);
	BOOST_CHECK_EQUAL(all_true.turns, 10);
	BOOST_CHECK_EQUAL(all_true.zoc, true);
	BOOST_CHECK_EQUAL(all_true.capture, true);
	BOOST_CHECK_EQUAL(all_true.invisible, true);
}

// Test paths::dest_vect with extreme coordinates
BOOST_AUTO_TEST_CASE( test_paths_dest_vect_extreme_coords )
{
	pathfind::paths::dest_vect destinations;

	// Insert locations with extreme coordinates
	destinations.insert(map_location(INT_MAX, INT_MAX));
	destinations.insert(map_location(INT_MIN, INT_MIN));
	destinations.insert(map_location(INT_MAX, INT_MIN));
	destinations.insert(map_location(INT_MIN, INT_MAX));

	BOOST_CHECK_EQUAL(destinations.size(), 4);

	// Test contains
	BOOST_CHECK(destinations.contains(map_location(INT_MAX, INT_MAX)));
	BOOST_CHECK(destinations.contains(map_location(INT_MIN, INT_MIN)));

	// Test find
	auto it = destinations.find(map_location(INT_MAX, INT_MIN));
	BOOST_CHECK(it != destinations.end());
}

// Test cost_calculator inheritance behavior
BOOST_AUTO_TEST_CASE( test_cost_calculator_inheritance )
{
	// Test that we can create various cost calculators
	class expensive_terrain_calc : public pathfind::cost_calculator {
	public:
		virtual double cost(const map_location& loc, const double so_far) const override {
			// Expensive terrain based on distance from origin
			return 1.0 + distance_between(map_location::ZERO(), loc);
		}
	};

	expensive_terrain_calc calc;

	BOOST_CHECK_EQUAL(calc.cost(map_location(0, 0), 0.0), 1.0);
	BOOST_CHECK_EQUAL(calc.cost(map_location(10, 10), 0.0), 1.0 + distance_between(map_location::ZERO(), map_location(10, 10)));

	// Verify no-path value is still accessible
	BOOST_CHECK_EQUAL(calc.getNoPathValue(), 42424242.0);
}

// Test map_location vector operations consistency
BOOST_AUTO_TEST_CASE( test_vector_operations_consistency )
{
	map_location a(5, 5);
	map_location b(3, 3);

	map_location neg_a = a.vector_negation();
	map_location neg_b = b.vector_negation();

	// Vector sum of a and its negation should be (approximately) zero
	map_location sum_a = a.vector_sum(neg_a);
	BOOST_CHECK_EQUAL(sum_a.x, 0);
	BOOST_CHECK_EQUAL(sum_a.y, 0);

	map_location sum_b = b.vector_sum(neg_b);
	BOOST_CHECK_EQUAL(sum_b.x, 0);
	BOOST_CHECK_EQUAL(sum_b.y, 0);
}

// Test distance_between with same x or same y
BOOST_AUTO_TEST_CASE( test_distance_same_axis )
{
	// Same x, different y
	map_location same_x1(5, 0);
	map_location same_x2(5, 10);

	int dist_x = distance_between(same_x1, same_x2);
	BOOST_CHECK(dist_x > 0);
	BOOST_CHECK(dist_x <= 10);  // Should be approximately the y difference

	// Same y, different x
	map_location same_y1(0, 5);
	map_location same_y2(10, 5);

	int dist_y = distance_between(same_y1, same_y2);
	BOOST_CHECK(dist_y > 0);
	BOOST_CHECK(dist_y <= 10);  // Should be approximately the x difference
}

// Test get_direction with all directions and multiple steps
BOOST_AUTO_TEST_CASE( test_get_direction_all_directions )
{
	map_location center(20, 20);

	using direction = map_location::direction;
	std::vector<direction> all_dirs = {
		direction::north,
		direction::north_east,
		direction::south_east,
		direction::south,
		direction::south_west,
		direction::north_west
	};

	for (const auto& dir : all_dirs) {
		// Single step
		map_location one_step = center.get_direction(dir);
		BOOST_CHECK(one_step.valid());
		BOOST_CHECK_EQUAL(distance_between(center, one_step), 1);

		// Multiple steps
		map_location three_steps = center.get_direction(dir, 3);
		BOOST_CHECK(three_steps.valid());

		// Zero steps
		map_location zero_steps = center.get_direction(dir, 0);
		BOOST_CHECK(zero_steps == center);
	}
}

// Test rotate_right_around_center full rotation
BOOST_AUTO_TEST_CASE( test_rotate_full_circle )
{
	map_location center(50, 50);
	map_location point(50, 40);  // 10 units north of center

	// Rotate 6 times (full circle in hex)
	map_location current = point;
	for (int i = 0; i < 6; ++i) {
		current = current.rotate_right_around_center(center, 1);
	}

	// Should be back at starting point
	BOOST_CHECK_EQUAL(current.x, point.x);
	BOOST_CHECK_EQUAL(current.y, point.y);
}

// Test get_ring with various radii
BOOST_AUTO_TEST_CASE( test_get_ring_radii )
{
	map_location center(100, 100);

	// Ring 0
	auto ring0 = center.get_ring(0, 0);
	BOOST_CHECK_EQUAL(ring0.size(), 1);
	BOOST_CHECK(ring0[0] == center);

	// Ring 1
	auto ring1 = center.get_ring(1, 1);
	BOOST_CHECK(!ring1.empty());
	for (const auto& loc : ring1) {
		BOOST_CHECK_EQUAL(distance_between(center, loc), 1);
	}

	// Ring 2-5 range
	auto ring2_5 = center.get_ring(2, 5);
	BOOST_CHECK(!ring2_5.empty());
	for (const auto& loc : ring2_5) {
		int dist = distance_between(center, loc);
		BOOST_CHECK(dist >= 2);
		BOOST_CHECK(dist <= 5);
	}
}

// Test cubic conversion round-trip with edge cases
BOOST_AUTO_TEST_CASE( test_cubic_roundtrip_edge_cases )
{
	std::vector<map_location> edge_cases = {
		map_location(0, 0),
		map_location(1, 0),
		map_location(0, 1),
		map_location(-1, 0),
		map_location(0, -1),
		map_location(1000, 1000),
		map_location(-1000, -1000)
	};

	for (const auto& loc : edge_cases) {
		cubic_location cubic = loc.to_cubic();
		map_location back = map_location::from_cubic(cubic);

		BOOST_CHECK_EQUAL(loc.x, back.x);
		BOOST_CHECK_EQUAL(loc.y, back.y);
	}
}

BOOST_AUTO_TEST_SUITE_END()
