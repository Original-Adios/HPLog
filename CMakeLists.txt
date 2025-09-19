cmake_minimum_required(VERSION 3.14)
project(Logger LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 让 fmt 可见
include_directories(${CMAKE_SOURCE_DIR}/third_party/fmt/include)

# 添加子目录
add_subdirectory(logger)
add_subdirectory(example)
