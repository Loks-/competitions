cmake_minimum_required( VERSION 3.5 )
project( competitions )
string( TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE_LOWERCASE )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_HOME_DIRECTORY}/${BUILD_TYPE_LOWERCASE}/build" )
include_directories( PRIVATE "${PROJECT_SOURCE_DIR}" )

file( GLOB_RECURSE GOOGLE_DISTRIBUTED_CODE_JAM_SOURCES RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/google/distributed_code_jam/*.cpp" )
add_executable( google_distributed_code_jam ${GOOGLE_DISTRIBUTED_CODE_JAM_SOURCES} )
target_include_directories(google_distributed_code_jam PRIVATE "${PROJECT_SOURCE_DIR}/google/distributed_code_jam" )

file( GLOB_RECURSE GOOGLE_CODE_JAM_SOURCES RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/google/code_jam/*.cpp" )
list( SORT GOOGLE_CODE_JAM_SOURCES )
set( GOOGLE_CODE_JAM_PROXY_H "${PROJECT_SOURCE_DIR}/google/code_jam/proxy_gen.h" )
set( GOOGLE_CODE_JAM_PROXY_CXX "${PROJECT_SOURCE_DIR}/google/code_jam/proxy_gen.cxx" )
file ( WRITE ${GOOGLE_CODE_JAM_PROXY_H}
	"#pragma once\n\n" )
file ( WRITE ${GOOGLE_CODE_JAM_PROXY_CXX}
	"#include \"common/proxy_run_base.h\"\n"
    "#include \"proxy_gen.h\"\n"
	"\n"
	"TSolutionsMap solutions_map {\n")
foreach( problemsourcefile ${GOOGLE_CODE_JAM_SOURCES} )
    string( REPLACE ".cpp" "" problemfullname ${problemsourcefile} )
    get_filename_component( problembasename ${problemfullname} NAME )
    if ( NOT ${problembasename} MATCHES "main")
        file ( APPEND ${GOOGLE_CODE_JAM_PROXY_H}
			"int main_${problembasename}();\n" )
        file ( APPEND ${GOOGLE_CODE_JAM_PROXY_CXX}
			"\t{ \"${problembasename}\", main_${problembasename} },\n")
    endif ( NOT ${problembasename} MATCHES "main")
endforeach( problemsourcefile ${GOOGLE_CODE_JAM_SOURCES} )
file ( APPEND ${GOOGLE_CODE_JAM_PROXY_CXX} "};\n" )
add_executable( google_code_jam ${GOOGLE_CODE_JAM_SOURCES} ${GOOGLE_CODE_JAM_PROXY_CXX} )

file( GLOB_RECURSE HACKERRANK_SOURCES RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/hackerrank/*.cpp" )
list( SORT HACKERRANK_SOURCES )
set( HACKERRANK_PROXY_H "${PROJECT_SOURCE_DIR}/hackerrank/proxy_gen.h" )
set( HACKERRANK_PROXY_CXX "${PROJECT_SOURCE_DIR}/hackerrank/proxy_gen.cxx" )
file ( WRITE ${HACKERRANK_PROXY_H}
	"#pragma once\n\n" )
file ( WRITE ${HACKERRANK_PROXY_CXX}
	"#include \"common/proxy_run_base.h\"\n"
    "#include \"proxy_gen.h\"\n"
	"\n"
	"TSolutionsMap solutions_map {\n")
foreach( problemsourcefile ${HACKERRANK_SOURCES} )
    string( REPLACE ".cpp" "" problemfullname ${problemsourcefile} )
    get_filename_component( problembasename ${problemfullname} NAME )
    if ( NOT ${problembasename} MATCHES "main")
        file ( APPEND ${HACKERRANK_PROXY_H}
			"int main_${problembasename}();\n" )
        file ( APPEND ${HACKERRANK_PROXY_CXX}
			"\t{ \"${problembasename}\", main_${problembasename} },\n")
    endif ( NOT ${problembasename} MATCHES "main")
endforeach( problemsourcefile ${HACKERRANK_SOURCES} )
file ( APPEND ${HACKERRANK_PROXY_CXX} "};\n" )
add_executable( hackerrank ${HACKERRANK_SOURCES} ${HACKERRANK_PROXY_CXX} )

file( GLOB_RECURSE IPCS_SOURCES RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/ipcs/*.cpp" )
list( SORT IPCS_SOURCES )
set( IPCS_PROXY_H "${PROJECT_SOURCE_DIR}/ipcs/proxy_gen.h" )
set( IPCS_PROXY_CXX "${PROJECT_SOURCE_DIR}/ipcs/proxy_gen.cxx" )
file ( WRITE ${IPCS_PROXY_H}
	"#pragma once\n\n" )
file ( WRITE ${IPCS_PROXY_CXX}
	"#include \"common/proxy_run_base.h\"\n"
    "#include \"proxy_gen.h\"\n"
	"\n"
	"TSolutionsMap solutions_map {\n")
foreach( problemsourcefile ${IPCS_SOURCES} )
    string( REPLACE ".cpp" "" problemfullname ${problemsourcefile} )
    get_filename_component( problembasename ${problemfullname} NAME )
    if ( NOT ${problembasename} MATCHES "main")
        file ( APPEND ${IPCS_PROXY_H}
			"int main_${problembasename}();\n" )
        file ( APPEND ${IPCS_PROXY_CXX}
			"\t{ \"${problembasename}\", main_${problembasename} },\n")
    endif ( NOT ${problembasename} MATCHES "main")
endforeach( problemsourcefile ${IPCS_SOURCES} )
file ( APPEND ${IPCS_PROXY_CXX} "};\n" )
add_executable( ipcs ${IPCS_SOURCES} ${IPCS_PROXY_CXX} )

file( GLOB_RECURSE TIMUS_SOURCES RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/timus/*.cpp" )
list( SORT TIMUS_SOURCES )
set( TIMUS_PROXY_H "${PROJECT_SOURCE_DIR}/timus/proxy_gen.h" )
set( TIMUS_PROXY_CXX "${PROJECT_SOURCE_DIR}/timus/proxy_gen.cxx" )
file ( WRITE ${TIMUS_PROXY_H}
	"#pragma once\n\n" )
file ( WRITE ${TIMUS_PROXY_CXX}
	"#include \"common/proxy_run_base.h\"\n"
    "#include \"proxy_gen.h\"\n"
	"\n"
	"TSolutionsMap solutions_map {\n")
foreach( problemsourcefile ${TIMUS_SOURCES} )
    string( REPLACE ".cpp" "" problemfullname ${problemsourcefile} )
    get_filename_component( problembasename ${problemfullname} NAME )
    if ( NOT ${problembasename} MATCHES "main")
        file ( APPEND ${TIMUS_PROXY_H}
			"int main_${problembasename}();\n" )
        file ( APPEND ${TIMUS_PROXY_CXX}
			"\t{ \"${problembasename}\", main_${problembasename} },\n")
    endif ( NOT ${problembasename} MATCHES "main")
endforeach( problemsourcefile ${TIMUS_SOURCES} )
file ( APPEND ${TIMUS_PROXY_CXX} "};\n" )
add_executable( timus ${TIMUS_SOURCES} ${TIMUS_PROXY_CXX} )

file( GLOB_RECURSE TESTER_SOURCES RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/tester/*.cpp" )
add_executable( tester ${TESTER_SOURCES} )

add_executable( include_headers "${PROJECT_SOURCE_DIR}/utils/include_headers.cpp" )
# add_executable( current_solution "${PROJECT_SOURCE_DIR}/utils/current_solution.cpp" )

enable_testing()

file( GLOB_RECURSE GOOGLE_CODE_JAM_TESTS RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/tests/google/code_jam/*.in" )
list( SORT GOOGLE_CODE_JAM_TESTS )
foreach( testfile_in ${GOOGLE_CODE_JAM_TESTS} )
    string( REPLACE ".in" "" testfullname ${testfile_in} )
    string( REPLACE ".in" ".out" testfile_out ${testfile_in} )
    get_filename_component( testshortname ${testfullname} NAME )
    get_filename_component( solution ${testshortname} NAME_WE )
	add_test( NAME ${testshortname} COMMAND google_code_jam ${solution} ${testfile_in} ${testfile_out} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
endforeach( testfile_in ${GOOGLE_CODE_JAM_TESTS} )

file( GLOB_RECURSE HACKERRANK_TESTS RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/tests/hackerrank/*.in" )
list( SORT HACKERRANK_TESTS )
foreach( testfile_in ${HACKERRANK_TESTS} )
    string( REPLACE ".in" "" testfullname ${testfile_in} )
    string( REPLACE ".in" ".out" testfile_out ${testfile_in} )
    get_filename_component( testshortname ${testfullname} NAME )
    get_filename_component( solution ${testshortname} NAME_WE )
	add_test( NAME ${testshortname} COMMAND hackerrank ${solution} ${testfile_in} ${testfile_out} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
endforeach( testfile_in ${HACKERRANK_TESTS} )

file( GLOB_RECURSE TIMUS_TESTS RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/tests/timus/*.in" )
list( SORT TIMUS_TESTS )
foreach( testfile_in ${TIMUS_TESTS} )
    string( REPLACE ".in" "" testfullname ${testfile_in} )
    string( REPLACE ".in" ".out" testfile_out ${testfile_in} )
    get_filename_component( testshortname ${testfullname} NAME )
    get_filename_component( solution ${testshortname} NAME_WE )
	add_test( NAME "timus.${testshortname}" COMMAND timus ${solution} ${testfile_in} ${testfile_out} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
endforeach( testfile_in ${TIMUS_TESTS} )

add_test( NAME tester_binary_search_tree COMMAND tester binary_search_tree )
add_test( NAME tester_convergent COMMAND tester convergent )
add_test( NAME tester_interpolation COMMAND tester interpolation )
add_test( NAME tester_mertens COMMAND tester mertens )
