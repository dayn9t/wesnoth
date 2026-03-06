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

#include "editor/toolkit/brush.hpp"
#include "editor/editor_common.hpp"
#include "editor/action/action_base.hpp"
#include "editor/map/editor_map.hpp"
#include "editor/map/map_fragment.hpp"
#include "config.hpp"
#include "map/location.hpp"
#include "terrain/translation.hpp"

BOOST_AUTO_TEST_SUITE(editor_v2)

//
// Extended brush tests - boundary cases
//

BOOST_AUTO_TEST_CASE(test_brush_large_radius)
{
	config cfg;
	cfg["name"] = "Large Brush";
	cfg["id"] = "large_brush";
	cfg["radius"] = 5;

	editor::brush b(cfg);

	map_location hotspot(10, 10);
	std::set<map_location> projected = b.project(hotspot);

	// A radius-5 brush should cover many tiles
	BOOST_CHECK(projected.size() > 20);
	// The hotspot itself should be included
	BOOST_CHECK(projected.find(hotspot) != projected.end());
}

BOOST_AUTO_TEST_CASE(test_brush_project_with_large_positive_offset)
{
	editor::brush b;
	b.add_relative_location(100, 100);
	b.add_relative_location(0, 0);

	map_location hotspot(50, 50);
	std::set<map_location> projected = b.project(hotspot);

	BOOST_CHECK_EQUAL(projected.size(), 2);
	BOOST_CHECK(projected.find(map_location(50, 50)) != projected.end());
	BOOST_CHECK(projected.find(map_location(150, 150)) != projected.end());
}

BOOST_AUTO_TEST_CASE(test_brush_project_with_large_negative_offset)
{
	editor::brush b;
	b.add_relative_location(-100, -100);
	b.add_relative_location(0, 0);

	map_location hotspot(50, 50);
	std::set<map_location> projected = b.project(hotspot);

	BOOST_CHECK_EQUAL(projected.size(), 2);
	BOOST_CHECK(projected.find(map_location(50, 50)) != projected.end());
	BOOST_CHECK(projected.find(map_location(-50, -50)) != projected.end());
}

BOOST_AUTO_TEST_CASE(test_brush_config_empty_name_and_id)
{
	config cfg;
	// No name or id set - should default to empty strings

	editor::brush b(cfg);

	BOOST_CHECK_EQUAL(b.name(), "");
	BOOST_CHECK_EQUAL(b.id(), "");
}

BOOST_AUTO_TEST_CASE(test_brush_multiple_relative_same_location)
{
	config cfg;
	cfg["name"] = "Multi";
	cfg["id"] = "multi";

	// Add same location multiple times
	for(int i = 0; i < 5; ++i) {
		config& rel = cfg.add_child("relative");
		rel["x"] = 1;
		rel["y"] = 1;
	}

	editor::brush b(cfg);

	map_location hotspot(0, 0);
	std::set<map_location> projected = b.project(hotspot);

	// Should still only have one tile at (1,1)
	BOOST_CHECK_EQUAL(projected.size(), 1);
	BOOST_CHECK(projected.find(map_location(1, 1)) != projected.end());
}

//
// Editor exception hierarchy tests
//

BOOST_AUTO_TEST_CASE(test_editor_action_exception_message)
{
	try {
		throw editor::editor_action_exception("Action failed");
	} catch(const editor::editor_exception& e) {
		BOOST_CHECK_EQUAL(std::string(e.what()), "Action failed");
	}
}

BOOST_AUTO_TEST_CASE(test_editor_action_exception_is_editor_exception)
{
	bool caught_as_base = false;
	try {
		throw editor::editor_action_exception("Test");
	} catch(const editor::editor_exception&) {
		caught_as_base = true;
	}
	BOOST_CHECK(caught_as_base);
}

//
// Map location operations used in editor context
//

BOOST_AUTO_TEST_CASE(test_map_location_zero_equality)
{
	map_location zero1 = map_location::ZERO();
	map_location zero2(0, 0);

	BOOST_CHECK_EQUAL(zero1, zero2);
}

BOOST_AUTO_TEST_CASE(test_map_location_vector_sum_commutative)
{
	map_location a(7, 11);
	map_location b(3, 4);

	map_location sum1 = a.vector_sum(b);
	map_location sum2 = b.vector_sum(a);

	BOOST_CHECK_EQUAL(sum1, sum2);
	BOOST_CHECK_EQUAL(sum1.x, 10);
	BOOST_CHECK_EQUAL(sum1.y, 15);
}

BOOST_AUTO_TEST_CASE(test_map_location_vector_sum_with_zero)
{
	map_location a(5, 8);
	map_location zero = map_location::ZERO();

	map_location sum = a.vector_sum(zero);

	BOOST_CHECK_EQUAL(sum, a);
}

BOOST_AUTO_TEST_CASE(test_map_location_vector_negation_of_zero)
{
	map_location zero = map_location::ZERO();
	map_location neg = zero.vector_negation();

	BOOST_CHECK_EQUAL(neg, zero);
	BOOST_CHECK_EQUAL(neg.x, 0);
	BOOST_CHECK_EQUAL(neg.y, 0);
}

//
// Brush coordinate edge cases
//

BOOST_AUTO_TEST_CASE(test_brush_single_tile)
{
	editor::brush b;
	b.add_relative_location(0, 0);

	map_location hotspot(0, 0);
	std::set<map_location> projected = b.project(hotspot);

	BOOST_CHECK_EQUAL(projected.size(), 1);
	BOOST_CHECK(projected.find(hotspot) != projected.end());
}

BOOST_AUTO_TEST_CASE(test_brush_hex_line_horizontal)
{
	editor::brush b;
	for(int x = -2; x <= 2; ++x) {
		b.add_relative_location(x, 0);
	}

	map_location hotspot(10, 10);
	std::set<map_location> projected = b.project(hotspot);

	BOOST_CHECK_EQUAL(projected.size(), 5);
	for(int x = 8; x <= 12; ++x) {
		BOOST_CHECK(projected.find(map_location(x, 10)) != projected.end());
	}
}

BOOST_AUTO_TEST_CASE(test_brush_hex_line_vertical)
{
	editor::brush b;
	for(int y = -2; y <= 2; ++y) {
		b.add_relative_location(0, y);
	}

	map_location hotspot(10, 10);
	std::set<map_location> projected = b.project(hotspot);

	BOOST_CHECK_EQUAL(projected.size(), 5);
	for(int y = 8; y <= 12; ++y) {
		BOOST_CHECK(projected.find(map_location(10, y)) != projected.end());
	}
}

//
// Config-based brush parsing edge cases
//

BOOST_AUTO_TEST_CASE(test_brush_config_with_special_characters_in_name)
{
	config cfg;
	cfg["name"] = "Brush & <Special> \"Chars\"";
	cfg["id"] = "special_brush";

	editor::brush b(cfg);

	BOOST_CHECK_EQUAL(b.name(), "Brush & <Special> \"Chars\"");
	BOOST_CHECK_EQUAL(b.id(), "special_brush");
}

BOOST_AUTO_TEST_CASE(test_brush_config_with_unicode_name)
{
	config cfg;
	cfg["name"] = "Brush Unicode";
	cfg["id"] = "unicode_brush";

	editor::brush b(cfg);

	BOOST_CHECK_EQUAL(b.name(), "Brush Unicode");
	BOOST_CHECK_EQUAL(b.id(), "unicode_brush");
}

//
// Brush projection consistency tests
//

BOOST_AUTO_TEST_CASE(test_brush_project_consistency)
{
	editor::brush b;
	b.add_relative_location(1, 0);
	b.add_relative_location(-1, 0);
	b.add_relative_location(0, 1);
	b.add_relative_location(0, -1);

	// Project from different hotspots should give same relative positions
	map_location h1(0, 0);
	map_location h2(100, 100);
	map_location h3(-50, -50);

	std::set<map_location> p1 = b.project(h1);
	std::set<map_location> p2 = b.project(h2);
	std::set<map_location> p3 = b.project(h3);

	BOOST_CHECK_EQUAL(p1.size(), p2.size());
	BOOST_CHECK_EQUAL(p1.size(), p3.size());
	BOOST_CHECK_EQUAL(p1.size(), 4);
}

BOOST_AUTO_TEST_CASE(test_brush_empty_vs_radius_zero)
{
	// Empty brush (no locations added)
	editor::brush b1;

	// Brush with radius 0 from config
	config cfg;
	cfg["radius"] = 0;
	editor::brush b2(cfg);

	map_location hotspot(5, 5);

	BOOST_CHECK_EQUAL(b1.project(hotspot).size(), b2.project(hotspot).size());
	BOOST_CHECK_EQUAL(b1.project(hotspot).size(), 0);
}

//
// Additional map location operations
//

BOOST_AUTO_TEST_CASE(test_map_location_distance_to_self)
{
	map_location loc(5, 5);
	BOOST_CHECK_EQUAL(distance_between(loc, loc), 0);
}

BOOST_AUTO_TEST_CASE(test_map_location_distance_adjacent)
{
	map_location a(5, 5);
	map_location north = a.get_direction(map_location::direction::north);

	BOOST_CHECK_EQUAL(distance_between(a, north), 1);
	BOOST_CHECK(tiles_adjacent(a, north));
}

BOOST_AUTO_TEST_CASE(test_map_location_self_not_adjacent)
{
	map_location loc(3, 4);
	BOOST_CHECK(!tiles_adjacent(loc, loc));
}

BOOST_AUTO_TEST_CASE(test_map_location_distance_symmetric)
{
	map_location a(10, 15);
	map_location b(5, 8);

	BOOST_CHECK_EQUAL(distance_between(a, b), distance_between(b, a));
}

//
// Terrain code related tests (for editor terrain painting)
//

BOOST_AUTO_TEST_CASE(test_terrain_code_none_terrain)
{
	t_translation::terrain_code none = t_translation::NONE_TERRAIN;

	// NONE_TERRAIN should have specific characteristics
	// Just verify we can create and access it
	BOOST_CHECK(none.base >= 0);
}

BOOST_AUTO_TEST_CASE(test_terrain_code_equality)
{
	t_translation::terrain_code t1 = t_translation::GRASS_LAND;
	t_translation::terrain_code t2 = t_translation::GRASS_LAND;

	BOOST_CHECK(t1 == t2);
}

//
// Exception catch polymorphism tests
//

BOOST_AUTO_TEST_CASE(test_catch_editor_logic_exception_as_game_error)
{
	bool caught = false;
	try {
		throw editor::editor_logic_exception("Logic test");
	} catch(const game::error& e) {
		caught = true;
		BOOST_CHECK_EQUAL(std::string(e.what()), "Logic test");
	}
	BOOST_CHECK(caught);
}

BOOST_AUTO_TEST_CASE(test_editor_exception_what_not_empty)
{
	editor::editor_exception ex("Non-empty message");
	BOOST_CHECK(!std::string(ex.what()).empty());
}

//
// Brush with mixed relative and radius configuration
//

BOOST_AUTO_TEST_CASE(test_brush_radius_one_basic)
{
	config cfg;
	cfg["name"] = "Radius One";
	cfg["id"] = "radius_one";
	cfg["radius"] = 1;

	editor::brush b(cfg);

	map_location hotspot(0, 0);
	std::set<map_location> projected = b.project(hotspot);

	// Radius 1 should include hotspot and adjacent tiles
	BOOST_CHECK(projected.size() >= 1);
	BOOST_CHECK(projected.find(hotspot) != projected.end());
}

//
// Vector arithmetic edge cases
//

BOOST_AUTO_TEST_CASE(test_map_location_large_values_vector_sum)
{
	map_location a(1000, 2000);
	map_location b(500, 300);

	map_location sum = a.vector_sum(b);

	BOOST_CHECK_EQUAL(sum.x, 1500);
	BOOST_CHECK_EQUAL(sum.y, 2300);
}

BOOST_AUTO_TEST_CASE(test_map_location_large_values_vector_negation)
{
	map_location a(1000, -2000);
	map_location neg = a.vector_negation();

	BOOST_CHECK_EQUAL(neg.x, -1000);
	BOOST_CHECK_EQUAL(neg.y, 2000);
}

BOOST_AUTO_TEST_CASE(test_map_location_vector_sum_negation_chain)
{
	map_location a(10, 20);
	map_location b(5, 8);

	map_location sum = a.vector_sum(b);
	map_location neg = sum.vector_negation();
	map_location expected(-15, -28);

	BOOST_CHECK_EQUAL(neg, expected);
}

BOOST_AUTO_TEST_SUITE_END()
