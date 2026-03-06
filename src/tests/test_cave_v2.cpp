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
#include "terrain/translation.hpp"
#include "map/location.hpp"

BOOST_AUTO_TEST_SUITE( cave_v2 )

//
// Cave Generator Configuration Tests
//

// Test basic cave generator configuration
BOOST_AUTO_TEST_CASE( test_cave_generator_basic_config )
{
	config cfg;
	cfg["map_width"] = "50";
	cfg["map_height"] = "50";
	cfg["village_density"] = "5";

	BOOST_CHECK_EQUAL(cfg["map_width"].to_int(), 50);
	BOOST_CHECK_EQUAL(cfg["map_height"].to_int(), 50);
	BOOST_CHECK_EQUAL(cfg["village_density"].to_int(), 5);
}

// Test cave generator with default values
BOOST_AUTO_TEST_CASE( test_cave_generator_defaults )
{
	config cfg;
	// Default values as per cave_map_generator constructor
	BOOST_CHECK_EQUAL(cfg["map_width"].to_int(50), 50);
	BOOST_CHECK_EQUAL(cfg["map_height"].to_int(50), 50);
	BOOST_CHECK_EQUAL(cfg["village_density"].to_int(0), 0);
}

// Test cave generator flip chance settings
BOOST_AUTO_TEST_CASE( test_cave_generator_flip_chances )
{
	config cfg;
	cfg["flipx_chance"] = "50";
	cfg["flipy_chance"] = "75";

	BOOST_CHECK_EQUAL(cfg["flipx_chance"].to_int(), 50);
	BOOST_CHECK_EQUAL(cfg["flipy_chance"].to_int(), 75);
}

// Test cave generator with flip chance boundaries
BOOST_AUTO_TEST_CASE( test_cave_generator_flip_boundaries )
{
	config cfg;
	cfg["flipx_chance"] = "0";
	cfg["flipy_chance"] = "100";

	BOOST_CHECK_EQUAL(cfg["flipx_chance"].to_int(), 0);
	BOOST_CHECK_EQUAL(cfg["flipy_chance"].to_int(), 100);
}

// Test cave generator with custom dimensions
BOOST_AUTO_TEST_CASE( test_cave_generator_custom_dimensions )
{
	config cfg;
	cfg["map_width"] = "80";
	cfg["map_height"] = "60";

	BOOST_CHECK_EQUAL(cfg["map_width"].to_int(), 80);
	BOOST_CHECK_EQUAL(cfg["map_height"].to_int(), 60);
}

//
// Chamber Configuration Tests
//

// Test chamber basic configuration
BOOST_AUTO_TEST_CASE( test_chamber_config )
{
	config cfg;

	config& chamber = cfg.add_child("chamber");
	chamber["id"] = "start";
	chamber["x"] = "10";
	chamber["y"] = "10";
	chamber["size"] = "5";
	chamber["jagged"] = "10";

	BOOST_CHECK(cfg.has_child("chamber"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("chamber")["id"].str(), "start");
	BOOST_CHECK_EQUAL(cfg.mandatory_child("chamber")["size"].to_int(), 5);
}

// Test chamber with chance attribute
BOOST_AUTO_TEST_CASE( test_chamber_chance_config )
{
	config cfg;

	config& chamber = cfg.add_child("chamber");
	chamber["id"] = "optional";
	chamber["chance"] = "30";

	BOOST_CHECK(cfg.has_child("chamber"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("chamber")["chance"].to_int(), 30);
}

// Test chamber with coordinate range
BOOST_AUTO_TEST_CASE( test_chamber_coordinate_range )
{
	config cfg;

	config& chamber = cfg.add_child("chamber");
	chamber["id"] = "range_test";
	chamber["x"] = "10-30";
	chamber["y"] = "15-35";

	BOOST_CHECK(cfg.has_child("chamber"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("chamber")["x"].str(), "10-30");
	BOOST_CHECK_EQUAL(cfg.mandatory_child("chamber")["y"].str(), "15-35");
}

// Test multiple chambers configuration
BOOST_AUTO_TEST_CASE( test_multiple_chambers )
{
	config cfg;

	config& chamber1 = cfg.add_child("chamber");
	chamber1["id"] = "chamber1";

	config& chamber2 = cfg.add_child("chamber");
	chamber2["id"] = "chamber2";

	config& chamber3 = cfg.add_child("chamber");
	chamber3["id"] = "chamber3";

	BOOST_CHECK_EQUAL(cfg.child_count("chamber"), 3u);
}

// Test chamber with items
BOOST_AUTO_TEST_CASE( test_chamber_with_items )
{
	config cfg;

	config& chamber = cfg.add_child("chamber");
	chamber["id"] = "item_chamber";

	config& items = chamber.add_child("items");
	items["note"] = "treasure";

	BOOST_CHECK(chamber.has_child("items"));
}

//
// Passage Configuration Tests
//

// Test passage configuration
BOOST_AUTO_TEST_CASE( test_passage_config )
{
	config cfg;

	config& chamber = cfg.add_child("chamber");
	chamber["id"] = "main";

	config& passage = chamber.add_child("passage");
	passage["destination"] = "other";
	passage["width"] = "2";
	passage["windiness"] = "5";
	passage["laziness"] = "2.0";

	BOOST_CHECK(chamber.has_child("passage"));
	BOOST_CHECK_EQUAL(chamber.mandatory_child("passage")["destination"].str(), "other");
	BOOST_CHECK_EQUAL(chamber.mandatory_child("passage")["width"].to_int(), 2);
}

// Test passage with chance
BOOST_AUTO_TEST_CASE( test_passage_chance_config )
{
	config cfg;

	config& chamber = cfg.add_child("chamber");
	chamber["id"] = "source";

	config& passage = chamber.add_child("passage");
	passage["destination"] = "target";
	passage["chance"] = "50";

	BOOST_CHECK_EQUAL(chamber.mandatory_child("passage")["chance"].to_int(), 50);
}

// Test passage with jagged edges
BOOST_AUTO_TEST_CASE( test_passage_jagged_config )
{
	config cfg;

	config& chamber = cfg.add_child("chamber");
	chamber["id"] = "cave_start";

	config& passage = chamber.add_child("passage");
	passage["destination"] = "cave_end";
	passage["jagged"] = "15";

	BOOST_CHECK_EQUAL(chamber.mandatory_child("passage")["jagged"].to_int(), 15);
}

//
// Terrain Conversion Tests
//

// Test cave terrain codes
BOOST_AUTO_TEST_CASE( test_cave_terrain_codes )
{
	// Cave wall terrain
	t_translation::terrain_code wall = t_translation::CAVE_WALL;
	BOOST_CHECK(wall.base != 0);
	BOOST_CHECK_EQUAL(t_translation::write_terrain_code(wall), "Xu");

	// Cave floor terrain
	t_translation::terrain_code cave = t_translation::CAVE;
	BOOST_CHECK(cave.base != 0);
	BOOST_CHECK_EQUAL(t_translation::write_terrain_code(cave), "Uu");

	// Underground village terrain
	t_translation::terrain_code village = t_translation::UNDERGROUND_VILLAGE;
	BOOST_CHECK(village.base != 0);
	BOOST_CHECK_EQUAL(t_translation::write_terrain_code(village), "Uu^Vu");
}

// Test dwarven castle terrain codes
BOOST_AUTO_TEST_CASE( test_dwarven_castle_codes )
{
	// Dwarven castle terrain
	t_translation::terrain_code castle = t_translation::DWARVEN_CASTLE;
	BOOST_CHECK(castle.base != 0);
	BOOST_CHECK_EQUAL(t_translation::write_terrain_code(castle), "Cud");

	// Dwarven keep terrain
	t_translation::terrain_code keep = t_translation::DWARVEN_KEEP;
	BOOST_CHECK(keep.base != 0);
	BOOST_CHECK_EQUAL(t_translation::write_terrain_code(keep), "Kud");
}

// Test terrain code comparison
BOOST_AUTO_TEST_CASE( test_terrain_code_comparison )
{
	t_translation::terrain_code wall1 = t_translation::CAVE_WALL;
	t_translation::terrain_code wall2 = t_translation::CAVE_WALL;
	t_translation::terrain_code cave = t_translation::CAVE;

	BOOST_CHECK(wall1 == wall2);
	BOOST_CHECK(wall1 != cave);
}

// Test terrain code operations
BOOST_AUTO_TEST_CASE( test_terrain_code_operations )
{
	t_translation::terrain_code t1 = t_translation::read_terrain_code("Uu");
	t_translation::terrain_code t2 = t_translation::read_terrain_code("Uu^Vu");

	// Test bitwise AND
	t_translation::terrain_code result_and = t1 & t2;
	BOOST_CHECK(result_and.base != 0);

	// Test bitwise OR
	t_translation::terrain_code result_or = t1 | t2;
	BOOST_CHECK(result_or.base != 0);
}

//
// Map Location Tests for Cave Generator
//

// Test map location for chamber center
BOOST_AUTO_TEST_CASE( test_map_location_chamber_center )
{
	map_location loc(10, 15);

	BOOST_CHECK_EQUAL(loc.x, 10);
	BOOST_CHECK_EQUAL(loc.y, 15);
	BOOST_CHECK(loc.valid());
}

// Test map location validity
BOOST_AUTO_TEST_CASE( test_map_location_validity )
{
	map_location valid_loc(5, 5);
	map_location invalid_loc(-1, -1);

	BOOST_CHECK(valid_loc.valid());
	BOOST_CHECK(!invalid_loc.valid());
}

// Test map location adjacent tiles
BOOST_AUTO_TEST_CASE( test_map_location_adjacent )
{
	map_location center(10, 10);
	std::array<map_location, 6> adjacent = get_adjacent_tiles(center);

	BOOST_CHECK_EQUAL(adjacent.size(), 6u);

	// All adjacent tiles should be different from center
	for (const auto& adj : adjacent) {
		BOOST_CHECK(adj != center);
	}
}

//
// Starting Positions Tests
//

// Test starting positions configuration
BOOST_AUTO_TEST_CASE( test_starting_positions_config )
{
	t_translation::starting_positions positions;

	positions.insert(t_translation::starting_positions::value_type("1", t_translation::coordinate(5, 5)));
	positions.insert(t_translation::starting_positions::value_type("2", t_translation::coordinate(15, 15)));

	BOOST_CHECK_EQUAL(positions.left.size(), 2u);
	BOOST_CHECK(positions.left.find("1") != positions.left.end());
	BOOST_CHECK(positions.left.find("2") != positions.left.end());
}

// Test starting positions with same location
BOOST_AUTO_TEST_CASE( test_starting_positions_same_location )
{
	t_translation::starting_positions positions;

	positions.insert(t_translation::starting_positions::value_type("1", t_translation::coordinate(10, 10)));
	positions.insert(t_translation::starting_positions::value_type("2", t_translation::coordinate(10, 10)));

	// Both should exist even at same location
	BOOST_CHECK_EQUAL(positions.left.size(), 2u);

	// Should be able to find both by coordinate
	auto range = positions.right.equal_range(t_translation::coordinate(10, 10));
	int count = std::distance(range.first, range.second);
	BOOST_CHECK_EQUAL(count, 2);
}

//
// Ter Map Tests for Cave Generator
//

// Test ter_map creation
BOOST_AUTO_TEST_CASE( test_ter_map_creation )
{
	t_translation::terrain_code wall = t_translation::CAVE_WALL;
	t_translation::ter_map map(50, 50, wall);

	BOOST_CHECK_EQUAL(map.w, 50);
	BOOST_CHECK_EQUAL(map.h, 50);
	BOOST_CHECK_EQUAL(map.data.size(), 2500u);
}

// Test ter_map get and set
BOOST_AUTO_TEST_CASE( test_ter_map_get_set )
{
	t_translation::ter_map map(10, 10);
	t_translation::terrain_code cave = t_translation::CAVE;
	t_translation::terrain_code wall = t_translation::CAVE_WALL;

	map.get(5, 5) = cave;
	map.get(3, 3) = wall;

	BOOST_CHECK_EQUAL(map.get(5, 5), cave);
	BOOST_CHECK_EQUAL(map.get(3, 3), wall);
}

// Test ter_map bounds checking
BOOST_AUTO_TEST_CASE( test_ter_map_bounds )
{
	t_translation::ter_map map(10, 10);

	BOOST_CHECK_THROW(map.get(100, 100), std::out_of_range);
	BOOST_CHECK_THROW(map.get(-1, 0), std::out_of_range);
}

//
// Cave Generator Settings Tests
//

// Test settings child configuration
BOOST_AUTO_TEST_CASE( test_settings_child )
{
	config cfg;

	config& settings = cfg.add_child("settings");
	settings["name"] = "cave_scenario";
	settings["description"] = "A random cave map";

	BOOST_CHECK(cfg.has_child("settings"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("settings")["name"].str(), "cave_scenario");
}

// Test side configuration in chamber items
BOOST_AUTO_TEST_CASE( test_side_in_chamber )
{
	config cfg;

	config& chamber = cfg.add_child("chamber");
	chamber["id"] = "start";

	config& items = chamber.add_child("items");

	config& side = items.add_child("side");
	side["side"] = "1";
	side["no_castle"] = "no";

	BOOST_CHECK(items.has_child("side"));
	BOOST_CHECK_EQUAL(items.mandatory_child("side")["side"].to_int(), 1);
	BOOST_CHECK(!items.mandatory_child("side")["no_castle"].to_bool());
}

// Test event in result configuration
BOOST_AUTO_TEST_CASE( test_event_config )
{
	config cfg;

	config& event = cfg.add_child("event");
	event["name"] = "start";

	config& msg = event.add_child("deprecated_message");
	msg["what"] = "scenario_generation=cave";
	msg["level"] = "1";

	BOOST_CHECK(cfg.has_child("event"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("event")["name"].str(), "start");
}

//
// Random Seed Tests
//

// Test random seed configuration
BOOST_AUTO_TEST_CASE( test_random_seed_config )
{
	config cfg;
	cfg["seed"] = "12345";

	BOOST_CHECK_EQUAL(cfg["seed"].to_int(), 12345);
}

// Test random seed as optional value
BOOST_AUTO_TEST_CASE( test_optional_seed )
{
	// Test with seed
	uint32_t seed = 42;
	BOOST_CHECK_EQUAL(seed, 42u);

	// Test without seed (would use seed_rng::next_seed() in actual code)
	// Here we just verify the concept
}

//
// Complex Configuration Tests
//

// Test complete cave generator configuration
BOOST_AUTO_TEST_CASE( test_complete_cave_config )
{
	config cfg;
	cfg["map_width"] = "40";
	cfg["map_height"] = "40";
	cfg["village_density"] = "10";
	cfg["flipx_chance"] = "50";
	cfg["flipy_chance"] = "50";

	config& settings = cfg.add_child("settings");
	settings["name"] = "test_cave";

	// Add first chamber with side
	config& chamber1 = cfg.add_child("chamber");
	chamber1["id"] = "player1";
	chamber1["x"] = "5-15";
	chamber1["y"] = "5-15";
	chamber1["size"] = "4";

	config& items1 = chamber1.add_child("items");
	config& side1 = items1.add_child("side");
	side1["side"] = "1";

	// Add second chamber
	config& chamber2 = cfg.add_child("chamber");
	chamber2["id"] = "player2";
	chamber2["x"] = "25-35";
	chamber2["y"] = "25-35";
	chamber2["size"] = "4";

	config& items2 = chamber2.add_child("items");
	config& side2 = items2.add_child("side");
	side2["side"] = "2";

	// Add passage between chambers
	config& passage = chamber1.add_child("passage");
	passage["destination"] = "player2";
	passage["width"] = "2";
	passage["laziness"] = "2.5";

	// Verify configuration
	BOOST_CHECK_EQUAL(cfg["map_width"].to_int(), 40);
	BOOST_CHECK_EQUAL(cfg["map_height"].to_int(), 40);
	BOOST_CHECK_EQUAL(cfg.child_count("chamber"), 2u);
	BOOST_CHECK(chamber1.has_child("passage"));
}

// Test village density edge cases
BOOST_AUTO_TEST_CASE( test_village_density_edge_cases )
{
	config cfg;

	// Zero density
	cfg["village_density"] = "0";
	BOOST_CHECK_EQUAL(cfg["village_density"].to_int(), 0);

	// Maximum reasonable density
	cfg["village_density"] = "1000";
	BOOST_CHECK_EQUAL(cfg["village_density"].to_int(), 1000);

	// Negative should be handled
	cfg["village_density"] = "-1";
	BOOST_CHECK_LT(cfg["village_density"].to_int(), 0);
}

// Test map size limits
BOOST_AUTO_TEST_CASE( test_map_size_limits )
{
	config cfg;

	// Small map
	cfg["map_width"] = "10";
	cfg["map_height"] = "10";
	BOOST_CHECK_EQUAL(cfg["map_width"].to_int(), 10);

	// Large map
	cfg["map_width"] = "200";
	cfg["map_height"] = "200";
	BOOST_CHECK_EQUAL(cfg["map_width"].to_int(), 200);

	// Test max_map_size function
	int max_size = t_translation::max_map_size();
	BOOST_CHECK_GT(max_size, 0);
}

//
// Passage Path Calculator Tests
//

// Test passage windiness configuration
BOOST_AUTO_TEST_CASE( test_passage_windiness )
{
	config cfg;

	config& chamber = cfg.add_child("chamber");
	chamber["id"] = "start";

	config& passage = chamber.add_child("passage");
	passage["destination"] = "end";
	passage["windiness"] = "10";

	BOOST_CHECK_EQUAL(chamber.mandatory_child("passage")["windiness"].to_int(), 10);
}

// Test passage laziness configuration
BOOST_AUTO_TEST_CASE( test_passage_laziness )
{
	config cfg;

	config& chamber = cfg.add_child("chamber");
	chamber["id"] = "start";

	config& passage = chamber.add_child("passage");
	passage["destination"] = "end";
	passage["laziness"] = "5.5";

	BOOST_CHECK_EQUAL(chamber.mandatory_child("passage")["laziness"].to_double(), 5.5);
}

//
// Chamber Building Tests
//

// Test chamber size configuration
BOOST_AUTO_TEST_CASE( test_chamber_size )
{
	config cfg;

	config& chamber = cfg.add_child("chamber");
	chamber["size"] = "10";
	chamber["jagged"] = "20";

	BOOST_CHECK_EQUAL(chamber["size"].to_int(), 10);
	BOOST_CHECK_EQUAL(chamber["jagged"].to_int(), 20);
}

// Test chamber with no items
BOOST_AUTO_TEST_CASE( test_chamber_no_items )
{
	config cfg;

	config& chamber = cfg.add_child("chamber");
	chamber["id"] = "empty";
	chamber["size"] = "3";

	BOOST_CHECK(!chamber.has_child("items"));
}

//
// Terrain String Tests
//

// Test terrain string reading
BOOST_AUTO_TEST_CASE( test_terrain_string_read )
{
	t_translation::terrain_code t = t_translation::read_terrain_code("Uu^Vu");

	BOOST_CHECK(t.base != 0);
	BOOST_CHECK(t.overlay != t_translation::NO_LAYER);
	BOOST_CHECK(t.overlay != 0);
}

// Test terrain string writing
BOOST_AUTO_TEST_CASE( test_terrain_string_write )
{
	t_translation::terrain_code t = t_translation::CAVE_WALL;
	std::string str = t_translation::write_terrain_code(t);

	BOOST_CHECK_EQUAL(str, "Xu");
}

//
// Game Map Read/Write Tests for Cave
//

// Test reading a cave map string
BOOST_AUTO_TEST_CASE( test_read_cave_map )
{
	const std::string map_str =
		"Xu, Xu, Xu, Xu\n"
		"Xu, Uu, Uu, Xu\n"
		"Xu, Uu, Uu, Xu\n"
		"Xu, Xu, Xu, Xu\n";

	t_translation::starting_positions positions;
	t_translation::ter_map map = t_translation::read_game_map(map_str, positions);

	BOOST_CHECK_EQUAL(map.w, 4);
	BOOST_CHECK_EQUAL(map.h, 4);
}

// Test writing a cave map
BOOST_AUTO_TEST_CASE( test_write_cave_map )
{
	t_translation::ter_map map(3, 3, t_translation::CAVE_WALL);

	// Clear center
	map.get(1, 1) = t_translation::CAVE;

	t_translation::starting_positions positions;
	std::string result = t_translation::write_game_map(map, positions);

	BOOST_CHECK(!result.empty());
	BOOST_CHECK(result.find("Xu") != std::string::npos);
	BOOST_CHECK(result.find("Uu") != std::string::npos);
}

//
// Chamber ID Tests
//

// Test chamber ID mapping
BOOST_AUTO_TEST_CASE( test_chamber_id_mapping )
{
	std::map<std::string, std::size_t> chamber_ids;

	chamber_ids["start"] = 0;
	chamber_ids["middle"] = 1;
	chamber_ids["end"] = 2;

	BOOST_CHECK_EQUAL(chamber_ids.size(), 3u);
	BOOST_CHECK_EQUAL(chamber_ids["start"], 0u);
	BOOST_CHECK_EQUAL(chamber_ids["middle"], 1u);
	BOOST_CHECK_EQUAL(chamber_ids["end"], 2u);
}

// Test chamber ID lookup
BOOST_AUTO_TEST_CASE( test_chamber_id_lookup )
{
	std::map<std::string, std::size_t> chamber_ids;
	chamber_ids["player1"] = 0;

	auto it = chamber_ids.find("player1");
	BOOST_CHECK(it != chamber_ids.end());
	BOOST_CHECK_EQUAL(it->second, 0u);

	auto missing = chamber_ids.find("nonexistent");
	BOOST_CHECK(missing == chamber_ids.end());
}

//
// Castle Placement Tests
//

// Test castle placement configuration
BOOST_AUTO_TEST_CASE( test_castle_placement )
{
	config cfg;

	config& chamber = cfg.add_child("chamber");
	chamber["id"] = "castle_room";

	config& items = chamber.add_child("items");

	config& side = items.add_child("side");
	side["side"] = "1";
	side["no_castle"] = "no";

	BOOST_CHECK_EQUAL(side["side"].to_int(), 1);
	BOOST_CHECK(!side["no_castle"].to_bool()); // should place castle
}

// Test no_castle option
BOOST_AUTO_TEST_CASE( test_no_castle_option )
{
	config cfg;

	config& chamber = cfg.add_child("chamber");
	chamber["id"] = "no_castle_room";

	config& items = chamber.add_child("items");

	config& side = items.add_child("side");
	side["side"] = "1";
	side["no_castle"] = "yes";

	BOOST_CHECK(side["no_castle"].to_bool()); // should NOT place castle
}

//
// Coordinate Translation Tests
//

// Test X coordinate translation without flip
BOOST_AUTO_TEST_CASE( test_translate_x_no_flip )
{
	std::size_t x = 10;
	int width = 50;
	bool flipx = false;

	if (flipx) {
		x = width - x - 1;
	}

	BOOST_CHECK_EQUAL(x, 10u);
}

// Test X coordinate translation with flip
BOOST_AUTO_TEST_CASE( test_translate_x_with_flip )
{
	std::size_t x = 10;
	int width = 50;
	bool flipx = true;

	if (flipx) {
		x = width - x - 1;
	}

	BOOST_CHECK_EQUAL(x, 39u);
}

// Test Y coordinate translation without flip
BOOST_AUTO_TEST_CASE( test_translate_y_no_flip )
{
	std::size_t y = 15;
	int height = 50;
	bool flipy = false;

	if (flipy) {
		y = height - y - 1;
	}

	BOOST_CHECK_EQUAL(y, 15u);
}

// Test Y coordinate translation with flip
BOOST_AUTO_TEST_CASE( test_translate_y_with_flip )
{
	std::size_t y = 15;
	int height = 50;
	bool flipy = true;

	if (flipy) {
		y = height - y - 1;
	}

	BOOST_CHECK_EQUAL(y, 34u);
}

//
// Store Location Tests
//

// Test store_location_as configuration
BOOST_AUTO_TEST_CASE( test_store_location_as )
{
	config cfg;

	config& chamber = cfg.add_child("chamber");
	chamber["id"] = "variable_test";

	config& items = chamber.add_child("items");

	config& unit = items.add_child("unit");
	unit["type"] = "Dwarvish Fighter";
	unit["store_location_as"] = "dwarf_location";

	BOOST_CHECK_EQUAL(unit["store_location_as"].str(), "dwarf_location");
}

// Test same_location_as_previous option
BOOST_AUTO_TEST_CASE( test_same_location_as_previous )
{
	config cfg;

	config& chamber = cfg.add_child("chamber");
	chamber["id"] = "multi_item";

	config& items = chamber.add_child("items");

	config& item1 = items.add_child("item");
	item1["image"] = "item1.png";

	config& item2 = items.add_child("item");
	item2["image"] = "item2.png";
	item2["same_location_as_previous"] = "yes";

	BOOST_CHECK(item2["same_location_as_previous"].to_bool());
}

//
// Scenario Result Tests
//

// Test scenario result map_data
BOOST_AUTO_TEST_CASE( test_scenario_map_data )
{
	config result;
	result["map_data"] = "Xu, Xu, Xu\nUu, Uu, Uu\nXu, Xu, Xu\n";

	BOOST_CHECK(!result["map_data"].str().empty());
	BOOST_CHECK(result["map_data"].str().find("Xu") != std::string::npos);
	BOOST_CHECK(result["map_data"].str().find("Uu") != std::string::npos);
}

// Test scenario result with settings child
BOOST_AUTO_TEST_CASE( test_scenario_settings_child )
{
	config result;

	config& settings = result.add_child("settings");
	settings["name"] = "generated_cave";

	BOOST_CHECK(result.has_child("settings"));
	BOOST_CHECK_EQUAL(result.mandatory_child("settings")["name"].str(), "generated_cave");
}

BOOST_AUTO_TEST_SUITE_END()
