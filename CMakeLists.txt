cmake_minimum_required(VERSION 3.24)
project(mygpt)

set(CMAKE_CXX_STANDARD 17)

add_executable(mygpt main.cpp api.h config.h api.cpp config.cpp)

include_directories(/Users/patrickbu/vcpkg/packages/nlohmann-json_arm64-osx/include)

target_link_libraries(mygpt PRIVATE curl)