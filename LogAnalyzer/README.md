# LogAnalyzer

一个高性能的 C++ 日志分析工具，用于解析、统计和分析各种格式的日志文件。

## 🎯 项目目标

- 📊 解析多种日志格式（Apache、Nginx、应用日志等）
- 📈 提供详细的统计分析
- 🔍 支持灵活的搜索和过滤
- ⚡ 高效处理大型日志文件
- 📱 生成多格式报告（文本、HTML、JSON）

## 🛠️ 技术栈

- **语言**: C++17
- **构建系统**: CMake
- **核心特性**: STL容器、多线程、正则表达式、智能指针
- **测试**: 单元测试（计划使用Google Test）

## 📁 项目结构

```
LogAnalyzer/
├── CMakeLists.txt          # 构建配置
├── README.md               # 项目说明
├── src/                    # 源代码
│   ├── main.cpp           
│   ├── LogEntry.cpp       
│   ├── LogParser.cpp      
│   └── LogAnalyzer.cpp    
├── include/                # 头文件
│   ├── LogEntry.h         
│   ├── LogParser.h        
│   └── LogAnalyzer.h      
├── tests/                  # 单元测试
├── examples/               # 示例数据
└── docs/                   # 文档
```

## 🚀 快速开始

### 构建项目

```bash
mkdir build
cd build
cmake ..
make
```

### 运行示例

```bash
# 基本统计
./loganalyzer --file access.log --stats

# 搜索过滤
./loganalyzer --file app.log --search "database" --level ERROR

# 生成报告
./loganalyzer --file *.log --report html
```

## 📈 功能特性

- [ ] 基本日志解析
- [ ] 日志级别统计
- [ ] 时间范围过滤
- [ ] 关键词搜索
- [ ] 多线程处理
- [ ] 报告生成
- [ ] 性能优化

## 🤝 贡献

这是一个学习项目，欢迎提出建议和改进意见！

## 📜 许可证

MIT License