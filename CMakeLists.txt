
# 设置最低CMake版本和项目信息
cmake_minimum_required(VERSION 3.15)
project(myDSALib VERSION 1.0.0 LANGUAGES CXX)

# Google Test Loading ================================================================
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
# ====================================================================================

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加子目录：构建DSALib库
add_subdirectory(DSALib)

# 创建可执行文件
add_executable(myDSALib_main main.cpp)

# 链接DSALib库到可执行文件
target_link_libraries(myDSALib_main PRIVATE DSALib)

# 启用测试
enable_testing()
add_subdirectory(tests)
