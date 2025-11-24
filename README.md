# C++ 学习路线图 📚

> 适合有 Java/Kotlin/TypeScript 基础的开发者

## 📖 推荐在线学习资源

### 中文资源
- **[菜鸟教程 - C++](https://www.runoob.com/cplusplus/cpp-tutorial.html)** - 快速入门，有在线编译器
- **[现代 C++ 教程](https://changkun.de/modern-cpp/)** - 专注现代C++特性
- **[cppreference 中文版](https://zh.cppreference.com/)** - 权威参考手册

### 英文资源（强烈推荐）
- **[LearnCPP.com](https://www.learncpp.com/)** ⭐ 最推荐，循序渐进
- **[cplusplus.com Tutorial](https://cplusplus.com/doc/tutorial/)** - 简洁清晰
- **[Microsoft C++ 文档](https://learn.microsoft.com/zh-cn/cpp/)** - 官方文档

### 互动练习
- **[LeetCode C++ Track](https://leetcode.cn/)** - 算法练习
- **[HackerRank C++](https://www.hackerrank.com/domains/cpp)** - 基础到进阶

---

## 🎯 学习路径（建议 6-8 周）

### 第一阶段：基础语法（1-2周）
📁 `01_basics/`
- [x] 变量和数据类型
- [x] 运算符和表达式
- [x] 控制流程（if/switch/循环）
- [x] 输入输出（cin/cout）

### 第二阶段：函数和指针（1-2周）⚠️ 重点
📁 `02_functions_pointers/`
- [x] 函数定义和调用
- [x] 指针基础（与Java引用的区别）
- [x] 引用（类似Kotlin的by reference）
- [x] 数组和指针的关系

### 第三阶段：面向对象（1-2周）
📁 `03_oop/`
- [x] 类和对象
- [x] 构造函数和析构函数（重点！）
- [x] 继承和多态
- [x] 访问控制（public/private/protected）

### 第四阶段：现代C++特性（1-2周）
📁 `04_modern_cpp/`
- [x] 智能指针（unique_ptr, shared_ptr）
- [x] Lambda表达式
- [x] STL容器（vector, map, set）
- [x] 迭代器和算法

### 第五阶段：实战练习（1-2周）
📁 `05_exercises/`
- [x] 综合项目练习
- [x] 常见算法实现

---

## 🔥 对比：Java/Kotlin vs C++

| 特性 | Java/Kotlin | C++ |
|------|-------------|-----|
| 内存管理 | 自动GC | 手动管理（new/delete）或智能指针 |
| 编译 | 字节码 + JVM | 直接编译为机器码 |
| 多继承 | 不支持（接口） | 支持多继承 |
| 指针 | 无显式指针 | 有指针概念 |
| 性能 | 较快 | 极快 |
| Lambda | Kotlin支持 | C++11+支持 |

---

## 💡 给Android开发者的提示

- **信号槽（Qt）** ≈ 事件监听器（Android）
- **智能指针** ≈ 自动内存管理
- **STL容器** ≈ Java Collections
- **Lambda** ≈ Kotlin Lambda表达式
- **析构函数** ≈ onDestroy生命周期

---

## 🚀 快速开始

### 一键运行（推荐）⭐
1. 打开任意 `.cpp` 文件
2. 点击右上角的 **▶️ 运行按钮**
3. 或按快捷键 `Ctrl+Alt+N`（macOS: `⌃⌥N`）

**已配置 Code Runner，无需手动编译！**

> 详细说明见 [QUICK_START.md](QUICK_START.md)

---

### 传统方式（终端命令）

```bash
# 进入目录
cd 01_basics

# 编译
g++ -std=c++17 01_hello_world.cpp -o hello

# 运行
./hello
```

### 环境配置（首次使用）
```bash
# macOS 安装编译器
xcode-select --install

# 或使用 Homebrew 安装 GCC
brew install gcc

# 验证安装
g++ --version
```

---

## 📝 学习建议

1. **循序渐进**：不要跳过指针和内存管理章节
2. **多动手**：每个概念都要写代码验证
3. **对比学习**：结合Java/Kotlin经验理解
4. **关注现代C++**：学习C++11/14/17特性，避免过时写法
5. **实践项目**：学完基础后尽快上手Qt项目

---

## 🎓 下一步：Qt开发

学完C++基础后，推荐：
1. **Qt官方文档**：https://doc.qt.io/
2. **Qt Creator IDE**：https://www.qt.io/download
3. **QML教程**：类似声明式UI（Jetpack Compose风格）

---

**祝学习愉快！有问题随时提问 🎉**
