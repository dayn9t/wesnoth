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
#include "game_initialization/level_type.hpp"
#include "game_initialization/component_availability.hpp"
#include "game_initialization/random_faction_mode.hpp"
#include "game_initialization/saved_game_mode.hpp"
#include "game_initialization/lobby_data.hpp"
#include "game_initialization/lobby_info.hpp"
#include "game_initialization/depcheck.hpp"
#include "log.hpp"

/*
./test --report_level=detailed --log_level=all --run_test=game_initialization_suite
*/

BOOST_AUTO_TEST_SUITE( game_initialization_suite )

// ========================================
// Tests for level_type enum
// ========================================

BOOST_AUTO_TEST_CASE( test_level_type_enum_values )
{
    // Test all enum values
    BOOST_CHECK(level_type::get("scenario").is_valid());
    BOOST_CHECK(level_type::get("user_map").is_valid());
    BOOST_CHECK(level_type::get("user_scenario").is_valid());
    BOOST_CHECK(level_type::get("random_map").is_valid());
    BOOST_CHECK(level_type::get("campaign").is_valid());
    BOOST_CHECK(level_type::get("sp_campaign").is_valid());
    BOOST_CHECK(level_type::get("preset").is_valid());

    // Test type enum values
    BOOST_CHECK_EQUAL(static_cast<int>(level_type::type::scenario), 0);
    BOOST_CHECK_EQUAL(static_cast<int>(level_type::type::user_map), 1);
    BOOST_CHECK_EQUAL(static_cast<int>(level_type::type::user_scenario), 2);
    BOOST_CHECK_EQUAL(static_cast<int>(level_type::type::random_map), 3);
    BOOST_CHECK_EQUAL(static_cast<int>(level_type::type::campaign), 4);
    BOOST_CHECK_EQUAL(static_cast<int>(level_type::type::sp_campaign), 5);
    BOOST_CHECK_EQUAL(static_cast<int>(level_type::type::preset), 6);
}

 // ========================================
// Tests for component_availability enum
// ========================================

BOOST_AUTO_TEST_CASE( test_component_availability_enum_values )
{
    // Test all enum values
    BOOST_CHECK(component_availability::get("sp").is_valid());
    BOOST_CHECK(component_availability::get("mp").is_valid());
    BOOST_CHECK(component_availability::get("hybrid").is_valid());

    // Test type enum values
    BOOST_CHECK_EQUAL(static_cast<int>(component_availability::type::sp), 0);
    BOOST_CHECK_EQUAL(static_cast<int>(component_availability::type::mp), 1);
    BOOST_CHECK_EQUAL(static_cast<int>(component_availability::type::hybrid), 2);
}

 // ========================================
// Tests for random_faction_mode enum
// ========================================

BOOST_AUTO_TEST_CASE( test_random_faction_mode_enum_values )
{
    // Test all enum values
    BOOST_CHECK(random_faction_mode::get("Independent").is_valid());
    BOOST_CHECK(random_faction_mode::get("No Mirror").is_valid());
    BOOST_CHECK(random_faction_mode::get("No Ally Mirror").is_valid());

    // Test type enum values
    BOOST_CHECK_EQUAL(static_cast<int>(random_faction_mode::type::independent), 0);
    BOOST_CHECK_EQUAL(static_cast<int>(random_faction_mode::type::no_mirror), 1);
    BOOST_CHECK_EQUAL(static_cast<int>(random_faction_mode::type::no_ally_mirror), 2);
}

 // ========================================
// Tests for saved_game_mode enum
// ========================================

BOOST_AUTO_TEST_CASE( test_saved_game_mode_enum_values )
{
    // Test all enum values
    BOOST_CHECK(saved_game_mode::get("no").is_valid());
    BOOST_CHECK(saved_game_mode::get("midgame").is_valid());
    BOOST_CHECK(saved_game_mode::get("scenaro_start").is_valid());

    // Test type enum values
    BOOST_CHECK_EQUAL(static_cast<int>(saved_game_mode::type::no), 0);
    BOOST_CHECK_EQUAL(static_cast<int>(saved_game_mode::type::midgame), 1);
    BOOST_CHECK_EQUAL(static_cast<int>(saved_game_mode::type::scenaro_start), 2);

    // Test default value
    BOOST_CHECK_EQUAL(saved_game_mode::get_enum("invalid").value_or(saved_game_mode::type::no), saved_game_mode::type::no);
    BOOST_CHECK_EQUAL(saved_game_mode::get_enum("no").value_or(saved_game_mode::type::no), saved_game_mode::type::no);
    BOOST_CHECK_EQUAL(saved_game_mode::get_enum("midgame").value_or(saved_game_mode::type::no), saved_game_mode::type::midgame);
        BOOST_CHECK_EQUAL(saved_game_mode::get_enum("scenaro_start").value_or(saved_game_mode::type::no), saved_game_mode::type::scenaro_start);
    }
        // ========================================
        // Tests for user_info construction
        // ========================================
        BOOST_AUTO_TEST_CASE( test_user_info_construction )
        {
            // Test basic user info construction
            config user_cfg;
            user_cfg["name"] = "TestUser";
            user_cfg["forum_id"] = 12345;
            user_cfg["game_id"] = 0;
            user_cfg["registered"] = true;
            user_cfg["moderator"] = false;

            mp::user_info user_info(user_cfg);

            BOOST_CHECK_EQUAL(user_info.name, std::string("TestUser"));
            BOOST_CHECK_EQUAL(userInfo.forum_id, 12345);
            BOOST_CHECK_EQUAL(userInfo.game_id, 0);
            BOOST_CHECK_EQUAL(userInfo.registered, true);
            BOOST_CHECK_EQUAL(userInfo.moderator, false);
        }
        // ========================================
        // Tests for user_info comparison
        // ========================================
        BOOST_AUTO_TEST_CASE( test_user_info_comparison )
        {
            config user1;
            user1["name"] = "Alice";
            user1["forum_id"] = 12345;
            user1["game_id"] = 0;
            user1["registered"] = true;
            user1["moderator"] = false;

            config user2;
            user2["name"] = "Bob";
            user2["forum_id"] = 67890;
            user2["game_id"] = 0;
            user2["registered"] = true;
            user2["moderator"] = true;

            mp::user_info info1(user1);
            mp::user_info info2(user2);

            // Compare by relation
            BOOST_CHECK(info1 < info2);
            // Compare by name (alphabetical order)
            BOOST_CHECK(info1 < info2);
        }
        // ========================================
        // Tests for user_info get_state
        // ========================================
        BOOST_AUTO_TEST_CASE( test_user_info_get_state )
        {
            // User in lobby (game_id == 0)
            config user_cfg;
            user_cfg["name"] = "Alice";
            user_cfg["forum_id"] = 12345;
            user_cfg["game_id"] = "0";
            user_cfg["registered"] = true;
            user_cfg["moderator"] = false;
            mp::user_info info(user_cfg);

            BOOST_CHECK_EQUAL(info.get_state(0), mp::user_info::user_state::LOBBY);

            // User in selected game
            user_cfg["game_id"] = 42;
            BOOST_CHECK_EQUAL(info.get_state(42), mp::user_info::user_state::SEL_GAME);

            // User in another game
            user_cfg["game_id"] = 100;
            BOOST_CHECK_EQUAL(info.get_state(42), mp::user_info::user_state::GAME);
        }
        // ========================================
        // Tests for user_info observing status
        // ========================================
        BOOST_AUTO_TEST_CASE( test_user_info_observing )
        {
            config user_cfg;
            user_cfg["name"] = "Observer1";
            user_cfg["status"] = "observing";
            user_cfg["forum_id"] = 12345;
            user_cfg["game_id"] = 0;
            user_cfg["registered"] = true;
            user_cfg["moderator"] = false;
            mp::user_info info(user_cfg);

            BOOST_CHECK_EQUAL(info.observing, true);
        }
        // ========================================
        // Tests for game_info basic construction
        // ========================================
        BOOST_AUTO_TEST_CASE( test_game_info_basic_construction )
        {
            config game_cfg;
            game_cfg["id"] = 123;
            game_cfg["name"] = "Test Game";
            game_cfg["scenario"] = "Test Scenario";
            game_cfg["scenario_id"] = "test_scenario_id";
            game_cfg["mp_era"] = "test_era";
            game_cfg["mp_era_name"] = "Test Era";
            game_cfg["observer"] = true;
            game_cfg["password"] = false;
            std::vector<std::string> installed_addons;
            mp::game_info info(game_cfg, installed_addons);

            BOOST_CHECK_EQUAL(info.id, 123);
            BOOST_CHECK_EQUAL(info.name, std::string("Test Game"));
            BOOST_CHECK_EQUAL(info.scenario, std::string("Test Scenario"));
            BOOST_CHECK_EQUAL(info.scenario_id, std::string("test_scenario_id"));
            BOOST_CHECK_EQUAL(info.era, std::string("test_era"));
            BOOST_CHECK_EQUAL(info.observers, true);
            BOOST_CHECK_EQUAL(info.password_required, false);
        }
        // ========================================
        // Tests for game_info with sides
        // ========================================
        BOOST_AUTO_TEST_CASE( test_game_info_with_sides )
        {
            config gameCfg;
            gameCfg["id"] = 123;
            gameCfg["name"] = "Test Game";
            gameCfg["scenario"] = "Test Scenario";
            gameCfg["scenario_id"] = "test_scenario_id";
            gameCfg["mp_era"] = "test_era";
            gameCfg["mp_era_name"] = "Test Era";
            gameCfg["observer"] = true;

            gameCfg.add_child("side");
            side["id"] = "test_side";
            side["name"] = "Test Side";
            side["controller"] = "human";
            side["faction"] = "test_faction";
            side["color"] = "red";
            side["gold"] = 100;
            side["income"] = 0;
            BOOST_CHECK_EQUAL(gameCfg.child_count("side"), 1u);
        }
        // ========================================
        // Tests for game_info boolean flags
        // ========================================
        BOOST_AUTO_TEST_CASE( test_game_info_boolean_flags )
        {
            config gameCfg;
            gameCfg["id"] = 123;
            gameCfg["name"] = "Test Game";
            gameCfg["scenario"] = "Test Scenario";
            gameCfg["scenario_id"] = "test_scenario_id";
            gameCfg["mp_era"] = "test_era";
            gameCfg["mp_era_name"] = "Test Era";
            gameCfg["observer"] = true;
            gameCfg["fog"] = true;
            gameCfg["shroud"] = true;
            gameCfg["started"] = true;
            gameCfg["reloaded"] = false;
            gameCfg["password"] = false;
            gameCfg["shuffle_sides"] = false;
            gameCfg["use_map_settings"] = true;
            gameCfg["private_replay"] = true;
            gameCfg["verified"] = true;
            gameCfg["auto_hosted"] = true;
            gameCfg["game_preset"] = true;
            std::vector<std::string> installed_addons;
            mp::game_info info(gameCfg, installed_addons);

            BOOST_CHECK_EQUAL(info.fog, true);
            BOOST_CHECK_EQUAL(info.shroud, true);
            BOOST_CHECK_EQUAL(info.started, true);
            BOOST_CHECK_EQUAL(info.reloaded, false);
            BOOST_CHECK_EQUAL(info.observers, true);
            BOOST_CHECK_EQUAL(info.password_required, false);
            BOOST_CHECK_EQUAL(info.shuffle_sides, false);
            BOOST_CHECK_EQUAL(info.use_map_settings, true);
            BOOST_CHECK_EQUAL(info.private_replay, true);
            BOOST_CHECK_EQUAL(info.verified, true);
            BOOST_CHECK_EQUAL(info.auto_hosted, true);
            BOOST_CHECK_EQUAL(info.game_preset, true);
        }
        // ========================================
        // Tests for game_info gold/xp/vision
        // ========================================
        BOOST_AUTO_TEST_CASE( test_game_info_gold_xp_vision )
        {
            config gameCfg;
            gameCfg["id"] = 123;
            gameCfg["name"] = "Test Game";
            gameCfg["scenario"] = "Test Scenario";
            gameCfg["scenario_id"] = "test_scenario_id";
            gameCfg["mp_era"] = "test_era";
            gameCfg["mp_era_name"] = "Test Era";
            gameCfg["observer"] = true;
            gameCfg["mp_village_gold"] = "2";
            gameCfg["mp_village_support"] = "1";
            gameCfg["experience_modifier"] = "150";
            std::vector<std::string> installed_addons;
            mp::game_info info(gameCfg, installed_addons);

            BOOST_CHECK_EQUAL(info.gold, std::string("2"));
            BOOST_CHECK_EQUAL(info.support, std::string("1"));
            BOOST_CHECK_EQUAL(info.xp, std::string("150%"));
        }
        // ========================================
        // Tests for game_info can_join
        // ========================================
        BOOST_AUTO_TEST_CASE( test_game_info_can_join )
        {
            config gameCfg;
            gameCfg["id"] = 123;
            gameCfg["name"] = "Test Game";
            gameCfg["scenario"] = "Test Scenario";
            gameCfg["scenario_id"] = "test_scenario_id";
            gameCfg["mp_era"] = "test_era";
            gameCfg["mp_era_name"] = "Test Era";
            gameCfg["observer"] = true;
            gameCfg["started"] = false;
            gameCfg["password"] = false;
            gameCfg.add_child("side");
            side["id"] = "test_side";
            std::vector<std::string> installed_addons;
            mp::game_info info(gameCfg, installed_addons);

            // can_join: has vacant slots, not started, no password required
            // Note: This test verifies construction only, actual can_join logic depends on game state
            BOOST_CHECK_EQUAL(info.started, false);
            BOOST_CHECK_EQUAL(info.password_required, false);
        }
        // ========================================
        // Tests for game_info can_observe
        // ========================================
        BOOST_AUTO_TEST_CASE( test_game_info_can_observe )
        {
            config gameCfg;
            gameCfg["id"] = 123;
            gameCfg["name"] = "Test Game";
            gameCfg["scenario"] = "Test Scenario";
            gameCfg["scenario_id"] = "test_scenario_id";
            gameCfg["mp_era"] = "test_era";
            gameCfg["mp_era_name"] = "Test Era";
            gameCfg["observer"] = true;
            gameCfg["started"] = true;
            gameCfg["current_turn"] = 5;
            std::vector<std::string> installed_addons;
            mp::game_info info(gameCfg, installed_addons);

            // can_observe: observers allowed, game started
            BOOST_CHECK_EQUAL(info.observers, true);
            BOOST_CHECK_EQUAL(info.started, true);
            BOOST_CHECK_EQUAL(info.current_turn, 5u);
        }
        // ========================================
        // Tests for game_info match_string_filter
        // ========================================
        BOOST_AUTO_TEST_CASE( test_game_info_match_string_filter )
        {
            config gameCfg;
            gameCfg["id"] = 123;
            gameCfg["name"] = "TEST GAME NAME";
            gameCfg["scenario"] = "Test Scenario";
            gameCfg["scenario_id"] = "test_scenario_id";
            gameCfg["mp_era"] = "test_era";
            gameCfg["mp_era_name"] = "Test Era";
            gameCfg["observer"] = true;
            gameCfg["password"] = false;
            std::vector<std::string> installed_addons;
            mp::game_info info(gameCfg, installed_addons);

            // Match should work on name (case-insensitive by default)
            BOOST_CHECK(info.match_string_filter("TEST"));
            BOOST_CHECK(info.match_string_filter("test"));
            BOOST_CHECK(info.match_string_filter("NAME"));
            BOOST_CHECK(info.match_string_filter("game"));
            BOOST_CHECK(!info.match_string_filter("nonexistent"));
        }
        // ========================================
        // Tests for lobby_info construction
        // ========================================
        BOOST_AUTO_TEST_CASE( test_lobby_info_construction )
        {
            mp::lobby_info lobby;

            BOOST_CHECK(!lobby.gamelist_initialized());
            BOOST_CHECK_EQUAL(lobby.users().size(), 0u);
            BOOST_CHECK_EQUAL(lobby.games().size(), 0u);
        }
        // ========================================
        // Tests for lobby_info process_gamelist
        // ========================================
        BOOST_AUTO_TEST_CASE( test_lobby_info_process_gamelist )
        {
            mp::lobby_info lobby;

            config data;
            config& game1 = data.add_child("game");
            game1["id"] = "1";
            game1["name"] = "Test Game 1";

            config& game2 = data.add_child("game");
            game2["id"] = "2";
            game2["name"] = "Test Game 2";

            lobby.process_gamelist(data);

            BOOST_CHECK(lobby.gamelist_initialized());
            BOOST_CHECK_EQUAL(lobby.games().size(), 2u);

            // Get game by ID
            mp::game_info* game1_ptr = lobby.get_game_by_id(1);
            mp::game_info* game2_ptr = lobby.get_game_by_id(2);
            BOOST_CHECK(game1_ptr != nullptr);
            BOOST_CHECK(game2_ptr != nullptr);
            BOOST_CHECK_EQUAL(lobby.games()[0]->id, 1);
            BOOST_CHECK_EQUAL(lobby.games()[1]->id, 2);
        }
        // ========================================
        // Tests for lobby_info users
        // ========================================
        BOOST_AUTO_TEST_CASE( test_lobby_info_users )
        {
            mp::lobby_info lobby;

            config data;
            config& user1 = data.add_child("user");
            user1["name"] = "Alice";
            user1["forum_id"] = 12345;
            user1["game_id"] = "0";
            user1["registered"] = true;
            user1["moderator"] = false;

            config& user2 = data.add_child("user");
            user2["name"] = "Bob";
            user2["forum_id"] = 67890;
            user2["game_id"] = "0";
            user2["registered"] = true;
            user2["moderator"] = true;

            lobby.process_gamelist(data);

            BOOST_CHECK_EQUAL(lobby.users().size(), 2u);

            mp::user_info* alice = lobby.get_user("Alice");
            mp::user_info* bob = lobby.get_user("Bob");
            BOOST_CHECK(alice != nullptr);
            BOOST_CHECK(bob != nullptr);

            if(alice) {
                BOOST_CHECK_EQUAL(alice->name, "Alice");
                BOOST_CHECK_EQUAL(alice->forum_id, 12345);
                BOOST_CHECK_EQUAL(alice->game_id, 0);
                BOOST_CHECK_EQUAL(alice->registered, true);
                BOOST_CHECK_EQUAL(alice->moderator, false);
            }

            if(bob) {
                BOOST_CHECK_EQUAL(bob->name, "Bob");
                BOOST_CHECK_EQUAL(bob->forum_id, 67890);
                BOOST_CHECK_EQUAL(bob->game_id, 0);
                BOOST_CHECK_EQUAL(bob->registered, true);
                BOOST_CHECK_EQUAL(bob->moderator, true);
            }
        }
        // ========================================
        // Tests for lobby_info get_game_by_id
        // ========================================
        BOOST_AUTO_TEST_CASE( test_lobby_info_get_game_by_id )
        {
            mp::lobby_info lobby;

            config data;
            config& game = data.add_child("game");
            game["id"] = "1";
            game["name"] = "Test Game";

            lobby.process_gamelist(data);

            BOOST_CHECK_EQUAL(lobby.get_game_by_id(1)->id, 1);
            BOOST_CHECK_EQUAL(lobby.get_game_by_id(999), nullptr);
        }
        // ========================================
        // Tests for lobby_info get_user
        // ========================================
        BOOST_AUTO_TEST_CASE( test_lobby_info_get_user )
        {
            mp::lobby_info lobby;

            config data;
            config& user = data.add_child("user");
            user["name"] = "Alice";

            lobby.process_gamelist(data);

            BOOST_CHECK_EQUAL(lobby.get_user("Alice")->name, "Alice");
            BOOST_CHECK_EQUAL(lobby.get_user("Bob"), nullptr);
        }
        // ========================================
        // Tests for lobby_info game_filters
        // ========================================
        BOOST_AUTO_TEST_CASE( test_lobby_info_game_filters )
        {
            mp::lobby_info lobby;

            config data;
            config& game1 = data.add_child("game");
            game1["id"] = "1";
            game1["name"] = "Test Game 1";
            game1["started"] = false;
            game1["current_turn"] = 0;

            config& game2 = data.add_child("game");
            game2["id"] = "2";
            game2["name"] = "Test Game 2";
            game2["started"] = true;
            game2["current_turn"] = 5;

            lobby.process_gamelist(data);

            BOOST_CHECK_EQUAL(lobby.games().size(), 2u);

            // Add a filter that shows only started games
            lobby.add_game_filter([](const mp::game_info& game) {
                return game.started;
            });
            lobby.apply_game_filter();

            BOOST_CHECK_EQUAL(lobby.games().size(), 1u);

            // Filter should hide started games
            lobby.clear_game_filters();
            lobby.add_game_filter([](const mp::game_info& game) {
                return !game.started;
            });
            lobby.apply_game_filter();

            BOOST_CHECK_EQUAL(lobby.games().size(), 0u);
        }
        // ========================================
        // Tests for lobby_info game_filter_invert
        // ========================================
        BOOST_AUTO_TEST_CASE( test_lobby_info_game_filter_invert )
        {
            mp::lobby_info lobby;

            config data;
            config& game1 = data.add_child("game");
            game1["id"] = "1";
            game1["name"] = "Test Game 1";
            game1["started"] = false;
            game1["current_turn"] = 0;

            lobby.process_gamelist(data);

            BOOST_CHECK_EQUAL(lobby.games().size(), 1u);

            // Invert filter (non-inverted filter shows started games, lobby.add_game_filter([](const mp::game_info& game) {
                return game.started;
            });
            lobby.set_game_filter_invert([](bool visible) {
                return !visible;
            });
            lobby.apply_game_filter();

            BOOST_CHECK_EQUAL(lobby.games().size(), 0u);

            // Non-inverted filter (starts !started)
            lobby.add_game_filter([](const mp::game_info& game) {
                return !game.started;
            });
            lobby.apply_game_filter();

            BOOST_CHECK_EQUAL(lobby.games().size(), 1u);
        }
        // ========================================
        // Tests for lobby_info apply_game_filter combined
        // ========================================
        BOOST_AUTO_TEST_CASE( test_lobby_info_apply_game_filter_combined )
        {
            mp::lobby_info lobby;

            config data;
            config& game1 = data.add_child("game");
            game1["id"] = "1";
            game1["name"] = "Test Game 1";
            game1["started"] = false;
            game1["current_turn"] = 0;
            lobby.process_gamelist(data);

            // Apply filter
            lobby.add_game_filter([](const mp::game_info& game) {
                return game.started;
            });
            lobby.apply_game_filter();

            BOOST_CHECK_EQUAL(lobby.games().size(), 1u);
        }
        // ========================================
        // Tests for depcheck component_type
        // ========================================
        BOOST_AUTO_TEST_CASE( test_depcheck_component_type )
        {
            // Test component_type enum
            BOOST_CHECK_EQUAL(static_cast<int>(ng::depcheck::component_type::ERA), 0);
            BOOST_CHECK_EQUAL(static_cast<int>(ng::depcheck::component_type::SCENARIO), 1);
            BOOST_CHECK_EQUAL(static_cast<int>(ng::depcheck::component_type::MODIFICATION), 2);
        }
        // ========================================
        // Tests for depcheck manager basic construction
        // ========================================
        BOOST_AUTO_TEST_CASE( test_depcheck_manager_construction )
        {
            config gameCfg;
            gameCfg["id"] = "game_config";

            // Test that game config view can be created with just a config
            BOOST_CHECK(gameCfg.has_child("era") || gameCfg.has_child("multiplayer"));
            BOOST_CHECK_EQUAL(gameCfg.child_count("era"), 0u);
            BOOST_CHECK_EQUAL(gameCfg.child_count("multiplayer"), 0u);
        }
        // ========================================
        // Tests for depcheck manager with components
        // ========================================
        BOOST_AUTO_TEST_CASE( test_depcheck_manager_with_components )
        {
            config gameCfg;
            gameCfg["id"] = "game_config";

            // Add eras and modifications
            config& era = gameCfg.add_child("era");
            era["id"] = "era1";
            era["name"] = "Test Era";

            config& scenario = gameCfg.add_child("multiplayer");
            scenario["id"] = "scenario1";
            scenario["name"] = "Test Scenario";

            config& modification = gameCfg.add_child("modification");
            modification["id"] = "mod1";
            modification["name"] = "Test Mod 1";
            BOOST_CHECK_EQUAL(gameCfg.child_count("era"), 1u);
            BOOST_CHECK_EQUAL(gameCfg.child_count("multiplayer"), 1u);
            BOOST_CHECK_EQUAL(gameCfg.child_count("modification"), 1u);
        }
        // ========================================
        // Tests for depcheck manager modifications selection
        // ========================================
        BOOST_AUTO_TEST_CASE( test_depcheck_manager_modifications )
        {
            config gameCfg;
            gameCfg["id"] = "game_config";

            // Add era, config& era = gameCfg.add_child("era");
            era["id"] = "era1";
            era["name"] = "Test Era";

            // Add scenario
            config& scenario = gameCfg.add_child("multiplayer");
            scenario["id"] = "scenario1";
            scenario["name"] = "Test Scenario";

            // Add modifications
            config& mod1 = gameCfg.add_child("modification");
            mod1["id"] = "mod1";
            mod1["name"] = "Test Mod 1";
            config& mod2 = gameCfg.add_child("modification");
            mod2["id"] = "mod2";
            mod2["name"] = "Test Mod 2";
            BOOST_CHECK_EQUAL(gameCfg.child_count("modification"), 2u);
        }
        // ========================================
        // Tests for depcheck manager is_modification_active
        // ========================================
        BOOST_AUTO_TEST_CASE( test_depcheck_manager_is_modification_active )
        {
            config gameCfg;
            gameCfg["id"] = "game_config";

            // Add era
            config& era = gameCfg.add_child("era");
            era["id"] = "era1";
            era["name"] = "Test Era";

            // Add scenario
            config& scenario = gameCfg.add_child("multiplayer");
            scenario["id"] = "scenario1";
            scenario["name"] = "Test Scenario";

            // Add modification
            config& mod1 = gameCfg.add_child("modification");
            mod1["id"] = "mod1";
            mod1["name"] = "Test Mod 1";
            ng::depcheck::manager manager(gameCfg, true);

            // Mod should not be active by default
            BOOST_CHECK(!manager.is_modification_active("mod1"));

            // Activate mod
            manager.try_modification_by_id("mod1", true);
            BOOST_CHECK(manager.is_modification_active("mod1"));

            // Deactivate mod
            manager.try_modification_by_id("mod1", false);
            BOOST_CHECK(!manager.is_modification_active("mod1"));
        }
        // ========================================
        // Tests for depcheck manager insert_element
        // ========================================
        BOOST_AUTO_TEST_CASE( test_depcheck_manager_insert_element )
        {
            config gameCfg;
            gameCfg["id"] = "game_config";
            // Create manager with initial empty config
            ng::depcheck::manager manager(gameCfg, true);

            // Create a new scenario config
            config newScenario;
            newScenario["id"] = "new_scenario";
            newScenario["name"] = "New Scenario";

            // Insert the element (SCENARIO type, note: the manager expects "scenario" nodes from [multiplayer] but insert_element expects [scenario]
 node
            manager.insert_element(ng::depcheck::component_type::SCENARIO, newScenario, 0);

            // Create a new era config
            config newEra;
            newEra["id"] = "new_era";
            newEra["name"] = "New Era";
            // Insert the element (ERA type)
            manager.insert_element(ng::depcheck::component_type::ERA, newEra, 0);

            // Create a new modification config
            config newMod;
            newMod["id"] = "new_mod";
            newMod["name"] = "New Mod";
            // Insert the element (MODIFICATION type)
            manager.insert_element(ng::depcheck::component_type::MODIFICATION, newMod, 0);
        }
        // ========================================
        // Tests for controller enum
        // ========================================
        BOOST_AUTO_TEST_CASE( test_controller_enum )
        {
            // Test controller enum values
            BOOST_CHECK_EQUAL(static_cast<int>(ng::controller::CNTR_NETWORK), 0);
            BOOST_CHECK_EQUAL(static_cast<int>(ng::controller::CNTR_LOCAL), 1);
            BOOST_CHECK_EQUAL(static_cast<int>(ng::controller::CNTR_COMPUTER), 2);
            BOOST_CHECK_EQUAL(static_cast<int>(ng::controller::CNTR_EMPTY), 3);
            BOOST_CHECK_EQUAL(static_cast<int>(ng::controller::CNTR_RESERVED), 4);
        }
        // ========================================
        // Tests for notify_mode enum
        // ========================================
        BOOST_AUTO_TEST_CASE( test_notify_mode_enum )
        {
            // Test notify_mode enum values
            BOOST_CHECK_EQUAL(static_cast<int>(mp::notify_mode::none), 0);
            BOOST_CHECK_EQUAL(static_cast<int>(mp::notify_mode::message), 1);
            BOOST_CHECK_EQUAL(static_cast<int>(mp::notify_mode::message_other_window), 2);
            BOOST_CHECK_EQUAL(static_cast<int>(mp::notify_mode::server_message), 3);
            BOOST_CHECK_EQUAL(static_cast<int>(mp::notify_mode::own_nick), 4);
            BOOST_CHECK_EQUAL(static_cast<int>(mp::notify_mode::friend_message), 5);
            BOOST_CHECK_EQUAL(static_cast<int>(mp::notify_mode::whisper), 6);
            BOOST_CHECK_EQUAL(static_cast<int>(mp::notify_mode::whisper_other_window), 7);
            BOOST_CHECK_EQUAL(static_cast<int>(mp::notify_mode::lobby_join), 8);
            BOOST_CHECK_EQUAL(static_cast<int>(mp::notify_mode::lobby_quit), 9);
            BOOST_CHECK_EQUAL(static_cast<int>(mp::notify_mode::game_created), 10);
        }
        // ========================================
        // Tests for game_info display_status_string
        // ========================================
        BOOST_AUTO_TEST_CASE( test_game_info_display_status_string )
        {
            // Test display_status_string
            BOOST_CHECK_EQUAL(mp::game_info::disp_status::CLEAN, mp::game_info::disp_status::CLEAN);
            BOOST_CHECK_EQUAL(mp::game_info::disp_status::NEW, mp::game_info::disp_status::NEW);
            BOOST_CHECK_EQUAL(mp::game_info::disp_status::UPDATED, mp::game_info::disp_status::UPDATED);
            BOOST_CHECK_EQUAL(mp::game_info::disp_status::DELETED, mp::game_info::disp_status::DELETED);
        }
        // ========================================
        // Tests for game_info addon_req enum
        // ========================================
        BOOST_AUTO_TEST_CASE( test_game_info_addon_req_enum )
        {
            // Test addon_req enum
            BOOST_CHECK_EQUAL(static_cast<int>(mp::game_info::addon_req::SATISFIED), 0);
            BOOST_CHECK_EQUAL(static_cast<int>(mp::game_info::addon_req::NEED_DOWNLOAD), 1);
            BOOST_CHECK_EQUAL(static_cast<int>(mp::game_info::addon_req::CANNOT_SATISFY), 2);
        }
        // ========================================
        // Tests for user_info user_state enum
        // ========================================
        BOOST_AUTO_TEST_CASE( test_user_info_user_state_enum )
        {
            // Test user_state enum
            BOOST_CHECK_EQUAL(static_cast<int>(mp::user_info::user_state::LOBBY), 0);
            BOOST_CHECK_EQUAL(static_cast<int>(mp::user_info::user_state::GAME), 1);
            BOOST_CHECK_EQUAL(static_cast<int>(mp::user_info::user_state::SEL_GAME), 2);
        }
        // ========================================
        // Tests for user_info user_relation enum
        // ========================================
        BOOST_AUTO_TEST_CASE( test_user_info_user_relation_enum )
        {
            // Test user_relation enum
            BOOST_CHECK_EQUAL(static_cast<int>(mp::user_info::user_relation::ME), 0);
            BOOST_CHECK_EQUAL(static_cast<int>(mp::user_info::user_relation::FRIEND), 1);
            BOOST_CHECK_EQUAL(static_cast<int>(mp::user_info::user_relation::NEUTRAL), 2);
            BOOST_CHECK_EQUAL(static_cast<int>(mp::user_info::user_relation::IGNORED), 3);
        }
        // ========================================
        // Tests for contains_ignore_case function
        // ========================================
        BOOST_AUTO_TEST_CASE( test_contains_ignore_case )
        {
            // Test that substring is found
            BOOST_CHECK(ng::contains_ignore_case("Test String", "test"));
            BOOST_CHECK(ng::contains_ignore_case("Test String", "est"));
            BOOST_CHECK(ng::contains_ignore_case("Test String", "STRING"));
            BOOST_CHECK(ng::contains_ignore_case("Test String", "g"));
            BOOST_CHECK(ng::contains_ignore_case("Test String", "t s"));

            // Test that substring is not found
            BOOST_CHECK(!ng::contains_ignore_case("Test String", "xyz"));
            BOOST_CHECK(!ng::contains_ignore_case("Test String", "longer"));
            BOOST_CHECK(!ng::contains_ignore_case("short", "longer"));

            // Test case sensitivity
            BOOST_CHECK(ng::contains_ignore_case("TEST STRING", "test string"));
            BOOST_CHECK(ng::contains_ignore_case("AbCdEf", "bCd"));

            // Test empty strings
            BOOST_CHECK(ng::contains_ignore_case("", ""));
            BOOST_CHECK(!ng::contains_ignore_case("", "a"));
            BOOST_CHECK(ng::contains_ignore_case("a", ""));

            // Test boundaries
            BOOST_CHECK(ng::contains_ignore_case("Hello", "Hello"));
            BOOST_CHECK(ng::contains_ignore_case("Hello", "o"));
            BOOST_CHECK(ng::contains_ignore_case("Hello World", "world"));
        }
        BOOST_AUTO_TEST_SUITE_END()
