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

BOOST_AUTO_TEST_SUITE( sound )

// Test sound configuration basic
BOOST_AUTO_TEST_CASE( test_sound_config_basic )
{
	config cfg;
	cfg["name"] = "sword-1.ogg";
	cfg["volume"] = "100";

	BOOST_CHECK_EQUAL(cfg["name"].str(), "sword-1.ogg");
	BOOST_CHECK_EQUAL(cfg["volume"].str(), "100");
}

// Test sound with delay
BOOST_AUTO_TEST_CASE( test_sound_with_delay )
{
	config cfg;
	cfg["name"] = "explosion.ogg";
	cfg["delay"] = "500";

	BOOST_CHECK_EQUAL(cfg["delay"].str(), "500");
}

// Test sound repeat
BOOST_AUTO_TEST_CASE( test_sound_repeat )
{
	config cfg;
	cfg["name"] = "ambient.ogg";
	cfg["repeat"] = "yes";

	BOOST_CHECK_EQUAL(cfg["repeat"].str(), "yes");
}

// Test music configuration
BOOST_AUTO_TEST_CASE( test_music_config )
{
	config cfg;
	cfg["name"] = "main_menu.ogg";
	cfg["ms_before"] = "1000";
	cfg["ms_after"] = "2000";

	BOOST_CHECK_EQUAL(cfg["name"].str(), "main_menu.ogg");
	BOOST_CHECK_EQUAL(cfg["ms_before"].str(), "1000");
	BOOST_CHECK_EQUAL(cfg["ms_after"].str(), "2000");
}

// Test sound source
BOOST_AUTO_TEST_CASE( test_sound_source )
{
	config cfg;
	cfg["sounds"] = "water.ogg";
	cfg["x"] = "10";
	cfg["y"] = "15";
	cfg["full_range"] = "5";

	BOOST_CHECK_EQUAL(cfg["sounds"].str(), "water.ogg");
	BOOST_CHECK_EQUAL(cfg["x"].str(), "10");
	BOOST_CHECK_EQUAL(cfg["y"].str(), "15");
}

BOOST_AUTO_TEST_SUITE_END()
