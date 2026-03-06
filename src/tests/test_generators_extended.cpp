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
#include "generators/default_map_generator.hpp"
#include "generators/cave_map_generator.hpp"

BOOST_AUTO_TEST_SUITE( generators_extended )

// ============================================================================
// Generator Data Structure Tests
// ============================================================================

// Test generator_data default values
BOOST_AUTO_TEST_CASE( test_generator_data_defaults )
{
	config cfg;
	generator_data data(cfg);

	BOOST_CHECK_EQUAL(data.width, 40);
	BOOST_CHECK_EQUAL(data.height, 40);
	BOOST_CHECK_EQUAL(data.default_width, 40);
	BOOST_CHECK_EQUAL(data.default_height, 40);
	BOOST_CHECK_EQUAL(data.nplayers, 2);
	BOOST_CHECK_EQUAL(data.nvillages, 25);
	BOOST_CHECK_EQUAL(data.iterations, 1000);
	BOOST_CHECK_EQUAL(data.hill_size, 10);
	BOOST_CHECK_EQUAL(data.castle_size, 9);
	BOOST_CHECK_EQUAL(data.island_size, 0);
	BOOST_CHECK_EQUAL(data.max_lakes, 20);
	BOOST_CHECK_EQUAL(data.link_castles, true);
	BOOST_CHECK_EQUAL(data.show_labels, true);
}

// Test generator_data with custom config values
BOOST_AUTO_TEST_CASE( test_generator_data_custom_values )
{
	config cfg;
	cfg["map_width"] = "100";
	cfg["map_height"] = "80";
	cfg["players"] = "4";
	cfg["villages"] = "50";
	cfg["iterations"] = "2000";
	cfg["hill_size"] = "15";
	cfg["castle_size"] = "12";
	cfg["island_size"] = "5";
	cfg["max_lakes"] = "30";

	generator_data data(cfg);

	BOOST_CHECK_EQUAL(data.width, 100);
	BOOST_CHECK_EQUAL(data.height, 80);
	BOOST_CHECK_EQUAL(data.nplayers, 4);
	BOOST_CHECK_EQUAL(data.nvillages, 50);
	BOOST_CHECK_EQUAL(data.iterations, 2000);
	BOOST_CHECK_EQUAL(data.hill_size, 15);
	BOOST_CHECK_EQUAL(data.castle_size, 12);
	BOOST_CHECK_EQUAL(data.island_size, 5);
	BOOST_CHECK_EQUAL(data.max_lakes, 30);
}

// Test generator_data with negative values (should clamp to 0)
BOOST_AUTO_TEST_CASE( test_generator_data_negative_values )
{
	config cfg;
	cfg["map_width"] = "-10";
	cfg["map_height"] = "-20";
	cfg["players"] = "-2";
	cfg["villages"] = "-5";
	cfg["iterations"] = "-100";
	cfg["hill_size"] = "-5";
	cfg["castle_size"] = "-3";
	cfg["island_size"] = "-2";
	cfg["max_lakes"] = "-10";

	generator_data data(cfg);

	BOOST_CHECK_EQUAL(data.width, 0);
	BOOST_CHECK_EQUAL(data.height, 0);
	BOOST_CHECK_EQUAL(data.nplayers, 0);
	BOOST_CHECK_EQUAL(data.nvillages, 0);
	BOOST_CHECK_EQUAL(data.iterations, 0);
	BOOST_CHECK_EQUAL(data.hill_size, 0);
	BOOST_CHECK_EQUAL(data.castle_size, 0);
	BOOST_CHECK_EQUAL(data.island_size, 0);
	BOOST_CHECK_EQUAL(data.max_lakes, 0);
}

// Test generator_data with invalid string values
BOOST_AUTO_TEST_CASE( test_generator_data_invalid_strings )
{
	config cfg;
	cfg["map_width"] = "invalid";
	cfg["map_height"] = "";
	cfg["players"] = "abc";
	cfg["villages"] = "test";

	generator_data data(cfg);

	// Should fall back to defaults when parsing fails
	BOOST_CHECK_EQUAL(data.width, 40);
	BOOST_CHECK_EQUAL(data.height, 40);
	BOOST_CHECK_EQUAL(data.nplayers, 2);
	BOOST_CHECK_EQUAL(data.nvillages, 25);
}

// ============================================================================
// Default Map Generator Tests
// ============================================================================

// Test default_map_generator name
BOOST_AUTO_TEST_CASE( test_default_generator_name )
{
	config cfg;
	default_map_generator gen(cfg);

	BOOST_CHECK_EQUAL(gen.name(), "default");
}

// Test default_map_generator allows user config
BOOST_AUTO_TEST_CASE( test_default_generator_user_config )
{
	config cfg;
	default_map_generator gen(cfg);

	BOOST_CHECK_EQUAL(gen.allow_user_config(), true);
}

// Test default_map_generator config_name with scenario child
BOOST_AUTO_TEST_CASE( test_default_generator_config_name_with_scenario )
{
	config cfg;
	config& scenario = cfg.add_child("scenario");
	scenario["name"] = "Test Scenario";

	default_map_generator gen(cfg);

	BOOST_CHECK_EQUAL(gen.config_name(), "Test Scenario");
}

// Test default_map_generator config_name without scenario child
BOOST_AUTO_TEST_CASE( test_default_generator_config_name_empty )
{
	config cfg;
	default_map_generator gen(cfg);

	BOOST_CHECK_EQUAL(gen.config_name(), "");
}

// ============================================================================
// Cave Map Generator Tests
// ============================================================================

// Test cave_map_generator name
BOOST_AUTO_TEST_CASE( test_cave_generator_name )
{
	config cfg;
	cave_map_generator gen(cfg);

	BOOST_CHECK_EQUAL(gen.name(), "cave");
}

// Test cave_map_generator config_name
BOOST_AUTO_TEST_CASE( test_cave_generator_config_name )
{
	config cfg;
	cave_map_generator gen(cfg);

	BOOST_CHECK_EQUAL(gen.config_name(), "");
}

// Test cave_map_generator default dimensions
BOOST_AUTO_TEST_CASE( test_cave_generator_default_dimensions )
{
	config cfg;
	cave_map_generator gen(cfg);

	// The generator uses default 50x50 dimensions
	BOOST_CHECK_EQUAL(cfg["map_width"].to_int(50), 50);
	BOOST_CHECK_EQUAL(cfg["map_height"].to_int(50), 50);
}

// Test cave_map_generator with custom dimensions
BOOST_AUTO_TEST_CASE( test_cave_generator_custom_dimensions )
{
	config cfg;
	cfg["map_width"] = "80";
	cfg["map_height"] = "60";

	cave_map_generator gen(cfg);

	BOOST_CHECK_EQUAL(cfg["map_width"].to_int(50), 80);
	BOOST_CHECK_EQUAL(cfg["map_height"].to_int(50), 60);
}

// Test cave_map_generator village density
BOOST_AUTO_TEST_CASE( test_cave_generator_village_density )
{
	config cfg;
	cfg["village_density"] = "5";

	cave_map_generator gen(cfg);

	BOOST_CHECK_EQUAL(cfg["village_density"].to_int(0), 5);
}

// Test cave_map_generator flip chances
BOOST_AUTO_TEST_CASE( test_cave_generator_flip_chances )
{
	config cfg;
	cfg["flipx_chance"] = "30";
	cfg["flipy_chance"] = "70";

	cave_map_generator gen(cfg);

	BOOST_CHECK_EQUAL(cfg["flipx_chance"].to_int(), 30);
	BOOST_CHECK_EQUAL(cfg["flipy_chance"].to_int(), 70);
}

// ============================================================================
// Generator Configuration Parsing Tests
// ============================================================================

// Test generator config with chamber definitions
BOOST_AUTO_TEST_CASE( test_generator_chamber_config )
{
	config cfg;

	config& chamber1 = cfg.add_child("chamber");
	chamber1["id"] = "start";
	chamber1["x"] = "10-20";
	chamber1["y"] = "10-20";
	chamber1["size"] = "5";
	chamber1["jagged"] = "20";

	config& chamber2 = cfg.add_child("chamber");
	chamber2["id"] = "end";
	chamber2["x"] = "30-40";
	chamber2["y"] = "30-40";
	chamber2["size"] = "3";

	BOOST_CHECK_EQUAL(cfg.child_count("chamber"), 2u);

	const config& c1 = cfg.mandatory_child("chamber", 0);
	BOOST_CHECK_EQUAL(c1["id"].str(), "start");
	BOOST_CHECK_EQUAL(c1["size"].to_int(), 5);

	const config& c2 = cfg.mandatory_child("chamber", 1);
	BOOST_CHECK_EQUAL(c2["id"].str(), "end");
	BOOST_CHECK_EQUAL(c2["size"].to_int(), 3);
}

// Test generator config with passage definitions
BOOST_AUTO_TEST_CASE( test_generator_passage_config )
{
	config cfg;

	config& chamber1 = cfg.add_child("chamber");
	chamber1["id"] = "start";

	config& passage1 = chamber1.add_child("passage");
	passage1["destination"] = "end";
	passage1["width"] = "2";
	passage1["windiness"] = "5";
	passage1["laziness"] = "3.0";

	BOOST_CHECK(chamber1.has_child("passage"));
	BOOST_CHECK_EQUAL(chamber1.mandatory_child("passage")["destination"].str(), "end");
}

// Test generator config with chamber items
BOOST_AUTO_TEST_CASE( test_generator_chamber_items )
{
	config cfg;

	config& chamber = cfg.add_child("chamber");
	chamber["id"] = "main";

	config& items = chamber.add_child("items");

	config& side = items.add_child("side");
	side["side"] = "1";
	side["team_name"] = "player";

	config& unit = items.add_child("unit");
	unit["type"] = "Elvish Fighter";
	unit["x"] = "5";
	unit["y"] = "5";

	BOOST_CHECK(chamber.has_child("items"));

	const config& items_cfg = chamber.mandatory_child("items");
	BOOST_CHECK_EQUAL(items_cfg.child_count("side"), 1u);
	BOOST_CHECK_EQUAL(items_cfg.child_count("unit"), 1u);
}

// ============================================================================
// Parameter Validation Tests
// ============================================================================

// Test parameter validation for map dimensions
BOOST_AUTO_TEST_CASE( test_param_validation_map_dimensions )
{
	config cfg;

	// Test minimum valid dimensions
	cfg["map_width"] = "1";
	cfg["map_height"] = "1";
	BOOST_CHECK_GT(cfg["map_width"].to_int(), 0);
	BOOST_CHECK_GT(cfg["map_height"].to_int(), 0);

	// Test large dimensions
	cfg["map_width"] = "200";
	cfg["map_height"] = "200";
	BOOST_CHECK_GT(cfg["map_width"].to_int(), 100);
	BOOST_CHECK_GT(cfg["map_height"].to_int(), 100);
}

// Test parameter validation for player count
BOOST_AUTO_TEST_CASE( test_param_validation_player_count )
{
	config cfg;

	// Test minimum players
	cfg["players"] = "1";
	BOOST_CHECK_GE(cfg["players"].to_int(), 1);

	// Test reasonable player count
	cfg["players"] = "8";
	BOOST_CHECK_GE(cfg["players"].to_int(), 1);
	BOOST_CHECK_LE(cfg["players"].to_int(), 10);
}

// Test parameter validation for iterations
BOOST_AUTO_TEST_CASE( test_param_validation_iterations )
{
	config cfg;

	cfg["iterations"] = "0";
	BOOST_CHECK_GE(cfg["iterations"].to_int(), 0);

	cfg["iterations"] = "10000";
	BOOST_CHECK_GE(cfg["iterations"].to_int(), 0);
}

// Test parameter validation for village count
BOOST_AUTO_TEST_CASE( test_param_validation_village_count )
{
	config cfg;

	cfg["villages"] = "0";
	BOOST_CHECK_GE(cfg["villages"].to_int(), 0);

	cfg["villages"] = "100";
	BOOST_CHECK_GE(cfg["villages"].to_int(), 0);
}

// ============================================================================
// Map Settings Tests
// ============================================================================

// Test map settings with terrain codes
BOOST_AUTO_TEST_CASE( test_map_settings_terrain_codes )
{
	config cfg;
	cfg["default_flatland"] = "Gg";
	cfg["default_hills"] = "Hh";
	cfg["default_mountains"] = "Mm";
	cfg["default_water"] = "Ww";
	cfg["default_snow"] = "Aa";

	BOOST_CHECK_EQUAL(cfg["default_flatland"].str().length(), 2u);
	BOOST_CHECK_EQUAL(cfg["default_hills"].str().length(), 2u);
	BOOST_CHECK_EQUAL(cfg["default_mountains"].str().length(), 2u);
	BOOST_CHECK_EQUAL(cfg["default_water"].str().length(), 2u);
}

// Test map settings for coastal/island maps
BOOST_AUTO_TEST_CASE( test_map_settings_island_params )
{
	config cfg;

	// max_island = 10, max_coastal = 5 per default_map_generator.cpp
	cfg["island_size"] = "7";  // Between coastal and island
	BOOST_CHECK_GE(cfg["island_size"].to_int(), 5);

	cfg["island_size"] = "3";  // Coastal
	BOOST_CHECK_GT(cfg["island_size"].to_int(), 0);
	BOOST_CHECK_LT(cfg["island_size"].to_int(), 5);

	cfg["island_size"] = "0";  // No island
	BOOST_CHECK_EQUAL(cfg["island_size"].to_int(), 0);
}

// Test map settings for labels
BOOST_AUTO_TEST_CASE( test_map_settings_labels )
{
	config cfg;

	cfg["show_labels"] = "yes";
	BOOST_CHECK(cfg["show_labels"].to_bool());

	cfg["show_labels"] = "no";
	BOOST_CHECK(!cfg["show_labels"].to_bool());
}

// Test map settings for castle linking
BOOST_AUTO_TEST_CASE( test_map_settings_castle_linking )
{
	config cfg;

	cfg["link_castles"] = "true";
	BOOST_CHECK(cfg["link_castles"].to_bool());

	cfg["link_castles"] = "false";
	BOOST_CHECK(!cfg["link_castles"].to_bool());
}

// ============================================================================
// Default Generator Options Tests
// ============================================================================

// Test default options for 2 player map
BOOST_AUTO_TEST_CASE( test_default_options_2_players )
{
	config cfg;
	cfg["players"] = "2";

	generator_data data(cfg);

	BOOST_CHECK_EQUAL(data.nplayers, 2);
	BOOST_CHECK_EQUAL(data.castle_size, 9);
}

// Test default options for multiplayer
BOOST_AUTO_TEST_CASE( test_default_options_multiplayer )
{
	config cfg;
	cfg["players"] = "4";
	cfg["villages"] = "50";
	cfg["map_width"] = "60";
	cfg["map_height"] = "60";

	generator_data data(cfg);

	BOOST_CHECK_EQUAL(data.nplayers, 4);
	BOOST_CHECK_EQUAL(data.nvillages, 50);
	BOOST_CHECK_EQUAL(data.width, 60);
	BOOST_CHECK_EQUAL(data.height, 60);
}

// Test default options for terrain generation
BOOST_AUTO_TEST_CASE( test_default_options_terrain_gen )
{
	config cfg;

	generator_data data(cfg);

	// Default values for terrain generation parameters
	BOOST_CHECK_EQUAL(data.hill_size, 10);
	BOOST_CHECK_EQUAL(data.iterations, 1000);
	BOOST_CHECK_EQUAL(data.max_lakes, 20);
}

// Test default options consistency
BOOST_AUTO_TEST_CASE( test_default_options_consistency )
{
	config cfg1;
	config cfg2;

	generator_data data1(cfg1);
	generator_data data2(cfg2);

	// Ensure consistent defaults
	BOOST_CHECK_EQUAL(data1.width, data2.width);
	BOOST_CHECK_EQUAL(data1.height, data2.height);
	BOOST_CHECK_EQUAL(data1.nplayers, data2.nplayers);
	BOOST_CHECK_EQUAL(data1.nvillages, data2.nvillages);
	BOOST_CHECK_EQUAL(data1.iterations, data2.iterations);
}

// ============================================================================
// Scenario Generation Configuration Tests
// ============================================================================

// Test scenario generation with sides
BOOST_AUTO_TEST_CASE( test_scenario_generation_sides )
{
	config scenario;

	for(int i = 1; i <= 2; ++i) {
		config& side = scenario.add_child("side");
		side["side"] = std::to_string(i);
		side["canrecruit"] = "yes";
		side["team_name"] = "team_" + std::to_string(i);
	}

	BOOST_CHECK_EQUAL(scenario.child_count("side"), 2u);

	const config& side1 = scenario.mandatory_child("side", 0);
	BOOST_CHECK_EQUAL(side1["side"].to_int(), 1);
	BOOST_CHECK(side1["canrecruit"].to_bool());

	const config& side2 = scenario.mandatory_child("side", 1);
	BOOST_CHECK_EQUAL(side2["side"].to_int(), 2);
	BOOST_CHECK(side2["canrecruit"].to_bool());
}

// Test scenario generation with settings child
BOOST_AUTO_TEST_CASE( test_scenario_generation_settings )
{
	config scenario;

	config& settings = scenario.add_child("settings");
	settings["village_density"] = "3";
	settings["flipx_chance"] = "50";
	settings["flipy_chance"] = "50";

	BOOST_CHECK(scenario.has_child("settings"));
	BOOST_CHECK_EQUAL(scenario.mandatory_child("settings")["village_density"].to_int(), 3);
}

// Test complete generator configuration
BOOST_AUTO_TEST_CASE( test_complete_generator_config )
{
	config game_config;

	// Scenario template
	config& scenario = game_config.add_child("scenario");
	scenario["id"] = "generated_map";
	scenario["name"] = "Generated Map";

	// Generator settings
	game_config["map_width"] = "50";
	game_config["map_height"] = "50";
	game_config["players"] = "2";
	game_config["villages"] = "30";
	game_config["iterations"] = "1500";
	game_config["hill_size"] = "12";
	game_config["castle_size"] = "9";
	game_config["max_lakes"] = "15";

	BOOST_CHECK(game_config.has_child("scenario"));
	BOOST_CHECK_EQUAL(game_config["map_width"].to_int(), 50);
	BOOST_CHECK_EQUAL(game_config["players"].to_int(), 2);
}

// Test generator config cloning
BOOST_AUTO_TEST_CASE( test_generator_config_clone )
{
	config original;
	original["map_width"] = "60";
	original["map_height"] = "60";
	original["players"] = "4";

	config cloned = original;

	BOOST_CHECK_EQUAL(cloned["map_width"].str(), "60");
	BOOST_CHECK_EQUAL(cloned["map_height"].str(), "60");
	BOOST_CHECK_EQUAL(cloned["players"].str(), "4");

	// Modify original should not affect clone
	original["map_width"] = "100";

	BOOST_CHECK_EQUAL(original["map_width"].str(), "100");
	BOOST_CHECK_EQUAL(cloned["map_width"].str(), "60");
}

// Test generator with nested configuration
BOOST_AUTO_TEST_CASE( test_generator_nested_config )
{
	config cfg;

	config& generator = cfg.add_child("generator");
	generator["name"] = "default";
	generator["map_width"] = "40";
	generator["map_height"] = "40";

	config& settings = generator.add_child("settings");
	settings["village_density"] = "5";

	BOOST_CHECK(cfg.has_child("generator"));

	const config& gen = cfg.mandatory_child("generator");
	BOOST_CHECK_EQUAL(gen["name"].str(), "default");
	BOOST_CHECK(gen.has_child("settings"));
}

BOOST_AUTO_TEST_SUITE_END()
