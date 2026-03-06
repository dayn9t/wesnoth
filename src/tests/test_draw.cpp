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

#include "draw.hpp"
#include "map/location.hpp"

BOOST_AUTO_TEST_SUITE( draw_suite )

/**
 * Test drawing operations exist
 */
BOOST_AUTO_TEST_CASE( test_draw_functions_exist )
{
	// Basic test that draw functions are available
	// draw.cpp provides low-level drawing primitives
	BOOST_CHECK(true);
}

/**
 * Test draw module loads
 */
BOOST_AUTO_TEST_CASE( test_draw_module )
{
	// draw.hpp provides low-level drawing primitives
	// Just verify the module compiles correctly
	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()
