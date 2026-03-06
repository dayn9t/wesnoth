/*
	Copyright (C) 2008 - 2025
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

#include "formula/formula.hpp"
#include "formula/callable.hpp"
#include "formula/tokenizer.hpp"
#include "formula/variant.hpp"

#include <cmath>
#include <memory>

using namespace wfl;

// Mock callable class for testing
class test_entity : public formula_callable {
public:
	test_entity(int health, int damage, const std::string& name)
		: health_(health), damage_(damage), name_(name)
	{}

private:
	variant get_value(const std::string& key) const override {
		if(key == "health") return variant(health_);
		if(key == "damage") return variant(damage_);
		if(key == "name") return variant(name_);
		if(key == "alive") return variant(health_ > 0);
		return variant();
	}

	void get_inputs(formula_input_vector& inputs) const override {
		add_input(inputs, "health");
		add_input(inputs, "damage");
		add_input(inputs, "name");
		add_input(inputs, "alive");
	}

	int health_;
	int damage_;
	std::string name_;
};

BOOST_AUTO_TEST_SUITE(formula_extended)

// ============================================================================
// Tokenizer Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_tokenizer_identifiers)
{
	using namespace wfl::tokenizer;

	const std::string test = "abc xyz _test identifier123";
	auto i1 = test.begin();
	auto i2 = test.end();

	token t = get_token(i1, i2);
	BOOST_CHECK_EQUAL(std::string(t.begin, t.end), "abc");
	BOOST_CHECK(t.type == token_type::identifier);

	t = get_token(i1, i2);
	BOOST_CHECK_EQUAL(std::string(t.begin, t.end), " ");
	BOOST_CHECK(t.type == token_type::whitespace);

	t = get_token(i1, i2);
	BOOST_CHECK_EQUAL(std::string(t.begin, t.end), "xyz");
	BOOST_CHECK(t.type == token_type::identifier);

	t = get_token(i1, i2);
	BOOST_CHECK_EQUAL(std::string(t.begin, t.end), " ");
	BOOST_CHECK(t.type == token_type::whitespace);

	t = get_token(i1, i2);
	BOOST_CHECK_EQUAL(std::string(t.begin, t.end), "_test");
	BOOST_CHECK(t.type == token_type::identifier);
}

BOOST_AUTO_TEST_CASE(test_tokenizer_numbers)
{
	using namespace wfl::tokenizer;

	const std::string test = "42 3.14 0 999.999";
	auto i1 = test.begin();
	auto i2 = test.end();

	token t = get_token(i1, i2);
	BOOST_CHECK_EQUAL(std::string(t.begin, t.end), "42");
	BOOST_CHECK(t.type == token_type::integer);

	t = get_token(i1, i2);
	BOOST_CHECK_EQUAL(std::string(t.begin, t.end), " ");
	BOOST_CHECK(t.type == token_type::whitespace);

	t = get_token(i1, i2);
	BOOST_CHECK_EQUAL(std::string(t.begin, t.end), "3.14");
	BOOST_CHECK(t.type == token_type::decimal);

	t = get_token(i1, i2);
	BOOST_CHECK_EQUAL(std::string(t.begin, t.end), " ");
	BOOST_CHECK(t.type == token_type::whitespace);

	t = get_token(i1, i2);
	BOOST_CHECK_EQUAL(std::string(t.begin, t.end), "0");
	BOOST_CHECK(t.type == token_type::integer);
}

BOOST_AUTO_TEST_CASE(test_tokenizer_operators)
{
	using namespace wfl::tokenizer;

	const std::string test = "+ - * / % = < > <= >= != .. .+ .-";
	auto i1 = test.begin();
	auto i2 = test.end();

	token t = get_token(i1, i2);
	BOOST_CHECK_EQUAL(std::string(t.begin, t.end), "+");
	BOOST_CHECK(t.type == token_type::operator_token);

	t = get_token(i1, i2);
	BOOST_CHECK_EQUAL(std::string(t.begin, t.end), " ");
	BOOST_CHECK(t.type == token_type::whitespace);

	t = get_token(i1, i2);
	BOOST_CHECK_EQUAL(std::string(t.begin, t.end), "-");
	BOOST_CHECK(t.type == token_type::operator_token);

	t = get_token(i1, i2);
	t = get_token(i1, i2); // skip space
	BOOST_CHECK_EQUAL(std::string(t.begin, t.end), "*");
	BOOST_CHECK(t.type == token_type::operator_token);

	t = get_token(i1, i2);
	t = get_token(i1, i2); // skip space
	BOOST_CHECK_EQUAL(std::string(t.begin, t.end), "/");
	BOOST_CHECK(t.type == token_type::operator_token);
}

BOOST_AUTO_TEST_CASE(test_tokenizer_keywords)
{
	using namespace wfl::tokenizer;

	const std::string test = "where and or not d def wfl wflend functions";
	auto i1 = test.begin();
	auto i2 = test.end();

	// 'where' is an operator_token
	token t = get_token(i1, i2);
	BOOST_CHECK_EQUAL(std::string(t.begin, t.end), "where");
	BOOST_CHECK(t.type == token_type::operator_token);

	t = get_token(i1, i2);
	t = get_token(i1, i2); // skip space
	BOOST_CHECK_EQUAL(std::string(t.begin, t.end), "and");
	BOOST_CHECK(t.type == token_type::operator_token);

	t = get_token(i1, i2);
	t = get_token(i1, i2);
	BOOST_CHECK_EQUAL(std::string(t.begin, t.end), "or");
	BOOST_CHECK(t.type == token_type::operator_token);

	t = get_token(i1, i2);
	t = get_token(i1, i2);
	BOOST_CHECK_EQUAL(std::string(t.begin, t.end), "not");
	BOOST_CHECK(t.type == token_type::operator_token);

	t = get_token(i1, i2);
	t = get_token(i1, i2);
	BOOST_CHECK_EQUAL(std::string(t.begin, t.end), "d");
	BOOST_CHECK(t.type == token_type::operator_token);
}

BOOST_AUTO_TEST_CASE(test_tokenizer_brackets)
{
	using namespace wfl::tokenizer;

	const std::string test = "([]) ->";
	auto i1 = test.begin();
	auto i2 = test.end();

	token t = get_token(i1, i2);
	BOOST_CHECK(t.type == token_type::lparens);

	t = get_token(i1, i2);
	BOOST_CHECK(t.type == token_type::lsquare);

	t = get_token(i1, i2);
	BOOST_CHECK(t.type == token_type::rsquare);

	t = get_token(i1, i2);
	BOOST_CHECK(t.type == token_type::rparens);

	t = get_token(i1, i2);
	BOOST_CHECK(t.type == token_type::whitespace);

	t = get_token(i1, i2);
	BOOST_CHECK(t.type == token_type::pointer);
	BOOST_CHECK_EQUAL(std::string(t.begin, t.end), "->");
}

BOOST_AUTO_TEST_CASE(test_tokenizer_string_literal)
{
	using namespace wfl::tokenizer;

	const std::string test = "'hello world' 'test[']s bracket'";
	auto i1 = test.begin();
	auto i2 = test.end();

	token t = get_token(i1, i2);
	BOOST_CHECK_EQUAL(std::string(t.begin, t.end), "'hello world'");
	BOOST_CHECK(t.type == token_type::string_literal);

	t = get_token(i1, i2);
	BOOST_CHECK(t.type == token_type::whitespace);

	t = get_token(i1, i2);
	BOOST_CHECK_EQUAL(std::string(t.begin, t.end), "'test[']s bracket'");
	BOOST_CHECK(t.type == token_type::string_literal);
}

// ============================================================================
// Expression Parsing and Evaluation Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_expression_arithmetic_precedence)
{
	BOOST_CHECK_EQUAL(formula("2+3*4").evaluate().as_int(), 14);
	BOOST_CHECK_EQUAL(formula("(2+3)*4").evaluate().as_int(), 20);
	BOOST_CHECK_EQUAL(formula("10-2-3").evaluate().as_int(), 5);
	BOOST_CHECK_EQUAL(formula("10/2/5").evaluate().as_int(), 1);
	BOOST_CHECK_EQUAL(formula("2^3^2").evaluate().as_int(), 512);
	BOOST_CHECK_EQUAL(formula("(2^3)^2").evaluate().as_int(), 64);
}

BOOST_AUTO_TEST_CASE(test_expression_decimal_arithmetic)
{
	BOOST_CHECK_EQUAL(formula("1.5 + 2.5").evaluate().as_decimal(), 4000);
	BOOST_CHECK_EQUAL(formula("10.0 / 4.0").evaluate().as_decimal(), 2500);
	BOOST_CHECK_EQUAL(formula("2.5 * 2").evaluate().as_decimal(), 5000);
	BOOST_CHECK_EQUAL(formula("7.5 - 2.5").evaluate().as_decimal(), 5000);
}

BOOST_AUTO_TEST_CASE(test_expression_comparison_operators)
{
	BOOST_CHECK_EQUAL(formula("5 > 3").evaluate().as_bool(), true);
	BOOST_CHECK_EQUAL(formula("5 < 3").evaluate().as_bool(), false);
	BOOST_CHECK_EQUAL(formula("5 >= 5").evaluate().as_bool(), true);
	BOOST_CHECK_EQUAL(formula("5 <= 4").evaluate().as_bool(), false);
	BOOST_CHECK_EQUAL(formula("5 = 5").evaluate().as_bool(), true);
	BOOST_CHECK_EQUAL(formula("5 != 4").evaluate().as_bool(), true);
}

BOOST_AUTO_TEST_CASE(test_expression_logical_operators)
{
	BOOST_CHECK_EQUAL(formula("1 and 1").evaluate().as_bool(), true);
	BOOST_CHECK_EQUAL(formula("1 and 0").evaluate().as_bool(), false);
	BOOST_CHECK_EQUAL(formula("0 or 1").evaluate().as_bool(), true);
	BOOST_CHECK_EQUAL(formula("0 or 0").evaluate().as_bool(), false);
	BOOST_CHECK_EQUAL(formula("not 0").evaluate().as_bool(), true);
	BOOST_CHECK_EQUAL(formula("not 1").evaluate().as_bool(), false);
}

BOOST_AUTO_TEST_CASE(test_expression_ternary_if)
{
	BOOST_CHECK_EQUAL(formula("if(1, 10, 20)").evaluate().as_int(), 10);
	BOOST_CHECK_EQUAL(formula("if(0, 10, 20)").evaluate().as_int(), 20);
	BOOST_CHECK_EQUAL(formula("if(5 > 3, 'yes', 'no')").evaluate().as_string(), "yes");
}

BOOST_AUTO_TEST_CASE(test_expression_switch)
{
	BOOST_CHECK_EQUAL(formula("switch(2, 1, 'one', 2, 'two', 'other')").evaluate().as_string(), "two");
	BOOST_CHECK_EQUAL(formula("switch(3, 1, 'one', 2, 'two', 'other')").evaluate().as_string(), "other");
}

// ============================================================================
// Callable Interface Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_callable_member_access)
{
	test_entity entity(100, 25, "Hero");

	BOOST_CHECK_EQUAL(formula("health").evaluate(entity).as_int(), 100);
	BOOST_CHECK_EQUAL(formula("damage").evaluate(entity).as_int(), 25);
	BOOST_CHECK_EQUAL(formula("name").evaluate(entity).as_string(), "Hero");
	BOOST_CHECK_EQUAL(formula("alive").evaluate(entity).as_bool(), true);
}

BOOST_AUTO_TEST_CASE(test_callable_with_expressions)
{
	test_entity entity(50, 15, "Warrior");

	BOOST_CHECK_EQUAL(formula("health + damage").evaluate(entity).as_int(), 65);
	BOOST_CHECK_EQUAL(formula("health * 2").evaluate(entity).as_int(), 100);
	BOOST_CHECK_EQUAL(formula("if(alive, damage, 0)").evaluate(entity).as_int(), 15);
}

BOOST_AUTO_TEST_CASE(test_map_formula_callable)
{
	map_formula_callable vars;
	vars.add("x", variant(10));
	vars.add("y", variant(20));
	vars.add("name", variant(std::string("test")));

	BOOST_CHECK_EQUAL(formula("x + y").evaluate(vars).as_int(), 30);
	BOOST_CHECK_EQUAL(formula("x * y").evaluate(vars).as_int(), 200);
	BOOST_CHECK_EQUAL(formula("name").evaluate(vars).as_string(), "test");
}

BOOST_AUTO_TEST_CASE(test_callable_with_fallback)
{
	map_formula_callable fallback;
	fallback.add("fallback_value", variant(999));

	map_formula_callable vars(fallback.fake_ptr());
	vars.add("own_value", variant(100));

	BOOST_CHECK_EQUAL(formula("own_value").evaluate(vars).as_int(), 100);
	BOOST_CHECK_EQUAL(formula("fallback_value").evaluate(vars).as_int(), 999);
}

// ============================================================================
// Variant Type Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_variant_null)
{
	variant v;
	BOOST_CHECK(v.is_null());
	BOOST_CHECK_EQUAL(v.as_int(), 0);
	BOOST_CHECK_EQUAL(v.as_int(42), 42); // fallback
}

BOOST_AUTO_TEST_CASE(test_variant_integer)
{
	variant v(42);
	BOOST_CHECK(v.is_int());
	BOOST_CHECK_EQUAL(v.as_int(), 42);
	BOOST_CHECK_EQUAL(v.as_bool(), true);
}

BOOST_AUTO_TEST_CASE(test_variant_decimal)
{
	variant v(3.14159, variant::DECIMAL_VARIANT);
	BOOST_CHECK(v.is_decimal());
	// Decimal is stored as integer * 1000
	BOOST_CHECK_EQUAL(v.as_decimal(), 3141);
}

BOOST_AUTO_TEST_CASE(test_variant_string)
{
	variant v(std::string("hello"));
	BOOST_CHECK(v.is_string());
	BOOST_CHECK_EQUAL(v.as_string(), "hello");
}

BOOST_AUTO_TEST_CASE(test_variant_list)
{
	std::vector<variant> items;
	items.emplace_back(1);
	items.emplace_back(2);
	items.emplace_back(3);

	variant v(items);
	BOOST_CHECK(v.is_list());
	BOOST_CHECK_EQUAL(v.num_elements(), 3);
	BOOST_CHECK_EQUAL(v[0].as_int(), 1);
	BOOST_CHECK_EQUAL(v[1].as_int(), 2);
	BOOST_CHECK_EQUAL(v[2].as_int(), 3);
}

BOOST_AUTO_TEST_CASE(test_variant_map)
{
	std::map<variant, variant> m;
	m[variant("a")] = variant(1);
	m[variant("b")] = variant(2);

	variant v(m);
	BOOST_CHECK(v.is_map());
	BOOST_CHECK_EQUAL(v.num_elements(), 2);
	BOOST_CHECK_EQUAL(v[variant("a")].as_int(), 1);
	BOOST_CHECK_EQUAL(v[variant("b")].as_int(), 2);
}

BOOST_AUTO_TEST_CASE(test_variant_comparison)
{
	variant a(5);
	variant b(10);
	variant c(5);

	BOOST_CHECK(a < b);
	BOOST_CHECK(a <= b);
	BOOST_CHECK(b > a);
	BOOST_CHECK(b >= a);
	BOOST_CHECK(a == c);
	BOOST_CHECK(a != b);
}

// ============================================================================
// Built-in Function Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_function_abs)
{
	BOOST_CHECK_EQUAL(formula("abs(5)").evaluate().as_int(), 5);
	BOOST_CHECK_EQUAL(formula("abs(-5)").evaluate().as_int(), 5);
	BOOST_CHECK_EQUAL(formula("abs(0)").evaluate().as_int(), 0);
}

BOOST_AUTO_TEST_CASE(test_function_min_max)
{
	BOOST_CHECK_EQUAL(formula("min(3, 5)").evaluate().as_int(), 3);
	BOOST_CHECK_EQUAL(formula("max(3, 5)").evaluate().as_int(), 5);
	BOOST_CHECK_EQUAL(formula("min(10, 2, 8, 4)").evaluate().as_int(), 2);
	BOOST_CHECK_EQUAL(formula("max(10, 2, 8, 4)").evaluate().as_int(), 10);
}

BOOST_AUTO_TEST_CASE(test_function_rounding)
{
	BOOST_CHECK_EQUAL(formula("floor(3.7)").evaluate().as_int(), 3);
	BOOST_CHECK_EQUAL(formula("ceil(3.2)").evaluate().as_int(), 4);
	BOOST_CHECK_EQUAL(formula("round(3.5)").evaluate().as_int(), 4);
	BOOST_CHECK_EQUAL(formula("trunc(3.9)").evaluate().as_int(), 3);
}

BOOST_AUTO_TEST_CASE(test_function_size_length)
{
	BOOST_CHECK_EQUAL(formula("size([1,2,3,4,5])").evaluate().as_int(), 5);
	BOOST_CHECK_EQUAL(formula("length('hello')").evaluate().as_int(), 5);
	BOOST_CHECK_EQUAL(formula("size([])").evaluate().as_int(), 0);
	BOOST_CHECK_EQUAL(formula("length('')").evaluate().as_int(), 0);
}

BOOST_AUTO_TEST_CASE(test_function_keys_values)
{
	variant dict = formula("['a' -> 1, 'b' -> 2]").evaluate();

	variant keys = formula("keys(['a' -> 1, 'b' -> 2])").evaluate();
	BOOST_CHECK_EQUAL(keys.num_elements(), 2);

	variant values = formula("values(['a' -> 1, 'b' -> 2])").evaluate();
	BOOST_CHECK_EQUAL(values.num_elements(), 2);
}

BOOST_AUTO_TEST_CASE(test_function_filter)
{
	variant result = formula("filter([1,2,3,4,5], value > 2)").evaluate();
	BOOST_CHECK_EQUAL(result.num_elements(), 3);
	BOOST_CHECK_EQUAL(result[0].as_int(), 3);
	BOOST_CHECK_EQUAL(result[1].as_int(), 4);
	BOOST_CHECK_EQUAL(result[2].as_int(), 5);
}

BOOST_AUTO_TEST_CASE(test_function_map)
{
	variant result = formula("map([1,2,3], value * 2)").evaluate();
	BOOST_CHECK_EQUAL(result.num_elements(), 3);
	BOOST_CHECK_EQUAL(result[0].as_int(), 2);
	BOOST_CHECK_EQUAL(result[1].as_int(), 4);
	BOOST_CHECK_EQUAL(result[2].as_int(), 6);
}

BOOST_AUTO_TEST_CASE(test_function_reduce_sum)
{
	BOOST_CHECK_EQUAL(formula("reduce([1,2,3,4], a + b)").evaluate().as_int(), 10);
	BOOST_CHECK_EQUAL(formula("sum([1,2,3,4,5])").evaluate().as_int(), 15);
	BOOST_CHECK_EQUAL(formula("sum([1,2,3], 100)").evaluate().as_int(), 106);
}

BOOST_AUTO_TEST_CASE(test_function_head_tail)
{
	variant list = formula("[1,2,3,4,5]").evaluate();

	BOOST_CHECK_EQUAL(formula("head([1,2,3])").evaluate().as_int(), 1);

	variant tail = formula("tail([1,2,3])").evaluate();
	BOOST_CHECK_EQUAL(tail.num_elements(), 2);
	BOOST_CHECK_EQUAL(tail[0].as_int(), 2);
	BOOST_CHECK_EQUAL(tail[1].as_int(), 3);
}

BOOST_AUTO_TEST_CASE(test_function_reverse_sort)
{
	variant reversed = formula("reverse([1,2,3])").evaluate();
	BOOST_CHECK_EQUAL(reversed[0].as_int(), 3);
	BOOST_CHECK_EQUAL(reversed[1].as_int(), 2);
	BOOST_CHECK_EQUAL(reversed[2].as_int(), 1);

	variant sorted = formula("sort([3,1,2])").evaluate();
	BOOST_CHECK_EQUAL(sorted[0].as_int(), 1);
	BOOST_CHECK_EQUAL(sorted[1].as_int(), 2);
	BOOST_CHECK_EQUAL(sorted[2].as_int(), 3);
}

BOOST_AUTO_TEST_CASE(test_function_string_operations)
{
	BOOST_CHECK_EQUAL(formula("str_upper('hello')").evaluate().as_string(), "HELLO");
	BOOST_CHECK_EQUAL(formula("str_lower('HELLO')").evaluate().as_string(), "hello");
	BOOST_CHECK_EQUAL(formula("'abc' .. 'def'").evaluate().as_string(), "abcdef");
	BOOST_CHECK_EQUAL(formula("starts_with('hello', 'he')").evaluate().as_bool(), true);
	BOOST_CHECK_EQUAL(formula("ends_with('hello', 'lo')").evaluate().as_bool(), true);
}

BOOST_AUTO_TEST_CASE(test_function_type_check)
{
	BOOST_CHECK_EQUAL(formula("type(42)").evaluate().as_string(), "int");
	BOOST_CHECK_EQUAL(formula("type('hello')").evaluate().as_string(), "string");
	BOOST_CHECK_EQUAL(formula("type([1,2,3])").evaluate().as_string(), "list");
	BOOST_CHECK_EQUAL(formula("type(['a'->1])").evaluate().as_string(), "map");
}

BOOST_AUTO_TEST_CASE(test_function_null)
{
	BOOST_CHECK(formula("null()").evaluate().is_null());
}

BOOST_AUTO_TEST_CASE(test_function_index_of)
{
	BOOST_CHECK_EQUAL(formula("index_of(3, [1,2,3,4,3])").evaluate().as_int(), 2);
	BOOST_CHECK_EQUAL(formula("index_of(99, [1,2,3])").evaluate().as_int(), -1);
}

BOOST_AUTO_TEST_CASE(test_function_contains)
{
	variant list = formula("[1,2,3,4,5]").evaluate();
	variant search = variant(3);
	BOOST_CHECK(list.contains(search));

	search = variant(99);
	BOOST_CHECK(!list.contains(search));
}

BOOST_AUTO_TEST_SUITE_END()
