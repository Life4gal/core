# ===================================================================================================
# PLATFORM

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(
		PROMETHEUS_PLATFORM_WINDOWS 
		ON
		CACHE BOOL
		""
	)
	# ==> macro.hpp
	set(PROMETHEUS_PLATFORM_NAME PROMETHEUS_PLATFORM_WINDOWS)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(
		PROMETHEUS_PLATFORM_LINUX 
		ON
		CACHE BOOL
		""
	)
	# ==> macro.hpp
	set(PROMETHEUS_PLATFORM_NAME PROMETHEUS_PLATFORM_LINUX)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	set(
		PROMETHEUS_PLATFORM_DARWIN 
		ON
		CACHE BOOL
		""
	)
	# ==> macro.hpp
	set(PROMETHEUS_PLATFORM_NAME PROMETHEUS_PLATFORM_DARWIN)
else ()
	message(FATAL_ERROR "[PROMETHEUS] Unknown Platform: ${CMAKE_SYSTEM_NAME}")
endif (CMAKE_SYSTEM_NAME STREQUAL "Windows")

# ===================================================================================================
# ARCHITECTURE

if(CMAKE_SYSTEM_PROCESSOR)
	string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" PROC_LOWER)
	
	if(PROC_LOWER MATCHES "amd64|x86_64|x64|win64")
		set(
			PROMETHEUS_ARCH_X64 
			ON
			CACHE BOOL
			""
		)
		# ==> macro.hpp
		set(PROMETHEUS_ARCH_NAME PROMETHEUS_ARCH_X64)
	elseif(PROC_LOWER MATCHES "i.86|i86|i686|i586|i486|i386|x86")
		set(
			PROMETHEUS_ARCH_X86 
			ON
			CACHE BOOL
			""
		)
		# ==> macro.hpp
		set(PROMETHEUS_ARCH_NAME PROMETHEUS_ARCH_X86)
	elseif(PROC_LOWER MATCHES "aarch64|arm64|armv8")
		set(
			PROMETHEUS_ARCH_ARM64 
			ON
			CACHE BOOL
			""
		)
		# ==> macro.hpp
		set(PROMETHEUS_ARCH_NAME PROMETHEUS_ARCH_ARM64)
	elseif(PROC_LOWER MATCHES "arm.*")
		set(
			PROMETHEUS_ARCH_ARM 
			ON
			CACHE BOOL
			""
		)
		# ==> macro.hpp
		set(PROMETHEUS_ARCH_NAME PROMETHEUS_ARCH_ARM)
	else()
		set(
			PROMETHEUS_ARCH_UNKNOWN 
			ON
			CACHE BOOL
			""
		)
		# ==> macro.hpp
		set(PROMETHEUS_ARCH_NAME PROMETHEUS_ARCH_UNKNOWN)
	endif()
else()
	set(
		PROMETHEUS_ARCH_UNKNOWN 
		ON
		CACHE BOOL
		""
	)
	# ==> macro.hpp
	set(PROMETHEUS_ARCH_NAME PROMETHEUS_ARCH_UNKNOWN)
endif(CMAKE_SYSTEM_PROCESSOR)

# ===================================================================================================
# COMPILER

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set(
		PROMETHEUS_COMPILER_MSVC 
		ON
		CACHE BOOL
		""
	)
	# ==> macro.hpp
	set(PROMETHEUS_COMPILER_NAME PROMETHEUS_COMPILER_MSVC)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	if (CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
		set(
			PROMETHEUS_COMPILER_CLANG_CL 
			ON
			CACHE BOOL
			""
		)
		# ==> macro.hpp
		set(PROMETHEUS_COMPILER_NAME PROMETHEUS_COMPILER_CLANG_CL)
	else ()
		set(
			PROMETHEUS_COMPILER_CLANG 
			ON
			CACHE BOOL
			""
		)
		# ==> macro.hpp
		set(PROMETHEUS_COMPILER_NAME PROMETHEUS_COMPILER_CLANG)
	endif (CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(
		PROMETHEUS_COMPILER_GNU 
		ON
		CACHE BOOL
		""
	)
	# ==> macro.hpp
	set(PROMETHEUS_COMPILER_NAME PROMETHEUS_COMPILER_GNU)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
	set(
		PROMETHEUS_COMPILER_CLANG_APPLE 
		ON
		CACHE BOOL
		""
	)
	# ==> macro.hpp
	set(PROMETHEUS_COMPILER_NAME PROMETHEUS_COMPILER_CLANG_APPLE)
else ()
	message(FATAL_ERROR "[PROMETHEUS] Unknown compiler: ${CMAKE_CXX_COMPILER}")
endif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

# ===================================================================================================
# COMPILE FLAGS

if (PROMETHEUS_COMPILER_MSVC)
	set(
		PROMETHEUS_COMPILE_FLAGS 
		"/D_CRT_SECURE_NO_WARNINGS"
		"/DNOMINMAX"
		"/DWIN32_LEAN_AND_MEAN"
		"/DVC_EXTRALEAN"
		"/DSTRICT"
		"/utf-8"
		"/W3"
		"/WX"
		"/Zc:preprocessor"
		"/permissive-"
		CACHE STRING
		""
	)
elseif (PROMETHEUS_COMPILER_CLANG_CL)
	set(
		PROMETHEUS_COMPILE_FLAGS 
		"/D_CRT_SECURE_NO_WARNINGS"
		"/DNOMINMAX"
		"/DWIN32_LEAN_AND_MEAN"
		"/DVC_EXTRALEAN"
		"/DSTRICT"
		"/utf-8"
		"/W3"
		"/WX"
		"/permissive-"
		CACHE STRING
		""
	)
elseif (PROMETHEUS_COMPILER_CLANG)
	set(
		PROMETHEUS_COMPILE_FLAGS 
		"-Wall"
		"-Wextra"
		"-Wpedantic"
		"-Werror"
		"-Wconversion"
		"-Wshadow"
		"-Wold-style-cast"
		"-Wnull-dereference"
		"-Wdouble-promotion"
		CACHE STRING
		""
	)
elseif (PROMETHEUS_COMPILER_GNU)
	set(
		PROMETHEUS_COMPILE_FLAGS 
		"-Wall"
		"-Wextra"
		"-Wpedantic"
		"-Werror"
		"-Wconversion"
		"-Wshadow"
		"-Wold-style-cast"
		"-Wnull-dereference"
		"-Wlogical-op"
		"-Wduplicated-cond"
		"-Wduplicated-branches"
		CACHE STRING
		""
	)

	# ====================
	# <stacktrace>
	# https://gcc.gnu.org/onlinedocs/gcc-12.3.0/libstdc++/manual/manual/using.html#manual.intro.using.flags
	list(APPEND PROMETHEUS_COMPILE_FLAGS "-lstdc++_libbacktrace")
	# https://gcc.gnu.org/onlinedocs/libstdc++/manual/using.html#manual.intro.using.flags
	list(APPEND PROMETHEUS_COMPILE_FLAGS "-lstdc++exp")
elseif (PROMETHEUS_COMPILER_CLANG_APPLE)
	set(
		PROMETHEUS_COMPILE_FLAGS 
		"-Wall"
		"-Wextra"
		"-Wpedantic"
		"-Werror"
		"-Wconversion"
	)
endif (PROMETHEUS_COMPILER_MSVC)

# ===================================================================================================
# GIT

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/git.cmake)

# ===================================================================================================
# OUTPUT INFO

message(STATUS "")
message(STATUS "================================================================")
message(STATUS "  ${PROJECT_NAME} - v${PROMETHEUS_CORE_VERSION}")
message(STATUS "================================================================")
# PLATFORM + ARCHITECTURE + COMPILER + COMPILE FLAGS
message(STATUS "  Platform: ${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "  CMake Version: ${CMAKE_VERSION}")
message(STATUS "  Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  Compile Flags: ${PROMETHEUS_COMPILE_FLAGS}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Build Test: ${PROMETHEUS_CORE_TEST}")
# GIT
message(STATUS "  Git: ")
message(STATUS "      Branch: ${PROMETHEUS_CORE_GIT_BRANCH_NAME}")
message(STATUS "      Commit: ${PROMETHEUS_CORE_GIT_COMMIT_HASH}${PROMETHEUS_CORE_GIT_DIRTY_FLAG}")
message(STATUS "      Date: ${PROMETHEUS_CORE_GIT_COMMIT_DATE}")
message(STATUS "      Tag: ${PROMETHEUS_CORE_GIT_TAG}")
message(STATUS "      Status: ${PROMETHEUS_CORE_GIT_DIRTY_STATUS}")
message(STATUS "================================================================")
message(STATUS "")
