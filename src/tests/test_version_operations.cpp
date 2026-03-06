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

#include "game_version.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(test_version_operations)

BOOST_AUTO_TEST_CASE(test_version_info_major)
{
	version_info v("2.5.3");

	BOOST_CHECK_EQUAL(v.major_version(), 2);
}

BOOST_AUTO_TEST_CASE(test_version_info_minor)
{
	version_info v("1.16.2");

	BOOST_CHECK_EQUAL(v.minor_version(), 16);
}

BOOST_AUTO_TEST_CASE(test_version_info_revision)
{
	version_info v("1.14.9");

	BOOST_CHECK_EQUAL(v.revision_level(), 9);
}

BOOST_AUTO_TEST_CASE(test_version_info_string)
{
	version_info v("1.16.0");

	BOOST_CHECK_EQUAL(v.str(), "1.16.0");
}

BOOST_AUTO_TEST_CASE(test_version_info_is_canonical_true)
{
	version_info v("1.16.0");

	BOOST_CHECK(v.is_canonical());
}

BOOST_AUTO_TEST_CASE(test_version_info_is_canonical_false)
{
	version_info v("1.16.0.1");

	BOOST_CHECK(!v.is_canonical());
}

BOOST_AUTO_TEST_CASE(test_version_info_comparison_less_major)
{
	version_info v1("1.0.0");
	version_info v2("2.0.0");

	BOOST_CHECK(v1 < v2);
	BOOST_CHECK(v2 > v1);
}

BOOST_AUTO_TEST_CASE(test_version_info_comparison_less_minor)
{
	version_info v1("1.15.0");
	version_info v2("1.16.0");

	BOOST_CHECK(v1 < v2);
}

BOOST_AUTO_TEST_CASE(test_version_info_comparison_less_revision)
{
	version_info v1("1.16.0");
	version_info v2("1.16.1");

	BOOST_CHECK(v1 < v2);
}

BOOST_AUTO_TEST_CASE(test_version_info_equality)
{
	version_info v1("1.16.0");
	version_info v2("1.16.0");

	BOOST_CHECK(v1 == v2);
	BOOST_CHECK(!(v1 < v2));
	BOOST_CHECK(!(v1 > v2));
}

BOOST_AUTO_TEST_CASE(test_version_info_inequality)
{
	version_info v1("1.16.0");
	version_info v2("1.16.1");

	BOOST_CHECK(v1 != v2);
}

BOOST_AUTO_TEST_CASE(test_version_info_special_version)
{
	version_info v("1.16.0-dev");

	BOOST_CHECK_EQUAL(v.special_version(), "dev");
}

BOOST_AUTO_TEST_CASE(test_version_info_special_version_separator)
{
	version_info v("1.16.0+beta");

	BOOST_CHECK_EQUAL(v.special_version_separator(), '+');
}

BOOST_AUTO_TEST_CASE(test_version_info_with_suffix_greater)
{
	version_info v1("1.16.0");
	version_info v2("1.16.0+dev");

	BOOST_CHECK(v2 > v1);
}

BOOST_AUTO_TEST_SUITE_END()