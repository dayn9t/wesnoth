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

BOOST_AUTO_TEST_SUITE( game_events )

// Test event configuration basic
BOOST_AUTO_TEST_CASE( test_event_config_basic )
{
	config cfg;
	cfg["name"] = "start";
	cfg["first_time_only"] = "yes";

	BOOST_CHECK_EQUAL(cfg["name"].str(), "start");
	BOOST_CHECK_EQUAL(cfg["first_time_only"].str(), "yes");
}

// Test event with filter
BOOST_AUTO_TEST_CASE( test_event_with_filter )
{
	config cfg;
	cfg["name"] = "moveto";
	
	config& filter = cfg.add_child("filter");
	filter["side"] = "1";
	filter["x"] = "10";
	filter["y"] = "15";

	BOOST_CHECK(cfg.has_child("filter"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("filter")["side"].str(), "1");
	BOOST_CHECK_EQUAL(cfg.mandatory_child("filter")["x"].str(), "10");
	BOOST_CHECK_EQUAL(cfg.mandatory_child("filter")["y"].str(), "15");
}

// Test event with condition
BOOST_AUTO_TEST_CASE( test_event_with_condition )
{
	config cfg;
	cfg["name"] = "turn_end";
	
	config& condition = cfg.add_child("condition");
	config& variable = condition.add_child("variable");
	variable["name"] = "turn_number";
	variable["equals"] = "5";

	BOOST_CHECK(cfg.has_child("condition"));
	BOOST_CHECK(cfg.mandatory_child("condition").has_child("variable"));
}

// Test event with actions
BOOST_AUTO_TEST_CASE( test_event_with_actions )
{
	config cfg;
	cfg["name"] = "victory";
	
	config& message = cfg.add_child("message");
	message["speaker"] = "narrator";
	message["message"] = "Victory!";

	config& endlevel = cfg.add_child("endlevel");
	endlevel["result"] = "victory";

	BOOST_CHECK(cfg.has_child("message"));
	BOOST_CHECK(cfg.has_child("endlevel"));
	BOOST_CHECK_EQUAL(cfg.child_count("message"), 1u);
	BOOST_CHECK_EQUAL(cfg.child_count("endlevel"), 1u);
}

// Test event with multiple filters
BOOST_AUTO_TEST_CASE( test_event_multiple_filters )
{
	config cfg;
	cfg["name"] = "attack";
	
	config& filter = cfg.add_child("filter");
	filter["side"] = "1";
	
	config& filter_second = cfg.add_child("filter_second");
	filter_second["side"] = "2";

	BOOST_CHECK(cfg.has_child("filter"));
	BOOST_CHECK(cfg.has_child("filter_second"));
}

// Test event delayed variable substitution
BOOST_AUTO_TEST_CASE( test_event_delayed_variable_substitution )
{
	config cfg;
	cfg["name"] = "test";
	cfg["id"] = "test_event_$turn_number";

	BOOST_CHECK_EQUAL(cfg["id"].str(), "test_event_$turn_number");
}

// Test event with nested conditions
BOOST_AUTO_TEST_CASE( test_event_nested_conditions )
{
	config cfg;
	cfg["name"] = "test";
	
	config& condition = cfg.add_child("condition");
	config& and_cond = condition.add_child("and");
	
	config& var1 = and_cond.add_child("variable");
	var1["name"] = "x";
	var1["equals"] = "1";
	
	config& var2 = and_cond.add_child("variable");
	var2["name"] = "y";
	var2["equals"] = "2";

	BOOST_CHECK(cfg.has_child("condition"));
	BOOST_CHECK(cfg.mandatory_child("condition").has_child("and"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("condition").mandatory_child("and").child_count("variable"), 2u);
}

BOOST_AUTO_TEST_SUITE_END()
