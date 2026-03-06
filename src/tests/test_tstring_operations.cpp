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

#include "tstring.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(test_tstring_operations)

BOOST_AUTO_TEST_CASE(test_tstring_default_construction)
{
	t_string str;

	BOOST_CHECK(str.empty());
}

BOOST_AUTO_TEST_CASE(test_tstring_from_cstring)
{
	t_string str("hello");

	BOOST_CHECK_EQUAL(str, "hello");
	BOOST_CHECK(!str.empty());
}

BOOST_AUTO_TEST_CASE(test_tstring_from_std_string)
{
	std::string s = "test string";
	t_string str(s);

	BOOST_CHECK_EQUAL(str, s);
}

BOOST_AUTO_TEST_CASE(test_tstring_copy)
{
	t_string str1("copy me");
	t_string str2(str1);

	BOOST_CHECK_EQUAL(str1, str2);
}

BOOST_AUTO_TEST_CASE(test_tstring_assignment)
{
	t_string str1;
	t_string str2("assigned");

	str1 = str2;

	BOOST_CHECK_EQUAL(str1, str2);
}

BOOST_AUTO_TEST_CASE(test_tstring_concatenation)
{
	t_string str1("hello");
	t_string str2(" world");

	t_string result = str1 + str2;

	BOOST_CHECK_EQUAL(result, "hello world");
}

BOOST_AUTO_TEST_CASE(test_tstring_size)
{
	t_string str("hello");

	BOOST_CHECK_EQUAL(str.size(), 5);
}

BOOST_AUTO_TEST_CASE(test_tstring_empty)
{
	t_string empty;
	t_string nonempty("text");

	BOOST_CHECK(empty.empty());
	BOOST_CHECK(!nonempty.empty());
}

BOOST_AUTO_TEST_CASE(test_tstring_comparison)
{
	t_string str1("same");
	t_string str2("same");
	t_string str3("different");

	BOOST_CHECK(str1 == str2);
	BOOST_CHECK(str1 != str3);
	BOOST_CHECK(str1 == "same");
}

BOOST_AUTO_TEST_CASE(test_tstring_translatable)
{
	t_string str("text", "wesnoth");

	BOOST_CHECK(!str.empty());
}

BOOST_AUTO_TEST_CASE(test_tstring_to_string)
{
	t_string str("test value");

	std::string s = str;

	BOOST_CHECK_EQUAL(s, "test value");
}

BOOST_AUTO_TEST_CASE(test_tstring_base_str)
{
	t_string str("base text");

	BOOST_CHECK_EQUAL(str.base_str(), "base text");
}

BOOST_AUTO_TEST_CASE(test_tstring_append)
{
	t_string str("start");
	str += " end";

	BOOST_CHECK_EQUAL(str, "start end");
}

BOOST_AUTO_TEST_SUITE_END()