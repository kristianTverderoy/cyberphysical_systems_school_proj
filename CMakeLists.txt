CMAKE_MINIMUM_REQUIRED(VERSION 3.28.3)
project(window)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
        threepp
        GIT_REPOSITORY https://github.com/markaren/threepp.git
        GIT_TAG 77a93ea   # use a tag/branch (required for GIT_SHALLOW)
        GIT_SHALLOW TRUE        # fetch only the tip to keep the download small
)
FetchContent_MakeAvailable(threepp)
find_package(unofficial-omniverse-physx-sdk CONFIG REQUIRED)


add_executable(window)

if(MSVC)
  target_compile_options(window PRIVATE /W4)
else()
  target_compile_options(window PRIVATE -Wall -Wextra)
endif()
target_sources(window PRIVATE window.cpp)
target_link_libraries(window PRIVATE threepp::threepp unofficial::omniverse-physx-sdk::sdk)
