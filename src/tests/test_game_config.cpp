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
#include "game_config.hpp"
#include "game_version.hpp"
#include <string>
#include <vector>

// ============================================================================
// game_config tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(test_game_config)

BOOST_AUTO_TEST_CASE(test_kill_xp_calculation)
{
	// Test kill XP calculation
	// kill_xp(level) returns kill_experience * level for non-zero level
	// or kill_experience / 2 for level 0

	BOOST_CHECK_GT(game_config::kill_experience, 0);

	// Level 0 units give half kill_experience
	BOOST_CHECK_EQUAL(game_config::kill_xp(0), game_config::kill_experience / 2);

	// Higher level units give more XP
	BOOST_CHECK_GT(game_config::kill_xp(2), game_config::kill_xp(1));
	BOOST_CHECK_GT(game_config::kill_xp(3), game_config::kill_xp(2));
}

BOOST_AUTO_TEST_CASE(test_combat_xp_calculation)
{
	// Test combat XP calculation
	// combat_xp(level) returns combat_experience * level

	BOOST_CHECK_GT(game_config::combat_experience, 0);

	// Verify proportional scaling
	BOOST_CHECK_EQUAL(game_config::combat_xp(1), game_config::combat_experience);
	BOOST_CHECK_EQUAL(game_config::combat_xp(2), game_config::combat_experience * 2);
	BOOST_CHECK_EQUAL(game_config::combat_xp(3), game_config::combat_experience * 3);
}

BOOST_AUTO_TEST_CASE(test_base_income_default)
{
	// Base income should have a reasonable default
	BOOST_CHECK_GE(game_config::base_income, 0);
}

BOOST_AUTO_TEST_CASE(test_village_income_default)
{
	// Village income should be positive
	BOOST_CHECK_GT(game_config::village_income, 0);
}

BOOST_AUTO_TEST_CASE(test_village_support_default)
{
	// Village support should be non-negative
	BOOST_CHECK_GE(game_config::village_support, 0);
}

BOOST_AUTO_TEST_CASE(test_recall_cost_default)
{
	// Recall cost should be reasonable
	BOOST_CHECK_GE(game_config::recall_cost, 0);
}

BOOST_AUTO_TEST_CASE(test_tile_size_default)
{
	// Tile size should be positive
	BOOST_CHECK_GT(game_config::tile_size, 0u);
}

BOOST_AUTO_TEST_CASE(test_zoom_levels_not_empty)
{
	// Zoom levels should not be empty
	BOOST_CHECK(!game_config::zoom_levels.empty());
}

BOOST_AUTO_TEST_CASE(test_gold_carryover_percentage)
{
	// Gold carryover should be between 0 and 100
	BOOST_CHECK_GE(game_config::gold_carryover_percentage, 0);
	BOOST_CHECK_LE(game_config::gold_carryover_percentage, 100);
}

BOOST_AUTO_TEST_CASE(test_server_info_structure)
{
	// Test server_info structure
	game_config::server_info info;
	info.name = "Test Server";
	info.address = "server.example.com:15000";

	BOOST_CHECK_EQUAL(info.name, "Test Server");
	BOOST_CHECK_EQUAL(info.address, "server.example.com:15000");
}

BOOST_AUTO_TEST_CASE(test_server_list_exists)
{
	// Server list should exist (may be empty)
	BOOST_CHECK_NO_THROW(game_config::server_list.size());
}

BOOST_AUTO_TEST_CASE(test_default_terrain_not_empty)
{
	// Default terrain should be set
	BOOST_CHECK(!game_config::default_terrain.empty());
}

BOOST_AUTO_TEST_CASE(test_title_music_exists)
{
	// Title music path should be set
	BOOST_CHECK(!game_config::title_music.empty());
}

BOOST_AUTO_TEST_CASE(test_lobby_music_exists)
{
	// Lobby music path should be set
	BOOST_CHECK(!game_config::lobby_music.empty());
}

BOOST_AUTO_TEST_CASE(test_defeat_music_not_empty)
{
	// Defeat music list should not be empty
	BOOST_CHECK(!game_config::default_defeat_music.empty());
}

BOOST_AUTO_TEST_CASE(test_victory_music_not_empty)
{
	// Victory music list should not be empty
	BOOST_CHECK(!game_config::default_victory_music.empty());
}

BOOST_AUTO_TEST_CASE(test_rest_heal_amount_positive)
{
	// Rest heal should be positive
	BOOST_CHECK_GT(game_config::rest_heal_amount, 0);
}

BOOST_AUTO_TEST_CASE(test_poison_amount)
{
	// Poison damage should be reasonable
	BOOST_CHECK_GE(game_config::poison_amount, 0);
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// version_info tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(test_game_version)

BOOST_AUTO_TEST_CASE(test_version_info_construction)
{
	version_info v("1.16.0");

	BOOST_CHECK_EQUAL(v.major_version(), 1);
	BOOST_CHECK_EQUAL(v.minor_version(), 1);
	BOOST_CHECK_EQUAL(v.revision_level(), 16);
}

BOOST_AUTO_TEST_CASE(test_version_info_comparison)
{
	version_info v1("1.0.0");
	version_info v2("1.0.1");
	version_info v3("1.1.0");
	version_info v4("2.0.0");

	BOOST_CHECK(v1 < v2);
	BOOST_CHECK(v2 < v3);
	BOOST_CHECK(v3 < v4);

	BOOST_CHECK(v4 > v3);
	BOOST_CHECK(v3 > v2);
	BOOST_CHECK(v2 > v1);
}

BOOST_AUTO_TEST_CASE(test_version_info_equality)
{
	version_info v1("1.16.0");
	version_info v2("1.16.0");
	version_info v3("1.16.1");

	BOOST_CHECK(v1 == v2);
	BOOST_CHECK(v1 != v3);
}

BOOST_AUTO_TEST_CASE(test_version_info_string)
{
	version_info v("1.16.2");
	version_info expected("1.16.2");

	BOOST_CHECK(v == expected);
}

BOOST_AUTO_TEST_CASE(test_version_info_with_suffix)
{
	version_info v("1.16.0+dev");

	// Should parse the version part
	BOOST_CHECK_EQUAL(v.major_version(), 1);
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Game config structure tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(test_game_config_structure)

BOOST_AUTO_TEST_CASE(test_config_game_settings)
{
	config game_config;

	game_config["base_income"] = "2";
	game_config["village_income"] = "1";
	game_config["village_support"] = "1";
	game_config["recall_cost"] = "20";

	BOOST_CHECK_EQUAL(game_config["base_income"].str(), "2");
	BOOST_CHECK_EQUAL(game_config["village_income"].str(), "1");
	BOOST_CHECK_EQUAL(game_config["village_support"].str(), "1");
	BOOST_CHECK_EQUAL(game_config["recall_cost"].str(), "20");
}

BOOST_AUTO_TEST_CASE(test_config_server_list)
{
	config servers;

	config& server1 = servers.add_child("server");
	server1["name"] = "Official Server";
	server1["address"] = "server.wesnoth.org:15000";

	config& server2 = servers.add_child("server");
	server2["name"] = "Test Server";
	server2["address"] = "test.wesnoth.org";

	BOOST_CHECK_EQUAL(servers.child_count("server"), 2);
}

BOOST_AUTO_TEST_CASE(test_config_music_settings)
{
	config sounds;

	sounds["title_music"] = "main_menu.ogg";
	sounds["lobby_music"] = "lobby.ogg";

	config& victory = sounds.add_child("victory_music");
	victory["music"] = "victory.ogg";

	config& defeat = sounds.add_child("defeat_music");
	defeat["music"] = "defeat.ogg";

	BOOST_CHECK_EQUAL(sounds["title_music"].str(), "main_menu.ogg");
	BOOST_CHECK(sounds.has_child("victory_music"));
	BOOST_CHECK(sounds.has_child("defeat_music"));
}

BOOST_AUTO_TEST_CASE(test_config_orb_colors)
{
	config colors;

	colors["ally_orb_color"] = "#00FF00";
	colors["enemy_orb_color"] = "#FF0000";
	colors["moved_orb_color"] = "#808080";
	colors["unmoved_orb_color"] = "#FFFFFF";

	BOOST_CHECK_EQUAL(colors["ally_orb_color"].str(), "#00FF00");
	BOOST_CHECK_EQUAL(colors["enemy_orb_color"].str(), "#FF0000");
}

BOOST_AUTO_TEST_CASE(test_config_zoom_levels)
{
	config zoom;

	config& level1 = zoom.add_child("zoom_level");
	level1["factor"] = "0.5";

	config& level2 = zoom.add_child("zoom_level");
	level2["factor"] = "1.0";

	config& level3 = zoom.add_child("zoom_level");
	level3["factor"] = "2.0";

	BOOST_CHECK_EQUAL(zoom.child_count("zoom_level"), 3);
}

BOOST_AUTO_TEST_CASE(test_config_terrain_defaults)
{
	config terrain;

	terrain["default_terrain"] = "Gg";
	terrain["tile_size"] = "72";

	BOOST_CHECK_EQUAL(terrain["default_terrain"].str(), "Gg");
	BOOST_CHECK_EQUAL(terrain["tile_size"].str(), "72");
}

BOOST_AUTO_TEST_SUITE_END()