project(libvarbit)
cmake_minimum_required (VERSION 2.8)

# settings
set(CMAKE_CXX_FLAGS "-O3 -Wextra -Werror")

# headers
install(
	DIRECTORY include/
	DESTINATION include/varbit
)

# tests
enable_testing(true)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} include)
aux_source_directory(test test_srcs)
add_executable(tests ${test_srcs})
target_link_libraries(tests ${GTEST_BOTH_LIBRARIES})
GTEST_ADD_TESTS(tests "" ${test_srcs})

# benchmarks
add_executable(benchmark_sum benchmark/sum.cc)