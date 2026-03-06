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

BOOST_AUTO_TEST_SUITE(test_random_operations)

BOOST_AUTO_TEST_CASE(test_synced_rng_construction)
{
	randomness::synced_rng rng([]() { return "12345678"; });

	BOOST_CHECK_NO_THROW(rng.next_random());
}

BOOST_AUTO_TEST_CASE(test_synced_rng_range)
{
	randomness::synced_rng rng([]() { return "abcdef12"; });

	for (int i = 0; i < 100; i++) {
		int val = rng.get_random_int(0, 10);
		BOOST_CHECK(val >= 0 && val <= 10);
	}
}

BOOST_AUTO_TEST_CASE(test_synced_rng_bool)
{
	randomness::synced_rng rng([]() { return "cafebabe"; });

	bool got_true = false;
	bool got_false = false;

	for (int i = 0; i < 100; i++) {
		if (rng.get_random_int(0, 1) == 1) {
			got_true = true;
		} else {
			got_false = true;
		}
	}

	BOOST_CHECK(got_true);
	BOOST_CHECK(got_false);
}

BOOST_AUTO_TEST_CASE(test_deterministic_rng_construction)
{
	config cfg;
	cfg["random_seed"] = "12345678";
	cfg["random_calls"] = 0;

	randomness::mt_rng mt(cfg);
	randomness::rng_deterministic rng(mt);

	BOOST_CHECK_NO_THROW(rng.next_random());
}

BOOST_AUTO_TEST_CASE(test_deterministic_rng_range)
{
	config cfg;
	cfg["random_seed"] = "12345678";
	cfg["random_calls"] = 0;

	randomness::mt_rng mt(cfg);
	randomness::rng_deterministic rng(mt);

	for (int i = 0; i < 100; i++) {
		int val = rng.get_random_int(0, 10);
		BOOST_CHECK(val >= 0 && val <= 10);
	}
}

BOOST_AUTO_TEST_CASE(test_deterministic_rng_reproducibility)
{
	config cfg;
	cfg["random_seed"] = "deadbeef";
	cfg["random_calls"] = 0;

	randomness::mt_rng mt1(cfg);
	randomness::mt_rng mt2(cfg);

	randomness::rng_deterministic rng1(mt1);
	randomness::rng_deterministic rng2(mt2);

	for (int i = 0; i < 50; i++) {
		BOOST_CHECK_EQUAL(rng1.get_random_int(0, 1000), rng2.get_random_int(0, 1000));
	}
}

BOOST_AUTO_TEST_SUITE_END()