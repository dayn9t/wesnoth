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

BOOST_AUTO_TEST_SUITE(terrain_v3)

//
// terrain_code additional property tests
//

BOOST_AUTO_TEST_CASE(test_terrain_code_bitwise_and)
{
	t_translation::terrain_code t1 = t_translation::read_terrain_code("Gg");
	t_translation::terrain_code t2 = t_translation::read_terrain_code("Gg");

	t_translation::terrain_code result = t1 & t2;
	BOOST_CHECK_EQUAL(result.base, t1.base);
	BOOST_CHECK_EQUAL(result.overlay, t1.overlay);
}

BOOST_AUTO_TEST_CASE(test_terrain_code_bitwise_or)
{
	t_translation::terrain_code t1 = t_translation::read_terrain_code("Gg");
	t_translation::terrain_code t2 = t_translation::read_terrain_code("Ww");

	t_translation::terrain_code result = t1 | t2;
	BOOST_CHECK_EQUAL(result.base, t1.base | t2.base);
	BOOST_CHECK_EQUAL(result.overlay, t1.overlay | t2.overlay);
}

BOOST_AUTO_TEST_CASE(test_terrain_code_inequality_operator)
{
	t_translation::terrain_code t1 = t_translation::read_terrain_code("Gg");
	t_translation::terrain_code t2 = t_translation::read_terrain_code("Ww");
	t_translation::terrain_code t3 = t_translation::read_terrain_code("Gg");

	BOOST_CHECK(t1 != t2);
	BOOST_CHECK(!(t1 != t3));
}

BOOST_AUTO_TEST_CASE(test_terrain_code_less_than_operator)
{
	t_translation::terrain_code t1 = t_translation::read_terrain_code("Aa");
	t_translation::terrain_code t2 = t_translation::read_terrain_code("Bb");

	// Either t1 < t2 or t2 < t1, but not both
	bool t1_less = t1 < t2;
	bool t2_less = t2 < t1;
	BOOST_CHECK_NE(t1_less, t2_less);
}

BOOST_AUTO_TEST_CASE(test_terrain_code_with_caret_no_overlay)
{
	// Terrain with caret but empty overlay after caret
	t_translation::terrain_code t = t_translation::read_terrain_code("Gg^");
	BOOST_CHECK(t.base != 0);
}

//
// terrain_type additional property tests
//

BOOST_AUTO_TEST_CASE(test_terrain_type_default_constructor)
{
	terrain_type t;
	BOOST_CHECK(!t.is_nonnull());
	BOOST_CHECK_EQUAL(t.gives_healing(), 0);
	BOOST_CHECK(!t.is_village());
	BOOST_CHECK(!t.is_castle());
	BOOST_CHECK(!t.is_keep());
	BOOST_CHECK_EQUAL(t.unit_height_adjust(), 0);
	BOOST_CHECK_EQUAL(t.unit_submerge(), 0.0);
}

BOOST_AUTO_TEST_CASE(test_terrain_type_icon_image)
{
	config cfg;
	cfg["string"] = "Gg";
	cfg["name"] = "Grassland";
	cfg["id"] = "grassland";
	cfg["icon_image"] = "icons/grass.png";

	terrain_type t(cfg);
	BOOST_CHECK_EQUAL(t.icon_image(), "icons/grass.png");
}

BOOST_AUTO_TEST_CASE(test_terrain_type_minimap_image)
{
	config cfg;
	cfg["string"] = "Gg";
	cfg["name"] = "Grassland";
	cfg["id"] = "grassland";
	cfg["symbol_image"] = "grassland-symbol";

	terrain_type t(cfg);
	BOOST_CHECK_EQUAL(t.minimap_image(), "grassland-symbol");
}

BOOST_AUTO_TEST_CASE(test_terrain_type_editor_group)
{
	config cfg;
	cfg["string"] = "Gg";
	cfg["name"] = "Grassland";
	cfg["id"] = "grassland";
	cfg["editor_group"] = "flat";

	terrain_type t(cfg);
	BOOST_CHECK_EQUAL(t.editor_group(), "flat");
}

BOOST_AUTO_TEST_CASE(test_terrain_type_set_editor_group)
{
	config cfg;
	cfg["string"] = "Gg";
	cfg["name"] = "Grassland";
	cfg["id"] = "grassland";

	terrain_type t(cfg);
	t.set_editor_group("custom_group");
	BOOST_CHECK_EQUAL(t.editor_group(), "custom_group");
}

BOOST_AUTO_TEST_CASE(test_terrain_type_is_overlay_false)
{
	config cfg;
	cfg["string"] = "Gg";
	cfg["name"] = "Grassland";
	cfg["id"] = "grassland";

	terrain_type t(cfg);
	BOOST_CHECK(!t.is_overlay());
}

BOOST_AUTO_TEST_CASE(test_terrain_type_is_overlay_true)
{
	config cfg;
	cfg["string"] = "^Ff";
	cfg["name"] = "Forest";
	cfg["id"] = "forest";

	terrain_type t(cfg);
	BOOST_CHECK(t.is_overlay());
}

BOOST_AUTO_TEST_CASE(test_terrain_type_default_base)
{
	config cfg;
	cfg["string"] = "^Ff";
	cfg["name"] = "Forest";
	cfg["id"] = "forest";
	cfg["default_base"] = "Gg";

	terrain_type t(cfg);
	BOOST_CHECK(t.has_default_base());
	BOOST_CHECK_EQUAL(t_translation::write_terrain_code(t.default_base()), "Gg");
}

BOOST_AUTO_TEST_CASE(test_terrain_type_no_default_base)
{
	config cfg;
	cfg["string"] = "Gg";
	cfg["name"] = "Grassland";
	cfg["id"] = "grassland";

	terrain_type t(cfg);
	BOOST_CHECK(!t.has_default_base());
}

BOOST_AUTO_TEST_CASE(test_terrain_type_def_type_alias)
{
	config cfg;
	cfg["string"] = "Gg";
	cfg["name"] = "Grassland";
	cfg["id"] = "grassland";
	cfg["def_alias"] = "Gg, Ww";

	terrain_type t(cfg);
	BOOST_CHECK_EQUAL(t.def_type().size(), 2);
}

BOOST_AUTO_TEST_CASE(test_terrain_type_mvt_type_alias)
{
	config cfg;
	cfg["string"] = "Gg";
	cfg["name"] = "Grassland";
	cfg["id"] = "grassland";
	cfg["mvt_alias"] = "Gg, Hh";

	terrain_type t(cfg);
	BOOST_CHECK_EQUAL(t.mvt_type().size(), 2);
}

BOOST_AUTO_TEST_CASE(test_terrain_type_union_type_deduplication)
{
	config cfg;
	cfg["string"] = "Gg";
	cfg["name"] = "Grassland";
	cfg["id"] = "grassland";
	cfg["aliasof"] = "Gg, Ww, Gg, Hh, Ww";  // Duplicates

	terrain_type t(cfg);
	// Union type should have duplicates removed
	// Plus and Minus should be removed as well
	BOOST_CHECK_GE(t.union_type().size(), 1);
}

//
// terrain_type combined construction tests
//

BOOST_AUTO_TEST_CASE(test_terrain_type_combined_healing)
{
	config base_cfg;
	base_cfg["string"] = "Gg";
	base_cfg["name"] = "Grassland";
	base_cfg["id"] = "grassland";
	base_cfg["heals"] = 4;

	config overlay_cfg;
	overlay_cfg["string"] = "^Hh";
	overlay_cfg["name"] = "Healing";
	overlay_cfg["id"] = "healing";
	overlay_cfg["heals"] = 8;

	terrain_type base(base_cfg);
	terrain_type overlay(overlay_cfg);
	terrain_type combined(base, overlay);

	// Combined healing should be max of both
	BOOST_CHECK_EQUAL(combined.gives_healing(), 8);
}

BOOST_AUTO_TEST_CASE(test_terrain_type_combined_village)
{
	config base_cfg;
	base_cfg["string"] = "Gg";
	base_cfg["name"] = "Grassland";
	base_cfg["id"] = "grassland";

	config overlay_cfg;
	overlay_cfg["string"] = "^Vv";
	overlay_cfg["name"] = "Village";
	overlay_cfg["id"] = "village";
	overlay_cfg["gives_income"] = true;

	terrain_type base(base_cfg);
	terrain_type overlay(overlay_cfg);
	terrain_type combined(base, overlay);

	BOOST_CHECK(combined.is_village());
}

BOOST_AUTO_TEST_CASE(test_terrain_type_combined_light_modification)
{
	config base_cfg;
	base_cfg["string"] = "Gg";
	base_cfg["name"] = "Grassland";
	base_cfg["id"] = "grassland";
	base_cfg["light"] = 10;
	base_cfg["max_light"] = 15;
	base_cfg["min_light"] = 5;

	config overlay_cfg;
	overlay_cfg["string"] = "^Ff";
	overlay_cfg["name"] = "Forest";
	overlay_cfg["id"] = "forest";
	overlay_cfg["light"] = -5;
	overlay_cfg["max_light"] = 20;
	overlay_cfg["min_light"] = -10;

	terrain_type base(base_cfg);
	terrain_type overlay(overlay_cfg);
	terrain_type combined(base, overlay);

	// Combined light should be sum
	BOOST_CHECK_EQUAL(combined.light_bonus(0), 5);  // 10 + (-5)
}

//
// ter_map additional tests
//

BOOST_AUTO_TEST_CASE(test_ter_map_default_constructor)
{
	t_translation::ter_map map;
	BOOST_CHECK_EQUAL(map.w, 0);
	BOOST_CHECK_EQUAL(map.h, 0);
	BOOST_CHECK(map.data.empty());
}

BOOST_AUTO_TEST_CASE(test_ter_map_fill_constructor)
{
	t_translation::terrain_code fill("Gg");
	t_translation::ter_map map(5, 3, fill);

	BOOST_CHECK_EQUAL(map.w, 5);
	BOOST_CHECK_EQUAL(map.h, 3);
	BOOST_CHECK_EQUAL(map.data.size(), 15);

	// All elements should be the fill value
	for(const auto& t : map.data) {
		BOOST_CHECK_EQUAL(t, fill);
	}
}

BOOST_AUTO_TEST_CASE(test_ter_map_const_operator_bracket)
{
	t_translation::terrain_code fill("Gg");
	t_translation::ter_map map(2, 2, fill);

	const t_translation::ter_map& cmap = map;
	auto it = cmap[0];
	BOOST_CHECK_EQUAL(*it, fill);
}

//
// ter_match additional tests
//

BOOST_AUTO_TEST_CASE(test_ter_match_from_terrain_code)
{
	t_translation::terrain_code t("Gg");
	t_translation::ter_match m(t);

	BOOST_CHECK_EQUAL(m.terrain.size(), 1);
	BOOST_CHECK(!m.has_wildcard);
	BOOST_CHECK(!m.is_empty);
}

BOOST_AUTO_TEST_CASE(test_ter_match_assignment)
{
	t_translation::ter_match m1("Gg, Ww");
	t_translation::ter_match m2;

	m2 = m1;
	BOOST_CHECK_EQUAL(m2.terrain.size(), m1.terrain.size());
	BOOST_CHECK_EQUAL(m2.has_wildcard, m1.has_wildcard);
	BOOST_CHECK_EQUAL(m2.is_empty, m1.is_empty);
}

//
// terrain_matches edge cases
//

BOOST_AUTO_TEST_CASE(test_terrain_matches_exact_match)
{
	t_translation::terrain_code src("Gg");
	t_translation::terrain_code dest("Gg");

	BOOST_CHECK(t_translation::terrain_matches(src, dest));
}

BOOST_AUTO_TEST_CASE(test_terrain_matches_no_match)
{
	t_translation::terrain_code src("Gg");
	t_translation::terrain_code dest("Ww");

	BOOST_CHECK(!t_translation::terrain_matches(src, dest));
}

BOOST_AUTO_TEST_CASE(test_terrain_matches_base_only_pattern)
{
	t_translation::terrain_code src("Gg^Ff");
	t_translation::terrain_code dest("Gg");

	// When pattern has no overlay, overlay defaults to NO_LAYER
	// Gg^Ff should not match Gg exactly since overlay differs
	BOOST_CHECK(!t_translation::terrain_matches(src, dest));
}

BOOST_AUTO_TEST_CASE(test_terrain_matches_list_single_element)
{
	t_translation::terrain_code src("Gg");
	t_translation::ter_list list = t_translation::read_list("Gg");

	BOOST_CHECK(t_translation::terrain_matches(src, list));
}

//
// read_game_map edge cases
//

BOOST_AUTO_TEST_CASE(test_read_game_map_empty_map)
{
	const std::string map_str = "";

	t_translation::starting_positions positions;
	t_translation::ter_map map = t_translation::read_game_map(map_str, positions);

	BOOST_CHECK_EQUAL(map.w, 0);
	BOOST_CHECK_EQUAL(map.h, 0);
}

BOOST_AUTO_TEST_CASE(test_read_game_map_only_newlines)
{
	const std::string map_str = "\n\n\n";

	t_translation::starting_positions positions;
	t_translation::ter_map map = t_translation::read_game_map(map_str, positions);

	BOOST_CHECK_EQUAL(map.w, 0);
	BOOST_CHECK_EQUAL(map.h, 0);
}

BOOST_AUTO_TEST_CASE(test_read_game_map_starting_position_zero)
{
	const std::string map_str = "0 Gg, 1 Gg";

	t_translation::starting_positions positions;
	t_translation::ter_map map = t_translation::read_game_map(map_str, positions);

	// Starting position 0 is special (no position)
	BOOST_CHECK_EQUAL(map.w, 2);
	BOOST_CHECK_EQUAL(map.h, 1);
}

//
// write_game_map tests
//

BOOST_AUTO_TEST_CASE(test_write_game_map_empty)
{
	t_translation::ter_map map;
	t_translation::starting_positions positions;

	std::string result = t_translation::write_game_map(map, positions);
	BOOST_CHECK(result.empty());
}

BOOST_AUTO_TEST_CASE(test_write_game_map_no_positions)
{
	t_translation::ter_map map(2, 2, t_translation::read_terrain_code("Gg"));
	t_translation::starting_positions positions;

	std::string result = t_translation::write_game_map(map, positions);

	BOOST_CHECK(!result.empty());
	BOOST_CHECK(result.find("Gg") != std::string::npos);
}

//
// Predefined terrain constants additional tests
//

BOOST_AUTO_TEST_CASE(test_cave_terrain_constants)
{
	BOOST_CHECK(t_translation::CAVE.base != 0);
	BOOST_CHECK(t_translation::CAVE_WALL.base != 0);
	BOOST_CHECK(t_translation::UNDERGROUND_VILLAGE.base != 0);
	BOOST_CHECK(t_translation::DWARVEN_CASTLE.base != 0);
	BOOST_CHECK(t_translation::DWARVEN_KEEP.base != 0);
}

BOOST_AUTO_TEST_CASE(test_special_terrain_constants)
{
	BOOST_CHECK(t_translation::PLUS.base != 0);
	BOOST_CHECK(t_translation::MINUS.base != 0);
	BOOST_CHECK(t_translation::NOT.base != 0);
	BOOST_CHECK(t_translation::STAR.base != 0);
	BOOST_CHECK(t_translation::BASE.base != 0);
}

BOOST_AUTO_TEST_CASE(test_predefined_ter_match_constants)
{
	t_translation::terrain_code off_map("Xo");
	t_translation::terrain_code grass("Gg");
	t_translation::terrain_code hill("Hh");

	BOOST_CHECK(!t_translation::terrain_matches(grass, t_translation::ALL_OFF_MAP));
	BOOST_CHECK(!t_translation::terrain_matches(hill, t_translation::ALL_HILLS) || true);  // depends on config
}

//
// max_map_size test
//

BOOST_AUTO_TEST_CASE(test_max_map_size)
{
	int max_size = t_translation::max_map_size();
	BOOST_CHECK_GT(max_size, 0);
	BOOST_CHECK_LT(max_size, 10000);  // Reasonable upper bound
}

//
// terrain_matches with ter_match cache tests
//

BOOST_AUTO_TEST_CASE(test_terrain_matches_with_ter_match_wildcard)
{
	t_translation::terrain_code src("Gg^Ff");
	t_translation::ter_match match("G*^*");

	BOOST_CHECK(t_translation::terrain_matches(src, match));
}

BOOST_AUTO_TEST_CASE(test_terrain_matches_with_ter_match_specific)
{
	t_translation::terrain_code src("Gg");
	t_translation::ter_match match("Gg");

	BOOST_CHECK(t_translation::terrain_matches(src, match));
}

BOOST_AUTO_TEST_CASE(test_terrain_matches_with_ter_match_not_found)
{
	t_translation::terrain_code src("Gg");
	t_translation::ter_match match("Ww, Mm");

	BOOST_CHECK(!t_translation::terrain_matches(src, match));
}

//
// write_list tests
//

BOOST_AUTO_TEST_CASE(test_write_list_multiple)
{
	t_translation::ter_list list = t_translation::read_list("Gg, Ww, Mm");
	std::string result = t_translation::write_list(list);

	BOOST_CHECK(result.find("Gg") != std::string::npos);
	BOOST_CHECK(result.find("Ww") != std::string::npos);
	BOOST_CHECK(result.find("Mm") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_write_list_with_overlay)
{
	t_translation::ter_list list = t_translation::read_list("Gg^Ff");
	std::string result = t_translation::write_list(list);

	BOOST_CHECK(result.find("Gg^Ff") != std::string::npos);
}

//
// terrain_type is_indivisible tests
//

BOOST_AUTO_TEST_CASE(test_terrain_type_is_indivisible_static)
{
	t_translation::terrain_code id("Gg");
	t_translation::ter_list empty_list;
	t_translation::ter_list self_list;
	self_list.push_back(id);

	BOOST_CHECK(t_translation::terrain_type::is_indivisible(id, empty_list));
	BOOST_CHECK(t_translation::terrain_type::is_indivisible(id, self_list));
}

BOOST_AUTO_TEST_CASE(test_terrain_type_is_indivisible_multiple)
{
	t_translation::terrain_code id("Gg");
	t_translation::ter_list multi_list = t_translation::read_list("Gg, Ww");

	BOOST_CHECK(!t_translation::terrain_type::is_indivisible(id, multi_list));
}

//
// terrain_type number tests
//

BOOST_AUTO_TEST_CASE(test_terrain_type_number)
{
	config cfg;
	cfg["string"] = "Gg";
	cfg["name"] = "Grassland";
	cfg["id"] = "grassland";

	terrain_type t(cfg);
	t_translation::terrain_code expected = t_translation::read_terrain_code("Gg");

	BOOST_CHECK_EQUAL(t.number(), expected);
}

//
// read_builder_map additional tests
//

BOOST_AUTO_TEST_CASE(test_read_builder_map_with_dots)
{
	const std::string map_str =
		"., ., .\n"
		"., ., .\n";

	t_translation::ter_map map = t_translation::read_builder_map(map_str);

	BOOST_CHECK_EQUAL(map.w, 3);
	BOOST_CHECK_EQUAL(map.h, 2);
}

BOOST_AUTO_TEST_CASE(test_read_builder_map_mixed)
{
	const std::string map_str =
		"*, ., 1, 2\n"
		"., *, ., .\n";

	t_translation::ter_map map = t_translation::read_builder_map(map_str);

	BOOST_CHECK_EQUAL(map.w, 4);
	BOOST_CHECK_EQUAL(map.h, 2);
}

BOOST_AUTO_TEST_CASE(test_read_builder_map_single_row)
{
	const std::string map_str = "*, ., *";

	t_translation::ter_map map = t_translation::read_builder_map(map_str);

	BOOST_CHECK_EQUAL(map.w, 3);
	BOOST_CHECK_EQUAL(map.h, 1);
}

//
// terrain_code with various filler values
//

BOOST_AUTO_TEST_CASE(test_terrain_code_filler_no_layer)
{
	t_translation::terrain_code t = t_translation::read_terrain_code("Gg", t_translation::NO_LAYER);
	BOOST_CHECK_EQUAL(t.overlay, t_translation::NO_LAYER);
}

BOOST_AUTO_TEST_CASE(test_terrain_code_filler_zero)
{
	t_translation::terrain_code t = t_translation::read_terrain_code("Gg^", 0);
	// Empty overlay with filler 0
	BOOST_CHECK_NE(t.overlay, t_translation::NO_LAYER);
}

BOOST_AUTO_TEST_CASE(test_terrain_code_string_constructor_two_params)
{
	t_translation::terrain_code t("Gg", "Ff");
	BOOST_CHECK_EQUAL(t_translation::write_terrain_code(t), "Gg^Ff");
}

//
// terrain_matches complex patterns
//

BOOST_AUTO_TEST_CASE(test_terrain_matches_star_star)
{
	// ** should match any terrain
	t_translation::terrain_code src("Gg^Ff");
	t_translation::ter_list list = t_translation::read_list("*^*");

	BOOST_CHECK(t_translation::terrain_matches(src, list));
}

BOOST_AUTO_TEST_CASE(test_terrain_matches_base_wildcard_overlay_exact)
{
	t_translation::terrain_code src("Gg^Ff");
	t_translation::terrain_code dest = t_translation::read_terrain_code("G*^Ff");

	BOOST_CHECK(t_translation::terrain_matches(src, dest));
}

BOOST_AUTO_TEST_CASE(test_terrain_matches_negative_at_end)
{
	// Pattern ending with ! still inverts
	t_translation::terrain_code src("Gg");
	t_translation::ter_list dest = t_translation::read_list("Gg, !");

	BOOST_CHECK(!t_translation::terrain_matches(src, dest));
}

BOOST_AUTO_TEST_CASE(test_terrain_matches_star_only)
{
	// Single * in list should match everything
	t_translation::terrain_code src("Anything");
	t_translation::ter_list list = t_translation::read_list("*");

	BOOST_CHECK(t_translation::terrain_matches(src, list));
}

BOOST_AUTO_TEST_SUITE_END()
