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
#include "tstring.hpp"
#include "units/attack_type.hpp"
#include "units/filter.hpp"
#include "units/types.hpp"
#include "units/unit.hpp"
#include "tests/utils/game_config_manager_tests.hpp"

#include <memory>

BOOST_AUTO_TEST_SUITE( units_suite )

/**
 * Test attack_type construction and basic properties
 */
BOOST_AUTO_TEST_CASE( test_attack_type_construction )
{
	// Test basic attack_type construction
	// Note: attack_type.id_ is initialized from cfg["name"], not cfg["id"]
	config attack_cfg;
	attack_cfg["name"] = "sword";
	attack_cfg["type"] = "blade";
	attack_cfg["range"] = "melee";
	attack_cfg["damage"] = 10;
	attack_cfg["number"] = 3;
	attack_cfg["icon"] = "attacks/sword.png";

	auto attack = std::make_shared<attack_type>(attack_cfg);

	// id() returns the value from cfg["name"], not cfg["id"]
	BOOST_CHECK_EQUAL(attack->id(), "sword");
	BOOST_CHECK_EQUAL(attack->type(), "blade");
	BOOST_CHECK_EQUAL(attack->range(), "melee");
	BOOST_CHECK_EQUAL(attack->damage(), 10);
	BOOST_CHECK_EQUAL(attack->num_attacks(), 3);
	BOOST_CHECK_EQUAL(attack->icon(), "attacks/sword.png");

	// Test setter methods
	attack->set_damage(15);
	BOOST_CHECK_EQUAL(attack->damage(), 15);

	attack->set_num_attacks(4);
	BOOST_CHECK_EQUAL(attack->num_attacks(), 4);

	attack->set_type("pierce");
	BOOST_CHECK_EQUAL(attack->type(), "pierce");

	attack->set_range("ranged");
	BOOST_CHECK_EQUAL(attack->range(), "ranged");
}

/**
 * Test attack_type with specials
 */
BOOST_AUTO_TEST_CASE( test_attack_type_specials )
{
	config attack_cfg;
	attack_cfg["name"] = "fire breath";
	attack_cfg["type"] = "fire";
	attack_cfg["range"] = "ranged";
	attack_cfg["damage"] = 8;
	attack_cfg["number"] = 2;

	// Add weapon special
	config& special = attack_cfg.add_child("specials").add_child("dummy");
	special["id"] = "test_special";
	special["name"] = "Test Special";

	auto attack = std::make_shared<attack_type>(attack_cfg);

	// Note: has_special_or_ability requires a display context which is not available in unit tests
	// Instead, verify the special was parsed by checking the specials list directly
	BOOST_CHECK_EQUAL(attack->specials().size(), 1);

	// Test writing and reading back
	config written_cfg;
	attack->write(written_cfg);

	auto attack2 = std::make_shared<attack_type>(written_cfg);
	BOOST_CHECK_EQUAL(attack2->specials().size(), 1);
}

/**
 * Test attack_type attack_empty method
 */
BOOST_AUTO_TEST_CASE( test_attack_type_empty )
{
	// Empty attack
	config empty_cfg;
	auto empty_attack = std::make_shared<attack_type>(empty_cfg);
	BOOST_CHECK(empty_attack->attack_empty());

	// Non-empty attack
	config attack_cfg;
	attack_cfg["name"] = "sword";
	attack_cfg["id"] = "sword";
	attack_cfg["type"] = "blade";
	attack_cfg["range"] = "melee";
	attack_cfg["damage"] = 5;
	attack_cfg["number"] = 2;

	auto attack = std::make_shared<attack_type>(attack_cfg);
	BOOST_CHECK(!attack->attack_empty());
}

/**
 * Test attack_type weight properties
 */
BOOST_AUTO_TEST_CASE( test_attack_type_weights )
{
	config attack_cfg;
	attack_cfg["name"] = "bow";
	attack_cfg["type"] = "pierce";
	attack_cfg["range"] = "ranged";
	attack_cfg["damage"] = 6;
	attack_cfg["number"] = 3;
	attack_cfg["attack_weight"] = 0.8;
	attack_cfg["defense_weight"] = 0.5;

	auto attack = std::make_shared<attack_type>(attack_cfg);

	BOOST_CHECK_CLOSE(attack->attack_weight(), 0.8, 0.001);
	BOOST_CHECK_CLOSE(attack->defense_weight(), 0.5, 0.001);

	// Test setters
	attack->set_attack_weight(1.0);
	BOOST_CHECK_CLOSE(attack->attack_weight(), 1.0, 0.001);

	attack->set_defense_weight(0.7);
	BOOST_CHECK_CLOSE(attack->defense_weight(), 0.7, 0.001);
}

/**
 * Test attack_type accuracy and parry
 */
BOOST_AUTO_TEST_CASE( test_attack_type_accuracy_parry )
{
	config attack_cfg;
	attack_cfg["name"] = "crossbow";
	attack_cfg["type"] = "pierce";
	attack_cfg["range"] = "ranged";
	attack_cfg["damage"] = 7;
	attack_cfg["number"] = 3;
	attack_cfg["accuracy"] = 10;
	attack_cfg["parry"] = -5;

	auto attack = std::make_shared<attack_type>(attack_cfg);

	BOOST_CHECK_EQUAL(attack->accuracy(), 10);
	BOOST_CHECK_EQUAL(attack->parry(), -5);

	// Test setters
	attack->set_accuracy(15);
	BOOST_CHECK_EQUAL(attack->accuracy(), 15);

	attack->set_parry(0);
	BOOST_CHECK_EQUAL(attack->parry(), 0);
}

/**
 * Test attack_type movement and attacks used
 */
BOOST_AUTO_TEST_CASE( test_attack_type_movement_used )
{
	config attack_cfg;
	attack_cfg["name"] = "heavy strike";
	attack_cfg["type"] = "impact";
	attack_cfg["range"] = "melee";
	attack_cfg["damage"] = 12;
	attack_cfg["number"] = 2;
	attack_cfg["movement_used"] = 2;
	attack_cfg["attacks_used"] = 1;

	auto attack = std::make_shared<attack_type>(attack_cfg);

	BOOST_CHECK_EQUAL(attack->movement_used(), 2);
	BOOST_CHECK_EQUAL(attack->attacks_used(), 1);

	// Test setters
	attack->set_movement_used(1);
	BOOST_CHECK_EQUAL(attack->movement_used(), 1);

	attack->set_attacks_used(2);
	BOOST_CHECK_EQUAL(attack->attacks_used(), 2);
}

/**
 * Test attack_type to_config method
 */
BOOST_AUTO_TEST_CASE( test_attack_type_to_config )
{
	// Note: attack_type.id_ is initialized from cfg["name"], and write() stores id_ as "name"
	config attack_cfg;
	attack_cfg["name"] = "spear";
	attack_cfg["type"] = "pierce";
	attack_cfg["range"] = "melee";
	attack_cfg["damage"] = 7;
	attack_cfg["number"] = 4;
	attack_cfg["icon"] = "attacks/spear.png";

	auto attack = std::make_shared<attack_type>(attack_cfg);

	config written = attack->to_config();

	// write() stores id_ as "name", and id_ comes from cfg["name"]
	BOOST_CHECK_EQUAL(written["name"].str(), "spear");
	BOOST_CHECK_EQUAL(written["type"].str(), "pierce");
	BOOST_CHECK_EQUAL(written["range"].str(), "melee");
	BOOST_CHECK_EQUAL(written["damage"].to_int(), 7);
	BOOST_CHECK_EQUAL(written["number"].to_int(), 4);
	BOOST_CHECK_EQUAL(written["icon"].str(), "attacks/spear.png");
}

/**
 * Test attack_type range properties (min/max range)
 */
BOOST_AUTO_TEST_CASE( test_attack_type_range_properties )
{
	config attack_cfg;
	attack_cfg["name"] = "longbow";
	attack_cfg["type"] = "pierce";
	attack_cfg["range"] = "ranged";
	attack_cfg["damage"] = 9;
	attack_cfg["number"] = 2;
	attack_cfg["min_range"] = 1;
	attack_cfg["max_range"] = 3;

	auto attack = std::make_shared<attack_type>(attack_cfg);

	BOOST_CHECK_EQUAL(attack->min_range(), 1);
	BOOST_CHECK_EQUAL(attack->max_range(), 3);

	// Test setters
	attack->set_min_range(2);
	BOOST_CHECK_EQUAL(attack->min_range(), 2);

	attack->set_max_range(4);
	BOOST_CHECK_EQUAL(attack->max_range(), 4);
}

/**
 * Test unit_filter basic construction
 */
BOOST_AUTO_TEST_CASE( test_unit_filter_construction )
{
	// Test empty filter
	config empty_filter_cfg;
	vconfig empty_vconfig(empty_filter_cfg);
	unit_filter empty_filter(empty_vconfig);
	BOOST_CHECK(empty_filter.empty());

	// Test filter with simple attributes
	config filter_cfg;
	filter_cfg["type"] = "Elvish Fighter";
	filter_cfg["side"] = 1;

	vconfig vcfg(filter_cfg);
	unit_filter filter(vcfg);

	BOOST_CHECK(!filter.empty());

	config retrieved = filter.to_config();
	BOOST_CHECK_EQUAL(retrieved["type"].str(), "Elvish Fighter");
	BOOST_CHECK_EQUAL(retrieved["side"].to_int(), 1);
}

/**
 * Test unit_filter with multiple criteria
 */
BOOST_AUTO_TEST_CASE( test_unit_filter_multiple_criteria )
{
	config filter_cfg;
	filter_cfg["type"] = "Orcish Grunt";
	filter_cfg["side"] = 2;
	filter_cfg["level"] = 1;

	vconfig vcfg(filter_cfg);
	unit_filter filter(vcfg);

	BOOST_CHECK(!filter.empty());

	config retrieved = filter.to_config();
	BOOST_CHECK_EQUAL(retrieved["type"].str(), "Orcish Grunt");
	BOOST_CHECK_EQUAL(retrieved["side"].to_int(), 2);
	BOOST_CHECK_EQUAL(retrieved["level"].to_int(), 1);
}

/**
 * Test unit_filter copy and move semantics
 */
BOOST_AUTO_TEST_CASE( test_unit_filter_copy_move )
{
	config filter_cfg;
	filter_cfg["type"] = "Test Unit";

	vconfig vcfg(filter_cfg);
	unit_filter original(vcfg);

	// Test copy construction
	unit_filter copied(original);
	BOOST_CHECK(!copied.empty());
	BOOST_CHECK_EQUAL(copied.to_config()["type"].str(), "Test Unit");

	// Test copy assignment
	unit_filter assigned = original;
	BOOST_CHECK(!assigned.empty());
	BOOST_CHECK_EQUAL(assigned.to_config()["type"].str(), "Test Unit");

	// Test move construction
	unit_filter moved(std::move(assigned));
	BOOST_CHECK(!moved.empty());
	BOOST_CHECK_EQUAL(moved.to_config()["type"].str(), "Test Unit");

	// Test move assignment
	unit_filter move_assigned = std::move(copied);
	BOOST_CHECK(!move_assigned.empty());
	BOOST_CHECK_EQUAL(move_assigned.to_config()["type"].str(), "Test Unit");
}

/**
 * Test unit_filter set_use_flat_tod method
 */
BOOST_AUTO_TEST_CASE( test_unit_filter_flat_tod )
{
	config filter_cfg;
	filter_cfg["type"] = "Daylight Unit";

	vconfig vcfg(filter_cfg);
	unit_filter filter(vcfg);

	// Test fluent interface for set_use_flat_tod
	unit_filter& ref = filter.set_use_flat_tod(true);
	BOOST_CHECK(&ref == &filter);

	filter.set_use_flat_tod(false);
}

/**
 * Test attack_type with alignment
 */
BOOST_AUTO_TEST_CASE( test_attack_type_alignment )
{
	config attack_cfg;
	attack_cfg["name"] = "holy sword";
	attack_cfg["type"] = "blade";
	attack_cfg["range"] = "melee";
	attack_cfg["damage"] = 10;
	attack_cfg["number"] = 3;
	attack_cfg["alignment"] = "lawful";

	auto attack = std::make_shared<attack_type>(attack_cfg);

	BOOST_CHECK(attack->alignment().has_value());
	BOOST_CHECK_EQUAL(attack->alignment_str(), "lawful");

	// Test setting alignment
	attack->set_attack_alignment("chaotic");
	BOOST_CHECK_EQUAL(attack->alignment_str(), "chaotic");

	// Test neutral alignment
	attack->set_attack_alignment("neutral");
	BOOST_CHECK_EQUAL(attack->alignment_str(), "neutral");
}

/**
 * Test attack_type matches_filter method
 */
BOOST_AUTO_TEST_CASE( test_attack_type_matches_filter )
{
	config attack_cfg;
	attack_cfg["name"] = "axe";
	attack_cfg["id"] = "axe_chop";
	attack_cfg["type"] = "blade";
	attack_cfg["range"] = "melee";
	attack_cfg["damage"] = 9;
	attack_cfg["number"] = 2;

	auto attack = std::make_shared<attack_type>(attack_cfg);

	// Test matching filter
	config filter1;
	filter1["type"] = "blade";
	BOOST_CHECK(attack->matches_filter(filter1));

	// Test matching by range
	config filter2;
	filter2["range"] = "melee";
	BOOST_CHECK(attack->matches_filter(filter2));

	// Test non-matching filter
	config filter3;
	filter3["type"] = "pierce";
	BOOST_CHECK(!attack->matches_filter(filter3));

	// Test multiple criteria
	config filter4;
	filter4["type"] = "blade";
	filter4["range"] = "melee";
	BOOST_CHECK(attack->matches_filter(filter4));
}

/**
 * Test attack_type describe_effect static method
 */
BOOST_AUTO_TEST_CASE( test_attack_type_describe_effect )
{
	// Test damage increase effect
	config effect1;
	effect1["apply_to"] = "damage";
	effect1["increase_damage"] = 2;
	std::string desc1 = attack_type::describe_effect(effect1);
	BOOST_CHECK(!desc1.empty());

	// Test attacks increase effect
	config effect2;
	effect2["apply_to"] = "attack";
	effect2["increase_attacks"] = 1;
	std::string desc2 = attack_type::describe_effect(effect2);
	BOOST_CHECK(!desc2.empty());
}

/**
 * Test attack_type changed flag
 */
BOOST_AUTO_TEST_CASE( test_attack_type_changed_flag )
{
	config attack_cfg;
	attack_cfg["name"] = "staff";
	attack_cfg["type"] = "impact";
	attack_cfg["range"] = "melee";
	attack_cfg["damage"] = 5;
	attack_cfg["number"] = 3;

	auto attack = std::make_shared<attack_type>(attack_cfg);

	// Initially changed is true after construction
	BOOST_CHECK(attack->get_changed());

	// Reset changed flag
	attack->set_changed(false);
	BOOST_CHECK(!attack->get_changed());

	// After modification
	attack->set_damage(6);
	BOOST_CHECK(attack->get_changed());

	// Reset changed flag again
	attack->set_changed(false);
	BOOST_CHECK(!attack->get_changed());
}

/**
 * Test attack_type write method
 */
BOOST_AUTO_TEST_CASE( test_attack_type_write )
{
	config attack_cfg;
	attack_cfg["name"] = "crossbow";
	attack_cfg["type"] = "pierce";
	attack_cfg["range"] = "ranged";
	attack_cfg["damage"] = 8;
	attack_cfg["number"] = 3;

	auto attack = std::make_shared<attack_type>(attack_cfg);

	// Modify some values
	attack->set_damage(10);
	attack->set_num_attacks(4);

	// Write to config
	config written;
	attack->write(written);

	// Verify written values
	BOOST_CHECK_EQUAL(written["name"].str(), "crossbow");
	BOOST_CHECK_EQUAL(written["type"].str(), "pierce");
	BOOST_CHECK_EQUAL(written["range"].str(), "ranged");
	BOOST_CHECK_EQUAL(written["damage"].to_int(), 10);
	BOOST_CHECK_EQUAL(written["number"].to_int(), 4);
}

/**
 * Test unit_race basic construction
 */
BOOST_AUTO_TEST_CASE( test_unit_race_construction )
{
	config race_cfg;
	race_cfg["id"] = "elf";
	race_cfg["name"] = "Elf";
	race_cfg["plural_name"] = "Elves";
	race_cfg["description"] = "A graceful and long-lived race";

	unit_race race(race_cfg);

	BOOST_CHECK_EQUAL(race.id(), "elf");
	BOOST_CHECK_EQUAL(race.plural_name(), "Elves");
	BOOST_CHECK_EQUAL(race.description(), "A graceful and long-lived race");
}

/**
 * Test unit_race with gender-specific names
 */
BOOST_AUTO_TEST_CASE( test_unit_race_gender_names )
{
	config race_cfg;
	race_cfg["id"] = "human";
	race_cfg["male_name"] = "Human";
	race_cfg["female_name"] = "Human";
	race_cfg["plural_name"] = "Humans";

	unit_race race(race_cfg);

	BOOST_CHECK_EQUAL(race.name(unit_race::MALE), "Human");
	BOOST_CHECK_EQUAL(race.name(unit_race::FEMALE), "Human");
}

/**
 * Test unit_race num_traits
 */
BOOST_AUTO_TEST_CASE( test_unit_race_num_traits )
{
	config race_cfg;
	race_cfg["id"] = "orc";
	race_cfg["name"] = "Orc";
	race_cfg["num_traits"] = 2;

	unit_race race(race_cfg);

	BOOST_CHECK_EQUAL(race.num_traits(), 2);
}

/**
 * Test unit_race undead_variation
 */
BOOST_AUTO_TEST_CASE( test_unit_race_undead_variation )
{
	config race_cfg;
	race_cfg["id"] = "dwarf";
	race_cfg["name"] = "Dwarf";
	race_cfg["undead_variation"] = "dwarf";

	unit_race race(race_cfg);

	BOOST_CHECK_EQUAL(race.undead_variation(), "dwarf");
}

/**
 * Test unit_alignments values
 */
BOOST_AUTO_TEST_CASE( test_unit_alignments_values )
{
	// Test alignment enum values
	BOOST_CHECK_EQUAL(unit_alignments::get_string(unit_alignments::type::lawful), "lawful");
	BOOST_CHECK_EQUAL(unit_alignments::get_string(unit_alignments::type::neutral), "neutral");
	BOOST_CHECK_EQUAL(unit_alignments::get_string(unit_alignments::type::chaotic), "chaotic");
	BOOST_CHECK_EQUAL(unit_alignments::get_string(unit_alignments::type::liminal), "liminal");

	// Test string to enum conversion
	auto lawful_opt = unit_alignments::get_enum("lawful");
	BOOST_CHECK(lawful_opt.has_value());
	BOOST_CHECK(lawful_opt.value() == unit_alignments::type::lawful);

	auto neutral_opt = unit_alignments::get_enum("neutral");
	BOOST_CHECK(neutral_opt.has_value());
	BOOST_CHECK(neutral_opt.value() == unit_alignments::type::neutral);

	auto chaotic_opt = unit_alignments::get_enum("chaotic");
	BOOST_CHECK(chaotic_opt.has_value());
	BOOST_CHECK(chaotic_opt.value() == unit_alignments::type::chaotic);

	auto liminal_opt = unit_alignments::get_enum("liminal");
	BOOST_CHECK(liminal_opt.has_value());
	BOOST_CHECK(liminal_opt.value() == unit_alignments::type::liminal);

	// Test invalid enum conversion
	auto invalid_opt = unit_alignments::get_enum("invalid");
	BOOST_CHECK(!invalid_opt.has_value());
}

/**
 * Test unit_filter with various criteria
 */
BOOST_AUTO_TEST_CASE( test_unit_filter_complex )
{
	config filter_cfg;
	filter_cfg["type"] = "Cavalry";
	filter_cfg["side"] = 1;
	filter_cfg["level"] = 2;
	filter_cfg["canrecruit"] = true;

	vconfig vcfg(filter_cfg);
	unit_filter filter(vcfg);

	BOOST_CHECK(!filter.empty());

	config retrieved = filter.to_config();
	BOOST_CHECK_EQUAL(retrieved["type"].str(), "Cavalry");
	BOOST_CHECK_EQUAL(retrieved["side"].to_int(), 1);
	BOOST_CHECK_EQUAL(retrieved["level"].to_int(), 2);
	BOOST_CHECK_EQUAL(retrieved["canrecruit"].to_bool(), true);
}

/**
 * Test unit_filter with empty config
 */
BOOST_AUTO_TEST_CASE( test_unit_filter_empty_config )
{
	config empty_cfg;
	vconfig empty_vcfg(empty_cfg);
	unit_filter empty_filter(empty_vcfg);

	BOOST_CHECK(empty_filter.empty());
}

/**
 * Test attack_type specials count
 */
BOOST_AUTO_TEST_CASE( test_attack_type_specials_count )
{
	config attack_cfg;
	attack_cfg["name"] = "magic missile";
	attack_cfg["type"] = "arcane";
	attack_cfg["range"] = "ranged";
	attack_cfg["damage"] = 7;
	attack_cfg["number"] = 4;

	// Add multiple specials
	config& specials = attack_cfg.add_child("specials");
	config& special1 = specials.add_child("dummy");
	special1["id"] = "special1";
	special1["name"] = "Special 1";

	config& special2 = specials.add_child("dummy");
	special2["id"] = "special2";
	special2["name"] = "Special 2";

	auto attack = std::make_shared<attack_type>(attack_cfg);

	BOOST_CHECK_EQUAL(attack->specials().size(), 2);
}

/**
 * Test attack_type write with description
 */
BOOST_AUTO_TEST_CASE( test_attack_type_write_with_config )
{
	config attack_cfg;
	attack_cfg["name"] = "fireball";
	attack_cfg["type"] = "fire";
	attack_cfg["range"] = "ranged";
	attack_cfg["damage"] = 14;
	attack_cfg["number"] = 2;

	auto attack = std::make_shared<attack_type>(attack_cfg);

	// Verify basic attributes
	BOOST_CHECK_EQUAL(attack->id(), "fireball");
	BOOST_CHECK_EQUAL(attack->type(), "fire");
	BOOST_CHECK_EQUAL(attack->damage(), 14);
	BOOST_CHECK_EQUAL(attack->num_attacks(), 2);
}

/**
 * Test attack_type icon setter
 */
BOOST_AUTO_TEST_CASE( test_attack_type_icon_setter )
{
	config attack_cfg;
	attack_cfg["name"] = "sword";
	attack_cfg["type"] = "blade";
	attack_cfg["range"] = "melee";
	attack_cfg["damage"] = 8;
	attack_cfg["number"] = 3;

	auto attack = std::make_shared<attack_type>(attack_cfg);

	// Set icon
	attack->set_icon("attacks/longsword.png");
	BOOST_CHECK_EQUAL(attack->icon(), "attacks/longsword.png");
}

/**
 * Test attack_type with different ranges
 */
BOOST_AUTO_TEST_CASE( test_attack_type_different_ranges )
{
	// Test melee range
	config melee_cfg;
	melee_cfg["name"] = "dagger";
	melee_cfg["type"] = "blade";
	melee_cfg["range"] = "melee";
	melee_cfg["damage"] = 5;
	melee_cfg["number"] = 3;

	auto melee_attack = std::make_shared<attack_type>(melee_cfg);
	BOOST_CHECK_EQUAL(melee_attack->range(), "melee");

	// Test ranged attack
	config ranged_cfg;
	ranged_cfg["name"] = "bow";
	ranged_cfg["type"] = "pierce";
	ranged_cfg["range"] = "ranged";
	ranged_cfg["damage"] = 6;
	ranged_cfg["number"] = 3;

	auto ranged_attack = std::make_shared<attack_type>(ranged_cfg);
	BOOST_CHECK_EQUAL(ranged_attack->range(), "ranged");
}

/**
 * Test attack_type damage types
 */
BOOST_AUTO_TEST_CASE( test_attack_type_damage_types )
{
	std::vector<std::string> damage_types = {"blade", "pierce", "impact", "fire", "cold", "arcane"};

	for(const auto& type : damage_types) {
		config attack_cfg;
		attack_cfg["name"] = "test_" + type;
		attack_cfg["type"] = type;
		attack_cfg["range"] = "melee";
		attack_cfg["damage"] = 10;
		attack_cfg["number"] = 2;

		auto attack = std::make_shared<attack_type>(attack_cfg);
		BOOST_CHECK_EQUAL(attack->type(), type);
	}
}

/**
 * Test unit_race uses_global_traits
 */
BOOST_AUTO_TEST_CASE( test_unit_race_global_traits )
{
	// Test with global traits enabled (default)
	config race_cfg1;
	race_cfg1["id"] = "test1";
	race_cfg1["name"] = "Test Race 1";

	unit_race race1(race_cfg1);
	BOOST_CHECK(race1.uses_global_traits());

	// Test with global traits disabled
	config race_cfg2;
	race_cfg2["id"] = "test2";
	race_cfg2["name"] = "Test Race 2";
	race_cfg2["ignore_global_traits"] = true;

	unit_race race2(race_cfg2);
	BOOST_CHECK(!race2.uses_global_traits());
}

/**
 * Test attack_type with attack_weight edge cases
 */
BOOST_AUTO_TEST_CASE( test_attack_type_weight_edge_cases )
{
	config attack_cfg;
	attack_cfg["name"] = "test_attack";
	attack_cfg["type"] = "blade";
	attack_cfg["range"] = "melee";
	attack_cfg["damage"] = 5;
	attack_cfg["number"] = 2;
	attack_cfg["attack_weight"] = 0.0;
	attack_cfg["defense_weight"] = 1.0;

	auto attack = std::make_shared<attack_type>(attack_cfg);

	BOOST_CHECK_CLOSE(attack->attack_weight(), 0.0, 0.001);
	BOOST_CHECK_CLOSE(attack->defense_weight(), 1.0, 0.001);

	// Test setting weights
	attack->set_attack_weight(0.5);
	BOOST_CHECK_CLOSE(attack->attack_weight(), 0.5, 0.001);
}

/**
 * Test attack_type accuracy edge cases
 */
BOOST_AUTO_TEST_CASE( test_attack_type_accuracy_edge_cases )
{
	config attack_cfg;
	attack_cfg["name"] = "precise_shot";
	attack_cfg["type"] = "pierce";
	attack_cfg["range"] = "ranged";
	attack_cfg["damage"] = 10;
	attack_cfg["number"] = 1;
	attack_cfg["accuracy"] = 20;
	attack_cfg["parry"] = 5;

	auto attack = std::make_shared<attack_type>(attack_cfg);

	BOOST_CHECK_EQUAL(attack->accuracy(), 20);
	BOOST_CHECK_EQUAL(attack->parry(), 5);

	// Set accuracy to 0
	attack->set_accuracy(0);
	BOOST_CHECK_EQUAL(attack->accuracy(), 0);
}

/**
 * Test attack_type movement_used edge cases
 */
BOOST_AUTO_TEST_CASE( test_attack_type_movement_used_edge_cases )
{
	config attack_cfg;
	attack_cfg["name"] = "charge";
	attack_cfg["type"] = "impact";
	attack_cfg["range"] = "melee";
	attack_cfg["damage"] = 15;
	attack_cfg["number"] = 1;
	attack_cfg["movement_used"] = 100;  // Uses all movement

	auto attack = std::make_shared<attack_type>(attack_cfg);

	BOOST_CHECK_EQUAL(attack->movement_used(), 100);

	// Set to 0 (no movement used)
	attack->set_movement_used(0);
	BOOST_CHECK_EQUAL(attack->movement_used(), 0);
}

/**
 * Test attack_type with multiple attribute changes
 */
BOOST_AUTO_TEST_CASE( test_attack_type_multiple_changes )
{
	config attack_cfg;
	attack_cfg["name"] = "versatile_attack";
	attack_cfg["type"] = "blade";
	attack_cfg["range"] = "melee";
	attack_cfg["damage"] = 10;
	attack_cfg["number"] = 3;

	auto attack = std::make_shared<attack_type>(attack_cfg);

	// Make multiple changes
	attack->set_damage(12);
	attack->set_num_attacks(4);
	attack->set_type("pierce");
	attack->set_range("ranged");

	// Verify all changes
	BOOST_CHECK_EQUAL(attack->damage(), 12);
	BOOST_CHECK_EQUAL(attack->num_attacks(), 4);
	BOOST_CHECK_EQUAL(attack->type(), "pierce");
	BOOST_CHECK_EQUAL(attack->range(), "ranged");

	// Write and verify preservation
	config written;
	attack->write(written);
	BOOST_CHECK_EQUAL(written["damage"].to_int(), 12);
	BOOST_CHECK_EQUAL(written["number"].to_int(), 4);
	BOOST_CHECK_EQUAL(written["type"].str(), "pierce");
	BOOST_CHECK_EQUAL(written["range"].str(), "ranged");
}

/**
 * Test unit_race editor_icon
 */
BOOST_AUTO_TEST_CASE( test_unit_race_editor_icon )
{
	config race_cfg;
	race_cfg["id"] = "test_race";
	race_cfg["name"] = "Test Race";
	race_cfg["editor_icon"] = "icons/test-icon.png";

	unit_race race(race_cfg);

	BOOST_CHECK_EQUAL(race.editor_icon(), "icons/test-icon.png");
}

/**
 * Test unit_filter assignment
 */
BOOST_AUTO_TEST_CASE( test_unit_filter_assignment )
{
	config filter_cfg1;
	filter_cfg1["type"] = "Knight";
	vconfig vcfg1(filter_cfg1);
	unit_filter filter1(vcfg1);

	config filter_cfg2;
	filter_cfg2["type"] = "Archer";
	vconfig vcfg2(filter_cfg2);
	unit_filter filter2(vcfg2);

	// Assignment operator
	filter1 = filter2;
	BOOST_CHECK_EQUAL(filter1.to_config()["type"].str(), "Archer");
}

BOOST_AUTO_TEST_SUITE_END()
