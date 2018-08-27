cmake_minimum_required( VERSION 3.5 )
project( competitions )
string( TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE_LOWERCASE )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/${BUILD_TYPE_LOWERCASE}/build )
include_directories( PRIVATE "${PROJECT_SOURCE_DIR}" )

file( GLOB_RECURSE GOOGLE_DISTRIBUTED_CODE_JAM_SOURCES RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/google/distributed_code_jam/*.cpp" ) 
add_executable( google_distributed_code_jam ${GOOGLE_DISTRIBUTED_CODE_JAM_SOURCES} )
target_include_directories(google_distributed_code_jam PRIVATE "${PROJECT_SOURCE_DIR}/google/distributed_code_jam" )

file( GLOB_RECURSE GOOGLE_CODE_JAM_SOURCES RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/google/code_jam/*.cpp" ) 
add_executable( google_code_jam ${GOOGLE_CODE_JAM_SOURCES} )

file( GLOB_RECURSE HACKERRANK_SOURCES RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/hackerrank/*.cpp" ) 
add_executable( hackerrank ${HACKERRANK_SOURCES} )

file( GLOB_RECURSE TESTER_SOURCES RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/tester/*.cpp" ) 
add_executable( tester ${TESTER_SOURCES} )

add_executable( include_headers ${PROJECT_SOURCE_DIR}/utils/include_headers.cpp )
add_executable( current_solution ${PROJECT_SOURCE_DIR}/utils/current_solution.cpp )
