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

#include "utils/general.hpp"
#include "utils/math.hpp"
#include "utils/guard_value.hpp"
#include "utils/rate_counter.hpp"
#include "utils/const_clone.hpp"
#include "utils/span.hpp"
#include "utils/charconv.hpp"
#include "lexical_cast.hpp"

#include <array>
#include <map>
#include <set>
#include <string>
#include <vector>

BOOST_AUTO_TEST_SUITE(test_utils_v2)

// ==================== utils::contains tests ====================

BOOST_AUTO_TEST_CASE(test_contains_vector_found)
{
	std::vector<int> vec = {1, 2, 3, 4, 5};
	BOOST_CHECK(utils::contains(vec, 3));
	BOOST_CHECK(utils::contains(vec, 1));
	BOOST_CHECK(utils::contains(vec, 5));
}

BOOST_AUTO_TEST_CASE(test_contains_vector_not_found)
{
	std::vector<int> vec = {1, 2, 3, 4, 5};
	BOOST_CHECK(!utils::contains(vec, 0));
	BOOST_CHECK(!utils::contains(vec, 6));
	BOOST_CHECK(!utils::contains(vec, 10));
}

BOOST_AUTO_TEST_CASE(test_contains_set)
{
	std::set<int> s = {10, 20, 30};
	BOOST_CHECK(utils::contains(s, 10));
	BOOST_CHECK(utils::contains(s, 20));
	BOOST_CHECK(!utils::contains(s, 5));
	BOOST_CHECK(!utils::contains(s, 15));
}

BOOST_AUTO_TEST_CASE(test_contains_map)
{
	std::map<std::string, int> m = {{"a", 1}, {"b", 2}, {"c", 3}};
	BOOST_CHECK(utils::contains(m, std::string("a")));
	BOOST_CHECK(utils::contains(m, std::string("b")));
	BOOST_CHECK(!utils::contains(m, std::string("d")));
	BOOST_CHECK(!utils::contains(m, std::string("x")));
}

// ==================== utils::erase_if tests ====================

BOOST_AUTO_TEST_CASE(test_erase_if_removes_matching)
{
	std::vector<int> vec = {1, 2, 3, 4, 5, 6};
	utils::erase_if(vec, [](int x) { return x % 2 == 0; });
	BOOST_CHECK_EQUAL(vec.size(), 3u);
	BOOST_CHECK(utils::contains(vec, 1));
	BOOST_CHECK(utils::contains(vec, 3));
	BOOST_CHECK(utils::contains(vec, 5));
}

BOOST_AUTO_TEST_CASE(test_erase_if_removes_none)
{
	std::vector<int> vec = {1, 3, 5};
	utils::erase_if(vec, [](int x) { return x % 2 == 0; });
	BOOST_CHECK_EQUAL(vec.size(), 3u);
}

BOOST_AUTO_TEST_CASE(test_erase_if_removes_all)
{
	std::vector<int> vec = {2, 4, 6};
	utils::erase_if(vec, [](int x) { return x % 2 == 0; });
	BOOST_CHECK_EQUAL(vec.size(), 0u);
}

// ==================== utils::erase tests ====================

BOOST_AUTO_TEST_CASE(test_erase_removes_element)
{
	std::vector<int> vec = {1, 2, 3, 2, 4};
	std::size_t removed = utils::erase(vec, 2);
	BOOST_CHECK_EQUAL(removed, 2u);
	BOOST_CHECK_EQUAL(vec.size(), 3u);
	BOOST_CHECK(!utils::contains(vec, 2));
}

BOOST_AUTO_TEST_CASE(test_erase_not_found)
{
	std::vector<int> vec = {1, 2, 3};
	std::size_t removed = utils::erase(vec, 99);
	BOOST_CHECK_EQUAL(removed, 0u);
	BOOST_CHECK_EQUAL(vec.size(), 3u);
}

// ==================== utils::sort_if tests ====================

BOOST_AUTO_TEST_CASE(test_sort_if_ascending)
{
	std::vector<int> vec = {5, 2, 8, 1, 9};
	utils::sort_if(vec, [](int a, int b) { return a < b; });
	BOOST_CHECK_EQUAL(vec[0], 1);
	BOOST_CHECK_EQUAL(vec[1], 2);
	BOOST_CHECK_EQUAL(vec[2], 5);
	BOOST_CHECK_EQUAL(vec[3], 8);
	BOOST_CHECK_EQUAL(vec[4], 9);
}

BOOST_AUTO_TEST_CASE(test_sort_if_descending)
{
	std::vector<int> vec = {5, 2, 8, 1, 9};
	utils::sort_if(vec, [](int a, int b) { return a > b; });
	BOOST_CHECK_EQUAL(vec[0], 9);
	BOOST_CHECK_EQUAL(vec[1], 8);
	BOOST_CHECK_EQUAL(vec[2], 5);
	BOOST_CHECK_EQUAL(vec[3], 2);
	BOOST_CHECK_EQUAL(vec[4], 1);
}

// ==================== utils::find tests ====================

BOOST_AUTO_TEST_CASE(test_find_if_found)
{
	std::vector<int> vec = {1, 2, 3, 4, 5};
	int* result = utils::find_if(vec, [](int x) { return x > 3; });
	BOOST_REQUIRE(result != nullptr);
	BOOST_CHECK_EQUAL(*result, 4);
}

BOOST_AUTO_TEST_CASE(test_find_if_not_found)
{
	std::vector<int> vec = {1, 2, 3};
	int* result = utils::find_if(vec, [](int x) { return x > 10; });
	BOOST_CHECK(result == nullptr);
}

// ==================== utils::from_range tests ====================

BOOST_AUTO_TEST_CASE(test_from_range_vector)
{
	std::set<int> s = {3, 1, 4, 1, 5};
	std::vector<int> vec = utils::from_range<int>(s);
	BOOST_CHECK_EQUAL(vec.size(), 4u);  // set removes duplicates
	BOOST_CHECK(utils::contains(vec, 1));
	BOOST_CHECK(utils::contains(vec, 3));
	BOOST_CHECK(utils::contains(vec, 4));
	BOOST_CHECK(utils::contains(vec, 5));
}

// ==================== utils::to_underlying tests ====================

enum class test_enum : int {
	value_a = 10,
	value_b = 20,
	value_c = -5
};

BOOST_AUTO_TEST_CASE(test_to_underlying_positive)
{
	BOOST_CHECK_EQUAL(utils::to_underlying(test_enum::value_a), 10);
	BOOST_CHECK_EQUAL(utils::to_underlying(test_enum::value_b), 20);
}

BOOST_AUTO_TEST_CASE(test_to_underlying_negative)
{
	BOOST_CHECK_EQUAL(utils::to_underlying(test_enum::value_c), -5);
}

// ==================== bounded_add tests ====================

BOOST_AUTO_TEST_CASE(test_bounded_add_positive_increment)
{
	BOOST_CHECK_EQUAL(bounded_add(10, 5, 20, 0), 15);
	BOOST_CHECK_EQUAL(bounded_add(18, 5, 20, 0), 20);
	BOOST_CHECK_EQUAL(bounded_add(20, 5, 20, 0), 20);
}

BOOST_AUTO_TEST_CASE(test_bounded_add_negative_increment)
{
	BOOST_CHECK_EQUAL(bounded_add(10, -5, 20, 0), 5);
	BOOST_CHECK_EQUAL(bounded_add(3, -5, 20, 0), 0);
	BOOST_CHECK_EQUAL(bounded_add(0, -5, 20, 0), 0);
}

BOOST_AUTO_TEST_CASE(test_bounded_add_custom_bounds)
{
	BOOST_CHECK_EQUAL(bounded_add(50, 30, 100, 10), 80);
	BOOST_CHECK_EQUAL(bounded_add(90, 20, 100, 10), 100);
	BOOST_CHECK_EQUAL(bounded_add(20, -15, 100, 10), 10);
}

// ==================== modulo tests ====================

BOOST_AUTO_TEST_CASE(test_modulo_positive)
{
	BOOST_CHECK_EQUAL(modulo(7, 5, 0), 2);
	BOOST_CHECK_EQUAL(modulo(10, 3, 0), 1);
	BOOST_CHECK_EQUAL(modulo(15, 4, 0), 3);
}

BOOST_AUTO_TEST_CASE(test_modulo_negative)
{
	BOOST_CHECK_EQUAL(modulo(-3, 5, 0), 2);
	BOOST_CHECK_EQUAL(modulo(-7, 5, 0), 3);
	BOOST_CHECK_EQUAL(modulo(-1, 10, 0), 9);
}

BOOST_AUTO_TEST_CASE(test_modulo_with_min)
{
	BOOST_CHECK_EQUAL(modulo(7, 5, 1), 3);
	BOOST_CHECK_EQUAL(modulo(-3, 5, 1), 4);
}

// ==================== round_damage tests ====================

BOOST_AUTO_TEST_CASE(test_round_damage_basic)
{
	BOOST_CHECK_EQUAL(round_damage(100.0, 100, 100), 100);
	BOOST_CHECK_EQUAL(round_damage(100.0, 50, 100), 50);
	BOOST_CHECK_EQUAL(round_damage(100.0, 200, 100), 200);
}

BOOST_AUTO_TEST_CASE(test_round_damage_zero)
{
	BOOST_CHECK_EQUAL(round_damage(0.0, 100, 100), 0);
}

BOOST_AUTO_TEST_CASE(test_round_damage_minimum_one)
{
	BOOST_CHECK_EQUAL(round_damage(1.0, 1, 100), 1);
}

// ==================== in_ranges tests ====================

BOOST_AUTO_TEST_CASE(test_in_ranges_single_range)
{
	std::vector<std::pair<int, int>> ranges = {{5, 10}};
	BOOST_CHECK(in_ranges(5, ranges));
	BOOST_CHECK(in_ranges(7, ranges));
	BOOST_CHECK(in_ranges(10, ranges));
	BOOST_CHECK(!in_ranges(4, ranges));
	BOOST_CHECK(!in_ranges(11, ranges));
}

BOOST_AUTO_TEST_CASE(test_in_ranges_multiple_ranges)
{
	std::vector<std::pair<int, int>> ranges = {{1, 5}, {10, 15}, {20, 25}};
	BOOST_CHECK(in_ranges(3, ranges));
	BOOST_CHECK(in_ranges(12, ranges));
	BOOST_CHECK(in_ranges(22, ranges));
	BOOST_CHECK(!in_ranges(7, ranges));
	BOOST_CHECK(!in_ranges(17, ranges));
	BOOST_CHECK(!in_ranges(30, ranges));
}

// ==================== div100rounded tests ====================

BOOST_AUTO_TEST_CASE(test_div100rounded_positive)
{
	BOOST_CHECK_EQUAL(div100rounded(100), 1);
	BOOST_CHECK_EQUAL(div100rounded(150), 2);
	BOOST_CHECK_EQUAL(div100rounded(149), 1);
	BOOST_CHECK_EQUAL(div100rounded(50), 1);
	BOOST_CHECK_EQUAL(div100rounded(49), 0);
}

BOOST_AUTO_TEST_CASE(test_div100rounded_negative)
{
	BOOST_CHECK_EQUAL(div100rounded(-100), -1);
	BOOST_CHECK_EQUAL(div100rounded(-150), -2);
	BOOST_CHECK_EQUAL(div100rounded(-149), -1);
	BOOST_CHECK_EQUAL(div100rounded(-50), -1);
	BOOST_CHECK_EQUAL(div100rounded(-49), 0);
}

// ==================== rate_counter tests ====================

BOOST_AUTO_TEST_CASE(test_rate_counter_rate_one)
{
	utils::rate_counter counter(1);
	BOOST_CHECK(counter.poll());  // 0 % 1 == 0, true
	BOOST_CHECK(counter.poll());  // 1 % 1 == 0, true
	BOOST_CHECK(counter.poll());  // 2 % 1 == 0, true
}

BOOST_AUTO_TEST_CASE(test_rate_counter_rate_three)
{
	utils::rate_counter counter(3);
	BOOST_CHECK(counter.poll());     // 0 % 3 == 0, true
	BOOST_CHECK(!counter.poll());    // 1 % 3 == 1, false
	BOOST_CHECK(!counter.poll());    // 2 % 3 == 2, false
	BOOST_CHECK(counter.poll());     // 3 % 3 == 0, true
	BOOST_CHECK(!counter.poll());    // 4 % 3 == 1, false
}

// ==================== guard_value tests ====================

BOOST_AUTO_TEST_CASE(test_guard_value_restores)
{
	int value = 10;
	{
		utils::guard_value<int> guard(value, 42);
		BOOST_CHECK_EQUAL(value, 42);
	}
	BOOST_CHECK_EQUAL(value, 10);
}

BOOST_AUTO_TEST_CASE(test_guard_value_nested)
{
	int value = 0;
	{
		utils::guard_value<int> guard1(value, 10);
		BOOST_CHECK_EQUAL(value, 10);
		{
			utils::guard_value<int> guard2(value, 20);
			BOOST_CHECK_EQUAL(value, 20);
		}
		BOOST_CHECK_EQUAL(value, 10);
	}
	BOOST_CHECK_EQUAL(value, 0);
}

// ==================== fixed_point operations tests ====================

BOOST_AUTO_TEST_CASE(test_fixed_point_multiply)
{
	BOOST_CHECK_EQUAL(fixed_point_multiply(256, 256), 256u);  // 1.0 * 1.0 = 1.0
	BOOST_CHECK_EQUAL(fixed_point_multiply(512, 256), 512u);  // 2.0 * 1.0 = 2.0
	BOOST_CHECK_EQUAL(fixed_point_multiply(256, 128), 128u);  // 1.0 * 0.5 = 0.5
}

BOOST_AUTO_TEST_CASE(test_fixed_point_divide)
{
	BOOST_CHECK_EQUAL(fixed_point_divide(1, 1), 256);    // 1 / 1 = 1.0
	BOOST_CHECK_EQUAL(fixed_point_divide(2, 2), 256);    // 2 / 2 = 1.0
	BOOST_CHECK_EQUAL(fixed_point_divide(1, 2), 128);    // 1 / 2 = 0.5
}

BOOST_AUTO_TEST_CASE(test_fixed_point_to_int)
{
	BOOST_CHECK_EQUAL(fixed_point_to_int(256), 1);      // 1.0
	BOOST_CHECK_EQUAL(fixed_point_to_int(512), 2);      // 2.0
	BOOST_CHECK_EQUAL(fixed_point_to_int(128), 0);      // 0.5 -> 0
	BOOST_CHECK_EQUAL(fixed_point_to_int(-256), -1);    // -1.0
	BOOST_CHECK_EQUAL(fixed_point_to_int(-512), -2);    // -2.0
}

// ==================== rounded_division tests ====================

BOOST_AUTO_TEST_CASE(test_rounded_division_exact)
{
	BOOST_CHECK_EQUAL(rounded_division(10, 5), 2);
	BOOST_CHECK_EQUAL(rounded_division(15, 3), 5);
}

BOOST_AUTO_TEST_CASE(test_rounded_division_round_down)
{
	BOOST_CHECK_EQUAL(rounded_division(10, 4), 2);  // 10/4 = 2.5, rounds to 3
	BOOST_CHECK_EQUAL(rounded_division(7, 3), 2);   // 7/3 = 2.33, rounds to 2
}

// ==================== const_clone tests ====================

BOOST_AUTO_TEST_CASE(test_const_clone_type)
{
	static_assert(std::is_same_v<utils::const_clone_t<int, int>, int>,
		"Non-const source should give non-const destination");
	static_assert(std::is_same_v<utils::const_clone_t<int, const int>, const int>,
		"Const source should give const destination");
	static_assert(std::is_same_v<utils::const_clone_t<int, const int&>, const int>,
		"Const reference source should give const destination");
	static_assert(std::is_same_v<utils::const_clone_t<int, int&>, int>,
		"Non-const reference source should give non-const destination");
}

BOOST_AUTO_TEST_CASE(test_const_clone_pointer)
{
	static_assert(std::is_same_v<utils::const_clone_ptr<int, int>, int*>,
		"Non-const source pointer");
	static_assert(std::is_same_v<utils::const_clone_ptr<int, const int>, const int*>,
		"Const source pointer");
}

// ==================== utils::span tests ====================

BOOST_AUTO_TEST_CASE(test_span_from_array)
{
	int arr[] = {1, 2, 3, 4, 5};
	utils::span<int> s(arr);
	BOOST_CHECK_EQUAL(s.size(), 5u);
	BOOST_CHECK_EQUAL(s[0], 1);
	BOOST_CHECK_EQUAL(s[4], 5);
}

BOOST_AUTO_TEST_CASE(test_span_from_vector)
{
	std::vector<int> vec = {10, 20, 30};
	utils::span<int> s(vec);
	BOOST_CHECK_EQUAL(s.size(), 3u);
	BOOST_CHECK_EQUAL(s[0], 10);
	BOOST_CHECK_EQUAL(s[1], 20);
	BOOST_CHECK_EQUAL(s[2], 30);
}

BOOST_AUTO_TEST_CASE(test_span_empty)
{
	utils::span<int> s;
	BOOST_CHECK(s.empty());
	BOOST_CHECK_EQUAL(s.size(), 0u);
}

BOOST_AUTO_TEST_CASE(test_span_subspan)
{
	int arr[] = {1, 2, 3, 4, 5};
	utils::span<int> s(arr);
	auto sub = s.subspan(1, 3);
	BOOST_CHECK_EQUAL(sub.size(), 3u);
	BOOST_CHECK_EQUAL(sub[0], 2);
	BOOST_CHECK_EQUAL(sub[1], 3);
	BOOST_CHECK_EQUAL(sub[2], 4);
}

BOOST_AUTO_TEST_CASE(test_span_first_last)
{
	int arr[] = {1, 2, 3, 4, 5};
	utils::span<int> s(arr);

	auto first3 = s.first(3);
	BOOST_CHECK_EQUAL(first3.size(), 3u);
	BOOST_CHECK_EQUAL(first3[0], 1);
	BOOST_CHECK_EQUAL(first3[2], 3);

	auto last2 = s.last(2);
	BOOST_CHECK_EQUAL(last2.size(), 2u);
	BOOST_CHECK_EQUAL(last2[0], 4);
	BOOST_CHECK_EQUAL(last2[1], 5);
}

// ==================== charconv utils tests ====================

BOOST_AUTO_TEST_CASE(test_stod_basic)
{
	BOOST_CHECK_CLOSE(utils::stod("3.14"), 3.14, 0.001);
	BOOST_CHECK_CLOSE(utils::stod("-2.5"), -2.5, 0.001);
	BOOST_CHECK_CLOSE(utils::stod("0.0"), 0.0, 0.001);
}

BOOST_AUTO_TEST_CASE(test_stod_with_whitespace)
{
	BOOST_CHECK_CLOSE(utils::stod("  3.14"), 3.14, 0.001);
	BOOST_CHECK_CLOSE(utils::stod("+2.5"), 2.5, 0.001);
}

BOOST_AUTO_TEST_CASE(test_stoi_basic)
{
	BOOST_CHECK_EQUAL(utils::stoi("42"), 42);
	BOOST_CHECK_EQUAL(utils::stoi("-17"), -17);
	BOOST_CHECK_EQUAL(utils::stoi("0"), 0);
}

BOOST_AUTO_TEST_CASE(test_stoi_with_whitespace)
{
	BOOST_CHECK_EQUAL(utils::stoi("  42"), 42);
	BOOST_CHECK_EQUAL(utils::stoi("+100"), 100);
}

// ==================== decayed_is_same tests ====================

BOOST_AUTO_TEST_CASE(test_decayed_is_same_basic)
{
	static_assert(utils::decayed_is_same<int, int>, "Same types should match");
	static_assert(utils::decayed_is_same<int, int&>, "Reference should decay");
	static_assert(utils::decayed_is_same<int, const int>, "Const should decay");
	static_assert(utils::decayed_is_same<int, const int&>, "Const reference should decay");
	static_assert(!utils::decayed_is_same<int, double>, "Different types should not match");
}

// ==================== charconv_buffer tests ====================

BOOST_AUTO_TEST_CASE(test_charconv_buffer_int)
{
	utils::charconv_buffer<int> buf(42);
	BOOST_CHECK_EQUAL(buf.to_string(), "42");
	BOOST_CHECK_EQUAL(buf.get_view(), "42");
}

BOOST_AUTO_TEST_CASE(test_charconv_buffer_negative)
{
	utils::charconv_buffer<int> buf(-123);
	BOOST_CHECK_EQUAL(buf.to_string(), "-123");
}

BOOST_AUTO_TEST_CASE(test_charconv_buffer_zero)
{
	utils::charconv_buffer<int> buf(0);
	BOOST_CHECK_EQUAL(buf.to_string(), "0");
}

// ==================== utils::ranges::find tests ====================

BOOST_AUTO_TEST_CASE(test_ranges_find_with_projection)
{
	struct Item {
		int id;
		std::string name;
	};

	std::vector<Item> items = {{1, "a"}, {2, "b"}, {3, "c"}};
	auto it = utils::ranges::find(items, 2, &Item::id);
	BOOST_CHECK(it != items.end());
	BOOST_CHECK_EQUAL(it->name, "b");

	auto not_found = utils::ranges::find(items, 99, &Item::id);
	BOOST_CHECK(not_found == items.end());
}

// ==================== overload tests ====================

BOOST_AUTO_TEST_CASE(test_overload_lambda)
{
	auto visitor = utils::overload(
		[](int i) { return i * 2; },
		[](const std::string& s) { return static_cast<int>(s.size()); }
	);

	BOOST_CHECK_EQUAL(visitor(5), 10);
	BOOST_CHECK_EQUAL(visitor(std::string("hello")), 5);
}

BOOST_AUTO_TEST_SUITE_END()
