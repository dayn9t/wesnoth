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
#include "config.hpp"
#include "map/location.hpp"

BOOST_AUTO_TEST_SUITE(editor)

//
// brush tests
//

BOOST_AUTO_TEST_CASE(test_brush_default_construction)
{
	editor::brush b;

	// Default brush should have empty name and id
	BOOST_CHECK_EQUAL(b.name(), "");
	BOOST_CHECK_EQUAL(b.id(), "");
}

BOOST_AUTO_TEST_CASE(test_brush_config_construction_empty)
{
	config cfg;
	cfg["name"] = "Test Brush";
	cfg["id"] = "test_brush";

	editor::brush b(cfg);

	BOOST_CHECK_EQUAL(b.name(), "Test Brush");
	BOOST_CHECK_EQUAL(b.id(), "test_brush");
}

BOOST_AUTO_TEST_CASE(test_brush_config_construction_with_radius)
{
	config cfg;
	cfg["name"] = "Radius Brush";
	cfg["id"] = "radius_brush";
	cfg["radius"] = 2;

	editor::brush b(cfg);

	BOOST_CHECK_EQUAL(b.name(), "Radius Brush");
	BOOST_CHECK_EQUAL(b.id(), "radius_brush");

	// A radius-2 brush should project to multiple tiles
	map_location hotspot(5, 5);
	std::set<map_location> projected = b.project(hotspot);
	BOOST_CHECK(projected.size() > 1);
	// The hotspot itself should be included
	BOOST_CHECK(projected.find(hotspot) != projected.end());
}

BOOST_AUTO_TEST_CASE(test_brush_config_construction_with_relative)
{
	config cfg;
	cfg["name"] = "Custom Brush";
	cfg["id"] = "custom_brush";

	config& rel1 = cfg.add_child("relative");
	rel1["x"] = 0;
	rel1["y"] = 0;

	config& rel2 = cfg.add_child("relative");
	rel2["x"] = 1;
	rel2["y"] = 0;

	config& rel3 = cfg.add_child("relative");
	rel3["x"] = 0;
	rel3["y"] = 1;

	editor::brush b(cfg);

	// Project from (0,0) - should get the relative locations
	std::set<map_location> projected = b.project(map_location(0, 0));
	BOOST_CHECK_EQUAL(projected.size(), 3);

	// Check that the projected locations are correct
	BOOST_CHECK(projected.find(map_location(0, 0)) != projected.end());
	BOOST_CHECK(projected.find(map_location(1, 0)) != projected.end());
	BOOST_CHECK(projected.find(map_location(0, 1)) != projected.end());
}

BOOST_AUTO_TEST_CASE(test_brush_add_relative_location)
{
	editor::brush b;

	b.add_relative_location(0, 0);
	b.add_relative_location(1, 0);
	b.add_relative_location(-1, 0);

	map_location hotspot(10, 10);
	std::set<map_location> projected = b.project(hotspot);

	BOOST_CHECK_EQUAL(projected.size(), 3);
	BOOST_CHECK(projected.find(map_location(10, 10)) != projected.end());
	BOOST_CHECK(projected.find(map_location(11, 10)) != projected.end());
	BOOST_CHECK(projected.find(map_location(9, 10)) != projected.end());
}

BOOST_AUTO_TEST_CASE(test_brush_add_duplicate_location)
{
	editor::brush b;

	b.add_relative_location(0, 0);
	b.add_relative_location(0, 0);  // Duplicate should be ignored (set)

	map_location hotspot(0, 0);
	std::set<map_location> projected = b.project(hotspot);

	// Should only have one tile due to set semantics
	BOOST_CHECK_EQUAL(projected.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_brush_project)
{
	editor::brush b;
	b.add_relative_location(0, 0);
	b.add_relative_location(1, 1);

	map_location hotspot(5, 5);
	std::set<map_location> projected = b.project(hotspot);

	BOOST_CHECK_EQUAL(projected.size(), 2);
	BOOST_CHECK(projected.find(map_location(5, 5)) != projected.end());
	BOOST_CHECK(projected.find(map_location(6, 6)) != projected.end());
}

BOOST_AUTO_TEST_CASE(test_brush_project_empty)
{
	editor::brush b;

	map_location hotspot(0, 0);
	std::set<map_location> projected = b.project(hotspot);

	BOOST_CHECK_EQUAL(projected.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_brush_project_negative_hotspot)
{
	editor::brush b;
	b.add_relative_location(0, 0);
	b.add_relative_location(1, 0);

	map_location hotspot(-5, -5);
	std::set<map_location> projected = b.project(hotspot);

	BOOST_CHECK_EQUAL(projected.size(), 2);
	BOOST_CHECK(projected.find(map_location(-5, -5)) != projected.end());
	BOOST_CHECK(projected.find(map_location(-4, -5)) != projected.end());
}

//
// editor_exception tests
//

BOOST_AUTO_TEST_CASE(test_editor_exception_message)
{
	try {
		throw editor::editor_exception("Test error message");
	} catch (const editor::editor_exception& e) {
		BOOST_CHECK_EQUAL(std::string(e.what()), "Test error message");
	}
}

BOOST_AUTO_TEST_CASE(test_editor_logic_exception_message)
{
	try {
		throw editor::editor_logic_exception("Logic error");
	} catch (const editor::editor_logic_exception& e) {
		BOOST_CHECK_EQUAL(std::string(e.what()), "Logic error");
	}
}

BOOST_AUTO_TEST_CASE(test_editor_logic_exception_is_editor_exception)
{
	// Verify inheritance hierarchy
	try {
		throw editor::editor_logic_exception("Test");
	} catch (const editor::editor_exception& e) {
		// Should catch as editor_exception too
		BOOST_CHECK_EQUAL(std::string(e.what()), "Test");
	}
}

//
// map_location vector operations used by editor
//

BOOST_AUTO_TEST_CASE(test_map_location_vector_sum)
{
	map_location a(3, 4);
	map_location b(1, 2);
	map_location sum = a.vector_sum(b);

	BOOST_CHECK_EQUAL(sum.x, 4);
	BOOST_CHECK_EQUAL(sum.y, 6);
}

BOOST_AUTO_TEST_CASE(test_map_location_vector_negation)
{
	map_location a(3, 4);
	map_location neg = a.vector_negation();

	BOOST_CHECK_EQUAL(neg.x, -3);
	BOOST_CHECK_EQUAL(neg.y, -4);
}

BOOST_AUTO_TEST_CASE(test_map_location_vector_negation_double)
{
	map_location a(5, 7);
	map_location neg = a.vector_negation();
	map_location negneg = neg.vector_negation();

	BOOST_CHECK_EQUAL(negneg.x, a.x);
	BOOST_CHECK_EQUAL(negneg.y, a.y);
}

BOOST_AUTO_TEST_CASE(test_map_location_vector_sum_negation_inverse)
{
	map_location a(10, 20);
	map_location neg = a.vector_negation();
	map_location sum = a.vector_sum(neg);

	BOOST_CHECK_EQUAL(sum.x, 0);
	BOOST_CHECK_EQUAL(sum.y, 0);
	BOOST_CHECK_EQUAL(sum, map_location::ZERO());
}

//
// config-based brush edge cases
//

BOOST_AUTO_TEST_CASE(test_brush_config_with_zero_radius)
{
	config cfg;
	cfg["name"] = "Zero Radius";
	cfg["id"] = "zero_radius";
	cfg["radius"] = 0;

	editor::brush b(cfg);

	// Zero radius should not add any tiles
	map_location hotspot(0, 0);
	std::set<map_location> projected = b.project(hotspot);
	BOOST_CHECK_EQUAL(projected.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_brush_config_with_negative_radius)
{
	config cfg;
	cfg["name"] = "Negative Radius";
	cfg["id"] = "neg_radius";
	cfg["radius"] = -1;

	editor::brush b(cfg);

	// Negative radius should not add any tiles
	map_location hotspot(0, 0);
	std::set<map_location> projected = b.project(hotspot);
	BOOST_CHECK_EQUAL(projected.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_brush_radius_and_relative_combined)
{
	config cfg;
	cfg["name"] = "Combined";
	cfg["id"] = "combined";
	cfg["radius"] = 1;

	// Also add a custom relative location
	config& rel = cfg.add_child("relative");
	rel["x"] = 10;
	rel["y"] = 10;

	editor::brush b(cfg);

	map_location hotspot(0, 0);
	std::set<map_location> projected = b.project(hotspot);

	// Should have tiles from radius=1 plus the custom relative location
	BOOST_CHECK(projected.size() >= 1);
	// The custom location should be included
	BOOST_CHECK(projected.find(map_location(10, 10)) != projected.end());
}

BOOST_AUTO_TEST_SUITE_END()
