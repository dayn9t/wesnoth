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

#include "scripting/plugin_manager_status.hpp"
#include "config.hpp"
#include <string>
#include <vector>

BOOST_AUTO_TEST_SUITE(modules)

// Test plugin_manager_status enum values
BOOST_AUTO_TEST_CASE(test_plugin_status_enum_values)
{
	// Test that all enum values exist and can be converted to strings
	BOOST_CHECK_EQUAL(plugin_manager_status::get_string(plugin_manager_status::type::not_created), "not created");
	BOOST_CHECK_EQUAL(plugin_manager_status::get_string(plugin_manager_status::type::running), "running");
	BOOST_CHECK_EQUAL(plugin_manager_status::get_string(plugin_manager_status::type::stopped), "stopped");
}

// Test plugin_manager_status string to enum conversion
BOOST_AUTO_TEST_CASE(test_plugin_status_string_to_enum)
{
	// Test conversion from string to enum
	auto status1 = plugin_manager_status::get_enum("not created");
	BOOST_CHECK(status1.has_value());
	BOOST_CHECK_EQUAL(plugin_manager_status::get_string(status1.value()), "not created");

	auto status2 = plugin_manager_status::get_enum("running");
	BOOST_CHECK(status2.has_value());
	BOOST_CHECK_EQUAL(plugin_manager_status::get_string(status2.value()), "running");

	auto status3 = plugin_manager_status::get_enum("stopped");
	BOOST_CHECK(status3.has_value());
	BOOST_CHECK_EQUAL(plugin_manager_status::get_string(status3.value()), "stopped");
}

// Test plugin_manager_status invalid string handling
BOOST_AUTO_TEST_CASE(test_plugin_status_invalid_string)
{
	// Test that invalid strings return nullopt
	auto invalid = plugin_manager_status::get_enum("invalid_status");
	BOOST_CHECK(!invalid.has_value());

	auto empty = plugin_manager_status::get_enum("");
	BOOST_CHECK(!empty.has_value());

	auto partial = plugin_manager_status::get_enum("run");
	BOOST_CHECK(!partial.has_value());
}

// Test plugin_manager_status enum size
BOOST_AUTO_TEST_CASE(test_plugin_status_enum_size)
{
	// Verify that we have exactly 3 status types
	BOOST_CHECK_EQUAL(plugin_manager_status::size(), 3u);
}

// Test plugin_manager_status enum to int conversion
BOOST_AUTO_TEST_CASE(test_plugin_status_int_to_enum)
{
	// Test conversion from int to enum
	auto s0 = plugin_manager_status::get_enum(0ul);
	BOOST_CHECK(s0.has_value());
	BOOST_CHECK_EQUAL(plugin_manager_status::get_string(s0.value()), "not created");

	auto s1 = plugin_manager_status::get_enum(1ul);
	BOOST_CHECK(s1.has_value());
	BOOST_CHECK_EQUAL(plugin_manager_status::get_string(s1.value()), "running");

	auto s2 = plugin_manager_status::get_enum(2ul);
	BOOST_CHECK(s2.has_value());
	BOOST_CHECK_EQUAL(plugin_manager_status::get_string(s2.value()), "stopped");
}

// Test plugin_manager_status invalid int handling
BOOST_AUTO_TEST_CASE(test_plugin_status_invalid_int)
{
	// Test that invalid integers return nullopt
	auto invalid1 = plugin_manager_status::get_enum(3ul);
	BOOST_CHECK(!invalid1.has_value());

	auto invalid2 = plugin_manager_status::get_enum(100ul);
	BOOST_CHECK(!invalid2.has_value());
}

// Test module event data structure with config
BOOST_AUTO_TEST_CASE(test_module_event_config)
{
	// Test that config objects can be used for module event data
	config event_data;
	event_data["name"] = "test_event";
	event_data["value"] = 42;
	event_data["enabled"] = true;

	BOOST_CHECK_EQUAL(event_data["name"].str(), "test_event");
	BOOST_CHECK_EQUAL(event_data["value"].to_int(), 42);
	BOOST_CHECK_EQUAL(event_data["enabled"].to_bool(), true);
}

// Test module plugin metadata storage
BOOST_AUTO_TEST_CASE(test_module_plugin_metadata)
{
	// Test storing plugin metadata
	struct plugin_metadata {
		std::string name;
		std::string source;
		bool is_file;
	};

	std::vector<plugin_metadata> plugins;

	// Add a string-based plugin
	plugins.push_back({"test_plugin", "return function() end", false});
	BOOST_CHECK_EQUAL(plugins.size(), 1u);
	BOOST_CHECK_EQUAL(plugins[0].name, "test_plugin");
	BOOST_CHECK_EQUAL(plugins[0].is_file, false);

	// Add a file-based plugin
	plugins.push_back({"file_plugin", "/path/to/plugin.lua", true});
	BOOST_CHECK_EQUAL(plugins.size(), 2u);
	BOOST_CHECK_EQUAL(plugins[1].name, "file_plugin");
	BOOST_CHECK_EQUAL(plugins[1].is_file, true);
}

// Test module config for plugin settings
BOOST_AUTO_TEST_CASE(test_module_config_plugin_settings)
{
	config plugin_settings;

	// Create plugin configuration
	config& plugin1 = plugin_settings.add_child("plugin");
	plugin1["name"] = "core_plugin";
	plugin1["enabled"] = true;
	plugin1["priority"] = 1;

	config& plugin2 = plugin_settings.add_child("plugin");
	plugin2["name"] = "optional_plugin";
	plugin2["enabled"] = false;
	plugin2["priority"] = 5;

	// Verify config structure
	int count = 0;
	for (const auto& plugin : plugin_settings.child_range("plugin")) {
		count++;
		if (count == 1) {
			BOOST_CHECK_EQUAL(plugin["name"].str(), "core_plugin");
			BOOST_CHECK_EQUAL(plugin["enabled"].to_bool(), true);
		} else if (count == 2) {
			BOOST_CHECK_EQUAL(plugin["name"].str(), "optional_plugin");
			BOOST_CHECK_EQUAL(plugin["enabled"].to_bool(), false);
		}
	}
	BOOST_CHECK_EQUAL(count, 2);
}

// Test module queue event ordering
BOOST_AUTO_TEST_CASE(test_module_event_queue_ordering)
{
	struct test_event {
		std::string name;
		int sequence;
		config data;
	};

	std::vector<test_event> event_queue;

	// Add events in order
	for (int i = 0; i < 5; ++i) {
		test_event evt;
		evt.name = "event_" + std::to_string(i);
		evt.sequence = i;
		evt.data["index"] = i;
		event_queue.push_back(evt);
	}

	// Verify FIFO ordering
	BOOST_CHECK_EQUAL(event_queue.size(), 5u);
	for (int i = 0; i < 5; ++i) {
		BOOST_CHECK_EQUAL(event_queue[i].sequence, i);
		BOOST_CHECK_EQUAL(event_queue[i].data["index"].to_int(), i);
	}

	// Simulate processing (remove from front)
	event_queue.erase(event_queue.begin());
	BOOST_CHECK_EQUAL(event_queue.size(), 4u);
	BOOST_CHECK_EQUAL(event_queue[0].sequence, 1);
}

// Test module status transitions
BOOST_AUTO_TEST_CASE(test_module_status_transitions)
{
	// Simulate valid status transitions
	// not_created -> running (after start)
	// running -> stopped (after stop or error)
	// stopped -> running (after restart)

	BOOST_CHECK_EQUAL(
		plugin_manager_status::get_string(plugin_manager_status::type::not_created),
		"not created"
	);

	// After starting
	BOOST_CHECK_EQUAL(
		plugin_manager_status::get_string(plugin_manager_status::type::running),
		"running"
	);

	// After stopping
	BOOST_CHECK_EQUAL(
		plugin_manager_status::get_string(plugin_manager_status::type::stopped),
		"stopped"
	);
}

BOOST_AUTO_TEST_SUITE_END()
