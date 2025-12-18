// This file is part of prometheus
// Copyright (C) 2022-2025 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <bit>
#include <cstring>
#include <type_traits>

#include <prometheus/macro.hpp>

#include <prometheus/functional/type_list.hpp>
#include <prometheus/platform/os.hpp>

namespace prometheus::memory
{
	/// @brief Loads an arithmetic value from potentially unaligned memory
	/// @tparam T Type to load (must be arithmetic)
	/// @tparam In Source pointer type (must be byte-like)
	/// @param source Pointer to source memory (must not be null)
	/// @return Value loaded from memory with proper byte order handling
	/// @note Uses memcpy in runtime for optimal performance, 
	///       manual byte-by-byte assembly in constexpr context. 
	///       This is safe for unaligned access unlike direct pointer dereferencing.
	template<typename T, typename In>
		requires std::is_arithmetic_v<T> and
		         (
			         functional::type_list<
				         char, const char,
				         signed char, const signed char,
				         unsigned char, const unsigned char,
				         std::byte, const std::byte //
			         >.any<In>()
		         )
	[[nodiscard]] constexpr auto unaligned_load(const In* source) noexcept -> T
	{
		PROMETHEUS_PLATFORM_ASSUME(source != nullptr, "Cannot unaligned_load from null!");

		T result{};

		// Runtime optimization: use memcpy for efficient unaligned access
		PROMETHEUS_SEMANTIC_IF_NOT_CONSTANT_EVALUATED
		{
			std::memcpy(&result, source, sizeof(T));
			return result;
		}

		// Constexpr implementation: manual byte assembly with endian awareness
		if constexpr (std::endian::native == std::endian::little) // NOLINT
		{
			// Little-endian: LSB at lowest address
			for (std::size_t i = sizeof(T); i != 0; --i)
			{
				if constexpr (sizeof(result) > 1)
				{
					result <<= 8;
				}
				result |= static_cast<std::uint8_t>(source[i - 1]);
			}
		}
		else
		{
			// Big-endian: MSB at lowest address
			for (std::size_t i = 0; i != sizeof(T); ++i)
			{
				if constexpr (sizeof(result) > 1)
				{
					result <<= 8;
				}
				result |= static_cast<std::uint8_t>(source[i]);
			}
		}

		return result;
	}

	/// @brief Convenience overload for void* source pointer
	template<typename T>
		requires std::is_arithmetic_v<T>
	[[nodiscard]] constexpr auto unaligned_load(const void* source) noexcept -> T
	{
		return memory::unaligned_load<T>(static_cast<const std::byte*>(source));
	}

	/// @brief Stores an arithmetic value to potentially unaligned memory
	/// @tparam T Type to store (must be arithmetic)
	/// @tparam Out Destination pointer type (must be byte-like)
	/// @param value Value to store
	/// @param dest Pointer to destination memory (must not be null)
	/// @note Converts to unsigned type before storing to avoid sign-extension issues
	///       and ensure consistent behavior across platforms.
	template<typename T, typename Out>
		requires std::is_arithmetic_v<T> and
		         (
			         functional::type_list<
				         char, const char,
				         signed char, const signed char,
				         unsigned char, const unsigned char,
				         std::byte, const std::byte //
			         >.any<Out>()
		         )
	constexpr auto unaligned_store(const T value, Out* dest) noexcept -> void
	{
		PROMETHEUS_PLATFORM_ASSUME(dest != nullptr, "Cannot unaligned_store from null!");

		using unsigned_type = std::make_unsigned_t<T>;

		auto unsigned_value = static_cast<unsigned_type>(value);

		// Runtime optimization: use memcpy for efficient unaligned access
		PROMETHEUS_SEMANTIC_IF_NOT_CONSTANT_EVALUATED
		{
			std::memcpy(dest, &unsigned_value, sizeof(T));
			return;
		}

		// Constexpr implementation: manual byte disassembly with endian awareness
		if constexpr (std::endian::native == std::endian::little) // NOLINT
		{
			// Little-endian: store LSB first
			for (std::size_t i = 0; i != sizeof(T); ++i)
			{
				dest[i] = static_cast<Out>(unsigned_value);
				unsigned_value >>= 8;
			}
		}
		else
		{
			// Big-endian: store MSB first
			for (std::size_t i = sizeof(T); i != 0; --i)
			{
				dest[i - 1] = static_cast<Out>(unsigned_value);
				unsigned_value >>= 8;
			}
		}
	}

	/// @brief Convenience overload for void* destination pointer
	template<typename T>
		requires std::is_arithmetic_v<T>
	constexpr auto unaligned_store(const T value, void* dest) noexcept -> void
	{
		memory::unaligned_store<T>(value, static_cast<std::byte*>(dest));
	}
}
