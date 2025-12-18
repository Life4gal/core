// This file is part of prometheus
// Copyright (C) 2022-2025 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <prometheus/platform/os.hpp>

#include <string>
#include <system_error>
#include <print>

#ifndef HAS_STD_DEBUGGING

#ifdef PROMETHEUS_PLATFORM_WINDOWS

#include <Windows.h>
#include <debugapi.h>

#elifdef PROMETHEUS_PLATFORM_LINUX

#include <cerrno>
#include <cstring>

#include <fstream>
#include <prometheus/string/charconv.hpp>

#elif defined(PROMETHEUS_PLATFORM_DARWIN)

#include <cerrno>
#include <cstring>

#include <sys/types.h>
#include <sys/sysctl.h>
#include <unistd.h>

#else

#error "fixme"

#endif

#endif

namespace prometheus::platform
{
	auto os_error_reason() -> std::string
	{
		const auto error_code =
#ifdef PROMETHEUS_PLATFORM_WINDOWS
						static_cast<int>(GetLastError())
#else
				errno
#endif
				;

		return std::system_category().message(error_code);
	}

#ifndef HAS_STD_DEBUGGING

	auto is_debugger_present() noexcept -> bool
	{
#ifdef PROMETHEUS_PLATFORM_WINDOWS

		// Check for local debugger
		if (IsDebuggerPresent() == TRUE)
		{
			return true;
		}

		// Check for remote debugger
		BOOL is_remote_debugger_present = FALSE;
		if (CheckRemoteDebuggerPresent(GetCurrentProcess(), &is_remote_debugger_present) == TRUE)
		{
			return is_remote_debugger_present == TRUE;
		}

		return false;

#elifdef PROMETHEUS_PLATFORM_LINUX

		// Check /proc/self/status for TracerPid
		std::ifstream status_file("/proc/self/status");
		if (not status_file.is_open())
		{
			return false;
		}

		std::string line;
		while (std::getline(status_file, line))
		{
			if (line.starts_with("TracerPid"))
			{
				// Extract PID value after "TracerPid:"
				const std::string_view sub{line.begin() + sizeof("TracerPid:") - 1, line.end()};

				if (const auto pid = string::from_string<int>(sub);
					// Non-zero TracerPid indicates a debugger is attached
					pid.has_value() && *pid != 0)
				{
					return true;
				}
			}
		}

		return false;

#elifdef PROMETHEUS_PLATFORM_DARWIN

		// Use sysctl to check for P_TRACED flag
		int mib[4];
		struct kinfo_proc info;
		size_t size = sizeof(info);

		mib[0] = CTL_KERN;
		mib[1] = KERN_PROC;
		mib[2] = KERN_PROC_PID;
		mib[3] = getpid();

		if (sysctl(mib, 4, &info, &size, NULL, 0) == -1)
		{
			return false;
		}

		return (info.kp_proc.p_flag & P_TRACED) != 0;

#else

#error "fixme"

#endif
	}

#endif

	auto breakpoint_message(std::string_view message) noexcept -> void
	{
		std::println(stderr, "BREAKPOINT: {}", message);
	}
}
