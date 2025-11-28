# Day 1 学习笔记模板

## 🎯 今日目标
- [ ] 阅读项目README，理解功能需求  
- [ ] 分析目录结构：为什么这样组织？
- [ ] 研究CMakeLists.txt：构建系统如何工作？
- [ ] 运行程序，体验所有功能
- [ ] 画出类的关系图

## 📚 学到的概念
### 1. 项目目录结构
**标准C++项目结构**:
```
LogAnalyzer/
├── include/        # 头文件 (.h)
├── src/           # 源文件 (.cpp)  
├── examples/      # 示例文件
├── tests/         # 测试文件
├── build/         # 构建输出
├── CMakeLists.txt # 构建配置
└── README.md      # 项目文档
```

**为什么这样组织？**
- 

### 2. CMake构建系统
**关键命令理解**:
```cmake
# 你对每行的理解
cmake_minimum_required(VERSION 3.16)  # 
project(LogAnalyzer VERSION 1.0.0)    # 
set(CMAKE_CXX_STANDARD 17)             #
```

### 3. 命名空间
**LogAnalyzer命名空间的作用**:
- 

## 🔍 功能分析
### 程序运行测试
```bash
# 测试的命令和结果
./loganalyzer --help
# 结果：

./loganalyzer --stats examples/sample.log  
# 结果：
```

### 功能清单
- [ ] 日志解析
- [ ] 统计分析  
- [ ] 级别过滤
- [ ] 格式检测
- [ ] 最近日志显示

## 🎨 类关系图
```
画出你理解的类关系图：

LogEntry ←→ LogParser ←→ main.cpp
   ↓
LogLevel (enum)
```

## ❓ 遇到的问题
1. **问题**: CMakeLists.txt中的某行不理解
   - **解决方案**: 查阅资料/明天重点学习

2. **问题**: 为什么需要namespace？
   - **解决方案**: 

## 💭 思考题答案
1. **为什么要分离 include/ 和 src/ 目录？**
   - 你的理解：

2. **为什么使用命名空间？**  
   - 你的理解：

3. **CMakeLists.txt 中每一行的作用是什么？**
   - 你的理解：

## 📝 今日收获
1. 
2. 
3. 

## 🚀 明日计划
- 深度分析 LogEntry.h 和 LogEntry.cpp
- 理解类的设计原则
- 学习现代C++特性的具体应用

---
**学习时间**: ___小时___分钟
**理解程度**: ⭐⭐⭐⭐⭐ (1-5星)