// This file is part of prometheus
// Copyright (C) 2022-2025 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <cstdint>

// for enumeration operations
#include <prometheus/functional/enumeration.hpp>

namespace prometheus::platform
{
	// ReSharper disable CppInconsistentNaming
	// ReSharper disable IdentifierTypo

	enum class InstructionSet : std::uint32_t // NOLINT(performance-enum-size)
	{
		NONE = 0b0000'0000'0000'0000,

		// Basic instruction sets
		SSE42 = 0b0000'0000'0000'0001,
		PCLMULQDQ = 0b0000'0000'0000'0010,

		// AVX/AVX2 instruction sets
		AVX = 0b0000'0000'0000'0100,
		AVX2 = 0b0000'0000'0000'1000,

		// Bit manipulation instructions
		BMI1 = 0b0000'0000'0001'0000,
		BMI2 = 0b0000'0000'0010'0000,

		// AVX-512 instruction sets (subset)
		// Foundation
		AVX512F = 0b0000'0000'0100'0000,
		// Double & Quadword
		AVX512DQ = 0b0000'0000'1000'0000,
		// Conflict Detection
		AVX512CD = 0b0000'0001'0000'0000,
		// Byte & Word
		AVX512BW = 0b0000'0010'0000'0000,
		// Vector Length
		AVX512VL = 0b0000'0100'0000'0000,
		// Vector Byte Manipulation 2
		AVX512VBMI2 = 0b0000'1000'0000'0000,
		// Population Count
		AVX512VPOPCNTDQ = 0b0001'0000'0000'0000,

		AVX512_CORE = AVX512F | AVX512DQ | AVX512CD | AVX512BW | AVX512VL,
		AVX512_ALL = AVX512_CORE | AVX512VBMI2 | AVX512VPOPCNTDQ,
		AVX_ALL = AVX | AVX2 | AVX512_CORE,
	};

	// ReSharper restore IdentifierTypo
	// ReSharper restore CppInconsistentNaming

	/**
     * @brief Detects all supported CPU instruction sets.
     * @return InstructionSet Bitmask of supported instruction sets.
     * 
     * This function performs CPUID calls to detect available instruction sets.
     * The result is cached after the first call for performance.
     */
	auto detect_supported_instructions() -> InstructionSet;

	/**
     * @brief Checks if a specific instruction set is supported.
     * @param set Instruction set to check.
     * @return true if the instruction set is supported, false otherwise.
     */
	[[nodiscard]] auto is_instruction_set_supported(InstructionSet set) noexcept -> bool;

	/**
    * @brief Gets the maximum SIMD width supported by the CPU.
    * @return Maximum SIMD width in bits (128, 256, or 512).
    */
	[[nodiscard]] auto get_max_simd_width() noexcept -> std::uint32_t;

	/**
     * @brief Gets the CPU vendor string.
     * @return CPU vendor string (e.g., "GenuineIntel", "AuthenticAMD").
     */
	[[nodiscard]] auto get_cpu_vendor() noexcept -> std::string;

	/**
     * @brief Gets the CPU brand string.
     * @return CPU brand string (e.g., "Intel(R) Core(TM) i7-10700K").
     */
	[[nodiscard]] auto get_cpu_brand() noexcept -> std::string;
}
