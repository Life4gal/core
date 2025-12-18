// This file is part of prometheus
// Copyright (C) 2022-2025 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <prometheus/platform/cpu.hpp>

#include <atomic>
#include <bit>
#include <mutex>

#include <prometheus/macro.hpp>

#if defined(PROMETHEUS_ARCH_X64) || defined(PROMETHEUS_ARCH_X86)

#ifdef PROMETHEUS_PLATFORM_WINDOWS

#include <intrin.h>

#elif defined(PROMETHEUS_PLATFORM_LINUX) || defined(PROMETHEUS_PLATFORM_DARWIN)

#include <cpuid.h>

#if defined(PROMETHEUS_PLATFORM_LINUX)

#include <x86intrin.h>

#endif

#endif

#elif defined(PROMETHEUS_ARCH_ARM64) || defined(PROMETHEUS_ARCH_ARM)

#ifdef PROMETHEUS_PLATFORM_WINDOWS


#elif defined(PROMETHEUS_PLATFORM_LINUX)


#if defined(PROMETHEUS_PLATFORM_DARWIN)


#endif

#endif

#else

#error "Unsupported CPU architecture"

#endif

namespace
{
	using prometheus::platform::InstructionSet;

	// Cache for CPU detection results
	std::atomic g_cached_instruction_set{InstructionSet::NONE};
	std::once_flag g_detection_flag;

#if defined(PROMETHEUS_ARCH_X64) || defined(PROMETHEUS_ARCH_X86)
	// CPUID result structure
	struct cpuid_result
	{
		std::uint32_t eax;
		std::uint32_t ebx;
		std::uint32_t ecx;
		std::uint32_t edx;
	};

	/**
     * @brief Executes CPUID instruction.
     * @param leaf CPUID leaf (EAX input).
     * @param subleaf CPUID sub-leaf (ECX input).
     * @return CpuidResult CPUID results.
     */
	[[nodiscard]] auto cpuid(const std::uint32_t leaf, const std::uint32_t subleaf = 0) noexcept -> cpuid_result
	{
		cpuid_result result;

#ifdef PROMETHEUS_PLATFORM_WINDOWS

		int registers[4];
		__cpuidex(registers, static_cast<int>(leaf), static_cast<int>(subleaf));
		result.eax = static_cast<std::uint32_t>(registers[0]);
		result.ebx = static_cast<std::uint32_t>(registers[1]);
		result.ecx = static_cast<std::uint32_t>(registers[2]);
		result.edx = static_cast<std::uint32_t>(registers[3]);

#elif defined(PROMETHEUS_COMPILER_GCC) || defined(PROMETHEUS_COMPILER_CLANG)

		__cpuid_count(leaf, subleaf, result.eax, result.ebx, result.ecx, result.edx);

#else

		// Fallback inline assembly
		asm volatile("cpuid" : "=a"(result.eax), "=b"(result.ebx), "=c"(result.ecx), "=d"(result.edx) : "a"(leaf), "c"(subleaf));

#endif

		return result;
	}

	/**
     * @brief Reads XCR0 (XFeature Enabled Register).
     * @return XCR0 register value.
     */
	[[nodiscard]] auto get_xcr0() noexcept -> std::uint64_t
	{
#ifdef PROMETHEUS_PLATFORM_WINDOWS

		return _xgetbv(0);

#elif defined(PROMETHEUS_COMPILER_GCC) || defined(PROMETHEUS_COMPILER_CLANG)

		std::uint32_t eax, edx;
		asm volatile("xgetbv" : "=a"(eax), "=d"(edx) : "c"(0));
		return (static_cast<std::uint64_t>(edx) << 32) | eax;

#else

		return 0; // Not supported

#endif
	}

	/**
     * @brief Detects x86/x64 CPU features.
     * @return Bitmask of supported instruction sets.
     */
	[[nodiscard]] auto detect_x86_features() noexcept -> InstructionSet
	{
		auto features = InstructionSet::NONE;

		// Check maximum supported CPUID leaf
		const auto max_basic_leaf = cpuid(0).eax;
		if (max_basic_leaf < 1)
		{
			// Very old CPU
			return features;
		}

		// Check basic features (CPUID leaf 1)
		const auto basic_features = cpuid(1);

		// Check for required features for further detection
		const bool has_osxsave = (basic_features.ecx & (1 << 27)) != 0;
		if (not has_osxsave)
		{
			// OS doesn't support extended states
			return features;
		}

		// Check individual features from leaf 1
		if (basic_features.ecx & (1 << 1))
		{
			// PCLMULQDQ
			features |= InstructionSet::PCLMULQDQ;
		}
		if (basic_features.ecx & (1 << 20))
		{
			// SSE4.2
			features |= InstructionSet::SSE42;
		}
		if (basic_features.ecx & (1 << 28))
		{
			// AVX
			features |= InstructionSet::AVX;
		}

		// Check OS support for AVX (XCR0)
		const auto xcr0 = get_xcr0();
		// Bits 1 and 2 (SSE and AVX)
		const bool os_supports_avx = (xcr0 & 0x6) == 0x6;

		if (not os_supports_avx)
		{
			return features; // OS doesn't save AVX registers
		}

		// Check extended features (CPUID leaf 7)
		if (max_basic_leaf >= 7)
		{
			const auto extended_features = cpuid(7, 0);

			if (extended_features.ebx & (1 << 3))
			{
				// BMI1
				features |= InstructionSet::BMI1;
			}
			if (extended_features.ebx & (1 << 5))
			{
				// AVX2
				features |= InstructionSet::AVX2;
			}
			if (extended_features.ebx & (1 << 8))
			{
				// BMI2
				features |= InstructionSet::BMI2;
			}

			// Check OS support for AVX-512
			// Bits 5,6,7
			const bool os_supports_avx512 = (xcr0 & 0xE0) == 0xE0;

			if (os_supports_avx512)
			{
				if (extended_features.ebx & (1 << 16))
				{
					// AVX512F
					features |= InstructionSet::AVX512F;
				}
				if (extended_features.ebx & (1 << 17))
				{
					// AVX512DQ
					features |= InstructionSet::AVX512DQ;
				}
				if (extended_features.ebx & (1 << 28))
				{
					// AVX512CD
					features |= InstructionSet::AVX512CD;
				}
				if (extended_features.ebx & (1 << 30))
				{
					// AVX512BW
					features |= InstructionSet::AVX512BW;
				}
				if (extended_features.ebx & (1 << 31))
				{
					// AVX512VL
					features |= InstructionSet::AVX512VL;
				}

				if (extended_features.ecx & (1 << 6))
				{
					// AVX512VBMI2
					features |= InstructionSet::AVX512VBMI2;
				}
				if (extended_features.ecx & (1 << 14))
				{
					// AVX512VPOPCNTDQ
					features |= InstructionSet::AVX512VPOPCNTDQ;
				}
			}
		}

		return features;
	}

	/**
     * @brief Gets CPU vendor string.
     */
	[[nodiscard]] auto get_x86_vendor() noexcept -> std::string
	{
		const auto result = cpuid(0);
		std::array<char, 13> vendor{};
		std::memcpy(vendor.data(), &result.ebx, 4);
		std::memcpy(vendor.data() + 4, &result.edx, 4);
		std::memcpy(vendor.data() + 8, &result.ecx, 4);
		vendor[12] = '\0';

		return std::string{vendor.data(), vendor.size()};
	}

	/**
     * @brief Gets CPU brand string.
     */
	[[nodiscard]] auto get_x86_brand() noexcept -> std::string
	{
		std::array<char, 49> brand{};
		for (std::ptrdiff_t i = 0; i < 3; ++i)
		{
			const auto result = cpuid(static_cast<std::uint32_t>(0x80000002 + i));
			std::memcpy(brand.data() + i * 16, &result.eax, 16);
		}
		brand[48] = '\0';

		// Trim trailing spaces
		std::string brand_str{brand.data(), brand.size()};
		if (const auto end_pos = brand_str.find_last_not_of(' ');
			end_pos != std::string::npos)
		{
			brand_str.resize(end_pos + 1);
		}
		return brand_str;
	}

#elif defined(PROMETHEUS_ARCH_ARM) || defined(PROMETHEUS_ARCH_ARM64)

	// ARM feature detection (placeholder)
	[[nodiscard]] auto detect_arm_features() noexcept -> InstructionSet
	{
		// TODO: Implement ARM feature detection
		return InstructionSet::NONE;
	}

	[[nodiscard]] auto get_arm_vendor() noexcept -> std::string
	{
		return "ARM";
	}

	[[nodiscard]] auto get_arm_brand() noexcept -> std::string
	{
		return "ARM Processor";
	}

#endif
}

namespace prometheus::platform
{
	auto detect_supported_instructions() -> InstructionSet
	{
		if (const auto cached = g_cached_instruction_set.load(std::memory_order_acquire);
			cached != InstructionSet::NONE)
		{
			return cached;
		}

		std::call_once(
			g_detection_flag,
			[]() noexcept -> void
			{
				auto detected = InstructionSet::NONE;

#if defined(PROMETHEUS_ARCH_X64) || defined(PROMETHEUS_ARCH_X86)

				detected = detect_x86_features();

#elif defined(PROMETHEUS_ARCH_ARM64) || defined(PROMETHEUS_ARCH_ARM)

				detected = detect_arm_features();

#else

				// Unknown architecture - assume baseline
				detected = InstructionSet::NONE;

#endif

				g_cached_instruction_set.store(detected, std::memory_order_release);
			}
		);

		return g_cached_instruction_set.load(std::memory_order_acquire);
	}

	auto is_instruction_set_supported(const InstructionSet set) noexcept -> bool
	{
		if (set == InstructionSet::NONE)
		{
			return true;
		}

		const auto supported = detect_supported_instructions();
		return (supported & set) == set;
	}

	auto get_max_simd_width() noexcept -> std::uint32_t
	{
		const auto features = detect_supported_instructions();

		if ((features & InstructionSet::AVX512F) == InstructionSet::AVX512F)
		{
			return 512;
		}

		if ((features & InstructionSet::AVX2) == InstructionSet::AVX2)
		{
			return 256;
		}

		if ((features & InstructionSet::AVX) == InstructionSet::AVX)
		{
			// AVX also provides 256-bit
			return 256;
		}

		// Default to 128-bit (SSE)
		return 128;
	}

	auto get_cpu_vendor() noexcept -> std::string
	{
#if defined(PROMETHEUS_ARCH_X64) || defined(PROMETHEUS_ARCH_X86)

		return get_x86_vendor();

#elif defined(PROMETHEUS_ARCH_ARM64) || defined(PROMETHEUS_ARCH_ARM)

		return get_arm_vendor();

#else

		return "Unknown";

#endif
	}

	auto get_cpu_brand() noexcept -> std::string
	{
#if defined(PROMETHEUS_ARCH_X64) || defined(PROMETHEUS_ARCH_X86)

		return get_x86_brand();

#elif defined(PROMETHEUS_ARCH_ARM64) || defined(PROMETHEUS_ARCH_ARM)

		return get_arm_brand();

#else

		return "Unknown Processor";

#endif
	}
}
