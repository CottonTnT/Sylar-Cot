cmake_minimum_required(VERSION 3.15)

project(
  SYLAR
  VERSION 1.0
  LANGUAGES CXX
)

set(CMAKE_BUILD_TYPE Debug) # 默认就是Debug模式
set(CMAKE_CXX_STANDARD 17) # 设置C++标准
set(CMAKE_CXX_STANDARD_REQUIRED off) # 编译器一定要支持c20标准
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # MSVC的生成器不支持这个选项

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXX_FLAGS} -rdynamic -O3 -g -std=c++17 -Wall -Wno-deprecated -Werror -Wno-unused-function")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(LIB_SRC 
# sylar/log.cpp)

file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp)
#递归查找 src 目录及其子目录中的所有 .cpp 文件，并将它们的路径保存在 srcs 变量中。CONFIGURE_DEPENDS 确保 CMake 在每次配置时重新检查文件列表，从而保持文件列表的最新状态。

# add_library(sylar SHARED ${LIB_SRC})

add_executable(test "${srcs}")
set(INCLUDE_DIR "include")
target_include_directories(test PUBLIC "${INCLUDE_DIR}")



# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)



