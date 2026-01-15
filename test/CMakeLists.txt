CPMAddPackage(
		NAME PrometheusUT
		GIT_TAG main
		GITHUB_REPOSITORY "Life4gal/ut"
		OPTIONS 
		"PROMETHEUS_UT_TEST OFF"
		"PROMETHEUS_UT_INSTALL OFF"
)

CPMAddPackage(
		NAME PrometheusCLP
		GIT_TAG main
		GITHUB_REPOSITORY "Life4gal/clp"
		OPTIONS 
		"PROMETHEUS_CLP_TEST OFF"
		"PROMETHEUS_CLP_INSTALL OFF"
)

project(
	PrometheusCoreTest
	LANGUAGES CXX
)

add_executable(
	${PROJECT_NAME} 
	
	${CMAKE_CURRENT_SOURCE_DIR}/meta/enumeration.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/meta/member.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/meta/dimension.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/functional/aligned_union.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/functional/function_ref.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/string/string_pool.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
) 

target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
	PrometheusCLP
	PrometheusUT
)

add_test(
		NAME ${PROJECT_NAME}
		COMMAND ${PROJECT_NAME}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
