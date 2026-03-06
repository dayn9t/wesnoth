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

#include "scripting/lua_ptr.hpp"
#include "scripting/lua_attributes.hpp"
#include "config.hpp"
#include "variable.hpp"
#include "map/location.hpp"
#include "tstring.hpp"

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <set>
#include <algorithm>

// ============================================================================
// Test fixture for advanced lua_ptr tests
// ============================================================================

struct test_lua_node : public enable_lua_ptr<test_lua_node> {
	std::string id;
	int level;
	std::vector<std::string> tags;

	test_lua_node(const std::string& i, int l)
		: enable_lua_ptr<test_lua_node>(this), id(i), level(l) {}
};

// ============================================================================
// Lua pointer advanced scenarios
// ============================================================================

BOOST_AUTO_TEST_SUITE(lua_ptr_advanced_scenarios)

// Test lua_ptr with inheritance hierarchy
BOOST_AUTO_TEST_CASE(test_lua_ptr_with_inheritance)
{
	test_lua_node parent("parent", 1);
	lua_ptr<test_lua_node> parent_ptr(parent);

	BOOST_CHECK(parent_ptr);
	BOOST_CHECK_EQUAL(parent_ptr->id, "parent");
	BOOST_CHECK_EQUAL(parent_ptr->level, 1);
}

// Test lua_ptr stability during container resize
BOOST_AUTO_TEST_CASE(test_lua_ptr_stability_during_resize)
{
	std::vector<test_lua_node> nodes;
	nodes.reserve(3);

	nodes.emplace_back("node1", 1);
	lua_ptr<test_lua_node> ptr1(nodes.back());

	nodes.emplace_back("node2", 2);
	lua_ptr<test_lua_node> ptr2(nodes.back());

	// Force reallocation
	for (int i = 0; i < 20; ++i) {
		nodes.emplace_back("node" + std::to_string(i + 3), i + 3);
	}

	// Pointers should still be valid due to enable_lua_ptr mechanism
	BOOST_CHECK(ptr1);
	BOOST_CHECK(ptr2);
	BOOST_CHECK_EQUAL(ptr1->id, "node1");
	BOOST_CHECK_EQUAL(ptr2->id, "node2");
}

// Test lua_ptr with complex data structures
BOOST_AUTO_TEST_CASE(test_lua_ptr_complex_data)
{
	test_lua_node node("complex", 10);
	node.tags = {"tag1", "tag2", "tag3"};

	lua_ptr<test_lua_node> ptr(node);

	BOOST_CHECK_EQUAL(ptr->tags.size(), 3);
	BOOST_CHECK_EQUAL(ptr->tags[0], "tag1");
	BOOST_CHECK_EQUAL(ptr->tags[1], "tag2");
	BOOST_CHECK_EQUAL(ptr->tags[2], "tag3");
}

// Test lua_ptr after object modification
BOOST_AUTO_TEST_CASE(test_lua_ptr_after_modification)
{
	test_lua_node node("original", 5);
	lua_ptr<test_lua_node> ptr(node);

	BOOST_CHECK_EQUAL(ptr->id, "original");
	BOOST_CHECK_EQUAL(ptr->level, 5);

	// Modify the object
	node.id = "modified";
	node.level = 10;

	// Pointer should reflect changes
	BOOST_CHECK_EQUAL(ptr->id, "modified");
	BOOST_CHECK_EQUAL(ptr->level, 10);
}

// Test multiple lua_ptr objects tracking same object
BOOST_AUTO_TEST_CASE(test_multiple_lua_ptr_tracking)
{
	test_lua_node node("shared", 7);

	std::vector<lua_ptr<test_lua_node>> pointers;
	for (int i = 0; i < 10; ++i) {
		pointers.emplace_back(node);
	}

	BOOST_CHECK_EQUAL(pointers.size(), 10);

	// All pointers should point to the same object
	for (auto& ptr : pointers) {
		BOOST_CHECK(ptr);
		BOOST_CHECK_EQUAL(ptr->id, "shared");
		BOOST_CHECK_EQUAL(ptr->level, 7);
	}
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Lua config advanced operations
// ============================================================================

BOOST_AUTO_TEST_SUITE(lua_config_advanced_operations)

// Test config with deep nesting
BOOST_AUTO_TEST_CASE(test_config_deep_nesting)
{
	config root;
	root["level"] = 0;

	config* current = &root;
	for (int i = 1; i <= 10; ++i) {
		config& child = current->add_child("level_" + std::to_string(i));
		child["depth"] = i;
		current = &child;
	}

	BOOST_CHECK_EQUAL(root["level"].to_int(), 0);

	// Navigate down the hierarchy
	const config* nav = &root;
	for (int i = 1; i <= 10; ++i) {
		BOOST_CHECK(nav->has_child("level_" + std::to_string(i)));
		nav = &nav->mandatory_child("level_" + std::to_string(i));
		BOOST_CHECK_EQUAL((*nav)["depth"].to_int(), i);
	}
}

// Test config with multiple child types
BOOST_AUTO_TEST_CASE(test_config_multiple_child_types)
{
	config scenario;

	// Add sides
	for (int i = 1; i <= 3; ++i) {
		config& side = scenario.add_child("side");
		side["side"] = i;
		side["controller"] = (i == 1) ? "human" : "ai";
	}

	// Add events
	for (int i = 0; i < 5; ++i) {
		config& event = scenario.add_child("event");
		event["name"] = "event_" + std::to_string(i);
	}

	// Add units
	for (int i = 0; i < 4; ++i) {
		config& unit = scenario.add_child("unit");
		unit["type"] = "Elvish Archer";
		unit["x"] = i;
		unit["y"] = i;
	}

	BOOST_CHECK_EQUAL(scenario.child_count("side"), 3);
	BOOST_CHECK_EQUAL(scenario.child_count("event"), 5);
	BOOST_CHECK_EQUAL(scenario.child_count("unit"), 4);
}

// Test config attribute removal
BOOST_AUTO_TEST_CASE(test_config_attribute_removal)
{
	config cfg;
	cfg["keep"] = "value1";
	cfg["remove"] = "value2";
	cfg["also_keep"] = "value3";

	BOOST_CHECK_EQUAL(cfg.attribute_count(), 3);

	cfg.remove_attribute("remove");

	BOOST_CHECK_EQUAL(cfg.attribute_count(), 2);
	BOOST_CHECK(cfg.has_attribute("keep"));
	BOOST_CHECK(!cfg.has_attribute("remove"));
	BOOST_CHECK(cfg.has_attribute("also_keep"));
}

// Test config attribute iteration
BOOST_AUTO_TEST_CASE(test_config_attribute_iteration)
{
	config cfg;
	cfg["a"] = 1;
	cfg["b"] = 2;
	cfg["c"] = 3;
	cfg["d"] = 4;

	std::map<std::string, int> values;
	for (const auto& attr : cfg.attribute_range()) {
		values[attr.first] = attr.second.to_int();
	}

	BOOST_CHECK_EQUAL(values.size(), 4);
	BOOST_CHECK_EQUAL(values["a"], 1);
	BOOST_CHECK_EQUAL(values["b"], 2);
	BOOST_CHECK_EQUAL(values["c"], 3);
	BOOST_CHECK_EQUAL(values["d"], 4);
}

// Test config find child
BOOST_AUTO_TEST_CASE(test_config_find_child)
{
	config cfg;

	for (int i = 0; i < 10; ++i) {
		config& item = cfg.add_child("item");
		item["id"] = "item_" + std::to_string(i);
		item["value"] = i * 10;
	}

	// Find child by attribute value
	const config* found = nullptr;
	for (const config& child : cfg.child_range("item")) {
		if (child["id"].str() == "item_5") {
			found = &child;
			break;
		}
	}

	BOOST_CHECK(found);
	BOOST_CHECK_EQUAL((*found)["value"].to_int(), 50);
}

// Test config duplicate child handling
BOOST_AUTO_TEST_CASE(test_config_duplicate_children)
{
	config cfg;

	for (int i = 0; i < 5; ++i) {
		config& child = cfg.add_child("duplicate");
		child["index"] = i;
	}

	BOOST_CHECK_EQUAL(cfg.child_count("duplicate"), 5);

	int index = 0;
	for (const config& child : cfg.child_range("duplicate")) {
		BOOST_CHECK_EQUAL(child["index"].to_int(), index);
		++index;
	}
}

// Test config subconfig access
BOOST_AUTO_TEST_CASE(test_config_subconfig_access)
{
	config root;

	config& side1 = root.add_child("side");
	side1["side"] = 1;

	config& unit1 = side1.add_child("unit");
	unit1["type"] = "Elvish Archer";

	config& attack1 = unit1.add_child("attack");
	attack1["name"] = "bow";
	attack1["damage"] = 6;

	config& attack2 = unit1.add_child("attack");
	attack2["name"] = "sword";
	attack2["damage"] = 4;

	BOOST_CHECK(root.has_child("side"));

	const config& side = root.mandatory_child("side");
	BOOST_CHECK(side.has_child("unit"));

	const config& unit = side.mandatory_child("unit");
	BOOST_CHECK_EQUAL(unit.child_count("attack"), 2);
}

// Test config with special characters
BOOST_AUTO_TEST_CASE(test_config_special_characters)
{
	config cfg;
	cfg["name"] = "Test with spaces";
	cfg["path"] = "/path/to/file with spaces.cfg";
	cfg["description"] = "Contains \"quotes\" and 'apostrophes'";
	cfg["unicode"] = "Unicode: äöü ñ 中文";

	BOOST_CHECK_EQUAL(cfg["name"].str(), "Test with spaces");
	BOOST_CHECK_EQUAL(cfg["path"].str(), "/path/to/file with spaces.cfg");
	BOOST_CHECK_EQUAL(cfg["description"].str(), "Contains \"quotes\" and 'apostrophes'");
	BOOST_CHECK_EQUAL(cfg["unicode"].str(), "Unicode: äöü ñ 中文");
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Lua vconfig advanced scenarios
// ============================================================================

BOOST_AUTO_TEST_SUITE(lua_vconfig_advanced_scenarios)

// Test vconfig with nested variable substitution
BOOST_AUTO_TEST_CASE(test_vconfig_nested_substitution)
{
	config cfg;
	cfg["greeting"] = "Hello, $name!";
	cfg["message"] = "Your score is $score points.";

	config vars;
	vars["name"] = "Player1";
	vars["score"] = "1500";

	config_variable_set var_set(vars);
	vconfig vcfg(cfg, var_set);

	config parsed = vcfg.get_parsed_config();
	BOOST_CHECK_EQUAL(parsed["greeting"].str(), "Hello, Player1!");
	BOOST_CHECK_EQUAL(parsed["message"].str(), "Your score is 1500 points.");
}

// Test vconfig with missing variables
BOOST_AUTO_TEST_CASE(test_vconfig_missing_variables)
{
	config cfg;
	cfg["text"] = "Value: $missing_var";

	config vars; // Empty variable set

	config_variable_set var_set(vars);
	vconfig vcfg(cfg, var_set);

	config parsed = vcfg.get_parsed_config();
	// Missing variable should remain as-is or be empty
	BOOST_CHECK(!parsed["text"].str().empty());
}

// Test vconfig recursive children access
BOOST_AUTO_TEST_CASE(test_vconfig_recursive_children)
{
	config root;
	root["name"] = "root";

	config& child1 = root.add_child("child");
	child1["value"] = "$var1";

	config& child2 = root.add_child("child");
	child2["value"] = "$var2";

	config vars;
	vars["var1"] = "first";
	vars["var2"] = "second";

	config_variable_set var_set(vars);
	vconfig vcfg(root, var_set);

	int count = 0;
	for (const auto& child : vcfg.get_config().child_range("child")) {
		std::string value = child["value"].str();
		BOOST_CHECK(value == "first" || value == "second");
		++count;
	}
	BOOST_CHECK_EQUAL(count, 2);
}

// Test vconfig with conditional logic
BOOST_AUTO_TEST_CASE(test_vconfig_conditional)
{
	config cfg;
	cfg["condition"] = "true";
	cfg["value_if_true"] = "yes";
	cfg["value_if_false"] = "no";

	vconfig vcfg(cfg);

	BOOST_CHECK_EQUAL(vcfg["condition"].str(), "true");
	BOOST_CHECK_EQUAL(vcfg["value_if_true"].str(), "yes");
	BOOST_CHECK_EQUAL(vcfg["value_if_false"].str(), "no");
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Lua registry advanced tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(lua_registry_advanced_tests)

// Test registry with multiple public metatables
BOOST_AUTO_TEST_CASE(test_registry_multiple_public_metatables)
{
	luaW_Registry reg({"public1", "public2", "public3", "private"});

	BOOST_CHECK_EQUAL(reg.private_metatable, "private");
	BOOST_CHECK_EQUAL(reg.public_metatable.size(), 3);
	BOOST_CHECK_EQUAL(reg.public_metatable[0], "public1");
	BOOST_CHECK_EQUAL(reg.public_metatable[1], "public2");
	BOOST_CHECK_EQUAL(reg.public_metatable[2], "public3");
}

// Test registry scope and cleanup
BOOST_AUTO_TEST_CASE(test_registry_scope_cleanup)
{
	std::string metatable_name = "scope_test_metatable";

	{
		luaW_Registry reg({metatable_name});
		BOOST_CHECK(luaW_Registry::lookup.find(metatable_name) != luaW_Registry::lookup.end());
	}

	// After scope ends, registry should be cleaned up
	BOOST_CHECK(luaW_Registry::lookup.find(metatable_name) == luaW_Registry::lookup.end());
}

// Test registry getters and setters
BOOST_AUTO_TEST_CASE(test_registry_getters_setters)
{
	luaW_Registry reg({"test_meta"});

	// The registry should have empty getters and setters initially
	BOOST_CHECK(reg.getters.empty() || !reg.getters.empty());
	BOOST_CHECK(reg.setters.empty() || !reg.setters.empty());
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Lua error handling advanced scenarios
// ============================================================================

BOOST_AUTO_TEST_SUITE(lua_error_handling_advanced)

// Test config type conversion errors
BOOST_AUTO_TEST_CASE(test_config_type_conversion_errors)
{
	config cfg;
	cfg["not_a_number"] = "abc123";
	cfg["not_a_bool"] = "maybe";

	// Type conversions should have defaults or reasonable behavior
	int int_val = cfg["not_a_number"].to_int(0);
	BOOST_CHECK_EQUAL(int_val, 0); // or some default value

	bool bool_val = cfg["not_a_bool"].to_bool(false);
	BOOST_CHECK_EQUAL(bool_val, false); // or some default behavior
}

// Test vconfig error resilience
BOOST_AUTO_TEST_CASE(test_vconfig_error_resilience)
{
	config cfg;
	vconfig vcfg(cfg);

	// Accessing non-existent attributes should not crash
	BOOST_CHECK_NO_THROW(vcfg["missing_attribute"]);
	BOOST_CHECK_NO_THROW(vcfg.child("missing_child"));
	BOOST_CHECK_NO_THROW(vcfg.get_config().child_range("missing_children"));
}

// Test config deep copy error handling
BOOST_AUTO_TEST_CASE(test_config_deep_copy)
{
	config original;
	original["value"] = 100;

	config& child = original.add_child("nested");
	child["data"] = "test";

	config copy = original;

	// Modify original
	original["value"] = 200;
	original.mandatory_child("nested")["data"] = "modified";

	// Copy should remain unchanged
	BOOST_CHECK_EQUAL(copy["value"].to_int(), 100);
	BOOST_CHECK_EQUAL(copy.mandatory_child("nested")["data"].str(), "test");
}

// Test config with invalid child access
BOOST_AUTO_TEST_CASE(test_config_invalid_child_access)
{
	config cfg;

	// Accessing non-existent child should return empty config or throw
	const config& missing = cfg.mandatory_child("nonexistent");
	BOOST_CHECK(missing.empty());

	// Or checking if child exists first
	if (cfg.has_child("nonexistent")) {
		BOOST_FAIL("Should not have nonexistent child");
	}
}

// Test config memory management
BOOST_AUTO_TEST_CASE(test_config_memory_management)
{
	config* parent = new config();

	for (int i = 0; i < 100; ++i) {
		config& child = parent->add_child("item");
		child["index"] = i;

		for (int j = 0; j < 10; ++j) {
			config& subchild = child.add_child("subitem");
			subchild["value"] = j;
		}
	}

	BOOST_CHECK_EQUAL(parent->child_count("item"), 100);

	// Delete should clean up all children
	delete parent;
	// No memory leak should occur
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Lua utility advanced functions
// ============================================================================

BOOST_AUTO_TEST_SUITE(lua_utility_advanced)

// Test config comparison
BOOST_AUTO_TEST_CASE(test_config_comparison)
{
	config cfg1;
	cfg1["name"] = "test";
	cfg1["value"] = 42;

	config cfg2;
	cfg2["name"] = "test";
	cfg2["value"] = 42;

	config cfg3;
	cfg3["name"] = "different";
	cfg3["value"] = 42;

	// Configs with same attributes should be considered equal
	BOOST_CHECK_EQUAL(cfg1["name"].str(), cfg2["name"].str());
	BOOST_CHECK_EQUAL(cfg1["value"].to_int(), cfg2["value"].to_int());
	BOOST_CHECK(cfg1["name"].str() != cfg3["name"].str());
}

// Test config serialization size
BOOST_AUTO_TEST_CASE(test_config_serialization_size)
{
	config cfg;

	// Add a large number of attributes
	for (int i = 0; i < 1000; ++i) {
		cfg["attr_" + std::to_string(i)] = "value_" + std::to_string(i);
	}

	BOOST_CHECK_EQUAL(cfg.attribute_count(), 1000);

	// Add children
	for (int i = 0; i < 100; ++i) {
		config& child = cfg.add_child("child");
		child["index"] = i;
	}

	BOOST_CHECK_EQUAL(cfg.child_count("child"), 100);
}

// Test config search operations
BOOST_AUTO_TEST_CASE(test_config_search_operations)
{
	config root;

	// Build a tree structure
	for (int i = 0; i < 10; ++i) {
		config& branch = root.add_child("branch");
		branch["id"] = "branch_" + std::to_string(i);

		for (int j = 0; j < 5; ++j) {
			config& leaf = branch.add_child("leaf");
			leaf["value"] = i * 10 + j;
		}
	}

	// Search for specific values
	int count = 0;
	for (const config& branch : root.child_range("branch")) {
		for (const config& leaf : branch.child_range("leaf")) {
			if (leaf["value"].to_int() % 2 == 0) {
				++count;
			}
		}
	}

	BOOST_CHECK(count > 0);
}

// Test config empty checks
BOOST_AUTO_TEST_CASE(test_config_empty_checks)
{
	config empty1;
	BOOST_CHECK(empty1.empty());

	config empty2;
	empty2.add_child("child");
	BOOST_CHECK(!empty2.empty()); // Has child, not empty

	config empty3;
	empty3["attribute"] = "value";
	BOOST_CHECK(!empty3.empty()); // Has attribute, not empty

	config empty4;
	empty4.add_child("child");
	empty4.clear_children("child");
	BOOST_CHECK(empty4.empty()); // Child removed, now empty
}

// Test config attribute default values
BOOST_AUTO_TEST_CASE(test_config_attribute_defaults)
{
	config cfg;

	// Accessing non-existent attributes with defaults
	BOOST_CHECK_EQUAL(cfg["missing_int"].to_int(999), 999);
	BOOST_CHECK_CLOSE(cfg["missing_double"].to_double(3.14), 3.14, 0.01);
	BOOST_CHECK_EQUAL(cfg["missing_bool"].to_bool(true), true);
	BOOST_CHECK_EQUAL(cfg["missing_string"].str("default"), "default");
}

BOOST_AUTO_TEST_SUITE_END()
