// This file is part of prometheus
// Copyright (C) 2022-2025 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <prometheus/version-core.hpp>

// =========================================================
// COMPILER
// =========================================================

#if __has_cpp_attribute(assume)
#define PROMETHEUS_COMPILER_ASSUME(expression, ...)                          \
	do                                                                       \
	{                                                                        \
		const auto assume_expression_result = static_cast<bool>(expression); \
		[[assume(assume_expression_result)]];                                \
	} while (false)
#else
#define PROMETHEUS_COMPILER_ASSUME(expression, ...)   \
	do                                                \
	{                                                 \
		PROMETHEUS_SEMANTIC_IF_NOT_CONSTANT_EVALUATED \
		{                                             \
			if (not static_cast<bool>(expression))    \
			{                                         \
				PROMETHEUS_COMPILER_UNREACHABLE();    \
			}                                         \
		}                                             \
	} while (false)
#endif

// =========================================================
// SEMANTIC
// =========================================================

#define PROMETHEUS_SEMANTIC_STATIC_UNREACHABLE(...)                                                                \
	[]<bool AlwaysFalse = false>()                                                                         \
	{                                                                                                      \
		static_assert(AlwaysFalse, "[UNREACHABLE BRANCH]" __VA_OPT__(":\"") __VA_ARGS__ __VA_OPT__("\"")); \
	}();                                                                                                   \
	PROMETHEUS_COMPILER_UNREACHABLE()

// #if defined(__cpp_if_consteval) and __cpp_if_consteval >= 202106L
// #define PROMETHEUS_SEMANTIC_IF_CONSTANT_EVALUATED if consteval
// #define PROMETHEUS_SEMANTIC_IF_NOT_CONSTANT_EVALUATED if not consteval
// #else
#define PROMETHEUS_SEMANTIC_IF_CONSTANT_EVALUATED if (std::is_constant_evaluated())
#define PROMETHEUS_SEMANTIC_IF_NOT_CONSTANT_EVALUATED if (not std::is_constant_evaluated())
// #endif

#if __has_cpp_attribute(assume)
#define PROMETHEUS_ERROR_ASSUME(expression, ...)                         \
	do                                                                       \
	{                                                                        \
		const auto assume_expression_result = static_cast<bool>(expression); \
		[[assume(assume_expression_result)]];                                \
	} while (false)
#else
#define PROMETHEUS_ERROR_ASSUME(expression, ...)      \
	do                                                    \
	{                                                     \
		PROMETHEUS_SEMANTIC_IF_NOT_CONSTANT_EVALUATED \
		{                                                 \
			if (not static_cast<bool>(expression))        \
			{                                             \
				PROMETHEUS_COMPILER_UNREACHABLE();       \
			}                                             \
		}                                                 \
	} while (false)
#endif

// =========================================================
// UTILITY
// =========================================================

#define PROMETHEUS_UTILITY_PRIVATE_STRING_CAT(lhs, rhs) lhs## rhs
#define PROMETHEUS_UTILITY_STRING_CAT(lhs, rhs) PROMETHEUS_UTILITY_PRIVATE_STRING_CAT(lhs, rhs)

#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_0(_0, ...) _0
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_1(_0, _1, ...) _1
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_2(_0, _1, _2, ...) _2
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_3(_0, _1, _2, _3, ...) _3
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_4(_0, _1, _2, _3, _4, ...) _4
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_5(_0, _1, _2, _3, _4, _5, ...) _5
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_6(_0, _1, _2, _3, _4, _5, _6, ...) _6
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_7(_0, _1, _2, _3, _4, _5, _6, _7, ...) _7
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_8(_0, _1, _2, _3, _4, _5, _6, _7, _8, ...) _8
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_9(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, ...) _9
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_10(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, ...) _10
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_11(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, ...) _11
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_12(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, ...) _12
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_13(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, ...) _13
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_14(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, ...) _14
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_15(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) _15
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, ...) _16
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_17(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, ...) _17
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_18(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, ...) _18
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_19(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, ...) _19
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_20(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, ...) _20
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_21(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, ...) _21
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_22(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, ...) _22
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_23(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, ...) _23
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_24(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, ...) _24
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_25(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, ...) _25
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_26(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, ...) _26
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_27(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, ...) _27
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_28(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, ...) _28
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_29(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, ...) _29
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_30(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, ...) _30
#define PROMETHEUS_UTILITY_PRIVATE_ARGS_N_31(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, ...) _31

#define PROMETHEUS_UTILITY_ARGS_N(n, ...) PROMETHEUS_UTILITY_STRING_CAT(PROMETHEUS_UTILITY_PRIVATE_ARGS_N_, n)(__VA_ARGS__)
#define PROMETHEUS_UTILITY_ARGS_LEN(...) PROMETHEUS_UTILITY_ARGS_N(31, __VA_ARGS__ __VA_OPT__(, ) 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_1(_0) #_0
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_2(_0, _1) #_0 #_1
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_3(_0, _1, _2) #_0 #_1 #_2
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_4(_0, _1, _2, _3) #_0, #_1, #_2, #_3
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_5(_0, _1, _2, _3, _4) #_0 #_1 #_2 #_3 #_4
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_6(_0, _1, _2, _3, _4, _5) #_0 #_1 #_2 #_3 #_4 #_5
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_7(_0, _1, _2, _3, _4, _5, _6) #_0 #_1 #_2 #_3 #_4 #_5 #_6
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_8(_0, _1, _2, _3, _4, _5, _6, _7) #_0 #_1 #_2 #_3 #_4 #_5 #_6 #_7
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_9(_0, _1, _2, _3, _4, _5, _6, _7, _8) #_0 #_1 #_2 #_3 #_4 #_5 #_6 #_7 #_8
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_10(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9) #_0 #_1 #_2 #_3 #_4 #_5 #_6, #_7 #_8 #_9
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_11(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) #_0 #_1 #_2 #_3 #_4 #_5 #_6, #_7 #_8 #_9 #_10
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_12(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) #_0 #_1 #_2 #_3 #_4 #_5 #_6, #_7 #_8 #_9 #_10 #_11
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_13(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) #_0 #_1 #_2 #_3 #_4 #_5 #_6, #_7 #_8 #_9 #_10 #_11 #_12
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_14(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13) #_0 #_1 #_2 #_3 #_4 #_5 #_6, #_7 #_8 #_9 #_10 #_11 #_12 #_13
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_15(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14) #_0 #_1 #_2 #_3 #_4 #_5 #_6, #_7 #_8 #_9 #_10 #_11 #_12 #_13 #_14
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15) #_0 #_1 #_2 #_3 #_4 #_5 #_6, #_7 #_8 #_9 #_10 #_11 #_12 #_13 #_14 #_15
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_17(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16) #_0 #_1 #_2 #_3 #_4 #_5 #_6, #_7 #_8 #_9 #_10 #_11 #_12 #_13 #_14 #_15 #_16
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_18(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17) #_0 #_1 #_2 #_3 #_4 #_5 #_6, #_7 #_8 #_9 #_10 #_11 #_12 #_13 #_14 #_15 #_16 #_17
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_19(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18) #_0 #_1 #_2 #_3 #_4 #_5 #_6, #_7 #_8 #_9 #_10 #_11 #_12 #_13 #_14 #_15 #_16 #_17 #_18
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_20(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19) #_0 #_1 #_2 #_3 #_4 #_5 #_6, #_7 #_8 #_9 #_10 #_11 #_12 #_13 #_14 #_15 #_16 #_17 #_18 #_19
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_21(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20) #_0 #_1 #_2 #_3 #_4 #_5 #_6, #_7 #_8 #_9 #_10 #_11 #_12 #_13 #_14 #_15 #_16 #_17 #_18 #_19 #_20
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_22(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21) #_0 #_1 #_2 #_3 #_4 #_5 #_6, #_7 #_8 #_9 #_10 #_11 #_12 #_13 #_14 #_15 #_16 #_17 #_18 #_19 #_20 #_21
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_23(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22) #_0 #_1 #_2 #_3 #_4 #_5 #_6, #_7 #_8 #_9 #_10 #_11 #_12 #_13 #_14 #_15 #_16 #_17 #_18 #_19 #_20 #_21 #_22
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_24(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23) #_0 #_1 #_2 #_3 #_4 #_5 #_6, #_7 #_8 #_9 #_10 #_11 #_12 #_13 #_14 #_15 #_16 #_17 #_18 #_19 #_20 #_21 #_22 #_23
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_25(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24) #_0 #_1 #_2 #_3 #_4 #_5 #_6, #_7 #_8 #_9 #_10 #_11 #_12 #_13 #_14 #_15 #_16 #_17 #_18 #_19 #_20 #_21 #_22 #_23 #_24
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_26(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25) #_0 #_1 #_2 #_3 #_4 #_5 #_6, #_7 #_8 #_9 #_10 #_11 #_12 #_13 #_14 #_15 #_16 #_17 #_18 #_19 #_20 #_21 #_22 #_23 #_24 #_25
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_27(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26) #_0 #_1 #_2 #_3 #_4 #_5 #_6, #_7 #_8 #_9 #_10 #_11 #_12 #_13 #_14 #_15 #_16 #_17 #_18 #_19 #_20 #_21 #_22 #_23 #_24 #_25 #_26
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_28(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27) #_0 #_1 #_2 #_3 #_4 #_5 #_6, #_7 #_8 #_9 #_10 #_11 #_12 #_13 #_14 #_15 #_16 #_17 #_18 #_19 #_20 #_21 #_22 #_23 #_24 #_25 #_26 #_27
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_29(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28) #_0 #_1 #_2 #_3 #_4 #_5 #_6, #_7 #_8 #_9 #_10 #_11 #_12 #_13 #_14 #_15 #_16 #_17 #_18 #_19 #_20 #_21 #_22 #_23 #_24 #_25 #_26 #_27 #_28
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_30(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29) #_0 #_1 #_2 #_3 #_4 #_5 #_6, #_7 #_8 #_9 #_10 #_11 #_12 #_13 #_14 #_15 #_16 #_17 #_18 #_19 #_20 #_21 #_22 #_23 #_24 #_25 #_26 #_27 #_28 #_29
#define PROMETHEUS_UTILITY_PRIVATE_TO_STRING_31(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30) #_0 #_1 #_2 #_3 #_4 #_5 #_6, #_7 #_8 #_9 #_10 #_11 #_12 #_13 #_14 #_15 #_16 #_17 #_18 #_19 #_20 #_21 #_22 #_23 #_24 #_25 #_26 #_27 #_28 #_29 #_30

#define PROMETHEUS_UTILITY_TO_STRING(...) PROMETHEUS_UTILITY_STRING_CAT(PROMETHEUS_UTILITY_PRIVATE_TO_STRING_, PROMETHEUS_UTILITY_ARGS_LEN(__VA_ARGS__))(__VA_ARGS__)

// =========================================================
// MODULE: prometheus.meta.string
// =========================================================

#define PROMETHEUS_META_PRIVATE_DO_GENERATE_STRING_CHAR_ARRAY(string_type, this_string, string_length, begin_index) \
[]<std::size_t... Index>(std::index_sequence<Index...>) constexpr noexcept                       \
	{ \
		return ::prometheus::meta::string_type<                                                       \
			[](std::size_t index) constexpr noexcept                                               \
			{                                                                                      \
				return (this_string)[(begin_index) + index];                                            \
			}(Index)...>{};\
	}(std::make_index_sequence<string_length>{})

#define PROMETHEUS_META_PRIVATE_STRING_CHAR_ARRAY(string_type, string) PROMETHEUS_META_PRIVATE_DO_GENERATE_STRING_CHAR_ARRAY(string_type, string, sizeof(string) / sizeof((string)[0]), 0)
#define PROMETHEUS_META_PRIVATE_STRING_CHAR_BILATERAL_ARRAY(string_type, inner_string_type, left_string, right_string) ::prometheus::meta::string_type<PROMETHEUS_META_PRIVATE_STRING_CHAR_ARRAY(inner_string_type, left_string), PROMETHEUS_META_PRIVATE_STRING_CHAR_ARRAY(inner_string_type, right_string)>
#define PROMETHEUS_META_PRIVATE_STRING_CHAR_SYMMETRY_ARRAY(string_type, inner_string_type, this_string) ::prometheus::meta::string_type<PROMETHEUS_META_PRIVATE_DO_GENERATE_STRING_CHAR_ARRAY(inner_string_type, this_string, sizeof(this_string) / sizeof((this_string)[0]) / 2, 0), PROMETHEUS_META_PRIVATE_DO_GENERATE_STRING_CHAR_ARRAY(inner_string_type, this_string, sizeof(this_string) / sizeof((this_string)[0]) / 2, sizeof(this_string) / sizeof((this_string)[0]) / 2)>

#define PROMETHEUS_META_STRING_CHAR_ARRAY(string) PROMETHEUS_META_PRIVATE_STRING_CHAR_ARRAY(char_array, string)
#define PROMETHEUS_META_STRING_WCHAR_ARRAY(string) PROMETHEUS_META_PRIVATE_STRING_CHAR_ARRAY(wchar_array, string)
// ReSharper disable once CppInconsistentNaming
#define PROMETHEUS_META_STRING_U8CHAR_ARRAY(string) PROMETHEUS_META_PRIVATE_STRING_CHAR_ARRAY(u8char_array, string)
// ReSharper disable once CppInconsistentNaming
#define PROMETHEUS_META_STRING_U16CHAR_ARRAY(string) PROMETHEUS_META_PRIVATE_STRING_CHAR_ARRAY(u16char_array, string)
// ReSharper disable once CppInconsistentNaming
#define PROMETHEUS_META_STRING_U32CHAR_ARRAY(string) PROMETHEUS_META_PRIVATE_STRING_CHAR_ARRAY(u32char_array, string)

// =========================================================
// MODULE: prometheus.platform.exception
// =========================================================

#define PROMETHEUS_PLATFORM_ASSUME_THROW(error_type, expression, message, ...) \
	do                                                                                  \
	{                                                                                   \
		if (not static_cast<bool>(expression))                                          \
		{                                                                               \
			if constexpr (__VA_OPT__(not) false)                                        \
			{                                                                           \
				throw error_type{std::format(message __VA_OPT__(, ) __VA_ARGS__)};      \
			}                                                                           \
			else                                                                        \
			{                                                                           \
				throw error_type{message};                                              \
			}                                                                           \
		}                                                                               \
	} while (false)

#define PROMETHEUS_PLATFORM_PANIC(error_type, message, ...)                                        \
	do                                                                                                      \
	{                                                                                                       \
		if constexpr (__VA_OPT__(not) false)                                                                \
		{                                                                                                   \
			::prometheus::platform::mob<error_type>::invoke<error_type>(std::format(message __VA_OPT__(, ) __VA_ARGS__)); \
		}                                                                                                   \
		else                                                                                                \
		{                                                                                                   \
			::prometheus::platform::mob<error_type>::invoke<error_type>(message);                                         \
		}                                                                                                   \
	} while (false)

#define PROMETHEUS_PLATFORM_PANIC_DATA(error_type, data, message, ...)                                   \
	do                                                                                                            \
	{                                                                                                             \
		if constexpr (__VA_OPT__(not) false)                                                                      \
		{                                                                                                         \
			::prometheus::platform::mob<error_type>::invoke<error_type>(std::format(message __VA_OPT__(, ) __VA_ARGS__), data); \
		}                                                                                                         \
		else                                                                                                      \
		{                                                                                                         \
			::prometheus::platform::mob<error_type>::invoke<error_type>(message, data);                                         \
		}                                                                                                         \
	} while (false)

#define PROMETHEUS_PLATFORM_ASSUME_PANIC(error_type, expression, message, ...) \
	do                                                                                  \
	{                                                                                   \
		if (not static_cast<bool>(expression))                                          \
		{                                                                               \
			PROMETHEUS_PLATFORM_PANIC(error_type, message, __VA_ARGS__);       \
		}                                                                               \
	} while (false)

#define PROMETHEUS_PLATFORM_ASSUME_PANIC_DATA(error_type, expression, data, message, ...) \
	do                                                                                             \
	{                                                                                              \
		if (not static_cast<bool>(expression))                                                     \
		{                                                                                          \
			PROMETHEUS_PLATFORM_PANIC_DATA(error_type, data, message, __VA_ARGS__);       \
		}                                                                                          \
	} while (false)

// =========================================================
// MODULE: prometheus.platform.os
// =========================================================

// INLINE breakpoint_if_debugging
#define PROMETHEUS_PLATFORM_BREAKPOINT_IF(expression, message)                                                                               \
	do                                                                                                                                        \
	{                                                                                                                                         \
		PROMETHEUS_SEMANTIC_IF_NOT_CONSTANT_EVALUATED                                                                                     \
		{                                                                                                                                     \
			if (static_cast<bool>(expression) and ::prometheus::platform::is_debugger_present())                                         \
			{                                                                                                                                 \
				::prometheus::platform::breakpoint_message("[" __FILE__ ":" PROMETHEUS_UTILITY_TO_STRING(__LINE__) "] -> " message); \
				PROMETHEUS_COMPILER_DEBUG_TRAP();                                                                                         \
			}                                                                                                                                 \
		}                                                                                                                                     \
	} while (false)

// INLINE breakpoint_or_terminate
#define PROMETHEUS_PLATFORM_BREAKPOINT_OR_TERMINATE_IF(expression, message)                                                                      \
	do                                                                                                                                            \
	{                                                                                                                                             \
		PROMETHEUS_SEMANTIC_IF_NOT_CONSTANT_EVALUATED                                                                                         \
		{                                                                                                                                         \
			if (static_cast<bool>(expression))                                                                                                    \
			{                                                                                                                                     \
				if (::prometheus::platform::is_debugger_present())                                                                           \
				{                                                                                                                                 \
					::prometheus::platform::breakpoint_message("[" __FILE__ ":" PROMETHEUS_UTILITY_TO_STRING(__LINE__) "] -> " message); \
					PROMETHEUS_COMPILER_DEBUG_TRAP();                                                                                         \
				}                                                                                                                                 \
				else                                                                                                                              \
				{                                                                                                                                 \
					std::terminate();                                                                                                             \
				}                                                                                                                                 \
			}                                                                                                                                     \
		}                                                                                                                                         \
	} while (false)

#define PROMETHEUS_PLATFORM_PRIVATE_DO_CHECK(debug_type, expression, ...) \
	PROMETHEUS_PLATFORM_BREAKPOINT_OR_TERMINATE_IF(not static_cast<bool>(expression), "[" debug_type "]: \"" __VA_ARGS__ "\" --> {" PROMETHEUS_UTILITY_TO_STRING(expression) "}")

#define PROMETHEUS_PLATFORM_ASSUME_TRAP(expression, ...) PROMETHEUS_PLATFORM_PRIVATE_DO_CHECK("ASSUME", expression __VA_OPT__(, ) __VA_ARGS__)

// DEBUG => DO_CHECK
// RELEASE => DO_ASSUME
#if PROMETHEUS_COMPILER_DEBUG

#define PROMETHEUS_PLATFORM_UNREACHABLE(...)                                        \
	PROMETHEUS_PLATFORM_PRIVATE_DO_CHECK("UNRECHABLE", false __VA_OPT__(, ) __VA_ARGS__); \
	PROMETHEUS_COMPILER_UNREACHABLE()

#define PROMETHEUS_PLATFORM_ASSUME PROMETHEUS_PLATFORM_ASSUME_TRAP

#else

#define PROMETHEUS_PLATFORM_UNREACHABLE PROMETHEUS_COMPILER_UNREACHABLE()

#define PROMETHEUS_PLATFORM_ASSUME PROMETHEUS_COMPILER_ASSUME

#endif
