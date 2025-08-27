
# 收集所有源文件（注意：file(GLOB)在开发中方便，但生产环境建议显式列表）
file(GLOB SOURCES "src/*.cpp")

# 创建静态库目标（如需共享库，将STATIC改为SHARED）
add_library(DSALib STATIC ${SOURCES})

# 设置公共头文件包含目录：允许链接此库的目标访问include下的所有头文件
target_include_directories(DSALib PUBLIC include)

