// This file is part of prometheus
// Copyright (C) 2022-2025 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <array>
#include <algorithm>
#include <limits>
#include <vector>
#include <ranges>

#include <prometheus/macro.hpp>

#include <prometheus/meta/name.hpp>

enum class PrometheusMetaEnumerationEnum123456789987654321: std::uint8_t
{
	_
};

namespace prometheus::meta
{
	namespace enumeration_detail
	{
		template<auto EnumValue>
			requires std::is_enum_v<std::decay_t<decltype(EnumValue)>>
		[[nodiscard]] constexpr auto name_of() noexcept -> std::string_view
		{
#if defined(PROMETHEUS_COMPILER_MSVC)
			// MSVC
			// class std::basic_string_view<char,struct std::char_traits<char> > `__calling_convention` `namespace`::get_full_function_name<`PrometheusMetaEnumerationEnum123456789987654321::_`>(void) noexcept
			constexpr std::string_view full_function_name = meta::get_full_function_name<PrometheusMetaEnumerationEnum123456789987654321::_>();
			constexpr auto full_function_name_size = full_function_name.size();

			constexpr std::string_view dummy_enum_value_name = "PrometheusMetaEnumerationEnum123456789987654321::_";
			constexpr auto dummy_enum_value_name_size = std::ranges::size(dummy_enum_value_name);

			// class std::basic_string_view<char,struct std::char_traits<char> > `__calling_convention` `namespace`::get_full_function_name<
			constexpr auto full_function_name_prefix_size = full_function_name.find(dummy_enum_value_name);
			static_assert(full_function_name_prefix_size != std::string_view::npos);

			// >(void) noexcept
			constexpr auto full_function_name_suffix_size = full_function_name_size - full_function_name_prefix_size - dummy_enum_value_name_size;
#elif defined(PROMETHEUS_COMPILER_CLANG) or defined(PROMETHEUS_COMPILER_CLANG_CL)
			// CLANG/CLANG-CL
			// std::string_view `namespace`::get_full_function_name() [V = `PrometheusMetaEnumerationEnum123456789987654321::_`]
			constexpr std::string_view full_function_name = meta::get_full_function_name<PrometheusMetaEnumerationEnum123456789987654321::_>();
			constexpr auto full_function_name_size = full_function_name.size();

			constexpr std::string_view dummy_struct_name{"PrometheusMetaEnumerationEnum123456789987654321::_"};
			constexpr auto dummy_struct_name_size = std::ranges::size(dummy_struct_name);

			// std::string_view `namespace`::get_full_function_name() [V =
			constexpr auto full_function_name_prefix_size = full_function_name.find(dummy_struct_name);
			static_assert(full_function_name_prefix_size != std::string_view::npos);

			// ]
			constexpr auto full_function_name_suffix_size = full_function_name_size - full_function_name_prefix_size - dummy_struct_name_size;
#else
			// GCC
			// constexpr std::string_view `namespace`::get_full_function_name() [with auto <anonymous> = `PrometheusMetaEnumerationEnum123456789987654321::_`; std::string_view = std::basic_string_view<char>]
			constexpr std::string_view full_function_name = meta::get_full_function_name<PrometheusMetaEnumerationEnum123456789987654321::_>();
			constexpr auto full_function_name_size = full_function_name.size();

			constexpr std::string_view dummy_struct_name{"PrometheusMetaEnumerationEnum123456789987654321::_"};
			constexpr auto dummy_struct_name_size = std::ranges::size(dummy_struct_name);

			// std::string_view `namespace`::get_full_function_name() [V =
			constexpr auto full_function_name_prefix_size = full_function_name.find(dummy_struct_name);
			static_assert(full_function_name_prefix_size != std::string_view::npos);

			// ; std::string_view = std::basic_string_view<char>]
			constexpr auto full_function_name_suffix_size = full_function_name_size - full_function_name_prefix_size - dummy_struct_name_size;
#endif

// #if defined(PROMETHEUS_COMPILER_APPLE_CLANG) or defined(PROMETHEUS_COMPILER_CLANG_CL) or defined(PROMETHEUS_COMPILER_CLANG)
// 			PROMETHEUS_COMPILER_DISABLE_WARNING_PUSH
// 			// clang-format off
// 			PROMETHEUS_COMPILER_DISABLE_WARNING(-Wenum-constexpr-conversion)
// 			// clang-format on
// #endif

			auto full_name = meta::get_full_function_name<EnumValue>();

// #if defined(PROMETHEUS_COMPILER_APPLE_CLANG) or defined(PROMETHEUS_COMPILER_CLANG_CL) or defined(PROMETHEUS_COMPILER_CLANG)
// 			PROMETHEUS_COMPILER_DISABLE_WARNING_POP
// #endif

			full_name.remove_prefix(full_function_name_prefix_size);
			full_name.remove_suffix(full_function_name_suffix_size);
			return full_name;
		}

		template<typename EnumType>
			requires std::is_enum_v<EnumType>
		[[nodiscard]] constexpr auto default_min_value() noexcept -> std::underlying_type_t<EnumType>
		{
			using value_type = std::underlying_type_t<EnumType>;

			if constexpr (std::is_signed_v<value_type>)
			{
				return static_cast<value_type>(-128);
			}
			else
			{
				return static_cast<value_type>(0);
			}
		}

		template<typename EnumType>
			requires std::is_enum_v<EnumType>
		[[nodiscard]] constexpr auto default_max_value() noexcept -> std::underlying_type_t<EnumType>
		{
			using value_type = std::underlying_type_t<EnumType>;

			if constexpr (std::is_signed_v<value_type>)
			{
				return static_cast<value_type>(127);
			}
			else
			{
				return static_cast<value_type>(255);
			}
		}

		template<typename>
		struct has_magic_enum_value : std::false_type {};

		template<typename T>
			requires requires { T::prometheus_magic_enum_flag; }
		struct has_magic_enum_value<T> : std::true_type
		{
			constexpr static auto magic = T::prometheus_magic_enum_flag;
		};

		template<typename T>
			requires requires { T::PrometheusMagicEnumFlag; }
		struct has_magic_enum_value<T> : std::true_type
		{
			constexpr static auto magic = T::PrometheusMagicEnumFlag;
		};

		template<typename T>
			requires requires { T::PROMETHEUS_MAGIC_ENUM_FLAG; }
		struct has_magic_enum_value<T> : std::true_type
		{
			constexpr static auto magic = T::PROMETHEUS_MAGIC_ENUM_FLAG;
		};

		template<typename T>
		constexpr auto has_magic_enum_value_v = has_magic_enum_value<T>::value;

		template<typename T>
		concept magic_enum_value_t = has_magic_enum_value_v<T>;
	}

	enum class EnumNamePolicy : std::uint8_t
	{
		// namespace_A::namespace_B::namespace_C::enum_name::Value // scoped enum
		// namespace_A::namespace_B::namespace_C::Value
		FULL,
		// enum_name::Value // scoped enum
		// Value
		WITH_SCOPED_NAME,
		// Value
		VALUE_ONLY,
	};

	namespace user_defined
	{
		/**
		 * template<>
		 * struct enum_name_policy<MyEnum>
		 * {
		 *		constexpr static auto value = EnumNamePolicy::VALUE_ONLY;
		 * };
		 */
		template<typename>
		struct enum_name_policy
		{
			constexpr static auto value = EnumNamePolicy::FULL;
		};

		/**
		 * template<>
		 * struct enum_range<MyEnum>
		 * {
		 *		constexpr static auto min = 0;
		 *		constexpr static auto max = 65535;
		 * };
		 * 
		 * note: Applies only to non-flag enumerations. If you do not specialize enum_range, any enumeration value outside the min/max range will be discarded.
		 */
		template<typename EnumType>
		struct enum_range
		{
			constexpr static auto min = enumeration_detail::default_min_value<EnumType>();
			constexpr static auto max = enumeration_detail::default_max_value<EnumType>();
		};

		/**
		 * template<>
		 * struct enum_is_flag<MyEnum> : std::true_type
		 * {
		 * };
		 *
		 * OR
		 *
		 * enum MyEnum
		 * {
		 *	E1 = 0,
		 *	E2 = 1,
		 *	E3 = 2,
		 *	...
		 *
		 *	prometheus_magic_enum_flag
		 *	// or
		 *	PrometheusMagicEnumFlag
		 *	// or
		 *	PROMETHEUS_MAGIC_ENUM_FLAG
		 * }
		 */
		template<typename>
		struct enum_is_flag : std::false_type {};

		template<enumeration_detail::magic_enum_value_t EnumType>
		struct enum_is_flag<EnumType> : std::true_type {};

		/**
		 * template<>
		 * struct enum_name<MyEnum>
		 * {
		 *		constexpr static std::string_view value{"MY-ENUM"};
		 * };
		 */
		template<typename EnumType>
			requires std::is_enum_v<EnumType>
		struct enum_name {};

		/**
		 * template<>
		 * struct enum_value_name<MyEnum::VALUE>
		 * {
		 *		constexpr static std::string_view value{"MY-ENUM-VALUE"};
		 * };
		 */
		template<auto EnumValue>
			requires std::is_enum_v<std::decay_t<decltype(EnumValue)>>
		struct enum_value_name {};
	}

	namespace enumeration_detail
	{
		template<typename EnumType, EnumNamePolicy Policy>
		[[nodiscard]] constexpr auto trim_full_name(const std::string_view name) noexcept -> std::string_view
		{
			if constexpr (Policy == EnumNamePolicy::FULL)
			{
				return name;
			}
			else if constexpr (Policy == EnumNamePolicy::WITH_SCOPED_NAME)
			{
				if constexpr (std::is_scoped_enum_v<EnumType>)
				{
					const auto last_double_colon = name.rfind("::");
					const auto optional_extra_double_colon = name.rfind("::", last_double_colon - 1);

					if (optional_extra_double_colon == std::string_view::npos)
					[[unlikely]]
					{
						// global namespace
						return name;
					}

					// namespace_A::namespace_B::...
					return name.substr(optional_extra_double_colon + 2);
				}
				else
				{
					return trim_full_name<EnumType, EnumNamePolicy::VALUE_ONLY>(name);
				}
			}
			else if constexpr (Policy == EnumNamePolicy::VALUE_ONLY)
			{
				const auto last_double_colon = name.rfind("::");

				if constexpr (std::is_scoped_enum_v<EnumType>)
				{
					PROMETHEUS_ERROR_ASSUME(last_double_colon != std::string_view::npos);

					return name.substr(last_double_colon + 2);
				}
				else
				{
					if (last_double_colon == std::string_view::npos)
					{
						// global namespace
						return name;
					}

					return name.substr(last_double_colon + 2);
				}
			}
			else
			{
				PROMETHEUS_SEMANTIC_STATIC_UNREACHABLE();
			}
		}

		// Check whether a value is a named enumeration value
		template<auto EnumValue>
			requires std::is_enum_v<std::decay_t<decltype(EnumValue)>>
		[[nodiscard]] constexpr auto is_valid_enum() noexcept -> bool
		{
			// skip the `magic`
			if constexpr (has_magic_enum_value_v<std::decay_t<decltype(EnumValue)>>)
			{
				if constexpr (EnumValue == has_magic_enum_value<std::decay_t<decltype(EnumValue)>>::magic)
				{
					return false;
				}
			}

// #if defined(PROMETHEUS_COMPILER_APPLE_CLANG) or defined(PROMETHEUS_COMPILER_CLANG_CL) or defined(PROMETHEUS_COMPILER_CLANG)
// 			PROMETHEUS_COMPILER_DISABLE_WARNING_PUSH
// 			// clang-format off
// 			PROMETHEUS_COMPILER_DISABLE_WARNING(-Wenum-constexpr-conversion)
// 			// clang-format on
// #endif

			constexpr auto name = name_of<EnumValue>();

			// MSVC
			// (enum MyEnum)0x1
			// `anonymous-namespace'::(enum MyEnum)0x1
			// CLANG
			// (MyEnum)1
			// (anonymous namespace)::(MyEnum)1
			// GNU
			// (MyEnum)1
			// (<unnamed>::MyEnum)1
#if defined(PROMETHEUS_COMPILER_MSVC)
			return not name.starts_with('(');
#elif defined(PROMETHEUS_COMPILER_APPLE_CLANG) or defined(PROMETHEUS_COMPILER_CLANG_CL) or defined(PROMETHEUS_COMPILER_CLANG)
			if (name.starts_with("(anonymous namespace)"))
			{
				return not
						name.starts_with("(anonymous namespace)::(");
			}
			return not name.starts_with('(');
#elif defined(PROMETHEUS_COMPILER_GNU)
			return not name.starts_with('(');
#else
#error "fixme"
#endif

// #if defined(PROMETHEUS_COMPILER_APPLE_CLANG) or defined(PROMETHEUS_COMPILER_CLANG_CL) or defined(PROMETHEUS_COMPILER_CLANG)
// 			PROMETHEUS_COMPILER_DISABLE_WARNING_POP
// #endif
		}

		// Check whether a value is a named enumeration value
		template<typename EnumType, std::integral auto EnumValue>
			requires std::is_enum_v<EnumType>
		[[nodiscard]] constexpr auto is_valid_enum() noexcept -> bool //
		{
// #if defined(PROMETHEUS_COMPILER_APPLE_CLANG) or defined(PROMETHEUS_COMPILER_CLANG_CL) or defined(PROMETHEUS_COMPILER_CLANG)
// 			PROMETHEUS_COMPILER_DISABLE_WARNING_PUSH
// 			// clang-format off
// 			PROMETHEUS_COMPILER_DISABLE_WARNING(-Wenum-constexpr-conversion)
// 			// clang-format on
// #endif

			return is_valid_enum<static_cast<EnumType>(EnumValue)>();

// #if defined(PROMETHEUS_COMPILER_APPLE_CLANG) or defined(PROMETHEUS_COMPILER_CLANG_CL) or defined(PROMETHEUS_COMPILER_CLANG)
// 			PROMETHEUS_COMPILER_DISABLE_WARNING_POP
// #endif
		}

		template<typename EnumType>
			requires std::is_enum_v<EnumType>
		[[nodiscard]] constexpr auto enum_name_with_user_defined() noexcept -> std::string_view
		{
			if constexpr (requires { { user_defined::enum_name<EnumType>::value } -> std::convertible_to<std::string_view>; })
			{
				return user_defined::enum_name<EnumType>::value;
			}
			else
			{
				// name::name_of
				return name_of<EnumType>();
			}
		}

		template<auto EnumValue>
			requires std::is_enum_v<std::decay_t<decltype(EnumValue)>>
		[[nodiscard]] constexpr auto enum_value_name_with_user_defined() noexcept -> std::string_view
		{
			if constexpr (requires { { user_defined::enum_value_name<EnumValue>::value } -> std::convertible_to<std::string_view>; })
			{
				return user_defined::enum_value_name<EnumValue>::value;
			}
			else
			{
				return name_of<EnumValue>();
			}
		}

		template<std::size_t Size, typename EnumType>
			requires std::is_enum_v<EnumType>
		[[nodiscard]] constexpr auto save_values(const std::vector<EnumType>& values) noexcept -> std::array<EnumType, Size>
		{
			std::array<EnumType, Size> result{};
			std::ranges::copy(values, result.data());
			return result;
		}

		// ==============================================================
		// FLAG
		// ==============================================================

		enum class FlagBehavior : std::uint8_t
		{
			// Only single-bit values (powers of two)
			SINGLE_BIT_ONLY,
			// All combinations of consecutive bits
			ALLOW_COMBINATION,
		};

		// Generates a bitmask with 'Count' consecutive bits starting from chunk 'Index'
		// Returns a value where bits at positions [Index*Count, Index*Count+Count-1] are set to 1
		// Example: flag_get_contiguous_bits<MyEnum, 3, 1>() with 8-bit underlying type would return 0b00011100
		template<typename EnumType, std::size_t Count, std::size_t Index>
			requires std::is_enum_v<EnumType>
		[[nodiscard]] constexpr auto flag_get_contiguous_bits() noexcept -> std::underlying_type_t<EnumType>
		{
			using type = std::underlying_type_t<EnumType>;

			// Create sequence of bit positions [0, digits-1] where 'digits' is the number of bits in the underlying type
			constexpr auto index_sequence = std::views::iota(static_cast<type>(0), static_cast<type>(std::numeric_limits<type>::digits));

			// Split the sequence into chunks of size 'Count'
			// Each chunk represents a group of consecutive bit positions
			constexpr auto index_chunk = index_sequence | std::views::chunk(static_cast<type>(Count));

			// Select the specific chunk at position 'Index'
			// This contains 'Count' consecutive bit positions
			constexpr auto indices = index_chunk[Index];

			// Generate bitmask by setting bits at all positions in the selected chunk
			// Fold left combines all positions using bitwise OR
			constexpr auto bits = std::ranges::fold_left(
				indices,
				static_cast<type>(0),
				[](const auto total, const auto current) noexcept -> auto
				{
					// Set bits at position 'current' by left-shifting 1
					return total | (type{1} << current);
				}
			);

			return bits;
		}

		// Generates all valid enum values
		template<typename EnumType, FlagBehavior Behavior>
			requires std::is_enum_v<EnumType>
		[[nodiscard]] constexpr auto flag_collect_values() noexcept -> std::vector<EnumType>
		{
			// Get number of bits in the underlying type
			constexpr auto digits = std::numeric_limits<std::underlying_type_t<EnumType>>::digits;

			// Container for valid enum values
			std::vector<EnumType> values{};

			// Always include zero value if it's valid
			// Zero often represents "no flags set" in flag enums
			if constexpr (is_valid_enum<EnumType, 0>())
			{
				values.push_back(static_cast<EnumType>(0));
			}

			if constexpr (Behavior == FlagBehavior::SINGLE_BIT_ONLY)
			{
				// MODE 1: Only single-bit flags (powers of two)
				// Example: 0b00000001, 0b00000010, 0b00000100, etc.

				// Single bits only
				constexpr std::size_t this_count = 1;

				// Lambda to check and add a specific single-bit value
				const auto check = [&values]<std::size_t ThisIndex>() noexcept -> void
				{
					// Generate bitmask for a single bit at position ThisIndex
					if constexpr (constexpr auto bits = flag_get_contiguous_bits<EnumType, this_count, ThisIndex>();
						is_valid_enum<EnumType, bits>())
					{
						values.push_back(static_cast<EnumType>(bits));
					}
				};

				// Calculate how many single-bit positions we need to check
				constexpr auto indices_count = digits / this_count + (digits % this_count == 0 ? 0 : 1);

				// Iterate over all possible bit positions using compile-time indices
				[check]<std::size_t... Index>(std::index_sequence<Index...>) noexcept -> void
				{
					(check.template operator()<Index>(), ...);
				}(std::make_index_sequence<indices_count>{});
			}
			else
			{
				// MODE 2: All combinations of consecutive bits
				// Includes single bits, pairs, triples, etc.
				// Example: 0b00000011, 0b00000111, 0b00001110, etc.

				// Iterate over all possible group sizes (1 to digits)
				[&values]<std::size_t... Count>(std::index_sequence<Count...>) noexcept -> void
				{
					// Lambda to process each possible group size
					const auto traversal = [&values]<std::size_t ThisCount>() noexcept -> void
					{
						// Lambda to check and add a specific bit pattern
						const auto check = [&values]<std::size_t ThisIndex>() noexcept -> void
						{
							// Generate bitmask for ThisCount consecutive bits starting at chunk ThisIndex
							if constexpr (constexpr auto bits = flag_get_contiguous_bits<EnumType, ThisCount, ThisIndex>();
								is_valid_enum<EnumType, bits>())
							{
								values.push_back(static_cast<EnumType>(bits));
							}
						};

						// Calculate how many positions of this group size we can fit
						constexpr auto indices_count = digits / ThisCount + (digits % ThisCount == 0 ? 0 : 1);

						// Check all possible starting positions for this group size
						return [check]<std::size_t... Index>(std::index_sequence<Index...>) noexcept -> void
						{
							(check.template operator()<Index>(), ...);
						}(std::make_index_sequence<indices_count>{});
					};

					// Process all group sizes from 1 to 'digits'
					(traversal.template operator()<Count + 1>(), ...);
				}(std::make_index_sequence<digits>{});
			}

			return values;
		}

		template<typename EnumType, FlagBehavior Behavior>
			requires std::is_enum_v<EnumType>
		struct cached_flag_values_size : std::integral_constant<std::size_t, flag_collect_values<EnumType, Behavior>().size()> {};

		template<typename EnumType, FlagBehavior Behavior>
			requires std::is_enum_v<EnumType>
		struct cached_flag_values
		{
			constexpr static auto value = enumeration_detail::save_values<cached_flag_values_size<EnumType, Behavior>::value>(flag_collect_values<EnumType, Behavior>());
		};

		template<typename EnumType, FlagBehavior Behavior, std::size_t Index>
			requires std::is_enum_v<EnumType> and (Index < cached_flag_values_size<EnumType, Behavior>::value)
		struct cached_flag_value : std::integral_constant<EnumType, cached_flag_values<EnumType, Behavior>::value[Index]> {};

		template<typename EnumType, FlagBehavior Behavior>
			requires std::is_enum_v<EnumType>
		struct cached_flag_min_value : std::integral_constant<
					EnumType,
					cached_flag_value<EnumType, Behavior, 0>::value
				> {};

		template<typename EnumType, FlagBehavior Behavior>
			requires std::is_enum_v<EnumType>
		struct cached_flag_max_value : std::integral_constant<
					EnumType,
					cached_flag_value<EnumType, Behavior, cached_flag_values_size<EnumType, Behavior>::value - 1>::value
				> {};

		template<typename EnumType, EnumNamePolicy Policy, FlagBehavior Behavior>
			requires std::is_enum_v<EnumType>
		[[nodiscard]] constexpr auto flag_collect_names() noexcept -> auto
		{
			constexpr auto size = cached_flag_values_size<EnumType, Behavior>::value;
			using return_type = std::array<std::pair<EnumType, std::string_view>, size>;

			return []<std::size_t... Index>(std::index_sequence<Index...>) noexcept -> return_type
			{
				return
						//
						return_type
						{
								//
								typename return_type::value_type
								{
										//
										cached_flag_value<EnumType, Behavior, Index>::value,
										enumeration_detail::trim_full_name<EnumType, Policy>(
											//
											name_of<cached_flag_value<EnumType, Behavior, Index>::value>()
										)
								}...
						};
			}(std::make_index_sequence<size>{});
		}

		template<typename EnumType, EnumNamePolicy Policy, FlagBehavior Behavior>
			requires std::is_enum_v<EnumType>
		struct cached_flag_names
		{
			constexpr static auto value = flag_collect_names<EnumType, Policy, Behavior>();
		};

		// Checks if an enum should be treated as a flags enum
		// Returns true if:
		// 1. User explicitly declared it as a flag enum via user_defined::enum_is_flag, OR
		// 2. It has enough single-bit values to be inferred as a flags enum
		template<typename EnumType>
			requires std::is_enum_v<EnumType>
		[[nodiscard]] constexpr auto is_flag_with_user_defined() noexcept -> bool
		{
			if constexpr (user_defined::enum_is_flag<EnumType>::value)
			{
				// User explicitly marked this as a flags enum
				return true;
			}
			else
			{
				constexpr auto single_bit_values_count = cached_flag_values_size<EnumType, FlagBehavior::SINGLE_BIT_ONLY>::value;
				constexpr auto combination_values_count = cached_flag_values_size<EnumType, FlagBehavior::ALLOW_COMBINATION>::value;

				// FIXME: 
				// The threshold that is too low may cause some enumeration values to be incorrectly identified as flag enumerations. 
				// The threshold that is too high may also cause enumeration types with only a few flag enumerations to not be identified as flag enumerations.
				return single_bit_values_count >= 4 or combination_values_count >= 6;
			}
		}

		// ==============================================================
		// ENUM
		// ==============================================================

		// Finds the minimum valid enum value

		template<typename EnumType>
		[[nodiscard]] constexpr auto enum_min_value() noexcept -> EnumType
		{
			using type = std::underlying_type_t<EnumType>;

			constexpr auto min = user_defined::enum_range<EnumType>::min;
			constexpr auto max = user_defined::enum_range<EnumType>::max;

			if constexpr (is_valid_enum<EnumType, min>())
			{
				return static_cast<EnumType>(min);
			}
			else
			{
				type result = min;
				const auto process = [&result]<type Current>() noexcept -> bool
				{
					if constexpr (is_valid_enum<EnumType, Current>())
					{
						result = Current;
						return false;
					}
					else
					{
						return true;
					}
				};

				[process]<std::size_t... Index>(std::index_sequence<Index...>) noexcept -> void
				{
					(process.template operator()<min + 1 + Index>() and ...);
				}(std::make_index_sequence<max - min>{});

				return static_cast<EnumType>(result);
			}
		}

		// Finds the maximum valid enum value

		template<typename EnumType>
		[[nodiscard]] constexpr auto enum_max_value() noexcept -> EnumType
		{
			using type = std::underlying_type_t<EnumType>;

			constexpr auto min = user_defined::enum_range<EnumType>::min;
			constexpr auto max = user_defined::enum_range<EnumType>::max;

			if constexpr (is_valid_enum<EnumType, max>())
			{
				return static_cast<EnumType>(max);
			}
			else
			{
				type result = max;
				const auto process = [&result]<type Current>() noexcept -> bool
				{
					if constexpr (is_valid_enum<EnumType, Current>())
					{
						result = Current;
						return false;
					}
					else
					{
						return true;
					}
				};

				[process]<std::size_t... Index>(std::index_sequence<Index...>) noexcept -> void
				{
					(process.template operator()<max - 1 - Index>() and ...);
				}(std::make_index_sequence<max - min>{});

				return static_cast<EnumType>(result);
			}
		}

		template<typename EnumType>
			requires std::is_enum_v<EnumType>
		struct cached_enum_min_value : std::integral_constant<EnumType, enum_min_value<EnumType>()> {};

		template<typename EnumType>
			requires std::is_enum_v<EnumType>
		struct cached_enum_max_value : std::integral_constant<EnumType, enum_max_value<EnumType>()> {};

		template<typename EnumType>
			requires std::is_enum_v<EnumType>
		[[nodiscard]] constexpr auto enum_collect_values() noexcept -> std::vector<EnumType>
		{
			using type = std::underlying_type_t<EnumType>;

			constexpr auto min = cached_enum_min_value<EnumType>::value;
			constexpr auto max = cached_enum_max_value<EnumType>::value;
			constexpr auto range = static_cast<type>(max) - static_cast<type>(min) + 1;

			std::vector<EnumType> values{};

			const auto check = [&values]<type Value>() noexcept -> void
			{
				if constexpr (is_valid_enum<EnumType, Value>())
				{
					values.push_back(static_cast<EnumType>(Value));
				}
			};

			[check]<std::size_t... Index>(std::index_sequence<Index...>) noexcept -> void
			{
				(check.template operator()<static_cast<type>(min) + Index>(), ...);
			}(std::make_index_sequence<range>{});

			return values;
		}

		template<typename EnumType>
			requires std::is_enum_v<EnumType>
		struct cached_enum_values_size : std::integral_constant<std::size_t, enum_collect_values<EnumType>().size()> {};

		template<typename EnumType>
			requires std::is_enum_v<EnumType>
		struct cached_enum_values
		{
			constexpr static auto value = enumeration_detail::save_values<cached_enum_values_size<EnumType>::value>(enum_collect_values<EnumType>());
		};

		template<typename EnumType, std::size_t Index>
			requires std::is_enum_v<EnumType> and (Index < cached_enum_values_size<EnumType>::value)
		struct cached_enum_value : std::integral_constant<EnumType, cached_enum_values<EnumType>::value[Index]> {};

		template<typename EnumType, EnumNamePolicy Policy>
			requires std::is_enum_v<EnumType>
		[[nodiscard]] constexpr auto enum_collect_names() noexcept -> auto
		{
			constexpr auto size = cached_enum_values_size<EnumType>::value;
			using return_type = std::array<std::pair<EnumType, std::string_view>, size>;

			return []<std::size_t... Index>(std::index_sequence<Index...>) noexcept -> return_type
			{
				return
						//
						return_type
						{
								//
								typename return_type::value_type
								{
										cached_enum_value<EnumType, Index>::value,
										enumeration_detail::trim_full_name<EnumType, Policy>(
											//
											name_of<cached_enum_value<EnumType, Index>::value>()
										)
								}...
						};
			}(std::make_index_sequence<size>{});
		}

		template<typename EnumType, EnumNamePolicy Policy>
			requires std::is_enum_v<EnumType>
		struct cached_enum_names
		{
			constexpr static auto value = enum_collect_names<EnumType, Policy>();
		};
	}

	template<typename EnumType>
		requires std::is_enum_v<EnumType>
	[[nodiscard]] constexpr auto is_flag() noexcept -> bool
	{
		return enumeration_detail::is_flag_with_user_defined<EnumType>();
	}

	template<typename EnumType>
		requires std::is_enum_v<EnumType>
	[[nodiscard]] constexpr auto min_value_of() noexcept -> EnumType
	{
		if constexpr (is_flag<EnumType>())
		{
			return enumeration_detail::cached_flag_min_value<EnumType, enumeration_detail::FlagBehavior::ALLOW_COMBINATION>::value;
		}
		else
		{
			return enumeration_detail::cached_enum_min_value<EnumType>::value;
		}
	}

	template<typename EnumType>
		requires std::is_enum_v<EnumType>
	[[nodiscard]] constexpr auto max_value_of() noexcept -> EnumType
	{
		if constexpr (is_flag<EnumType>())
		{
			return enumeration_detail::cached_flag_max_value<EnumType, enumeration_detail::FlagBehavior::ALLOW_COMBINATION>::value;
		}
		else
		{
			return enumeration_detail::cached_enum_max_value<EnumType>::value;
		}
	}

	// enum class MyEnum
	// {
	//		E1,
	//		E2,
	//		E3,
	// }
	//
	// names_of<MyEnum> => [ "E1", "E2", "E3" ]
	//
	// enum class MyFlag
	// {
	//		F1 = 0x0001,
	//		F2 = 0x0010,
	//		F3 = 0x0100,
	//		F4 = 0x1000,
	//		F5 = F1 | F2,
	// }
	//
	// names_of<MyFlag> => [ "F1", "F2", "F3", "F4", "F5" ]

	template<typename EnumType, EnumNamePolicy Policy>
		requires std::is_enum_v<EnumType>
	[[nodiscard]] constexpr auto names_of() noexcept -> auto
	{
		if constexpr (is_flag<EnumType>())
		{
			return enumeration_detail::cached_flag_names<EnumType, Policy, enumeration_detail::FlagBehavior::ALLOW_COMBINATION>::value;
		}
		else
		{
			return enumeration_detail::cached_enum_names<EnumType, Policy>::value;
		}
	}

	template<typename EnumType>
		requires std::is_enum_v<EnumType>
	[[nodiscard]] constexpr auto names_of() noexcept -> auto //
	{
		return names_of<EnumType, user_defined::enum_name_policy<EnumType>::value>();
	}

	constexpr std::string_view enum_name_not_found{"?"};

	// enum class MyEnum
	// {
	//		E1 = 0,
	//		E2 = 1,
	//		E3 = 2,
	// }
	//
	// name_of(MyEnum::E1) => "E1"
	// name_of(MyEnum::E2) => "E2"
	// name_of(static_cast<MyEnum>(42)) => "?" (enum_name_not_found)
	//
	// name_of<MyEnum>(0) => "E1"
	// name_of<MyEnum>(1) => "E2"
	// name_of<MyEnum>(42) => "?" (enum_name_not_found)
	//
	// enum class MyFlag
	// {
	//		F1 = 0x0001,
	//		F2 = 0x0010,
	//		F3 = 0x0100,
	//		F4 = 0x1000,
	//		F5 = F1 | F2,
	// }
	//
	// name_of(MyFlag::F1) => "F1"
	// name_of(MyFlag::F2) => "F2"
	// name_of(MyFlag::F5) => "F5"
	// name_of(static_cast<MyFlag>(0x1'0000)) => "?" (enum_name_not_found)
	//
	// name_of<MyFlag>(0x0001) => "F1"
	// name_of<MyFlag>(0x0010) => "F2"
	// name_of<MyFlag>(0x0001 | 0x0010) => "F5"
	// name_of<MyFlag>(0x1'0000) => "?" (enum_name_not_found)

	template<EnumNamePolicy Policy, typename EnumType>
		requires std::is_enum_v<EnumType>
	[[nodiscard]] constexpr auto name_of(const EnumType enum_value) noexcept -> std::string_view
	{
		constexpr auto list = names_of<EnumType, Policy>();

		if (const auto it = std::ranges::find(
				list,
				enum_value,
				[](const auto& pair) noexcept -> EnumType
				{
					return pair.first;
				}
			);
			it != std::ranges::end(list))
		{
			return it->second;
		}
		return enum_name_not_found;
	}

	template<typename EnumType>
		requires std::is_enum_v<EnumType>
	[[nodiscard]] constexpr auto name_of(const EnumType enum_value) noexcept -> std::string_view //
	{
		return meta::name_of<user_defined::enum_name_policy<EnumType>::value, EnumType>(enum_value);
	}

	template<typename EnumType, EnumNamePolicy Policy = user_defined::enum_name_policy<EnumType>::value>
		requires std::is_enum_v<EnumType>
	[[nodiscard]] constexpr auto name_of(const std::integral auto enum_value) noexcept -> std::string_view
	{
		return meta::name_of<Policy, EnumType>(static_cast<EnumType>(enum_value));
	}

	// enum class MyFlag
	// {
	//		F1 = 0x0001,
	//		F2 = 0x0010,
	//		F3 = 0x0100,
	//		F4 = 0x1000,
	//		F5 = F1 | F2,
	// }
	//
	// full_name_of(MyFlag::F1) => "F1"
	// full_name_of(MyFlag::F2) => "F2"
	// full_name_of(MyFlag::F5) => "F1|F2"
	// full_name_of(MyFlag::F3 | MyFlag::F4) => "F3|F4"
	// full_name_of(static_cast<MyFlag>(0x1'0000)) => "?" (enum_name_not_found)
	//
	// name_of<MyFlag>(0x0001) => "F1"
	// name_of<MyFlag>(0x0010) => "F2"
	// name_of<MyFlag>(0x0001 | 0x0010) => "F1|F2"
	// name_of<MyFlag>(0x0100 | 0x1000) => "F3|F4"
	// name_of<MyFlag>(0x1'0000) => "?" (enum_name_not_found)

	template<EnumNamePolicy Policy, typename Allocator = std::allocator<char>, typename EnumType>
		requires(std::is_enum_v<EnumType> and is_flag<EnumType>())
	[[nodiscard]] constexpr auto full_name_of(
		const EnumType enum_value,
		const std::string_view split = "|",
		const Allocator& allocator = {}
	) noexcept -> std::basic_string<char, std::char_traits<char>, Allocator>
	{
		const auto value = std::to_underlying(enum_value);
		const auto unsigned_value = static_cast<std::make_unsigned_t<std::underlying_type_t<EnumType>>>(value);

		std::basic_string<char, std::char_traits<char>, Allocator> result{allocator};

		if (std::popcount(unsigned_value) == 1)
		{
			const auto i = std::countr_zero(unsigned_value);
			if constexpr (requires { result.append_range(meta::name_of<EnumType, Policy>(1 << i)); })
			{
				result.append_range(meta::name_of<EnumType, Policy>(1 << i));
			}
			else
			{
				result.append(meta::name_of<EnumType, Policy>(1 << i));
			}

			return result;
		}

		for (std::size_t i = 0; i < std::numeric_limits<std::underlying_type_t<EnumType>>::digits; ++i)
		{
			if (value & (1 << i))
			{
				if constexpr (requires { result.append_range(meta::name_of<EnumType, Policy>(1 << i)); })
				{
					result.append_range(meta::name_of<EnumType, Policy>(1 << i));
					result.append_range(split);
				}
				else
				{
					result.append(meta::name_of<EnumType, Policy>(1 << i));
					result.append(split);
				}
			}
		}

		if (not result.empty())
		{
			result.erase(result.size() - split.size(), split.size());
		}

		return result;
	}

	template<typename Allocator = std::allocator<char>, typename EnumType>
		requires(std::is_enum_v<EnumType> and is_flag<EnumType>())
	[[nodiscard]] constexpr auto full_name_of(
		const EnumType enum_value,
		const std::string_view split = "|",
		const Allocator& allocator = {}
	) noexcept -> std::basic_string<char, std::char_traits<char>, Allocator>
	{
		return meta::full_name_of<user_defined::enum_name_policy<EnumType>::value, Allocator, EnumType>(enum_value, split, allocator);
	}

	template<typename EnumType, EnumNamePolicy Policy = user_defined::enum_name_policy<EnumType>::value, typename Allocator = std::allocator<char>>
		requires(std::is_enum_v<EnumType> and is_flag<EnumType>())
	[[nodiscard]] constexpr auto full_name_of(
		const std::integral auto enum_value,
		const std::string_view split = "|",
		const Allocator& allocator = {}
	) noexcept -> std::basic_string<char, std::char_traits<char>, Allocator>
	{
		return meta::full_name_of<Policy, Allocator, EnumType>(static_cast<EnumType>(enum_value), split, allocator);
	}

	// enum class MyEnum
	// {
	//		E1 = 0,
	//		E2 = 1,
	//		E3 = 2,
	// }
	//
	// value_of("E1") => E1
	// value_of("E2") => E2
	// value_of("E1|E2") => E1|E2
	// value_of("E1|E4") => empty[Strict] / E1
	// value_of("?") => empty
	//
	// enum class MyFlag
	// {
	//		F1 = 0x0001,
	//		F2 = 0x0010,
	//		F3 = 0x0100,
	//		F4 = 0x1000,
	//		F5 = F1 | F2,
	// }
	//
	// value_of("F1") => F1
	// value_of("F2") => F2
	// value_of("F1|F2") => F1|F2
	// value_of("F1|F0") => empty[Strict] / F1
	// value_of("F5") => F5
	// value_of("?") => empty

	template<typename EnumType, EnumNamePolicy Policy = user_defined::enum_name_policy<EnumType>::value, bool Strict = true>
		requires std::is_enum_v<EnumType>
	[[nodiscard]] constexpr auto value_of(
		const std::string_view enum_name,
		const EnumType empty,
		const std::string_view split
	) noexcept -> EnumType
	{
		constexpr auto list = names_of<EnumType, Policy>();

		// error C2662: `const std::ranges::split_view<std::basic_string_view<char,std::char_traits<char>>,std::basic_string_view<char,std::char_traits<char>>>` => `std::ranges::split_view<std::basic_string_view<char,std::char_traits<char>>,std::basic_string_view<char,std::char_traits<char>>> &`
		// ReSharper disable once CppLocalVariableMayBeConst
		auto names = enum_name | std::views::split(split);
		auto result = std::to_underlying(empty);

		for (const auto& each: names)
		{
			const std::string_view s{each};

			if (const auto it = std::ranges::find(list | std::views::values, s);
				it != std::ranges::end(list | std::views::values))
			{
				result |= std::to_underlying(it.base()->first);
			}
			else
			{
				if constexpr (Strict)
				{
					return empty;
				}
			}
		}

		return static_cast<EnumType>(result);
	}

	template<typename EnumType, EnumNamePolicy Policy = user_defined::enum_name_policy<EnumType>::value, bool Strict = true>
		requires std::is_enum_v<EnumType>
	[[nodiscard]] constexpr auto value_of(
		const std::string_view enum_name,
		const std::string_view split,
		const EnumType empty
	) noexcept -> EnumType
	{
		return meta::value_of<EnumType, Policy, Strict>(enum_name, empty, split);
	}

	template<typename EnumType, EnumNamePolicy Policy = user_defined::enum_name_policy<EnumType>::value, bool Strict = true>
		requires std::is_enum_v<EnumType>
	[[nodiscard]] constexpr auto value_of(const std::string_view enum_name, const std::string_view split) noexcept -> EnumType
	{
		return meta::value_of<EnumType, Policy, Strict>(enum_name, static_cast<EnumType>(0), split);
	}

	template<typename EnumType, EnumNamePolicy Policy = user_defined::enum_name_policy<EnumType>::value, bool Strict = true>
		requires std::is_enum_v<EnumType>
	[[nodiscard]] constexpr auto value_of(const std::string_view enum_name, const EnumType empty) noexcept -> EnumType
	{
		return meta::value_of<EnumType, Policy, Strict>(enum_name, empty, "|");
	}

	template<typename EnumType, EnumNamePolicy Policy = user_defined::enum_name_policy<EnumType>::value, bool Strict = true>
		requires std::is_enum_v<EnumType>
	[[nodiscard]] constexpr auto value_of(const std::string_view enum_name) noexcept -> EnumType
	{
		return meta::value_of<EnumType, Policy, Strict>(enum_name, static_cast<EnumType>(0), "|");
	}
}
