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

BOOST_AUTO_TEST_SUITE( generators )

// Test map generator configuration
BOOST_AUTO_TEST_CASE( test_generator_config_basic )
{
	config cfg;
	cfg["map_width"] = "40";
	cfg["map_height"] = "40";
	cfg["iterations"] = "1000";

	BOOST_CHECK_EQUAL(cfg["map_width"].str(), "40");
	BOOST_CHECK_EQUAL(cfg["map_height"].str(), "40");
	BOOST_CHECK_EQUAL(cfg["iterations"].str(), "1000");
}

// Test cave generator settings
BOOST_AUTO_TEST_CASE( test_cave_generator_settings )
{
	config cfg;
	cfg["village_density"] = "3";
	cfg["flipx_chance"] = "50";
	cfg["flipy_chance"] = "50";

	BOOST_CHECK_EQUAL(cfg["village_density"].str(), "3");
	BOOST_CHECK_EQUAL(cfg["flipx_chance"].str(), "50");
	BOOST_CHECK_EQUAL(cfg["flipy_chance"].str(), "50");
}

// Test default generator settings
BOOST_AUTO_TEST_CASE( test_default_generator_settings )
{
	config cfg;
	cfg["max_lakes"] = "20";
	cfg["min_lake_height"] = "200";
	cfg["lake_size"] = "10";

	BOOST_CHECK_EQUAL(cfg["max_lakes"].str(), "20");
	BOOST_CHECK_EQUAL(cfg["min_lake_height"].str(), "200");
	BOOST_CHECK_EQUAL(cfg["lake_size"].str(), "10");
}

// Test generator with scenarios
BOOST_AUTO_TEST_CASE( test_generator_scenarios )
{
	config cfg;
	cfg["id"] = "test_scenario";
	cfg["name"] = "Test Scenario";
	
	config& generator = cfg.add_child("generator");
	generator["name"] = "default";
	generator["map_width"] = "50";

	BOOST_CHECK(cfg.has_child("generator"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("generator")["name"].str(), "default");
}

// Test generator terrain settings
BOOST_AUTO_TEST_CASE( test_generator_terrain )
{
	config cfg;
	cfg["default_flatland"] = "Gg";
	cfg["default_hills"] = "Hh";
	cfg["default_mountains"] = "Mm";
	cfg["default_water"] = "Ww";

	BOOST_CHECK_EQUAL(cfg["default_flatland"].str(), "Gg");
	BOOST_CHECK_EQUAL(cfg["default_hills"].str(), "Hh");
	BOOST_CHECK_EQUAL(cfg["default_mountains"].str(), "Mm");
	BOOST_CHECK_EQUAL(cfg["default_water"].str(), "Ww");
}

// Test generator village settings
BOOST_AUTO_TEST_CASE( test_generator_villages )
{
	config cfg;
	cfg["villages"] = "10";
	cfg["village_density"] = "5";

	BOOST_CHECK_EQUAL(cfg["villages"].str(), "10");
	BOOST_CHECK_EQUAL(cfg["village_density"].str(), "5");
}

// Test generator castle settings
BOOST_AUTO_TEST_CASE( test_generator_castles )
{
	config cfg;
	cfg["castle_size"] = "9";
	cfg["players"] = "2";

	BOOST_CHECK_EQUAL(cfg["castle_size"].str(), "9");
	BOOST_CHECK_EQUAL(cfg["players"].str(), "2");
}

// Test generator empty
BOOST_AUTO_TEST_CASE( test_generator_empty )
{
	config cfg;

	BOOST_CHECK(cfg.empty());
	BOOST_CHECK(cfg["map_width"].empty());
}

// Test generator hill size settings
BOOST_AUTO_TEST_CASE( test_generator_hill_size )
{
	config cfg;
	cfg["hill_size"] = "5";
	cfg["hill_size_frequency"] = "high";

	BOOST_CHECK_EQUAL(cfg["hill_size"].str(), "5");
	BOOST_CHECK_EQUAL(cfg["hill_size_frequency"].str(), "high");
}

// Test generator island settings
BOOST_AUTO_TEST_CASE( test_generator_island_settings )
{
	config cfg;
	cfg["island_size"] = "100";
	cfg["island_off_center"] = "50";

	BOOST_CHECK_EQUAL(cfg["island_size"].str(), "100");
	BOOST_CHECK_EQUAL(cfg["island_off_center"].str(), "50");
}

// Test generator link_castles setting
BOOST_AUTO_TEST_CASE( test_generator_link_castles )
{
	config cfg;
	cfg["link_castles"] = "true";

	BOOST_CHECK_EQUAL(cfg["link_castles"].str(), "true");
}

// Test generator show_labels setting
BOOST_AUTO_TEST_CASE( test_generator_show_labels )
{
	config cfg;
	cfg["show_labels"] = "false";

	BOOST_CHECK_EQUAL(cfg["show_labels"].str(), "false");
}

// Test generator with different map sizes
BOOST_AUTO_TEST_CASE( test_generator_map_sizes )
{
	config small_cfg;
	small_cfg["map_width"] = "20";
	small_cfg["map_height"] = "20";
	BOOST_CHECK_EQUAL(small_cfg["map_width"].str(), "20");
	BOOST_CHECK_EQUAL(small_cfg["map_height"].str(), "20");

	config medium_cfg;
	medium_cfg["map_width"] = "40";
	medium_cfg["map_height"] = "40";
	BOOST_CHECK_EQUAL(medium_cfg["map_width"].str(), "40");
	BOOST_CHECK_EQUAL(medium_cfg["map_height"].str(), "40");

	config large_cfg;
	large_cfg["map_width"] = "100";
	large_cfg["map_height"] = "100";
	BOOST_CHECK_EQUAL(large_cfg["map_width"].str(), "100");
	BOOST_CHECK_EQUAL(large_cfg["map_height"].str(), "100");
}

// Test generator road settings
BOOST_AUTO_TEST_CASE( test_generator_road_settings )
{
	config cfg;
	cfg["road_windiness"] = "10";
	cfg["road_length"] = "50";

	BOOST_CHECK_EQUAL(cfg["road_windiness"].str(), "10");
	BOOST_CHECK_EQUAL(cfg["road_length"].str(), "50");
}

// Test generator river settings
BOOST_AUTO_TEST_CASE( test_generator_river_settings )
{
	config cfg;
	cfg["river_frequency"] = "medium";
	cfg["max_rivers"] = "5";

	BOOST_CHECK_EQUAL(cfg["river_frequency"].str(), "medium");
	BOOST_CHECK_EQUAL(cfg["max_rivers"].str(), "5");
}

// Test generator forest settings
BOOST_AUTO_TEST_CASE( test_generator_forest_settings )
{
	config cfg;
	cfg["forest_size"] = "4";
	cfg["forest_frequency"] = "high";

	BOOST_CHECK_EQUAL(cfg["forest_size"].str(), "4");
	BOOST_CHECK_EQUAL(cfg["forest_frequency"].str(), "high");
}

// Test generator swamp settings
BOOST_AUTO_TEST_CASE( test_generator_swamp_settings )
{
	config cfg;
	cfg["swamp_size"] = "3";
	cfg["swamp_frequency"] = "low";

	BOOST_CHECK_EQUAL(cfg["swamp_size"].str(), "3");
	BOOST_CHECK_EQUAL(cfg["swamp_frequency"].str(), "low");
}

// Test generator mountain settings
BOOST_AUTO_TEST_CASE( test_generator_mountain_settings )
{
	config cfg;
	cfg["mountain_size"] = "6";
	cfg["mountain_frequency"] = "medium";

	BOOST_CHECK_EQUAL(cfg["mountain_size"].str(), "6");
	BOOST_CHECK_EQUAL(cfg["mountain_frequency"].str(), "medium");
}

// Test generator snow settings
BOOST_AUTO_TEST_CASE( test_generator_snow_settings )
{
	config cfg;
	cfg["snow_size"] = "5";
	cfg["snow_frequency"] = "low";

	BOOST_CHECK_EQUAL(cfg["snow_size"].str(), "5");
	BOOST_CHECK_EQUAL(cfg["snow_frequency"].str(), "low");
}

// Test generator desert settings
BOOST_AUTO_TEST_CASE( test_generator_desert_settings )
{
	config cfg;
	cfg["desert_size"] = "4";
	cfg["desert_frequency"] = "medium";

	BOOST_CHECK_EQUAL(cfg["desert_size"].str(), "4");
	BOOST_CHECK_EQUAL(cfg["desert_frequency"].str(), "medium");
}

// Test generator village placement
BOOST_AUTO_TEST_CASE( test_generator_village_placement )
{
	config cfg;
	cfg["village_placement"] = "random";
	cfg["village_density"] = "3";

	BOOST_CHECK_EQUAL(cfg["village_placement"].str(), "random");
	BOOST_CHECK_EQUAL(cfg["village_density"].str(), "3");
}

// Test generator with multiple players
BOOST_AUTO_TEST_CASE( test_generator_multiplayer )
{
	config cfg;
	cfg["players"] = "4";
	cfg["teams"] = "2";

	BOOST_CHECK_EQUAL(cfg["players"].str(), "4");
	BOOST_CHECK_EQUAL(cfg["teams"].str(), "2");
}

// Test generator seed settings
BOOST_AUTO_TEST_CASE( test_generator_seed )
{
	config cfg;
	cfg["random_seed"] = "12345";

	BOOST_CHECK_EQUAL(cfg["random_seed"].str(), "12345");
}

// Test generator with scenario attributes
BOOST_AUTO_TEST_CASE( test_generator_scenario_attributes )
{
	config cfg;
	cfg["id"] = "scenario_01";
	cfg["name"] = "First Scenario";
	cfg["description"] = "The beginning of the campaign";
	cfg["next_scenario"] = "scenario_02";

	BOOST_CHECK_EQUAL(cfg["id"].str(), "scenario_01");
	BOOST_CHECK_EQUAL(cfg["name"].str(), "First Scenario");
	BOOST_CHECK_EQUAL(cfg["description"].str(), "The beginning of the campaign");
	BOOST_CHECK_EQUAL(cfg["next_scenario"].str(), "scenario_02");
}

// Test generator with time of day
BOOST_AUTO_TEST_CASE( test_generator_time_of_day )
{
	config cfg;
	cfg["time_of_day"] = "dawn";

	BOOST_CHECK_EQUAL(cfg["time_of_day"].str(), "dawn");
}

// Test generator with terrain masks
BOOST_AUTO_TEST_CASE( test_generator_terrain_mask )
{
	config cfg;
	cfg["mask"] = "maps/mask.map";
	cfg["mask_usage"] = "mask";

	BOOST_CHECK_EQUAL(cfg["mask"].str(), "maps/mask.map");
	BOOST_CHECK_EQUAL(cfg["mask_usage"].str(), "mask");
}

// Test generator with border settings
BOOST_AUTO_TEST_CASE( test_generator_border_settings )
{
	config cfg;
	cfg["border_size"] = "1";
	cfg["border_terrain"] = "Xv";

	BOOST_CHECK_EQUAL(cfg["border_size"].str(), "1");
	BOOST_CHECK_EQUAL(cfg["border_terrain"].str(), "Xv");
}

// Test generator with recall cost
BOOST_AUTO_TEST_CASE( test_generator_recall_cost )
{
	config cfg;
	cfg["recall_cost"] = "20";

	BOOST_CHECK_EQUAL(cfg["recall_cost"].str(), "20");
}

// Test generator with victory conditions
BOOST_AUTO_TEST_CASE( test_generator_victory_conditions )
{
	config cfg;
	cfg["victory_when_enemies_defeated"] = "yes";
	cfg["defeat_on_leaders_lost"] = "yes";

	BOOST_CHECK_EQUAL(cfg["victory_when_enemies_defeated"].str(), "yes");
	BOOST_CHECK_EQUAL(cfg["defeat_on_leaders_lost"].str(), "yes");
}

// Test generator with complex nested configuration
BOOST_AUTO_TEST_CASE( test_generator_complex_config )
{
	config cfg;
	cfg["map_width"] = "50";
	cfg["map_height"] = "50";
	cfg["iterations"] = "1000";
	cfg["hill_size"] = "5";
	cfg["villages"] = "15";
	cfg["players"] = "2";
	cfg["castle_size"] = "9";

	config& side = cfg.add_child("side");
	side["side"] = "1";
	side["team_name"] = "team1";
	side["canrecruit"] = "yes";

	BOOST_CHECK(cfg.has_child("side"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("side")["side"].str(), "1");
}

// Test generator with multiple sides
BOOST_AUTO_TEST_CASE( test_generator_multiple_sides )
{
	config cfg;

	for (int i = 1; i <= 4; ++i) {
		config& side = cfg.add_child("side");
		side["side"] = std::to_string(i);
		side["team_name"] = (i % 2 == 1) ? "odd" : "even";
		side["canrecruit"] = "yes";
	}

	BOOST_CHECK_EQUAL(cfg.child_count("side"), 4u);
}

// Test generator with event configuration
BOOST_AUTO_TEST_CASE( test_generator_events )
{
	config cfg;

	config& event = cfg.add_child("event");
	event["name"] = "start";
	event["first_time_only"] = "no";

	BOOST_CHECK(cfg.has_child("event"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("event")["name"].str(), "start");
}

// Test generator with label configuration
BOOST_AUTO_TEST_CASE( test_generator_labels )
{
	config cfg;

	config& label = cfg.add_child("label");
	label["x"] = "10";
	label["y"] = "10";
	label["text"] = "Start";

	BOOST_CHECK(cfg.has_child("label"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("label")["text"].str(), "Start");
}

// Test generator with item configuration
BOOST_AUTO_TEST_CASE( test_generator_items )
{
	config cfg;

	config& item = cfg.add_child("item");
	item["x"] = "5";
	item["y"] = "5";
	item["image"] = "items/potion.png";

	BOOST_CHECK(cfg.has_child("item"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("item")["image"].str(), "items/potion.png");
}

// Test generator with time schedule
BOOST_AUTO_TEST_CASE( test_generator_time_schedule )
{
	config cfg;

	config& time = cfg.add_child("time");
	time["id"] = "dawn";
	time["name"] = "Dawn";
	time["image"] = "misc/time-schedules/default/dawn.png";

	BOOST_CHECK(cfg.has_child("time"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("time")["id"].str(), "dawn");
}

// Test generator with music playlist
BOOST_AUTO_TEST_CASE( test_generator_music )
{
	config cfg;

	config& music = cfg.add_child("music");
	music["name"] = "background.ogg";
	music["ms_before"] = "5000";
	music["play_once"] = "no";

	BOOST_CHECK(cfg.has_child("music"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("music")["name"].str(), "background.ogg");
}

// Test generator with objectives
BOOST_AUTO_TEST_CASE( test_generator_objectives )
{
	config cfg;
	cfg["objectives"] = "Defeat the enemy leader";

	BOOST_CHECK_EQUAL(cfg["objectives"].str(), "Defeat the enemy leader");
}

// Test generator with turn limit
BOOST_AUTO_TEST_CASE( test_generator_turn_limit )
{
	config cfg;
	cfg["turns"] = "30";

	BOOST_CHECK_EQUAL(cfg["turns"].str(), "30");
}

// Test generator with gold settings
BOOST_AUTO_TEST_CASE( test_generator_gold )
{
	config cfg;
	cfg["gold"] = "100";
	cfg["income"] = "2";

	BOOST_CHECK_EQUAL(cfg["gold"].str(), "100");
	BOOST_CHECK_EQUAL(cfg["income"].str(), "2");
}

// Test generator with fog and shroud
BOOST_AUTO_TEST_CASE( test_generator_fog_shroud )
{
	config cfg;
	cfg["fog"] = "yes";
	cfg["shroud"] = "no";

	BOOST_CHECK_EQUAL(cfg["fog"].str(), "yes");
	BOOST_CHECK_EQUAL(cfg["shroud"].str(), "no");
}

// Test generator with village gold and support
BOOST_AUTO_TEST_CASE( test_generator_village_gold )
{
	config cfg;
	cfg["village_gold"] = "1";
	cfg["village_support"] = "1";

	BOOST_CHECK_EQUAL(cfg["village_gold"].str(), "1");
	BOOST_CHECK_EQUAL(cfg["village_support"].str(), "1");
}

// Test generator with experience modifier
BOOST_AUTO_TEST_CASE( test_generator_xp_modifier )
{
	config cfg;
	cfg["experience_modifier"] = "100";

	BOOST_CHECK_EQUAL(cfg["experience_modifier"].str(), "100");
}

// Test generator with leader settings
BOOST_AUTO_TEST_CASE( test_generator_leader )
{
	config cfg;
	cfg["type"] = "Commander";
	cfg["placement"] = "leader";

	BOOST_CHECK_EQUAL(cfg["type"].str(), "Commander");
	BOOST_CHECK_EQUAL(cfg["placement"].str(), "leader");
}

// Test generator with recruit list
BOOST_AUTO_TEST_CASE( test_generator_recruit )
{
	config cfg;
	cfg["recruit"] = "Elvish Fighter,Elvish Archer";

	BOOST_CHECK_EQUAL(cfg["recruit"].str(), "Elvish Fighter,Elvish Archer");
}

// Test generator with terrain alterations
BOOST_AUTO_TEST_CASE( test_generator_terrain_alterations )
{
	config cfg;

	config& terrain = cfg.add_child("terrain");
	terrain["x"] = "15";
	terrain["y"] = "15";
	terrain["terrain"] = "Gg";

	BOOST_CHECK(cfg.has_child("terrain"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("terrain")["terrain"].str(), "Gg");
}

// Test generator with unit placement
BOOST_AUTO_TEST_CASE( test_generator_unit_placement )
{
	config cfg;

	config& unit = cfg.add_child("unit");
	unit["type"] = "Elvish Fighter";
	unit["x"] = "10";
	unit["y"] = "10";
	unit["side"] = "1";

	BOOST_CHECK(cfg.has_child("unit"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("unit")["type"].str(), "Elvish Fighter");
}

// Test generator with random start time
BOOST_AUTO_TEST_CASE( test_generator_random_start )
{
	config cfg;
	cfg["random_start_time"] = "yes";

	BOOST_CHECK_EQUAL(cfg["random_start_time"].str(), "yes");
}

// Test generator with map_data
BOOST_AUTO_TEST_CASE( test_generator_map_data )
{
	config cfg;
	cfg["map_data"] = "Gg, Gg, Gg\nGg, Gg, Gg";

	BOOST_CHECK_EQUAL(cfg["map_data"].str(), "Gg, Gg, Gg\nGg, Gg, Gg");
}

BOOST_AUTO_TEST_SUITE_END()
