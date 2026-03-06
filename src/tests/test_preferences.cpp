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

BOOST_AUTO_TEST_SUITE( preferences )

// Test preferences configuration basic
BOOST_AUTO_TEST_CASE( test_preferences_basic )
{
	config cfg;
	cfg["fullscreen"] = "yes";
	cfg["sound"] = "yes";
	cfg["music"] = "yes";

	BOOST_CHECK_EQUAL(cfg["fullscreen"].str(), "yes");
	BOOST_CHECK_EQUAL(cfg["sound"].str(), "yes");
	BOOST_CHECK_EQUAL(cfg["music"].str(), "yes");
}

// Test preferences resolution
BOOST_AUTO_TEST_CASE( test_preferences_resolution )
{
	config cfg;
	cfg["resolution"] = "1920x1080";
	cfg["windowed"] = "no";

	BOOST_CHECK_EQUAL(cfg["resolution"].str(), "1920x1080");
	BOOST_CHECK_EQUAL(cfg["windowed"].str(), "no");
}

// Test preferences volume
BOOST_AUTO_TEST_CASE( test_preferences_volume )
{
	config cfg;
	cfg["sound_volume"] = "80";
	cfg["music_volume"] = "60";
	cfg["bell_volume"] = "50";

	BOOST_CHECK_EQUAL(cfg["sound_volume"].str(), "80");
	BOOST_CHECK_EQUAL(cfg["music_volume"].str(), "60");
	BOOST_CHECK_EQUAL(cfg["bell_volume"].str(), "50");
}

// Test preferences language
BOOST_AUTO_TEST_CASE( test_preferences_language )
{
	config cfg;
	cfg["locale"] = "en_US";

	BOOST_CHECK_EQUAL(cfg["locale"].str(), "en_US");
}

// Test preferences game settings
BOOST_AUTO_TEST_CASE( test_preferences_game_settings )
{
	config cfg;
	cfg["scroll_speed"] = "50";
	cfg["turbo_speed"] = "2.0";
	cfg["skip_ai_moves"] = "no";

	BOOST_CHECK_EQUAL(cfg["scroll_speed"].str(), "50");
	BOOST_CHECK_EQUAL(cfg["turbo_speed"].str(), "2.0");
	BOOST_CHECK_EQUAL(cfg["skip_ai_moves"].str(), "no");
}

// Test preferences multiplayer
BOOST_AUTO_TEST_CASE( test_preferences_multiplayer )
{
	config cfg;
	cfg["login"] = "player1";
	cfg["remember_password"] = "yes";

	BOOST_CHECK_EQUAL(cfg["login"].str(), "player1");
	BOOST_CHECK_EQUAL(cfg["remember_password"].str(), "yes");
}

// Test preferences advanced
BOOST_AUTO_TEST_CASE( test_preferences_advanced )
{
	config cfg;
	cfg["show_fps"] = "no";
	cfg["show_standing_animations"] = "yes";
	cfg["idle_anim_rate"] = "1.0";

	BOOST_CHECK_EQUAL(cfg["show_fps"].str(), "no");
	BOOST_CHECK_EQUAL(cfg["show_standing_animations"].str(), "yes");
	BOOST_CHECK_EQUAL(cfg["idle_anim_rate"].str(), "1.0");
}

// Test preferences empty
BOOST_AUTO_TEST_CASE( test_preferences_empty )
{
	config cfg;

	BOOST_CHECK(cfg.empty());
	BOOST_CHECK(cfg["fullscreen"].empty());
}

// Test preferences serialization
BOOST_AUTO_TEST_SUITE_END()
