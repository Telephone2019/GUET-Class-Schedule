# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("GUET-Class-Schedule")

set(CMAKE_CXX_FLAGS "/source-charset:utf-8 /execution-charset:utf-8 /validate-charset /std:c++17")

set(CMAKE_CXX_STANDARD 17)

# 包含子项目。
add_subdirectory ("GUET-Class-Schedule")
