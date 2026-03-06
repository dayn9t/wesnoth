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

#include "events.hpp"

BOOST_AUTO_TEST_SUITE( events_suite )

/**
 * Test event context construction
 */
BOOST_AUTO_TEST_CASE( test_event_context )
{
	// Test that event context can be created
	events::event_context ctx;
	BOOST_CHECK(true);
}

/**
 * Test main thread setter
 */
BOOST_AUTO_TEST_CASE( test_events_main_thread )
{
	// set_main_thread should not crash
	events::set_main_thread();
	BOOST_CHECK(true);
}

/**
 * Test event types exist
 */
BOOST_AUTO_TEST_CASE( test_event_types )
{
	// Test that SDL event constants are accessible
	BOOST_CHECK(SDL_FIRSTEVENT > 0);
	BOOST_CHECK(SDL_QUIT > 0);
	BOOST_CHECK(SDL_MOUSEMOTION > 0);
	BOOST_CHECK(SDL_MOUSEBUTTONDOWN > 0);
	BOOST_CHECK(SDL_KEYDOWN > 0);
	BOOST_CHECK(SDL_KEYUP > 0);
}

BOOST_AUTO_TEST_SUITE_END()
