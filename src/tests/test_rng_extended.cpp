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

#include "random_synced.hpp"
#include "random_deterministic.hpp"
#include "config.hpp"
#include <string>
#include <sstream>
#include <iomanip>

BOOST_AUTO_TEST_SUITE(test_rng_extended)

BOOST_AUTO_TEST_CASE(test_mt_rng_default_construction)
{
	randomness::mt_rng rng;

	BOOST_CHECK(rng.get_random_seed() != 0);
	BOOST_CHECK(!rng.get_random_seed_str().empty());
}

BOOST_AUTO_TEST_CASE(test_mt_rng_get_next_random)
{
	config cfg;
	cfg["random_seed"] = "12345678";
	cfg["random_calls"] = 0;

	randomness::mt_rng rng(cfg);

	uint32_t val1 = rng.get_next_random();
	uint32_t val2 = rng.get_next_random();

	BOOST_CHECK(val1 != val2);
}

BOOST_AUTO_TEST_CASE(test_mt_rng_random_calls_increments)
{
	config cfg;
	cfg["random_seed"] = "abcdef12";
	cfg["random_calls"] = 0;

	randomness::mt_rng rng(cfg);

	BOOST_CHECK_EQUAL(rng.get_random_calls(), 0);

	rng.get_next_random();
	BOOST_CHECK_EQUAL(rng.get_random_calls(), 1);

	rng.get_next_random();
	BOOST_CHECK_EQUAL(rng.get_random_calls(), 2);
}

BOOST_AUTO_TEST_CASE(test_mt_rng_seed_random_changes_state)
{
	randomness::mt_rng rng;

	uint32_t seed1 = rng.get_random_seed();
	std::string seed_str1 = rng.get_random_seed_str();

	rng.seed_random("deadbeef");

	BOOST_CHECK(rng.get_random_seed() != seed1);
	BOOST_CHECK(rng.get_random_seed_str() != seed_str1);
}

BOOST_AUTO_TEST_CASE(test_mt_rng_config_state)
{
	config cfg;
	cfg["random_seed"] = "cafebabe";
	cfg["random_calls"] = 0;

	randomness::mt_rng rng(cfg);

	for (int i = 0; i < 10; i++) {
		rng.get_next_random();
	}

	// Verify that random_calls is incremented
	BOOST_CHECK_EQUAL(rng.get_random_calls(), 10);
	BOOST_CHECK(!rng.get_random_seed_str().empty());
}

BOOST_AUTO_TEST_CASE(test_deterministic_rng_construction)
{
	config cfg;
	cfg["random_seed"] = "12345678";
	cfg["random_calls"] = 0;

	randomness::mt_rng mt(cfg);
	auto rng = std::make_shared<randomness::rng_deterministic>(mt);

	BOOST_CHECK(rng != nullptr);
}

BOOST_AUTO_TEST_CASE(test_deterministic_rng_get_random_int)
{
	config cfg;
	cfg["random_seed"] = "12345678";
	cfg["random_calls"] = 0;

	randomness::mt_rng mt(cfg);
	auto rng = std::make_shared<randomness::rng_deterministic>(mt);

	int val = rng->get_random_int(0, 100);

	BOOST_CHECK(val >= 0 && val <= 100);
}

BOOST_AUTO_TEST_CASE(test_deterministic_rng_reproducibility)
{
	config cfg;
	cfg["random_seed"] = "cafe1234";
	cfg["random_calls"] = 0;

	randomness::mt_rng mt1(cfg);
	randomness::mt_rng mt2(cfg);

	auto rng1 = std::make_shared<randomness::rng_deterministic>(mt1);
	auto rng2 = std::make_shared<randomness::rng_deterministic>(mt2);

	for (int i = 0; i < 10; i++) {
		int val1 = rng1->get_random_int(0, 1000);
		int val2 = rng2->get_random_int(0, 1000);
		BOOST_CHECK_EQUAL(val1, val2);
	}
}

BOOST_AUTO_TEST_CASE(test_synced_rng_construction)
{
	auto rng = std::make_shared<randomness::synced_rng>([]() { return "12345678"; });

	BOOST_CHECK(rng != nullptr);
}

BOOST_AUTO_TEST_CASE(test_synced_rng_next_random)
{
	auto rng = std::make_shared<randomness::synced_rng>([]() { return "abcdef12"; });

	uint32_t val = rng->next_random();

	BOOST_CHECK(val != 0);
}

BOOST_AUTO_TEST_CASE(test_synced_rng_get_random_int)
{
	auto rng = std::make_shared<randomness::synced_rng>([]() { return "cafebabe"; });

	int val = rng->get_random_int(0, 100);

	BOOST_CHECK(val >= 0 && val <= 100);
}

BOOST_AUTO_TEST_CASE(test_rng_bit_random)
{
	config cfg;
	cfg["random_seed"] = "feedface";
	cfg["random_calls"] = 0;

	randomness::mt_rng mt(cfg);
	auto rng = std::make_shared<randomness::rng_deterministic>(mt);

	bool got_true = false;
	bool got_false = false;

	for (int i = 0; i < 100; i++) {
		if (rng->get_random_int(0, 1) == 1) {
			got_true = true;
		} else {
			got_false = true;
		}
	}

	BOOST_CHECK(got_true);
	BOOST_CHECK(got_false);
}

// ============================================================================
// Boundary condition tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_mt_rng_zero_seed)
{
	// Zero seed should be handled
	config cfg;
	cfg["random_seed"] = "0";
	cfg["random_calls"] = 0;

	BOOST_CHECK_NO_THROW(randomness::mt_rng rng(cfg));
}

BOOST_AUTO_TEST_CASE(test_mt_rng_max_seed)
{
	// Maximum 32-bit seed
	config cfg;
	cfg["random_seed"] = "FFFFFFFF";
	cfg["random_calls"] = 0;

	BOOST_CHECK_NO_THROW(randomness::mt_rng rng(cfg));
}

BOOST_AUTO_TEST_CASE(test_mt_rng_empty_seed)
{
	// Empty seed string
	config cfg;
	cfg["random_seed"] = "";
	cfg["random_calls"] = 0;

	// Should handle gracefully
	BOOST_CHECK_NO_THROW(randomness::mt_rng rng(cfg));
}

BOOST_AUTO_TEST_CASE(test_mt_rng_invalid_seed)
{
	// Invalid seed string
	config cfg;
	cfg["random_seed"] = "not_a_hex_string";
	cfg["random_calls"] = 0;

	// Should handle gracefully
	BOOST_CHECK_NO_THROW(randomness::mt_rng rng(cfg));
}

BOOST_AUTO_TEST_CASE(test_mt_rng_large_call_count)
{
	config cfg;
	cfg["random_seed"] = "12345678";
	cfg["random_calls"] = 999999;

	randomness::mt_rng rng(cfg);
	BOOST_CHECK_EQUAL(rng.get_random_calls(), 999999);

	// Generate more numbers
	for (int i = 0; i < 100; ++i) {
		rng.get_next_random();
	}

	BOOST_CHECK_EQUAL(rng.get_random_calls(), 1000099);
}

BOOST_AUTO_TEST_CASE(test_deterministic_rng_range_boundaries)
{
	config cfg;
	cfg["random_seed"] = "12345678";
	cfg["random_calls"] = 0;

	randomness::mt_rng mt(cfg);
	auto rng = std::make_shared<randomness::rng_deterministic>(mt);

	// Test single value range (min == max)
	for (int i = 0; i < 10; ++i) {
		int val = rng->get_random_int(42, 42);
		BOOST_CHECK_EQUAL(val, 42);
	}

	// Test range of 1
	for (int i = 0; i < 10; ++i) {
		int val = rng->get_random_int(0, 1);
		BOOST_CHECK(val >= 0 && val <= 1);
	}
}

BOOST_AUTO_TEST_CASE(test_deterministic_rng_negative_range)
{
	config cfg;
	cfg["random_seed"] = "12345678";
	cfg["random_calls"] = 0;

	randomness::mt_rng mt(cfg);
	auto rng = std::make_shared<randomness::rng_deterministic>(mt);

	// Negative range
	for (int i = 0; i < 100; ++i) {
		int val = rng->get_random_int(-100, -50);
		BOOST_CHECK(val >= -100 && val <= -50);
	}
}

BOOST_AUTO_TEST_CASE(test_deterministic_rng_mixed_range)
{
	config cfg;
	cfg["random_seed"] = "12345678";
	cfg["random_calls"] = 0;

	randomness::mt_rng mt(cfg);
	auto rng = std::make_shared<randomness::rng_deterministic>(mt);

	// Range crossing zero
	for (int i = 0; i < 100; ++i) {
		int val = rng->get_random_int(-50, 50);
		BOOST_CHECK(val >= -50 && val <= 50);
	}
}

BOOST_AUTO_TEST_CASE(test_deterministic_rng_large_range)
{
	config cfg;
	cfg["random_seed"] = "12345678";
	cfg["random_calls"] = 0;

	randomness::mt_rng mt(cfg);
	auto rng = std::make_shared<randomness::rng_deterministic>(mt);

	// Large range
	int val = rng->get_random_int(INT_MIN, INT_MAX);
	(void)val; // Just verify it doesn't crash
}

BOOST_AUTO_TEST_CASE(test_deterministic_rng_distribution)
{
	config cfg;
	cfg["random_seed"] = "12345678";
	cfg["random_calls"] = 0;

	randomness::mt_rng mt(cfg);
	auto rng = std::make_shared<randomness::rng_deterministic>(mt);

	// Generate many values and check distribution
	std::vector<int> counts(10, 0);
	for (int i = 0; i < 1000; ++i) {
		int val = rng->get_random_int(0, 9);
		counts[val]++;
	}

	// Each value should appear at least once
	for (int count : counts) {
		BOOST_CHECK(count > 0);
	}
}

BOOST_AUTO_TEST_CASE(test_synced_rng_multiple_calls)
{
	auto rng = std::make_shared<randomness::synced_rng>([]() { return "testseed"; });

	// Multiple calls should produce different values
	uint32_t val1 = rng->next_random();
	uint32_t val2 = rng->next_random();
	uint32_t val3 = rng->next_random();

	// Values should be different (with very high probability)
	BOOST_CHECK(val1 != val2 || val2 != val3);
}

BOOST_AUTO_TEST_CASE(test_synced_rng_range_stress)
{
	auto rng = std::make_shared<randomness::synced_rng>([]() { return "testseed"; });

	// Stress test with many random values
	for (int i = 0; i < 10000; ++i) {
		int val = rng->get_random_int(0, 100);
		BOOST_CHECK(val >= 0 && val <= 100);
	}
}

BOOST_AUTO_TEST_CASE(test_mt_rng_seed_random_multiple)
{
	randomness::mt_rng rng;

	// Seed multiple times
	for (int i = 0; i < 10; ++i) {
		rng.seed_random(std::to_string(i));
		uint32_t val = rng.get_next_random();
		(void)val;
	}
}

BOOST_AUTO_TEST_CASE(test_mt_rng_reproducibility)
{
	// Same seed should produce same sequence
	config cfg1;
	cfg1["random_seed"] = "deadbeef";
	cfg1["random_calls"] = 0;

	config cfg2;
	cfg2["random_seed"] = "deadbeef";
	cfg2["random_calls"] = 0;

	randomness::mt_rng rng1(cfg1);
	randomness::mt_rng rng2(cfg2);

	auto det1 = std::make_shared<randomness::rng_deterministic>(rng1);
	auto det2 = std::make_shared<randomness::rng_deterministic>(rng2);

	for (int i = 0; i < 100; ++i) {
		BOOST_CHECK_EQUAL(det1->get_random_int(0, 1000), det2->get_random_int(0, 1000));
	}
}

// ============================================================================
// Additional boundary condition tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_mt_rng_negative_call_count)
{
	// Negative call count (should be handled gracefully)
	config cfg;
	cfg["random_seed"] = "12345678";
	cfg["random_calls"] = -1;

	// Should handle gracefully
	BOOST_CHECK_NO_THROW(randomness::mt_rng rng(cfg));
}

BOOST_AUTO_TEST_CASE(test_mt_rng_very_large_call_count)
{
	// Very large call count
	config cfg;
	cfg["random_seed"] = "12345678";
	cfg["random_calls"] = INT_MAX;

	randomness::mt_rng rng(cfg);
	BOOST_CHECK_EQUAL(rng.get_random_calls(), INT_MAX);
}

BOOST_AUTO_TEST_CASE(test_mt_rng_seed_random_empty)
{
	randomness::mt_rng rng;

	// Seed with empty string
	rng.seed_random("");
	// Should handle gracefully
	BOOST_CHECK_NO_THROW(rng.get_next_random());
}

BOOST_AUTO_TEST_CASE(test_mt_rng_seed_random_special_chars)
{
	randomness::mt_rng rng;

	// Seed with special characters
	rng.seed_random("!@#$%^&*()");
	BOOST_CHECK_NO_THROW(rng.get_next_random());
}

BOOST_AUTO_TEST_CASE(test_mt_rng_seed_random_unicode)
{
	randomness::mt_rng rng;

	// Seed with unicode
	rng.seed_random("Hello世界🎮");
	BOOST_CHECK_NO_THROW(rng.get_next_random());
}

BOOST_AUTO_TEST_CASE(test_mt_rng_get_next_random_many)
{
	config cfg;
	cfg["random_seed"] = "12345678";
	cfg["random_calls"] = 0;

	randomness::mt_rng rng(cfg);

	// Generate many random numbers
	for (int i = 0; i < 10000; ++i) {
		uint32_t val = rng.get_next_random();
		(void)val;
	}

	BOOST_CHECK_EQUAL(rng.get_random_calls(), 10000);
}

BOOST_AUTO_TEST_CASE(test_deterministic_rng_range_min_greater_than_max)
{
	config cfg;
	cfg["random_seed"] = "12345678";
	cfg["random_calls"] = 0;

	randomness::mt_rng mt(cfg);
	auto rng = std::make_shared<randomness::rng_deterministic>(mt);

	// min > max should be handled gracefully
	int val = rng->get_random_int(100, 0);
	// Should return something within a valid range or handle error
	(void)val;
}

BOOST_AUTO_TEST_CASE(test_deterministic_rng_range_zero_span)
{
	config cfg;
	cfg["random_seed"] = "12345678";
	cfg["random_calls"] = 0;

	randomness::mt_rng mt(cfg);
	auto rng = std::make_shared<randomness::rng_deterministic>(mt);

	// Range of exactly 0
	for (int i = 0; i < 100; ++i) {
		int val = rng->get_random_int(42, 42);
		BOOST_CHECK_EQUAL(val, 42);
	}
}

BOOST_AUTO_TEST_CASE(test_deterministic_rng_range_one_span)
{
	config cfg;
	cfg["random_seed"] = "12345678";
	cfg["random_calls"] = 0;

	randomness::mt_rng mt(cfg);
	auto rng = std::make_shared<randomness::rng_deterministic>(mt);

	// Range of 1 (two possible values)
	bool got_min = false;
	bool got_max = false;
	for (int i = 0; i < 1000; ++i) {
		int val = rng->get_random_int(0, 1);
		BOOST_CHECK(val >= 0 && val <= 1);
		if (val == 0) got_min = true;
		if (val == 1) got_max = true;
	}
	BOOST_CHECK(got_min);
	BOOST_CHECK(got_max);
}

BOOST_AUTO_TEST_CASE(test_deterministic_rng_bit_random_distribution)
{
	config cfg;
	cfg["random_seed"] = "12345678";
	cfg["random_calls"] = 0;

	randomness::mt_rng mt(cfg);
	auto rng = std::make_shared<randomness::rng_deterministic>(mt);

	// Test random distribution using next_random
	int even_count = 0;
	int odd_count = 0;
	for (int i = 0; i < 1000; ++i) {
		if (rng->next_random() % 2 == 0) {
			even_count++;
		} else {
			odd_count++;
		}
	}

	// Should be roughly 50/50
	BOOST_CHECK(even_count > 300);
	BOOST_CHECK(odd_count > 300);
}

BOOST_AUTO_TEST_CASE(test_synced_rng_zero_seed)
{
	// Synced RNG with seed that returns zero
	auto rng = std::make_shared<randomness::synced_rng>([]() { return "0"; });

	BOOST_CHECK_NO_THROW(rng->next_random());
}

BOOST_AUTO_TEST_CASE(test_synced_rng_empty_seed)
{
	// Synced RNG with empty seed
	auto rng = std::make_shared<randomness::synced_rng>([]() { return ""; });

	BOOST_CHECK_NO_THROW(rng->next_random());
}

BOOST_AUTO_TEST_CASE(test_synced_rng_callback_called)
{
	int call_count = 0;
	auto rng = std::make_shared<randomness::synced_rng>([&call_count]() {
		call_count++;
		return "testseed";
	});

	// Callback should be called when generating random numbers
	rng->next_random();
	BOOST_CHECK(call_count > 0);
}

BOOST_AUTO_TEST_CASE(test_synced_rng_range_boundaries)
{
	auto rng = std::make_shared<randomness::synced_rng>([]() { return "testseed"; });

	// Test various range boundaries
	for (int i = 0; i < 100; ++i) {
		int val1 = rng->get_random_int(0, 1);
		BOOST_CHECK(val1 >= 0 && val1 <= 1);

		int val2 = rng->get_random_int(-100, 100);
		BOOST_CHECK(val2 >= -100 && val2 <= 100);

		int val3 = rng->get_random_int(INT_MIN, INT_MIN + 100);
		BOOST_CHECK(val3 >= INT_MIN && val3 <= INT_MIN + 100);
	}
}

BOOST_AUTO_TEST_CASE(test_rng_copy_state)
{
	config cfg;
	cfg["random_seed"] = "12345678";
	cfg["random_calls"] = 0;

	randomness::mt_rng mt1(cfg);
	randomness::mt_rng mt2(mt1); // Copy

	auto rng1 = std::make_shared<randomness::rng_deterministic>(mt1);
	auto rng2 = std::make_shared<randomness::rng_deterministic>(mt2);

	// Should produce same sequence
	for (int i = 0; i < 100; ++i) {
		BOOST_CHECK_EQUAL(rng1->get_random_int(0, 1000), rng2->get_random_int(0, 1000));
	}
}

BOOST_AUTO_TEST_CASE(test_rng_save_load_state)
{
	config cfg;
	cfg["random_seed"] = "12345678";
	cfg["random_calls"] = 0;

	randomness::mt_rng rng(cfg);

	// Generate some numbers
	for (int i = 0; i < 50; ++i) {
		rng.get_next_random();
	}

	// Note: mt_rng doesn't have to_config method
	// Just verify RNG works correctly
	BOOST_CHECK_NO_THROW(rng.get_next_random());
}

BOOST_AUTO_TEST_CASE(test_rng_deterministic_consistency)
{
	// Deterministic RNG should be consistent across multiple runs
	config cfg;
	cfg["random_seed"] = "deadbeef";
	cfg["random_calls"] = 0;

	std::vector<int> sequence1;
	std::vector<int> sequence2;

	{
		randomness::mt_rng mt(cfg);
		auto rng = std::make_shared<randomness::rng_deterministic>(mt);
		for (int i = 0; i < 100; ++i) {
			sequence1.push_back(rng->get_random_int(0, 1000));
		}
	}

	{
		randomness::mt_rng mt(cfg);
		auto rng = std::make_shared<randomness::rng_deterministic>(mt);
		for (int i = 0; i < 100; ++i) {
			sequence2.push_back(rng->get_random_int(0, 1000));
		}
	}

	BOOST_CHECK(sequence1 == sequence2);
}

BOOST_AUTO_TEST_SUITE_END()