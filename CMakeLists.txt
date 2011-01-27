project(libvarbit)
cmake_minimum_required (VERSION 2.8)

# settings
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -Wformat=2 -Wold-style-cast -Winit-self -Wmissing-include-dirs -Wpointer-arith -Wredundant-decls -Wundef -Wwrite-strings")

# headers
install(
	DIRECTORY include/
	DESTINATION include/varbit)


# tests
enable_testing(true)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})
aux_source_directory(test test_sources)
add_executable(tests ${test_sources})
target_link_libraries(tests ${GTEST_BOTH_LIBRARIES} pthread)
GTEST_ADD_TESTS(tests "" ${test_sources})

# benchmarks
include(CheckIncludeFileCXX)
check_include_file_cxx(papi.h PFUNC_HAVE_PAPI_H)
if(PFUNC_HAVE_PAPI_H)
	aux_source_directory(benchmark benchmark_sources)
	foreach(source_filename ${benchmark_sources})
		string(REGEX REPLACE "benchmark/(.*).cc" "\\1"
			binary_filename ${source_filename})
		add_executable("benchmark_${binary_filename}" ${source_filename})
		target_link_libraries("benchmark_${binary_filename}" "papi")
	endforeach()
else()
	message("-- Benchmarks disabled")
endif()

# style
add_custom_target(style
	python tools/cpplint.py
	--filter=-legal,-readability/streams,-build/header_guard
	benchmark/* include/* test/*
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
