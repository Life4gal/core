// This file is part of prometheus
// Copyright (C) 2022-2025 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <prometheus/platform/exception.hpp>

// std::println(FILE*)
#include <print>
// std::println(ostream&)
#include <ostream>

namespace prometheus::platform
{
	namespace
	{
		template<typename Out>
		auto do_print(Out& out, const IException& exception) noexcept -> void //
			requires requires { std::println(out); }
		{
			const auto& message = exception.what();
			const auto& location = exception.where();
			const auto& stacktrace = exception.when();

			std::println(
				out,
				"Error occurs while invoke function:\n{}\nat {}:{}\nReason:\n{}\nStack trace:\n{}",
				location.function_name(),
				location.file_name(),
				location.line(),
				message,
				stacktrace
			);
		}
	}

	auto IException::print(std::FILE* stream) const noexcept -> void
	{
		do_print(stream, *this);
	}

	auto IException::print(std::ostream& os) const noexcept -> void
	{
		do_print(os, *this);
	}
}
