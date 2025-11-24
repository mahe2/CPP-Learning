# CMake 完全指南 - 从 0 到 1

## 1. CMake 是什么？

**CMake = Cross-platform Make**（跨平台构建工具）

### 类比理解（对比 Java/Kotlin）

```
Java/Kotlin 生态：
  源代码 (.java/.kt) → Gradle/Maven → 自动编译 → .jar/.apk

C++ 生态：
  源代码 (.cpp/.h) → CMake → 生成构建文件 → 编译器 → 可执行文件
```

### 核心作用

1. **自动化编译**：管理多个源文件的编译
2. **跨平台**：同一个配置，Windows/macOS/Linux 都能用
3. **依赖管理**：处理第三方库的链接
4. **项目组织**：管理大型项目的目录结构

---

## 2. 为什么需要 CMake？

### 问题场景

**场景 1：单文件编译（简单）**
```bash
g++ -std=c++17 hello.cpp -o hello
```
✅ 简单，一行搞定

**场景 2：多文件项目（复杂）**
```bash
# 手动编译每个文件
g++ -c src/main.cpp -o build/main.o
g++ -c src/utils.cpp -o build/utils.o
g++ -c src/math.cpp -o build/math.o
g++ build/main.o build/utils.o build/math.o -o myapp

# 如果有 50 个文件呢？😱
```
❌ 繁琐、易出错、难维护

**场景 3：跨平台（噩梦）**
```bash
# macOS
g++ -std=c++17 ...

# Windows
cl.exe /std:c++17 ...

# Linux
g++ -std=c++17 ...
```
❌ 每个平台都要写不同的编译命令

### CMake 的解决方案

**一个 `CMakeLists.txt` 搞定所有问题！**

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyApp)

# 自动找到所有 .cpp 文件
file(GLOB SOURCES "src/*.cpp")

# 一行代码编译所有文件
add_executable(myapp ${SOURCES})
```

---

## 3. CMake vs Gradle/Maven 对比

| 特性 | CMake (C++) | Gradle (Java/Kotlin) | Maven (Java) |
|------|-------------|---------------------|--------------|
| 配置文件 | `CMakeLists.txt` | `build.gradle.kts` | `pom.xml` |
| 语法 | CMake 脚本 | Kotlin DSL | XML |
| 依赖管理 | 需要手动或第三方 | ✅ 自动（Maven Central） | ✅ 自动 |
| 编译 | 生成 Makefile 再编译 | 直接编译 | 直接编译 |
| 跨平台 | ✅ 优秀 | ✅ 优秀（JVM） | ✅ 优秀（JVM） |
| 学习曲线 | 中等 | 中等 | 简单 |

---

## 4. 0 基础上手教程

### 步骤 1：安装 CMake

**macOS:**
```bash
brew install cmake
```

**验证安装:**
```bash
cmake --version
# 输出: cmake version 3.x.x
```

### 步骤 2：第一个 CMake 项目

**项目结构:**
```
my_first_cmake/
├── CMakeLists.txt    # CMake 配置文件
└── main.cpp          # 源代码
```

**main.cpp:**
```cpp
#include <iostream>
using namespace std;

int main() {
    cout << "Hello CMake!" << endl;
    return 0;
}
```

**CMakeLists.txt:**
```cmake
# 1. 指定 CMake 最低版本
cmake_minimum_required(VERSION 3.10)

# 2. 项目名称
project(HelloCMake)

# 3. 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)

# 4. 生成可执行文件
add_executable(hello main.cpp)
```

**编译运行:**
```bash
# 创建 build 目录（构建文件放这里，保持源码干净）
mkdir build
cd build

# 生成构建文件
cmake ..

# 编译
cmake --build .

# 运行
./hello
```

### 步骤 3：多文件项目

**项目结构:**
```
calculator/
├── CMakeLists.txt
├── main.cpp
├── math.cpp
└── math.h
```

**math.h:**
```cpp
#pragma once

int add(int a, int b);
int subtract(int a, int b);
```

**math.cpp:**
```cpp
#include "math.h"

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}
```

**main.cpp:**
```cpp
#include <iostream>
#include "math.h"
using namespace std;

int main() {
    cout << "5 + 3 = " << add(5, 3) << endl;
    cout << "5 - 3 = " << subtract(5, 3) << endl;
    return 0;
}
```

**CMakeLists.txt:**
```cmake
cmake_minimum_required(VERSION 3.10)
project(Calculator)

set(CMAKE_CXX_STANDARD 17)

# 方法 1：手动列出所有文件
add_executable(calc main.cpp math.cpp)

# 方法 2：自动找到所有 .cpp 文件（推荐小项目）
# file(GLOB SOURCES "*.cpp")
# add_executable(calc ${SOURCES})
```

### 步骤 4：子目录结构

**项目结构:**
```
myapp/
├── CMakeLists.txt
├── main.cpp
├── src/
│   ├── CMakeLists.txt
│   ├── utils.cpp
│   └── utils.h
└── include/
    └── config.h
```

**根目录 CMakeLists.txt:**
```cmake
cmake_minimum_required(VERSION 3.10)
project(MyApp)

set(CMAKE_CXX_STANDARD 17)

# 添加头文件目录
include_directories(include)

# 添加子目录
add_subdirectory(src)

# 主程序
add_executable(myapp main.cpp)

# 链接子目录的库
target_link_libraries(myapp utils_lib)
```

**src/CMakeLists.txt:**
```cmake
# 创建库
add_library(utils_lib utils.cpp)
```

---

## 5. 常用 CMake 命令

### 基础命令

```cmake
# 1. 设置 CMake 最低版本
cmake_minimum_required(VERSION 3.10)

# 2. 项目名称和版本
project(MyProject VERSION 1.0)

# 3. 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 4. 生成可执行文件
add_executable(myapp main.cpp)

# 5. 创建库
add_library(mylib STATIC lib.cpp)      # 静态库 .a
add_library(mylib SHARED lib.cpp)      # 动态库 .so/.dylib

# 6. 添加头文件目录
include_directories(include)
target_include_directories(myapp PRIVATE include)

# 7. 链接库
target_link_libraries(myapp mylib)

# 8. 添加子目录
add_subdirectory(src)

# 9. 查找文件
file(GLOB SOURCES "src/*.cpp")
file(GLOB_RECURSE SOURCES "src/**/*.cpp")  # 递归查找

# 10. 条件编译
if(WIN32)
    # Windows 特定代码
elseif(APPLE)
    # macOS 特定代码
else()
    # Linux 特定代码
endif()
```

### 变量

```cmake
# 设置变量
set(MY_VAR "hello")
set(SOURCE_FILES main.cpp utils.cpp)

# 使用变量
message(STATUS "Value: ${MY_VAR}")
add_executable(myapp ${SOURCE_FILES})

# 常用内置变量
${CMAKE_SOURCE_DIR}      # 源码根目录
${CMAKE_BINARY_DIR}      # 构建目录
${CMAKE_CURRENT_SOURCE_DIR}  # 当前 CMakeLists.txt 所在目录
${PROJECT_NAME}          # 项目名称
${CMAKE_CXX_COMPILER}    # C++ 编译器路径
```

---

## 6. 实战：Qt 项目的 CMake

**这是您未来学 Qt 会用到的！**

```cmake
cmake_minimum_required(VERSION 3.16)
project(MyQtApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)  # Qt 的 MOC 自动处理
set(CMAKE_AUTOUIC ON)  # UI 文件自动处理
set(CMAKE_AUTORCC ON)  # 资源文件自动处理

# 查找 Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# 源文件
set(SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

# 生成可执行文件
add_executable(myqtapp ${SOURCES})

# 链接 Qt 库
target_link_libraries(myqtapp Qt6::Core Qt6::Widgets)
```

---

## 7. 对比：手动 vs CMake

### 手动编译（小项目）

```bash
# 优点：简单直接
g++ -std=c++17 main.cpp -o myapp

# 缺点：
# ❌ 多文件时命令很长
# ❌ 跨平台需要不同命令
# ❌ 依赖管理困难
```

### CMake（中大型项目）

```cmake
# 优点：
# ✅ 一次配置，到处编译
# ✅ 自动处理依赖
# ✅ 支持增量编译（只编译修改的文件）
# ✅ 行业标准，很多库都支持

# 缺点：
# ❌ 学习成本（但不高）
# ❌ 小项目有点杀鸡用牛刀
```

---

## 8. 学习难度评估

### 对比 Gradle/Maven

| 难度维度 | CMake | Gradle | Maven |
|----------|-------|--------|-------|
| 基础语法 | ⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐ |
| 依赖管理 | ⭐⭐⭐⭐ | ⭐⭐ | ⭐⭐ |
| 跨平台 | ⭐⭐ | ⭐ | ⭐ |
| 文档质量 | ⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐⭐ |
| **总体难度** | **中等** | **中等** | **简单** |

### 您的优势

✅ 已经会 Gradle（Kotlin/Android），概念类似  
✅ CMake 基础语法比 Gradle 简单  
✅ 现在有 AI（ChatGPT/Copilot）帮忙  

**预计学习时间：**
- 基础使用：2-3 小时
- 熟练使用：1-2 周
- 高级特性：1-2 个月

---

## 9. 何时使用 CMake？

### ✅ 推荐使用

- 多文件 C++ 项目（>5 个文件）
- 需要跨平台编译
- 使用第三方库（Qt、Boost、OpenCV 等）
- 团队协作项目
- **Qt 开发（必须）**

### ❌ 可以不用

- 单文件小程序（直接 `g++` 即可）
- 练习 C++ 语法（您现在的阶段）
- 竞赛编程（LeetCode）

---

## 10. 学习建议

### 现阶段（C++ 基础学习）

```bash
# 继续用简单的 g++ 命令
g++ -std=c++17 main.cpp -o myapp

# 或者用 VS Code 的 Code Runner（已配置好）
# 点击右上角 ▶️ 按钮即可
```

**原因：**
- 专注于 C++ 语法学习
- 避免被构建工具分散注意力
- 目前的项目都很小，不需要 CMake

### 后期（Qt 开发阶段）

**必须学 CMake，因为：**
1. Qt 官方推荐用 CMake
2. Qt Creator 集成了 CMake
3. 现代 C++ 项目标配

**学习路径：**
1. 先完成 C++ 基础学习（当前）
2. 开始 Qt 时，跟着 Qt 官方教程学 CMake
3. 边做项目边学，自然就会了

---

## 11. 快速参考

### 最常用的 CMake 命令

```cmake
# 1. 创建简单项目
cmake_minimum_required(VERSION 3.10)
project(MyApp)
set(CMAKE_CXX_STANDARD 17)
add_executable(myapp main.cpp)

# 2. 多文件项目
file(GLOB SOURCES "src/*.cpp")
add_executable(myapp ${SOURCES})

# 3. 添加库
target_link_libraries(myapp pthread)

# 4. 添加头文件路径
target_include_directories(myapp PRIVATE include)
```

### 常用终端命令

```bash
# 初次构建
mkdir build && cd build
cmake ..
cmake --build .

# 重新编译（修改代码后）
cmake --build .

# 清理
rm -rf build

# 指定编译类型
cmake -DCMAKE_BUILD_TYPE=Debug ..     # 调试版本
cmake -DCMAKE_BUILD_TYPE=Release ..   # 发布版本
```

---

## 12. 总结

### CMake 的本质

```
CMake ≈ Java 的 Maven/Gradle
作用：自动化编译、依赖管理、跨平台构建
```

### 0 基础能上手吗？

**能！但建议分阶段：**

1. **现在（C++ 学习期）**：不用 CMake，用 `g++` 或 Code Runner
2. **Qt 开发期**：必须学 CMake，但那时你 C++ 基础已经很好了
3. **实际难度**：比 Gradle 简单，2-3 小时就能上手基础

### 记忆口诀

```
CMake 是管家，帮你编译 C++
就像 Gradle 管 Kotlin，Maven 管 Java
单文件不需要，多文件才用它
Qt 开发必须学，但现在不着急
```

---

## 附录：创建一个 CMake 示例

想试试看吗？我可以在您的工作空间创建一个 CMake 示例项目！
