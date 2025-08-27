
# 设置最低CMake版本和项目信息
cmake_minimum_required(VERSION 3.10)
project(myDSALib VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加子目录：构建DSALib库
add_subdirectory(DSALib)

# 创建可执行文件
add_executable(myDSALib_main main.cpp)

# 链接DSALib库到可执行文件
target_link_libraries(myDSALib_main PRIVATE DSALib)

