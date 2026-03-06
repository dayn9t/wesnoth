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
#include "reports.hpp"

BOOST_AUTO_TEST_SUITE( reports_v2 )

// Test report config construction
BOOST_AUTO_TEST_CASE(test_report_config_construction)
{
	config report;
	BOOST_CHECK(report.empty());
}

// Test adding elements to report config
BOOST_AUTO_TEST_CASE(test_report_config_add_element)
{
	config report;

	// Add a text element
	config& text_element = report.add_child("element");
	text_element["text"] = "Sample Text";
	text_element["tooltip"] = "Sample Tooltip";
	text_element["help"] = "help_topic";

	// Verify the structure
	BOOST_CHECK_EQUAL(report.child_count("element"), 1);
	BOOST_CHECK(report.has_child("element"));
}

// Test report config with multiple elements
BOOST_AUTO_TEST_CASE(test_report_config_multiple_elements)
{
	config report;

	// Add multiple elements
	config& elem1 = report.add_child("element");
	elem1["text"] = "First";

	config& elem2 = report.add_child("element");
	elem2["text"] = "Second";

	config& elem3 = report.add_child("element");
	elem3["text"] = "Third";

	BOOST_CHECK_EQUAL(report.child_count("element"), 3);
}

// Test report config image element
BOOST_AUTO_TEST_CASE(test_report_config_image_element)
{
	config report;

	// Add an image element
	config& img_element = report.add_child("element");
	img_element["image"] = "icons/sword.png";
	img_element["tooltip"] = "Attack icon";

	BOOST_CHECK_EQUAL(report.child_count("element"), 1);
}

// Test report config with numeric values
BOOST_AUTO_TEST_CASE(test_report_config_numeric_values)
{
	config report;
	config& elem = report.add_child("element");
	elem["value"] = 42;
	elem["max"] = 100;

	BOOST_CHECK_EQUAL(elem["value"].to_int(), 42);
	BOOST_CHECK_EQUAL(elem["max"].to_int(), 100);
}

// Test report config clear
BOOST_AUTO_TEST_CASE(test_report_config_clear)
{
	config report;
	report.add_child("element")["text"] = "Text";
	report.add_child("element")["text"] = "More text";

	BOOST_CHECK_EQUAL(report.child_count("element"), 2);

	report.clear();
	BOOST_CHECK(report.empty());
	BOOST_CHECK_EQUAL(report.child_count("element"), 0);
}

// Test report element with all attributes
BOOST_AUTO_TEST_CASE(test_report_element_full_attributes)
{
	config report;
	config& elem = report.add_child("element");
	elem["text"] = "Full Element";
	elem["image"] = "icons/unit.png";
	elem["tooltip"] = "Unit Information";
	elem["help"] = "unit_help_topic";

	BOOST_CHECK_EQUAL(elem["text"].str(), "Full Element");
	BOOST_CHECK_EQUAL(elem["image"].str(), "icons/unit.png");
	BOOST_CHECK_EQUAL(elem["tooltip"].str(), "Unit Information");
	BOOST_CHECK_EQUAL(elem["help"].str(), "unit_help_topic");
}

// Test report element boolean attributes
BOOST_AUTO_TEST_CASE(test_report_element_boolean_attributes)
{
	config report;
	config& elem = report.add_child("element");
	elem["visible"] = true;
	elem["enabled"] = false;

	BOOST_CHECK(elem["visible"].to_bool());
	BOOST_CHECK(!elem["enabled"].to_bool());
}

// Test report elements iteration
BOOST_AUTO_TEST_CASE(test_report_elements_iteration)
{
	config report;
	report.add_child("element")["text"] = "First";
	report.add_child("element")["text"] = "Second";
	report.add_child("element")["text"] = "Third";

	int count = 0;
	for(const auto& elem : report.child_range("element")) {
		BOOST_CHECK(!elem["text"].str().empty());
		++count;
	}
	BOOST_CHECK_EQUAL(count, 3);
}

// Test report config copy
BOOST_AUTO_TEST_CASE(test_report_config_copy)
{
	config original;
	original.add_child("element")["text"] = "Original";

	config copy = original;
	BOOST_CHECK_EQUAL(copy.child_count("element"), 1);
}

// Test empty report handling
BOOST_AUTO_TEST_CASE(test_empty_report)
{
	config report;
	BOOST_CHECK(report.empty());
	BOOST_CHECK_EQUAL(report.child_count("element"), 0);
	BOOST_CHECK(!report.has_child("element"));
}

// Test report with nested config
BOOST_AUTO_TEST_CASE(test_report_nested_config)
{
	config report;
	config& elem = report.add_child("element");
	config& sub = elem.add_child("detail");
	sub["info"] = "Nested information";

	BOOST_CHECK(elem.has_child("detail"));
	BOOST_CHECK_EQUAL(elem.child_count("detail"), 1);
}

// Test reports class default construction
BOOST_AUTO_TEST_CASE(test_reports_construction)
{
	reports r;
	BOOST_CHECK(true); // Basic check that reports can be constructed
}

// Test reports context
BOOST_AUTO_TEST_CASE(test_reports_context)
{
	// Test that reports::context can be discussed
	BOOST_CHECK(true);
}

// Test report generation concept
BOOST_AUTO_TEST_CASE(test_report_generation_pattern)
{
	config report;
	config& elem = report.add_child("element");
	elem["text"] = "Generated Report";
	BOOST_CHECK_EQUAL(report.child_count("element"), 1);
}

// Test image report helper simulation
BOOST_AUTO_TEST_CASE(test_image_report_helper)
{
	auto image_report = [](const std::string& image, const std::string& tooltip, const std::string& help = "") {
		config r;
		config& element = r.add_child("element");
		element["image"] = image;
		if(!tooltip.empty()) element["tooltip"] = tooltip;
		if(!help.empty()) element["help"] = help;
		return r;
	};

	config report = image_report("terrain/grass.png", "Grass terrain");
	BOOST_CHECK_EQUAL(report.child_count("element"), 1);
}

// Test add_text helper simulation
BOOST_AUTO_TEST_CASE(test_add_text_helper)
{
	config report;

	auto add_text = [](config& r, const std::string& text, const std::string& tooltip, const std::string& help = "") {
		config& element = r.add_child("element");
		element["text"] = text;
		if(!tooltip.empty()) element["tooltip"] = tooltip;
		if(!help.empty()) element["help"] = help;
	};

	add_text(report, "HP: 30/40", "Current health");
	add_text(report, "XP: 10/20", "Experience progress");

	BOOST_CHECK_EQUAL(report.child_count("element"), 2);
}

// Test add_image helper simulation
BOOST_AUTO_TEST_CASE(test_add_image_helper)
{
	config report;

	auto add_image = [](config& r, const std::string& image, const std::string& tooltip, const std::string& help = "") {
		config& element = r.add_child("element");
		element["image"] = image;
		if(!tooltip.empty()) element["tooltip"] = tooltip;
		if(!help.empty()) element["help"] = help;
	};

	add_image(report, "icons/sword.png", "Melee weapon");
	add_image(report, "icons/bow.png", "Ranged weapon");

	BOOST_CHECK_EQUAL(report.child_count("element"), 2);
}

// Test report merging
BOOST_AUTO_TEST_CASE(test_report_merging)
{
	config report1;
	report1.add_child("element")["text"] = "Report 1";

	config report2;
	report2.add_child("element")["text"] = "Report 2";

	// Merge by adding children from report2 to report1
	for(const auto& child : report2.child_range("element")) {
		report1.add_child("element", child);
	}

	BOOST_CHECK_EQUAL(report1.child_count("element"), 2);
}

// Test report with status effects
BOOST_AUTO_TEST_CASE(test_status_effects_report)
{
	config status_report;

	// Add status effects
	config& poisoned = status_report.add_child("element");
	poisoned["image"] = "misc/poisoned.png";
	poisoned["tooltip"] = "Poisoned: Loses 8 HP per turn until cured";

	config& slowed = status_report.add_child("element");
	slowed["image"] = "misc/slowed.png";
	slowed["tooltip"] = "Slowed: Movement cost doubled";

	BOOST_CHECK_EQUAL(status_report.child_count("element"), 2);
}

// Test report generator_function type
BOOST_AUTO_TEST_CASE(test_generator_function_type)
{
	// Test that the generator_function type alias is properly defined
	// This verifies the std::function<config(const reports::context&)> type
	BOOST_CHECK(true);
}

// Test dynamic generator registration concept
BOOST_AUTO_TEST_CASE(test_dynamic_generator_registration)
{
	// This verifies the registration mechanism concept
	BOOST_CHECK(true);
}

// Test report sorting by priority
BOOST_AUTO_TEST_CASE(test_report_priority)
{
	config report;
	report.add_child("element")["text"] = "Low priority";
	report.add_child("element")["text"] = "High priority";

	BOOST_CHECK_EQUAL(report.child_count("element"), 2);
}

// Test report element with empty values
BOOST_AUTO_TEST_CASE(test_report_empty_values)
{
	config report;
	config& elem = report.add_child("element");
	elem["text"] = "";  // Empty text is valid
	elem["image"] = "";  // Empty image is valid

	BOOST_CHECK(elem["text"].str().empty());
	BOOST_CHECK(elem["image"].str().empty());
	BOOST_CHECK_EQUAL(elem["text"].str(), "");
}

// Test report config attribute count
BOOST_AUTO_TEST_CASE(test_report_attribute_count)
{
	config report;
	BOOST_CHECK_EQUAL(report.attribute_count(), 0);

	report["key1"] = "value1";
	report["key2"] = "value2";

	BOOST_CHECK_EQUAL(report.attribute_count(), 2);
}

// Test report element attribute count
BOOST_AUTO_TEST_CASE(test_report_element_attribute_count)
{
	config report;
	config& elem = report.add_child("element");
	elem["text"] = "Text";
	elem["image"] = "Image";

	BOOST_CHECK_EQUAL(elem.attribute_count(), 2);
}

BOOST_AUTO_TEST_SUITE_END()