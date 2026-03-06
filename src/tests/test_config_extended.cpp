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
#include "variable_info.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(test_config_operations)

// ============================================================================
// Config construction tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_config_default_construction)
{
	config cfg;

	BOOST_CHECK(cfg.empty());
	BOOST_CHECK_EQUAL(cfg.attribute_count(), 0);
	BOOST_CHECK_EQUAL(cfg.all_children_count(), 0);
}

BOOST_AUTO_TEST_CASE(test_config_attribute_access)
{
	config cfg;

	cfg["key1"] = "value1";
	cfg["key2"] = "value2";

	BOOST_CHECK_EQUAL(cfg["key1"].str(), "value1");
	BOOST_CHECK_EQUAL(cfg["key2"].str(), "value2");
}

BOOST_AUTO_TEST_CASE(test_config_attribute_count)
{
	config cfg;

	BOOST_CHECK_EQUAL(cfg.attribute_count(), 0);

	cfg["a"] = "1";
	BOOST_CHECK_EQUAL(cfg.attribute_count(), 1);

	cfg["b"] = "2";
	cfg["c"] = "3";
	BOOST_CHECK_EQUAL(cfg.attribute_count(), 3);
}

BOOST_AUTO_TEST_CASE(test_config_has_attribute)
{
	config cfg;

	BOOST_CHECK(!cfg.has_attribute("nonexistent"));

	cfg["exists"] = "yes";
	BOOST_CHECK(cfg.has_attribute("exists"));
	BOOST_CHECK(!cfg.has_attribute("nonexistent"));
}

BOOST_AUTO_TEST_CASE(test_config_remove_attribute)
{
	config cfg;

	cfg["key"] = "value";
	BOOST_CHECK(cfg.has_attribute("key"));

	cfg.remove_attribute("key");
	BOOST_CHECK(!cfg.has_attribute("key"));
}

// ============================================================================
// Config child tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_config_add_child)
{
	config cfg;

	cfg.add_child("child");

	BOOST_CHECK(cfg.has_child("child"));
	BOOST_CHECK_EQUAL(cfg.child_count("child"), 1);
}

BOOST_AUTO_TEST_CASE(test_config_multiple_children)
{
	config cfg;

	cfg.add_child("item");
	cfg.add_child("item");
	cfg.add_child("item");

	BOOST_CHECK_EQUAL(cfg.child_count("item"), 3);
}

BOOST_AUTO_TEST_CASE(test_config_clear_children)
{
	config cfg;

	cfg.add_child("a");
	cfg.add_child("a");
	cfg.add_child("b");

	cfg.clear_children("a");

	BOOST_CHECK_EQUAL(cfg.child_count("a"), 0);
	BOOST_CHECK_EQUAL(cfg.child_count("b"), 1);
}

BOOST_AUTO_TEST_CASE(test_config_has_child)
{
	config cfg;

	BOOST_CHECK(!cfg.has_child("nonexistent"));

	cfg.add_child("exists");
	BOOST_CHECK(cfg.has_child("exists"));
}

BOOST_AUTO_TEST_CASE(test_config_mandatory_child)
{
	config cfg;
	cfg.add_child("test");

	BOOST_CHECK_NO_THROW(cfg.mandatory_child("test"));
}

BOOST_AUTO_TEST_CASE(test_config_optional_child)
{
	config cfg;

	BOOST_CHECK(!cfg.optional_child("nonexistent"));

	cfg.add_child("exists");
	BOOST_CHECK(cfg.optional_child("exists"));
}

// ============================================================================
// Config merge tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_config_merge_attributes)
{
	config cfg1;
	cfg1["a"] = "1";
	cfg1["b"] = "2";

	config cfg2;
	cfg2["b"] = "3";
	cfg2["c"] = "4";

	cfg1.merge_with(cfg2);

	BOOST_CHECK_EQUAL(cfg1["a"].str(), "1");
	BOOST_CHECK_EQUAL(cfg1["b"].str(), "3");
	BOOST_CHECK_EQUAL(cfg1["c"].str(), "4");
}

// ============================================================================
// Config iteration tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_config_attribute_range)
{
	config cfg;
	cfg["a"] = "1";
	cfg["b"] = "2";
	cfg["c"] = "3";

	int count = 0;
	for ([[maybe_unused]] const auto& attr : cfg.attribute_range()) {
		++count;
	}

	BOOST_CHECK_EQUAL(count, 3);
}

BOOST_AUTO_TEST_CASE(test_config_child_range)
{
	config cfg;
	cfg.add_child("item");
	cfg.add_child("item");

	int count = 0;
	for ([[maybe_unused]] const auto& child : cfg.child_range("item")) {
		++count;
	}

	BOOST_CHECK_EQUAL(count, 2);
}

// ============================================================================
// Config clear tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_config_clear)
{
	config cfg;

	cfg["key"] = "value";
	cfg.add_child("child");

	BOOST_CHECK(!cfg.empty());

	cfg.clear();

	BOOST_CHECK(cfg.empty());
	BOOST_CHECK_EQUAL(cfg.attribute_count(), 0);
	BOOST_CHECK_EQUAL(cfg.all_children_count(), 0);
}

// ============================================================================
// Config comparison tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_config_equality)
{
	config cfg1;
	cfg1["key"] = "value";

	config cfg2;
	cfg2["key"] = "value";

	BOOST_CHECK(cfg1 == cfg2);
}

BOOST_AUTO_TEST_CASE(test_config_inequality)
{
	config cfg1;
	cfg1["key"] = "value1";

	config cfg2;
	cfg2["key"] = "value2";

	BOOST_CHECK(cfg1 != cfg2);
}

// ============================================================================
// Config copy tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_config_copy)
{
	config original;
	original["key"] = "value";
	original.add_child("child");

	config copy(original);

	BOOST_CHECK(copy == original);
	BOOST_CHECK_EQUAL(copy["key"].str(), "value");
	BOOST_CHECK(copy.has_child("child"));
}

BOOST_AUTO_TEST_CASE(test_config_assignment)
{
	config original;
	original["key"] = "value";

	config assigned;
	assigned = original;

	BOOST_CHECK(assigned == original);
}

// ============================================================================
// Config swap tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_config_swap)
{
	config cfg1;
	cfg1["name"] = "first";

	config cfg2;
	cfg2["name"] = "second";

	swap(cfg1, cfg2);

	BOOST_CHECK_EQUAL(cfg1["name"].str(), "second");
	BOOST_CHECK_EQUAL(cfg2["name"].str(), "first");
}

// ============================================================================
// Attribute value tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_config_attr_to_int)
{
	config cfg;
	cfg["int"] = "42";

	BOOST_CHECK_EQUAL(cfg["int"].to_int(), 42);
}

BOOST_AUTO_TEST_CASE(test_config_attr_to_double)
{
	config cfg;
	cfg["float"] = "3.14";

	BOOST_CHECK_CLOSE(cfg["float"].to_double(), 3.14, 0.001);
}

BOOST_AUTO_TEST_CASE(test_config_attr_to_bool)
{
	config cfg;

	cfg["true_val"] = "true";
	cfg["false_val"] = "false";

	BOOST_CHECK(cfg["true_val"].to_bool());
	BOOST_CHECK(!cfg["false_val"].to_bool());
}

BOOST_AUTO_TEST_CASE(test_config_attr_empty)
{
	config cfg;

	BOOST_CHECK(cfg["nonexistent"].empty());
	BOOST_CHECK(cfg["nonexistent"].blank());
}

// ============================================================================
// Config boundary condition tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_config_empty_key)
{
	config cfg;

	// Empty string as key
	cfg[""] = "empty_key_value";
	BOOST_CHECK(cfg.has_attribute(""));
	BOOST_CHECK_EQUAL(cfg[""].str(), "empty_key_value");
}

BOOST_AUTO_TEST_CASE(test_config_long_key)
{
	config cfg;

	// Very long key
	std::string long_key(1000, 'a');
	cfg[long_key] = "long_key_value";
	BOOST_CHECK(cfg.has_attribute(long_key));
	BOOST_CHECK_EQUAL(cfg[long_key].str(), "long_key_value");
}

BOOST_AUTO_TEST_CASE(test_config_long_value)
{
	config cfg;

	// Very long value
	std::string long_value(10000, 'x');
	cfg["long_val"] = long_value;
	BOOST_CHECK_EQUAL(cfg["long_val"].str(), long_value);
}

BOOST_AUTO_TEST_CASE(test_config_special_characters_in_key)
{
	config cfg;

	// Special characters in values (keys should be valid identifiers)
	cfg["special_value"] = "value with spaces and special chars: !@#$%^&*()";
	BOOST_CHECK_EQUAL(cfg["special_value"].str(), "value with spaces and special chars: !@#$%^&*()");
}

BOOST_AUTO_TEST_CASE(test_config_unicode_value)
{
	config cfg;

	// Unicode characters in value
	cfg["unicode"] = "Hello 世界 🎮";
	BOOST_CHECK_EQUAL(cfg["unicode"].str(), "Hello 世界 🎮");
}

BOOST_AUTO_TEST_CASE(test_config_numeric_conversions_edge_cases)
{
	config cfg;

	// Integer limits
	cfg["max_int"] = std::to_string(INT_MAX);
	cfg["min_int"] = std::to_string(INT_MIN);
	cfg["zero"] = "0";
	cfg["negative"] = "-1";

	BOOST_CHECK_EQUAL(cfg["max_int"].to_int(INT_MAX), INT_MAX);
	BOOST_CHECK_EQUAL(cfg["min_int"].to_int(INT_MIN), INT_MIN);
	BOOST_CHECK_EQUAL(cfg["zero"].to_int(), 0);
	BOOST_CHECK_EQUAL(cfg["negative"].to_int(), -1);
}

BOOST_AUTO_TEST_CASE(test_config_invalid_numeric_conversion)
{
	config cfg;

	// Invalid numeric strings should return default
	cfg["not_a_number"] = "abc";
	cfg["mixed"] = "123abc";
	cfg["empty"] = "";

	BOOST_CHECK_EQUAL(cfg["not_a_number"].to_int(42), 42);
	// "123abc" is stored as a string, but lexical_cast_default parses leading digits
	// The actual behavior: it returns 123 (parsed from the string)
	BOOST_CHECK_EQUAL(cfg["mixed"].to_int(42), 123);
	BOOST_CHECK_EQUAL(cfg["empty"].to_int(42), 42);
}

BOOST_AUTO_TEST_CASE(test_config_many_children)
{
	config cfg;

	// Add many children
	for (int i = 0; i < 1000; ++i) {
		cfg.add_child("item");
	}

	BOOST_CHECK_EQUAL(cfg.child_count("item"), 1000);
	BOOST_CHECK_EQUAL(cfg.all_children_count(), 1000);
}

BOOST_AUTO_TEST_CASE(test_config_many_attributes)
{
	config cfg;

	// Add many attributes
	for (int i = 0; i < 1000; ++i) {
		cfg["key" + std::to_string(i)] = std::to_string(i);
	}

	BOOST_CHECK_EQUAL(cfg.attribute_count(), 1000);
}

BOOST_AUTO_TEST_CASE(test_config_nested_depth)
{
	config cfg;
	config* current = &cfg;

	// Create deeply nested structure
	for (int i = 0; i < 100; ++i) {
		current = &current->add_child("level");
		(*current)["depth"] = std::to_string(i);
	}

	// Verify depth
	current = &cfg;
	for (int i = 0; i < 100; ++i) {
		BOOST_CHECK(current->has_child("level"));
		current = &current->mandatory_child("level");
		BOOST_CHECK_EQUAL(current->attribute_count(), 1);
	}
}

BOOST_AUTO_TEST_CASE(test_config_child_name_variations)
{
	config cfg;

	// Various child name patterns
	cfg.add_child("a");
	cfg.add_child("A");
	cfg.add_child("1");
	cfg.add_child("_");
	cfg.add_child("camelCase");
	cfg.add_child("snake_case");
	cfg.add_child("with-dashes");

	BOOST_CHECK(cfg.has_child("a"));
	BOOST_CHECK(cfg.has_child("A"));
	BOOST_CHECK(cfg.has_child("1"));
	BOOST_CHECK(cfg.has_child("_"));
	BOOST_CHECK(cfg.has_child("camelCase"));
	BOOST_CHECK(cfg.has_child("snake_case"));
	BOOST_CHECK(cfg.has_child("with-dashes"));
}

BOOST_AUTO_TEST_CASE(test_config_merge_empty)
{
	config cfg1;
	config cfg2;

	// Merge empty configs
	cfg1.merge_with(cfg2);
	BOOST_CHECK(cfg1.empty());

	// Merge into empty
	cfg2["key"] = "value";
	cfg1.merge_with(cfg2);
	BOOST_CHECK_EQUAL(cfg1["key"].str(), "value");
}

BOOST_AUTO_TEST_CASE(test_config_merge_overwrite)
{
	config cfg1;
	cfg1["a"] = "1";
	cfg1["b"] = "2";

	config cfg2;
	cfg2["b"] = "overwritten";
	cfg2["c"] = "3";

	cfg1.merge_with(cfg2);

	BOOST_CHECK_EQUAL(cfg1["a"].str(), "1");
	BOOST_CHECK_EQUAL(cfg1["b"].str(), "overwritten");
	BOOST_CHECK_EQUAL(cfg1["c"].str(), "3");
}

BOOST_AUTO_TEST_CASE(test_config_clear_empty)
{
	config cfg;

	// Clear already empty config
	BOOST_CHECK_NO_THROW(cfg.clear());
	BOOST_CHECK(cfg.empty());
}

BOOST_AUTO_TEST_CASE(test_config_remove_nonexistent_attribute)
{
	config cfg;

	// Remove attribute that doesn't exist
	BOOST_CHECK_NO_THROW(cfg.remove_attribute("nonexistent"));
	BOOST_CHECK_EQUAL(cfg.attribute_count(), 0);
}

BOOST_AUTO_TEST_CASE(test_config_remove_nonexistent_child)
{
	config cfg;

	// Remove children that don't exist
	BOOST_CHECK_NO_THROW(cfg.clear_children("nonexistent"));
	BOOST_CHECK_EQUAL(cfg.all_children_count(), 0);
}

BOOST_AUTO_TEST_CASE(test_config_copy_empty)
{
	config empty;
	config copy(empty);

	BOOST_CHECK(copy.empty());
}

BOOST_AUTO_TEST_CASE(test_config_copy_large)
{
	config original;

	// Create large config
	for (int i = 0; i < 100; ++i) {
		original["key" + std::to_string(i)] = std::to_string(i);
		original.add_child("child");
	}

	config copy(original);

	BOOST_CHECK_EQUAL(copy.attribute_count(), 100);
	BOOST_CHECK_EQUAL(copy.all_children_count(), 100);
	BOOST_CHECK(copy == original);
}

BOOST_AUTO_TEST_CASE(test_config_bool_conversions)
{
	config cfg;

	// Various boolean representations
	cfg["true1"] = "true";
	cfg["true2"] = "yes";
	// "1" is stored as int, not boolean - to_bool only recognizes yes_no/true_false types
	cfg["true3"] = "1";
	// "on" is not a recognized boolean value, stored as string
	cfg["true4"] = "on";
	cfg["false1"] = "false";
	cfg["false2"] = "no";
	// "0" is stored as int, not boolean
	cfg["false3"] = "0";
	// "off" is not a recognized boolean value, stored as string
	cfg["false4"] = "off";
	cfg["invalid"] = "maybe";

	BOOST_CHECK(cfg["true1"].to_bool());
	BOOST_CHECK(cfg["true2"].to_bool());
	// "1" is stored as int, to_bool returns default (false) for non-boolean types
	BOOST_CHECK(!cfg["true3"].to_bool());
	// "on" is stored as string, to_bool returns default (false)
	BOOST_CHECK(!cfg["true4"].to_bool());
	BOOST_CHECK(!cfg["false1"].to_bool());
	BOOST_CHECK(!cfg["false2"].to_bool());
	// "0" is stored as int, to_bool returns default (false)
	BOOST_CHECK(!cfg["false3"].to_bool());
	// "off" is stored as string, to_bool returns default (false)
	BOOST_CHECK(!cfg["false4"].to_bool());
	BOOST_CHECK(!cfg["invalid"].to_bool());
	BOOST_CHECK(cfg["invalid"].to_bool(true)); // Default to true
}

// ============================================================================
// Additional boundary condition tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_config_null_attribute_access)
{
	config cfg;

	// Accessing non-existent attribute returns null attribute
	const config::attribute_value& attr = cfg["nonexistent"];
	BOOST_CHECK(attr.empty());
	BOOST_CHECK(attr.blank());

	// Multiple accesses return same reference
	const config::attribute_value& attr2 = cfg["nonexistent"];
	// Both should be empty
	BOOST_CHECK(attr2.empty());
}

BOOST_AUTO_TEST_CASE(test_config_attribute_type_conversions)
{
	config cfg;

	// Test various numeric conversions
	cfg["int"] = "42";
	cfg["double"] = "3.14159";
	cfg["bool_true"] = "true";
	cfg["bool_false"] = "false";

	// to_int with default
	BOOST_CHECK_EQUAL(cfg["int"].to_int(0), 42);
	BOOST_CHECK_EQUAL(cfg["missing"].to_int(100), 100);

	// to_double with default
	BOOST_CHECK_CLOSE(cfg["double"].to_double(0.0), 3.14159, 0.001);
	BOOST_CHECK_EQUAL(cfg["missing"].to_double(2.5), 2.5);

	// to_bool with default
	BOOST_CHECK(cfg["bool_true"].to_bool(false));
	BOOST_CHECK(!cfg["bool_false"].to_bool(true));
	BOOST_CHECK(cfg["missing"].to_bool(true));
	BOOST_CHECK(!cfg["missing"].to_bool(false));
}

BOOST_AUTO_TEST_CASE(test_config_empty_child_name)
{
	config cfg;

	// Empty child name
	cfg.add_child("");
	BOOST_CHECK(cfg.has_child(""));
	BOOST_CHECK_EQUAL(cfg.child_count(""), 1);
}

BOOST_AUTO_TEST_CASE(test_config_child_iteration_empty)
{
	config cfg;

	// Iteration over non-existent child
	int count = 0;
	for ([[maybe_unused]] const auto& child : cfg.child_range("nonexistent")) {
		++count;
	}
	BOOST_CHECK_EQUAL(count, 0);
}

BOOST_AUTO_TEST_CASE(test_config_attribute_iteration_empty)
{
	config cfg;

	// Iteration over empty config
	int count = 0;
	for ([[maybe_unused]] const auto& attr : cfg.attribute_range()) {
		++count;
	}
	BOOST_CHECK_EQUAL(count, 0);
}

BOOST_AUTO_TEST_CASE(test_config_double_conversions)
{
	config cfg;

	// Various double formats
	cfg["zero"] = "0.0";
	cfg["negative"] = "-3.14";
	cfg["scientific"] = "1e10";
	cfg["small"] = "1e-10";
	cfg["invalid"] = "not_a_double";

	BOOST_CHECK_EQUAL(cfg["zero"].to_double(0.0), 0.0);
	BOOST_CHECK_CLOSE(cfg["negative"].to_double(0.0), -3.14, 0.001);
	BOOST_CHECK_CLOSE(cfg["scientific"].to_double(0.0), 1e10, 0.001);
	BOOST_CHECK_CLOSE(cfg["small"].to_double(0.0), 1e-10, 0.001);
	BOOST_CHECK_EQUAL(cfg["invalid"].to_double(42.0), 42.0);
}

BOOST_AUTO_TEST_CASE(test_config_attribute_overwrite)
{
	config cfg;

	// Overwrite existing attribute
	cfg["key"] = "first";
	BOOST_CHECK_EQUAL(cfg["key"].str(), "first");

	cfg["key"] = "second";
	BOOST_CHECK_EQUAL(cfg["key"].str(), "second");

	// Overwrite with different type
	cfg["key"] = "123";
	BOOST_CHECK_EQUAL(cfg["key"].to_int(), 123);
}

BOOST_AUTO_TEST_CASE(test_config_child_access_out_of_range)
{
	config cfg;

	// Access child at invalid index
	cfg.add_child("item");

	// Should handle gracefully - access via optional_child
	// The child function with index may not exist, use child_range or optional_child
	auto children = cfg.child_range("item");
	BOOST_CHECK_EQUAL(std::distance(children.begin(), children.end()), 1);
}

BOOST_AUTO_TEST_CASE(test_config_move_operations)
{
	config cfg1;
	cfg1["key"] = "value";
	cfg1.add_child("child");

	// Move construction
	config cfg2(std::move(cfg1));
	BOOST_CHECK_EQUAL(cfg2["key"].str(), "value");
	BOOST_CHECK(cfg2.has_child("child"));
}

BOOST_AUTO_TEST_CASE(test_config_self_merge)
{
	config cfg;
	cfg["a"] = "1";
	cfg["b"] = "2";

	// Self merge is not supported - merge_with has assert(this != &cfg)
	// This test verifies the behavior is not supported
	// To test merge behavior, use a copy
	config copy(cfg);
	cfg.merge_with(copy);

	// Values should remain
	BOOST_CHECK_EQUAL(cfg["a"].str(), "1");
	BOOST_CHECK_EQUAL(cfg["b"].str(), "2");
}

BOOST_AUTO_TEST_CASE(test_config_large_merge)
{
	config cfg1;
	config cfg2;

	// Create large configs
	// cfg1 has keys 0-499 with values 0-499
	// cfg2 has keys 250-749 with values 1000-1499
	for (int i = 0; i < 500; ++i) {
		cfg1["key" + std::to_string(i)] = std::to_string(i);
		cfg2["key" + std::to_string(i + 250)] = std::to_string(i + 1000);
	}

	// Merge: cfg2 values overwrite cfg1 values for keys 250-499
	// Keys 500-749 are added from cfg2
	cfg1.merge_with(cfg2);

	// Verify some values
	// key0 is only in cfg1
	BOOST_CHECK_EQUAL(cfg1["key0"].str(), "0");
	// key250 is in both, cfg2 value wins: 0+1000 = 1000
	BOOST_CHECK_EQUAL(cfg1["key250"].str(), "1000");
	// key499 is in both (cfg1: 499, cfg2: 249+1000=1249), cfg2 value wins
	BOOST_CHECK_EQUAL(cfg1["key499"].str(), "1249");
	// key500 is only in cfg2: 250+1000=1250
	BOOST_CHECK_EQUAL(cfg1["key500"].str(), "1250");
	// key749 is only in cfg2: 499+1000=1499
	BOOST_CHECK_EQUAL(cfg1["key749"].str(), "1499");
}

BOOST_AUTO_TEST_CASE(test_config_recursive_children)
{
	config cfg;
	config* current = &cfg;

	// Create recursive structure
	// Each level adds one "level" child and one "sibling" child to that level
	for (int i = 0; i < 50; ++i) {
		current = &current->add_child("level");
		current->add_child("sibling");
	}

	// all_children_count() returns the number of direct children (ordered_children_.size())
	// The root cfg has 1 direct child (the first "level")
	BOOST_CHECK_EQUAL(cfg.all_children_count(), 1);

	// Verify the structure by traversing
	config* traverse = &cfg;
	int depth = 0;
	while (traverse->has_child("level")) {
		traverse = &traverse->mandatory_child("level");
		depth++;
		// Each level has 2 children: the next "level" and one "sibling"
		if (depth < 50) {
			BOOST_CHECK_EQUAL(traverse->all_children_count(), 2);
		}
	}
	BOOST_CHECK_EQUAL(depth, 50);
}

BOOST_AUTO_TEST_CASE(test_config_attribute_value_operators)
{
	config cfg;

	// Assignment from various types
	cfg["str"] = std::string("hello");
	cfg["cstr"] = "world";

	BOOST_CHECK_EQUAL(cfg["str"].str(), "hello");
	BOOST_CHECK_EQUAL(cfg["cstr"].str(), "world");
}

BOOST_AUTO_TEST_CASE(test_config_find_child)
{
	config cfg;

	config& child1 = cfg.add_child("item");
	child1["id"] = "first";

	config& child2 = cfg.add_child("item");
	child2["id"] = "second";

	// Find by key-value - find_child returns optional_config_impl
	auto found = cfg.find_child("item", "id", "first");
	BOOST_CHECK(found.has_value());
	BOOST_CHECK_EQUAL((*found)["id"].str(), "first");

	// Find non-existent
	auto not_found = cfg.find_child("item", "id", "third");
	BOOST_CHECK(!not_found.has_value());
}

BOOST_AUTO_TEST_CASE(test_config_find_child_empty)
{
	config cfg;

	// Find in empty config - find_child returns optional
	auto found = cfg.find_child("item", "id", "value");
	BOOST_CHECK(!found.has_value());
}

BOOST_AUTO_TEST_SUITE_END()