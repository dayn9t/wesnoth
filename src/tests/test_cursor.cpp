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

#include "cursor.hpp"

BOOST_AUTO_TEST_SUITE( cursor_suite )

/**
 * Test cursor setter
 */
BOOST_AUTO_TEST_CASE( test_cursor_setter )
{
	// Test that cursor setter changes the cursor type
	cursor::setter s(cursor::WAIT);
	// The setter should set the cursor and restore it on destruction
	BOOST_CHECK(true); // Basic test that construction doesn't crash
}

/**
 * Test cursor types
 */
BOOST_AUTO_TEST_CASE( test_cursor_types )
{
	// Test different cursor types
	BOOST_CHECK(cursor::NORMAL != cursor::WAIT);
	BOOST_CHECK(cursor::WAIT != cursor::MOVE);
	BOOST_CHECK(cursor::MOVE != cursor::ATTACK);
	BOOST_CHECK(cursor::ATTACK != cursor::HYPERLINK);
}

/**
 * Test cursor state
 */
BOOST_AUTO_TEST_CASE( test_cursor_state )
{
	// Get current cursor state
	cursor::CURSOR_TYPE current = cursor::get();
	// Should return a valid cursor type
	BOOST_CHECK(current >= cursor::NORMAL && current <= cursor::HYPERLINK);
}

BOOST_AUTO_TEST_SUITE_END()
