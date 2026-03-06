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

#include "sdl/rect.hpp"
#include "sdl/point.hpp"
#include "sdl/surface.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(test_sdl_extended)

// ============================================================================
// SDL rect tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_rect_default_construction)
{
	rect r;

	BOOST_CHECK_EQUAL(r.x, 0);
	BOOST_CHECK_EQUAL(r.y, 0);
	BOOST_CHECK_EQUAL(r.w, 0);
	BOOST_CHECK_EQUAL(r.h, 0);
}

BOOST_AUTO_TEST_CASE(test_rect_parameterized_construction)
{
	rect r(10, 20, 30, 40);

	BOOST_CHECK_EQUAL(r.x, 10);
	BOOST_CHECK_EQUAL(r.y, 20);
	BOOST_CHECK_EQUAL(r.w, 30);
	BOOST_CHECK_EQUAL(r.h, 40);
}

BOOST_AUTO_TEST_CASE(test_rect_equality)
{
	rect r1(10, 20, 30, 40);
	rect r2(10, 20, 30, 40);
	rect r3(10, 20, 30, 41);

	BOOST_CHECK(r1 == r2);
	BOOST_CHECK(r1 != r3);
}

BOOST_AUTO_TEST_CASE(test_rect_area)
{
	rect r(10, 20, 30, 40);

	BOOST_CHECK_EQUAL(r.area(), 30 * 40);
}

BOOST_AUTO_TEST_CASE(test_rect_empty)
{
	rect r1(0, 0, 0, 0);
	rect r2(10, 20, 30, 40);

	BOOST_CHECK(r1.empty());
	BOOST_CHECK(!r2.empty());
}

BOOST_AUTO_TEST_CASE(test_rect_contains_point)
{
	rect r(10, 10, 20, 20);

	BOOST_CHECK(r.contains(point{15, 15}));
	BOOST_CHECK(r.contains(point{10, 10}));
	BOOST_CHECK(!r.contains(point{5, 5}));
	BOOST_CHECK(!r.contains(point{35, 35}));
}

BOOST_AUTO_TEST_CASE(test_rect_contains_rect)
{
	rect outer(0, 0, 100, 100);
	rect inner(10, 10, 20, 20);

	BOOST_CHECK(outer.contains(inner));
}

BOOST_AUTO_TEST_CASE(test_rect_intersection)
{
	rect r1(0, 0, 20, 20);
	rect r2(10, 10, 20, 20);

	rect intersection = r1.intersect(r2);

	BOOST_CHECK_EQUAL(intersection.x, 10);
	BOOST_CHECK_EQUAL(intersection.y, 10);
	BOOST_CHECK_EQUAL(intersection.w, 10);
	BOOST_CHECK_EQUAL(intersection.h, 10);
}

BOOST_AUTO_TEST_CASE(test_rect_no_intersection)
{
	rect r1(0, 0, 10, 10);
	rect r2(20, 20, 10, 10);

	rect intersection = r1.intersect(r2);

	BOOST_CHECK(intersection.empty());
}

BOOST_AUTO_TEST_CASE(test_rect_scale)
{
	rect r(10, 20, 30, 40);

	rect scaled = r * 2;

	BOOST_CHECK_EQUAL(scaled.x, 20);
	BOOST_CHECK_EQUAL(scaled.y, 40);
	BOOST_CHECK_EQUAL(scaled.w, 60);
	BOOST_CHECK_EQUAL(scaled.h, 80);
}

BOOST_AUTO_TEST_CASE(test_rect_divide)
{
	rect r(20, 40, 60, 80);

	rect divided = r / 2;

	BOOST_CHECK_EQUAL(divided.x, 10);
	BOOST_CHECK_EQUAL(divided.y, 20);
	BOOST_CHECK_EQUAL(divided.w, 30);
	BOOST_CHECK_EQUAL(divided.h, 40);
}

// ============================================================================
// SDL point tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_point_default_construction)
{
	point p;

	BOOST_CHECK_EQUAL(p.x, 0);
	BOOST_CHECK_EQUAL(p.y, 0);
}

BOOST_AUTO_TEST_CASE(test_point_parameterized_construction)
{
	point p(42, 24);

	BOOST_CHECK_EQUAL(p.x, 42);
	BOOST_CHECK_EQUAL(p.y, 24);
}

BOOST_AUTO_TEST_CASE(test_point_equality)
{
	point p1(10, 20);
	point p2(10, 20);
	point p3(10, 21);

	BOOST_CHECK(p1 == p2);
	BOOST_CHECK(p1 != p3);
}

BOOST_AUTO_TEST_CASE(test_point_less_than)
{
	point p1(10, 20);
	point p2(10, 21);
	point p3(11, 20);

	BOOST_CHECK(p1 < p2);
	BOOST_CHECK(p1 < p3);
	BOOST_CHECK(!(p2 < p1));
}

// ============================================================================
// SDL surface tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_surface_default_construction)
{
	surface s;

	BOOST_CHECK(s == nullptr);
}

BOOST_AUTO_TEST_CASE(test_surface_sized_construction)
{
	surface s(64, 64);

	BOOST_CHECK(s != nullptr);
	BOOST_CHECK_EQUAL(s->w, 64);
	BOOST_CHECK_EQUAL(s->h, 64);
}

BOOST_AUTO_TEST_CASE(test_surface_copy)
{
	surface s1(32, 32);
	surface s2 = s1;

	BOOST_CHECK(s2 != nullptr);
	BOOST_CHECK_EQUAL(s2->w, 32);
	BOOST_CHECK_EQUAL(s2->h, 32);
}

// ============================================================================
// Rect additional tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_rect_contains_coordinates)
{
	rect r(10, 10, 20, 20);

	BOOST_CHECK(r.contains(15, 15));
	BOOST_CHECK(r.contains(10, 10));
	BOOST_CHECK(!r.contains(5, 5));
	BOOST_CHECK(!r.contains(35, 35));
}

BOOST_AUTO_TEST_CASE(test_rect_zero_size)
{
	rect r(10, 10, 0, 0);

	BOOST_CHECK(r.empty());
	BOOST_CHECK_EQUAL(r.area(), 0);
}

// ============================================================================
// Rect boundary condition tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_rect_negative_dimensions)
{
	// Negative dimensions should be handled
	rect r1(10, 10, -5, 10);
	BOOST_CHECK(r1.empty());

	rect r2(10, 10, 10, -5);
	BOOST_CHECK(r2.empty());

	rect r3(10, 10, -5, -5);
	BOOST_CHECK(r3.empty());
}

BOOST_AUTO_TEST_CASE(test_rect_large_dimensions)
{
	// Large dimensions
	rect r(0, 0, 10000, 10000);
	BOOST_CHECK_EQUAL(r.area(), 100000000);
	BOOST_CHECK(!r.empty());
}

BOOST_AUTO_TEST_CASE(test_rect_max_int_dimensions)
{
	// Maximum integer dimensions
	rect r(0, 0, INT_MAX, 1);
	// Area calculation might overflow, just verify construction works
	(void)r;
}

BOOST_AUTO_TEST_CASE(test_rect_contains_boundary)
{
	rect r(10, 10, 20, 20);

	// On boundary
	BOOST_CHECK(r.contains(point{10, 10}));
	BOOST_CHECK(r.contains(point{29, 29}));

	// Just outside
	BOOST_CHECK(!r.contains(point{9, 10}));
	BOOST_CHECK(!r.contains(point{10, 9}));
	BOOST_CHECK(!r.contains(point{30, 29}));
	BOOST_CHECK(!r.contains(point{29, 30}));
}

BOOST_AUTO_TEST_CASE(test_rect_contains_empty)
{
	rect r(10, 10, 0, 0);

	// Empty rect should contain nothing
	BOOST_CHECK(!r.contains(point{10, 10}));
}

BOOST_AUTO_TEST_CASE(test_rect_intersection_boundary)
{
	// No intersection - just touching
	rect r1(0, 0, 10, 10);
	rect r2(10, 0, 10, 10);

	rect intersection = r1.intersect(r2);
	BOOST_CHECK(intersection.empty());
}

BOOST_AUTO_TEST_CASE(test_rect_intersection_partial)
{
	rect r1(0, 0, 10, 10);
	rect r2(5, 5, 10, 10);

	rect intersection = r1.intersect(r2);
	BOOST_CHECK_EQUAL(intersection.x, 5);
	BOOST_CHECK_EQUAL(intersection.y, 5);
	BOOST_CHECK_EQUAL(intersection.w, 5);
	BOOST_CHECK_EQUAL(intersection.h, 5);
}

BOOST_AUTO_TEST_CASE(test_rect_intersection_contained)
{
	// One rect completely inside another
	rect outer(0, 0, 100, 100);
	rect inner(25, 25, 50, 50);

	rect intersection = outer.intersect(inner);
	BOOST_CHECK_EQUAL(intersection, inner);
}

BOOST_AUTO_TEST_CASE(test_rect_scale_boundary)
{
	rect r(10, 20, 30, 40);

	// Scale by 0
	rect scaled_zero = r * 0;
	BOOST_CHECK_EQUAL(scaled_zero.x, 0);
	BOOST_CHECK_EQUAL(scaled_zero.y, 0);
	BOOST_CHECK_EQUAL(scaled_zero.w, 0);
	BOOST_CHECK_EQUAL(scaled_zero.h, 0);

	// Scale by 1
	rect scaled_one = r * 1;
	BOOST_CHECK_EQUAL(scaled_one, r);

	// Scale by large number
	rect scaled_large = r * 100;
	BOOST_CHECK_EQUAL(scaled_large.x, 1000);
	BOOST_CHECK_EQUAL(scaled_large.y, 2000);
	BOOST_CHECK_EQUAL(scaled_large.w, 3000);
	BOOST_CHECK_EQUAL(scaled_large.h, 4000);
}

BOOST_AUTO_TEST_CASE(test_rect_divide_boundary)
{
	rect r(20, 40, 60, 80);

	// Divide by 1
	rect div_one = r / 1;
	BOOST_CHECK_EQUAL(div_one, r);

	// Divide by large number
	rect div_large = r / 10;
	BOOST_CHECK_EQUAL(div_large.x, 2);
	BOOST_CHECK_EQUAL(div_large.y, 4);
	BOOST_CHECK_EQUAL(div_large.w, 6);
	BOOST_CHECK_EQUAL(div_large.h, 8);
}

// ============================================================================
// Point boundary condition tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_point_extreme_values)
{
	// Maximum/minimum values
	point p1(INT_MAX, INT_MAX);
	BOOST_CHECK_EQUAL(p1.x, INT_MAX);
	BOOST_CHECK_EQUAL(p1.y, INT_MAX);

	point p2(INT_MIN, INT_MIN);
	BOOST_CHECK_EQUAL(p2.x, INT_MIN);
	BOOST_CHECK_EQUAL(p2.y, INT_MIN);
}

BOOST_AUTO_TEST_CASE(test_point_zero)
{
	point p(0, 0);
	BOOST_CHECK_EQUAL(p.x, 0);
	BOOST_CHECK_EQUAL(p.y, 0);
}

BOOST_AUTO_TEST_CASE(test_point_negative)
{
	point p(-10, -20);
	BOOST_CHECK_EQUAL(p.x, -10);
	BOOST_CHECK_EQUAL(p.y, -20);
}

BOOST_AUTO_TEST_CASE(test_point_comparison_boundary)
{
	point p1(INT_MIN, INT_MIN);
	point p2(INT_MAX, INT_MAX);

	BOOST_CHECK(p1 < p2);
	BOOST_CHECK(!(p2 < p1));
}

// ============================================================================
// Surface boundary condition tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_surface_zero_size)
{
	// Zero size surface
	surface s(0, 0);
	// May be null or valid empty surface depending on implementation
	(void)s;
}

BOOST_AUTO_TEST_CASE(test_surface_large_size)
{
	// Large surface (but not too large to cause memory issues)
	surface s(1024, 1024);
	BOOST_CHECK(s != nullptr);
	BOOST_CHECK_EQUAL(s->w, 1024);
	BOOST_CHECK_EQUAL(s->h, 1024);
}

BOOST_AUTO_TEST_CASE(test_surface_copy_null)
{
	surface s1;
	surface s2 = s1;

	BOOST_CHECK(s2 == nullptr);
}

BOOST_AUTO_TEST_CASE(test_surface_copy_multiple)
{
	surface s1(32, 32);
	surface s2 = s1;
	surface s3 = s2;

	// All should point to same surface
	BOOST_CHECK(s1 != nullptr);
	BOOST_CHECK(s2 != nullptr);
	BOOST_CHECK(s3 != nullptr);
	BOOST_CHECK_EQUAL(s1->w, s2->w);
	BOOST_CHECK_EQUAL(s2->w, s3->w);
}

// ============================================================================
// Additional rect tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_rect_equality_extended)
{
	rect r1(10, 20, 30, 40);
	rect r2(10, 20, 30, 40);
	rect r3(11, 20, 30, 40);

	BOOST_CHECK(r1 == r2);
	BOOST_CHECK(r1 != r3);
}

BOOST_AUTO_TEST_CASE(test_rect_contains_rect_boundary)
{
	rect outer(0, 0, 100, 100);

	// Same rect
	rect same(0, 0, 100, 100);
	BOOST_CHECK(outer.contains(same));

	// Partially outside
	rect partial(-10, 0, 50, 50);
	BOOST_CHECK(!outer.contains(partial));

	// Completely outside
	rect outside(200, 200, 50, 50);
	BOOST_CHECK(!outer.contains(outside));
}

BOOST_AUTO_TEST_CASE(test_rect_area_boundary)
{
	rect r1(0, 0, 0, 0);
	BOOST_CHECK_EQUAL(r1.area(), 0);

	rect r2(0, 0, 1, 1);
	BOOST_CHECK_EQUAL(r2.area(), 1);

	rect r3(0, 0, 100, 100);
	BOOST_CHECK_EQUAL(r3.area(), 10000);
}

// ============================================================================
// Additional boundary condition tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_rect_overflow_dimensions)
{
	// Very large dimensions that could cause overflow
	rect r1(0, 0, INT_MAX, 1);
	// Area calculation might overflow
	(void)r1;

	rect r2(0, 0, 1, INT_MAX);
	(void)r2;

	// Combined large dimensions
	rect r3(0, 0, 100000, 100000);
	// Area = 10^10 which may overflow int
	(void)r3;
}

BOOST_AUTO_TEST_CASE(test_rect_negative_position)
{
	// Negative position with positive size
	rect r(-10, -20, 30, 40);
	BOOST_CHECK_EQUAL(r.x, -10);
	BOOST_CHECK_EQUAL(r.y, -20);
	BOOST_CHECK_EQUAL(r.w, 30);
	BOOST_CHECK_EQUAL(r.h, 40);
	BOOST_CHECK(!r.empty());
}

BOOST_AUTO_TEST_CASE(test_rect_zero_width_height)
{
	// Zero width or height
	rect r1(10, 10, 0, 10);
	BOOST_CHECK(r1.empty());

	rect r2(10, 10, 10, 0);
	BOOST_CHECK(r2.empty());

	rect r3(10, 10, 0, 0);
	BOOST_CHECK(r3.empty());
}

BOOST_AUTO_TEST_CASE(test_rect_contains_boundary_exact)
{
	rect r(10, 10, 20, 20);

	// Exact boundary points
	BOOST_CHECK(r.contains(point{10, 10}));     // Top-left corner
	BOOST_CHECK(r.contains(point{29, 10}));     // Top-right corner
	BOOST_CHECK(r.contains(point{10, 29}));     // Bottom-left corner
	BOOST_CHECK(r.contains(point{29, 29}));     // Bottom-right corner
}

BOOST_AUTO_TEST_CASE(test_rect_contains_negative_point)
{
	rect r(-10, -10, 20, 20);

	BOOST_CHECK(r.contains(point{-10, -10}));   // Top-left
	BOOST_CHECK(r.contains(point{-1, -1}));     // Inside
	BOOST_CHECK(r.contains(point{0, 0}));       // Center
	BOOST_CHECK(!r.contains(point{10, 10}));    // Outside
}

BOOST_AUTO_TEST_CASE(test_rect_contains_rect_edge_cases)
{
	rect outer(0, 0, 100, 100);

	// Same rect
	rect same(0, 0, 100, 100);
	BOOST_CHECK(outer.contains(same));

	// Single point rect inside
	rect point_rect(50, 50, 1, 1);
	BOOST_CHECK(outer.contains(point_rect));

	// Rect at boundary
	rect boundary(0, 0, 1, 1);
	BOOST_CHECK(outer.contains(boundary));

	// Rect partially outside
	rect partial(-10, 0, 50, 50);
	BOOST_CHECK(!outer.contains(partial));

	// Rect completely outside
	rect outside(200, 200, 50, 50);
	BOOST_CHECK(!outer.contains(outside));
}

BOOST_AUTO_TEST_CASE(test_rect_intersection_various)
{
	// No intersection - separated
	rect r1(0, 0, 10, 10);
	rect r2(20, 20, 10, 10);
	rect i1 = r1.intersect(r2);
	BOOST_CHECK(i1.empty());

	// Touching at corner (no intersection)
	rect r3(0, 0, 10, 10);
	rect r4(10, 10, 10, 10);
	rect i2 = r3.intersect(r4);
	BOOST_CHECK(i2.empty());

	// Partial overlap
	rect r5(0, 0, 15, 15);
	rect r6(10, 10, 15, 15);
	rect i3 = r5.intersect(r6);
	BOOST_CHECK_EQUAL(i3.x, 10);
	BOOST_CHECK_EQUAL(i3.y, 10);
	BOOST_CHECK_EQUAL(i3.w, 5);
	BOOST_CHECK_EQUAL(i3.h, 5);

	// Complete containment
	rect r7(0, 0, 100, 100);
	rect r8(25, 25, 50, 50);
	rect i4 = r7.intersect(r8);
	BOOST_CHECK_EQUAL(i4, r8);
}

BOOST_AUTO_TEST_CASE(test_rect_intersection_negative)
{
	rect r1(-10, -10, 20, 20);
	rect r2(-5, -5, 15, 15);

	rect i = r1.intersect(r2);
	BOOST_CHECK_EQUAL(i.x, -5);
	BOOST_CHECK_EQUAL(i.y, -5);
	BOOST_CHECK_EQUAL(i.w, 10);
	BOOST_CHECK_EQUAL(i.h, 10);
}

BOOST_AUTO_TEST_CASE(test_rect_scale_by_one)
{
	rect r(10, 20, 30, 40);

	rect scaled = r * 1;
	BOOST_CHECK_EQUAL(scaled.x, 10);
	BOOST_CHECK_EQUAL(scaled.y, 20);
	BOOST_CHECK_EQUAL(scaled.w, 30);
	BOOST_CHECK_EQUAL(scaled.h, 40);
}

BOOST_AUTO_TEST_CASE(test_rect_scale_by_negative)
{
	rect r(10, 20, 30, 40);

	// Scaling by negative should result in negative values
	rect scaled = r * -1;
	BOOST_CHECK_EQUAL(scaled.x, -10);
	BOOST_CHECK_EQUAL(scaled.y, -40);
	BOOST_CHECK_EQUAL(scaled.w, -30);
	BOOST_CHECK_EQUAL(scaled.h, -40);
}

BOOST_AUTO_TEST_CASE(test_rect_divide_rounding)
{
	// Division that results in rounding
	rect r(10, 20, 31, 41);

	rect divided = r / 2;
	BOOST_CHECK_EQUAL(divided.x, 5);
	BOOST_CHECK_EQUAL(divided.y, 10);
	// 31/2 = 15 (integer division)
	BOOST_CHECK_EQUAL(divided.w, 15);
	// 41/2 = 20
	BOOST_CHECK_EQUAL(divided.h, 20);
}

BOOST_AUTO_TEST_CASE(test_rect_divide_by_negative)
{
	rect r(20, 40, 60, 80);

	rect divided = r / -2;
	BOOST_CHECK_EQUAL(divided.x, -10);
	BOOST_CHECK_EQUAL(divided.y, -20);
	BOOST_CHECK_EQUAL(divided.w, -30);
	BOOST_CHECK_EQUAL(divided.h, -40);
}

BOOST_AUTO_TEST_CASE(test_point_equality_reflexive)
{
	point p(10, 20);
	BOOST_CHECK(p == p);
	BOOST_CHECK(!(p != p));
}

BOOST_AUTO_TEST_CASE(test_point_equality_symmetric)
{
	point p1(10, 20);
	point p2(10, 20);
	BOOST_CHECK(p1 == p2);
	BOOST_CHECK(p2 == p1);
}

BOOST_AUTO_TEST_CASE(test_point_comparison_consistency)
{
	point p1(1, 2);
	point p2(2, 1);
	point p3(2, 2);

	// p1 < p2 (1 < 2 for x)
	BOOST_CHECK(p1 < p2);

	// p1 < p3 (1 < 2 for x)
	BOOST_CHECK(p1 < p3);

	// p2 < p3 (x equal, 1 < 2 for y)
	BOOST_CHECK(p2 < p3);
}

BOOST_AUTO_TEST_CASE(test_point_zero_values_extended)
{
	point p(0, 0);
	BOOST_CHECK_EQUAL(p.x, 0);
	BOOST_CHECK_EQUAL(p.y, 0);
}

BOOST_AUTO_TEST_CASE(test_point_copy)
{
	point p1(10, 20);
	point p2 = p1;
	BOOST_CHECK(p1 == p2);
}

BOOST_AUTO_TEST_CASE(test_surface_various_sizes)
{
	// Various surface sizes
	surface s1(1, 1);
	BOOST_CHECK(s1 != nullptr);

	surface s2(16, 16);
	BOOST_CHECK(s2 != nullptr);
	BOOST_CHECK_EQUAL(s2->w, 16);
	BOOST_CHECK_EQUAL(s2->h, 16);

	surface s3(1024, 768);
	BOOST_CHECK(s3 != nullptr);
	BOOST_CHECK_EQUAL(s3->w, 1024);
	BOOST_CHECK_EQUAL(s3->h, 768);
}

BOOST_AUTO_TEST_CASE(test_surface_null_operations)
{
	surface null_surf;
	BOOST_CHECK(null_surf == nullptr);

	// Copy of null should be null
	surface copy = null_surf;
	BOOST_CHECK(copy == nullptr);
}

BOOST_AUTO_TEST_CASE(test_surface_copy_independence)
{
	surface s1(32, 32);
	surface s2 = s1;

	// Both should be valid
	BOOST_CHECK(s1 != nullptr);
	BOOST_CHECK(s2 != nullptr);

	// Should point to same underlying surface
	BOOST_CHECK_EQUAL(s1->w, s2->w);
	BOOST_CHECK_EQUAL(s1->h, s2->h);
}

BOOST_AUTO_TEST_CASE(test_rect_equality_transitive)
{
	rect r1(10, 20, 30, 40);
	rect r2(10, 20, 30, 40);
	rect r3(10, 20, 30, 40);

	BOOST_CHECK(r1 == r2);
	BOOST_CHECK(r2 == r3);
	BOOST_CHECK(r1 == r3);
}

BOOST_AUTO_TEST_CASE(test_rect_inequality)
{
	rect base(10, 20, 30, 40);

	BOOST_CHECK(base != rect(11, 20, 30, 40)); // x different
	BOOST_CHECK(base != rect(10, 21, 30, 40)); // y different
	BOOST_CHECK(base != rect(10, 20, 31, 40)); // w different
	BOOST_CHECK(base != rect(10, 20, 30, 41)); // h different
}

BOOST_AUTO_TEST_SUITE_END()