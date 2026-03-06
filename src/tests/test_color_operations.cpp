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

#include "color.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(test_color_operations)

BOOST_AUTO_TEST_CASE(test_color_default_construction)
{
	color_t c;

	// Default constructor initializes to white (255, 255, 255, 255)
	BOOST_CHECK_EQUAL(c.r, 255);
	BOOST_CHECK_EQUAL(c.g, 255);
	BOOST_CHECK_EQUAL(c.b, 255);
	BOOST_CHECK_EQUAL(c.a, 255);
}

BOOST_AUTO_TEST_CASE(test_color_rgb_construction)
{
	color_t c(255, 128, 64);

	BOOST_CHECK_EQUAL(c.r, 255);
	BOOST_CHECK_EQUAL(c.g, 128);
	BOOST_CHECK_EQUAL(c.b, 64);
	BOOST_CHECK_EQUAL(c.a, 255);
}

BOOST_AUTO_TEST_CASE(test_color_rgba_construction)
{
	color_t c(255, 128, 64, 200);

	BOOST_CHECK_EQUAL(c.r, 255);
	BOOST_CHECK_EQUAL(c.g, 128);
	BOOST_CHECK_EQUAL(c.b, 64);
	BOOST_CHECK_EQUAL(c.a, 200);
}

BOOST_AUTO_TEST_CASE(test_color_from_hex_string)
{
	color_t c = color_t::from_hex_string("#FF8040");

	BOOST_CHECK_EQUAL(c.r, 255);
	BOOST_CHECK_EQUAL(c.g, 128);
	BOOST_CHECK_EQUAL(c.b, 64);
}

BOOST_AUTO_TEST_CASE(test_color_from_rgba_string)
{
	color_t c = color_t::from_rgba_string("255,128,64,200");

	BOOST_CHECK_EQUAL(c.r, 255);
	BOOST_CHECK_EQUAL(c.g, 128);
	BOOST_CHECK_EQUAL(c.b, 64);
	BOOST_CHECK_EQUAL(c.a, 200);
}

BOOST_AUTO_TEST_CASE(test_color_equality)
{
	color_t c1(255, 128, 64);
	color_t c2(255, 128, 64);
	color_t c3(255, 128, 65);

	BOOST_CHECK(c1 == c2);
	BOOST_CHECK(c1 != c3);
}

BOOST_AUTO_TEST_CASE(test_color_black)
{
	color_t black = color_t::from_hex_string("#000000");

	BOOST_CHECK_EQUAL(black.r, 0);
	BOOST_CHECK_EQUAL(black.g, 0);
	BOOST_CHECK_EQUAL(black.b, 0);
}

BOOST_AUTO_TEST_CASE(test_color_white)
{
	color_t white = color_t::from_hex_string("#FFFFFF");

	BOOST_CHECK_EQUAL(white.r, 255);
	BOOST_CHECK_EQUAL(white.g, 255);
	BOOST_CHECK_EQUAL(white.b, 255);
}

BOOST_AUTO_TEST_CASE(test_color_red)
{
	color_t red = color_t::from_hex_string("#FF0000");

	BOOST_CHECK_EQUAL(red.r, 255);
	BOOST_CHECK_EQUAL(red.g, 0);
	BOOST_CHECK_EQUAL(red.b, 0);
}

BOOST_AUTO_TEST_CASE(test_color_green)
{
	color_t green = color_t::from_hex_string("#00FF00");

	BOOST_CHECK_EQUAL(green.r, 0);
	BOOST_CHECK_EQUAL(green.g, 255);
	BOOST_CHECK_EQUAL(green.b, 0);
}

BOOST_AUTO_TEST_CASE(test_color_blue)
{
	color_t blue = color_t::from_hex_string("#0000FF");

	BOOST_CHECK_EQUAL(blue.r, 0);
	BOOST_CHECK_EQUAL(blue.g, 0);
	BOOST_CHECK_EQUAL(blue.b, 255);
}

BOOST_AUTO_TEST_CASE(test_color_transparent)
{
	color_t c(255, 128, 64, 0);

	BOOST_CHECK_EQUAL(c.a, 0);
}

BOOST_AUTO_TEST_CASE(test_color_opaque)
{
	color_t c(255, 128, 64, 255);

	BOOST_CHECK_EQUAL(c.a, 255);
}

BOOST_AUTO_TEST_CASE(test_color_copy)
{
	color_t c1(100, 150, 200);
	color_t c2 = c1;

	BOOST_CHECK(c1 == c2);
}

BOOST_AUTO_TEST_SUITE_END()