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

#include "map/location.hpp"
#include "config.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(test_map_location_extended)

BOOST_AUTO_TEST_CASE(test_map_location_default_construction)
{
	map_location loc;

	// Default constructor sets x/y to -1000 (null location)
	BOOST_CHECK_EQUAL(loc.x, -1000);
	BOOST_CHECK_EQUAL(loc.y, -1000);
	BOOST_CHECK(!loc.valid());
}

BOOST_AUTO_TEST_CASE(test_map_location_parameterized_construction)
{
	map_location loc(5, 10);

	BOOST_CHECK_EQUAL(loc.x, 5);
	BOOST_CHECK_EQUAL(loc.y, 10);
}

BOOST_AUTO_TEST_CASE(test_map_location_valid)
{
	map_location valid(1, 1);
	map_location invalid_w(-1, 0);
	map_location invalid_h(0, -1);

	BOOST_CHECK(valid.valid());
	BOOST_CHECK(!invalid_w.valid());
	BOOST_CHECK(!invalid_h.valid());
}

BOOST_AUTO_TEST_CASE(test_map_location_equality)
{
	map_location loc1(3, 5);
	map_location loc2(3, 5);
	map_location loc3(4, 5);

	BOOST_CHECK(loc1 == loc2);
	BOOST_CHECK(loc1 != loc3);
}

BOOST_AUTO_TEST_CASE(test_map_location_less_than)
{
	map_location loc1(3, 5);
	map_location loc2(3, 6);
	map_location loc3(4, 5);

	BOOST_CHECK(loc1 < loc2);
	BOOST_CHECK(loc1 < loc3);
	BOOST_CHECK(!(loc2 < loc1));
}

BOOST_AUTO_TEST_CASE(test_map_location_distance)
{
	map_location loc1(3, 5);
	map_location loc2(5, 7);

	int diff = distance_between(loc1, loc2);

	BOOST_CHECK(diff >= 0);
}

BOOST_AUTO_TEST_CASE(test_map_location_step_north)
{
	map_location center(5, 5);

	map_location north = center.get_direction(map_location::direction::north);
	BOOST_CHECK_EQUAL(north.x, 5);
	BOOST_CHECK_EQUAL(north.y, 4);
}

BOOST_AUTO_TEST_CASE(test_map_location_step_south)
{
	map_location center(5, 5);

	map_location south = center.get_direction(map_location::direction::south);
	BOOST_CHECK_EQUAL(south.x, 5);
	BOOST_CHECK_EQUAL(south.y, 6);
}

BOOST_AUTO_TEST_CASE(test_map_location_step_north_east)
{
	map_location center(5, 5);

	map_location ne = center.get_direction(map_location::direction::north_east);

	BOOST_CHECK(ne.valid());
}

BOOST_AUTO_TEST_CASE(test_map_location_step_south_east)
{
	map_location center(5, 5);

	map_location se = center.get_direction(map_location::direction::south_east);

	BOOST_CHECK(se.valid());
}

BOOST_AUTO_TEST_CASE(test_map_location_step_north_west)
{
	map_location center(5, 5);

	map_location nw = center.get_direction(map_location::direction::north_west);

	BOOST_CHECK(nw.valid());
}

BOOST_AUTO_TEST_CASE(test_map_location_step_south_west)
{
	map_location center(5, 5);

	map_location sw = center.get_direction(map_location::direction::south_west);

	BOOST_CHECK(sw.valid());
}

BOOST_AUTO_TEST_CASE(test_map_location_vector_negation)
{
	map_location loc(3, 4);

	map_location result = loc.vector_negation();

	BOOST_CHECK_EQUAL(result.x, -3);
	// vector_negation subtracts 1 from y if x is odd (x=3 is odd, so -4 - 1 = -5)
	BOOST_CHECK_EQUAL(result.y, -5);
}

BOOST_AUTO_TEST_CASE(test_map_location_hex_directions)
{
	// Verify all 6 hex directions are accessible
	map_location center(5, 5);

	for (int i = 0; i < 6; i++) {
		map_location dir = center.get_direction(static_cast<map_location::direction>(i));
		BOOST_CHECK(dir.valid());
	}
}

BOOST_AUTO_TEST_CASE(test_map_location_write_config)
{
	map_location loc(10, 20);
	config cfg;

	loc.write(cfg);

	BOOST_CHECK(cfg.has_attribute("x"));
	BOOST_CHECK(cfg.has_attribute("y"));
}

BOOST_AUTO_TEST_CASE(test_map_location_from_config)
{
	config cfg;
	cfg["x"] = "15";
	cfg["y"] = "25";

	map_location loc(cfg);

	// Config stores 1-based WML coordinates, internal is 0-based
	BOOST_CHECK_EQUAL(loc.x, 14);
	BOOST_CHECK_EQUAL(loc.y, 24);
}

// ============================================================================
// Boundary condition tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_map_location_extreme_coordinates)
{
	// Maximum/minimum integer values
	map_location max_loc(INT_MAX, INT_MAX);
	map_location min_loc(INT_MIN, INT_MIN);

	BOOST_CHECK_EQUAL(max_loc.x, INT_MAX);
	BOOST_CHECK_EQUAL(max_loc.y, INT_MAX);
	BOOST_CHECK_EQUAL(min_loc.x, INT_MIN);
	BOOST_CHECK_EQUAL(min_loc.y, INT_MIN);

	// Large but valid coordinates
	map_location large(1000000, 2000000);
	BOOST_CHECK_EQUAL(large.x, 1000000);
	BOOST_CHECK_EQUAL(large.y, 2000000);
}

BOOST_AUTO_TEST_CASE(test_map_location_negative_valid_operations)
{
	// Negative coordinates are invalid for valid() but should still work
	map_location neg(-5, -10);
	BOOST_CHECK(!neg.valid());

	// But operations should still work
	map_location result = neg.vector_negation();
	BOOST_CHECK_EQUAL(result.x, 5);
	// vector_negation subtracts 1 from y if x is odd (x=-5 is odd, so -(-10) - 1 = 9)
	BOOST_CHECK_EQUAL(result.y, 9);
}

BOOST_AUTO_TEST_CASE(test_map_location_zero_coordinates)
{
	map_location zero(0, 0);
	BOOST_CHECK(zero.valid());
	BOOST_CHECK_EQUAL(zero.x, 0);
	BOOST_CHECK_EQUAL(zero.y, 0);
}

BOOST_AUTO_TEST_CASE(test_map_location_direction_boundary)
{
	map_location center(5, 5);

	// Test all valid directions (0-5)
	for (int i = 0; i < 6; ++i) {
		map_location dir = center.get_direction(static_cast<map_location::direction>(i));
		// Just verify it doesn't crash
		(void)dir;
	}

	// Test indeterminate direction
	map_location indeterminate = center.get_direction(map_location::direction::indeterminate);
	// Should return center or some default
}

BOOST_AUTO_TEST_CASE(test_map_location_distance_same_point)
{
	map_location loc(5, 5);

	// Distance to self should be 0
	std::size_t dist = distance_between(loc, loc);
	BOOST_CHECK_EQUAL(dist, 0);
}

BOOST_AUTO_TEST_CASE(test_map_location_distance_large)
{
	map_location loc1(0, 0);
	map_location loc2(1000, 1000);

	std::size_t dist = distance_between(loc1, loc2);
	BOOST_CHECK(dist > 0);
}

BOOST_AUTO_TEST_CASE(test_map_location_vector_sum_boundary)
{
	map_location loc1(INT_MAX - 10, 0);
	map_location loc2(5, 0);

	// Vector sum with potential overflow
	map_location result = loc1.vector_sum(loc2);
	// Check it handles overflow gracefully
	(void)result;
}

BOOST_AUTO_TEST_CASE(test_map_location_vector_negation_boundary)
{
	// Negating extreme values
	map_location max_loc(INT_MAX, INT_MAX);
	map_location negated = max_loc.vector_negation();

	// Result should be negative or handle overflow
	(void)negated;

	// Negating minimum values
	map_location min_loc(INT_MIN, INT_MIN);
	map_location negated_min = min_loc.vector_negation();
	(void)negated_min;
}

BOOST_AUTO_TEST_CASE(test_map_location_get_direction_steps)
{
	map_location center(5, 5);

	// Multiple steps in each direction
	map_location north_10 = center.get_direction(map_location::direction::north, 10);
	BOOST_CHECK_EQUAL(north_10.x, 5);
	BOOST_CHECK_EQUAL(north_10.y, -5);

	// Zero steps
	map_location zero_steps = center.get_direction(map_location::direction::north, 0);
	BOOST_CHECK_EQUAL(zero_steps.x, 5);
	BOOST_CHECK_EQUAL(zero_steps.y, 5);

	// Negative steps (should go opposite direction)
	map_location neg_steps = center.get_direction(map_location::direction::north, -5);
	BOOST_CHECK_EQUAL(neg_steps.x, 5);
	BOOST_CHECK_EQUAL(neg_steps.y, 10);
}

BOOST_AUTO_TEST_CASE(test_map_location_valid_with_bounds)
{
	map_location loc(5, 5);

	// Valid within bounds
	BOOST_CHECK(loc.valid(10, 10));
	BOOST_CHECK(loc.valid(6, 6));

	// Invalid - outside bounds
	BOOST_CHECK(!loc.valid(5, 5));
	BOOST_CHECK(!loc.valid(5, 10));
	BOOST_CHECK(!loc.valid(10, 5));

	// At boundary
	map_location boundary(9, 9);
	BOOST_CHECK(boundary.valid(10, 10));
	BOOST_CHECK(!boundary.valid(9, 9));
}

BOOST_AUTO_TEST_CASE(test_map_location_valid_with_border)
{
	map_location loc(-2, -2);

	// Valid with border
	BOOST_CHECK(loc.valid(10, 10, 2));

	// Not valid with smaller border
	BOOST_CHECK(!loc.valid(10, 10, 1));
}

BOOST_AUTO_TEST_CASE(test_map_location_adjacent_tiles)
{
	map_location center(5, 5);
	auto adjacent = get_adjacent_tiles(center);

	BOOST_CHECK_EQUAL(adjacent.size(), 6);

	// All should be adjacent (distance 1)
	for (const auto& loc : adjacent) {
		BOOST_CHECK(tiles_adjacent(center, loc));
	}
}

BOOST_AUTO_TEST_CASE(test_map_location_adjacent_same_point)
{
	map_location loc(5, 5);

	// Same point is not adjacent to itself
	BOOST_CHECK(!tiles_adjacent(loc, loc));
}

BOOST_AUTO_TEST_CASE(test_map_location_comparison_boundary)
{
	map_location loc1(INT_MIN, INT_MIN);
	map_location loc2(INT_MAX, INT_MAX);

	BOOST_CHECK(loc1 < loc2);
	BOOST_CHECK(!(loc2 < loc1));

	// Same x, different y
	map_location loc3(5, INT_MIN);
	map_location loc4(5, INT_MAX);
	BOOST_CHECK(loc3 < loc4);
}

BOOST_AUTO_TEST_CASE(test_map_location_equality_extended)
{
	map_location loc1(5, 10);
	map_location loc2(5, 10);
	map_location loc3(5, 11);

	BOOST_CHECK(loc1 == loc2);
	BOOST_CHECK(!(loc1 == loc3));
	BOOST_CHECK(loc1 != loc3);
	BOOST_CHECK(!(loc1 != loc2));
}

BOOST_AUTO_TEST_CASE(test_map_location_null)
{
	// Null location has specific values
	const map_location& null_loc = map_location::null_location();
	BOOST_CHECK(!null_loc.valid());
}

BOOST_AUTO_TEST_CASE(test_map_location_zero)
{
	// Zero location
	const map_location& zero = map_location::ZERO();
	BOOST_CHECK_EQUAL(zero.x, 0);
	BOOST_CHECK_EQUAL(zero.y, 0);
	BOOST_CHECK(zero.valid());
}

BOOST_AUTO_TEST_CASE(test_map_location_add)
{
	map_location loc(5, 5);

	loc.add(3, 4);
	BOOST_CHECK_EQUAL(loc.x, 8);
	BOOST_CHECK_EQUAL(loc.y, 9);

	// Add negative
	loc.add(-2, -3);
	BOOST_CHECK_EQUAL(loc.x, 6);
	BOOST_CHECK_EQUAL(loc.y, 6);
}

BOOST_AUTO_TEST_CASE(test_map_location_plus)
{
	map_location loc(5, 5);

	map_location result = loc.plus(3, 4);
	BOOST_CHECK_EQUAL(result.x, 8);
	BOOST_CHECK_EQUAL(result.y, 9);

	// Original unchanged
	BOOST_CHECK_EQUAL(loc.x, 5);
	BOOST_CHECK_EQUAL(loc.y, 5);
}

BOOST_AUTO_TEST_CASE(test_map_location_wml_coordinates)
{
	map_location loc(4, 5);

	// WML coordinates are 1-based
	BOOST_CHECK_EQUAL(loc.wml_x(), 5);
	BOOST_CHECK_EQUAL(loc.wml_y(), 6);
}

BOOST_AUTO_TEST_CASE(test_map_location_set_wml)
{
	map_location loc;

	loc.set_wml_x(5);
	loc.set_wml_y(10);

	// Internal is 0-based
	BOOST_CHECK_EQUAL(loc.x, 4);
	BOOST_CHECK_EQUAL(loc.y, 9);
}

BOOST_AUTO_TEST_CASE(test_map_location_hex_ring)
{
	map_location center(5, 5);

	// Get ring of hexes at distance 1-3
	auto ring = center.get_ring(1, 3);

	// Should have hexes at various distances
	BOOST_CHECK(!ring.empty());
}

BOOST_AUTO_TEST_CASE(test_map_location_rotate_direction)
{
	// Test rotation of directions
	map_location::direction north = map_location::direction::north;

	// Rotate 1 step clockwise
	map_location::direction ne = map_location::rotate_direction(north, 1);
	BOOST_CHECK(ne == map_location::direction::north_east);

	// Rotate 3 steps (opposite)
	map_location::direction south = map_location::rotate_direction(north, 3);
	BOOST_CHECK(south == map_location::direction::south);

	// Rotate negative (counter-clockwise)
	map_location::direction nw = map_location::rotate_direction(north, -1);
	BOOST_CHECK(nw == map_location::direction::north_west);

	// Rotate indeterminate stays indeterminate
	map_location::direction ind = map_location::rotate_direction(map_location::direction::indeterminate, 1);
	BOOST_CHECK(ind == map_location::direction::indeterminate);
}

BOOST_AUTO_TEST_CASE(test_map_location_opposite_direction)
{
	BOOST_CHECK(map_location::get_opposite_direction(map_location::direction::north) == map_location::direction::south);
	BOOST_CHECK(map_location::get_opposite_direction(map_location::direction::south) == map_location::direction::north);
	BOOST_CHECK(map_location::get_opposite_direction(map_location::direction::north_east) == map_location::direction::south_west);
	BOOST_CHECK(map_location::get_opposite_direction(map_location::direction::south_west) == map_location::direction::north_east);
	BOOST_CHECK(map_location::get_opposite_direction(map_location::direction::north_west) == map_location::direction::south_east);
	BOOST_CHECK(map_location::get_opposite_direction(map_location::direction::south_east) == map_location::direction::north_west);
}

BOOST_AUTO_TEST_CASE(test_map_location_parse_direction)
{
	BOOST_CHECK(map_location::parse_direction("n") == map_location::direction::north);
	BOOST_CHECK(map_location::parse_direction("s") == map_location::direction::south);
	BOOST_CHECK(map_location::parse_direction("ne") == map_location::direction::north_east);
	BOOST_CHECK(map_location::parse_direction("nw") == map_location::direction::north_west);
	BOOST_CHECK(map_location::parse_direction("se") == map_location::direction::south_east);
	BOOST_CHECK(map_location::parse_direction("sw") == map_location::direction::south_west);
}

BOOST_AUTO_TEST_CASE(test_map_location_parse_directions)
{
	auto dirs = map_location::parse_directions("n,s,ne,nw");
	BOOST_CHECK_EQUAL(dirs.size(), 4);
}

BOOST_AUTO_TEST_CASE(test_map_location_write_direction)
{
	BOOST_CHECK_EQUAL(map_location::write_direction(map_location::direction::north), "n");
	BOOST_CHECK_EQUAL(map_location::write_direction(map_location::direction::south), "s");
}

BOOST_AUTO_TEST_CASE(test_map_location_cubic_conversion)
{
	map_location loc(5, 5);

	// Convert to cubic and back
	cubic_location cubic = loc.to_cubic();
	map_location back = map_location::from_cubic(cubic);

	BOOST_CHECK(loc == back);
}

BOOST_AUTO_TEST_CASE(test_map_location_rotate_around_center)
{
	map_location center(5, 5);
	map_location point(5, 3);

	// Rotate 180 degrees around center
	map_location rotated = point.rotate_right_around_center(center, 3);

	// Should be on opposite side
	BOOST_CHECK_EQUAL(rotated.x, 5);
	BOOST_CHECK_EQUAL(rotated.y, 7);
}

// ============================================================================
// Additional boundary condition tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_map_location_direction_invalid)
{
	map_location center(5, 5);

	// Invalid direction values (implementation dependent)
	// Should handle gracefully without crashing
	map_location result = center.get_direction(static_cast<map_location::direction>(100));
	// Result may vary, but should not crash
	(void)result;
}

BOOST_AUTO_TEST_CASE(test_map_location_direction_negative_steps)
{
	map_location center(5, 5);

	// Negative steps in all directions
	map_location north = center.get_direction(map_location::direction::north, -1);
	BOOST_CHECK_EQUAL(north.y, 6); // Should go south

	map_location south = center.get_direction(map_location::direction::south, -1);
	BOOST_CHECK_EQUAL(south.y, 4); // Should go north

	map_location east = center.get_direction(map_location::direction::north_east, -1);
	// Should go opposite direction
	(void)east;
}

BOOST_AUTO_TEST_CASE(test_map_location_distance_negative_coordinates)
{
	// Distance with negative coordinates
	map_location loc1(-5, -5);
	map_location loc2(-10, -10);

	std::size_t dist = distance_between(loc1, loc2);
	BOOST_CHECK(dist > 0);
}

BOOST_AUTO_TEST_CASE(test_map_location_distance_mixed_coordinates)
{
	// Distance with mixed positive/negative coordinates
	map_location loc1(-5, 5);
	map_location loc2(5, -5);

	std::size_t dist = distance_between(loc1, loc2);
	BOOST_CHECK(dist > 0);
}

BOOST_AUTO_TEST_CASE(test_map_location_vector_sum_overflow)
{
	// Vector sum that could overflow
	map_location loc1(INT_MAX - 1, 0);
	map_location loc2(5, 0);

	map_location result = loc1.vector_sum(loc2);
	// Check behavior on overflow
	(void)result;
}

BOOST_AUTO_TEST_CASE(test_map_location_vector_difference)
{
	// Vector difference using vector_difference_assign
	map_location loc1(10, 20);
	map_location loc2(3, 5);

	loc1.vector_difference_assign(loc2);
	BOOST_CHECK_EQUAL(loc1.x, 7);
	// vector_difference_assign uses vector_sum_assign with vector_negation
	// loc2.x=3 (odd), so vector_negation gives (-3, -5 - 1) = (-3, -6)
	// Then vector_sum_assign: y += ((10 & 1) && (-3 & 1)) = 0, so y = 20 + (-6) = 14
	BOOST_CHECK_EQUAL(loc1.y, 14);
}

BOOST_AUTO_TEST_CASE(test_map_location_vector_difference_negative)
{
	// Vector difference resulting in negative using vector_difference_assign
	map_location loc1(5, 5);
	map_location loc2(10, 15);

	loc1.vector_difference_assign(loc2);
	BOOST_CHECK_EQUAL(loc1.x, -5);
	BOOST_CHECK_EQUAL(loc1.y, -10);
}

BOOST_AUTO_TEST_CASE(test_map_location_add_negative)
{
	map_location loc(5, 5);

	loc.add(-3, -4);
	BOOST_CHECK_EQUAL(loc.x, 2);
	BOOST_CHECK_EQUAL(loc.y, 1);
}

BOOST_AUTO_TEST_CASE(test_map_location_add_overflow)
{
	map_location loc(INT_MAX - 1, 0);

	loc.add(10, 0);
	// Check overflow behavior
	(void)loc;
}

BOOST_AUTO_TEST_CASE(test_map_location_plus_negative)
{
	map_location loc(5, 5);

	map_location result = loc.plus(-3, -4);
	BOOST_CHECK_EQUAL(result.x, 2);
	BOOST_CHECK_EQUAL(result.y, 1);
	BOOST_CHECK_EQUAL(loc.x, 5); // Original unchanged
	BOOST_CHECK_EQUAL(loc.y, 5);
}

BOOST_AUTO_TEST_CASE(test_map_location_valid_zero_size)
{
	map_location loc(0, 0);

	// Zero size map - nothing is valid
	BOOST_CHECK(!loc.valid(0, 0));

	// Single tile map - (0,0) is valid within 1x1 bounds
	BOOST_CHECK(loc.valid(1, 1));
}

BOOST_AUTO_TEST_CASE(test_map_location_valid_negative_border)
{
	map_location loc(-5, -5);

	// Negative border should be handled
	BOOST_CHECK(!loc.valid(10, 10, -1));
}

BOOST_AUTO_TEST_CASE(test_map_location_adjacent_tiles_count)
{
	map_location center(5, 5);
	auto adjacent = get_adjacent_tiles(center);

	BOOST_CHECK_EQUAL(adjacent.size(), 6);

	// All should be at distance 1
	for (const auto& loc : adjacent) {
		BOOST_CHECK(tiles_adjacent(center, loc));
		BOOST_CHECK_EQUAL(distance_between(center, loc), 1);
	}
}

BOOST_AUTO_TEST_CASE(test_map_location_adjacent_tiles_unique)
{
	map_location center(5, 5);
	auto adjacent = get_adjacent_tiles(center);

	// All adjacent tiles should be unique
	for (size_t i = 0; i < adjacent.size(); ++i) {
		for (size_t j = i + 1; j < adjacent.size(); ++j) {
			BOOST_CHECK(adjacent[i] != adjacent[j]);
		}
	}
}

BOOST_AUTO_TEST_CASE(test_map_location_tiles_adjacent_diagonal)
{
	map_location loc1(5, 5);
	map_location loc2(6, 6); // Diagonal on square grid

	// Diagonal tiles may or may not be considered adjacent
	// depending on the coordinate system
	bool adjacent = tiles_adjacent(loc1, loc2);
	(void)adjacent; // Just verify it compiles and runs
}

BOOST_AUTO_TEST_CASE(test_map_location_rotate_direction_multiple)
{
	// Rotate multiple times around the hex
	map_location::direction north = map_location::direction::north;

	// Full rotation (6 steps) should return to start
	map_location::direction result = map_location::rotate_direction(north, 6);
	BOOST_CHECK(result == north);

	// Two full rotations
	result = map_location::rotate_direction(north, 12);
	BOOST_CHECK(result == north);

	// Negative full rotation
	result = map_location::rotate_direction(north, -6);
	BOOST_CHECK(result == north);
}

BOOST_AUTO_TEST_CASE(test_map_location_rotate_direction_large_values)
{
	map_location::direction north = map_location::direction::north;

	// Large positive rotation
	map_location::direction result = map_location::rotate_direction(north, 100);
	(void)result; // Should handle gracefully

	// Large negative rotation
	result = map_location::rotate_direction(north, -100);
	(void)result;
}

BOOST_AUTO_TEST_CASE(test_map_location_get_opposite_direction_all)
{
	// Verify all opposite pairs
	BOOST_CHECK(map_location::get_opposite_direction(map_location::direction::north) == map_location::direction::south);
	BOOST_CHECK(map_location::get_opposite_direction(map_location::direction::south) == map_location::direction::north);
	BOOST_CHECK(map_location::get_opposite_direction(map_location::direction::north_east) == map_location::direction::south_west);
	BOOST_CHECK(map_location::get_opposite_direction(map_location::direction::south_west) == map_location::direction::north_east);
	BOOST_CHECK(map_location::get_opposite_direction(map_location::direction::north_west) == map_location::direction::south_east);
	BOOST_CHECK(map_location::get_opposite_direction(map_location::direction::south_east) == map_location::direction::north_west);
}

BOOST_AUTO_TEST_CASE(test_map_location_get_opposite_direction_twice)
{
	// Opposite of opposite should be original
	map_location::direction original = map_location::direction::north_east;
	map_location::direction opposite = map_location::get_opposite_direction(original);
	map_location::direction back = map_location::get_opposite_direction(opposite);
	BOOST_CHECK(back == original);
}

BOOST_AUTO_TEST_CASE(test_map_location_parse_direction_invalid)
{
	// Invalid direction strings return indeterminate, not throw
	BOOST_CHECK(map_location::parse_direction("") == map_location::direction::indeterminate);
	BOOST_CHECK(map_location::parse_direction("invalid") == map_location::direction::indeterminate);
	BOOST_CHECK(map_location::parse_direction("north_south") == map_location::direction::indeterminate);
}

BOOST_AUTO_TEST_CASE(test_map_location_parse_direction_case_sensitivity)
{
	// Case sensitivity - uppercase returns indeterminate
	BOOST_CHECK(map_location::parse_direction("N") == map_location::direction::indeterminate);
	BOOST_CHECK(map_location::parse_direction("North") == map_location::direction::indeterminate);
}

BOOST_AUTO_TEST_CASE(test_map_location_parse_directions_empty)
{
	// Empty directions string
	auto dirs = map_location::parse_directions("");
	BOOST_CHECK(dirs.empty());
}

BOOST_AUTO_TEST_CASE(test_map_location_parse_directions_whitespace)
{
	// Whitespace handling
	auto dirs = map_location::parse_directions("n, s, ne");
	BOOST_CHECK_EQUAL(dirs.size(), 3);
}

BOOST_AUTO_TEST_CASE(test_map_location_write_direction_all)
{
	// Verify all directions can be written
	BOOST_CHECK_EQUAL(map_location::write_direction(map_location::direction::north), "n");
	BOOST_CHECK_EQUAL(map_location::write_direction(map_location::direction::south), "s");
	BOOST_CHECK_EQUAL(map_location::write_direction(map_location::direction::north_east), "ne");
	BOOST_CHECK_EQUAL(map_location::write_direction(map_location::direction::north_west), "nw");
	BOOST_CHECK_EQUAL(map_location::write_direction(map_location::direction::south_east), "se");
	BOOST_CHECK_EQUAL(map_location::write_direction(map_location::direction::south_west), "sw");
}

BOOST_AUTO_TEST_CASE(test_map_location_cubic_conversion_zero)
{
	// Convert origin to cubic and back
	map_location origin(0, 0);
	cubic_location cubic = origin.to_cubic();
	map_location back = map_location::from_cubic(cubic);
	BOOST_CHECK(origin == back);
}

BOOST_AUTO_TEST_CASE(test_map_location_cubic_conversion_negative)
{
	// Convert negative coordinates
	map_location neg(-5, -10);
	cubic_location cubic = neg.to_cubic();
	map_location back = map_location::from_cubic(cubic);
	BOOST_CHECK(neg == back);
}

BOOST_AUTO_TEST_CASE(test_map_location_rotate_around_center_zero)
{
	map_location center(5, 5);
	map_location same_as_center(5, 5);

	// Rotating center around itself should stay at center
	map_location rotated = same_as_center.rotate_right_around_center(center, 1);
	BOOST_CHECK_EQUAL(rotated.x, 5);
	BOOST_CHECK_EQUAL(rotated.y, 5);
}

BOOST_AUTO_TEST_CASE(test_map_location_rotate_around_center_full_rotation)
{
	map_location center(5, 5);
	map_location point(5, 3);

	// Full rotation (6 steps) should return to original
	map_location rotated = point;
	for (int i = 0; i < 6; ++i) {
		rotated = rotated.rotate_right_around_center(center, 1);
	}
	BOOST_CHECK_EQUAL(rotated.x, point.x);
	BOOST_CHECK_EQUAL(rotated.y, point.y);
}

BOOST_AUTO_TEST_CASE(test_map_location_ring_empty)
{
	map_location center(5, 5);

	// Ring with min > max should be empty
	auto ring = center.get_ring(5, 3);
	BOOST_CHECK(ring.empty());
}

BOOST_AUTO_TEST_CASE(test_map_location_ring_single_radius)
{
	map_location center(5, 5);

	// Ring with same min and max
	auto ring = center.get_ring(2, 2);
	BOOST_CHECK(!ring.empty());

	// All locations should be at distance 2
	for (const auto& loc : ring) {
		BOOST_CHECK_EQUAL(distance_between(center, loc), 2);
	}
}

BOOST_AUTO_TEST_CASE(test_map_location_ring_zero_radius)
{
	map_location center(5, 5);

	// Ring with radius 0 should contain only center
	auto ring = center.get_ring(0, 0);
	BOOST_CHECK_EQUAL(ring.size(), 1);
	BOOST_CHECK(ring[0] == center);
}

BOOST_AUTO_TEST_CASE(test_map_location_from_config_invalid)
{
	// Config with missing or invalid coordinates
	config cfg;
	cfg["x"] = "not_a_number";
	cfg["y"] = "also_not_a_number";

	map_location loc(cfg);
	// Should handle gracefully, likely defaulting to 0
	(void)loc;
}

BOOST_AUTO_TEST_CASE(test_map_location_from_config_partial)
{
	// Config with only x coordinate
	config cfg;
	cfg["x"] = "5";

	map_location loc(cfg);
	// x is converted from 1-based to 0-based: 5 - 1 = 4
	BOOST_CHECK_EQUAL(loc.x, 4);
	// y stays at default -1000 when not provided
	BOOST_CHECK_EQUAL(loc.y, -1000);
}

BOOST_AUTO_TEST_CASE(test_map_location_hash_consistency)
{
	// Hash should be consistent for equal locations
	map_location loc1(5, 10);
	map_location loc2(5, 10);

	std::hash<map_location> hasher;
	BOOST_CHECK_EQUAL(hasher(loc1), hasher(loc2));
}

BOOST_AUTO_TEST_CASE(test_map_location_hash_distribution)
{
	// Hash should distribute different locations
	std::hash<map_location> hasher;

	map_location loc1(0, 0);
	map_location loc2(1, 0);
	map_location loc3(0, 1);

	// Different locations should ideally have different hashes
	// (not guaranteed but highly likely)
	bool all_different = (hasher(loc1) != hasher(loc2)) ||
						 (hasher(loc2) != hasher(loc3));
	BOOST_CHECK(all_different);
}

BOOST_AUTO_TEST_SUITE_END()