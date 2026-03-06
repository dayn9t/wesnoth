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

BOOST_AUTO_TEST_SUITE(terrain)

//
// terrain_code tests
//

BOOST_AUTO_TEST_CASE(test_terrain_code_default_construction)
{
	t_translation::terrain_code t;
	BOOST_CHECK_EQUAL(t.base, 0);
	BOOST_CHECK_EQUAL(t.overlay, t_translation::NO_LAYER);
}

BOOST_AUTO_TEST_CASE(test_terrain_code_construction_with_base_only)
{
	t_translation::terrain_code t("Gg");
	BOOST_CHECK(t.base != 0);
	BOOST_CHECK_EQUAL(t.overlay, t_translation::NO_LAYER);
}

BOOST_AUTO_TEST_CASE(test_terrain_code_construction_with_base_and_overlay)
{
	t_translation::terrain_code t("Gg", "Ff");
	BOOST_CHECK(t.base != 0);
	BOOST_CHECK(t.overlay != t_translation::NO_LAYER);
	BOOST_CHECK(t.overlay != 0);
}

BOOST_AUTO_TEST_CASE(test_terrain_code_constexpr_construction)
{
	constexpr t_translation::terrain_code t(0x47670000, t_translation::NO_LAYER);
	BOOST_CHECK_EQUAL(t.base, 0x47670000);
	BOOST_CHECK_EQUAL(t.overlay, t_translation::NO_LAYER);
}

BOOST_AUTO_TEST_CASE(test_terrain_code_equality)
{
	t_translation::terrain_code t1("Gg");
	t_translation::terrain_code t2("Gg");
	t_translation::terrain_code t3("Gg", "Ff");
	t_translation::terrain_code t4("Gg", "Ff");
	t_translation::terrain_code t5("Ww");

	BOOST_CHECK(t1 == t2);
	BOOST_CHECK(t3 == t4);
	BOOST_CHECK(t1 != t3);
	BOOST_CHECK(t1 != t5);
}

BOOST_AUTO_TEST_CASE(test_terrain_code_comparison)
{
	t_translation::terrain_code t1("Aa");
	t_translation::terrain_code t2("Bb");

	// Comparison should be based on base first, then overlay
	BOOST_CHECK(t1 < t2 || t2 < t1); // They should be orderable
	BOOST_CHECK(!(t1 < t2 && t2 < t1)); // Ordering should be consistent
}

BOOST_AUTO_TEST_CASE(test_terrain_code_bitwise_operators)
{
	t_translation::terrain_code t1(0x12345678, 0x87654321);
	t_translation::terrain_code t2(0xFF00FF00, 0x0FF00FF0);

	t_translation::terrain_code and_result = t1 & t2;
	t_translation::terrain_code or_result = t1 | t2;

	BOOST_CHECK_EQUAL(and_result.base, 0x12005600);
	BOOST_CHECK_EQUAL(and_result.overlay, 0x07604320);
	BOOST_CHECK_EQUAL(or_result.base, 0xFF34FF78);
	BOOST_CHECK_EQUAL(or_result.overlay, 0x8FF54FF1);
}

//
// read_terrain_code / write_terrain_code tests
//

BOOST_AUTO_TEST_CASE(test_read_write_terrain_code_simple)
{
	const std::string str = "Gg";
	t_translation::terrain_code t = t_translation::read_terrain_code(str);
	std::string result = t_translation::write_terrain_code(t);
	BOOST_CHECK_EQUAL(result, str);
}

BOOST_AUTO_TEST_CASE(test_read_write_terrain_code_with_overlay)
{
	const std::string str = "Gg^Ff";
	t_translation::terrain_code t = t_translation::read_terrain_code(str);
	std::string result = t_translation::write_terrain_code(t);
	BOOST_CHECK_EQUAL(result, str);
}

BOOST_AUTO_TEST_CASE(test_read_terrain_code_wildcard)
{
	t_translation::terrain_code t = t_translation::read_terrain_code("G*");
	BOOST_CHECK(t_translation::has_wildcard(t));
}

BOOST_AUTO_TEST_CASE(test_read_terrain_code_star_wildcard)
{
	t_translation::terrain_code t = t_translation::read_terrain_code("*");
	BOOST_CHECK(t_translation::has_wildcard(t));
}

//
// ter_list tests
//

BOOST_AUTO_TEST_CASE(test_read_list_single)
{
	const std::string str = "Gg";
	t_translation::ter_list list = t_translation::read_list(str);
	BOOST_CHECK_EQUAL(list.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_read_list_multiple)
{
	const std::string str = "Gg, Ww, Mm";
	t_translation::ter_list list = t_translation::read_list(str);
	BOOST_CHECK_EQUAL(list.size(), 3);
}

BOOST_AUTO_TEST_CASE(test_read_list_empty)
{
	const std::string str = "";
	t_translation::ter_list list = t_translation::read_list(str);
	BOOST_CHECK_EQUAL(list.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_write_list)
{
	t_translation::ter_list list;
	list.push_back(t_translation::read_terrain_code("Gg"));
	list.push_back(t_translation::read_terrain_code("Ww"));

	std::string result = t_translation::write_list(list);
	BOOST_CHECK_EQUAL(result, "Gg, Ww");
}

//
// terrain_matches tests
//

BOOST_AUTO_TEST_CASE(test_terrain_matches_exact)
{
	t_translation::terrain_code src("Gg");
	t_translation::terrain_code dest("Gg");

	BOOST_CHECK(t_translation::terrain_matches(src, dest));
}

BOOST_AUTO_TEST_CASE(test_terrain_matches_different)
{
	t_translation::terrain_code src("Gg");
	t_translation::terrain_code dest("Ww");

	BOOST_CHECK(!t_translation::terrain_matches(src, dest));
}

BOOST_AUTO_TEST_CASE(test_terrain_matches_wildcard_base)
{
	t_translation::terrain_code src("Gg");
	t_translation::terrain_code dest("G*");

	BOOST_CHECK(t_translation::terrain_matches(src, dest));
}

BOOST_AUTO_TEST_CASE(test_terrain_matches_wildcard_both)
{
	t_translation::terrain_code src("Gg");
	t_translation::terrain_code star("*");

	BOOST_CHECK(t_translation::terrain_matches(src, star));
}

BOOST_AUTO_TEST_CASE(test_terrain_matches_list)
{
	t_translation::terrain_code src("Gg");
	t_translation::ter_list dest = t_translation::read_list("Ww, Gg, Mm");

	BOOST_CHECK(t_translation::terrain_matches(src, dest));
}

BOOST_AUTO_TEST_CASE(test_terrain_matches_list_no_match)
{
	t_translation::terrain_code src("Gg");
	t_translation::ter_list dest = t_translation::read_list("Ww, Mm, Hh");

	BOOST_CHECK(!t_translation::terrain_matches(src, dest));
}

BOOST_AUTO_TEST_CASE(test_terrain_matches_with_not)
{
	t_translation::terrain_code src("Gg");
	t_translation::ter_list dest = t_translation::read_list("!, G*");

	// The ! inverts the match - Gg matches G* but ! inverts to false
	BOOST_CHECK(!t_translation::terrain_matches(src, dest));
}

//
// has_wildcard tests
//

BOOST_AUTO_TEST_CASE(test_has_wildcard_code_yes)
{
	t_translation::terrain_code t = t_translation::read_terrain_code("G*");
	BOOST_CHECK(t_translation::has_wildcard(t));
}

BOOST_AUTO_TEST_CASE(test_has_wildcard_code_no)
{
	t_translation::terrain_code t = t_translation::read_terrain_code("Gg");
	BOOST_CHECK(!t_translation::has_wildcard(t));
}

BOOST_AUTO_TEST_CASE(test_has_wildcard_list_yes)
{
	t_translation::ter_list list = t_translation::read_list("Gg, G*, Ww");
	BOOST_CHECK(t_translation::has_wildcard(list));
}

BOOST_AUTO_TEST_CASE(test_has_wildcard_list_no)
{
	t_translation::ter_list list = t_translation::read_list("Gg, Ww, Mm");
	BOOST_CHECK(!t_translation::has_wildcard(list));
}

//
// ter_match tests
//

BOOST_AUTO_TEST_CASE(test_ter_match_default_construction)
{
	t_translation::ter_match m;
	BOOST_CHECK(m.is_empty);
	BOOST_CHECK(!m.has_wildcard);
}

BOOST_AUTO_TEST_CASE(test_ter_match_from_string)
{
	t_translation::ter_match m("Gg");
	BOOST_CHECK(!m.is_empty);
}

BOOST_AUTO_TEST_CASE(test_ter_match_from_terrain_code)
{
	t_translation::terrain_code t("Gg");
	t_translation::ter_match m(t);
	BOOST_CHECK(!m.is_empty);
}

//
// ter_map tests
//

BOOST_AUTO_TEST_CASE(test_ter_map_default_construction)
{
	t_translation::ter_map map;
	BOOST_CHECK_EQUAL(map.w, 0);
	BOOST_CHECK_EQUAL(map.h, 0);
	BOOST_CHECK_EQUAL(map.data.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_ter_map_sized_construction)
{
	t_translation::ter_map map(3, 2);
	BOOST_CHECK_EQUAL(map.w, 3);
	BOOST_CHECK_EQUAL(map.h, 2);
	BOOST_CHECK_EQUAL(map.data.size(), 6);
}

BOOST_AUTO_TEST_CASE(test_ter_map_filled_construction)
{
	t_translation::terrain_code fill("Gg");
	t_translation::ter_map map(2, 2, fill);

	BOOST_CHECK_EQUAL(map.get(0, 0), fill);
	BOOST_CHECK_EQUAL(map.get(1, 0), fill);
	BOOST_CHECK_EQUAL(map.get(0, 1), fill);
	BOOST_CHECK_EQUAL(map.get(1, 1), fill);
}

//
// read_game_map / write_game_map tests
//

BOOST_AUTO_TEST_CASE(test_read_game_map_simple)
{
	const std::string map_str =
		"Gg, Gg, Gg\n"
		"Gg, Gg, Gg\n";

	t_translation::starting_positions positions;
	t_translation::ter_map map = t_translation::read_game_map(map_str, positions);

	BOOST_CHECK_EQUAL(map.w, 3);
	BOOST_CHECK_EQUAL(map.h, 2);
}

BOOST_AUTO_TEST_CASE(test_read_game_map_with_starting_positions)
{
	const std::string map_str =
		"1 Gg, Gg, Gg\n"
		" Gg, 2 Gg, Gg\n";

	t_translation::starting_positions positions;
	t_translation::ter_map map = t_translation::read_game_map(map_str, positions);

	BOOST_CHECK_EQUAL(map.w, 3);
	BOOST_CHECK_EQUAL(map.h, 2);
	// Note: positions should contain entries for starting positions 1 and 2
	BOOST_CHECK(positions.left.find("1") != positions.left.end());
	BOOST_CHECK(positions.left.find("2") != positions.left.end());
}

BOOST_AUTO_TEST_CASE(test_write_game_map_roundtrip)
{
	const std::string map_str =
		"Gg, Gg, Gg\n"
		"Ww, Ww, Ww\n";

	t_translation::starting_positions positions;
	t_translation::ter_map map = t_translation::read_game_map(map_str, positions);

	std::string result = t_translation::write_game_map(map, positions);

	// Read the result back and verify dimensions match
	t_translation::ter_map map2 = t_translation::read_game_map(result, positions);
	BOOST_CHECK_EQUAL(map2.w, map.w);
	BOOST_CHECK_EQUAL(map2.h, map.h);
}

//
// terrain_type tests (basic tests that don't require WML initialization)
//

BOOST_AUTO_TEST_CASE(test_terrain_type_default_construction)
{
	terrain_type t;
	// Default constructed terrain_type should have is_nonnull() == false
	BOOST_CHECK(!t.is_nonnull());
}

BOOST_AUTO_TEST_CASE(test_terrain_type_config_construction)
{
	config cfg;
	cfg["string"] = "Gg";
	cfg["name"] = "Grassland";
	cfg["id"] = "grassland";

	terrain_type t(cfg);
	// Basic check that it was constructed
	BOOST_CHECK_EQUAL(t.id(), "grassland");
}

BOOST_AUTO_TEST_CASE(test_terrain_type_is_indivisible_static)
{
	t_translation::terrain_code id("Gg");

	// Empty underlying list means indivisible
	t_translation::ter_list empty_list;
	BOOST_CHECK(terrain_type::is_indivisible(id, empty_list));

	// List containing only itself means indivisible
	t_translation::ter_list self_list;
	self_list.push_back(id);
	BOOST_CHECK(terrain_type::is_indivisible(id, self_list));

	// List containing something else means not indivisible
	t_translation::ter_list other_list;
	other_list.push_back(t_translation::read_terrain_code("Ww"));
	BOOST_CHECK(!terrain_type::is_indivisible(id, other_list));
}

//
// NONE_TERRAIN constant tests
//

BOOST_AUTO_TEST_CASE(test_none_terrain)
{
	t_translation::terrain_code none = t_translation::NONE_TERRAIN;
	BOOST_CHECK_EQUAL(none.base, 0);
	BOOST_CHECK_EQUAL(none.overlay, t_translation::NO_LAYER);
}

BOOST_AUTO_TEST_CASE(test_no_layer_value)
{
	BOOST_CHECK_EQUAL(t_translation::NO_LAYER, 0xFFFFFFFF);
}

//
// max_map_size test
//

BOOST_AUTO_TEST_CASE(test_max_map_size)
{
	int max_size = t_translation::max_map_size();
	BOOST_CHECK(max_size > 0);
	BOOST_CHECK(max_size < 10000); // Reasonable upper bound
}

BOOST_AUTO_TEST_SUITE_END()
