// This file is part of prometheus
// Copyright (C) 2022-2025 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <string>
#include <source_location>
#include <stacktrace>

#if __has_include(<debugging>)
#include <debugging>
#define HAS_STD_DEBUGGING
#endif

#include <prometheus/platform/exception.hpp>

namespace prometheus::platform
{
	/**
     * @brief Retrieves the last OS error as a human-readable string.
     * @return std::string Description of the last OS error.
     * 
     * This function retrieves the last error from the operating system
     * and converts it to a string using the system's error category.
     * On Windows, it uses GetLastError(), on Unix-like systems, it uses errno.
     */
	[[nodiscard]] auto os_error_reason() -> std::string;

	/**
     * @brief Exception class for operating system errors.
     * 
     * Represents errors that originate from OS/system calls.
     * Automatically captures the last OS error when thrown.
     */
	class OsError final : public Exception<void>
	{
	public:
		using Exception::Exception;

		/**
        * @brief Throws an OsError with the current OS error message.
        */
		[[noreturn]] static auto panic(
			const std::source_location& location = std::source_location::current(),
			std::stacktrace stacktrace = std::stacktrace::current()
		) noexcept(false) -> void //
		{
			platform::panic<OsError>(os_error_reason(), location, std::move(stacktrace));
		}
	};

	/**
     * @brief Checks if a debugger is attached to the current process.
     * @return true if a debugger is present, false otherwise.
     * 
     * Uses platform-specific APIs to detect debugger presence.
     * Falls back to std::is_debugger_present() when <debugging> is available.
     */
	[[nodiscard]] auto is_debugger_present() noexcept -> bool
#ifdef HAS_STD_DEBUGGING
		{
			return std::is_debugger_present();
		}
#else
	;
#endif

	/**
     * @brief Prints a breakpoint message to stderr.
     * @param message Message to display before breaking.
     * 
     * This function only prints the message; actual breakpoint
     * triggering is done by the caller.
     */
	auto breakpoint_message(std::string_view message) noexcept -> void;

#ifdef HAS_STD_DEBUGGING
	/**
     * @brief Triggers a breakpoint if a debugger is present.
     * @param message Message to display before breaking.
     * 
     * If a debugger is attached, this will break into it.
     * Otherwise, it has no effect (execution continues).
     */
	inline auto breakpoint_if_debugging(const std::string_view message) noexcept -> void
	{
		breakpoint_message(message);
		std::breakpoint_if_debugging();
	}
#else
	// use PROMETHEUS_PLATFORM_BREAKPOINT_IF
#endif

#ifdef HAS_STD_DEBUGGING
	/**
     * @brief Breaks into debugger if present, otherwise terminates.
     * @param message Message to display before breaking/terminating.
     * 
     * This is useful for fatal errors where you want to debug if possible,
     * but terminate cleanly if not being debugged.
     */
	// inline auto breakpoint_or_terminate(const std::string_view message) noexcept -> void
	// {
	// 	breakpoint_message(message);
	// 	if (is_debugger_present())
	// 	{
	// 		PROMETHEUS_COMPILER_DEBUG_TRAP();
	// 	}
	// 	else
	// 	{
	// 		std::terminate();
	// 	}
	// }
#else
	// use PROMETHEUS_PLATFORM_BREAKPOINT_OR_TERMINATE_IF
#endif
}
