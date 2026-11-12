CMAKE_MINIMUM_REQUIRED(VERSION 3.28.3)
project(window)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
        JoltPhysics
        GIT_REPOSITORY https://github.com/jrouwe/JoltPhysics.git
        GIT_TAG v5.3.0          # pick a release tag from the repo's releases page
        GIT_SHALLOW TRUE
        SOURCE_SUBDIR Build
)
set(TARGET_UNIT_TESTS OFF)
set(TARGET_HELLO_WORLD OFF)
set(TARGET_PERFORMANCE_TEST OFF)
set(TARGET_SAMPLES OFF)
set(TARGET_VIEWER OFF)
set(USE_STATIC_MSVC_RUNTIME_LIBRARY OFF)   # only matters for MSVC; avoids CRT mismatch with threepp

FetchContent_MakeAvailable(JoltPhysics)

FetchContent_Declare(
        threepp
        GIT_REPOSITORY https://github.com/markaren/threepp.git
        GIT_TAG 20b5d10         # use a tag/branch (required for GIT_SHALLOW)
        GIT_SHALLOW TRUE        # fetch only the tip to keep the download small
)
FetchContent_MakeAvailable(threepp)

add_executable(window)

if(MSVC)
  target_compile_options(window PRIVATE /W4)
else()
  target_compile_options(window PRIVATE -Wall -Wextra)
endif()

if(MINGW)
  target_link_options(window PRIVATE -static -static-libgcc -static-libstdc++)
endif()
target_sources(window PRIVATE window.cpp)
target_link_libraries(window PRIVATE threepp::threepp Jolt)
