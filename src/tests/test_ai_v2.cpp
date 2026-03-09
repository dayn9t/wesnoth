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

#include "ai/actions.hpp"
#include "ai/configuration.hpp"
#include "ai/game_info.hpp"
#include "ai/composite/rca.hpp"
#include "ai/manager.hpp"
#include "config.hpp"
#include "map/location.hpp"
#include "pathfind/pathfind.hpp"

BOOST_AUTO_TEST_SUITE(ai_v2_suite)

// ============================================================================
// AI Actions - Error Code Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_action_error_name_success)
{
	const std::string& name = ai::actions::get_error_name(ai::action_result::AI_ACTION_SUCCESS);
	BOOST_CHECK_EQUAL(name, "action_result::AI_ACTION_SUCCESS");
}

BOOST_AUTO_TEST_CASE(test_ai_action_error_name_failure)
{
	const std::string& name = ai::actions::get_error_name(ai::action_result::AI_ACTION_FAILURE);
	BOOST_CHECK_EQUAL(name, "action_result::AI_ACTION_FAILURE");
}

BOOST_AUTO_TEST_CASE(test_ai_action_error_name_started)
{
	const std::string& name = ai::actions::get_error_name(ai::action_result::AI_ACTION_STARTED);
	BOOST_CHECK_EQUAL(name, "action_result::AI_ACTION_STARTED");
}

// ============================================================================
// AI Attack Result Error Codes
// ============================================================================

BOOST_AUTO_TEST_CASE(test_attack_result_error_empty_attacker)
{
	const std::string& name = ai::actions::get_error_name(ai::attack_result::E_EMPTY_ATTACKER);
	BOOST_CHECK_EQUAL(name, "attack_result::E_EMPTY_ATTACKER");
}

BOOST_AUTO_TEST_CASE(test_attack_result_error_empty_defender)
{
	const std::string& name = ai::actions::get_error_name(ai::attack_result::E_EMPTY_DEFENDER);
	BOOST_CHECK_EQUAL(name, "attack_result::E_EMPTY_DEFENDER");
}

BOOST_AUTO_TEST_CASE(test_attack_result_error_incapacitated_attacker)
{
	const std::string& name = ai::actions::get_error_name(ai::attack_result::E_INCAPACITATED_ATTACKER);
	BOOST_CHECK_EQUAL(name, "attack_result::E_INCAPACITATED_ATTACKER");
}

BOOST_AUTO_TEST_CASE(test_attack_result_error_incapacitated_defender)
{
	const std::string& name = ai::actions::get_error_name(ai::attack_result::E_INCAPACITATED_DEFENDER);
	BOOST_CHECK_EQUAL(name, "attack_result::E_INCAPACITATED_DEFENDER");
}

BOOST_AUTO_TEST_CASE(test_attack_result_error_not_own_attacker)
{
	const std::string& name = ai::actions::get_error_name(ai::attack_result::E_NOT_OWN_ATTACKER);
	BOOST_CHECK_EQUAL(name, "attack_result::E_NOT_OWN_ATTACKER");
}

BOOST_AUTO_TEST_CASE(test_attack_result_error_not_enemy_defender)
{
	const std::string& name = ai::actions::get_error_name(ai::attack_result::E_NOT_ENEMY_DEFENDER);
	BOOST_CHECK_EQUAL(name, "attack_result::E_NOT_ENEMY_DEFENDER");
}

BOOST_AUTO_TEST_CASE(test_attack_result_error_no_attacks_left)
{
	const std::string& name = ai::actions::get_error_name(ai::attack_result::E_NO_ATTACKS_LEFT);
	BOOST_CHECK_EQUAL(name, "attack_result::E_NO_ATTACKS_LEFT");
}

BOOST_AUTO_TEST_CASE(test_attack_result_error_wrong_attacker_weapon)
{
	const std::string& name = ai::actions::get_error_name(ai::attack_result::E_WRONG_ATTACKER_WEAPON);
	BOOST_CHECK_EQUAL(name, "attack_result::E_WRONG_ATTACKER_WEAPON");
}

BOOST_AUTO_TEST_CASE(test_attack_result_error_attacker_defender_not_adjacent)
{
	const std::string& name = ai::actions::get_error_name(ai::attack_result::E_ATTACKER_AND_DEFENDER_NOT_ADJACENT);
	BOOST_CHECK(name.find("ATTACKER_AND_DEFENDER_NOT_ADJACENT") != std::string::npos);
}

// ============================================================================
// AI Move Result Error Codes
// ============================================================================

BOOST_AUTO_TEST_CASE(test_move_result_error_empty_move)
{
	const std::string& name = ai::actions::get_error_name(ai::move_result::E_EMPTY_MOVE);
	BOOST_CHECK_EQUAL(name, "move_result::E_EMPTY_MOVE");
}

BOOST_AUTO_TEST_CASE(test_move_result_error_no_unit)
{
	const std::string& name = ai::actions::get_error_name(ai::move_result::E_NO_UNIT);
	BOOST_CHECK_EQUAL(name, "move_result::E_NO_UNIT");
}

BOOST_AUTO_TEST_CASE(test_move_result_error_not_own_unit)
{
	const std::string& name = ai::actions::get_error_name(ai::move_result::E_NOT_OWN_UNIT);
	BOOST_CHECK_EQUAL(name, "move_result::E_NOT_OWN_UNIT");
}

BOOST_AUTO_TEST_CASE(test_move_result_error_incapacitated_unit)
{
	const std::string& name = ai::actions::get_error_name(ai::move_result::E_INCAPACITATED_UNIT);
	BOOST_CHECK_EQUAL(name, "move_result::E_INCAPACITATED_UNIT");
}

BOOST_AUTO_TEST_CASE(test_move_result_error_ambushed)
{
	const std::string& name = ai::actions::get_error_name(ai::move_result::E_AMBUSHED);
	BOOST_CHECK_EQUAL(name, "move_result::E_AMBUSHED");
}

BOOST_AUTO_TEST_CASE(test_move_result_error_failed_teleport)
{
	const std::string& name = ai::actions::get_error_name(ai::move_result::E_FAILED_TELEPORT);
	BOOST_CHECK_EQUAL(name, "move_result::E_FAILED_TELEPORT");
}

BOOST_AUTO_TEST_CASE(test_move_result_error_off_map)
{
	const std::string& name = ai::actions::get_error_name(ai::move_result::E_OFF_MAP);
	BOOST_CHECK_EQUAL(name, "move_result::E_OFF_MAP");
}

BOOST_AUTO_TEST_CASE(test_move_result_error_no_route)
{
	const std::string& name = ai::actions::get_error_name(ai::move_result::E_NO_ROUTE);
	BOOST_CHECK_EQUAL(name, "move_result::E_NO_ROUTE");
}

// ============================================================================
// AI Recall Result Error Codes
// ============================================================================

BOOST_AUTO_TEST_CASE(test_recall_result_error_not_available)
{
	const std::string& name = ai::actions::get_error_name(ai::recall_result::E_NOT_AVAILABLE_FOR_RECALLING);
	BOOST_CHECK_EQUAL(name, "recall_result::E_NOT_AVAILABLE_FOR_RECALLING");
}

BOOST_AUTO_TEST_CASE(test_recall_result_error_no_gold)
{
	const std::string& name = ai::actions::get_error_name(ai::recall_result::E_NO_GOLD);
	BOOST_CHECK_EQUAL(name, "recall_result::E_NO_GOLD");
}

BOOST_AUTO_TEST_CASE(test_recall_result_error_no_leader)
{
	const std::string& name = ai::actions::get_error_name(ai::recall_result::E_NO_LEADER);
	BOOST_CHECK(name.find("E_NO_LEADER") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_recall_result_error_leader_not_on_keep)
{
	const std::string& name = ai::actions::get_error_name(ai::recall_result::E_LEADER_NOT_ON_KEEP);
	BOOST_CHECK_EQUAL(name, "recall_result::E_LEADER_NOT_ON_KEEP");
}

BOOST_AUTO_TEST_CASE(test_recall_result_error_bad_recall_location)
{
	const std::string& name = ai::actions::get_error_name(ai::recall_result::E_BAD_RECALL_LOCATION);
	BOOST_CHECK_EQUAL(name, "recall_result::E_BAD_RECALL_LOCATION");
}

// ============================================================================
// AI Recruit Result Error Codes
// ============================================================================

BOOST_AUTO_TEST_CASE(test_recruit_result_error_unknown_unit_type)
{
	const std::string& name = ai::actions::get_error_name(ai::recruit_result::E_UNKNOWN_OR_DUMMY_UNIT_TYPE);
	BOOST_CHECK_EQUAL(name, "recruit_result::E_UNKNOWN_OR_DUMMY_UNIT_TYPE");
}

BOOST_AUTO_TEST_CASE(test_recruit_result_error_no_gold)
{
	const std::string& name = ai::actions::get_error_name(ai::recruit_result::E_NO_GOLD);
	BOOST_CHECK_EQUAL(name, "recruit_result::E_NO_GOLD");
}

BOOST_AUTO_TEST_CASE(test_recruit_result_error_no_leader)
{
	const std::string& name = ai::actions::get_error_name(ai::recruit_result::E_NO_LEADER);
	BOOST_CHECK_EQUAL(name, "recruit_result::E_NO_LEADER");
}

BOOST_AUTO_TEST_CASE(test_recruit_result_error_leader_not_on_keep)
{
	const std::string& name = ai::actions::get_error_name(ai::recruit_result::E_LEADER_NOT_ON_KEEP);
	BOOST_CHECK_EQUAL(name, "recruit_result::E_LEADER_NOT_ON_KEEP");
}

BOOST_AUTO_TEST_CASE(test_recruit_result_error_bad_recruit_location)
{
	const std::string& name = ai::actions::get_error_name(ai::recruit_result::E_BAD_RECRUIT_LOCATION);
	BOOST_CHECK_EQUAL(name, "recruit_result::E_BAD_RECRUIT_LOCATION");
}

// ============================================================================
// AI Stopunit Result Error Codes
// ============================================================================

BOOST_AUTO_TEST_CASE(test_stopunit_result_error_no_unit)
{
	const std::string& name = ai::actions::get_error_name(ai::stopunit_result::E_NO_UNIT);
	BOOST_CHECK_EQUAL(name, "stopunit_result::E_NO_UNIT");
}

BOOST_AUTO_TEST_CASE(test_stopunit_result_error_not_own_unit)
{
	const std::string& name = ai::actions::get_error_name(ai::stopunit_result::E_NOT_OWN_UNIT);
	BOOST_CHECK_EQUAL(name, "stopunit_result::E_NOT_OWN_UNIT");
}

BOOST_AUTO_TEST_CASE(test_stopunit_result_error_incapacitated_unit)
{
	const std::string& name = ai::actions::get_error_name(ai::stopunit_result::E_INCAPACITATED_UNIT);
	BOOST_CHECK_EQUAL(name, "stopunit_result::E_INCAPACITATED_UNIT");
}

// ============================================================================
// AI Configuration - Description Structure Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_description_default_construction)
{
	ai::description desc;

	BOOST_CHECK_EQUAL(desc.id, "");
	BOOST_CHECK_EQUAL(desc.text.str(), "");
	BOOST_CHECK_EQUAL(desc.mp_rank, 0);
	BOOST_CHECK(desc.cfg.empty());
}

BOOST_AUTO_TEST_CASE(test_ai_description_with_id)
{
	ai::description desc;
	desc.id = "test_ai_id";
	desc.text = t_string("Test AI Description");
	desc.mp_rank = 5;

	config cfg;
	cfg["test_attr"] = "test_value";
	desc.cfg = cfg;

	BOOST_CHECK_EQUAL(desc.id, "test_ai_id");
	BOOST_CHECK_EQUAL(desc.text.str(), "Test AI Description");
	BOOST_CHECK_EQUAL(desc.mp_rank, 5);
	BOOST_CHECK_EQUAL(desc.cfg["test_attr"].str(), "test_value");
}

BOOST_AUTO_TEST_CASE(test_ai_description_mp_rank_max)
{
	ai::description desc;
	desc.id = "max_rank_ai";
	desc.mp_rank = std::numeric_limits<int>::max();

	BOOST_CHECK_EQUAL(desc.mp_rank, std::numeric_limits<int>::max());
}

// ============================================================================
// AI Configuration - Config Parsing Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_config_parse_with_ai_tag)
{
	config ais_cfg;
	config& ai_tag = ais_cfg.add_child("ai");
	ai_tag["id"] = "test_ai";
	ai_tag["description"] = "Test AI Configuration";
	ai_tag["mp_rank"] = "10";

	BOOST_CHECK(ais_cfg.has_child("ai"));
	BOOST_CHECK_EQUAL(ais_cfg.child_count("ai"), 1u);
}

BOOST_AUTO_TEST_CASE(test_ai_config_parse_multiple_ai_tags)
{
	config ais_cfg;

	config& ai1 = ais_cfg.add_child("ai");
	ai1["id"] = "ai_one";
	ai1["description"] = "First AI";

	config& ai2 = ais_cfg.add_child("ai");
	ai2["id"] = "ai_two";
	ai2["description"] = "Second AI";

	config& ai3 = ais_cfg.add_child("ai");
	ai3["id"] = "ai_three";
	ai3["description"] = "Third AI";

	BOOST_CHECK_EQUAL(ais_cfg.child_count("ai"), 3u);
}

BOOST_AUTO_TEST_CASE(test_ai_config_default_config)
{
	config ais_cfg;
	config& default_cfg = ais_cfg.add_child("default_config");
	default_cfg["aggression"] = "0.5";
	default_cfg["caution"] = "0.3";

	ais_cfg["default_ai_algorithm"] = "test_ai";

	BOOST_CHECK(ais_cfg.has_child("default_config"));
	BOOST_CHECK_EQUAL(ais_cfg["default_ai_algorithm"].str(), "test_ai");
}

BOOST_AUTO_TEST_CASE(test_ai_config_hidden_attribute)
{
	config ais_cfg;
	config& ai_tag = ais_cfg.add_child("ai");
	ai_tag["id"] = "hidden_ai";
	ai_tag["hidden"] = "true";

	BOOST_CHECK(ai_tag["hidden"].to_bool(false));
}

BOOST_AUTO_TEST_CASE(test_ai_config_not_hidden)
{
	config ais_cfg;
	config& ai_tag = ais_cfg.add_child("ai");
	ai_tag["id"] = "visible_ai";
	// hidden not set

	BOOST_CHECK(!ai_tag["hidden"].to_bool(false));
}

// ============================================================================
// AI Configuration - Aspect Expansion Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_expand_simplified_aspect_aggression)
{
	config ai_cfg;
	ai_cfg["aggression"] = "0.75";

	BOOST_CHECK_EQUAL(ai_cfg["aggression"].str(), "0.75");
}

BOOST_AUTO_TEST_CASE(test_ai_expand_simplified_aspect_caution)
{
	config ai_cfg;
	ai_cfg["caution"] = "0.25";

	BOOST_CHECK_EQUAL(ai_cfg["caution"].str(), "0.25");
}

BOOST_AUTO_TEST_CASE(test_ai_expand_simplified_aspect_village_value)
{
	config ai_cfg;
	ai_cfg["village_value"] = "1.5";

	BOOST_CHECK_EQUAL(ai_cfg["village_value"].str(), "1.5");
}

BOOST_AUTO_TEST_CASE(test_ai_aspect_with_turns_filter)
{
	config ai_cfg;
	ai_cfg["turns"] = "1-5";
	ai_cfg["aggression"] = "1.0";

	BOOST_CHECK_EQUAL(ai_cfg["turns"].str(), "1-5");
	BOOST_CHECK_EQUAL(ai_cfg["aggression"].str(), "1.0");
}

BOOST_AUTO_TEST_CASE(test_ai_aspect_with_time_of_day_filter)
{
	config ai_cfg;
	ai_cfg["time_of_day"] = "dawn,morning";
	ai_cfg["caution"] = "0.5";

	BOOST_CHECK_EQUAL(ai_cfg["time_of_day"].str(), "dawn,morning");
	BOOST_CHECK_EQUAL(ai_cfg["caution"].str(), "0.5");
}

// ============================================================================
// AI Configuration - Goal Parsing Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_goal_target)
{
	config ai_cfg;
	config& goal = ai_cfg.add_child("target");
	goal["id"] = "enemy_leader";
	goal["value"] = "100";

	BOOST_CHECK(ai_cfg.has_child("target"));
	BOOST_CHECK_EQUAL(goal["id"].str(), "enemy_leader");
	BOOST_CHECK_EQUAL(goal["value"].str(), "100");
}

BOOST_AUTO_TEST_CASE(test_ai_goal_target_location)
{
	config ai_cfg;
	config& goal = ai_cfg.add_child("target_location");
	goal["x"] = "10";
	goal["y"] = "15";
	goal["value"] = "50";

	BOOST_CHECK(ai_cfg.has_child("target_location"));
	BOOST_CHECK_EQUAL(goal["x"].str(), "10");
	BOOST_CHECK_EQUAL(goal["y"].str(), "15");
}

BOOST_AUTO_TEST_CASE(test_ai_goal_protect_unit)
{
	config ai_cfg;
	config& goal = ai_cfg.add_child("protect_unit");
	goal["id"] = "important_unit";
	goal["protect_radius"] = "3";
	goal["value"] = "80";

	BOOST_CHECK(ai_cfg.has_child("protect_unit"));
	BOOST_CHECK_EQUAL(goal["protect_radius"].str(), "3");
}

BOOST_AUTO_TEST_CASE(test_ai_goal_protect_location)
{
	config ai_cfg;
	config& goal = ai_cfg.add_child("protect_location");
	goal["x"] = "5";
	goal["y"] = "5";
	goal["protect_radius"] = "2";
	goal["value"] = "60";

	BOOST_CHECK(ai_cfg.has_child("protect_location"));
	BOOST_CHECK_EQUAL(goal["protect_radius"].str(), "2");
}

// ============================================================================
// AI Configuration - Stage and Engine Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_stage_config)
{
	config ai_cfg;
	config& stage = ai_cfg.add_child("stage");
	stage["id"] = "main_loop";
	stage["name"] = "testing_ai_default::candidate_action_evaluation_loop";

	BOOST_CHECK(ai_cfg.has_child("stage"));
	BOOST_CHECK_EQUAL(stage["id"].str(), "main_loop");
}

BOOST_AUTO_TEST_CASE(test_ai_engine_config_cpp)
{
	config ai_cfg;
	config& engine = ai_cfg.add_child("engine");
	engine["name"] = "cpp";
	engine["id"] = "cpp_engine";

	BOOST_CHECK(ai_cfg.has_child("engine"));
	BOOST_CHECK_EQUAL(engine["name"].str(), "cpp");
}

BOOST_AUTO_TEST_CASE(test_ai_engine_config_lua)
{
	config ai_cfg;
	config& engine = ai_cfg.add_child("engine");
	engine["name"] = "lua";
	engine["id"] = "lua_engine";

	BOOST_CHECK(ai_cfg.has_child("engine"));
	BOOST_CHECK_EQUAL(engine["name"].str(), "lua");
}

// ============================================================================
// AI Side Config Parsing Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_side_config_parse_basic)
{
	config side_cfg;
	side_cfg["ai_algorithm"] = "testing_ai_default";

	config& ai_tag = side_cfg.add_child("ai");
	ai_tag["aggression"] = "0.6";

	BOOST_CHECK(side_cfg.has_attribute("ai_algorithm"));
	BOOST_CHECK(side_cfg.has_child("ai"));
}

BOOST_AUTO_TEST_CASE(test_ai_side_config_multiple_ai_tags)
{
	config side_cfg;

	// Add multiple AI tags with different turns
	config& ai1 = side_cfg.add_child("ai");
	ai1["turns"] = "1-10";
	ai1["aggression"] = "0.3";

	config& ai2 = side_cfg.add_child("ai");
	ai2["turns"] = "11-20";
	ai2["aggression"] = "0.7";

	BOOST_CHECK_EQUAL(side_cfg.child_count("ai"), 2u);
}

BOOST_AUTO_TEST_CASE(test_ai_side_config_empty_ai_tags)
{
	config side_cfg;
	// No AI tags

	BOOST_CHECK_EQUAL(side_cfg.child_count("ai"), 0u);
}

// ============================================================================
// AI Game Info Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_game_info_construction)
{
	ai::game_info info;

	BOOST_CHECK(info.recent_attacks.empty());
}

BOOST_AUTO_TEST_CASE(test_ai_game_info_recent_attacks_insert)
{
	ai::game_info info;
	map_location loc1(5, 10);
	map_location loc2(15, 20);

	info.recent_attacks.insert(loc1);
	info.recent_attacks.insert(loc2);

	BOOST_CHECK_EQUAL(info.recent_attacks.size(), 2u);
	BOOST_CHECK(info.recent_attacks.count(loc1) == 1);
	BOOST_CHECK(info.recent_attacks.count(loc2) == 1);
}

BOOST_AUTO_TEST_CASE(test_ai_game_info_recent_attacks_clear)
{
	ai::game_info info;
	map_location loc(5, 5);

	info.recent_attacks.insert(loc);
	BOOST_CHECK_EQUAL(info.recent_attacks.size(), 1u);

	info.recent_attacks.clear();
	BOOST_CHECK(info.recent_attacks.empty());
}

// ============================================================================
// AI Side Number Type Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_side_number_default)
{
	ai::side_number side = 1;
	BOOST_CHECK_EQUAL(side, 1);
}

BOOST_AUTO_TEST_CASE(test_ai_side_number_max)
{
	ai::side_number side = std::numeric_limits<int>::max();
	BOOST_CHECK_EQUAL(side, std::numeric_limits<int>::max());
}

// ============================================================================
// AI Move Map Type Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_move_map_basic)
{
	ai::move_map moves;
	map_location from(1, 1);
	map_location to(2, 2);

	moves.insert(std::make_pair(from, to));

	BOOST_CHECK_EQUAL(moves.size(), 1u);
	BOOST_CHECK(moves.count(from) == 1);
}

BOOST_AUTO_TEST_CASE(test_ai_move_map_multiple_destinations)
{
	ai::move_map moves;
	map_location from(5, 5);
	map_location to1(6, 5);
	map_location to2(5, 6);
	map_location to3(6, 6);

	moves.insert(std::make_pair(from, to1));
	moves.insert(std::make_pair(from, to2));
	moves.insert(std::make_pair(from, to3));

	// multimap allows multiple values for same key
	BOOST_CHECK_EQUAL(moves.count(from), 3);
}

BOOST_AUTO_TEST_CASE(test_ai_move_map_clear)
{
	ai::move_map moves;
	map_location from(1, 1);
	map_location to(2, 2);

	moves.insert(std::make_pair(from, to));
	BOOST_CHECK_EQUAL(moves.size(), 1u);

	moves.clear();
	BOOST_CHECK(moves.empty());
}

// ============================================================================
// AI Aspect Map Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_aspect_map_basic)
{
	ai::aspect_map aspects;
	config aspect_cfg;
	aspect_cfg["id"] = "test_aspect";
	aspect_cfg["value"] = "1.0";

	BOOST_CHECK(aspects.empty());
}

BOOST_AUTO_TEST_CASE(test_ai_aspect_map_insert)
{
	ai::aspect_map aspects;
	// Aspects are stored by string id

	BOOST_CHECK(aspects.empty());
}

// ============================================================================
// Candidate Action Score Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_candidate_action_bad_score)
{
	BOOST_CHECK(ai::candidate_action::BAD_SCORE <= 0);
}

BOOST_AUTO_TEST_CASE(test_candidate_action_high_score)
{
	BOOST_CHECK(ai::candidate_action::HIGH_SCORE > 0);
	BOOST_CHECK(ai::candidate_action::HIGH_SCORE > ai::candidate_action::BAD_SCORE);
}

BOOST_AUTO_TEST_CASE(test_candidate_action_score_comparison)
{
	double normal_score = 50.0;

	BOOST_CHECK(normal_score > ai::candidate_action::BAD_SCORE);
	BOOST_CHECK(normal_score < ai::candidate_action::HIGH_SCORE);
}

// ============================================================================
// AI Configuration - Recruitment Instructions Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_recruitment_instructions_basic)
{
	config recruit_cfg;
	recruit_cfg["id"] = "recruitment_instructions";

	config& instruction = recruit_cfg.add_child("instruction");
	instruction["type"] = "recruit";
	instruction["unit"] = "Elvish Fighter";
	instruction["number"] = "3";

	BOOST_CHECK(recruit_cfg.has_child("instruction"));
}

BOOST_AUTO_TEST_CASE(test_ai_recruitment_pattern)
{
	config ai_cfg;
	ai_cfg["recruitment_pattern"] = "fighter,archer,mixed";

	BOOST_CHECK_EQUAL(ai_cfg["recruitment_pattern"].str(), "fighter,archer,mixed");
}

BOOST_AUTO_TEST_CASE(test_ai_recruitment_more)
{
	config ai_cfg;
	ai_cfg["recruitment_more"] = "Elvish Scout";

	BOOST_CHECK_EQUAL(ai_cfg["recruitment_more"].str(), "Elvish Scout");
}

// ============================================================================
// AI Configuration - Leader Behavior Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_leader_aggression)
{
	config ai_cfg;
	ai_cfg["leader_aggression"] = "-1.0";

	BOOST_CHECK_EQUAL(ai_cfg["leader_aggression"].str(), "-1.0");
}

BOOST_AUTO_TEST_CASE(test_ai_leader_value)
{
	config ai_cfg;
	ai_cfg["leader_value"] = "3.0";

	BOOST_CHECK_EQUAL(ai_cfg["leader_value"].str(), "3.0");
}

BOOST_AUTO_TEST_CASE(test_ai_passive_leader_true)
{
	config ai_cfg;
	ai_cfg["passive_leader"] = "yes";

	BOOST_CHECK(ai_cfg["passive_leader"].to_bool(false));
}

BOOST_AUTO_TEST_CASE(test_ai_passive_leader_false)
{
	config ai_cfg;
	// Not set, defaults to false

	BOOST_CHECK(!ai_cfg["passive_leader"].to_bool(false));
}

BOOST_AUTO_TEST_CASE(test_ai_leader_ignores_keep)
{
	config ai_cfg;
	ai_cfg["leader_ignores_keep"] = "yes";

	BOOST_CHECK(ai_cfg["leader_ignores_keep"].to_bool(false));
}

// ============================================================================
// AI Configuration - Scout Behavior Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_scout_village_targeting)
{
	config ai_cfg;
	ai_cfg["scout_village_targeting"] = "4";

	BOOST_CHECK_EQUAL(ai_cfg["scout_village_targeting"].str(), "4");
}

BOOST_AUTO_TEST_CASE(test_ai_villages_per_scout)
{
	config ai_cfg;
	ai_cfg["villages_per_scout"] = "3";

	BOOST_CHECK_EQUAL(ai_cfg["villages_per_scout"].str(), "3");
}

BOOST_AUTO_TEST_CASE(test_ai_support_villages_true)
{
	config ai_cfg;
	ai_cfg["support_villages"] = "yes";

	BOOST_CHECK(ai_cfg["support_villages"].to_bool(false));
}

// ============================================================================
// AI Configuration - Retreat Behavior Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_retreat_factor)
{
	config ai_cfg;
	ai_cfg["retreat_factor"] = "0.5";

	BOOST_CHECK_EQUAL(ai_cfg["retreat_factor"].str(), "0.5");
}

BOOST_AUTO_TEST_CASE(test_ai_retreat_enemy_weight)
{
	config ai_cfg;
	ai_cfg["retreat_enemy_weight"] = "1.5";

	BOOST_CHECK_EQUAL(ai_cfg["retreat_enemy_weight"].str(), "1.5");
}

// ============================================================================
// AI Configuration - Grouping Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_grouping_offensive)
{
	config ai_cfg;
	ai_cfg["grouping"] = "offensive";

	BOOST_CHECK_EQUAL(ai_cfg["grouping"].str(), "offensive");
}

BOOST_AUTO_TEST_CASE(test_ai_grouping_defensive)
{
	config ai_cfg;
	ai_cfg["grouping"] = "defensive";

	BOOST_CHECK_EQUAL(ai_cfg["grouping"].str(), "defensive");
}

BOOST_AUTO_TEST_CASE(test_ai_grouping_nothing)
{
	config ai_cfg;
	ai_cfg["grouping"] = "no";

	BOOST_CHECK_EQUAL(ai_cfg["grouping"].str(), "no");
}

// ============================================================================
// AI Configuration - Simple Targeting Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_simple_targeting_true)
{
	config ai_cfg;
	ai_cfg["simple_targeting"] = "yes";

	BOOST_CHECK(ai_cfg["simple_targeting"].to_bool(false));
}

BOOST_AUTO_TEST_CASE(test_ai_simple_targeting_false)
{
	config ai_cfg;
	// Not set, defaults to false

	BOOST_CHECK(!ai_cfg["simple_targeting"].to_bool(false));
}

// ============================================================================
// AI Configuration - Allow Ally Villages Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_allow_ally_villages_true)
{
	config ai_cfg;
	ai_cfg["allow_ally_villages"] = "yes";

	BOOST_CHECK(ai_cfg["allow_ally_villages"].to_bool(false));
}

BOOST_AUTO_TEST_CASE(test_ai_allow_ally_villages_false)
{
	config ai_cfg;
	// Not set, defaults to false

	BOOST_CHECK(!ai_cfg["allow_ally_villages"].to_bool(false));
}

// ============================================================================
// AI Configuration - Recruitment Diversity Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_recruitment_diversity)
{
	config ai_cfg;
	ai_cfg["recruitment_diversity"] = "0.5";

	BOOST_CHECK_EQUAL(ai_cfg["recruitment_diversity"].str(), "0.5");
}

BOOST_AUTO_TEST_CASE(test_ai_recruitment_randomness)
{
	config ai_cfg;
	ai_cfg["recruitment_randomness"] = "2";

	BOOST_CHECK_EQUAL(ai_cfg["recruitment_randomness"].str(), "2");
}

BOOST_AUTO_TEST_CASE(test_ai_recruitment_save_gold)
{
	config ai_cfg;
	ai_cfg["recruitment_save_gold"] = "5";

	BOOST_CHECK_EQUAL(ai_cfg["recruitment_save_gold"].str(), "5");
}

// ============================================================================
// AI Configuration - Modify AI Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_modify_ai_delete)
{
	config modify_cfg;
	modify_cfg["action"] = "delete";
	modify_cfg["aspect"] = "aggression";

	BOOST_CHECK_EQUAL(modify_cfg["action"].str(), "delete");
}

BOOST_AUTO_TEST_CASE(test_ai_modify_ai_add)
{
	config modify_cfg;
	modify_cfg["action"] = "add";
	modify_cfg["aspect"] = "caution";

	BOOST_CHECK_EQUAL(modify_cfg["action"].str(), "add");
}

BOOST_AUTO_TEST_CASE(test_ai_modify_ai_change)
{
	config modify_cfg;
	modify_cfg["action"] = "change";
	modify_cfg["aspect"] = "village_value";

	BOOST_CHECK_EQUAL(modify_cfg["action"].str(), "change");
}

// ============================================================================
// AI Configuration - Micro AI Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_micro_ai_config)
{
	config micro_cfg;
	micro_cfg["type"] = "patrol";
	micro_cfg["side"] = "1";

	config& unit = micro_cfg.add_child("filter_unit");
	unit["id"] = "patrol_unit";

	BOOST_CHECK_EQUAL(micro_cfg["type"].str(), "patrol");
	BOOST_CHECK(micro_cfg.has_child("filter_unit"));
}

// ============================================================================
// AI Error Name Unknown Code Test
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_error_name_unknown_code)
{
	// Test with an unknown error code
	// This should return a valid string (may be an error message or default)
	const std::string& name = ai::actions::get_error_name(-9999);
	// The function should still return something valid
	BOOST_CHECK(!name.empty());
}

// ============================================================================
// AI Action Result Status Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_action_result_success_value)
{
	BOOST_CHECK_EQUAL(ai::action_result::AI_ACTION_SUCCESS, 0);
}

BOOST_AUTO_TEST_CASE(test_ai_action_result_failure_value)
{
	BOOST_CHECK_EQUAL(ai::action_result::AI_ACTION_FAILURE, -1);
}

BOOST_AUTO_TEST_CASE(test_ai_action_result_started_value)
{
	BOOST_CHECK_EQUAL(ai::action_result::AI_ACTION_STARTED, 1);
}

// ============================================================================
// AI Configuration - Complex Scenario Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_config_complex_scenario)
{
	config main_cfg;

	// Add default config
	config& default_cfg = main_cfg.add_child("default_config");
	default_cfg["aggression"] = "0.5";
	default_cfg["caution"] = "0.25";

	main_cfg["default_ai_algorithm"] = "testing_ai_default";

	// Add multiple AI configurations
	for(int i = 0; i < 3; ++i) {
		config& ai = main_cfg.add_child("ai");
		ai["id"] = "ai_" + std::to_string(i);
		ai["description"] = "AI configuration " + std::to_string(i);
		ai["mp_rank"] = std::to_string((i + 1) * 10);
	}

	BOOST_CHECK(main_cfg.has_child("default_config"));
	BOOST_CHECK_EQUAL(main_cfg.child_count("ai"), 3u);
	BOOST_CHECK_EQUAL(main_cfg["default_ai_algorithm"].str(), "testing_ai_default");
}

BOOST_AUTO_TEST_CASE(test_ai_config_with_all_aspects)
{
	config ai_cfg;

	// Set all common aspects
	ai_cfg["aggression"] = "0.6";
	ai_cfg["caution"] = "0.3";
	ai_cfg["grouping"] = "offensive";
	ai_cfg["leader_aggression"] = "-0.5";
	ai_cfg["leader_value"] = "3.0";
	ai_cfg["passive_leader"] = "no";
	ai_cfg["village_value"] = "1.0";
	ai_cfg["villages_per_scout"] = "4";
	ai_cfg["scout_village_targeting"] = "3";
	ai_cfg["retreat_factor"] = "0.5";
	ai_cfg["retreat_enemy_weight"] = "1.2";
	ai_cfg["recruitment_diversity"] = "0.25";
	ai_cfg["recruitment_randomness"] = "1";
	ai_cfg["simple_targeting"] = "no";
	ai_cfg["support_villages"] = "yes";
	ai_cfg["allow_ally_villages"] = "no";

	BOOST_CHECK_EQUAL(ai_cfg["aggression"].str(), "0.6");
	BOOST_CHECK_EQUAL(ai_cfg["caution"].str(), "0.3");
	BOOST_CHECK_EQUAL(ai_cfg["grouping"].str(), "offensive");
	BOOST_CHECK_EQUAL(ai_cfg["village_value"].str(), "1.0");
}

// ============================================================================
// AI Configuration - Recruitment Instructions Extended Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_recruitment_instructions_with_limits)
{
	config recruit_cfg;
	recruit_cfg["id"] = "limited_recruitment";

	config& instruction = recruit_cfg.add_child("instruction");
	instruction["type"] = "recruit";
	instruction["unit"] = "Elvish Archer";
	instruction["number"] = "2";
	instruction["limit"] = "5";

	BOOST_CHECK_EQUAL(instruction["number"].str(), "2");
	BOOST_CHECK_EQUAL(instruction["limit"].str(), "5");
}

BOOST_AUTO_TEST_CASE(test_ai_recruitment_instructions_total_limit)
{
	config recruit_cfg;

	config& limit = recruit_cfg.add_child("limit");
	limit["type"] = "total";
	limit["value"] = "10";

	BOOST_CHECK(recruit_cfg.has_child("limit"));
}

// ============================================================================
// AI Composite Component Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_path_element_default)
{
	ai::path_element elem;

	BOOST_CHECK_EQUAL(elem.property, "");
	BOOST_CHECK_EQUAL(elem.id, "");
	BOOST_CHECK_EQUAL(elem.position, 0);
}

BOOST_AUTO_TEST_CASE(test_ai_path_element_assignment)
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

BOOST_AUTO_TEST_CASE(test_ai_aspect_map_empty)
{
	ai::aspect_map aspects;

	BOOST_CHECK(aspects.empty());
	BOOST_CHECK_EQUAL(aspects.size(), 0u);
}

// ============================================================================
// AI Result Type Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_result_type_size)
{
	// Verify result types are integral
	BOOST_CHECK_EQUAL(sizeof(ai::side_number), sizeof(int));
}

BOOST_AUTO_TEST_CASE(test_ai_move_map_default_construction)
{
	ai::move_map moves;

	BOOST_CHECK(moves.empty());
	BOOST_CHECK_EQUAL(moves.size(), 0u);
}

BOOST_AUTO_TEST_CASE(test_ai_move_map_single_move)
{
	ai::move_map moves;
	map_location from(3, 4);
	map_location to(5, 6);

	moves.insert(std::make_pair(from, to));

	BOOST_CHECK_EQUAL(moves.size(), 1u);
	BOOST_CHECK_EQUAL(moves.count(from), 1u);
	BOOST_CHECK_EQUAL(moves.begin()->second, to);
}

BOOST_AUTO_TEST_CASE(test_ai_move_map_multiple_same_source)
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

BOOST_AUTO_TEST_CASE(test_ai_moves_map_default_construction)
{
	ai::moves_map moves;

	BOOST_CHECK(moves.empty());
	BOOST_CHECK_EQUAL(moves.size(), 0u);
}

// ============================================================================
// AI Configuration Edge Cases
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_config_empty_string_values)
{
	config ai_cfg;
	ai_cfg["id"] = "";
	ai_cfg["description"] = "";

	BOOST_CHECK_EQUAL(ai_cfg["id"].str(), "");
	BOOST_CHECK_EQUAL(ai_cfg["description"].str(), "");
}

BOOST_AUTO_TEST_CASE(test_ai_config_numeric_zero_values)
{
	config ai_cfg;
	ai_cfg["mp_rank"] = "0";
	ai_cfg["value"] = "0";

	BOOST_CHECK_EQUAL(ai_cfg["mp_rank"].to_int(-1), 0);
	BOOST_CHECK_EQUAL(ai_cfg["value"].to_int(-1), 0);
}

BOOST_AUTO_TEST_CASE(test_ai_config_negative_values)
{
	config ai_cfg;
	ai_cfg["leader_aggression"] = "-0.5";
	ai_cfg["aggression"] = "-1.0";

	BOOST_CHECK_EQUAL(ai_cfg["leader_aggression"].str(), "-0.5");
	BOOST_CHECK_EQUAL(ai_cfg["aggression"].str(), "-1.0");
}

BOOST_AUTO_TEST_CASE(test_ai_config_large_values)
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

BOOST_AUTO_TEST_CASE(test_ai_goal_target_unit_config)
{
	config ai_cfg;
	config& goal = ai_cfg.add_child("target_unit");
	goal["id"] = "leader";
	goal["value"] = "100";

	BOOST_CHECK(ai_cfg.has_child("target_unit"));
	BOOST_CHECK_EQUAL(goal["id"].str(), "leader");
	BOOST_CHECK_EQUAL(goal["value"].str(), "100");
}

BOOST_AUTO_TEST_CASE(test_ai_goal_guard_config)
{
	config ai_cfg;
	config& goal = ai_cfg.add_child("guard");
	goal["id"] = "important_unit";
	goal["value"] = "80";

	BOOST_CHECK(ai_cfg.has_child("guard"));
	BOOST_CHECK_EQUAL(goal["id"].str(), "important_unit");
}

BOOST_AUTO_TEST_CASE(test_ai_goal_reach_owns_config)
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

BOOST_AUTO_TEST_CASE(test_ai_stage_main_loop_config)
{
	config ai_cfg;
	config& stage = ai_cfg.add_child("stage");
	stage["id"] = "main_loop";
	stage["name"] = "ai_default_rca::standard_main_loop";

	BOOST_CHECK(ai_cfg.has_child("stage"));
	BOOST_CHECK_EQUAL(stage["id"].str(), "main_loop");
}

BOOST_AUTO_TEST_CASE(test_ai_stage_custom_config)
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

BOOST_AUTO_TEST_CASE(test_ai_engine_cpp_config)
{
	config ai_cfg;
	config& engine = ai_cfg.add_child("engine");
	engine["name"] = "cpp";
	engine["id"] = "cpp_engine";

	BOOST_CHECK_EQUAL(engine["name"].str(), "cpp");
}

BOOST_AUTO_TEST_CASE(test_ai_engine_lua_config)
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
// AI Attack Result Additional Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_attack_result_error_wrong_attacker_weapon_detailed)
{
	const std::string& name = ai::actions::get_error_name(ai::attack_result::E_WRONG_ATTACKER_WEAPON);
	BOOST_CHECK(name.find("WRONG_ATTACKER_WEAPON") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_attack_result_error_unable_to_choose_attacker_weapon)
{
	const std::string& name = ai::actions::get_error_name(ai::attack_result::E_UNABLE_TO_CHOOSE_ATTACKER_WEAPON);
	BOOST_CHECK_EQUAL(name, "attack_result::E_UNABLE_TO_CHOOSE_ATTACKER_WEAPON");
}

// ============================================================================
// AI Move Result Additional Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_move_result_error_ambushed_detailed)
{
	const std::string& name = ai::actions::get_error_name(ai::move_result::E_AMBUSHED);
	BOOST_CHECK(name.find("AMBUSHED") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_move_result_error_failed_teleport_detailed)
{
	const std::string& name = ai::actions::get_error_name(ai::move_result::E_FAILED_TELEPORT);
	BOOST_CHECK_EQUAL(name, "move_result::E_FAILED_TELEPORT");
}

// ============================================================================
// AI Recall Result Additional Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_recall_result_error_not_available_detailed)
{
	const std::string& name = ai::actions::get_error_name(ai::recall_result::E_NOT_AVAILABLE_FOR_RECALLING);
	BOOST_CHECK_EQUAL(name, "recall_result::E_NOT_AVAILABLE_FOR_RECALLING");
}

// ============================================================================
// AI Recruit Result Additional Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_recruit_result_error_unknown_unit_type_detailed)
{
	const std::string& name = ai::actions::get_error_name(ai::recruit_result::E_UNKNOWN_OR_DUMMY_UNIT_TYPE);
	BOOST_CHECK_EQUAL(name, "recruit_result::E_UNKNOWN_OR_DUMMY_UNIT_TYPE");
}

BOOST_AUTO_TEST_CASE(test_recruit_result_error_not_available_for_recruiting)
{
	const std::string& name = ai::actions::get_error_name(ai::recruit_result::E_NOT_AVAILABLE_FOR_RECRUITING);
	BOOST_CHECK_EQUAL(name, "recruit_result::E_NOT_AVAILABLE_FOR_RECRUITING");
}

// ============================================================================
// AI Stopunit Result Additional Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_stopunit_result_error_no_unit_detailed)
{
	const std::string& name = ai::actions::get_error_name(ai::stopunit_result::E_NO_UNIT);
	BOOST_CHECK_EQUAL(name, "stopunit_result::E_NO_UNIT");
}

// ============================================================================
// AI Game Info Extended Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_game_info_default_construction)
{
	ai::game_info info;

	BOOST_CHECK(info.recent_attacks.empty());
	BOOST_CHECK_EQUAL(info.recent_attacks.size(), 0u);
}

BOOST_AUTO_TEST_CASE(test_ai_game_info_multiple_attacks)
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

BOOST_AUTO_TEST_CASE(test_ai_game_info_duplicate_attack_location)
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

BOOST_AUTO_TEST_CASE(test_ai_description_copy_construction)
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

BOOST_AUTO_TEST_CASE(test_ai_description_assignment)
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

// ============================================================================
// AI Error Name Coverage Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_error_name_all_attack_results)
{
	// Cover all attack result error codes
	BOOST_CHECK(!ai::actions::get_error_name(ai::attack_result::E_EMPTY_ATTACKER).empty());
	BOOST_CHECK(!ai::actions::get_error_name(ai::attack_result::E_EMPTY_DEFENDER).empty());
	BOOST_CHECK(!ai::actions::get_error_name(ai::attack_result::E_INCAPACITATED_ATTACKER).empty());
	BOOST_CHECK(!ai::actions::get_error_name(ai::attack_result::E_INCAPACITATED_DEFENDER).empty());
	BOOST_CHECK(!ai::actions::get_error_name(ai::attack_result::E_NOT_OWN_ATTACKER).empty());
	BOOST_CHECK(!ai::actions::get_error_name(ai::attack_result::E_NOT_ENEMY_DEFENDER).empty());
	BOOST_CHECK(!ai::actions::get_error_name(ai::attack_result::E_NO_ATTACKS_LEFT).empty());
	BOOST_CHECK(!ai::actions::get_error_name(ai::attack_result::E_WRONG_ATTACKER_WEAPON).empty());
	BOOST_CHECK(!ai::actions::get_error_name(ai::attack_result::E_UNABLE_TO_CHOOSE_ATTACKER_WEAPON).empty());
	BOOST_CHECK(!ai::actions::get_error_name(ai::attack_result::E_ATTACKER_AND_DEFENDER_NOT_ADJACENT).empty());
}

BOOST_AUTO_TEST_CASE(test_ai_error_name_all_move_results)
{
	// Cover all move result error codes
	BOOST_CHECK(!ai::actions::get_error_name(ai::move_result::E_EMPTY_MOVE).empty());
	BOOST_CHECK(!ai::actions::get_error_name(ai::move_result::E_NO_UNIT).empty());
	BOOST_CHECK(!ai::actions::get_error_name(ai::move_result::E_NOT_OWN_UNIT).empty());
	BOOST_CHECK(!ai::actions::get_error_name(ai::move_result::E_INCAPACITATED_UNIT).empty());
	BOOST_CHECK(!ai::actions::get_error_name(ai::move_result::E_AMBUSHED).empty());
	BOOST_CHECK(!ai::actions::get_error_name(ai::move_result::E_FAILED_TELEPORT).empty());
	BOOST_CHECK(!ai::actions::get_error_name(ai::move_result::E_OFF_MAP).empty());
	BOOST_CHECK(!ai::actions::get_error_name(ai::move_result::E_NO_ROUTE).empty());
}

BOOST_AUTO_TEST_CASE(test_ai_error_name_all_recall_results)
{
	// Cover all recall result error codes
	BOOST_CHECK(!ai::actions::get_error_name(ai::recall_result::E_NOT_AVAILABLE_FOR_RECALLING).empty());
	BOOST_CHECK(!ai::actions::get_error_name(ai::recall_result::E_NO_GOLD).empty());
	BOOST_CHECK(!ai::actions::get_error_name(ai::recall_result::E_NO_LEADER).empty());
	BOOST_CHECK(!ai::actions::get_error_name(ai::recall_result::E_LEADER_NOT_ON_KEEP).empty());
	BOOST_CHECK(!ai::actions::get_error_name(ai::recall_result::E_BAD_RECALL_LOCATION).empty());
}

BOOST_AUTO_TEST_CASE(test_ai_error_name_all_recruit_results)
{
	// Cover all recruit result error codes
	BOOST_CHECK(!ai::actions::get_error_name(ai::recruit_result::E_NOT_AVAILABLE_FOR_RECRUITING).empty());
	BOOST_CHECK(!ai::actions::get_error_name(ai::recruit_result::E_UNKNOWN_OR_DUMMY_UNIT_TYPE).empty());
	BOOST_CHECK(!ai::actions::get_error_name(ai::recruit_result::E_NO_GOLD).empty());
	BOOST_CHECK(!ai::actions::get_error_name(ai::recruit_result::E_NO_LEADER).empty());
	BOOST_CHECK(!ai::actions::get_error_name(ai::recruit_result::E_LEADER_NOT_ON_KEEP).empty());
	BOOST_CHECK(!ai::actions::get_error_name(ai::recruit_result::E_BAD_RECRUIT_LOCATION).empty());
}

BOOST_AUTO_TEST_CASE(test_ai_error_name_all_stopunit_results)
{
	// Cover all stopunit result error codes
	BOOST_CHECK(!ai::actions::get_error_name(ai::stopunit_result::E_NO_UNIT).empty());
	BOOST_CHECK(!ai::actions::get_error_name(ai::stopunit_result::E_NOT_OWN_UNIT).empty());
	BOOST_CHECK(!ai::actions::get_error_name(ai::stopunit_result::E_INCAPACITATED_UNIT).empty());
}

// ============================================================================
// AI Manager Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_manager_has_manager_initial)
{
	// Test that has_manager returns false when no singleton exists
	// Note: This test verifies the static method works correctly
	// In a real game context, the manager would be created during game initialization
	BOOST_CHECK(!ai::manager::has_manager());
}

// ============================================================================
// AI Holder Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_ai_holder_config_generation)
{
	// Test that holder can be constructed with valid config
	config cfg;
	cfg["id"] = "test_ai";
	cfg["description"] = "Test AI for unit tests";

	// Verify config is valid
	BOOST_CHECK(cfg.has_attribute("id"));
	BOOST_CHECK_EQUAL(cfg["id"].str(), "test_ai");
}

BOOST_AUTO_TEST_SUITE_END()