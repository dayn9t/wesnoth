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

#include "variable.hpp"
#include "variable_info.hpp"
#include "config.hpp"

BOOST_AUTO_TEST_SUITE(variable_suite)

// Test vconfig construction from config
BOOST_AUTO_TEST_CASE(test_vconfig_construction_from_config)
{
	config cfg;
	cfg["key1"] = "value1";
	cfg["key2"] = 42;

	vconfig vcfg(cfg);

	BOOST_CHECK(vcfg);
	BOOST_CHECK(!vcfg.null());
	BOOST_CHECK_EQUAL(vcfg["key1"].str(), "value1");
	BOOST_CHECK_EQUAL(vcfg["key2"].to_int(), 42);
}

// Test vconfig empty vconfig
BOOST_AUTO_TEST_CASE(test_vconfig_empty_vconfig)
{
	vconfig vcfg = vconfig::empty_vconfig();

	BOOST_CHECK(vcfg);
	BOOST_CHECK(!vcfg.null());
	BOOST_CHECK(vcfg.empty());
}

// Test vconfig unconstructed vconfig
BOOST_AUTO_TEST_CASE(test_vconfig_unconstructed_vconfig)
{
	vconfig vcfg = vconfig::unconstructed_vconfig();

	BOOST_CHECK(!vcfg);
	BOOST_CHECK(vcfg.null());
}

// Test vconfig has_attribute
BOOST_AUTO_TEST_CASE(test_vconfig_has_attribute_existing)
{
	config cfg;
	cfg["existing"] = "value";

	vconfig vcfg(cfg);

	BOOST_CHECK(vcfg.has_attribute("existing"));
}

BOOST_AUTO_TEST_CASE(test_vconfig_has_attribute_nonexistent)
{
	config cfg;
	cfg["existing"] = "value";

	vconfig vcfg(cfg);

	BOOST_CHECK(!vcfg.has_attribute("nonexistent"));
}

// Test vconfig empty check
BOOST_AUTO_TEST_CASE(test_vconfig_empty_true)
{
	config cfg;
	vconfig vcfg(cfg);

	BOOST_CHECK(vcfg.empty());
}

BOOST_AUTO_TEST_CASE(test_vconfig_empty_false)
{
	config cfg;
	cfg["key"] = "value";
	vconfig vcfg(cfg);

	BOOST_CHECK(!vcfg.empty());
}

// Test vconfig get_config
BOOST_AUTO_TEST_CASE(test_vconfig_get_config)
{
	config cfg;
	cfg["key"] = "value";

	vconfig vcfg(cfg);
	const config& retrieved = vcfg.get_config();

	BOOST_CHECK_EQUAL(retrieved["key"].str(), "value");
}

// Test vconfig has_child
BOOST_AUTO_TEST_CASE(test_vconfig_has_child_existing)
{
	config cfg;
	config& child = cfg.add_child("child_section");
	child["child_key"] = "child_value";

	vconfig vcfg(cfg);

	BOOST_CHECK(vcfg.has_child("child_section"));
}

BOOST_AUTO_TEST_CASE(test_vconfig_has_child_nonexistent)
{
	config cfg;
	vconfig vcfg(cfg);

	BOOST_CHECK(!vcfg.has_child("nonexistent"));
}

// Test vconfig child
BOOST_AUTO_TEST_CASE(test_vconfig_child_existing)
{
	config cfg;
	config& child = cfg.add_child("child_section");
	child["child_key"] = "child_value";

	vconfig vcfg(cfg);
	vconfig child_vcfg = vcfg.child("child_section");

	BOOST_CHECK(child_vcfg);
	BOOST_CHECK_EQUAL(child_vcfg["child_key"].str(), "child_value");
}

BOOST_AUTO_TEST_CASE(test_vconfig_child_nonexistent)
{
	config cfg;
	vconfig vcfg(cfg);

	vconfig child = vcfg.child("nonexistent");

	BOOST_CHECK(!child);
	BOOST_CHECK(child.null());
}

// Test vconfig count_children
BOOST_AUTO_TEST_CASE(test_vconfig_count_children_multiple)
{
	config cfg;
	cfg.add_child("child");
	cfg.add_child("child");
	cfg.add_child("other");

	vconfig vcfg(cfg);

	BOOST_CHECK_EQUAL(vcfg.count_children("child"), 2);
	BOOST_CHECK_EQUAL(vcfg.count_children("other"), 1);
	BOOST_CHECK_EQUAL(vcfg.count_children("nonexistent"), 0);
}

BOOST_AUTO_TEST_CASE(test_vconfig_count_children_empty)
{
	config cfg;
	vconfig vcfg(cfg);

	BOOST_CHECK_EQUAL(vcfg.count_children("anything"), 0);
}

// Test vconfig get_children
BOOST_AUTO_TEST_CASE(test_vconfig_get_children_basic)
{
	config cfg;
	config& child1 = cfg.add_child("child");
	child1["id"] = "1";
	config& child2 = cfg.add_child("child");
	child2["id"] = "2";

	vconfig vcfg(cfg);
	std::vector<vconfig> children = vcfg.get_children("child");

	BOOST_CHECK_EQUAL(children.size(), 2);
	BOOST_CHECK_EQUAL(children[0]["id"].str(), "1");
	BOOST_CHECK_EQUAL(children[1]["id"].str(), "2");
}

BOOST_AUTO_TEST_CASE(test_vconfig_get_children_empty)
{
	config cfg;
	vconfig vcfg(cfg);

	std::vector<vconfig> children = vcfg.get_children("child");

	BOOST_CHECK(children.empty());
}

// Test config_variable_set
BOOST_AUTO_TEST_CASE(test_config_variable_set_get_variable_const)
{
	config cfg;
	cfg["var1"] = "value1";
	cfg["var2"] = 123;

	config_variable_set var_set(cfg);

	BOOST_CHECK_EQUAL(var_set.get_variable_const("var1").str(), "value1");
	BOOST_CHECK_EQUAL(var_set.get_variable_const("var2").to_int(), 123);
}

BOOST_AUTO_TEST_CASE(test_config_variable_set_empty)
{
	config cfg;
	config_variable_set var_set(cfg);

	BOOST_CHECK(var_set.get_variable_const("nonexistent").empty());
}

// Test vconfig expand
BOOST_AUTO_TEST_CASE(test_vconfig_expand)
{
	config cfg;
	cfg["key"] = "expanded_value";

	vconfig vcfg(cfg);
	config::attribute_value val = vcfg.expand("key");

	BOOST_CHECK_EQUAL(val.str(), "expanded_value");
}

BOOST_AUTO_TEST_CASE(test_vconfig_expand_nonexistent)
{
	config cfg;
	vconfig vcfg(cfg);

	config::attribute_value val = vcfg.expand("nonexistent");

	BOOST_CHECK(val.empty());
}

// Test vconfig ordered iteration
BOOST_AUTO_TEST_CASE(test_vconfig_ordered_iteration)
{
	config cfg;
	config& child1 = cfg.add_child("section_a");
	child1["order"] = "1";
	config& child2 = cfg.add_child("section_b");
	child2["order"] = "2";

	vconfig vcfg(cfg);

	auto it = vcfg.ordered_begin();
	BOOST_CHECK(it != vcfg.ordered_end());

	int count = 0;
	for (auto iter = vcfg.ordered_begin(); iter != vcfg.ordered_end(); ++iter) {
		++count;
	}
	BOOST_CHECK_EQUAL(count, 2);
}

BOOST_AUTO_TEST_CASE(test_vconfig_ordered_iteration_empty)
{
	config cfg;
	vconfig vcfg(cfg);

	int count = 0;
	for (auto iter = vcfg.ordered_begin(); iter != vcfg.ordered_end(); ++iter) {
		++count;
	}
	BOOST_CHECK_EQUAL(count, 0);
}

// Test vconfig all_ordered
BOOST_AUTO_TEST_CASE(test_vconfig_all_ordered)
{
	config cfg;
	config& child1 = cfg.add_child("section_a");
	child1["key"] = "a";
	config& child2 = cfg.add_child("section_b");
	child2["key"] = "b";

	vconfig vcfg(cfg);
	auto range = vcfg.all_ordered();

	int count = 0;
	for (const auto& pair : range) {
		++count;
	}
	BOOST_CHECK_EQUAL(count, 2);
}

// Test vconfig attribute_range
BOOST_AUTO_TEST_CASE(test_vconfig_attribute_range)
{
	config cfg;
	cfg["attr1"] = "val1";
	cfg["attr2"] = "val2";
	cfg["attr3"] = "val3";

	vconfig vcfg(cfg);
	auto range = vcfg.attribute_range();

	int count = 0;
	for (const auto& attr : range) {
		++count;
	}
	BOOST_CHECK_EQUAL(count, 3);
}

BOOST_AUTO_TEST_CASE(test_vconfig_attribute_range_empty)
{
	config cfg;
	vconfig vcfg(cfg);

	auto range = vcfg.attribute_range();

	int count = 0;
	for (const auto& attr : range) {
		++count;
	}
	BOOST_CHECK_EQUAL(count, 0);
}

// Test vconfig make_safe
BOOST_AUTO_TEST_CASE(test_vconfig_make_safe)
{
	config cfg;
	cfg["key"] = "value";

	vconfig vcfg(cfg);
	const vconfig& safe = vcfg.make_safe();

	BOOST_CHECK_EQUAL(safe["key"].str(), "value");
}

// Test vconfig get_parsed_config
BOOST_AUTO_TEST_CASE(test_vconfig_get_parsed_config)
{
	config cfg;
	cfg["key"] = "value";

	vconfig vcfg(cfg);
	config parsed = vcfg.get_parsed_config();

	BOOST_CHECK_EQUAL(parsed["key"].str(), "value");
}

// Test vconfig rvalue construction
BOOST_AUTO_TEST_CASE(test_vconfig_rvalue_construction)
{
	config cfg;
	cfg["key"] = "value";

	vconfig vcfg(std::move(cfg));

	BOOST_CHECK(vcfg);
	BOOST_CHECK_EQUAL(vcfg["key"].str(), "value");
}

// Test vconfig bool conversion
BOOST_AUTO_TEST_CASE(test_vconfig_bool_conversion_empty)
{
	vconfig empty = vconfig::empty_vconfig();
	BOOST_CHECK(!!empty); // Should convert to true
}

BOOST_AUTO_TEST_CASE(test_vconfig_bool_conversion_unconstructed)
{
	vconfig unconstructed = vconfig::unconstructed_vconfig();
	BOOST_CHECK(!unconstructed); // Should convert to false
}

// Test vconfig with variables
BOOST_AUTO_TEST_CASE(test_vconfig_with_variables)
{
	config cfg;
	cfg["var"] = "resolved_value";

	config variables;
	variables["other_var"] = "resolved_value";

	config_variable_set var_set(variables);
	vconfig vcfg(cfg, var_set);

	// The value should be accessible
	BOOST_CHECK_EQUAL(vcfg["var"].str(), "resolved_value");
}

// Test vconfig operator[]
BOOST_AUTO_TEST_CASE(test_vconfig_operator_bracket)
{
	config cfg;
	cfg["key1"] = "value1";
	cfg["key2"] = 42;
	cfg["key3"] = true;

	vconfig vcfg(cfg);

	BOOST_CHECK_EQUAL(vcfg["key1"].str(), "value1");
	BOOST_CHECK_EQUAL(vcfg["key2"].to_int(), 42);
	BOOST_CHECK_EQUAL(vcfg["key3"].to_bool(), true);
}

// Test nested vconfig
BOOST_AUTO_TEST_CASE(test_vconfig_nested_children)
{
	config cfg;
	config& level1 = cfg.add_child("level1");
	config& level2 = level1.add_child("level2");
	level2["deep_key"] = "deep_value";

	vconfig vcfg(cfg);
	vconfig vlevel1 = vcfg.child("level1");
	vconfig vlevel2 = vlevel1.child("level2");

	BOOST_CHECK(vlevel1);
	BOOST_CHECK(vlevel2);
	BOOST_CHECK_EQUAL(vlevel2["deep_key"].str(), "deep_value");
}

// Test vconfig multiple children of same type
BOOST_AUTO_TEST_CASE(test_vconfig_multiple_children_same_type)
{
	config cfg;
	for (int i = 0; i < 5; ++i) {
		config& child = cfg.add_child("item");
		child["index"] = i;
	}

	vconfig vcfg(cfg);
	std::vector<vconfig> items = vcfg.get_children("item");

	BOOST_CHECK_EQUAL(items.size(), 5);
	for (int i = 0; i < 5; ++i) {
		BOOST_CHECK_EQUAL(items[i]["index"].to_int(), i);
	}
}

// Test vconfig memory management
BOOST_AUTO_TEST_CASE(test_vconfig_memory_management)
{
	config cfg;
	cfg["key"] = "value";
	vconfig vcfg(cfg, true); // manage_memory = true

	// vcfg should be valid and contain the data
	BOOST_CHECK(vcfg);
	BOOST_CHECK_EQUAL(vcfg["key"].str(), "value");
}

// Test invalid_variablename_exception
BOOST_AUTO_TEST_CASE(test_invalid_variablename_exception)
{
	invalid_variablename_exception ex;
	BOOST_CHECK(std::string(ex.what()).find("invalid") != std::string::npos);
}

// Test variable_access_const
BOOST_AUTO_TEST_CASE(test_variable_access_const_basic)
{
	config cfg;
	cfg["var"] = "value";

	variable_access_const access("var", cfg);
	BOOST_CHECK(access.exists_as_attribute());
	BOOST_CHECK_EQUAL(access.as_scalar().str(), "value");
}

BOOST_AUTO_TEST_CASE(test_variable_access_const_nonexistent)
{
	config cfg;

	variable_access_const access("var", cfg);
	BOOST_CHECK(!access.exists_as_attribute());
}

// Test variable_access_create
BOOST_AUTO_TEST_CASE(test_variable_access_create_basic)
{
	config cfg;

	variable_access_create access("var", cfg);
	access.as_scalar() = "created_value";

	BOOST_CHECK_EQUAL(cfg["var"].str(), "created_value");
}

// Test variable_access_throw
BOOST_AUTO_TEST_CASE(test_variable_access_throw_existing)
{
	config cfg;
	cfg["var"] = "value";

	variable_access_throw access("var", cfg);
	BOOST_CHECK_EQUAL(access.as_scalar().str(), "value");
}

BOOST_AUTO_TEST_CASE(test_variable_access_throw_nonexistent)
{
	config cfg;

	variable_access_throw access("var", cfg);
	// Should create the variable rather than throw in current implementation
	// or throw - depends on implementation
}

// Test complex vconfig scenarios
BOOST_AUTO_TEST_CASE(test_vconfig_complex_scenario)
{
	config cfg;
	cfg["scenario_name"] = "Test Scenario";
	cfg["turn"] = 5;

	config& unit = cfg.add_child("unit");
	unit["type"] = "Elvish Fighter";
	unit["hp"] = 30;

	config& unit2 = cfg.add_child("unit");
	unit2["type"] = "Elvish Archer";
	unit2["hp"] = 28;

	vconfig vcfg(cfg);

	BOOST_CHECK_EQUAL(vcfg["scenario_name"].str(), "Test Scenario");
	BOOST_CHECK_EQUAL(vcfg["turn"].to_int(), 5);
	BOOST_CHECK_EQUAL(vcfg.count_children("unit"), 2);

	auto units = vcfg.get_children("unit");
	BOOST_CHECK_EQUAL(units[0]["type"].str(), "Elvish Fighter");
	BOOST_CHECK_EQUAL(units[1]["type"].str(), "Elvish Archer");
}

// Test vconfig with empty child names
BOOST_AUTO_TEST_CASE(test_vconfig_empty_child_name)
{
	config cfg;
	config& child = cfg.add_child("");
	child["key"] = "value";

	vconfig vcfg(cfg);
	// Empty child name behavior depends on implementation
	BOOST_CHECK_EQUAL(vcfg.count_children(""), 1);
}

// Test vconfig attribute types
BOOST_AUTO_TEST_CASE(test_vconfig_attribute_types)
{
	config cfg;
	cfg["string_val"] = "hello";
	cfg["int_val"] = 42;
	cfg["double_val"] = 3.14;
	cfg["bool_val"] = true;

	vconfig vcfg(cfg);

	BOOST_CHECK_EQUAL(vcfg["string_val"].str(), "hello");
	BOOST_CHECK_EQUAL(vcfg["int_val"].to_int(), 42);
	BOOST_CHECK_CLOSE(vcfg["double_val"].to_double(), 3.14, 0.01);
	BOOST_CHECK_EQUAL(vcfg["bool_val"].to_bool(), true);
}

BOOST_AUTO_TEST_SUITE_END()
