/*
 * C++ 能力提升路线图
 * 从基础掌握到高级开发者的完整学习路径
 */

#include <iostream>
#include <vector>
#include <string>
using namespace std;

void print_learning_path() {
    cout << "\n╔══════════════════════════════════════════════════════════╗" << endl;
    cout << "║                C++ 能力提升路线图                        ║" << endl;
    cout << "║            从基础掌握到高级开发者                        ║" << endl;
    cout << "╚══════════════════════════════════════════════════════════╝" << endl;
    
    cout << "\n🎯 当前状态评估：" << endl;
    cout << "  ✅ 基本语法：变量、函数、类、继承" << endl;
    cout << "  ✅ 内存管理：指针、引用、智能指针" << endl;
    cout << "  ✅ 容器和算法：STL 基础使用" << endl;
    cout << "  ✅ 多线程：thread、mutex、async" << endl;
    cout << "  ✅ 现代C++：Lambda、RAII、移动语义" << endl;
    cout << "  📚 代码阅读：能理解大部分 C++ 代码" << endl;
    
    cout << "\n🚀 提升目标：" << endl;
    cout << "  🎯 编写高质量、可维护的 C++ 代码" << endl;
    cout << "  🎯 掌握 C++ 最佳实践和设计模式" << endl;
    cout << "  🎯 具备解决复杂问题的能力" << endl;
    cout << "  🎯 能够参与实际项目开发" << endl;
}

void stage_1_consolidation() {
    cout << "\n" << string(60, '=') << endl;
    cout << "阶段一：巩固基础 + 实践项目（1-2周）" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n📚 1.1 深化核心概念" << endl;
    cout << "  🔸 异常处理：try-catch、自定义异常、RAII与异常安全" << endl;
    cout << "  🔸 模板进阶：模板特化、SFINAE、概念(Concepts)" << endl;
    cout << "  🔸 内存管理：内存池、对象池、自定义allocator" << endl;
    cout << "  🔸 并发编程：线程池、生产者消费者、读写锁" << endl;
    
    cout << "\n🛠️ 1.2 实践项目建议" << endl;
    cout << "  💡 项目1：命令行工具" << endl;
    cout << "     - 文件处理工具（搜索、替换、统计）" << endl;
    cout << "     - 日志分析器" << endl;
    cout << "     - 简单的计算器或表达式求值器" << endl;
    
    cout << "\n  💡 项目2：数据结构实现" << endl;
    cout << "     - 自实现 vector、list、map" << endl;
    cout << "     - 线程安全的容器" << endl;
    cout << "     - LRU 缓存实现" << endl;
    
    cout << "\n  💡 项目3：简单服务器" << endl;
    cout << "     - HTTP 服务器" << endl;
    cout << "     - 聊天服务器" << endl;
    cout << "     - 文件传输工具" << endl;
    
    cout << "\n🎯 学习重点：" << endl;
    cout << "  ✅ 代码组织：头文件、源文件分离" << endl;
    cout << "  ✅ 构建系统：CMake 基础" << endl;
    cout << "  ✅ 调试技能：GDB、内存检查工具" << endl;
    cout << "  ✅ 版本控制：Git 进阶使用" << endl;
}

void stage_2_advanced_concepts() {
    cout << "\n" << string(60, '=') << endl;
    cout << "阶段二：高级概念学习（2-3周）" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n🚀 2.1 现代 C++ 特性深入" << endl;
    cout << "  🔸 C++11/14/17/20 新特性" << endl;
    cout << "     - auto、decltype、constexpr" << endl;
    cout << "     - 完美转发、万能引用" << endl;
    cout << "     - 协程 (C++20)" << endl;
    cout << "     - 模块系统 (C++20)" << endl;
    
    cout << "\n  🔸 模板元编程" << endl;
    cout << "     - SFINAE 和 enable_if" << endl;
    cout << "     - 类型萃取 (type traits)" << endl;
    cout << "     - 变参模板" << endl;
    cout << "     - 编译期计算" << endl;
    
    cout << "\n🏗️ 2.2 设计模式与架构" << endl;
    cout << "  🔸 创建型模式：单例、工厂、建造者" << endl;
    cout << "  🔸 结构型模式：适配器、装饰器、代理" << endl;
    cout << "  🔸 行为型模式：观察者、策略、命令" << endl;
    cout << "  🔸 SOLID 原则在 C++ 中的应用" << endl;
    
    cout << "\n⚡ 2.3 性能优化" << endl;
    cout << "  🔸 性能分析：Profiling 工具使用" << endl;
    cout << "  🔸 算法优化：时间复杂度、空间复杂度" << endl;
    cout << "  🔸 编译器优化：-O2、-O3、LTO" << endl;
    cout << "  🔸 缓存友好编程：数据局部性" << endl;
    
    cout << "\n💡 推荐项目：" << endl;
    cout << "  📦 实现一个小型数据库" << endl;
    cout << "  🎮 开发简单游戏引擎" << endl;
    cout << "  🌐 网络库封装" << endl;
}

void stage_3_professional_development() {
    cout << "\n" << string(60, '=') << endl;
    cout << "阶段三：专业开发能力（3-4周）" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n🏢 3.1 企业级开发技能" << endl;
    cout << "  🔸 代码规范：Google、LLVM 代码风格" << endl;
    cout << "  🔸 代码审查：Code Review 最佳实践" << endl;
    cout << "  🔸 单元测试：GoogleTest、Catch2" << endl;
    cout << "  🔸 持续集成：GitHub Actions、Jenkins" << endl;
    
    cout << "\n🛠️ 3.2 工具链精通" << endl;
    cout << "  🔸 构建系统：CMake 高级用法、Bazel" << endl;
    cout << "  🔸 包管理：Conan、vcpkg" << endl;
    cout << "  🔸 静态分析：Clang-tidy、PVS-Studio" << endl;
    cout << "  🔸 动态分析：Valgrind、AddressSanitizer" << endl;
    
    cout << "\n🚀 3.3 领域专业化" << endl;
    cout << "  💻 系统编程：操作系统内核、驱动开发" << endl;
    cout << "  🎮 游戏开发：图形编程、物理引擎" << endl;
    cout << "  🌐 网络编程：高性能服务器、分布式系统" << endl;
    cout << "  🤖 高性能计算：GPU 编程、并行计算" << endl;
    cout << "  📱 嵌入式：IoT、实时系统" << endl;
    
    cout << "\n💡 大型项目建议：" << endl;
    cout << "  🏗️ 贡献开源项目（如 LLVM、Boost）" << endl;
    cout << "  🎯 开发完整的应用程序" << endl;
    cout << "  📚 写技术博客分享经验" << endl;
}

void learning_resources() {
    cout << "\n" << string(60, '=') << endl;
    cout << "📚 学习资源推荐" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n📖 经典书籍：" << endl;
    cout << "  🥇 《Effective C++》 - Scott Meyers" << endl;
    cout << "  🥇 《More Effective C++》 - Scott Meyers" << endl;
    cout << "  🥇 《Effective Modern C++》 - Scott Meyers" << endl;
    cout << "  📗 《C++ Primer》 - Stanley Lippman" << endl;
    cout << "  📗 《The C++ Programming Language》 - Bjarne Stroustrup" << endl;
    
    cout << "\n💻 在线资源：" << endl;
    cout << "  🌐 CppReference.com - 权威文档" << endl;
    cout << "  🎥 CppCon 演讲视频 - 顶级专家分享" << endl;
    cout << "  🏆 LeetCode C++ 刷题 - 算法练习" << endl;
    cout << "  📺 Jason Turner's C++ Weekly" << endl;
    
    cout << "\n🛠️ 实践平台：" << endl;
    cout << "  💻 GitHub - 开源项目贡献" << endl;
    cout << "  🎮 Game Jams - 游戏开发竞赛" << endl;
    cout << "  🏆 编程竞赛 - ACM、Google Code Jam" << endl;
    cout << "  💼 实习项目 - 实际工程经验" << endl;
}

void practical_advice() {
    cout << "\n" << string(60, '=') << endl;
    cout << "💡 实用建议" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n🎯 学习方法：" << endl;
    cout << "  📝 1. 理论学习 + 立即实践" << endl;
    cout << "     - 学一个概念就写代码验证" << endl;
    cout << "     - 不要只看不动手" << endl;
    
    cout << "\n  🔧 2. 阅读优秀代码" << endl;
    cout << "     - 研究 STL 实现" << endl;
    cout << "     - 阅读知名开源项目" << endl;
    cout << "     - 分析代码设计思路" << endl;
    
    cout << "\n  🤝 3. 参与社区" << endl;
    cout << "     - Stack Overflow 回答问题" << endl;
    cout << "     - GitHub 贡献代码" << endl;
    cout << "     - 技术博客分享" << endl;
    
    cout << "\n⏱️ 时间规划：" << endl;
    cout << "  🌅 每天 2-3 小时编程练习" << endl;
    cout << "  📅 每周 1-2 个小项目" << endl;
    cout << "  📚 每月读 1 本技术书籍" << endl;
    cout << "  🎯 每季度完成 1 个大项目" << endl;
    
    cout << "\n🚀 进阶路径：" << endl;
    cout << "  📊 Level 1: 能写基本程序（已达到）" << endl;
    cout << "  📈 Level 2: 能写高质量代码（目标）" << endl;
    cout << "  🏆 Level 3: 能设计系统架构" << endl;
    cout << "  🌟 Level 4: 能解决复杂技术问题" << endl;
    cout << "  👑 Level 5: 技术专家/架构师" << endl;
}

void immediate_action_plan() {
    cout << "\n" << string(60, '=') << endl;
    cout << "🎯 立即行动计划（接下来2周）" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n📅 第1周：深化理解" << endl;
    cout << "  🌟 Day 1-2: 异常处理深入学习" << endl;
    cout << "     - 编写异常安全的代码" << endl;
    cout << "     - RAII 与异常的配合" << endl;
    
    cout << "\n  🌟 Day 3-4: 模板高级特性" << endl;
    cout << "     - 模板特化和偏特化" << endl;
    cout << "     - SFINAE 技术" << endl;
    
    cout << "\n  🌟 Day 5-7: 项目实践" << endl;
    cout << "     - 选择一个命令行工具项目开始" << endl;
    cout << "     - 使用现代 C++ 特性重构" << endl;
    
    cout << "\n📅 第2周：项目驱动" << endl;
    cout << "  🚀 选择项目方向：" << endl;
    cout << "     Option A: 日志分析工具" << endl;
    cout << "     Option B: 简单 HTTP 服务器" << endl;
    cout << "     Option C: 数据结构库实现" << endl;
    
    cout << "\n  📋 项目要求：" << endl;
    cout << "     ✅ 使用现代 C++ 特性" << endl;
    cout << "     ✅ 良好的代码组织" << endl;
    cout << "     ✅ 异常处理和错误管理" << endl;
    cout << "     ✅ 单元测试覆盖" << endl;
    cout << "     ✅ 文档和注释" << endl;
}

int main() {
    print_learning_path();
    stage_1_consolidation();
    stage_2_advanced_concepts();
    stage_3_professional_development();
    learning_resources();
    practical_advice();
    immediate_action_plan();
    
    cout << "\n" << string(60, '=') << endl;
    cout << "🎉 结语" << endl;
    cout << string(60, '=') << endl;
    cout << "\n恭喜你掌握了 C++ 基础！接下来的路径就是：" << endl;
    cout << "💪 持续练习 → 🏗️ 项目实战 → 📚 深度学习 → 🌟 专业精进" << endl;
    cout << "\n记住：C++ 是一门需要大量实践的语言！" << endl;
    cout << "多写代码，多解决问题，你会越来越强！🚀" << endl;
    
    return 0;
}