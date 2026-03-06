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

#include "lexical_cast.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(test_lexical_cast_operations)

BOOST_AUTO_TEST_CASE(test_lexical_cast_int_to_string)
{
	BOOST_CHECK_EQUAL(lexical_cast<std::string>(0), "0");
	BOOST_CHECK_EQUAL(lexical_cast<std::string>(1), "1");
	BOOST_CHECK_EQUAL(lexical_cast<std::string>(-1), "-1");
	BOOST_CHECK_EQUAL(lexical_cast<std::string>(12345), "12345");
	BOOST_CHECK_EQUAL(lexical_cast<std::string>(-12345), "-12345");
}

BOOST_AUTO_TEST_CASE(test_lexical_cast_string_to_int)
{
	BOOST_CHECK_EQUAL(lexical_cast<int>("0"), 0);
	BOOST_CHECK_EQUAL(lexical_cast<int>("1"), 1);
	BOOST_CHECK_EQUAL(lexical_cast<int>("-1"), -1);
	BOOST_CHECK_EQUAL(lexical_cast<int>("12345"), 12345);
	BOOST_CHECK_EQUAL(lexical_cast<int>("-12345"), -12345);
}

BOOST_AUTO_TEST_CASE(test_lexical_cast_default_int)
{
	BOOST_CHECK_EQUAL(lexical_cast_default<int>("invalid", 42), 42);
	BOOST_CHECK_EQUAL(lexical_cast_default<int>("123", 0), 123);
	BOOST_CHECK_EQUAL(lexical_cast_default<int>("", 100), 100);
}

BOOST_AUTO_TEST_CASE(test_lexical_cast_default_empty)
{
	BOOST_CHECK_EQUAL(lexical_cast_default<int>("", 0), 0);
	BOOST_CHECK_EQUAL(lexical_cast_default<std::string>(42, "default"), "42");
}

BOOST_AUTO_TEST_CASE(test_lexical_cast_double)
{
	BOOST_CHECK_CLOSE(lexical_cast<double>("3.14"), 3.14, 0.001);
	BOOST_CHECK_CLOSE(lexical_cast<double>("-2.5"), -2.5, 0.001);
	BOOST_CHECK_CLOSE(lexical_cast<double>("0.0"), 0.0, 0.001);
}

BOOST_AUTO_TEST_CASE(test_lexical_cast_float)
{
	BOOST_CHECK_CLOSE(lexical_cast<float>("1.5"), 1.5f, 0.001f);
	BOOST_CHECK_CLOSE(lexical_cast<float>("-0.5"), -0.5f, 0.001f);
}

BOOST_AUTO_TEST_CASE(test_lexical_cast_long_long)
{
	BOOST_CHECK_EQUAL(lexical_cast<long long>("123456789012"), 123456789012LL);
	BOOST_CHECK_EQUAL(lexical_cast<long long>("-123456789012"), -123456789012LL);
}

BOOST_AUTO_TEST_CASE(test_lexical_cast_unsigned)
{
	BOOST_CHECK_EQUAL(lexical_cast<unsigned>("123"), 123u);
	BOOST_CHECK_EQUAL(lexical_cast<unsigned>("0"), 0u);
}

BOOST_AUTO_TEST_CASE(test_lexical_cast_bool_true)
{
	BOOST_CHECK_EQUAL(lexical_cast<bool>("true"), true);
	BOOST_CHECK_EQUAL(lexical_cast<bool>("1"), true);
	BOOST_CHECK_EQUAL(lexical_cast<bool>("yes"), true);
}

BOOST_AUTO_TEST_CASE(test_lexical_cast_bool_false)
{
	BOOST_CHECK_EQUAL(lexical_cast<bool>("false"), false);
	BOOST_CHECK_EQUAL(lexical_cast<bool>("0"), false);
	BOOST_CHECK_EQUAL(lexical_cast<bool>("no"), false);
}

BOOST_AUTO_TEST_CASE(test_lexical_cast_string_to_string)
{
	BOOST_CHECK_EQUAL(lexical_cast<std::string>("hello"), "hello");
	BOOST_CHECK_EQUAL(lexical_cast<std::string>(""), "");
}

BOOST_AUTO_TEST_CASE(test_lexical_cast_char_to_string)
{
	BOOST_CHECK_EQUAL(lexical_cast<std::string>('a'), "a");
	BOOST_CHECK_EQUAL(lexical_cast<std::string>('Z'), "Z");
}

BOOST_AUTO_TEST_CASE(test_lexical_cast_large_int)
{
	BOOST_CHECK_EQUAL(lexical_cast<std::string>(2147483647), "2147483647");
	BOOST_CHECK_EQUAL(lexical_cast<int>("2147483647"), 2147483647);
}

BOOST_AUTO_TEST_CASE(test_lexical_cast_zero)
{
	BOOST_CHECK_EQUAL(lexical_cast<std::string>(0), "0");
	BOOST_CHECK_EQUAL(lexical_cast<int>("0"), 0);
	BOOST_CHECK_EQUAL(lexical_cast<double>("0"), 0.0);
}

BOOST_AUTO_TEST_SUITE_END()