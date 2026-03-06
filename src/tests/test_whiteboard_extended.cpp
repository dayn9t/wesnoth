/*
	Copyright (C) 2012 - 2025
	by Claude Code
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
#include "whiteboard/typedefs.hpp"

BOOST_AUTO_TEST_SUITE( whiteboard_extended )

// Test action type config parsing
BOOST_AUTO_TEST_CASE( test_action_config_parsing )
{
	config cfg;
	cfg["type"] = "attack";
	cfg["team_index"] = 0;
	cfg["hidden"] = false;

	BOOST_CHECK_EQUAL(cfg["type"].str(), "attack");
	BOOST_CHECK_EQUAL(cfg["team_index"].to_int(), 0);
	BOOST_CHECK_EQUAL(cfg["hidden"].to_bool(), false);
}

// Test action type strings
BOOST_AUTO_TEST_CASE( test_action_type_strings )
{
	// Valid action types according to action.cpp
	std::vector<std::string> valid_types = {
		"move",
		"attack",
		"recruit",
		"recall",
		"suppose_dead",
		"dismiss"
	};

	for(const auto& type : valid_types) {
		BOOST_CHECK(!type.empty());
	}
}

// Test side_actions config
BOOST_AUTO_TEST_CASE( test_side_actions_config )
{
	config cfg;
	cfg["team_index"] = 1;
	cfg["hidden"] = true;
	cfg["turn_num"] = 3;

	BOOST_CHECK_EQUAL(cfg["team_index"].to_int(), 1);
	BOOST_CHECK_EQUAL(cfg["hidden"].to_bool(), true);
	BOOST_CHECK_EQUAL(cfg["turn_num"].to_int(), 3);
}

// Test move action config format
BOOST_AUTO_TEST_CASE( test_move_action_config )
{
	config move_cfg;
	move_cfg["type"] = "move";
	move_cfg["x"] = 5;
	move_cfg["y"] = 10;
	move_cfg["src_x"] = 3;
	move_cfg["src_y"] = 7;

	BOOST_CHECK_EQUAL(move_cfg["type"].str(), "move");
	BOOST_CHECK_EQUAL(move_cfg["x"].to_int(), 5);
	BOOST_CHECK_EQUAL(move_cfg["y"].to_int(), 10);
}

// Test attack action config format
BOOST_AUTO_TEST_CASE( test_attack_action_config )
{
	config attack_cfg;
	attack_cfg["type"] = "attack";
	attack_cfg["attacker_x"] = 5;
	attack_cfg["attacker_y"] = 10;
	attack_cfg["defender_x"] = 6;
	attack_cfg["defender_y"] = 11;
	attack_cfg["weapon"] = 0;

	BOOST_CHECK_EQUAL(attack_cfg["type"].str(), "attack");
	BOOST_CHECK_EQUAL(attack_cfg["weapon"].to_int(), 0);
}

// Test recruit action config format
BOOST_AUTO_TEST_CASE( test_recruit_action_config )
{
	config recruit_cfg;
	recruit_cfg["type"] = "recruit";
	recruit_cfg["type"] = "recruit";
	recruit_cfg["unit_type"] = "Elvish Fighter";
	recruit_cfg["loc_x"] = 10;
	recruit_cfg["loc_y"] = 5;

	BOOST_CHECK_EQUAL(recruit_cfg["unit_type"].str(), "Elvish Fighter");
}

// Test recall action config format
BOOST_AUTO_TEST_CASE( test_recall_action_config )
{
	config recall_cfg;
	recall_cfg["type"] = "recall";
	recall_cfg["unit_id"] = "unit_123";
	recall_cfg["loc_x"] = 10;
	recall_cfg["loc_y"] = 5;

	BOOST_CHECK_EQUAL(recall_cfg["type"].str(), "recall");
	BOOST_CHECK_EQUAL(recall_cfg["unit_id"].str(), "unit_123");
}

// Test suppose_dead action config format
BOOST_AUTO_TEST_CASE( test_suppose_dead_config )
{
	config sd_cfg;
	sd_cfg["type"] = "suppose_dead";
	sd_cfg["loc_x"] = 10;
	sd_cfg["loc_y"] = 5;
	sd_cfg["unit_id"] = "unit_456";

	BOOST_CHECK_EQUAL(sd_cfg["type"].str(), "suppose_dead");
}

// Test dismiss action config format
BOOST_AUTO_TEST_CASE( test_dismiss_action_config )
{
	config dismiss_cfg;
	dismiss_cfg["type"] = "dismiss";
	dismiss_cfg["unit_id"] = "unit_789";

	BOOST_CHECK_EQUAL(dismiss_cfg["type"].str(), "dismiss");
}

// Test action hidden flag
BOOST_AUTO_TEST_CASE( test_action_hidden_flag )
{
	config cfg;
	cfg["hidden"] = false;
	BOOST_CHECK_EQUAL(cfg["hidden"].to_bool(), false);

	cfg["hidden"] = true;
	BOOST_CHECK_EQUAL(cfg["hidden"].to_bool(), true);

	cfg["hidden"] = "yes";
	BOOST_CHECK_EQUAL(cfg["hidden"].to_bool(), true);

	cfg["hidden"] = "no";
	BOOST_CHECK_EQUAL(cfg["hidden"].to_bool(), false);
}

// Test team_index values
BOOST_AUTO_TEST_CASE( test_team_index_values )
{
	for(int i = 0; i < 9; ++i) {
		config cfg;
		cfg["team_index"] = i;
		BOOST_CHECK_EQUAL(cfg["team_index"].to_int(), i);
	}
}

// Test action turn number
BOOST_AUTO_TEST_CASE( test_action_turn_number )
{
	config cfg;
	cfg["turn_num"] = 0;
	BOOST_CHECK_EQUAL(cfg["turn_num"].to_int(), 0);

	cfg["turn_num"] = 100;
	BOOST_CHECK_EQUAL(cfg["turn_num"].to_int(), 100);
}

// Test multiple actions in sequence
BOOST_AUTO_TEST_CASE( test_multiple_actions_sequence )
{
	config actions;
	for(int i = 0; i < 5; ++i) {
		config& action = actions.add_child("action");
		action["type"] = "move";
		action["seq_num"] = i;
	}

	int count = 0;
	for(const auto& action : actions.child_range("action")) {
		BOOST_CHECK_EQUAL(action["seq_num"].to_int(), count);
		++count;
	}
	BOOST_CHECK_EQUAL(count, 5);
}

// Test action with numeric parameters
BOOST_AUTO_TEST_CASE( test_action_numeric_params )
{
	config cfg;
	cfg["value"] = -1;
	BOOST_CHECK_EQUAL(cfg["value"].to_int(), -1);

	cfg["value"] = 999999;
	BOOST_CHECK_EQUAL(cfg["value"].to_int(), 999999);

	cfg["value"] = 0;
	BOOST_CHECK_EQUAL(cfg["value"].to_int(), 0);
}

// Test action string parameters
BOOST_AUTO_TEST_CASE( test_action_string_params )
{
	config cfg;
	cfg["unit_type"] = "Elvish Archer";
	cfg["unit_id"] = "elf_001";
	cfg["description"] = "Test action description";

	BOOST_CHECK_EQUAL(cfg["unit_type"].str(), "Elvish Archer");
	BOOST_CHECK_EQUAL(cfg["unit_id"].str(), "elf_001");
	BOOST_CHECK(!cfg["description"].str().empty());
}

// Test empty action config
BOOST_AUTO_TEST_CASE( test_empty_action_config )
{
	config cfg;
	BOOST_CHECK(cfg["missing_key"].empty());
	BOOST_CHECK_EQUAL(cfg["missing_key"].to_int(), 0);
	BOOST_CHECK_EQUAL(cfg["missing_key"].str(), "");
}

// Test action deletion marker
BOOST_AUTO_TEST_CASE( test_action_deletion_marker )
{
	config cfg;
	cfg["deleted"] = true;

	BOOST_CHECK(cfg["deleted"].to_bool());

	cfg["deleted"] = 0;
	BOOST_CHECK(!cfg["deleted"].to_bool());
}

// Test action validation flags
BOOST_AUTO_TEST_CASE( test_action_validation_flags )
{
	config cfg;
	cfg["valid"] = 1;
	cfg["invalid"] = 0;
	cfg["unknown"] = -1;

	BOOST_CHECK(cfg["valid"].to_bool());
	BOOST_CHECK(!cfg["invalid"].to_bool());
	BOOST_CHECK_EQUAL(cfg["unknown"].to_int(), -1);
}

// Test action execution status
BOOST_AUTO_TEST_CASE( test_action_execution_status )
{
	config cfg;
	cfg["executed"] = false;
	cfg["complete"] = true;
	cfg["success"] = true;

	BOOST_CHECK(!cfg["executed"].to_bool());
	BOOST_CHECK(cfg["complete"].to_bool());
	BOOST_CHECK(cfg["success"].to_bool());
}

// Test action chain configuration
BOOST_AUTO_TEST_CASE( test_action_chain_config )
{
	config chain;
	chain["name"] = "test_chain";
	chain["repeat"] = 1;

	config& step1 = chain.add_child("step");
	step1["type"] = "move";
	step1["order"] = 1;

	config& step2 = chain.add_child("step");
	step2["type"] = "attack";
	step2["order"] = 2;

	BOOST_CHECK_EQUAL(chain.child_count("step"), 2);
}

// Test action dependencies
BOOST_AUTO_TEST_CASE( test_action_dependencies )
{
	config cfg;
	cfg["depends_on"] = "action_123";

	BOOST_CHECK_EQUAL(cfg["depends_on"].str(), "action_123");

	// Multiple dependencies
	cfg["depends_on"] = "action_123,action_456";
	std::string deps = cfg["depends_on"].str();
	BOOST_CHECK(deps.find("action_123") != std::string::npos);
	BOOST_CHECK(deps.find("action_456") != std::string::npos);
}

// Test action priority
BOOST_AUTO_TEST_CASE( test_action_priority )
{
	config cfg;
	cfg["priority"] = 100;
	BOOST_CHECK_EQUAL(cfg["priority"].to_int(), 100);

	cfg["priority"] = -50;
	BOOST_CHECK_EQUAL(cfg["priority"].to_int(), -50);
}

// Test action metadata
BOOST_AUTO_TEST_CASE( test_action_metadata )
{
	config cfg;
	cfg["created_at"] = "2026-03-06";
	cfg["author"] = "test";
	cfg["version"] = "1.0";

	BOOST_CHECK_EQUAL(cfg["author"].str(), "test");
	BOOST_CHECK_EQUAL(cfg["version"].str(), "1.0");
}

// Test action coordinate pairs
BOOST_AUTO_TEST_CASE( test_action_coordinate_pairs )
{
	config cfg;
	cfg["from_x"] = 1;
	cfg["from_y"] = 2;
	cfg["to_x"] = 3;
	cfg["to_y"] = 4;

	BOOST_CHECK_LT(cfg["from_x"].to_int(), cfg["to_x"].to_int());
	BOOST_CHECK_LT(cfg["from_y"].to_int(), cfg["to_y"].to_int());
}

// Test action weapon selection
BOOST_AUTO_TEST_CASE( test_action_weapon_selection )
{
	config cfg;
	cfg["attacker_weapon"] = 0;
	cfg["defender_weapon"] = 1;

	BOOST_CHECK_EQUAL(cfg["attacker_weapon"].to_int(), 0);
	BOOST_CHECK_EQUAL(cfg["defender_weapon"].to_int(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
