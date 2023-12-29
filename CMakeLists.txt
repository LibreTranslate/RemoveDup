cmake_minimum_required(VERSION 3.10)
project(removedup VERSION 1.0 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Choose the type of build, options are: Debug Release
RelWithDebInfo MinSizeRel."
      FORCE)
endif()

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(removedup main.cpp dedup.cpp)
target_link_libraries(removedup)
install(TARGETS removedup RUNTIME DESTINATION bin)