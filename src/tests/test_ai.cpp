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

#include "ai/configuration.hpp"
#include "ai/game_info.hpp"
#include "ai/composite/ai.hpp"
#include "ai/composite/rca.hpp"
#include "config.hpp"
#include "map/location.hpp"
#include "pathfind/pathfind.hpp"

BOOST_AUTO_TEST_SUITE( ai )

// Test AI configuration parsing
BOOST_AUTO_TEST_CASE( test_ai_configuration_basic )
{
	config cfg;
	cfg["id"] = "test_ai";
	cfg["description"] = "Test AI";
	cfg["version"] = "1.0";

	BOOST_CHECK_EQUAL(cfg["id"].str(), "test_ai");
	BOOST_CHECK_EQUAL(cfg["description"].str(), "Test AI");
	BOOST_CHECK_EQUAL(cfg["version"].str(), "1.0");
}

// Test AI configuration with aspects
BOOST_AUTO_TEST_CASE( test_ai_configuration_aspects )
{
	config cfg;
	config& aspect = cfg.add_child("aspect");
	aspect["id"] = "aggression";
	aspect["value"] = "0.5";

	BOOST_CHECK(cfg.has_child("aspect"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("aspect")["id"].str(), "aggression");
	BOOST_CHECK_EQUAL(cfg.mandatory_child("aspect")["value"].str(), "0.5");
}

// Test AI configuration with goals
BOOST_AUTO_TEST_CASE( test_ai_configuration_goals )
{
	config cfg;
	config& goal = cfg.add_child("goal");
	goal["name"] = "protect_leader";
	goal["value"] = "100";

	BOOST_CHECK(cfg.has_child("goal"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("goal")["name"].str(), "protect_leader");
	BOOST_CHECK_EQUAL(cfg.mandatory_child("goal")["value"].str(), "100");
}

// Test AI configuration with stages
BOOST_AUTO_TEST_CASE( test_ai_configuration_stages )
{
	config cfg;
	config& stage = cfg.add_child("stage");
	stage["id"] = "main_loop";
	stage["name"] = "Main Loop";

	BOOST_CHECK(cfg.has_child("stage"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("stage")["id"].str(), "main_loop");
	BOOST_CHECK_EQUAL(cfg.mandatory_child("stage")["name"].str(), "Main Loop");
}

// Test AI configuration with multiple aspects
BOOST_AUTO_TEST_CASE( test_ai_configuration_multiple_aspects )
{
	config cfg;

	config& aspect1 = cfg.add_child("aspect");
	aspect1["id"] = "aggression";
	aspect1["value"] = "0.5";

	config& aspect2 = cfg.add_child("aspect");
	aspect2["id"] = "caution";
	aspect2["value"] = "0.3";

	BOOST_CHECK_EQUAL(cfg.child_count("aspect"), 2u);
}

// Test AI configuration empty
BOOST_AUTO_TEST_CASE( test_ai_configuration_empty )
{
	config cfg;

	BOOST_CHECK(cfg.empty());
	BOOST_CHECK_EQUAL(cfg.child_count("aspect"), 0u);
	BOOST_CHECK_EQUAL(cfg.child_count("goal"), 0u);
	BOOST_CHECK_EQUAL(cfg.child_count("stage"), 0u);
}

// Test AI configuration clear
BOOST_AUTO_TEST_CASE( test_ai_configuration_clear )
{
	config cfg;
	cfg["id"] = "test_ai";
	cfg.add_child("aspect");

	BOOST_CHECK(!cfg.empty());

	cfg.clear();

	BOOST_CHECK(cfg.empty());
	BOOST_CHECK_EQUAL(cfg.child_count("aspect"), 0u);
}

// ============================================================================
// AI Composite Component Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_ai_path_element_default )
{
	ai::path_element elem;

	BOOST_CHECK_EQUAL(elem.property, "");
	BOOST_CHECK_EQUAL(elem.id, "");
	BOOST_CHECK_EQUAL(elem.position, 0);
}

BOOST_AUTO_TEST_CASE( test_ai_path_element_assignment )
{
	ai::path_element elem;
	elem.property = "test_property";
	elem.id = "test_id";
	elem.position = 5;

	BOOST_CHECK_EQUAL(elem.property, "test_property");
	BOOST_CHECK_EQUAL(elem.id, "test_id");
	BOOST_CHECK_EQUAL(elem.position, 5);
}

// ============================================================================
// AI Aspect Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_ai_aspect_map_empty )
{
	ai::aspect_map aspects;

	BOOST_CHECK(aspects.empty());
	BOOST_CHECK_EQUAL(aspects.size(), 0u);
}

BOOST_AUTO_TEST_CASE( test_ai_known_aspect_map_empty )
{
	ai::known_aspect_map aspects;

	BOOST_CHECK(aspects.empty());
	BOOST_CHECK_EQUAL(aspects.size(), 0u);
}

// ============================================================================
// AI Result Type Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_ai_result_type_size )
{
	// Verify result types are integral
	BOOST_CHECK_EQUAL(sizeof(ai::side_number), sizeof(int));
}

BOOST_AUTO_TEST_CASE( test_ai_move_map_default_construction )
{
	ai::move_map moves;

	BOOST_CHECK(moves.empty());
	BOOST_CHECK_EQUAL(moves.size(), 0u);
}

BOOST_AUTO_TEST_CASE( test_ai_move_map_single_move )
{
	ai::move_map moves;
	map_location from(3, 4);
	map_location to(5, 6);

	moves.insert(std::make_pair(from, to));

	BOOST_CHECK_EQUAL(moves.size(), 1u);
	BOOST_CHECK_EQUAL(moves.count(from), 1u);
	BOOST_CHECK_EQUAL(moves.begin()->second, to);
}

BOOST_AUTO_TEST_CASE( test_ai_move_map_multiple_same_source )
{
	ai::move_map moves;
	map_location from(0, 0);
	map_location to1(1, 0);
	map_location to2(0, 1);
	map_location to3(1, 1);

	moves.insert(std::make_pair(from, to1));
	moves.insert(std::make_pair(from, to2));
	moves.insert(std::make_pair(from, to3));

	// multimap allows multiple destinations from same source
	BOOST_CHECK_EQUAL(moves.size(), 3u);
	BOOST_CHECK_EQUAL(moves.count(from), 3u);
}

BOOST_AUTO_TEST_CASE( test_ai_move_map_clear )
{
	ai::move_map moves;
	map_location from(1, 1);
	map_location to(2, 2);

	moves.insert(std::make_pair(from, to));
	BOOST_CHECK_EQUAL(moves.size(), 1u);

	moves.clear();
	BOOST_CHECK(moves.empty());
	BOOST_CHECK_EQUAL(moves.size(), 0u);
}

BOOST_AUTO_TEST_CASE( test_ai_moves_map_default_construction )
{
	ai::moves_map moves;

	BOOST_CHECK(moves.empty());
	BOOST_CHECK_EQUAL(moves.size(), 0u);
}

// ============================================================================
// AI Candidate Action Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_ai_candidate_action_score_constants )
{
	// BAD_SCORE should be 0
	BOOST_CHECK_EQUAL(ai::candidate_action::BAD_SCORE, 0.0);

	// HIGH_SCORE should be 10000000
	BOOST_CHECK_EQUAL(ai::candidate_action::HIGH_SCORE, 10000000.0);

	// HIGH_SCORE should be greater than BAD_SCORE
	BOOST_CHECK_GT(ai::candidate_action::HIGH_SCORE, ai::candidate_action::BAD_SCORE);
}

BOOST_AUTO_TEST_CASE( test_ai_candidate_action_score_values )
{
	// Typical scores should be between BAD and HIGH
	double normal_score = 50.0;
	double excellent_score = 90.0;
	double poor_score = 10.0;

	BOOST_CHECK_GT(normal_score, ai::candidate_action::BAD_SCORE);
	BOOST_CHECK_LT(normal_score, ai::candidate_action::HIGH_SCORE);

	BOOST_CHECK_GT(excellent_score, ai::candidate_action::BAD_SCORE);
	BOOST_CHECK_GT(poor_score, ai::candidate_action::BAD_SCORE);
}

// ============================================================================
// AI Configuration Edge Cases
// ============================================================================

BOOST_AUTO_TEST_CASE( test_ai_config_empty_string_values )
{
	config ai_cfg;
	ai_cfg["id"] = "";
	ai_cfg["description"] = "";

	BOOST_CHECK_EQUAL(ai_cfg["id"].str(), "");
	BOOST_CHECK_EQUAL(ai_cfg["description"].str(), "");
}

BOOST_AUTO_TEST_CASE( test_ai_config_numeric_zero_values )
{
	config ai_cfg;
	ai_cfg["mp_rank"] = "0";
	ai_cfg["value"] = "0";

	BOOST_CHECK_EQUAL(ai_cfg["mp_rank"].to_int(-1), 0);
	BOOST_CHECK_EQUAL(ai_cfg["value"].to_int(-1), 0);
}

BOOST_AUTO_TEST_CASE( test_ai_config_negative_values )
{
	config ai_cfg;
	ai_cfg["leader_aggression"] = "-0.5";
	ai_cfg["aggression"] = "-1.0";

	BOOST_CHECK_EQUAL(ai_cfg["leader_aggression"].str(), "-0.5");
	BOOST_CHECK_EQUAL(ai_cfg["aggression"].str(), "-1.0");
}

BOOST_AUTO_TEST_CASE( test_ai_config_large_values )
{
	config ai_cfg;
	ai_cfg["value"] = "999999";
	ai_cfg["mp_rank"] = "1000000";

	BOOST_CHECK_GT(ai_cfg["value"].to_int(0), 100000);
	BOOST_CHECK_GT(ai_cfg["mp_rank"].to_int(0), 100000);
}


// ============================================================================
// AI Goal Configuration Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_ai_goal_target_unit_config )
{
	config ai_cfg;
	config& goal = ai_cfg.add_child("target_unit");
	goal["id"] = "leader";
	goal["value"] = "100";

	BOOST_CHECK(ai_cfg.has_child("target_unit"));
	BOOST_CHECK_EQUAL(goal["id"].str(), "leader");
	BOOST_CHECK_EQUAL(goal["value"].str(), "100");
}

BOOST_AUTO_TEST_CASE( test_ai_goal_guard_config )
{
	config ai_cfg;
	config& goal = ai_cfg.add_child("guard");
	goal["id"] = "important_unit";
	goal["value"] = "80";

	BOOST_CHECK(ai_cfg.has_child("guard"));
	BOOST_CHECK_EQUAL(goal["id"].str(), "important_unit");
}

BOOST_AUTO_TEST_CASE( test_ai_goal_reach_owns_config )
{
	config ai_cfg;
	config& goal = ai_cfg.add_child("reach_owns");
	goal["id"] = "keep";
	goal["value"] = "50";

	BOOST_CHECK(ai_cfg.has_child("reach_owns"));
	BOOST_CHECK_EQUAL(goal["id"].str(), "keep");
}

// ============================================================================
// AI Stage Configuration Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_ai_stage_main_loop_config )
{
	config ai_cfg;
	config& stage = ai_cfg.add_child("stage");
	stage["id"] = "main_loop";
	stage["name"] = "ai_default_rca::standard_main_loop";

	BOOST_CHECK(ai_cfg.has_child("stage"));
	BOOST_CHECK_EQUAL(stage["id"].str(), "main_loop");
}

BOOST_AUTO_TEST_CASE( test_ai_stage_custom_config )
{
	config ai_cfg;
	config& stage = ai_cfg.add_child("stage");
	stage["id"] = "custom_stage";
	stage["name"] = "custom_stage_name";
	stage["max_executions"] = "5";

	BOOST_CHECK_EQUAL(stage["max_executions"].str(), "5");
}

// ============================================================================
// AI Engine Configuration Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_ai_engine_cpp_config )
{
	config ai_cfg;
	config& engine = ai_cfg.add_child("engine");
	engine["name"] = "cpp";
	engine["id"] = "cpp_engine";

	BOOST_CHECK_EQUAL(engine["name"].str(), "cpp");
}

BOOST_AUTO_TEST_CASE( test_ai_engine_lua_config )
{
	config ai_cfg;
	config& engine = ai_cfg.add_child("engine");
	engine["name"] = "lua";
	engine["id"] = "lua_engine";
	engine["code"] = "return true";

	BOOST_CHECK_EQUAL(engine["name"].str(), "lua");
	BOOST_CHECK_EQUAL(engine["code"].str(), "return true");
}

// ============================================================================
// AI Game Info Extended Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_ai_game_info_default_construction )
{
	ai::game_info info;

	BOOST_CHECK(info.recent_attacks.empty());
	BOOST_CHECK_EQUAL(info.recent_attacks.size(), 0u);
}

BOOST_AUTO_TEST_CASE( test_ai_game_info_multiple_attacks )
{
	ai::game_info info;

	std::set<map_location> locations;
	for(int i = 0; i < 10; ++i) {
		map_location loc(i, i);
		info.recent_attacks.insert(loc);
		locations.insert(loc);
	}

	BOOST_CHECK_EQUAL(info.recent_attacks.size(), locations.size());
}

BOOST_AUTO_TEST_CASE( test_ai_game_info_duplicate_attack_location )
{
	ai::game_info info;
	map_location loc(5, 5);

	info.recent_attacks.insert(loc);
	info.recent_attacks.insert(loc);  // Duplicate

	// set should only contain unique locations
	BOOST_CHECK_EQUAL(info.recent_attacks.size(), 1u);
}

// ============================================================================
// AI Description Extended Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_ai_description_copy_construction )
{
	ai::description original;
	original.id = "test_id";
	original.text = t_string("Test Text");
	original.mp_rank = 42;

	ai::description copy = original;

	BOOST_CHECK_EQUAL(copy.id, original.id);
	BOOST_CHECK_EQUAL(copy.text.str(), original.text.str());
	BOOST_CHECK_EQUAL(copy.mp_rank, original.mp_rank);
}

BOOST_AUTO_TEST_CASE( test_ai_description_assignment )
{
	ai::description desc1, desc2;
	desc1.id = "id1";
	desc1.mp_rank = 10;

	desc2.id = "id2";
	desc2.mp_rank = 20;

	desc2 = desc1;

	BOOST_CHECK_EQUAL(desc2.id, "id1");
	BOOST_CHECK_EQUAL(desc2.mp_rank, 10);
}

BOOST_AUTO_TEST_SUITE_END()
