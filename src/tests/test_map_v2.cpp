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
#include <climits>
#include <cmath>
#include <set>
#include <sstream>

BOOST_AUTO_TEST_SUITE(test_map_v2)

// ============================================================================
// Map Location Direction Tests - Extended
// ============================================================================

BOOST_AUTO_TEST_CASE(test_direction_all_directions_count)
{
	auto dirs = map_location::all_directions();
	BOOST_CHECK_EQUAL(dirs.size(), 6);

	// Verify all directions are present and unique
	std::set<map_location::direction> unique_dirs(dirs.begin(), dirs.end());
	BOOST_CHECK_EQUAL(unique_dirs.size(), 6);
}

BOOST_AUTO_TEST_CASE(test_direction_parse_with_operators)
{
	// Test minus operator (opposite direction)
	BOOST_CHECK(map_location::parse_direction("-n") == map_location::direction::south);
	BOOST_CHECK(map_location::parse_direction("-s") == map_location::direction::north);
	BOOST_CHECK(map_location::parse_direction("-ne") == map_location::direction::south_west);
	BOOST_CHECK(map_location::parse_direction("-nw") == map_location::direction::south_east);
	BOOST_CHECK(map_location::parse_direction("-se") == map_location::direction::north_west);
	BOOST_CHECK(map_location::parse_direction("-sw") == map_location::direction::north_east);
}

BOOST_AUTO_TEST_CASE(test_direction_parse_with_rotation)
{
	// Test clockwise rotation
	BOOST_CHECK(map_location::parse_direction("n:cw") == map_location::direction::north_east);
	BOOST_CHECK(map_location::parse_direction("ne:cw") == map_location::direction::south_east);
	BOOST_CHECK(map_location::parse_direction("s:cw") == map_location::direction::south_west);

	// Test counter-clockwise rotation
	BOOST_CHECK(map_location::parse_direction("n:ccw") == map_location::direction::north_west);
	BOOST_CHECK(map_location::parse_direction("se:ccw") == map_location::direction::north_east);
	BOOST_CHECK(map_location::parse_direction("s:ccw") == map_location::direction::south_east);
}

BOOST_AUTO_TEST_CASE(test_direction_parse_with_parentheses)
{
	// Test with parentheses for grouping
	map_location::direction result = map_location::parse_direction("((n))");
	BOOST_CHECK(result == map_location::direction::north);

	// Combined operators
	result = map_location::parse_direction("(-(n))");
	BOOST_CHECK(result == map_location::direction::south);
}

BOOST_AUTO_TEST_CASE(test_direction_write_translated)
{
	// Test translated direction strings (just verify they return non-empty strings)
	std::string north = map_location::write_translated_direction(map_location::direction::north);
	std::string south = map_location::write_translated_direction(map_location::direction::south);

	BOOST_CHECK(!north.empty());
	BOOST_CHECK(!south.empty());
	BOOST_CHECK(north != south); // Different directions should have different translations
}

// ============================================================================
// Map Location Vector Operations - Extended
// ============================================================================

BOOST_AUTO_TEST_CASE(test_vector_sum_even_x_coordinates)
{
	// Test vector sum with even x coordinates (no adjustment needed)
	map_location loc1(4, 5); // even x
	map_location loc2(2, 3); // even x

	map_location result = loc1.vector_sum(loc2);
	BOOST_CHECK_EQUAL(result.x, 6);
	BOOST_CHECK_EQUAL(result.y, 8);
}

BOOST_AUTO_TEST_CASE(test_vector_sum_odd_x_coordinates)
{
	// Test vector sum where both x are odd (y adjustment needed)
	map_location loc1(3, 5); // odd x
	map_location loc2(5, 3); // odd x

	map_location result = loc1.vector_sum(loc2);
	BOOST_CHECK_EQUAL(result.x, 8);
	// When both x are odd, y += 1
	BOOST_CHECK_EQUAL(result.y, 9);
}

BOOST_AUTO_TEST_CASE(test_vector_sum_mixed_x_coordinates)
{
	// Test vector sum with mixed even/odd x coordinates
	map_location loc1(3, 5); // odd x
	map_location loc2(4, 3); // even x

	map_location result = loc1.vector_sum(loc2);
	BOOST_CHECK_EQUAL(result.x, 7);
	// When only one x is odd, no adjustment
	BOOST_CHECK_EQUAL(result.y, 8);
}

BOOST_AUTO_TEST_CASE(test_vector_negation_even_x)
{
	// Test negation with even x
	map_location loc(4, 5); // even x

	map_location result = loc.vector_negation();
	BOOST_CHECK_EQUAL(result.x, -4);
	// No y adjustment when x is even
	BOOST_CHECK_EQUAL(result.y, -5);
}

BOOST_AUTO_TEST_CASE(test_vector_negation_identity)
{
	// Double negation should return original (with slight adjustment for odd x)
	map_location loc(5, 10);
	map_location negated = loc.vector_negation();
	map_location back = negated.vector_negation();

	BOOST_CHECK_EQUAL(back.x, loc.x);
	BOOST_CHECK_EQUAL(back.y, loc.y);
}

// ============================================================================
// Map Location Distance - Extended
// ============================================================================

BOOST_AUTO_TEST_CASE(test_distance_between_horizontal)
{
	// Test horizontal distance (same y)
	map_location loc1(0, 0);
	map_location loc2(5, 0);

	std::size_t dist = distance_between(loc1, loc2);
	BOOST_CHECK_EQUAL(dist, 5);
}

BOOST_AUTO_TEST_CASE(test_distance_between_vertical)
{
	// Test vertical distance (same x, even x)
	map_location loc1(4, 0);
	map_location loc2(4, 5);

	std::size_t dist = distance_between(loc1, loc2);
	BOOST_CHECK_EQUAL(dist, 5);
}

BOOST_AUTO_TEST_CASE(test_distance_between_diagonal)
{
	// Test diagonal distance
	map_location loc1(0, 0);
	map_location loc2(3, 3);

	std::size_t dist = distance_between(loc1, loc2);
	// Diagonal distance in hex grid is not euclidean
	BOOST_CHECK(dist > 0);
	BOOST_CHECK(dist <= 6); // Should be approximately 4-6
}

BOOST_AUTO_TEST_CASE(test_distance_between_adjacent_hexes)
{
	// Verify distance is 1 for all 6 adjacent hexes
	map_location center(5, 5);
	auto adjacent = get_adjacent_tiles(center);

	for (const auto& adj : adjacent) {
		BOOST_CHECK_EQUAL(distance_between(center, adj), 1);
		BOOST_CHECK_EQUAL(distance_between(adj, center), 1); // Symmetric
	}
}

// ============================================================================
// Map Location Adjacent Tiles - Extended
// ============================================================================

BOOST_AUTO_TEST_CASE(test_adjacent_tiles_even_x_row)
{
	// Test adjacent tiles for even x position
	map_location center(4, 5); // even x
	auto adjacent = get_adjacent_tiles(center);

	// North should be at (4, 4)
	map_location north = center.get_direction(map_location::direction::north);
	BOOST_CHECK_EQUAL(north.x, 4);
	BOOST_CHECK_EQUAL(north.y, 4);

	// South should be at (4, 6)
	map_location south = center.get_direction(map_location::direction::south);
	BOOST_CHECK_EQUAL(south.x, 4);
	BOOST_CHECK_EQUAL(south.y, 6);

	// Verify all 6 tiles are at distance 1
	for (const auto& adj : adjacent) {
		BOOST_CHECK_EQUAL(distance_between(center, adj), 1);
	}
}

BOOST_AUTO_TEST_CASE(test_adjacent_tiles_odd_x_row)
{
	// Test adjacent tiles for odd x position
	map_location center(5, 5); // odd x
	auto adjacent = get_adjacent_tiles(center);

	// North should be at (5, 4)
	map_location north = center.get_direction(map_location::direction::north);
	BOOST_CHECK_EQUAL(north.x, 5);
	BOOST_CHECK_EQUAL(north.y, 4);

	// South should be at (5, 6)
	map_location south = center.get_direction(map_location::direction::south);
	BOOST_CHECK_EQUAL(south.x, 5);
	BOOST_CHECK_EQUAL(south.y, 6);

	// Verify all 6 tiles are at distance 1
	for (const auto& adj : adjacent) {
		BOOST_CHECK_EQUAL(distance_between(center, adj), 1);
	}
}

BOOST_AUTO_TEST_CASE(test_tiles_adjacent_various)
{
	map_location center(5, 5);

	// Directly north and south are adjacent
	BOOST_CHECK(tiles_adjacent(center, map_location(5, 4)));
	BOOST_CHECK(tiles_adjacent(center, map_location(5, 6)));

	// Same tile is not adjacent
	BOOST_CHECK(!tiles_adjacent(center, center));

	// Far away is not adjacent
	BOOST_CHECK(!tiles_adjacent(center, map_location(10, 10)));
	BOOST_CHECK(!tiles_adjacent(center, map_location(0, 0)));
}

// ============================================================================
// Map Location Ring Operations
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ring_radius_one)
{
	map_location center(5, 5);
	auto ring = center.get_ring(1, 1);

	// Ring at radius 1 should have 6 hexes
	BOOST_CHECK_EQUAL(ring.size(), 6);

	// All should be at distance 1
	for (const auto& loc : ring) {
		BOOST_CHECK_EQUAL(distance_between(center, loc), 1);
	}
}

BOOST_AUTO_TEST_CASE(test_ring_radius_two)
{
	map_location center(5, 5);
	auto ring = center.get_ring(2, 2);

	// Ring at radius 2 should have 12 hexes
	BOOST_CHECK_EQUAL(ring.size(), 12);

	// All should be at distance 2
	for (const auto& loc : ring) {
		BOOST_CHECK_EQUAL(distance_between(center, loc), 2);
	}
}

BOOST_AUTO_TEST_CASE(test_ring_combined_range)
{
	map_location center(5, 5);
	auto ring = center.get_ring(1, 2);

	// Combined ring at radius 1-2 should have 6 + 12 = 18 hexes
	BOOST_CHECK_EQUAL(ring.size(), 18);

	// All should be at distance 1 or 2
	for (const auto& loc : ring) {
		std::size_t dist = distance_between(center, loc);
		BOOST_CHECK(dist >= 1 && dist <= 2);
	}
}

BOOST_AUTO_TEST_CASE(test_ring_large_radius)
{
	map_location center(5, 5);
	auto ring = center.get_ring(3, 3);

	// Ring at radius 3 should have 18 hexes
	BOOST_CHECK_EQUAL(ring.size(), 18);

	// All should be at distance 3
	for (const auto& loc : ring) {
		BOOST_CHECK_EQUAL(distance_between(center, loc), 3);
	}
}

// ============================================================================
// Map Location Rotate Around Center - Extended
// ============================================================================

BOOST_AUTO_TEST_CASE(test_rotate_around_center_identity)
{
	map_location center(10, 10);
	map_location point(10, 8);

	// Zero rotation should stay same
	map_location rotated = point.rotate_right_around_center(center, 0);
	BOOST_CHECK_EQUAL(rotated.x, point.x);
	BOOST_CHECK_EQUAL(rotated.y, point.y);
}

BOOST_AUTO_TEST_CASE(test_rotate_around_center_180_degrees)
{
	map_location center(10, 10);
	map_location point(10, 8);

	// 180 degree rotation (3 steps)
	map_location rotated = point.rotate_right_around_center(center, 3);
	BOOST_CHECK_EQUAL(rotated.x, 10);
	BOOST_CHECK_EQUAL(rotated.y, 12);
}

BOOST_AUTO_TEST_CASE(test_rotate_preserves_distance)
{
	map_location center(5, 5);
	map_location point(7, 6);

	std::size_t original_dist = distance_between(center, point);

	// All rotations should preserve distance
	for (int i = 0; i < 6; ++i) {
		map_location rotated = point.rotate_right_around_center(center, i);
		BOOST_CHECK_EQUAL(distance_between(center, rotated), original_dist);
	}
}

// ============================================================================
// Map Location Valid Bounds - Extended
// ============================================================================

BOOST_AUTO_TEST_CASE(test_valid_at_origin)
{
	map_location origin(0, 0);

	BOOST_CHECK(origin.valid());
	BOOST_CHECK(origin.valid(1, 1));
	BOOST_CHECK(origin.valid(10, 10));
	BOOST_CHECK(!origin.valid(0, 0));
}

BOOST_AUTO_TEST_CASE(test_valid_at_boundary)
{
	map_location boundary(9, 9);

	BOOST_CHECK(boundary.valid());
	BOOST_CHECK(boundary.valid(10, 10));
	BOOST_CHECK(!boundary.valid(9, 9));
	BOOST_CHECK(!boundary.valid(5, 5));
}

BOOST_AUTO_TEST_CASE(test_valid_with_large_border)
{
	map_location loc(-5, -5);

	// With large border should be valid
	BOOST_CHECK(loc.valid(10, 10, 5));

	// With small border should be invalid
	BOOST_CHECK(!loc.valid(10, 10, 2));
	BOOST_CHECK(!loc.valid(10, 10, 0));
}

// ============================================================================
// Map Location WML Coordinate Conversion - Extended
// ============================================================================

BOOST_AUTO_TEST_CASE(test_wml_coordinates_boundary)
{
	// Test at origin
	map_location origin(0, 0);
	BOOST_CHECK_EQUAL(origin.wml_x(), 1);
	BOOST_CHECK_EQUAL(origin.wml_y(), 1);

	// Test at (1, 1)
	map_location loc(1, 1);
	BOOST_CHECK_EQUAL(loc.wml_x(), 2);
	BOOST_CHECK_EQUAL(loc.wml_y(), 2);
}

BOOST_AUTO_TEST_CASE(test_set_wml_coordinates)
{
	map_location loc;

	loc.set_wml_x(1);
	loc.set_wml_y(1);
	BOOST_CHECK_EQUAL(loc.x, 0);
	BOOST_CHECK_EQUAL(loc.y, 0);

	loc.set_wml_x(10);
	loc.set_wml_y(20);
	BOOST_CHECK_EQUAL(loc.x, 9);
	BOOST_CHECK_EQUAL(loc.y, 19);
}

// ============================================================================
// Map Location Config Read/Write - Extended
// ============================================================================

BOOST_AUTO_TEST_CASE(test_write_locations_multiple)
{
	std::vector<map_location> locs;
	locs.emplace_back(0, 0);
	locs.emplace_back(5, 5);
	locs.emplace_back(10, 10);

	config cfg;
	write_locations(locs, cfg);

	BOOST_CHECK(cfg.has_attribute("x"));
	BOOST_CHECK(cfg.has_attribute("y"));

	// Should have comma-separated values
	std::string x_val = cfg["x"];
	std::string y_val = cfg["y"];
	BOOST_CHECK(x_val.find(',') != std::string::npos);
	BOOST_CHECK(y_val.find(',') != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_read_locations_from_config)
{
	config cfg;
	cfg["x"] = "1,6,11";
	cfg["y"] = "1,6,11";

	std::vector<map_location> locs = read_locations(cfg);

	BOOST_CHECK_EQUAL(locs.size(), 3);

	// WML coordinates are 1-based, internal are 0-based
	BOOST_CHECK_EQUAL(locs[0].x, 0);
	BOOST_CHECK_EQUAL(locs[0].y, 0);
	BOOST_CHECK_EQUAL(locs[1].x, 5);
	BOOST_CHECK_EQUAL(locs[1].y, 5);
	BOOST_CHECK_EQUAL(locs[2].x, 10);
	BOOST_CHECK_EQUAL(locs[2].y, 10);
}

BOOST_AUTO_TEST_CASE(test_write_location_range)
{
	std::set<map_location> locs;
	locs.insert(map_location(0, 0));
	locs.insert(map_location(0, 1));
	locs.insert(map_location(0, 2));
	locs.insert(map_location(1, 0));

	config cfg;
	write_location_range(locs, cfg);

	BOOST_CHECK(cfg.has_attribute("x"));
	BOOST_CHECK(cfg.has_attribute("y"));
}

// ============================================================================
// Map Location Matches Range
// ============================================================================

BOOST_AUTO_TEST_CASE(test_matches_range_single_point)
{
	map_location loc(5, 5);

	// Should match its own coordinates
	BOOST_CHECK(loc.matches_range("6", "6")); // WML coords: 5+1=6

	// Should not match different coordinates
	BOOST_CHECK(!loc.matches_range("1", "1"));
}

BOOST_AUTO_TEST_CASE(test_matches_range_range)
{
	map_location loc(5, 5);

	// Should match range containing the point
	BOOST_CHECK(loc.matches_range("1-10", "1-10"));
	BOOST_CHECK(loc.matches_range("6", "6")); // WML coords

	// Should not match range not containing the point
	BOOST_CHECK(!loc.matches_range("1-5", "1-5"));
}

BOOST_AUTO_TEST_CASE(test_matches_range_empty)
{
	map_location loc(5, 5);

	// Empty ranges should match all
	BOOST_CHECK(loc.matches_range("", ""));
}

BOOST_AUTO_TEST_CASE(test_matches_range_x_only)
{
	map_location loc(5, 5);

	// Only x specified - should match if x matches
	BOOST_CHECK(loc.matches_range("6", "")); // WML x = 6
}

BOOST_AUTO_TEST_CASE(test_matches_range_y_only)
{
	map_location loc(5, 5);

	// Only y specified - should match if y matches
	BOOST_CHECK(loc.matches_range("", "6")); // WML y = 6
}

// ============================================================================
// Map Location Relative Direction - Extended
// ============================================================================

BOOST_AUTO_TEST_CASE(test_get_relative_dir_same_location)
{
	map_location loc(5, 5);

	// Same location should return indeterminate
	map_location::direction dir = loc.get_relative_dir(loc);
	BOOST_CHECK(dir == map_location::direction::indeterminate);
}

BOOST_AUTO_TEST_CASE(test_get_relative_dir_cardinal)
{
	map_location center(5, 5);

	// Directly north
	map_location north(5, 3);
	BOOST_CHECK(center.get_relative_dir(north) == map_location::direction::north);

	// Directly south
	map_location south(5, 7);
	BOOST_CHECK(center.get_relative_dir(south) == map_location::direction::south);
}

BOOST_AUTO_TEST_CASE(test_get_relative_dir_default_mode)
{
	map_location center(5, 5);

	// Test DEFAULT mode
	map_location north(5, 3);
	map_location::direction dir = center.get_relative_dir(north, map_location::DEFAULT);
	// Should be generally northward
	BOOST_CHECK(dir == map_location::direction::north ||
	            dir == map_location::direction::north_east ||
	            dir == map_location::direction::north_west);
}

// ============================================================================
// Map Location Cubic Conversion - Extended
// ============================================================================

BOOST_AUTO_TEST_CASE(test_cubic_conversion_positive)
{
	map_location loc(10, 10);

	cubic_location cubic = loc.to_cubic();
	map_location back = map_location::from_cubic(cubic);

	BOOST_CHECK_EQUAL(loc.x, back.x);
	BOOST_CHECK_EQUAL(loc.y, back.y);
}

BOOST_AUTO_TEST_CASE(test_cubic_conversion_properties)
{
	map_location loc(5, 5);
	cubic_location cubic = loc.to_cubic();

	// Cubic coordinates should satisfy q + r + s = 0
	BOOST_CHECK_EQUAL(cubic.q + cubic.r + cubic.s, 0);
}

BOOST_AUTO_TEST_CASE(test_cubic_conversion_various)
{
	// Test various locations
	std::vector<map_location> test_locs = {
		map_location(0, 0),
		map_location(1, 0),
		map_location(0, 1),
		map_location(5, 5),
		map_location(10, 3),
		map_location(100, 100)
	};

	for (const auto& loc : test_locs) {
		cubic_location cubic = loc.to_cubic();
		map_location back = map_location::from_cubic(cubic);
		BOOST_CHECK_EQUAL(loc.x, back.x);
		BOOST_CHECK_EQUAL(loc.y, back.y);
		BOOST_CHECK_EQUAL(cubic.q + cubic.r + cubic.s, 0);
	}
}

// ============================================================================
// Map Location Hash - Extended
// ============================================================================

BOOST_AUTO_TEST_CASE(test_hash_uniqueness)
{
	std::hash<map_location> hasher;

	// Different locations should (likely) have different hashes
	std::set<std::size_t> hashes;
	int collisions = 0;

	for (int x = 0; x < 10; ++x) {
		for (int y = 0; y < 10; ++y) {
			std::size_t h = hasher(map_location(x, y));
			if (hashes.count(h) > 0) {
				collisions++;
			}
			hashes.insert(h);
		}
	}

	// Allow some collisions but not too many
	BOOST_CHECK(collisions < 10);
}

BOOST_AUTO_TEST_CASE(test_hash_consistency_multiple_calls)
{
	map_location loc(5, 10);
	std::hash<map_location> hasher;

	std::size_t h1 = hasher(loc);
	std::size_t h2 = hasher(loc);
	std::size_t h3 = hasher(loc);

	BOOST_CHECK_EQUAL(h1, h2);
	BOOST_CHECK_EQUAL(h2, h3);
}

// ============================================================================
// Map Location Stream Output
// ============================================================================

BOOST_AUTO_TEST_CASE(test_stream_output)
{
	map_location loc(4, 5); // WML coords would be 5,6

	std::stringstream ss;
	ss << loc;

	std::string result = ss.str();
	BOOST_CHECK(!result.empty());
	// Should contain WML coordinates (1-based)
	BOOST_CHECK(result.find("5") != std::string::npos);
	BOOST_CHECK(result.find("6") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_stream_output_vector)
{
	std::vector<map_location> locs;
	locs.emplace_back(0, 0);
	locs.emplace_back(5, 5);

	std::stringstream ss;
	ss << locs;

	std::string result = ss.str();
	BOOST_CHECK(!result.empty());
}

BOOST_AUTO_TEST_CASE(test_stream_output_direction)
{
	std::stringstream ss;
	ss << map_location::direction::north;

	std::string result = ss.str();
	BOOST_CHECK_EQUAL(result, "n");
}

// ============================================================================
// Map Location Do Compare
// ============================================================================

BOOST_AUTO_TEST_CASE(test_do_compare_equal)
{
	map_location loc1(5, 5);
	map_location loc2(5, 5);

	BOOST_CHECK_EQUAL(loc1.do_compare(loc2), 0);
}

BOOST_AUTO_TEST_CASE(test_do_compare_less_x)
{
	map_location loc1(3, 5);
	map_location loc2(5, 5);

	BOOST_CHECK_LT(loc1.do_compare(loc2), 0);
	BOOST_CHECK_GT(loc2.do_compare(loc1), 0);
}

BOOST_AUTO_TEST_CASE(test_do_compare_less_y)
{
	map_location loc1(5, 3);
	map_location loc2(5, 5);

	BOOST_CHECK_LT(loc1.do_compare(loc2), 0);
	BOOST_CHECK_GT(loc2.do_compare(loc1), 0);
}

// ============================================================================
// Edge Cases and Stress Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_multiple_direction_steps)
{
	map_location center(5, 5);

	// Go 5 steps north
	map_location north_5 = center.get_direction(map_location::direction::north, 5);
	BOOST_CHECK_EQUAL(north_5.x, 5);
	BOOST_CHECK_EQUAL(north_5.y, 0);

	// Go 5 steps south
	map_location south_5 = center.get_direction(map_location::direction::south, 5);
	BOOST_CHECK_EQUAL(south_5.x, 5);
	BOOST_CHECK_EQUAL(south_5.y, 10);
}

BOOST_AUTO_TEST_CASE(test_direction_negative_large_steps)
{
	map_location center(5, 5);

	// Negative steps should go opposite direction
	map_location neg_north = center.get_direction(map_location::direction::north, -10);
	map_location pos_south = center.get_direction(map_location::direction::south, 10);

	BOOST_CHECK_EQUAL(neg_north.x, pos_south.x);
	BOOST_CHECK_EQUAL(neg_north.y, pos_south.y);
}

BOOST_AUTO_TEST_CASE(test_get_direction_indeterminate)
{
	map_location center(5, 5);

	// Indeterminate direction should return null location
	map_location result = center.get_direction(map_location::direction::indeterminate);
	BOOST_CHECK(!result.valid());
}

BOOST_AUTO_TEST_SUITE_END()
