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

#include "arrow.hpp"
#include "map/location.hpp"

BOOST_AUTO_TEST_SUITE( arrow_suite )

/**
 * Test arrow construction
 */
BOOST_AUTO_TEST_CASE( test_arrow_construction )
{
	arrow a;
	BOOST_CHECK(a.get_path().empty());
}

/**
 * Test arrow construction with hidden parameter
 */
BOOST_AUTO_TEST_CASE( test_arrow_construction_hidden )
{
	arrow a(false);  // Not hidden
	BOOST_CHECK(a.get_path().empty());
}

/**
 * Test arrow path setting
 */
BOOST_AUTO_TEST_CASE( test_arrow_path )
{
	arrow a;
	std::vector<map_location> path;
	path.emplace_back(1, 1);
	path.emplace_back(2, 2);
	path.emplace_back(3, 3);

	a.set_path(path);
	BOOST_CHECK_EQUAL(a.get_path().size(), 3);
	BOOST_CHECK_EQUAL(a.get_path()[0], map_location(1, 1));
	BOOST_CHECK_EQUAL(a.get_path()[1], map_location(2, 2));
	BOOST_CHECK_EQUAL(a.get_path()[2], map_location(3, 3));
}

/**
 * Test arrow path reset
 */
BOOST_AUTO_TEST_CASE( test_arrow_reset )
{
	arrow a;
	std::vector<map_location> path;
	path.emplace_back(1, 1);
	path.emplace_back(2, 2);

	a.set_path(path);
	BOOST_CHECK_EQUAL(a.get_path().size(), 2);

	a.reset();
	BOOST_CHECK(a.get_path().empty());
}

/**
 * Test arrow color setting
 */
BOOST_AUTO_TEST_CASE( test_arrow_color )
{
	arrow a;
	a.set_color("yellow");
	BOOST_CHECK_EQUAL(a.get_color(), "yellow");

	a.set_color("red");
	BOOST_CHECK_EQUAL(a.get_color(), "red");
}

/**
 * Test arrow style
 */
BOOST_AUTO_TEST_CASE( test_arrow_style )
{
	arrow a;
	a.set_style("highlighted");

	a.set_style("standard");
}

/**
 * Test arrow visibility
 */
BOOST_AUTO_TEST_CASE( test_arrow_visibility )
{
	arrow a;

	a.hide();

	a.show();
}

/**
 * Test arrow path contains
 */
BOOST_AUTO_TEST_CASE( test_arrow_path_contains )
{
	arrow a;
	std::vector<map_location> path;
	path.emplace_back(1, 1);
	path.emplace_back(2, 2);
	path.emplace_back(3, 3);
	a.set_path(path);

	BOOST_CHECK(a.path_contains(map_location(1, 1)));
	BOOST_CHECK(a.path_contains(map_location(2, 2)));
	BOOST_CHECK(a.path_contains(map_location(3, 3)));
	BOOST_CHECK(!a.path_contains(map_location(4, 4)));
}

/**
 * Test arrow valid path
 */
BOOST_AUTO_TEST_CASE( test_arrow_valid_path )
{
	std::vector<map_location> valid_path;
	valid_path.emplace_back(1, 1);
	valid_path.emplace_back(2, 2);

	BOOST_CHECK(arrow::valid_path(valid_path));
}

/**
 * Test arrow empty path
 */
BOOST_AUTO_TEST_CASE( test_arrow_empty_path )
{
	arrow a;
	std::vector<map_location> empty_path;

	a.set_path(empty_path);
	BOOST_CHECK(a.get_path().empty());
}

BOOST_AUTO_TEST_SUITE_END()
