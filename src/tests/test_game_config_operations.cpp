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

#include "game_config.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(test_game_config_operations)

BOOST_AUTO_TEST_CASE(test_kill_xp_positive)
{
	BOOST_CHECK_GT(game_config::kill_experience, 0);
}

BOOST_AUTO_TEST_CASE(test_combat_xp_positive)
{
	BOOST_CHECK_GT(game_config::combat_experience, 0);
}

BOOST_AUTO_TEST_CASE(test_kill_xp_calculation_level_zero)
{
	BOOST_CHECK_EQUAL(game_config::kill_xp(0), game_config::kill_experience / 2);
}

BOOST_AUTO_TEST_CASE(test_kill_xp_calculation_level_one)
{
	BOOST_CHECK_EQUAL(game_config::kill_xp(1), game_config::kill_experience);
}

BOOST_AUTO_TEST_CASE(test_kill_xp_calculation_level_two)
{
	BOOST_CHECK_EQUAL(game_config::kill_xp(2), game_config::kill_experience * 2);
}

BOOST_AUTO_TEST_CASE(test_combat_xp_calculation)
{
	BOOST_CHECK_EQUAL(game_config::combat_xp(1), game_config::combat_experience);
	BOOST_CHECK_EQUAL(game_config::combat_xp(2), game_config::combat_experience * 2);
	BOOST_CHECK_EQUAL(game_config::combat_xp(3), game_config::combat_experience * 3);
}

BOOST_AUTO_TEST_CASE(test_base_income_non_negative)
{
	BOOST_CHECK_GE(game_config::base_income, 0);
}

BOOST_AUTO_TEST_CASE(test_village_income_positive)
{
	BOOST_CHECK_GT(game_config::village_income, 0);
}

BOOST_AUTO_TEST_CASE(test_village_support_non_negative)
{
	BOOST_CHECK_GE(game_config::village_support, 0);
}

BOOST_AUTO_TEST_CASE(test_recall_cost_non_negative)
{
	BOOST_CHECK_GE(game_config::recall_cost, 0);
}

BOOST_AUTO_TEST_CASE(test_tile_size_positive)
{
	BOOST_CHECK_GT(game_config::tile_size, 0u);
}

BOOST_AUTO_TEST_CASE(test_zoom_levels_not_empty)
{
	BOOST_CHECK(!game_config::zoom_levels.empty());
}

BOOST_AUTO_TEST_CASE(test_gold_carryover_range)
{
	BOOST_CHECK_GE(game_config::gold_carryover_percentage, 0);
	BOOST_CHECK_LE(game_config::gold_carryover_percentage, 100);
}

BOOST_AUTO_TEST_CASE(test_rest_heal_positive)
{
	BOOST_CHECK_GT(game_config::rest_heal_amount, 0);
}

BOOST_AUTO_TEST_CASE(test_poison_amount_non_negative)
{
	BOOST_CHECK_GE(game_config::poison_amount, 0);
}

BOOST_AUTO_TEST_CASE(test_default_terrain_not_empty)
{
	BOOST_CHECK(!game_config::default_terrain.empty());
}

BOOST_AUTO_TEST_CASE(test_title_music_not_empty)
{
	BOOST_CHECK(!game_config::title_music.empty());
}

BOOST_AUTO_TEST_CASE(test_lobby_music_not_empty)
{
	BOOST_CHECK(!game_config::lobby_music.empty());
}

BOOST_AUTO_TEST_CASE(test_defeat_music_not_empty)
{
	BOOST_CHECK(!game_config::default_defeat_music.empty());
}

BOOST_AUTO_TEST_CASE(test_victory_music_not_empty)
{
	BOOST_CHECK(!game_config::default_victory_music.empty());
}

BOOST_AUTO_TEST_SUITE_END()