cmake_minimum_required(VERSION 2.8.9)

project(avim)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
set(CMAKE_COMPILER_IS_CLANGXX 1)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
if(UNIX)
set(CMAKE_COMPILER_IS_UNIX_ICC 1)
else()
# CMAKE_COMPILER_IS_WIN_ICC 的 c++11 模式命令行参数不一样
set(CMAKE_COMPILER_IS_WIN_ICC 1)
endif()
endif()

option(RUN_PROTOC "run protoc to generate protobuf protocol parser" ON)
option(BUILD_C_WRAPPER "build a libavproto that exports C interface" OFF)

find_package(Threads REQUIRED)
find_package(Boost 1.54 REQUIRED thread system coroutine context regex)
find_package(Protobuf REQUIRED)
find_package(OpenSSL REQUIRED)
#find_package(PkgConfig REQUIRED)

#pkg_check_modules(PKCS11 REQUIRED libpkcs11-helper-1 )

include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${OPENSSL_INCLUDE_DIR})
#include_directories(${PKCS11_INCLUDE_DIRS})
include_directories(libavproto/include)
include_directories(.)

link_libraries(${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} ${PKCS11_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX OR CMAKE_COMPILER_IS_UNIX_ICC)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

endif()

add_subdirectory(avim_proto)
add_subdirectory(libavproto)

link_libraries(avproto++)


add_subdirectory(demo)
