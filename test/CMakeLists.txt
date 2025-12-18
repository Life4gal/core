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

	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
) 

target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
	PrometheusCore
)
