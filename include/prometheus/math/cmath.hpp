// This file is part of prometheus
// Copyright (C) 2022-2025 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <cmath>
#include <numbers>
#include <numeric>
#include <limits>

#include <prometheus/macro.hpp>

#if not defined(__cpp_lib_constexpr_cmath) or __cpp_lib_constexpr_cmath < 202306L
#define CMATH_WORKAROUND_REQUIRED
#endif

namespace prometheus::math
{
	template<typename T>
		requires std::is_arithmetic_v<T>
	[[nodiscard]] constexpr auto is_nan(const T value) noexcept -> bool
	{
		if constexpr (not std::is_floating_point_v<T>)
		{
			return false;
		}
		else
		{
#ifdef CMATH_WORKAROUND_REQUIRED
			PROMETHEUS_SEMANTIC_IF_CONSTANT_EVALUATED
			{
				// ReSharper disable once CppIdenticalOperandsInBinaryExpression
				return value != value; // NOLINT(misc-redundant-expression)
			}
#endif

			return std::isnan(value);
		}
	}

	template<typename T>
		requires std::is_arithmetic_v<T>
	[[nodiscard]] constexpr auto abs(const T value) noexcept -> T
	{
		if constexpr (std::is_unsigned_v<T>)
		{
			return value;
		}
		else
		{
#ifdef CMATH_WORKAROUND_REQUIRED
			PROMETHEUS_SEMANTIC_IF_CONSTANT_EVALUATED
			{
				return value > 0 ? value : -value;
			}
#endif

			return std::abs(value);
		}
	}

	template<typename T>
		requires std::is_arithmetic_v<T>
	[[nodiscard]] constexpr auto floor(const T value) noexcept -> T
	{
		if constexpr (not std::is_floating_point_v<T>)
		{
			return value;
		}
		else
		{
#ifdef CMATH_WORKAROUND_REQUIRED
			PROMETHEUS_SEMANTIC_IF_CONSTANT_EVALUATED
			{
				if (value >= 0 or static_cast<T>(static_cast<unsigned long long>(value)) == value)
				{
					return static_cast<T>(static_cast<unsigned long long>(value));
				}

				return static_cast<T>(static_cast<unsigned long long>(value) - 1);
			}
#endif

			return std::floor(value);
		}
	}

	template<typename T>
		requires std::is_arithmetic_v<T>
	[[nodiscard]] constexpr auto ceil(const T value) noexcept -> T
	{
		if constexpr (not std::is_floating_point_v<T>)
		{
			return value;
		}
		else
		{
#ifdef CMATH_WORKAROUND_REQUIRED
			PROMETHEUS_SEMANTIC_IF_CONSTANT_EVALUATED
			{
				if (value >= 0 or static_cast<T>(static_cast<unsigned long long>(value)) == value)
				{
					return static_cast<T>(static_cast<unsigned long long>(value));
				}

				return static_cast<T>(static_cast<unsigned long long>(value) + 1);
			}
#endif

			return std::ceil(value);
		}
	}

	template<typename T>
		requires std::is_arithmetic_v<T>
	// ReSharper disable once IdentifierTypo
	[[nodiscard]] constexpr auto tgamma(const T value) noexcept -> T
	{
		// PROMETHEUS_PLATFORM_ASSUME(value >= T{0});

		if constexpr (not std::is_floating_point_v<T>)
		{
			if (value <= 1)
			{
				return 1;
			}

			T result = 1;
			for (T i = 2; i <= value; ++i)
			{
				result *= i;
			}

			return result;
		}
		else
		{
#ifdef CMATH_WORKAROUND_REQUIRED
			PROMETHEUS_SEMANTIC_IF_CONSTANT_EVALUATED
			{
				return (value <= 1) ? 1 : (value * math::tgamma(value - 1));
			}
#endif

			return static_cast<T>(std::tgamma(value));
		}
	}

	template<std::integral T>
	[[nodiscard]] constexpr auto factorial(const T value) noexcept -> T //
	{
		return math::tgamma(value);
	}

	template<typename T>
		requires std::is_arithmetic_v<T>
	[[nodiscard]] constexpr auto pow(const T base, const int exp) noexcept -> T
	{
		// PROMETHEUS_PLATFORM_ASSUME(exp >= 0);

#ifdef CMATH_WORKAROUND_REQUIRED
		PROMETHEUS_SEMANTIC_IF_CONSTANT_EVALUATED
		{
			if (exp == 0)
			{
				return static_cast<T>(1);
			}

			if (exp < 0)
			{
				return static_cast<T>(1) / math::pow(base, -exp);
			}

			if (exp % 2 == 0)
			{
				T half = math::pow(base, exp / 2);
				return half * half;
			}

			return static_cast<T>(base * math::pow(base, exp - 1));
		}
#endif

		return static_cast<T>(std::pow(base, exp));
	}

	template<typename T>
		requires std::is_arithmetic_v<T>
	[[nodiscard]] constexpr auto sqrt(const T value) noexcept -> T
	{
		// PROMETHEUS_PLATFORM_ASSUME(value >= 0);

#ifdef CMATH_WORKAROUND_REQUIRED
		PROMETHEUS_SEMANTIC_IF_CONSTANT_EVALUATED
		{
			if (value == 0) // NOLINT(clang-diagnostic-float-equal)
			{
				return value;
			}

			auto prev = static_cast<T>(0);
			auto current = static_cast<T>(value / 2);

			while (current != prev) // NOLINT(clang-diagnostic-float-equal)
			{
				prev = current;
				current = (current + value / current) / 2;
			}

			return current;
		}
#endif

		return static_cast<T>(std::sqrt(value));
	}

	template<typename T>
		requires std::is_arithmetic_v<T>
	// ReSharper disable once IdentifierTypo
	[[nodiscard]] constexpr auto hypot(const T x, const std::type_identity_t<T> y) noexcept -> T
	{
#ifdef CMATH_WORKAROUND_REQUIRED
		PROMETHEUS_SEMANTIC_IF_CONSTANT_EVALUATED
		{
			return math::sqrt(math::pow(x, 2) + math::pow(y, 2));
		}
#endif

		return static_cast<T>(std::hypot(x, y));
	}

	template<typename T>
		requires std::is_arithmetic_v<T>
	// ReSharper disable once IdentifierTypo
	[[nodiscard]] constexpr auto hypot(const T x, const std::type_identity_t<T> y, const std::type_identity_t<T> z) noexcept -> T
	{
#ifdef CMATH_WORKAROUND_REQUIRED
		PROMETHEUS_SEMANTIC_IF_CONSTANT_EVALUATED
		{
			return math::sqrt(math::pow(x, 2) + math::pow(y, 2) + math::pow(z, 2));
		}
#endif

		return static_cast<T>(std::hypot(x, y, z));
	}

	template<typename T>
		requires std::is_arithmetic_v<T>
	[[nodiscard]] constexpr auto normalize(const T x, const T y) noexcept -> auto
	{
		using value_type = std::conditional_t<sizeof(T) == sizeof(float), float, double>;
		using result_type = std::pair<value_type, value_type>;

		const auto length_2 = math::pow(static_cast<value_type>(x), 2) + math::pow(static_cast<value_type>(y), 2);
		if (length_2 <= std::numeric_limits<value_type>::epsilon() * std::numeric_limits<value_type>::epsilon())
		{
			return result_type{0, 0};
		}

		const auto length = math::sqrt(length_2);
		return result_type{static_cast<value_type>(x) / length, static_cast<value_type>(y) / length};
	}

#ifdef CMATH_WORKAROUND_REQUIRED
	// ReSharper disable once IdentifierTypo
	namespace cmath_detail
	{
		/// @brief Computes tangent using series expansion near pi/2 singularity
		/// @details When tan(x) approaches infinity near pi/2, we use a series expansion
		///          based on Bernoulli numbers to compute 1/tan(x-pi/2)
		/// @param value Input angle in radians
		/// @return Tangent value or pi/2 for values extremely close to pi/2
		template<typename T>
		[[nodiscard]] constexpr auto tan_series_exp(const T value) noexcept -> T
		{
			const auto z = value - std::numbers::pi_v<T> / 2;

			// Handle values extremely close to pi/2 to avoid division by zero
			if (std::numeric_limits<T>::min() > math::abs(z))
			{
				return std::numbers::pi_v<T> / 2;
			}

			// Series expansion: tan(z) ≈ -1/z + z/3 + z³/45 + 2z⁵/945 + z⁷/4725 + ...
			// This is derived from the Laurent series expansion of tan(x) around pi/2
			return -1 / z + (z / 3 + (math::pow(z, 3) / 45 + (2 * math::pow(z, 5) / 945 + math::pow(z, 7) / 4725)));
		}

		/// @brief Recursive helper for tangent continued fraction computation
		/// @details Implements the continued fraction representation of tan(x):
		///          tan(x) = x/(1 - x²/(3 - x²/(5 - x²/(7 - ...))))
		/// @param value x² where x is the input angle
		/// @param current Current depth in the continued fraction (starts at 1)
		/// @param max Maximum recursion depth (determines precision)
		/// @return Partial result of the continued fraction evaluation
		template<typename T>
		[[nodiscard]] constexpr auto tan_cf_recurse(const T value, const int current, const int max) noexcept -> T
		{
			const auto z = static_cast<T>(2 * current - 1);

			if (current < max)
			{
				// Continue fraction evaluation: z - value / next_term
				return z - value / cmath_detail::tan_cf_recurse(value, current + 1, max);
			}

			// Base case: return the last term z (odd integer)
			return z;
		}

		/// @brief Main continued fraction implementation for tangent
		/// @details Uses different recursion depths based on input magnitude for optimal
		///          balance between accuracy and compile-time performance
		/// @param value Input angle in radians (after range reduction)
		/// @return tan(value) computed via continued fractions
		template<typename T>
		[[nodiscard]] constexpr auto tan_cf_main(const T value) noexcept -> T
		{
			// For values near pi/2 (1.57), use series expansion to handle singularity
			if (value > static_cast<T>(1.55) and value < static_cast<T>(1.6))
			{
				// Special case: near π/2 singularity
				return cmath_detail::tan_series_exp(value);
			}

			// Adjust recursion depth based on input magnitude:
			// Larger values need more terms for same accuracy
			if (value > static_cast<T>(1.4))
			{
				// High precision for large inputs (45 terms)
				return value / cmath_detail::tan_cf_recurse(value * value, 1, 45);
			}

			if (value > static_cast<T>(1))
			{
				// Medium precision (35 terms)
				return value / cmath_detail::tan_cf_recurse(value * value, 1, 35);
			}

			// Lower precision for small inputs (25 terms)
			return value / cmath_detail::tan_cf_recurse(value * value, 1, 25);
		}

		/// @brief Range reduction and main entry point for constexpr tangent
		/// @details Reduces input to [0, π] using periodicity of tangent,
		///          then calls the appropriate calculation method
		/// @param value Input angle in radians
		/// @param count Safety counter to detect infinite recursion (max 2 reductions)
		/// @return tan(value) or NaN if reduction fails
		template<typename T>
		[[nodiscard]] constexpr auto tan_begin(const T value, const int count = 0) noexcept -> T
		{
			// Reduce angle to [0, π] using tan(x) = tan(x - π)
			if (value > std::numbers::pi_v<T>)
			{
				// Safety check: prevent infinite recursion from pathological inputs
				if (count > 1)
				{
					return std::numeric_limits<T>::quiet_NaN();
				}

				// Reduce by multiples of π (not 2π because tan has period π, not 2π)
				return cmath_detail::tan_begin(value - std::numbers::pi_v<T> * math::floor(value - std::numbers::pi_v<T>), count + 1);
			}

			// Value now in [0, π], compute tangent
			return cmath_detail::tan_cf_main(value);
		}
	}
#endif

	template<typename T>
		requires std::is_arithmetic_v<T>
	[[nodiscard]] constexpr auto tan(const T value) noexcept -> T
	{
#ifdef CMATH_WORKAROUND_REQUIRED
		PROMETHEUS_SEMANTIC_IF_CONSTANT_EVALUATED
		{
			if (math::is_nan(value))
			{
				return std::numeric_limits<T>::quiet_NaN();
			}

			if (value < static_cast<T>(0))
			{
				return -cmath_detail::tan_begin(-value);
			}

			return cmath_detail::tan_begin(value);
		}
#endif

		return std::tan(value);
	}

	template<typename T>
		requires std::is_arithmetic_v<T>
	[[nodiscard]] constexpr auto sin(const T value) noexcept -> T
	{
#ifdef CMATH_WORKAROUND_REQUIRED
		PROMETHEUS_SEMANTIC_IF_CONSTANT_EVALUATED
		{
			if (math::is_nan(value))
			{
				return std::numeric_limits<T>::quiet_NaN();
			}

			if (std::numeric_limits<T>::min() > math::abs(value - std::numbers::pi_v<T> / 2))
			{
				return 1;
			}

			if (std::numeric_limits<T>::min() > math::abs(value + std::numbers::pi_v<T> / 2))
			{
				return -1;
			}

			if (std::numeric_limits<T>::min() > math::abs(value - std::numbers::pi_v<T>))
			{
				return 0;
			}

			if (std::numeric_limits<T>::min() > math::abs(value + std::numbers::pi_v<T>))
			{
				return -0;
			}

			// sin(x) = 2tan(x/2) / (1 + tan²(x/2))
			const auto z = math::tan(value / static_cast<T>(2));
			return (static_cast<T>(2) * z) / (static_cast<T>(1) + z * z);
		}
#endif

		return static_cast<T>(std::sin(value));
	}

	template<typename T>
		requires std::is_arithmetic_v<T>
	[[nodiscard]] constexpr auto cos(const T value) noexcept -> T
	{
#ifdef CMATH_WORKAROUND_REQUIRED
		PROMETHEUS_SEMANTIC_IF_CONSTANT_EVALUATED
		{
			if (math::is_nan(value))
			{
				return std::numeric_limits<T>::quiet_NaN();
			}

			if (std::numeric_limits<T>::min() > math::abs(value - std::numbers::pi_v<T> / 2))
			{
				return 0;
			}

			if (std::numeric_limits<T>::min() > math::abs(value + std::numbers::pi_v<T> / 2))
			{
				return -0;
			}

			if (std::numeric_limits<T>::min() > math::abs(value - std::numbers::pi_v<T>))
			{
				return -1;
			}

			if (std::numeric_limits<T>::min() > math::abs(value + std::numbers::pi_v<T>))
			{
				return -1;
			}

			// cos(x) = (1 - tan²(x/2)) / (1 + tan²(x/2))
			const auto z = math::tan(value / static_cast<T>(2));
			return (static_cast<T>(1) - z * z) / (static_cast<T>(1) + z * z);
		}
#endif

		return static_cast<T>(std::cos(value));
	}
}

#undef CMATH_WORKAROUND_REQUIRED
