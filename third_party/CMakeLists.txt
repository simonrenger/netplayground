set(BENCHMARK_ENABLE_GTEST_TESTS off)
set(BENCHMARK_ENABLE_TESTING off)

add_subdirectory(benchmark)
add_subdirectory(Catch2)
add_subdirectory(enet)

add_library(eventpp INTERFACE)
target_include_directories(eventpp INTERFACE "eventpp/include/")

# add_subdirectory(ogre3d-common)