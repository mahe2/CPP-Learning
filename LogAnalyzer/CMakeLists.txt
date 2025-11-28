cmake_minimum_required(VERSION 3.16)
project(LogAnalyzer VERSION 1.0.0 LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 编译器标志
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

# 包含目录
include_directories(include)

# 收集源文件
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

# 创建可执行文件
add_executable(loganalyzer ${SOURCES} ${HEADERS})

# 链接线程库
find_package(Threads REQUIRED)
target_link_libraries(loganalyzer Threads::Threads)

# 如果需要，可以添加其他库
# find_package(Boost REQUIRED COMPONENTS system filesystem)
# target_link_libraries(loganalyzer ${Boost_LIBRARIES})

# 安装规则
install(TARGETS loganalyzer DESTINATION bin)

# 测试（暂时注释掉，稍后添加测试）
# enable_testing()
# add_subdirectory(tests)