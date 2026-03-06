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
#include "config.hpp"
#include <string>
#include <vector>
#include <memory>

// Test object for lua_ptr testing
struct test_lua_object : public enable_lua_ptr<test_lua_object> {
	std::string name;
	int value;
	test_lua_object(const std::string& n, int v)
		: enable_lua_ptr<test_lua_object>(this), name(n), value(v) {}
};

BOOST_AUTO_TEST_SUITE(lua_ptr_extended)

// Basic construction and access
BOOST_AUTO_TEST_CASE(test_lua_ptr_basic_construction)
{
	test_lua_object obj("test", 42);
	lua_ptr<test_lua_object> ptr(obj);

	BOOST_CHECK(ptr);
	BOOST_CHECK_EQUAL(ptr.get_ptr(), &obj);
	BOOST_CHECK_EQUAL(ptr->name, "test");
	BOOST_CHECK_EQUAL(ptr->value, 42);
}

// Test pointer invalidation on destruction
BOOST_AUTO_TEST_CASE(test_lua_ptr_invalidation_on_destruction)
{
	lua_ptr<test_lua_object> ptr(*std::make_unique<test_lua_object>("temp", 100).get());

	// After the unique_ptr is destroyed, ptr should be invalid
	BOOST_CHECK(!ptr);
}

// Test move constructor
BOOST_AUTO_TEST_CASE(test_lua_ptr_move_constructor)
{
	test_lua_object obj("move_test", 55);
	lua_ptr<test_lua_object> ptr1(obj);

	BOOST_CHECK(ptr1);

	lua_ptr<test_lua_object> ptr2(std::move(ptr1));
	BOOST_CHECK(ptr2);
	BOOST_CHECK_EQUAL(ptr2->name, "move_test");
}

// Test move of owning object
BOOST_AUTO_TEST_CASE(test_lua_ptr_owning_object_move)
{
	std::vector<test_lua_object> vec;
	vec.emplace_back("vector_obj", 200);

	lua_ptr<test_lua_object> ptr(vec.back());
	BOOST_CHECK(ptr);
	BOOST_CHECK_EQUAL(ptr->name, "vector_obj");

	auto& moved = vec.emplace_back(std::move(vec.front()));
	BOOST_CHECK(ptr);
	BOOST_CHECK_EQUAL(ptr.get_ptr(), &moved);
	BOOST_CHECK_EQUAL(ptr->name, "vector_obj");
}

// Test multiple lua_ptr to same object
BOOST_AUTO_TEST_CASE(test_lua_ptr_multiple_pointers)
{
	test_lua_object obj("shared", 300);
	lua_ptr<test_lua_object> ptr1(obj);
	lua_ptr<test_lua_object> ptr2(obj);

	BOOST_CHECK(ptr1);
	BOOST_CHECK(ptr2);
	BOOST_CHECK_EQUAL(ptr1.get_ptr(), ptr2.get_ptr());
	BOOST_CHECK_EQUAL(ptr1->name, ptr2->name);
}

// Test with shared_ptr container
BOOST_AUTO_TEST_CASE(test_lua_ptr_with_shared_ptr)
{
	auto shared = std::make_shared<test_lua_object>("shared_ptr_obj", 400);
	lua_ptr<test_lua_object> ptr(*shared);

	BOOST_CHECK(ptr);
	BOOST_CHECK_EQUAL(ptr->name, "shared_ptr_obj");

	shared.reset();
	BOOST_CHECK(!ptr);
}

// Test operator bool
BOOST_AUTO_TEST_CASE(test_lua_ptr_operator_bool)
{
	lua_ptr<test_lua_object> ptr(*std::make_unique<test_lua_object>("temp", 1).get());

	BOOST_CHECK(!ptr);
	BOOST_CHECK(!static_cast<bool>(ptr));
}

// Test operator!
BOOST_AUTO_TEST_CASE(test_lua_ptr_operator_not)
{
	lua_ptr<test_lua_object> ptr(*std::make_unique<test_lua_object>("temp", 1).get());

	BOOST_CHECK(!ptr);
}

// Test with container reallocation
BOOST_AUTO_TEST_CASE(test_lua_ptr_container_reallocation)
{
	std::vector<test_lua_object> vec;
	vec.reserve(10);

	vec.emplace_back("first", 1);
	lua_ptr<test_lua_object> ptr(vec.back());

	BOOST_CHECK(ptr);
	BOOST_CHECK_EQUAL(ptr->name, "first");

	for (int i = 0; i < 5; ++i) {
		vec.emplace_back("elem" + std::to_string(i), i + 2);
	}

	BOOST_CHECK(ptr);
}

// Test with nested containers
BOOST_AUTO_TEST_CASE(test_lua_ptr_nested_container)
{
	std::vector<std::unique_ptr<test_lua_object>> container;

	container.push_back(std::make_unique<test_lua_object>("nested1", 10));
	container.push_back(std::make_unique<test_lua_object>("nested2", 20));

	lua_ptr<test_lua_object> ptr1(*container[0]);
	lua_ptr<test_lua_object> ptr2(*container[1]);

	BOOST_CHECK(ptr1);
	BOOST_CHECK(ptr2);
	BOOST_CHECK_EQUAL(ptr1->value, 10);
	BOOST_CHECK_EQUAL(ptr2->value, 20);

	container.erase(container.begin());

	BOOST_CHECK(!ptr1);
	BOOST_CHECK(ptr2);
}

// Test assignment operator for owning object
BOOST_AUTO_TEST_CASE(test_lua_ptr_owning_object_assignment)
{
	test_lua_object obj1("first", 100);
	test_lua_object obj2("second", 200);

	lua_ptr<test_lua_object> ptr(obj1);
	BOOST_CHECK_EQUAL(ptr->name, "first");

	obj1 = std::move(obj2);
	BOOST_CHECK(ptr);
	BOOST_CHECK_EQUAL(ptr.get_ptr(), &obj1);
	BOOST_CHECK_EQUAL(ptr->name, "first");
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Lua-related config tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(lua_config_tests)

// Test Lua script configuration structure
BOOST_AUTO_TEST_CASE(test_lua_script_config_basic)
{
	config cfg;
	cfg["code"] = "return 42";
	cfg["name"] = "simple_test";

	BOOST_CHECK_EQUAL(cfg["code"].str(), "return 42");
	BOOST_CHECK_EQUAL(cfg["name"].str(), "simple_test");
}

// Test Lua script with arguments
BOOST_AUTO_TEST_CASE(test_lua_script_with_arguments)
{
	config cfg;
	cfg["code"] = "return args.x + args.y";

	config& args = cfg.add_child("args");
	args["x"] = "10";
	args["y"] = "20";

	BOOST_CHECK(cfg.has_child("args"));

	const config& args_cfg = cfg.mandatory_child("args");
	BOOST_CHECK_EQUAL(args_cfg["x"].str(), "10");
	BOOST_CHECK_EQUAL(args_cfg["y"].str(), "20");
}

// Test Lua event handler configuration
BOOST_AUTO_TEST_CASE(test_lua_event_handler_config)
{
	config event;
	event["name"] = "moveto";
	event["id"] = "test_event";
	event["first_time_only"] = "no";

	config& filter = event.add_child("filter");
	filter["x"] = "1-10";
	filter["y"] = "1-10";

	config& lua = event.add_child("lua");
	lua["code"] = "wesnoth.message('Event triggered')";

	BOOST_CHECK_EQUAL(event["name"].str(), "moveto");
	BOOST_CHECK(event.has_child("filter"));
	BOOST_CHECK(event.has_child("lua"));
}

// Test Lua module configuration
BOOST_AUTO_TEST_CASE(test_lua_module_config)
{
	config module;
	module["name"] = "custom_module";
	module["version"] = "1.0.0";

	config& file1 = module.add_child("file");
	file1["path"] = "scripts/main.lua";

	config& file2 = module.add_child("file");
	file2["path"] = "scripts/utils.lua";

	BOOST_CHECK_EQUAL(module["name"].str(), "custom_module");
	BOOST_CHECK_EQUAL(module.child_count("file"), 2);
}

// Test Lua AI configuration
BOOST_AUTO_TEST_CASE(test_lua_ai_config)
{
	config ai;
	ai["ai_algorithm"] = "lua_ai";

	config& lua_ai = ai.add_child("lua_ai");
	lua_ai["id"] = "custom_ai";
	lua_ai["evaluation"] = "return best_move";
	lua_ai["execution"] = "execute_move(best_move)";

	BOOST_CHECK_EQUAL(ai["ai_algorithm"].str(), "lua_ai");

	const config& lai = ai.mandatory_child("lua_ai");
	BOOST_CHECK_EQUAL(lai["id"].str(), "custom_ai");
}

// Test Lua tag structure
BOOST_AUTO_TEST_CASE(test_lua_tag_structure)
{
	config tag;
	tag["name"] = "[lua]";

	config& content = tag.add_child("content");
	content["code"] = "local x = 42";

	BOOST_CHECK(tag.has_child("content"));
}

// Test multiple Lua handlers
BOOST_AUTO_TEST_CASE(test_multiple_lua_handlers)
{
	config handlers;

	for (int i = 0; i < 3; ++i) {
		config& handler = handlers.add_child("lua_handler");
		handler["id"] = "handler_" + std::to_string(i);
		handler["priority"] = std::to_string(i * 10);
	}

	BOOST_CHECK_EQUAL(handlers.child_count("lua_handler"), 3);
}

// Test Lua WML actions
BOOST_AUTO_TEST_CASE(test_lua_wml_actions)
{
	config wml;
	wml["tag"] = "custom_action";

	config& action = wml.add_child("action");
	action["name"] = "my_action";

	config& param = action.add_child("parameter");
	param["name"] = "target";
	param["type"] = "string";

	BOOST_CHECK(wml.has_child("action"));
}

// Test Lua table serialization to config
BOOST_AUTO_TEST_CASE(test_lua_table_serialization)
{
	config table;
	table["type"] = "table";

	config& field1 = table.add_child("field");
	field1["name"] = "count";
	field1["value"] = "100";
	field1["type"] = "number";

	config& field2 = table.add_child("field");
	field2["name"] = "name";
	field2["value"] = "test_object";
	field2["type"] = "string";

	BOOST_CHECK_EQUAL(table.child_count("field"), 2);
}

// Test Lua GUI2 integration config
BOOST_AUTO_TEST_CASE(test_lua_gui2_config)
{
	config dialog;
	dialog["id"] = "custom_dialog";

	config& definition = dialog.add_child("definition");
	definition["type"] = "modal";

	config& lua_handler = definition.add_child("lua_handler");
	lua_handler["file"] = "scripts/dialog.lua";
	lua_handler["function"] = "show_dialog";

	BOOST_CHECK(dialog.has_child("definition"));
}

// Test Lua unit modification config
BOOST_AUTO_TEST_CASE(test_lua_unit_mod_config)
{
	config mod;
	mod["apply_to"] = "unit";

	config& filter = mod.add_child("filter");
	filter["type"] = "Elvish Fighter";

	config& effect = mod.add_child("effect");
	effect["apply_to"] = "attack";
	effect["increase_damage"] = "5";

	config& lua_node = effect.add_child("lua");
	lua_node["code"] = "custom_damage_calculation(unit)";

	BOOST_CHECK(mod.has_child("filter"));
	BOOST_CHECK(mod.has_child("effect"));
}

// Test Lua preload script config
BOOST_AUTO_TEST_CASE(test_lua_preload_config)
{
	config preload;
	preload["name"] = "preload";

	config& script = preload.add_child("lua");
	script["code"] = "wesnoth.require('compatibility')";

	BOOST_CHECK(preload.has_child("lua"));
}

// Test Lua ability definition
BOOST_AUTO_TEST_CASE(test_lua_ability_config)
{
	config ability;
	ability["id"] = "custom_ability";
	ability["name"] = "Custom Ability";

	config& lua_effect = ability.add_child("effect");
	lua_effect["apply_to"] = "lua";
	lua_effect["code"] = "apply_ability_effect(unit)";

	BOOST_CHECK(ability.has_child("effect"));
}

BOOST_AUTO_TEST_SUITE_END()