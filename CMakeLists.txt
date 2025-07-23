cmake_minimum_required(VERSION 3.5.0)
project(CCC VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
set(CMAKE_CXX_STANDARD 20)
set(ExprTest text/ExprTest.cpp)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
# 查找源文件目录中的所有.cpp文件
file(GLOB SOURCES "${SOURCE_DIR}/*.cpp")

add_executable(CCC  ${SOURCES}) 
add_subdirectory(src)
include(CTest)
include_directories(./include)

enable_testing()
set(CMAKE_CXX_EXTENSIONS 20)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)
