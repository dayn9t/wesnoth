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
#include "game_events/handlers.hpp"
#include "game_events/pump.hpp"
#include "map/location.hpp"

BOOST_AUTO_TEST_SUITE( game_events_extended )

// ============================================================================
// Event Handler Configuration Tests
// ============================================================================

// Test event handler with priority
BOOST_AUTO_TEST_CASE( test_event_handler_priority )
{
	config cfg;
	cfg["name"] = "test_event";
	cfg["priority"] = "5.5";

	BOOST_CHECK_EQUAL(cfg["name"].str(), "test_event");
	BOOST_CHECK_EQUAL(cfg["priority"].to_double(), 5.5);
}

// Test event handler with negative priority
BOOST_AUTO_TEST_CASE( test_event_handler_negative_priority )
{
	config cfg;
	cfg["name"] = "late_event";
	cfg["priority"] = "-10.0";

	BOOST_CHECK_EQUAL(cfg["priority"].to_double(), -10.0);
}

// Test event handler with empty name and id
BOOST_AUTO_TEST_CASE( test_event_handler_empty_identifiers )
{
	config cfg;
	cfg["name"] = "";
	cfg["id"] = "";

	BOOST_CHECK(cfg["name"].empty());
	BOOST_CHECK(cfg["id"].empty());
}

// Test event handler first_time_only false
BOOST_AUTO_TEST_CASE( test_event_handler_repeatable )
{
	config cfg;
	cfg["name"] = "repeating_event";
	cfg["first_time_only"] = "no";

	BOOST_CHECK_EQUAL(cfg["first_time_only"].to_bool(), false);
}

// Test event handler with complex id
BOOST_AUTO_TEST_CASE( test_event_handler_complex_id )
{
	config cfg;
	cfg["name"] = "attack";
	cfg["id"] = "my_scenario_special_attack_event_001";

	BOOST_CHECK_EQUAL(cfg["id"].str(), "my_scenario_special_attack_event_001");
}

// Test event with multiple action tags
BOOST_AUTO_TEST_CASE( test_event_multiple_actions )
{
	config cfg;
	cfg["name"] = "turn_start";

	config& sound = cfg.add_child("sound");
	sound["name"] = "ambient/morning.ogg";

	config& modify_unit = cfg.add_child("modify_unit");
	modify_unit["type"] = "Elvish Fighter";

	config& message = cfg.add_child("message");
	message["speaker"] = "narrator";
	message["message"] = "A new turn begins!";

	BOOST_CHECK(cfg.has_child("sound"));
	BOOST_CHECK(cfg.has_child("modify_unit"));
	BOOST_CHECK(cfg.has_child("message"));
	BOOST_CHECK_EQUAL(cfg.child_count("message"), 1u);
}

// Test event with fire_event action
BOOST_AUTO_TEST_CASE( test_event_fire_event_action )
{
	config cfg;
	cfg["name"] = "pre_start";

	config& fire_event = cfg.add_child("fire_event");
	fire_event["name"] = "custom_event";

	config& filter = fire_event.add_child("filter");
	filter["x"] = "10";
	filter["y"] = "15";

	BOOST_CHECK(cfg.has_child("fire_event"));
	BOOST_CHECK(cfg.mandatory_child("fire_event").has_child("filter"));
}

// ============================================================================
// Condition Filtering Tests
// ============================================================================

// Test condition with variable equals
BOOST_AUTO_TEST_CASE( test_condition_variable_equals )
{
	config condition;
	config& variable = condition.add_child("variable");
	variable["name"] = "gold_amount";
	variable["equals"] = "100";

	BOOST_CHECK(condition.has_child("variable"));
	BOOST_CHECK_EQUAL(condition.mandatory_child("variable")["name"].str(), "gold_amount");
	BOOST_CHECK_EQUAL(condition.mandatory_child("variable")["equals"].str(), "100");
}

// Test condition with variable greater_than
BOOST_AUTO_TEST_CASE( test_condition_variable_comparison )
{
	config condition;
	config& variable = condition.add_child("variable");
	variable["name"] = "turn_number";
	variable["greater_than"] = "5";

	BOOST_CHECK_EQUAL(condition.mandatory_child("variable")["greater_than"].str(), "5");
}

// Test condition with boolean comparison
BOOST_AUTO_TEST_CASE( test_condition_boolean )
{
	config condition;
	config& variable = condition.add_child("variable");
	variable["name"] = "quest_completed";
	variable["boolean_equals"] = "yes";

	BOOST_CHECK_EQUAL(condition.mandatory_child("variable")["boolean_equals"].to_bool(), true);
}

// Test condition with numerical_not_equals
BOOST_AUTO_TEST_CASE( test_condition_numerical_not_equals )
{
	config condition;
	config& variable = condition.add_child("variable");
	variable["name"] = "enemy_count";
	variable["numerical_not_equals"] = "0";

	BOOST_CHECK_EQUAL(condition.mandatory_child("variable")["numerical_not_equals"].str(), "0");
}

// Test condition with contains
BOOST_AUTO_TEST_CASE( test_condition_string_contains )
{
	config condition;
	config& variable = condition.add_child("variable");
	variable["name"] = "unit_name";
	variable["contains"] = "Elf";

	BOOST_CHECK_EQUAL(condition.mandatory_child("variable")["contains"].str(), "Elf");
}

// Test condition with less_than_equal_to
BOOST_AUTO_TEST_CASE( test_condition_less_than_equal )
{
	config condition;
	config& variable = condition.add_child("variable");
	variable["name"] = "unit_hp";
	variable["less_than_equal_to"] = "50";

	BOOST_CHECK_EQUAL(condition.mandatory_child("variable")["less_than_equal_to"].str(), "50");
}

// Test condition with greater_than_equal_to
BOOST_AUTO_TEST_CASE( test_condition_greater_than_equal )
{
	config condition;
	config& variable = condition.add_child("variable");
	variable["name"] = "experience";
	variable["greater_than_equal_to"] = "20";

	BOOST_CHECK_EQUAL(condition.mandatory_child("variable")["greater_than_equal_to"].str(), "20");
}

// Test condition with not_equals
BOOST_AUTO_TEST_CASE( test_condition_string_not_equals )
{
	config condition;
	config& variable = condition.add_child("variable");
	variable["name"] = "unit_type";
	variable["not_equals"] = "Elvish Scout";

	BOOST_CHECK_EQUAL(condition.mandatory_child("variable")["not_equals"].str(), "Elvish Scout");
}

// ============================================================================
// Filter Configuration Tests
// ============================================================================

// Test filter with side condition
BOOST_AUTO_TEST_CASE( test_filter_side_condition )
{
	config cfg;
	cfg["name"] = "turn_refresh";

	config& filter_side = cfg.add_child("filter_side");
	filter_side["side"] = "1,2,3";
	filter_side["team_name"] = "good guys";

	BOOST_CHECK(cfg.has_child("filter_side"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("filter_side")["side"].str(), "1,2,3");
}

// Test filter with attack type
BOOST_AUTO_TEST_CASE( test_filter_attack_type )
{
	config cfg;
	cfg["name"] = "attack";

	config& filter_attack = cfg.add_child("filter_attack");
	filter_attack["name"] = "sword";
	filter_attack["range"] = "melee";

	BOOST_CHECK(cfg.has_child("filter_attack"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("filter_attack")["name"].str(), "sword");
}

// Test filter with filter_second_attack
BOOST_AUTO_TEST_CASE( test_filter_second_attack )
{
	config cfg;
	cfg["name"] = "defender_hits";

	config& filter_second_attack = cfg.add_child("filter_second_attack");
	filter_second_attack["type"] = "fire";
	filter_second_attack["damage"] = "10";

	BOOST_CHECK(cfg.has_child("filter_second_attack"));
}

// Test filter formula
BOOST_AUTO_TEST_CASE( test_filter_formula )
{
	config cfg;
	cfg["name"] = "moveto";
	cfg["filter_formula"] = "unit.level > 2 and unit.hitpoints < unit.max_hitpoints";

	BOOST_CHECK(cfg.has_attribute("filter_formula"));
	BOOST_CHECK_EQUAL(cfg["filter_formula"].str(), "unit.level > 2 and unit.hitpoints < unit.max_hitpoints");
}

// Test filter_condition block
BOOST_AUTO_TEST_CASE( test_filter_condition_block )
{
	config cfg;
	cfg["name"] = "attack_end";

	config& filter_condition = cfg.add_child("filter_condition");
	config& have_unit = filter_condition.add_child("have_unit");
	have_unit["type"] = "Elvish Shaman";
	have_unit["side"] = "1";

	BOOST_CHECK(cfg.has_child("filter_condition"));
	BOOST_CHECK(cfg.mandatory_child("filter_condition").has_child("have_unit"));
}

// ============================================================================
// Trigger Management Tests
// ============================================================================

// Test queued_event data structure
BOOST_AUTO_TEST_CASE( test_queued_event_structure )
{
	config data;
	data["damage"] = "15";
	data["hit"] = "yes";

	game_events::queued_event ev(
		"attack hits",
		"attack_id_001",
		map_location(5, 10),
		map_location(6, 10),
		data
	);

	BOOST_CHECK_EQUAL(ev.name, "attack_hits"); // spaces replaced with underscores
	BOOST_CHECK_EQUAL(ev.id, "attack_id_001");
	BOOST_CHECK_EQUAL(ev.data["damage"].str(), "15");
}

// Test queued_event with empty id
BOOST_AUTO_TEST_CASE( test_queued_event_empty_id )
{
	game_events::queued_event ev(
		"moveto",
		"",
		map_location(1, 1),
		map_location(2, 2),
		config()
	);

	BOOST_CHECK_EQUAL(ev.name, "moveto");
	BOOST_CHECK(ev.id.empty());
}

// Test event with weapon data
BOOST_AUTO_TEST_CASE( test_event_weapon_data )
{
	config data;

	config& first_weapon = data.add_child("first");
	first_weapon["name"] = "bow";
	first_weapon["type"] = "pierce";
	first_weapon["damage"] = "8";
	first_weapon["number"] = "4";

	config& second_weapon = data.add_child("second");
	second_weapon["name"] = "claws";
	second_weapon["type"] = "blade";
	second_weapon["damage"] = "6";
	second_weapon["number"] = "2";

	game_events::queued_event ev(
		"attack",
		"",
		map_location(0, 0),
		map_location(1, 0),
		data
	);

	BOOST_CHECK(ev.data.has_child("first"));
	BOOST_CHECK(ev.data.has_child("second"));
	BOOST_CHECK_EQUAL(ev.data.mandatory_child("first")["name"].str(), "bow");
	BOOST_CHECK_EQUAL(ev.data.mandatory_child("second")["name"].str(), "claws");
}

// Test event serialization
BOOST_AUTO_TEST_CASE( test_event_write_config )
{
	game_events::event_handler handler("test_event", "test_id_001");
	handler.set_priority(3.5);
	handler.set_repeatable(true);

	config args;
	args["test_attr"] = "test_value";
	handler.set_arguments(args);

	config output_cfg;
	handler.write_config(output_cfg);

	BOOST_CHECK_EQUAL(output_cfg["name"].str(), "test_event");
	BOOST_CHECK_EQUAL(output_cfg["id"].str(), "test_id_001");
	BOOST_CHECK_EQUAL(output_cfg["priority"].to_double(), 3.5);
	BOOST_CHECK_EQUAL(output_cfg["first_time_only"].to_bool(), false);
}

// Test event handler disabled state
BOOST_AUTO_TEST_CASE( test_event_handler_disabled_state )
{
	game_events::event_handler handler("one_time_event", "one_time_id");

	BOOST_CHECK(!handler.disabled());
	BOOST_CHECK(handler.repeatable() == false); // default is first_time_only=true

	handler.disable();
	BOOST_CHECK(handler.disabled());
}

// Test event handler with multiple filter types
BOOST_AUTO_TEST_CASE( test_event_handler_multiple_filters )
{
	config cfg;
	cfg["name"] = "complex_attack_event";

	config& filter = cfg.add_child("filter");
	filter["type"] = "Elvish Fighter";
	filter["side"] = "1";

	config& filter_second = cfg.add_child("filter_second");
	filter_second["type"] = "Orcish Grunt";

	config& filter_attack = cfg.add_child("filter_attack");
	filter_attack["name"] = "sword";

	config& filter_condition = cfg.add_child("filter_condition");
	config& variable = filter_condition.add_child("variable");
	variable["name"] = "turn_number";
	variable["greater_than"] = "3";

	BOOST_CHECK(cfg.has_child("filter"));
	BOOST_CHECK(cfg.has_child("filter_second"));
	BOOST_CHECK(cfg.has_child("filter_attack"));
	BOOST_CHECK(cfg.has_child("filter_condition"));
}

// Test event with insert_tag dynamic filter
BOOST_AUTO_TEST_CASE( test_event_insert_tag_filter )
{
	config cfg;
	cfg["name"] = "dynamic_filter_event";

	config& insert_tag = cfg.add_child("insert_tag");
	insert_tag["name"] = "filter";
	insert_tag["variable"] = "dynamic_filter_config";

	BOOST_CHECK(cfg.has_child("insert_tag"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("insert_tag")["name"].str(), "filter");
	BOOST_CHECK_EQUAL(cfg.mandatory_child("insert_tag")["variable"].str(), "dynamic_filter_config");
}

// Test event with have_unit condition
BOOST_AUTO_TEST_CASE( test_event_have_unit_condition )
{
	config condition;

	config& have_unit = condition.add_child("have_unit");
	have_unit["type"] = "Elvish Champion";
	have_unit["side"] = "1";
	have_unit["count"] = "2";

	BOOST_CHECK(condition.has_child("have_unit"));
	BOOST_CHECK_EQUAL(condition.mandatory_child("have_unit")["type"].str(), "Elvish Champion");
	BOOST_CHECK_EQUAL(condition.mandatory_child("have_unit")["count"].str(), "2");
}

// Test event with have_location condition
BOOST_AUTO_TEST_CASE( test_event_have_location_condition )
{
	config condition;

	config& have_location = condition.add_child("have_location");
	have_location["terrain"] = "Gg";
	have_location["x"] = "1-10";
	have_location["y"] = "1-10";
	have_location["count"] = "5-10";

	BOOST_CHECK(condition.has_child("have_location"));
	BOOST_CHECK_EQUAL(condition.mandatory_child("have_location")["terrain"].str(), "Gg");
}

// Test event with complex nested OR condition
BOOST_AUTO_TEST_CASE( test_event_complex_or_condition )
{
	config cfg;
	cfg["name"] = "complex_or_event";

	config& condition = cfg.add_child("condition");

	// First condition (base)
	config& var1 = condition.add_child("variable");
	var1["name"] = "flag_a";
	var1["boolean_equals"] = "yes";

	// OR branch
	config& or_cond = condition.add_child("or");
	config& or_var = or_cond.add_child("variable");
	or_var["name"] = "flag_b";
	or_var["boolean_equals"] = "yes";

	// Another OR branch
	config& or_cond2 = condition.add_child("or");
	config& or_var2 = or_cond2.add_child("variable");
	or_var2["name"] = "flag_c";
	or_var2["boolean_equals"] = "yes";

	BOOST_CHECK(cfg.has_child("condition"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("condition").child_count("or"), 2u);
}

// Test event with complex nested AND condition
BOOST_AUTO_TEST_CASE( test_event_complex_and_condition )
{
	config cfg;
	cfg["name"] = "complex_and_event";

	config& condition = cfg.add_child("condition");

	// First condition (base)
	config& var1 = condition.add_child("variable");
	var1["name"] = "player_gold";
	var1["greater_than"] = "50";

	// AND branch
	config& and_cond = condition.add_child("and");
	config& and_var = and_cond.add_child("variable");
	and_var["name"] = "turn_number";
	and_var["less_than"] = "10";

	BOOST_CHECK(cfg.has_child("condition"));
	BOOST_CHECK(cfg.mandatory_child("condition").has_child("and"));
}

// Test event with NOT condition
BOOST_AUTO_TEST_CASE( test_event_not_condition )
{
	config cfg;
	cfg["name"] = "not_condition_event";

	config& condition = cfg.add_child("condition");

	config& not_cond = condition.add_child("not");
	config& not_var = not_cond.add_child("variable");
	not_var["name"] = "boss_defeated";
	not_var["boolean_equals"] = "yes";

	BOOST_CHECK(cfg.has_child("condition"));
	BOOST_CHECK(cfg.mandatory_child("condition").has_child("not"));
}

// Test event handler menu item flag
BOOST_AUTO_TEST_CASE( test_event_handler_menu_item )
{
	game_events::event_handler handler("menu_event", "menu_id_001");
	handler.set_menu_item(true);

	BOOST_CHECK(handler.is_menu_item());
	BOOST_CHECK_EQUAL(handler.id(), "menu_id_001");
}

// Test event with true/false conditions
BOOST_AUTO_TEST_CASE( test_event_true_false_conditions )
{
	config true_cfg;
	true_cfg.add_child("true");

	config false_cfg;
	false_cfg.add_child("false");

	BOOST_CHECK(true_cfg.has_child("true"));
	BOOST_CHECK(false_cfg.has_child("false"));
}

// Test event with empty filter
BOOST_AUTO_TEST_CASE( test_event_empty_filter )
{
	config cfg;
	cfg["name"] = "unfiltered_event";

	config& filter = cfg.add_child("filter");
	// Empty filter - should match everything

	BOOST_CHECK(cfg.has_child("filter"));
	BOOST_CHECK(cfg.mandatory_child("filter").empty());
}

// Test event handler arguments preservation
BOOST_AUTO_TEST_CASE( test_event_handler_arguments )
{
	game_events::event_handler handler("args_event", "args_id");

	config args;
	args["custom_flag"] = "enabled";
	args["difficulty"] = "hard";

	config& nested = args.add_child("nested_action");
	nested["type"] = "heal";
	nested["value"] = "10";

	handler.set_arguments(args);

	const config& stored_args = handler.arguments();
	BOOST_CHECK_EQUAL(stored_args["custom_flag"].str(), "enabled");
	BOOST_CHECK_EQUAL(stored_args["difficulty"].str(), "hard");
	BOOST_CHECK(stored_args.has_child("nested_action"));
}

// Test event with range-based variable check
BOOST_AUTO_TEST_CASE( test_condition_range_check )
{
	config condition;
	config& variable = condition.add_child("variable");
	variable["name"] = "unit_count";
	variable["numerical_equals"] = "5";

	// Test that we can construct valid variable checks
	BOOST_CHECK(condition.has_child("variable"));
	BOOST_CHECK_EQUAL(condition.mandatory_child("variable")["name"].str(), "unit_count");
}

// Test event handler empty check
BOOST_AUTO_TEST_CASE( test_event_handler_empty_check )
{
	game_events::event_handler handler("empty_test", "empty_id");
	// No arguments set

	BOOST_CHECK(handler.empty());

	config args;
	args["action"] = "test";
	handler.set_arguments(args);

	BOOST_CHECK(!handler.empty());
}

BOOST_AUTO_TEST_SUITE_END()
