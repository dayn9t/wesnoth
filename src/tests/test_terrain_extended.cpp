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

#include "terrain/translation.hpp"
#include "terrain/terrain.hpp"
#include "config.hpp"

BOOST_AUTO_TEST_SUITE(terrain_extended)

//
// Extended terrain_code tests
//

BOOST_AUTO_TEST_CASE(test_terrain_code_string_construction_various)
{
	// Test 4-character terrain codes
	t_translation::terrain_code t1("Abcd");
	BOOST_CHECK_EQUAL(t_translation::write_terrain_code(t1), "Abcd");

	// Test 1-character terrain codes
	t_translation::terrain_code t2("X");
	BOOST_CHECK_EQUAL(t_translation::write_terrain_code(t2), "X");

	// Test 3-character terrain codes
	t_translation::terrain_code t3("Gg^Ff");
	BOOST_CHECK(t3.base != 0);
	BOOST_CHECK(t3.overlay != t_translation::NO_LAYER);
	BOOST_CHECK(t3.overlay != 0);
}

BOOST_AUTO_TEST_CASE(test_terrain_code_empty_overlay)
{
	// Test terrain with empty overlay (should use filler)
	t_translation::terrain_code t("Gg", 0);
	BOOST_CHECK_EQUAL(t.overlay, 0);
}

BOOST_AUTO_TEST_CASE(test_terrain_code_special_characters)
{
	// Test terrain codes with special characters
	t_translation::terrain_code slash("A/");
	BOOST_CHECK(slash.base != 0);

	t_translation::terrain_code pipe("A|");
	BOOST_CHECK(pipe.base != 0);

	t_translation::terrain_code underscore("A_");
	BOOST_CHECK(underscore.base != 0);
}

BOOST_AUTO_TEST_CASE(test_terrain_code_copy_and_assign)
{
	t_translation::terrain_code t1("Gg", "Ff");
	t_translation::terrain_code t2(t1);
	BOOST_CHECK(t1 == t2);

	t_translation::terrain_code t3;
	t3 = t1;
	BOOST_CHECK(t1 == t3);
}

//
// Extended read_terrain_code / write_terrain_code tests
//

BOOST_AUTO_TEST_CASE(test_read_terrain_code_with_spaces)
{
	// Spaces should be trimmed
	t_translation::terrain_code t1 = t_translation::read_terrain_code("  Gg  ");
	BOOST_CHECK_EQUAL(t_translation::write_terrain_code(t1), "Gg");

	t_translation::terrain_code t2 = t_translation::read_terrain_code("\tWw\t");
	BOOST_CHECK_EQUAL(t_translation::write_terrain_code(t2), "Ww");
}

BOOST_AUTO_TEST_CASE(test_read_terrain_code_empty_string)
{
	t_translation::terrain_code t = t_translation::read_terrain_code("");
	BOOST_CHECK(t == t_translation::NONE_TERRAIN);
}

BOOST_AUTO_TEST_CASE(test_read_terrain_code_whitespace_only)
{
	t_translation::terrain_code t = t_translation::read_terrain_code("   ");
	BOOST_CHECK(t == t_translation::NONE_TERRAIN);
}

BOOST_AUTO_TEST_CASE(test_write_terrain_code_none_terrain)
{
	std::string result = t_translation::write_terrain_code(t_translation::NONE_TERRAIN);
	BOOST_CHECK(result.empty());
}

//
// Extended ter_list tests
//

BOOST_AUTO_TEST_CASE(test_read_list_with_wildcards)
{
	t_translation::ter_list list = t_translation::read_list("G*, W*, M*");
	BOOST_CHECK_EQUAL(list.size(), 3);
	BOOST_CHECK(t_translation::has_wildcard(list));
}

BOOST_AUTO_TEST_CASE(test_read_list_with_mixed_terrains)
{
	t_translation::ter_list list = t_translation::read_list("Gg, G*, Ww, *, !");
	BOOST_CHECK_EQUAL(list.size(), 5);
	BOOST_CHECK(t_translation::has_wildcard(list));
}

BOOST_AUTO_TEST_CASE(test_read_list_trailing_comma)
{
	// Trailing comma behavior
	t_translation::ter_list list = t_translation::read_list("Gg, Ww,");
	// Should have 3 elements, last one being NONE_TERRAIN
	BOOST_CHECK_GE(list.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_write_list_empty)
{
	t_translation::ter_list list;
	std::string result = t_translation::write_list(list);
	BOOST_CHECK(result.empty());
}

BOOST_AUTO_TEST_CASE(test_write_list_single)
{
	t_translation::ter_list list;
	list.push_back(t_translation::read_terrain_code("Gg"));
	std::string result = t_translation::write_list(list);
	BOOST_CHECK_EQUAL(result, "Gg");
}

//
// Extended terrain_matches tests
//

BOOST_AUTO_TEST_CASE(test_terrain_matches_overlay_wildcard)
{
	t_translation::terrain_code src = t_translation::read_terrain_code("Gg^Ff");
	t_translation::terrain_code dest = t_translation::read_terrain_code("Gg^*");

	BOOST_CHECK(t_translation::terrain_matches(src, dest));
}

BOOST_AUTO_TEST_CASE(test_terrain_matches_base_wildcard_with_overlay)
{
	t_translation::terrain_code src = t_translation::read_terrain_code("Gg^Ff");
	t_translation::terrain_code dest = t_translation::read_terrain_code("G*^Ff");

	BOOST_CHECK(t_translation::terrain_matches(src, dest));
}

BOOST_AUTO_TEST_CASE(test_terrain_matches_both_wildcards)
{
	t_translation::terrain_code src = t_translation::read_terrain_code("Gg^Ff");
	t_translation::terrain_code dest = t_translation::read_terrain_code("G*^*");

	BOOST_CHECK(t_translation::terrain_matches(src, dest));
}

BOOST_AUTO_TEST_CASE(test_terrain_matches_list_with_star_at_end)
{
	t_translation::terrain_code src("Gg");
	t_translation::ter_list dest = t_translation::read_list("Ww, *");

	BOOST_CHECK(t_translation::terrain_matches(src, dest));
}

BOOST_AUTO_TEST_CASE(test_terrain_matches_list_with_star_at_beginning)
{
	t_translation::terrain_code src("Gg");
	t_translation::ter_list dest = t_translation::read_list("*, Ww");

	BOOST_CHECK(t_translation::terrain_matches(src, dest));
}

BOOST_AUTO_TEST_CASE(test_terrain_matches_complex_not_pattern)
{
	// !(Gg or Ww) - Gg should match first but ! inverts it
	t_translation::terrain_code src("Gg");
	t_translation::ter_list dest = t_translation::read_list("!, Gg, Ww");

	BOOST_CHECK(!t_translation::terrain_matches(src, dest));
}

BOOST_AUTO_TEST_CASE(test_terrain_matches_not_followed_by_star)
{
	// !* matches nothing (star matches all, ! inverts to none)
	t_translation::terrain_code src("Gg");
	t_translation::ter_list dest = t_translation::read_list("!, *");

	BOOST_CHECK(!t_translation::terrain_matches(src, dest));
}

BOOST_AUTO_TEST_CASE(test_terrain_matches_empty_list)
{
	t_translation::terrain_code src("Gg");
	t_translation::ter_list dest;

	BOOST_CHECK(!t_translation::terrain_matches(src, dest));
}

//
// Extended ter_match tests
//

BOOST_AUTO_TEST_CASE(test_ter_match_with_complex_pattern)
{
	t_translation::ter_match m("G*, W*, !, M*");
	BOOST_CHECK(!m.is_empty);
	BOOST_CHECK(m.has_wildcard);
	BOOST_CHECK_EQUAL(m.terrain.size(), 4);
}

BOOST_AUTO_TEST_CASE(test_ter_match_empty_string)
{
	t_translation::ter_match m("");
	BOOST_CHECK(m.is_empty);
}

BOOST_AUTO_TEST_CASE(test_ter_match_copy_construction)
{
	t_translation::ter_match m1("Gg, Ww");
	t_translation::ter_match m2(m1);

	BOOST_CHECK_EQUAL(m1.terrain.size(), m2.terrain.size());
	BOOST_CHECK_EQUAL(m1.has_wildcard, m2.has_wildcard);
	BOOST_CHECK_EQUAL(m1.is_empty, m2.is_empty);
}

BOOST_AUTO_TEST_CASE(test_ter_match_terrain_matches_consistency)
{
	// Test that ter_match and ter_list produce consistent results
	t_translation::terrain_code src("Gg^Ff");
	t_translation::ter_list list = t_translation::read_list("G*, W*");
	t_translation::ter_match match("G*, W*");

	bool list_result = t_translation::terrain_matches(src, list);
	bool match_result = t_translation::terrain_matches(src, match);

	BOOST_CHECK_EQUAL(list_result, match_result);
}

//
// Extended ter_map tests
//

BOOST_AUTO_TEST_CASE(test_ter_map_operator_bracket)
{
	t_translation::terrain_code fill("Gg");
	t_translation::ter_map map(3, 2, fill);

	// Test operator[] access
	BOOST_CHECK_EQUAL(map[0][0], fill);
	BOOST_CHECK_EQUAL(map[2][1], fill);
}

BOOST_AUTO_TEST_CASE(test_ter_map_get_and_set)
{
	t_translation::ter_map map(2, 2);
	t_translation::terrain_code gg("Gg");
	t_translation::terrain_code ww("Ww");

	map.get(0, 0) = gg;
	map.get(1, 1) = ww;

	BOOST_CHECK_EQUAL(map.get(0, 0), gg);
	BOOST_CHECK_EQUAL(map.get(1, 1), ww);
}

BOOST_AUTO_TEST_CASE(test_ter_map_out_of_range)
{
	t_translation::ter_map map(2, 2);

	BOOST_CHECK_THROW(map.get(5, 5), std::out_of_range);
}

//
// Extended read_game_map / write_game_map tests
//

BOOST_AUTO_TEST_CASE(test_read_game_map_single_row)
{
	const std::string map_str = "Gg, Ww, Mm";

	t_translation::starting_positions positions;
	t_translation::ter_map map = t_translation::read_game_map(map_str, positions);

	BOOST_CHECK_EQUAL(map.w, 3);
	BOOST_CHECK_EQUAL(map.h, 1);
}

BOOST_AUTO_TEST_CASE(test_read_game_map_with_newlines)
{
	const std::string map_str = "\n\nGg, Ww\nMm, Hh\n\n";

	t_translation::starting_positions positions;
	t_translation::ter_map map = t_translation::read_game_map(map_str, positions);

	BOOST_CHECK_EQUAL(map.w, 2);
	BOOST_CHECK_EQUAL(map.h, 2);
}

BOOST_AUTO_TEST_CASE(test_read_game_map_with_overlays)
{
	const std::string map_str =
		"Gg^Ff, Ww\n"
		"Mm^Xm, Hh\n";

	t_translation::starting_positions positions;
	t_translation::ter_map map = t_translation::read_game_map(map_str, positions);

	BOOST_CHECK_EQUAL(map.w, 2);
	BOOST_CHECK_EQUAL(map.h, 2);

	// Check that overlays are preserved
	t_translation::terrain_code expected = t_translation::read_terrain_code("Gg^Ff");
	BOOST_CHECK_EQUAL(map.get(0, 0), expected);
}

BOOST_AUTO_TEST_CASE(test_read_game_map_multiple_starting_positions)
{
	const std::string map_str =
		"1 Gg, 2 Gg, 3 Gg\n"
		"4 Gg, 5 Gg, 6 Gg\n";

	t_translation::starting_positions positions;
	t_translation::ter_map map = t_translation::read_game_map(map_str, positions);

	BOOST_CHECK_EQUAL(map.w, 3);
	BOOST_CHECK_EQUAL(map.h, 2);

	// Check all starting positions are recorded
	for(int i = 1; i <= 6; ++i) {
		BOOST_CHECK(positions.left.find(std::to_string(i)) != positions.left.end());
	}
}

BOOST_AUTO_TEST_CASE(test_write_game_map_with_positions)
{
	t_translation::ter_map map(2, 2, t_translation::read_terrain_code("Gg"));
	t_translation::starting_positions positions;
	positions.insert(t_translation::starting_positions::value_type("1", t_translation::coordinate(0, 0)));
	positions.insert(t_translation::starting_positions::value_type("2", t_translation::coordinate(1, 1)));

	std::string result = t_translation::write_game_map(map, positions);

	// Result should contain starting positions
	BOOST_CHECK(result.find("1") != std::string::npos);
	BOOST_CHECK(result.find("2") != std::string::npos);
}

//
// Predefined terrain constants tests
//

BOOST_AUTO_TEST_CASE(test_predefined_terrains_valid)
{
	// Test that predefined terrains are valid
	BOOST_CHECK(t_translation::VOID_TERRAIN.base != 0);
	BOOST_CHECK(t_translation::FOGGED.base != 0);
	BOOST_CHECK(t_translation::OFF_MAP_USER.base != 0);

	BOOST_CHECK(t_translation::HUMAN_CASTLE.base != 0);
	BOOST_CHECK(t_translation::HUMAN_KEEP.base != 0);
	BOOST_CHECK(t_translation::SHALLOW_WATER.base != 0);
	BOOST_CHECK(t_translation::DEEP_WATER.base != 0);
	BOOST_CHECK(t_translation::GRASS_LAND.base != 0);
	BOOST_CHECK(t_translation::FOREST.base != 0);
	BOOST_CHECK(t_translation::MOUNTAIN.base != 0);
	BOOST_CHECK(t_translation::HILL.base != 0);
}

BOOST_AUTO_TEST_CASE(test_predefined_terrains_write)
{
	BOOST_CHECK_EQUAL(t_translation::write_terrain_code(t_translation::GRASS_LAND), "Gg");
	BOOST_CHECK_EQUAL(t_translation::write_terrain_code(t_translation::SHALLOW_WATER), "Ww");
	BOOST_CHECK_EQUAL(t_translation::write_terrain_code(t_translation::DEEP_WATER), "Wo");
	BOOST_CHECK_EQUAL(t_translation::write_terrain_code(t_translation::MOUNTAIN), "Mm");
	BOOST_CHECK_EQUAL(t_translation::write_terrain_code(t_translation::HILL), "Hh");
}

BOOST_AUTO_TEST_CASE(test_forest_has_overlay)
{
	// FOREST should have an overlay layer
	BOOST_CHECK(t_translation::FOREST.overlay != t_translation::NO_LAYER);
	BOOST_CHECK(t_translation::write_terrain_code(t_translation::FOREST) == "Gg^Ff");
}

//
// Predefined ter_match constants tests
//

BOOST_AUTO_TEST_CASE(test_all_forests_match)
{
	t_translation::terrain_code forest1("Gg^Ff");
	t_translation::terrain_code forest2("Ff");
	t_translation::terrain_code grass("Gg");

	BOOST_CHECK(t_translation::terrain_matches(forest1, t_translation::ALL_FORESTS));
	BOOST_CHECK(t_translation::terrain_matches(forest2, t_translation::ALL_FORESTS));
	BOOST_CHECK(!t_translation::terrain_matches(grass, t_translation::ALL_FORESTS));
}

BOOST_AUTO_TEST_CASE(test_all_hills_match)
{
	t_translation::terrain_code hill("Hh");
	t_translation::terrain_code grass("Gg");

	BOOST_CHECK(t_translation::terrain_matches(hill, t_translation::ALL_HILLS));
	BOOST_CHECK(!t_translation::terrain_matches(grass, t_translation::ALL_HILLS));
}

BOOST_AUTO_TEST_CASE(test_all_mountains_match)
{
	t_translation::terrain_code mountain("Mm");
	t_translation::terrain_code hill("Hh");

	BOOST_CHECK(t_translation::terrain_matches(mountain, t_translation::ALL_MOUNTAINS));
	BOOST_CHECK(!t_translation::terrain_matches(hill, t_translation::ALL_MOUNTAINS));
}

//
// terrain_type tests (extended)
//

BOOST_AUTO_TEST_CASE(test_terrain_type_config_with_alias)
{
	config cfg;
	cfg["string"] = "Gg";
	cfg["name"] = "Grassland";
	cfg["id"] = "grassland";
	cfg["aliasof"] = "Gg, Ww";  // Multiple aliases

	terrain_type t(cfg);
	BOOST_CHECK_EQUAL(t.id(), "grassland");
	BOOST_CHECK(t.mvt_type().size() >= 1);
}

BOOST_AUTO_TEST_CASE(test_terrain_type_config_village)
{
	config cfg;
	cfg["string"] = "Vv";
	cfg["name"] = "Village";
	cfg["id"] = "village";
	cfg["gives_income"] = true;

	terrain_type t(cfg);
	BOOST_CHECK(t.is_village());
}

BOOST_AUTO_TEST_CASE(test_terrain_type_config_castle)
{
	config cfg;
	cfg["string"] = "Cc";
	cfg["name"] = "Castle";
	cfg["id"] = "castle";
	cfg["recruit_onto"] = true;

	terrain_type t(cfg);
	BOOST_CHECK(t.is_castle());
}

BOOST_AUTO_TEST_CASE(test_terrain_type_config_keep)
{
	config cfg;
	cfg["string"] = "Kk";
	cfg["name"] = "Keep";
	cfg["id"] = "keep";
	cfg["recruit_from"] = true;

	terrain_type t(cfg);
	BOOST_CHECK(t.is_keep());
}

BOOST_AUTO_TEST_CASE(test_terrain_type_config_healing)
{
	config cfg;
	cfg["string"] = "Hh";
	cfg["name"] = "Healing";
	cfg["id"] = "healing";
	cfg["heals"] = 8;

	terrain_type t(cfg);
	BOOST_CHECK_EQUAL(t.gives_healing(), 8);
}

BOOST_AUTO_TEST_CASE(test_terrain_type_config_light_modification)
{
	config cfg;
	cfg["string"] = "Ll";
	cfg["name"] = "Light";
	cfg["id"] = "light";
	cfg["light"] = 25;
	cfg["max_light"] = 30;
	cfg["min_light"] = -10;

	terrain_type t(cfg);
	BOOST_CHECK_EQUAL(t.light_bonus(0), 25);
	BOOST_CHECK_EQUAL(t.light_bonus(10), 30);  // capped at max_light
}

BOOST_AUTO_TEST_CASE(test_terrain_type_config_height_adjust)
{
	config cfg;
	cfg["string"] = "Tt";
	cfg["name"] = "Tall";
	cfg["id"] = "tall";
	cfg["unit_height_adjust"] = 20;

	terrain_type t(cfg);
	BOOST_CHECK_EQUAL(t.unit_height_adjust(), 20);
}

BOOST_AUTO_TEST_CASE(test_terrain_type_config_submerge)
{
	config cfg;
	cfg["string"] = "Ww";
	cfg["name"] = "Water";
	cfg["id"] = "water";
	cfg["submerge"] = 0.5;

	terrain_type t(cfg);
	BOOST_CHECK_EQUAL(t.unit_submerge(), 0.5);
}

BOOST_AUTO_TEST_CASE(test_terrain_type_config_hidden)
{
	config cfg;
	cfg["string"] = "Xx";
	cfg["name"] = "Hidden";
	cfg["id"] = "hidden";
	cfg["hidden"] = true;

	terrain_type t(cfg);
	BOOST_CHECK(t.hide_in_editor());
}

BOOST_AUTO_TEST_CASE(test_terrain_type_config_hide_help)
{
	config cfg;
	cfg["string"] = "Xx";
	cfg["name"] = "Hidden Help";
	cfg["id"] = "hidden_help";
	cfg["hide_help"] = true;

	terrain_type t(cfg);
	BOOST_CHECK(t.hide_help());
}

BOOST_AUTO_TEST_CASE(test_terrain_type_combined_construction)
{
	config base_cfg;
	base_cfg["string"] = "Gg";
	base_cfg["name"] = "Grassland";
	base_cfg["id"] = "grassland";

	config overlay_cfg;
	overlay_cfg["string"] = "^Ff";
	overlay_cfg["name"] = "Forest";
	overlay_cfg["id"] = "forest";

	terrain_type base(base_cfg);
	terrain_type overlay(overlay_cfg);
	terrain_type combined(base, overlay);

	BOOST_CHECK(combined.is_combined());
	BOOST_CHECK_EQUAL(combined.id(), "grassland^forest");
}

BOOST_AUTO_TEST_CASE(test_terrain_type_equality_same)
{
	config cfg;
	cfg["string"] = "Gg";
	cfg["name"] = "Grassland";
	cfg["id"] = "grassland";

	terrain_type t1(cfg);
	terrain_type t2(cfg);

	BOOST_CHECK(t1 == t2);
}

BOOST_AUTO_TEST_CASE(test_terrain_type_inequality_different)
{
	config cfg1;
	cfg1["string"] = "Gg";
	cfg1["name"] = "Grassland";
	cfg1["id"] = "grassland";

	config cfg2;
	cfg2["string"] = "Ww";
	cfg2["name"] = "Water";
	cfg2["id"] = "water";

	terrain_type t1(cfg1);
	terrain_type t2(cfg2);

	BOOST_CHECK(!(t1 == t2));
}

//
// read_builder_map tests
//

BOOST_AUTO_TEST_CASE(test_read_builder_map_simple)
{
	const std::string map_str =
		"*, ., 1\n"
		"., *, .\n";

	t_translation::ter_map map = t_translation::read_builder_map(map_str);

	BOOST_CHECK_EQUAL(map.w, 3);
	BOOST_CHECK_EQUAL(map.h, 2);
}

BOOST_AUTO_TEST_CASE(test_read_builder_map_with_anchors)
{
	const std::string map_str =
		"1, 2, 3\n"
		"4, 5, 6\n";

	t_translation::ter_map map = t_translation::read_builder_map(map_str);

	BOOST_CHECK_EQUAL(map.w, 3);
	BOOST_CHECK_EQUAL(map.h, 2);
}

BOOST_AUTO_TEST_CASE(test_read_builder_map_empty)
{
	const std::string map_str = "";

	t_translation::ter_map map = t_translation::read_builder_map(map_str);

	BOOST_CHECK_EQUAL(map.w, 0);
	BOOST_CHECK_EQUAL(map.h, 0);
}

//
// WILDCARD and NO_LAYER constant tests
//

BOOST_AUTO_TEST_CASE(test_wildcard_constant)
{
	BOOST_CHECK_EQUAL(t_translation::WILDCARD, 0x2A000000);
}

BOOST_AUTO_TEST_CASE(test_no_layer_constant)
{
	BOOST_CHECK_EQUAL(t_translation::NO_LAYER, 0xFFFFFFFF);
}

//
// Additional edge case tests
//

BOOST_AUTO_TEST_CASE(test_terrain_matches_with_plus_minus_in_list)
{
	// PLUS and MINUS are special terrains used in alias lists
	t_translation::terrain_code plus = t_translation::PLUS;
	t_translation::terrain_code minus = t_translation::MINUS;

	BOOST_CHECK(plus.base != 0);
	BOOST_CHECK(minus.base != 0);
	BOOST_CHECK(plus != minus);
}

BOOST_AUTO_TEST_CASE(test_terrain_matches_not_in_middle)
{
	t_translation::terrain_code src("Gg");
	t_translation::ter_list dest = t_translation::read_list("Ww, !, Gg");

	// Gg matches Gg but there's a ! before it that inverts the result
	BOOST_CHECK(!t_translation::terrain_matches(src, dest));
}

BOOST_AUTO_TEST_CASE(test_terrain_matches_multiple_inversions)
{
	t_translation::terrain_code src("Gg");
	t_translation::ter_list dest = t_translation::read_list("!, !, Gg");

	// Two inversions should cancel out
	BOOST_CHECK(t_translation::terrain_matches(src, dest));
}

BOOST_AUTO_TEST_CASE(test_terrain_code_with_four_char_base)
{
	// Test 4-character base terrain code
	t_translation::terrain_code t("_bas");  // BASE terrain
	BOOST_CHECK(t.base != 0);
	BOOST_CHECK_EQUAL(t.overlay, t_translation::NO_LAYER);
}

BOOST_AUTO_TEST_CASE(test_starting_positions_bimap)
{
	t_translation::starting_positions pos;

	pos.insert(t_translation::starting_positions::value_type("1", t_translation::coordinate(0, 0)));
	pos.insert(t_translation::starting_positions::value_type("2", t_translation::coordinate(1, 1)));
	pos.insert(t_translation::starting_positions::value_type("3", t_translation::coordinate(0, 0)));  // Same location, different player

	// Should have 3 entries
	BOOST_CHECK_EQUAL(pos.left.size(), 3);

	// Look up by coordinate should find multiple entries
	auto range = pos.right.equal_range(t_translation::coordinate(0, 0));
	int count = std::distance(range.first, range.second);
	BOOST_CHECK_EQUAL(count, 2);
}

BOOST_AUTO_TEST_SUITE_END()
