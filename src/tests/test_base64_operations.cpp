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

#include "serialization/base64.hpp"
#include <vector>
#include <string>

BOOST_AUTO_TEST_SUITE(test_base64_operations)

BOOST_AUTO_TEST_CASE(test_base64_encode_empty)
{
	std::vector<uint8_t> empty;
	std::string result = base64::encode(empty);

	BOOST_CHECK(result.empty());
}

BOOST_AUTO_TEST_CASE(test_base64_encode_single_byte)
{
	std::vector<uint8_t> data = {'A'};
	std::string result = base64::encode(data);

	BOOST_CHECK(!result.empty());
}

BOOST_AUTO_TEST_CASE(test_base64_encode_foo)
{
	std::vector<uint8_t> data = {'f', 'o', 'o'};
	std::string result = base64::encode(data);

	BOOST_CHECK_EQUAL(result, "Zm9v");
}

BOOST_AUTO_TEST_CASE(test_base64_encode_foob)
{
	std::vector<uint8_t> data = {'f', 'o', 'o', 'b'};
	std::string result = base64::encode(data);

	BOOST_CHECK_EQUAL(result, "Zm9vYg==");
}

BOOST_AUTO_TEST_CASE(test_base64_decode_empty)
{
	std::string empty;
	std::vector<uint8_t> result = base64::decode(empty);

	BOOST_CHECK(result.empty());
}

BOOST_AUTO_TEST_CASE(test_base64_decode_foo)
{
	std::vector<uint8_t> result = base64::decode("Zm9v");
	std::vector<uint8_t> expected = {'f', 'o', 'o'};

	BOOST_CHECK(result == expected);
}

BOOST_AUTO_TEST_CASE(test_base64_decode_foob)
{
	std::vector<uint8_t> result = base64::decode("Zm9vYg==");
	std::vector<uint8_t> expected = {'f', 'o', 'o', 'b'};

	BOOST_CHECK(result == expected);
}

BOOST_AUTO_TEST_CASE(test_base64_roundtrip)
{
	std::vector<uint8_t> original = {'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!'};
	std::string encoded = base64::encode(original);
	std::vector<uint8_t> decoded = base64::decode(encoded);

	BOOST_CHECK(decoded == original);
}

BOOST_AUTO_TEST_CASE(test_crypt64_encode_empty)
{
	std::vector<uint8_t> empty;
	std::string result = crypt64::encode(empty);

	BOOST_CHECK(result.empty());
}

BOOST_AUTO_TEST_CASE(test_crypt64_encode_foo)
{
	std::vector<uint8_t> data = {'f', 'o', 'o'};
	std::string result = crypt64::encode(data);

	BOOST_CHECK_EQUAL(result, "axqP");
}

BOOST_AUTO_TEST_CASE(test_crypt64_decode_foo)
{
	std::vector<uint8_t> result = crypt64::decode("axqP");
	std::vector<uint8_t> expected = {'f', 'o', 'o'};

	BOOST_CHECK(result == expected);
}

BOOST_AUTO_TEST_CASE(test_crypt64_roundtrip)
{
	std::vector<uint8_t> original = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	std::string encoded = crypt64::encode(original);
	std::vector<uint8_t> decoded = crypt64::decode(encoded);

	BOOST_CHECK(decoded == original);
}

BOOST_AUTO_TEST_CASE(test_crypt64_encode_decode_char)
{
	BOOST_CHECK_EQUAL(crypt64::encode(0), '.');
	BOOST_CHECK_EQUAL(crypt64::encode(63), 'z');
	BOOST_CHECK_EQUAL(crypt64::decode('.'), 0);
	BOOST_CHECK_EQUAL(crypt64::decode('z'), 63);
}

BOOST_AUTO_TEST_SUITE_END()