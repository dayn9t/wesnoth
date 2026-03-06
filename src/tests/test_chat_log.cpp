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

#include "chat_log.hpp"

BOOST_AUTO_TEST_SUITE( chat_log_suite )

/**
 * Test chat log entry construction
 */
BOOST_AUTO_TEST_CASE( test_chat_log_entry )
{
	chat_log::log_entry entry;
	entry.nick = "TestUser";
	entry.message = "Hello, World!";
	entry.time = 1234567890;

	BOOST_CHECK_EQUAL(entry.nick, "TestUser");
	BOOST_CHECK_EQUAL(entry.message, "Hello, World!");
	BOOST_CHECK_EQUAL(entry.time, 1234567890);
}

/**
 * Test chat log type constants
 */
BOOST_AUTO_TEST_CASE( test_chat_log_types )
{
	BOOST_CHECK(chat_log::MSG_WHISPER != chat_log::MSG_STANDARD);
	BOOST_CHECK(chat_log::MSG_STANDARD != chat_log::MSG_MOD);
	BOOST_CHECK(chat_log::MSG_MOD != chat_log::MSG_SERVER);
}

BOOST_AUTO_TEST_SUITE_END()
