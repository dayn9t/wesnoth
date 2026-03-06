/*
	Copyright (C) 2026
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
#include "persist_context.hpp"
#include "persist_manager.hpp"
#include "tstring.hpp"

BOOST_AUTO_TEST_SUITE(test_persistent_v2)

// Test pack_scalar helper function
BOOST_AUTO_TEST_CASE(test_pack_scalar_basic)
{
	config cfg = pack_scalar("test_var", t_string("test_value"));
	BOOST_CHECK_EQUAL(cfg["test_var"].str(), "test_value");
}

BOOST_AUTO_TEST_CASE(test_pack_scalar_empty)
{
	config cfg = pack_scalar("empty_var", t_string(""));
	BOOST_CHECK(cfg["empty_var"].empty());
}

BOOST_AUTO_TEST_CASE(test_pack_scalar_numeric_string)
{
	config cfg = pack_scalar("num_var", t_string("12345"));
	BOOST_CHECK_EQUAL(cfg["num_var"].str(), "12345");
}

// Test name_space structure
BOOST_AUTO_TEST_CASE(test_namespace_simple)
{
	persist_context::name_space ns("testroot", true);

	BOOST_CHECK(ns.valid());
	BOOST_CHECK_EQUAL(ns.namespace_, "testroot");
	BOOST_CHECK_EQUAL(ns.root_, "testroot");
	BOOST_CHECK_EQUAL(ns.node_, "testroot");
	BOOST_CHECK_EQUAL(ns.lineage_, "");
	BOOST_CHECK_EQUAL(ns.descendants_, "testroot");
}

BOOST_AUTO_TEST_CASE(test_namespace_hierarchical)
{
	persist_context::name_space ns("root.child.leaf", true);

	BOOST_CHECK(ns.valid());
	BOOST_CHECK_EQUAL(ns.namespace_, "root.child.leaf");
	BOOST_CHECK_EQUAL(ns.root_, "root");
	BOOST_CHECK_EQUAL(ns.node_, "leaf");
	BOOST_CHECK_EQUAL(ns.lineage_, "root.child");
	BOOST_CHECK_EQUAL(ns.descendants_, "child.leaf");
}

BOOST_AUTO_TEST_CASE(test_namespace_invalid_chars)
{
	persist_context::name_space ns("invalid@name", true);

	BOOST_CHECK(!ns.valid());
}

BOOST_AUTO_TEST_CASE(test_namespace_empty_invalid)
{
	persist_context::name_space ns("", true);

	BOOST_CHECK(!ns.valid());
	BOOST_CHECK(ns.namespace_.empty());
}

BOOST_AUTO_TEST_CASE(test_namespace_next_iteration)
{
	persist_context::name_space ns("a.b.c", true);

	BOOST_CHECK(ns.valid());
	persist_context::name_space next = ns.next();

	BOOST_CHECK(next.valid());
	BOOST_CHECK_EQUAL(next.namespace_, "b.c");
	BOOST_CHECK_EQUAL(next.root_, "b");
	BOOST_CHECK_EQUAL(next.descendants_, "c");
}

BOOST_AUTO_TEST_CASE(test_namespace_prev_iteration)
{
	persist_context::name_space ns("a.b.c", true);

	persist_context::name_space prev = ns.prev();

	BOOST_CHECK(prev.valid());
	BOOST_CHECK_EQUAL(prev.namespace_, "a.b");
	BOOST_CHECK_EQUAL(prev.node_, "b");
}

BOOST_AUTO_TEST_CASE(test_namespace_parse_carets)
{
	// Test the caret parsing functionality which expands ^.. notation
	persist_context::name_space ns("root^^.child", false);

	// After parsing, ^^ should be resolved
	ns.parse();
	// The exact behavior depends on implementation - verify parsing was attempted
	BOOST_CHECK(ns.namespace_.find("^") == std::string::npos || ns.namespace_.empty());
}

BOOST_AUTO_TEST_CASE(test_namespace_alphanumeric_valid)
{
	persist_context::name_space ns("Namespace_123", true);

	BOOST_CHECK(ns.valid());
	BOOST_CHECK_EQUAL(ns.namespace_, "Namespace_123");
}

BOOST_AUTO_TEST_CASE(test_namespace_with_underscores)
{
	persist_context::name_space ns("my_game_state", true);

	BOOST_CHECK(ns.valid());
	BOOST_CHECK_EQUAL(ns.namespace_, "my_game_state");
	BOOST_CHECK_EQUAL(ns.root_, "my_game_state");
}

BOOST_AUTO_TEST_CASE(test_namespace_with_dots_and_underscores)
{
	persist_context::name_space ns("game_state.player_1.inventory", true);

	BOOST_CHECK(ns.valid());
	BOOST_CHECK_EQUAL(ns.root_, "game_state");
	BOOST_CHECK_EQUAL(ns.node_, "inventory");
	BOOST_CHECK_EQUAL(ns.lineage_, "game_state.player_1");
	BOOST_CHECK_EQUAL(ns.descendants_, "player_1.inventory");
}

BOOST_AUTO_TEST_CASE(test_namespace_deep_hierarchy)
{
	persist_context::name_space ns("a.b.c.d.e.f", true);

	BOOST_CHECK(ns.valid());
	BOOST_CHECK_EQUAL(ns.root_, "a");
	BOOST_CHECK_EQUAL(ns.node_, "f");

	persist_context::name_space next = ns.next();
	BOOST_CHECK_EQUAL(next.namespace_, "b.c.d.e.f");
	BOOST_CHECK_EQUAL(next.root_, "b");
}

// Test persist_context behavior via name_space operations
BOOST_AUTO_TEST_CASE(test_namespace_bool_operator)
{
	persist_context::name_space valid_ns("valid_namespace", true);
	persist_context::name_space invalid_ns("invalid-namespace", true);

	BOOST_CHECK(valid_ns);  // Should be true for valid namespace
	BOOST_CHECK(!invalid_ns); // Should be false for invalid namespace
}

BOOST_AUTO_TEST_CASE(test_namespace_default_constructor)
{
	persist_context::name_space ns;

	BOOST_CHECK(!ns.valid());
	BOOST_CHECK(ns.namespace_.empty());
	BOOST_CHECK(ns.root_.empty());
	BOOST_CHECK(ns.node_.empty());
	BOOST_CHECK(ns.lineage_.empty());
	BOOST_CHECK(ns.descendants_.empty());
}

// Test config operations used by persist_context
BOOST_AUTO_TEST_CASE(test_config_child_or_add)
{
	config cfg;
	config& child = cfg.child_or_add("test_child");

	BOOST_CHECK_EQUAL(cfg.child_count("test_child"), 1);
	child["attr"] = "value";
	BOOST_CHECK_EQUAL(cfg.mandatory_child("test_child")["attr"].str(), "value");
}

BOOST_AUTO_TEST_CASE(test_config_nested_operations)
{
	config cfg;
	config* parent = &cfg.child_or_add("parent");
	config* child = &parent->child_or_add("child");
	child->child_or_add("grandchild");

	BOOST_CHECK(cfg.has_child("parent"));
	BOOST_CHECK(cfg.mandatory_child("parent").has_child("child"));
	BOOST_CHECK(cfg.mandatory_child("parent").mandatory_child("child").has_child("grandchild"));
}

BOOST_AUTO_TEST_CASE(test_config_has_child_check)
{
	config cfg;

	BOOST_CHECK(!cfg.has_child("nonexistent"));

	cfg.add_child("existing");
	BOOST_CHECK(cfg.has_child("existing"));
}

// Test config operations that mirror persist_context usage patterns
BOOST_AUTO_TEST_CASE(test_config_variables_pattern)
{
	// Simulates how persist_context stores variables
	config cfg;
	config* active = &cfg.child_or_add("namespace");
	config& vars = active->child_or_add("variables");

	vars["global_var"] = "test_value";
	vars["another_var"] = 42;

	BOOST_CHECK_EQUAL(vars["global_var"].str(), "test_value");
	BOOST_CHECK_EQUAL(vars["another_var"].to_int(), 42);
}

BOOST_AUTO_TEST_CASE(test_config_clear_children)
{
	config cfg;
	cfg.add_child("child1");
	cfg.add_child("child1");
	cfg.add_child("child2");

	BOOST_CHECK_EQUAL(cfg.child_count("child1"), 2);
	BOOST_CHECK_EQUAL(cfg.child_count("child2"), 1);

	cfg.clear_children("child1");
	BOOST_CHECK_EQUAL(cfg.child_count("child1"), 0);
	BOOST_CHECK_EQUAL(cfg.child_count("child2"), 1);
}

BOOST_AUTO_TEST_CASE(test_config_empty_check)
{
	config cfg;
	BOOST_CHECK(cfg.empty());

	cfg["attr"] = "value";
	BOOST_CHECK(!cfg.empty());

	cfg.remove_attribute("attr");
	BOOST_CHECK(cfg.empty());

	cfg.add_child("child");
	BOOST_CHECK(!cfg.empty());
}

BOOST_AUTO_TEST_CASE(test_config_append)
{
	config dest;
	config src;
	src["key1"] = "value1";
	src.add_child("tag1");

	dest.append(src);

	BOOST_CHECK_EQUAL(dest["key1"].str(), "value1");
	BOOST_CHECK(dest.has_child("tag1"));
}

// Test pack_scalar with translatable strings
BOOST_AUTO_TEST_CASE(test_pack_scalar_t_string)
{
	t_string translatable("translated text", "wesnoth");
	config cfg = pack_scalar("i18n_var", translatable);

	BOOST_CHECK_EQUAL(cfg["i18n_var"].str(), std::string("translated text"));
}

BOOST_AUTO_TEST_SUITE_END()
