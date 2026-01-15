cmake_minimum_required(VERSION 3.25)

set(
	PROMETHEUS_CORE_MAJOR_VERSION 
	1
	CACHE STRING 
	"PROMETHEUS-CORE major version"
)
set(
	PROMETHEUS_CORE_MINOR_VERSION 
	1 
	CACHE STRING 
	"PROMETHEUS-CORE minor version"
)
set(
	PROMETHEUS_CORE_PATCH_VERSION 
	1
	CACHE STRING 
	"PROMETHEUS-CORE patch version"
)
set(
	PROMETHEUS_CORE_VERSION 
	"${PROMETHEUS_CORE_MAJOR_VERSION}.${PROMETHEUS_CORE_MINOR_VERSION}.${PROMETHEUS_CORE_PATCH_VERSION}" 
	CACHE STRING 
	"PROMETHEUS-CORE version"
)

project(
		PrometheusCore
		VERSION ${PROMETHEUS_CORE_VERSION}
		DESCRIPTION "prometheus-core"
		HOMEPAGE_URL "https://github.com/Life4gal/core"
		LANGUAGES CXX
)

# ===================================================================================================
# OPTION

option(PROMETHEUS_CORE_TEST "Generate the test target." ON)
option(PROMETHEUS_CORE_INSTALL "Install targets and generate package config." ${PROJECT_IS_TOP_LEVEL})

# ===================================================================================================
# DEFINES

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/defines.cmake)

# ===================================================================================================
# DEPENDENCIES

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/find_cpm.cmake)

# packageProject
#CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.13.0")

# ===================================================================================================
# PACK

set(PROMETHEUS_CORE_VERSION_HEADER_FILE ${CMAKE_CURRENT_BINARY_DIR}/include/prometheus/version-core.hpp)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.hpp.in
	${PROMETHEUS_CORE_VERSION_HEADER_FILE}
	@ONLY
)

# ===================================================================================================
# LIBRARY

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_POSITION_INDEPENDENT_CODE ON)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(${PROJECT_NAME})

set_target_properties(
	${PROJECT_NAME} PROPERTIES
	VERSION ${PROMETHEUS_CORE_VERSION}
	SOVERSION ${PROMETHEUS_CORE_MAJOR_VERSION}
)

target_include_directories(
	${PROJECT_NAME} 

	PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	# PROMETHEUS_CORE_VERSION_HEADER_FILE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

target_compile_options(
	${PROJECT_NAME}

	PUBLIC
	${PROMETHEUS_COMPILE_FLAGS}
)

#target_compile_definitions(
#	${PROJECT_NAME}
#	PUBLIC
#	
#)

target_compile_features(
	${PROJECT_NAME}
	PUBLIC
	cxx_std_23
)

#target_link_libraries(
#    ${PROJECT_NAME} 
#    PUBLIC 
#
#)

set(
	PROMETHEUS_CORE_SOURCE_FILES_PUBLIC

	# =========================
	# META
	# =========================

	${PROJECT_SOURCE_DIR}/include/prometheus/meta/name.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/meta/string.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/meta/enumeration.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/meta/member.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/meta/member.visit.inl
	${PROJECT_SOURCE_DIR}/include/prometheus/meta/to_string.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/meta/dimension.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/meta/dimension.cache.inl

	${PROJECT_SOURCE_DIR}/include/prometheus/meta/meta.hpp
	
	# =========================
	# PLATFORM
	# =========================

	${PROJECT_SOURCE_DIR}/include/prometheus/platform/exception.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/platform/os.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/platform/cpu.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/platform/environment.hpp

	${PROJECT_SOURCE_DIR}/include/prometheus/platform/platform.hpp

	# =========================
	# FUNCTIONAL
	# =========================

	${PROJECT_SOURCE_DIR}/include/prometheus/functional/type_list.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/functional/value_list.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/functional/functor.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/functional/aligned_union.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/functional/function_ref.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/functional/hash.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/functional/enumeration.hpp

	${PROJECT_SOURCE_DIR}/include/prometheus/functional/functional.hpp

	# =========================
	# MATH
	# =========================

	${PROJECT_SOURCE_DIR}/include/prometheus/math/cmath.hpp

	${PROJECT_SOURCE_DIR}/include/prometheus/math/math.hpp
	
	# =========================
	# MEMORY
	# =========================

	${PROJECT_SOURCE_DIR}/include/prometheus/memory/rw.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/memory/unique_ptr.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/memory/reference_wrapper.hpp

	${PROJECT_SOURCE_DIR}/include/prometheus/memory/memory.hpp
	
	# =========================
	# NUMERIC
	# =========================

	${PROJECT_SOURCE_DIR}/include/prometheus/numeric/random_engine.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/numeric/random.hpp

	${PROJECT_SOURCE_DIR}/include/prometheus/numeric/numeric.hpp

	# =========================
	# PRIMITIVE
	# =========================

	${PROJECT_SOURCE_DIR}/include/prometheus/primitive/point.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/primitive/extent.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/primitive/rect.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/primitive/circle.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/primitive/ellipse.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/primitive/color.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/primitive/vertex.hpp

	${PROJECT_SOURCE_DIR}/include/prometheus/primitive/primitive.hpp

	# =========================
	# STRING
	# =========================

	${PROJECT_SOURCE_DIR}/include/prometheus/string/charconv.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/string/string_pool.hpp

	${PROJECT_SOURCE_DIR}/include/prometheus/string/string.hpp
)

set(
	PROMETHEUS_CORE_SOURCE_FILES_PRIVATE

	# =========================
	# PLATFORM
	# =========================

	${PROJECT_SOURCE_DIR}/src/platform/exception.cpp
	${PROJECT_SOURCE_DIR}/src/platform/os.cpp
	${PROJECT_SOURCE_DIR}/src/platform/cpu.cpp
	${PROJECT_SOURCE_DIR}/src/platform/environment.cpp
)

set_source_files_properties(
	${PROMETHEUS_CORE_SOURCE_FILES_PUBLIC}
	${PROMETHEUS_CORE_SOURCE_FILES_PRIVATE}
	PROPERTIES
	LANGUAGE CXX
)

target_sources(
	${PROJECT_NAME}
	PUBLIC
	FILE_SET public_header_files
	TYPE HEADERS
	BASE_DIRS 
	${CMAKE_CURRENT_SOURCE_DIR}/include
	# version header file
	${CMAKE_CURRENT_BINARY_DIR}/include
	FILES
	${PROMETHEUS_CORE_SOURCE_FILES_PUBLIC}
	${PROMETHEUS_CORE_VERSION_HEADER_FILE}
)

target_sources(
	${PROJECT_NAME}
	PRIVATE
	${PROMETHEUS_CORE_SOURCE_FILES_PRIVATE}
)

target_sources(
		${PROJECT_NAME}
		PUBLIC
		FILE_SET macro_header_files
		TYPE HEADERS
		BASE_DIRS 
		${CMAKE_CURRENT_SOURCE_DIR}/include
		FILES
		${CMAKE_CURRENT_SOURCE_DIR}/include/prometheus/macro.hpp
)

# ===================================================================================================
# TESTS

if (PROMETHEUS_CORE_TEST)
	enable_testing()
	add_subdirectory(test)
endif (PROMETHEUS_CORE_TEST)

# ===================================================================================================
# INSTALL

if (PROMETHEUS_CORE_INSTALL)
	include(GNUInstallDirs)
	include(CMakePackageConfigHelpers)

	install(
		TARGETS ${PROJECT_NAME}
		EXPORT ${PROJECT_NAME}Targets
		FILE_SET public_header_files DESTINATION include
		FILE_SET macro_header_files DESTINATION include
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		INCLUDES DESTINATION include
	)

	install(
		EXPORT ${PROJECT_NAME}Targets
		FILE ${PROJECT_NAME}Targets.cmake
		NAMESPACE ${PROJECT_NAME}::
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
	)

	configure_package_config_file(
		${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
		${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
		INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
	)

	write_basic_package_version_file(
		${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
		VERSION ${PROMETHEUS_CORE_VERSION}
		COMPATIBILITY SameMajorVersion
	)

	install(
		FILES
		 ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
		 ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
	)

	configure_file(
		${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}.pc.in
		${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
		@ONLY
	)

	install(
		FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
	)

	set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
	set(CPACK_PACKAGE_VENDOR ${PROJECT_NAME})
	set(CPACK_PACKAGE_VERSION ${PROMETHEUS_CORE_VERSION})
	set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Prometheus Core Library")
	set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
	set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
	set(CPACK_RPM_PACKAGE_REQUIRES "")

	include(CPack)
endif (PROMETHEUS_CORE_INSTALL)
