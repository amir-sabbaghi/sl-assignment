cmake_minimum_required(VERSION 3.12)

set (CMAKE_CXX_STANDARD 11)

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.14.tar.gz"
    SHA1 "f58bcf274f29097ac380bfe4955d2a66f077e741"
)

project(sl-test)
hunter_add_package(Boost COMPONENTS iostreams serialization)
find_package(Boost CONFIG REQUIRED iostreams serialization)

add_executable(client client.cpp Object.cpp)
target_link_libraries(client Boost::boost Boost::serialization)
add_executable(server server.cpp Object.cpp)
target_link_libraries(server Boost::boost Boost::serialization)
