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

#include "ai/configuration.hpp"
#include "config.hpp"

BOOST_AUTO_TEST_SUITE( ai )

// Test AI configuration parsing
BOOST_AUTO_TEST_CASE( test_ai_configuration_basic )
{
	config cfg;
	cfg["id"] = "test_ai";
	cfg["description"] = "Test AI";
	cfg["version"] = "1.0";

	BOOST_CHECK_EQUAL(cfg["id"].str(), "test_ai");
	BOOST_CHECK_EQUAL(cfg["description"].str(), "Test AI");
	BOOST_CHECK_EQUAL(cfg["version"].str(), "1.0");
}

// Test AI configuration with aspects
BOOST_AUTO_TEST_CASE( test_ai_configuration_aspects )
{
	config cfg;
	config& aspect = cfg.add_child("aspect");
	aspect["id"] = "aggression";
	aspect["value"] = "0.5";

	BOOST_CHECK(cfg.has_child("aspect"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("aspect")["id"].str(), "aggression");
	BOOST_CHECK_EQUAL(cfg.mandatory_child("aspect")["value"].str(), "0.5");
}

// Test AI configuration with goals
BOOST_AUTO_TEST_CASE( test_ai_configuration_goals )
{
	config cfg;
	config& goal = cfg.add_child("goal");
	goal["name"] = "protect_leader";
	goal["value"] = "100";

	BOOST_CHECK(cfg.has_child("goal"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("goal")["name"].str(), "protect_leader");
	BOOST_CHECK_EQUAL(cfg.mandatory_child("goal")["value"].str(), "100");
}

// Test AI configuration with stages
BOOST_AUTO_TEST_CASE( test_ai_configuration_stages )
{
	config cfg;
	config& stage = cfg.add_child("stage");
	stage["id"] = "main_loop";
	stage["name"] = "Main Loop";

	BOOST_CHECK(cfg.has_child("stage"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("stage")["id"].str(), "main_loop");
	BOOST_CHECK_EQUAL(cfg.mandatory_child("stage")["name"].str(), "Main Loop");
}

// Test AI configuration with multiple aspects
BOOST_AUTO_TEST_CASE( test_ai_configuration_multiple_aspects )
{
	config cfg;

	config& aspect1 = cfg.add_child("aspect");
	aspect1["id"] = "aggression";
	aspect1["value"] = "0.5";

	config& aspect2 = cfg.add_child("aspect");
	aspect2["id"] = "caution";
	aspect2["value"] = "0.3";

	BOOST_CHECK_EQUAL(cfg.child_count("aspect"), 2u);
}

// Test AI configuration empty
BOOST_AUTO_TEST_CASE( test_ai_configuration_empty )
{
	config cfg;

	BOOST_CHECK(cfg.empty());
	BOOST_CHECK_EQUAL(cfg.child_count("aspect"), 0u);
	BOOST_CHECK_EQUAL(cfg.child_count("goal"), 0u);
	BOOST_CHECK_EQUAL(cfg.child_count("stage"), 0u);
}

// Test AI configuration clear
BOOST_AUTO_TEST_CASE( test_ai_configuration_clear )
{
	config cfg;
	cfg["id"] = "test_ai";
	cfg.add_child("aspect");

	BOOST_CHECK(!cfg.empty());

	cfg.clear();

	BOOST_CHECK(cfg.empty());
	BOOST_CHECK_EQUAL(cfg.child_count("aspect"), 0u);
}

BOOST_AUTO_TEST_SUITE_END()
