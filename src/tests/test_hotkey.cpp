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

#include "config.hpp"

BOOST_AUTO_TEST_SUITE( hotkey )

// Test hotkey configuration basic
BOOST_AUTO_TEST_CASE( test_hotkey_config_basic )
{
	config cfg;
	cfg["command"] = "undo";
	cfg["key"] = "ctrl+z";

	BOOST_CHECK_EQUAL(cfg["command"].str(), "undo");
	BOOST_CHECK_EQUAL(cfg["key"].str(), "ctrl+z");
}

// Test hotkey with multiple keys
BOOST_AUTO_TEST_CASE( test_hotkey_multiple_keys )
{
	config cfg;
	cfg["command"] = "save";
	cfg["key"] = "ctrl+s";
	cfg["alt_key"] = "F5";

	BOOST_CHECK_EQUAL(cfg["command"].str(), "save");
	BOOST_CHECK_EQUAL(cfg["key"].str(), "ctrl+s");
	BOOST_CHECK_EQUAL(cfg["alt_key"].str(), "F5");
}

// Test hotkey with modifiers
BOOST_AUTO_TEST_CASE( test_hotkey_modifiers )
{
	config cfg;
	cfg["command"] = "quit";
	cfg["key"] = "ctrl+shift+q";

	BOOST_CHECK_EQUAL(cfg["key"].str(), "ctrl+shift+q");
}

// Test hotkey scope
BOOST_AUTO_TEST_CASE( test_hotkey_scope )
{
	config cfg;
	cfg["command"] = "attack";
	cfg["key"] = "a";
	cfg["scope"] = "game";

	BOOST_CHECK_EQUAL(cfg["scope"].str(), "game");
}

// Test hotkey disabled
BOOST_AUTO_TEST_CASE( test_hotkey_disabled )
{
	config cfg;
	cfg["command"] = "test";
	cfg["key"] = "t";
	cfg["disabled"] = "yes";

	BOOST_CHECK_EQUAL(cfg["disabled"].str(), "yes");
}

// Test hotkey description
BOOST_AUTO_TEST_CASE( test_hotkey_description )
{
	config cfg;
	cfg["command"] = "help";
	cfg["key"] = "F1";
	cfg["description"] = "Show help";

	BOOST_CHECK_EQUAL(cfg["description"].str(), "Show help");
}

// Test hotkey empty
BOOST_AUTO_TEST_CASE( test_hotkey_empty )
{
	config cfg;

	BOOST_CHECK(cfg.empty());
	BOOST_CHECK(cfg["command"].empty());
	BOOST_CHECK(cfg["key"].empty());
}

// Test hotkey serialization
BOOST_AUTO_TEST_SUITE_END()
