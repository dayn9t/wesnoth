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

BOOST_AUTO_TEST_SUITE( font )

// Test font configuration basic
BOOST_AUTO_TEST_CASE( test_font_config_basic )
{
	config cfg;
	cfg["name"] = "DejaVuSans.ttf";
	cfg["size"] = "14";

	BOOST_CHECK_EQUAL(cfg["name"].str(), "DejaVuSans.ttf");
	BOOST_CHECK_EQUAL(cfg["size"].str(), "14");
}

// Test font with style
BOOST_AUTO_TEST_CASE( test_font_with_style )
{
	config cfg;
	cfg["name"] = "DejaVuSans.ttf";
	cfg["size"] = "16";
	cfg["bold"] = "yes";
	cfg["italic"] = "no";

	BOOST_CHECK_EQUAL(cfg["bold"].str(), "yes");
	BOOST_CHECK_EQUAL(cfg["italic"].str(), "no");
}

// Test font color
BOOST_AUTO_TEST_CASE( test_font_color )
{
	config cfg;
	cfg["name"] = "DejaVuSans.ttf";
	cfg["color"] = "255,255,255";

	BOOST_CHECK_EQUAL(cfg["color"].str(), "255,255,255");
}

// Test font markup
BOOST_AUTO_TEST_CASE( test_font_markup )
{
	config cfg;
	cfg["text"] = "<b>Bold</b> <i>Italic</i>";

	BOOST_CHECK_EQUAL(cfg["text"].str(), "<b>Bold</b> <i>Italic</i>");
}

// Test font empty
BOOST_AUTO_TEST_CASE( test_font_empty )
{
	config cfg;

	BOOST_CHECK(cfg.empty());
	BOOST_CHECK(cfg["name"].empty());
}

BOOST_AUTO_TEST_SUITE_END()
