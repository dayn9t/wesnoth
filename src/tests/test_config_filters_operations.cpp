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
#include "variable.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(test_config_filters_operations)

BOOST_AUTO_TEST_CASE(test_vconfig_has_attribute)
{
	config cfg;
	cfg["name"] = "test";

	vconfig vcfg(cfg);

	BOOST_CHECK(vcfg.has_attribute("name"));
	BOOST_CHECK(!vcfg.has_attribute("nonexistent"));
}

BOOST_AUTO_TEST_CASE(test_vconfig_attribute_value)
{
	config cfg;
	cfg["count"] = "42";

	vconfig vcfg(cfg);

	BOOST_CHECK_EQUAL(vcfg["count"].to_int(), 42);
}

BOOST_AUTO_TEST_CASE(test_vconfig_optional_child)
{
	config cfg;
	cfg.add_child("item");

	vconfig vcfg(cfg);

	BOOST_CHECK(vcfg.has_child("item"));
	BOOST_CHECK(!vcfg.has_child("nonexistent"));
}

BOOST_AUTO_TEST_CASE(test_vconfig_get_children)
{
	config cfg;
	cfg.add_child("item");
	cfg.add_child("item");
	cfg.add_child("item");

	vconfig vcfg(cfg);

	auto children = vcfg.get_children("item");

	BOOST_CHECK_EQUAL(children.size(), 3);
}

BOOST_AUTO_TEST_SUITE_END()