cmake_minimum_required(VERSION 3.10)
project(vsomeip_client_lib)
set(CMAKE_CXX_STANDARD 14)
find_package (vsomeip3 3.4.10 REQUIRED)
find_package( Boost 1.55 COMPONENTS system thread log REQUIRED )
include_directories(
    ${Boost_INCLUDE_DIR}
    ${VSOMEIP_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
)
add_library(i2c_lib STATIC src/i2c.cpp)
add_library(vsomeipclient STATIC src/vsomeipclient.cpp)

target_link_libraries(vsomeipclient i2c_lib vsomeip3 ${Boost_LIBRARIES})
add_executable(client_test test/main.cpp)

target_link_libraries(client_test vsomeipclient i2c_lib vsomeip3 ${Boost_LIBRARIES})


