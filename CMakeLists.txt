cmake_minimum_required( VERSION 3.5 )
project( competitions )
if (APPLE)
    set(CMAKE_C_COMPILER /opt/homebrew/bin/gcc-11)
    set(CMAKE_CXX_COMPILER /opt/homebrew/bin/g++-11)
endif (APPLE)
string( TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE_LOWERCASE )
set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_HOME_DIRECTORY}/${BUILD_TYPE_LOWERCASE}/build" )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -Warray-bounds=0 -Werror=stringop-overflow=0")
include_directories( PRIVATE "${PROJECT_SOURCE_DIR}" )
find_package( Threads )

file( GLOB_RECURSE ADVENT_SOURCES RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/advent_of_code/*.cpp" )
list( SORT ADVENT_SOURCES )
set( ADVENT_PROXY_H "${PROJECT_SOURCE_DIR}/advent_of_code/proxy_gen.h" )
set( ADVENT_PROXY_CXX "${PROJECT_SOURCE_DIR}/advent_of_code/proxy_gen.cxx" )
file ( WRITE ${ADVENT_PROXY_H}
	"#pragma once\n\n" )
file ( WRITE ${ADVENT_PROXY_CXX}
	"#include \"common/proxy_run_base.h\"\n"
    "#include \"proxy_gen.h\"\n"
	"\n"
	"TSolutionsMap solutions_map {\n")
foreach( problemsourcefile ${ADVENT_SOURCES} )
    string( REPLACE ".cpp" "" problemfullname ${problemsourcefile} )
    get_filename_component( problembasename ${problemfullname} NAME )
    if ( NOT ${problembasename} MATCHES "base|main")
        file ( APPEND ${ADVENT_PROXY_H}
			"int main_${problembasename}();\n" )
        file ( APPEND ${ADVENT_PROXY_CXX}
			"\t{ \"${problembasename}\", main_${problembasename} },\n")
    endif ( NOT ${problembasename} MATCHES "base|main")
endforeach( problemsourcefile ${ADVENT_SOURCES} )
file ( APPEND ${ADVENT_PROXY_CXX} "};\n" )
add_executable( advent_of_code ${ADVENT_SOURCES} ${ADVENT_PROXY_CXX} )

file( GLOB_RECURSE CODINGAME_PRACTICE_SOURCES RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/codingame/practice/*.cpp" )
add_executable( codingame_practice ${CODINGAME_PRACTICE_SOURCES} )

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
    if ( NOT ${problembasename} MATCHES "base|main")
        file ( APPEND ${GOOGLE_CODE_JAM_PROXY_H}
			"int main_${problembasename}();\n" )
        file ( APPEND ${GOOGLE_CODE_JAM_PROXY_CXX}
			"\t{ \"${problembasename}\", main_${problembasename} },\n")
    endif ( NOT ${problembasename} MATCHES "base|main")
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
    if ( NOT ${problembasename} MATCHES "base|main")
        file ( APPEND ${HACKERRANK_PROXY_H}
			"int main_${problembasename}();\n" )
        file ( APPEND ${HACKERRANK_PROXY_CXX}
			"\t{ \"${problembasename}\", main_${problembasename} },\n")
    endif ( NOT ${problembasename} MATCHES "base|main")
endforeach( problemsourcefile ${HACKERRANK_SOURCES} )
file ( APPEND ${HACKERRANK_PROXY_CXX} "};\n" )
add_executable( hackerrank ${HACKERRANK_SOURCES} ${HACKERRANK_PROXY_CXX} )

file( GLOB_RECURSE HACKERRANK_C_SOURCES RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/hackerrank/practice/language/c/*.c" )
list( SORT HACKERRANK_C_SOURCES )
set( HACKERRANK_PROXY_C_H "${PROJECT_SOURCE_DIR}/hackerrank/practice/language/c/proxy_gen.h" )
set( HACKERRANK_PROXY_C_C "${PROJECT_SOURCE_DIR}/hackerrank/practice/language/c/proxy_gen.c" )
file ( WRITE ${HACKERRANK_PROXY_C_H} "" )
file ( WRITE ${HACKERRANK_PROXY_C_C}
    "#include \"proxy_gen.h\"\n"
    "\n"
    "#include <stdio.h>\n"
    "#include <string.h>\n"
	"\n"
    "int ProxyRun(const char* solution_name) {\n"
    "  if (0) return 0;\n")
foreach( problemsourcefile ${HACKERRANK_C_SOURCES} )
    string( REPLACE ".c" "" problemfullname ${problemsourcefile} )
    get_filename_component( problembasename ${problemfullname} NAME )
    if ( NOT ${problembasename} MATCHES "main|proxy_gen" )
        file ( APPEND ${HACKERRANK_PROXY_C_H}
			"int main_${problembasename}();\n" )
        file ( APPEND ${HACKERRANK_PROXY_C_C}
            "  else if (strcmp(solution_name, \"${problembasename}\") == 0)\n"
            "    return main_${problembasename}();\n" )
    endif ( NOT ${problembasename} MATCHES "main|proxy_gen")
endforeach( problemsourcefile ${HACKERRANK_SOURCES} )
file ( APPEND ${HACKERRANK_PROXY_C_C}
    "  printf(\"Unknown solution name: %s\", solution_name);\n"
    "  return -2;\n}\n" )
add_executable( hackerrank_c ${HACKERRANK_C_SOURCES} ${HACKERRANK_PROXY_C_C} )
set_target_properties( hackerrank_c PROPERTIES COMPILE_FLAGS "-Wno-unused-result" )

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
    if ( NOT ${problembasename} MATCHES "base|main")
        file ( APPEND ${IPCS_PROXY_H}
			"int main_${problembasename}();\n" )
        file ( APPEND ${IPCS_PROXY_CXX}
			"\t{ \"${problembasename}\", main_${problembasename} },\n")
    endif ( NOT ${problembasename} MATCHES "base|main")
endforeach( problemsourcefile ${IPCS_SOURCES} )
file ( APPEND ${IPCS_PROXY_CXX} "};\n" )
add_executable( ipcs ${IPCS_SOURCES} ${IPCS_PROXY_CXX} )

file( GLOB_RECURSE LEETCODE_SOURCES RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/leetcode/*.cpp" )
list( SORT LEETCODE_SOURCES )
set( LEETCODE_PROXY_H "${PROJECT_SOURCE_DIR}/leetcode/proxy_gen.h" )
set( LEETCODE_PROXY_CXX "${PROJECT_SOURCE_DIR}/leetcode/proxy_gen.cxx" )
file ( WRITE ${LEETCODE_PROXY_H}
	"#pragma once\n\n" )
file ( WRITE ${LEETCODE_PROXY_CXX}
	"#include \"common/proxy_run_base.h\"\n"
    "#include \"proxy_gen.h\"\n"
	"\n"
	"TSolutionsMap solutions_map {\n")
foreach( problemsourcefile ${LEETCODE_SOURCES} )
    string( REPLACE ".cpp" "" problemfullname ${problemsourcefile} )
    get_filename_component( problembasename ${problemfullname} NAME )
    if ( NOT ${problembasename} MATCHES "base|main")
        file ( APPEND ${LEETCODE_PROXY_H}
			"int main_${problembasename}();\n" )
        file ( APPEND ${LEETCODE_PROXY_CXX}
			"\t{ \"${problembasename}\", main_${problembasename} },\n")
    endif ( NOT ${problembasename} MATCHES "base|main")
endforeach( problemsourcefile ${LEETCODE_SOURCES} )
file ( APPEND ${LEETCODE_PROXY_CXX} "};\n" )
add_executable( leetcode ${LEETCODE_SOURCES} ${LEETCODE_PROXY_CXX} )

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
    if ( NOT ${problembasename} MATCHES "base|main")
        file ( APPEND ${TIMUS_PROXY_H}
			"int main_${problembasename}();\n" )
        file ( APPEND ${TIMUS_PROXY_CXX}
			"\t{ \"${problembasename}\", main_${problembasename} },\n")
    endif ( NOT ${problembasename} MATCHES "base|main")
endforeach( problemsourcefile ${TIMUS_SOURCES} )
file ( APPEND ${TIMUS_PROXY_CXX} "};\n" )
add_executable( timus ${TIMUS_SOURCES} ${TIMUS_PROXY_CXX} )

file( GLOB_RECURSE TESTER_SOURCES RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/tester/*.cpp" )
add_executable( tester ${TESTER_SOURCES} )

add_executable( include_headers "${PROJECT_SOURCE_DIR}/utils/include_headers.cpp" )
# add_executable( current_solution "${PROJECT_SOURCE_DIR}/utils/current_solution.cpp" )

enable_testing()

file( GLOB_RECURSE ADVENT_OF_CODE_TESTS RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/tests/advent_of_code/*.out" )
list( SORT ADVENT_OF_CODE_TESTS )
foreach( testfile_out ${ADVENT_OF_CODE_TESTS} )
    get_filename_component( testshortname ${testfile_out} NAME_WE )
    string( SUBSTRING ${testshortname} 0 4 solution )
    string( SUBSTRING ${testshortname} 0 2 year )
	add_test( NAME "aoc.${testshortname}" COMMAND advent_of_code ${solution} "advent_of_code/20${year}/input/${testshortname}.txt" ${testfile_out} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
endforeach( testfile_out ${ADVENT_OF_CODE_TESTS} )

file( GLOB_RECURSE GOOGLE_CODE_JAM_TESTS RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/tests/google/code_jam/*.out" )
list( SORT GOOGLE_CODE_JAM_TESTS )
foreach( testfile_out ${GOOGLE_CODE_JAM_TESTS} )
    string( REPLACE ".out" "" testfullname ${testfile_out} )
    string( REPLACE ".out" ".in" testfile_in ${testfile_out} )
    get_filename_component( testshortname ${testfullname} NAME )
    get_filename_component( solution ${testshortname} NAME_WE )
	add_test( NAME ${testshortname} COMMAND google_code_jam ${solution} ${testfile_in} ${testfile_out} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
endforeach( testfile_out ${GOOGLE_CODE_JAM_TESTS} )

add_test( NAME number_guessing COMMAND python3 tests/google/code_jam/2018/ps/number_guessing.py ./release/build/google_code_jam number_guessing WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
add_test( NAME go_gopher COMMAND python3 tests/google/code_jam/2018/qr/go_gopher.py ./release/build/google_code_jam go_gopher WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
add_test( NAME lollipop_shop COMMAND python3 tests/google/code_jam/2018/r1c/lollipop_shop.py ./release/build/google_code_jam lollipop_shop WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
add_test( NAME name_preserving_network COMMAND python3 tests/google/code_jam/2018/r3/name_preserving_network.py ./release/build/google_code_jam name_preserving_network WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
add_test( NAME go_gophers COMMAND python3 tests/google/code_jam/2018/wf/go_gophers.py ./release/build/google_code_jam go_gophers WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
# add_test( NAME dat_bae.0 COMMAND python3 tests/google/code_jam/2019/interactive_runner.py python3 tests/google/code_jam/2019/qr/dat_bae.py 0 -- ./release/build/google_code_jam dat_bae WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
add_test( NAME dat_bae.1 COMMAND python3 tests/google/code_jam/2019/interactive_runner.py python3 tests/google/code_jam/2019/qr/dat_bae.py 1 -- ./release/build/google_code_jam dat_bae WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
add_test( NAME golf_gophers.0 COMMAND python3 tests/google/code_jam/2019/interactive_runner.py python3 tests/google/code_jam/2019/r1a/golf_gophers.py 0 -- ./release/build/google_code_jam golf_gophers WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
add_test( NAME golf_gophers.1 COMMAND python3 tests/google/code_jam/2019/interactive_runner.py python3 tests/google/code_jam/2019/r1a/golf_gophers.py 1 -- ./release/build/google_code_jam golf_gophers WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
# add_test( NAME draupnir.0 COMMAND python3 tests/google/code_jam/2019/interactive_runner.py python3 tests/google/code_jam/2019/r1b/draupnir.py 0 -- ./release/build/google_code_jam draupnir WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
add_test( NAME draupnir.1 COMMAND python3 tests/google/code_jam/2019/interactive_runner.py python3 tests/google/code_jam/2019/r1b/draupnir.py 1 -- ./release/build/google_code_jam draupnir WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
# add_test( NAME power_arrangers.0 COMMAND python3 tests/google/code_jam/2019/interactive_runner.py python3 tests/google/code_jam/2019/r1c/power_arrangers.py 0 -- ./release/build/google_code_jam power_arrangers WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
add_test( NAME power_arrangers.1 COMMAND python3 tests/google/code_jam/2019/interactive_runner.py python3 tests/google/code_jam/2019/r1c/power_arrangers.py 1 -- ./release/build/google_code_jam power_arrangers WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
add_test( NAME pottery_lottery COMMAND python3 tests/google/code_jam/2019/interactive_runner.py python3 tests/google/code_jam/2019/r2/pottery_lottery.py -- ./release/build/google_code_jam pottery_lottery WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
# add_test( NAME zilionim.0 COMMAND python3 tests/google/code_jam/2019/interactive_runner.py python3 tests/google/code_jam/2019/r3/zilionim.py 0 -- ./release/build/google_code_jam zilionim WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
# add_test( NAME zilionim.1 COMMAND python3 tests/google/code_jam/2019/interactive_runner.py python3 tests/google/code_jam/2019/r3/zilionim.py 1 -- ./release/build/google_code_jam zilionim WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
add_test( NAME zilionim.2 COMMAND python3 tests/google/code_jam/2019/interactive_runner.py python3 tests/google/code_jam/2019/r3/zilionim.py 2 -- ./release/build/google_code_jam zilionim WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
add_test( NAME board_meeting.0 COMMAND python3 tests/google/code_jam/2019/interactive_runner.py python3 tests/google/code_jam/2019/wf/board_meeting.py 0 -- ./release/build/google_code_jam board_meeting WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
add_test( NAME board_meeting.1 COMMAND python3 tests/google/code_jam/2019/interactive_runner.py python3 tests/google/code_jam/2019/wf/board_meeting.py 1 -- ./release/build/google_code_jam board_meeting WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
add_test( NAME esab_atad.0 COMMAND python3 tests/google/code_jam/2020/qr/interactive_runner.py python3 tests/google/code_jam/2020/qr/esab_atad.py 0 -- ./release/build/google_code_jam esab_atad WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
add_test( NAME esab_atad.1 COMMAND python3 tests/google/code_jam/2020/qr/interactive_runner.py python3 tests/google/code_jam/2020/qr/esab_atad.py 1 -- ./release/build/google_code_jam esab_atad WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
add_test( NAME esab_atad.2 COMMAND python3 tests/google/code_jam/2020/qr/interactive_runner.py python3 tests/google/code_jam/2020/qr/esab_atad.py 2 -- ./release/build/google_code_jam esab_atad WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
# add_test( NAME blindfolded_bullseye.0 COMMAND python3 tests/google/code_jam/2020/interactive_runner.py python3 tests/google/code_jam/2020/r1b/blindfolded_bullseye.py 0 -- ./release/build/google_code_jam blindfolded_bullseye WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
# add_test( NAME blindfolded_bullseye.1 COMMAND python3 tests/google/code_jam/2020/interactive_runner.py python3 tests/google/code_jam/2020/r1b/blindfolded_bullseye.py 1 -- ./release/build/google_code_jam blindfolded_bullseye WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
add_test( NAME blindfolded_bullseye.2 COMMAND python3 tests/google/code_jam/2020/interactive_runner.py python3 tests/google/code_jam/2020/r1b/blindfolded_bullseye.py 2 -- ./release/build/google_code_jam blindfolded_bullseye WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
# add_test( NAME pen_testing.0 COMMAND python3 tests/google/code_jam/2020/interactive_runner.py python3 tests/google/code_jam/2020/r3/pen_testing.py 0 -- ./release/build/google_code_jam pen_testing WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
# add_test( NAME pen_testing.1 COMMAND python3 tests/google/code_jam/2020/interactive_runner.py python3 tests/google/code_jam/2020/r3/pen_testing.py 1 -- ./release/build/google_code_jam pen_testing WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
# add_test( NAME pen_testing.2 COMMAND python3 tests/google/code_jam/2020/interactive_runner.py python3 tests/google/code_jam/2020/r3/pen_testing.py 2 -- ./release/build/google_code_jam pen_testing WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
# add_test( NAME median_sort.0 COMMAND python3 tests/google/code_jam/2020/interactive_runner.py python3 tests/google/code_jam/2021/qr/median_sort.py 0 -- ./release/build/google_code_jam median_sort WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
# add_test( NAME median_sort.1 COMMAND python3 tests/google/code_jam/2020/interactive_runner.py python3 tests/google/code_jam/2021/qr/median_sort.py 1 -- ./release/build/google_code_jam median_sort WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
add_test( NAME median_sort.2 COMMAND python3 tests/google/code_jam/2020/interactive_runner.py python3 tests/google/code_jam/2021/qr/median_sort.py 2 -- ./release/build/google_code_jam median_sort WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
# add_test( NAME digit_blocks.0 COMMAND python3 tests/google/code_jam/2020/interactive_runner.py python3 tests/google/code_jam/2021/r1b/digit_blocks.py 0 -- ./release/build/google_code_jam digit_blocks WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
add_test( NAME digit_blocks.1 COMMAND python3 tests/google/code_jam/2020/interactive_runner.py python3 tests/google/code_jam/2021/r1b/digit_blocks.py 1 -- ./release/build/google_code_jam digit_blocks WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
add_test( NAME minimum_sort COMMAND python3 tests/google/code_jam/2020/interactive_runner.py python3 tests/google/code_jam/2021/r2/minimum_sort.py -- ./release/build/google_code_jam minimum_sort WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
# add_test( NAME twisty_little_passages COMMAND python3 tests/google/code_jam/2020/interactive_runner.py python3 tests/google/code_jam/2022/qr/twisty_little_passages.py -- ./release/build/google_code_jam twisty_little_passages WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
add_test( NAME equal_sum COMMAND python3 tests/google/code_jam/2020/interactive_runner.py python3 tests/google/code_jam/2022/r1a/equal_sum.py -- ./release/build/google_code_jam equal_sum WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
add_test( NAME asedatab COMMAND python3 tests/google/code_jam/2020/interactive_runner.py python3 tests/google/code_jam/2022/r1b/asedatab.py -- ./release/build/google_code_jam asedatab WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
# add_test( NAME revenge_of_gorosort.0 COMMAND python3 tests/google/code_jam/2020/interactive_runner.py python3 tests/google/code_jam/2022/r3/revenge_of_gorosort.py 0 -- ./release/build/google_code_jam revenge_of_gorosort WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
# add_test( NAME revenge_of_gorosort.1 COMMAND python3 tests/google/code_jam/2020/interactive_runner.py python3 tests/google/code_jam/2022/r3/revenge_of_gorosort.py 1 -- ./release/build/google_code_jam revenge_of_gorosort WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
# add_test( NAME revenge_of_gorosort.2 COMMAND python3 tests/google/code_jam/2020/interactive_runner.py python3 tests/google/code_jam/2022/r3/revenge_of_gorosort.py 2 -- ./release/build/google_code_jam revenge_of_gorosort WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )

file( GLOB_RECURSE HACKERRANK_TESTS RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/tests/hackerrank/*.in" )
list( SORT HACKERRANK_TESTS )
foreach( testfile_in ${HACKERRANK_TESTS} )
    string( REPLACE ".in" "" testfullname ${testfile_in} )
    string( REPLACE ".in" ".out" testfile_out ${testfile_in} )
    get_filename_component( testshortname ${testfullname} NAME )
    get_filename_component( solution ${testshortname} NAME_WE )
	add_test( NAME ${testshortname} COMMAND hackerrank ${solution} ${testfile_in} ${testfile_out} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
endforeach( testfile_in ${HACKERRANK_TESTS} )

file( GLOB_RECURSE HACKERRANK_LANGUAGE_C_TESTS RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/tests/hackerrank_language/c/*.in" )
list( SORT HACKERRANK_LANGUAGE_C_TESTS )
foreach( testfile_in ${HACKERRANK_LANGUAGE_C_TESTS} )
    string( REPLACE ".in" "" testfullname ${testfile_in} )
    string( REPLACE ".in" ".out" testfile_out ${testfile_in} )
    get_filename_component( testshortname ${testfullname} NAME )
    get_filename_component( solution ${testshortname} NAME_WE )
	add_test( NAME ${testshortname} COMMAND hackerrank_c ${solution} ${testfile_in} ${testfile_out} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
endforeach( testfile_in ${HACKERRANK_LANGUAGE_C_TESTS} )

file( GLOB_RECURSE LEETCODE_TESTS RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/leetcode/problems/*.cpp" )
list( SORT LEETCODE_TESTS )
foreach( solution_cpp ${LEETCODE_TESTS} )
    string( REPLACE ".cpp" "" solution_path ${solution_cpp} )
    get_filename_component( id ${solution_path} NAME )
	add_test( NAME "leetcode.${id}" COMMAND leetcode ${id} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
endforeach( solution_cpp ${LEETCODE_TESTS} )

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
add_test( NAME tester_bst_split_join COMMAND tester bst_split_join )
add_test( NAME tester_convergent COMMAND tester convergent )
add_test( NAME tester_generating_function COMMAND tester generating_function )
add_test( NAME tester_fixed_universe_successor COMMAND tester fixed_universe_successor )
add_test( NAME tester_graph_distance COMMAND tester graph_distance )
add_test( NAME tester_graph_distance_u COMMAND tester graph_distance_unsigned )
add_test( NAME tester_graph_distance_pc COMMAND tester graph_distance_positive_cost )
add_test( NAME tester_heap_base COMMAND tester heap_base )
add_test( NAME tester_heap_ext COMMAND tester heap_ext )
add_test( NAME tester_interpolation COMMAND tester interpolation )
add_test( NAME tester_long_mult COMMAND tester long_mult )
add_test( NAME tester_lowest_common_ancestor COMMAND tester lowest_common_ancestor )
add_test( NAME tester_mertens COMMAND tester mertens )
add_test( NAME tester_mertens_compact COMMAND tester mertens_compact )
add_test( NAME tester_minimum_spanning_tree COMMAND tester minimum_spanning_tree )
add_test( NAME tester_modular_fft COMMAND tester modular_fft )
add_test( NAME tester_primes_generation COMMAND tester primes_generation )
add_test( NAME tester_range_minimum_query COMMAND tester range_minimum_query )
add_test( NAME tester_tree_path_maxima COMMAND tester tree_path_maxima )
