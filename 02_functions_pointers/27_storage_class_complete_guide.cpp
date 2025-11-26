/**
 * C++ 存储类完全指南 - 系统性理解
 * 
 * 核心问题：
 * 1. C++ 存储类一共有几种？
 * 2. 它们可以用于什么？（文件、变量、函数、类）
 * 3. 为什么感觉难掌握？问题出在哪？
 */

#include <iostream>
#include <string>

using namespace std;

// ============================================
// 1. 存储类的完整分类
// ============================================

void explain_storage_class_overview() {
    cout << "\n" << string(70, '=') << endl;
    cout << "1. C++ 存储类完整分类" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【为什么感觉难掌握？问题出在这里！】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
问题根源：
❌ 存储类（Storage Class）这个术语有误导性！
❌ 它不是一个统一的概念，而是多个不同机制的混合
❌ 传统教材把它们放在一起讲，造成混淆

正确理解：
✅ 应该分为三类独立的概念来学习
✅ 它们解决不同的问题
✅ 适用范围也不同


【三大类概念】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

第一类：生命周期和作用域（核心）
┌─────────────────────────────────────────┐
│ auto（已废弃，现在是类型推导）            │
│ static（三种不同用法！）                  │
│ extern（跨文件链接）                      │
│ register（已废弃，编译器自动优化）        │
└─────────────────────────────────────────┘
作用：控制变量的生命周期和可见性


第二类：类型限定符（常被混淆）
┌─────────────────────────────────────────┐
│ const（只读）                             │
│ volatile（禁止优化）                      │
│ mutable（const 中可修改）                 │
└─────────────────────────────────────────┘
作用：控制变量的读写特性


第三类：现代 C++ 特性
┌─────────────────────────────────────────┐
│ thread_local（C++11，线程局部存储）       │
│ inline（C++17，可用于变量）               │
│ constexpr（编译时常量）                   │
└─────────────────────────────────────────┘
作用：现代特性，解决特定问题


【真正需要掌握的只有 2 个！】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
1. static（最重要，但有 3 种不同含义）
2. extern（多文件协作）

其他的：
- auto：现在是类型推导（auto x = 10;）
- register：已废弃，不用管
- const/volatile：不属于存储类，是类型修饰符
- thread_local：高级特性，暂时不用
- inline：C++17 后才能用于变量

)" << endl;
}

// ============================================
// 2. static 的三种用法（最容易混淆！）
// ============================================

// 用法1：静态局部变量
void staticLocalVariable() {
    static int counter = 0;  // 只初始化一次
    counter++;
    cout << "函数被调用 " << counter << " 次" << endl;
}

// 用法2：静态全局变量（文件作用域）
static int filePrivateVar = 100;  // 只在当前文件可见

// 用法3：类静态成员（稍后演示）

void explain_static_three_uses() {
    cout << "\n" << string(70, '=') << endl;
    cout << "2. static 的三种用法（核心！）" << endl;
    cout << string(70, '=') << endl;
    
    cout << "\n【为什么 static 难理解？】" << endl;
    cout << "因为同一个关键字，在不同位置，意义完全不同！\n" << endl;
    
    cout << "用法1：静态局部变量（函数内）" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "位置：函数内部" << endl;
    cout << "作用：保持变量值，函数调用之间不销毁" << endl;
    cout << "生命周期：程序运行期间" << endl;
    cout << "可见性：只在函数内可见" << endl;
    cout << "\n示例：" << endl;
    cout << "  void func() {" << endl;
    cout << "      static int count = 0;  // 只初始化一次" << endl;
    cout << "      count++;" << endl;
    cout << "  }" << endl;
    
    cout << "\n实际演示：" << endl;
    staticLocalVariable();  // 1
    staticLocalVariable();  // 2
    staticLocalVariable();  // 3
    
    cout << "\n用法2：静态全局变量/函数（文件外）" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "位置：函数外部（全局作用域）" << endl;
    cout << "作用：限制变量/函数只在当前文件可见" << endl;
    cout << "生命周期：程序运行期间" << endl;
    cout << "可见性：只在当前文件" << endl;
    cout << "用途：避免命名冲突，隐藏实现" << endl;
    cout << "\n示例：" << endl;
    cout << "  static int fileVar = 10;  // 只在本文件可见" << endl;
    cout << "  static void helper() { }  // 只在本文件可见" << endl;
    cout << "\n当前文件的静态变量值：" << filePrivateVar << endl;
    
    cout << "\n用法3：类静态成员（类内）" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "位置：类定义内部" << endl;
    cout << "作用：所有对象共享，不属于单个对象" << endl;
    cout << "生命周期：程序运行期间" << endl;
    cout << "可见性：通过类名访问" << endl;
    cout << "\n示例：" << endl;
    cout << "  class Counter {" << endl;
    cout << "  public:" << endl;
    cout << "      static int total;  // 所有对象共享" << endl;
    cout << "  };" << endl;
    cout << "  Counter::total = 0;  // 需要在类外定义" << endl;
}

// ============================================
// 3. 适用范围速查表
// ============================================

void show_applicability_table() {
    cout << "\n" << string(70, '=') << endl;
    cout << "3. 存储类适用范围速查表" << endl;
    cout << string(70, '=') << endl;
    
    cout << "\n【全局变量（文件作用域）】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "可用：" << endl;
    cout << "  ✅ static        - 限制在当前文件" << endl;
    cout << "  ✅ extern        - 声明在其他文件定义" << endl;
    cout << "  ✅ const         - 只读全局变量" << endl;
    cout << "  ✅ constexpr     - 编译时常量" << endl;
    cout << "  ✅ inline (C++17)- 头文件中定义" << endl;
    cout << "\n不可用：" << endl;
    cout << "  ❌ auto (已废弃) - 全局变量不能用" << endl;
    cout << "  ❌ register      - 全局变量不能用" << endl;
    
    cout << "\n【局部变量（函数内）】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "可用：" << endl;
    cout << "  ✅ static        - 保持值，不销毁" << endl;
    cout << "  ✅ auto (C++11+) - 类型推导（不是存储类了）" << endl;
    cout << "  ✅ const         - 只读局部变量" << endl;
    cout << "  ✅ constexpr     - 编译时常量" << endl;
    cout << "\n不可用：" << endl;
    cout << "  ❌ extern        - 局部变量不能 extern" << endl;
    cout << "  ❌ register (已废弃) - 编译器自动优化" << endl;
    
    cout << "\n【函数】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "可用：" << endl;
    cout << "  ✅ static        - 限制在当前文件" << endl;
    cout << "  ✅ extern        - 默认就是（可省略）" << endl;
    cout << "  ✅ inline        - 内联函数" << endl;
    cout << "  ✅ constexpr     - 编译时执行" << endl;
    cout << "\n不可用：" << endl;
    cout << "  ❌ auto          - 函数不能用" << endl;
    cout << "  ❌ register      - 函数不能用" << endl;
    cout << "  ❌ const         - 函数不能 const（但成员函数可以）" << endl;
    
    cout << "\n【类成员】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "可用：" << endl;
    cout << "  ✅ static        - 所有对象共享" << endl;
    cout << "  ✅ const         - 只读成员" << endl;
    cout << "  ✅ mutable       - const 函数中可修改" << endl;
    cout << "  ✅ constexpr     - 编译时常量成员" << endl;
    cout << "  ✅ inline (C++17)- 类内初始化静态成员" << endl;
    cout << "\n不可用：" << endl;
    cout << "  ❌ extern        - 类成员不能 extern" << endl;
    cout << "  ❌ auto          - 成员变量不能 auto" << endl;
    cout << "  ❌ register      - 已废弃" << endl;
    
    cout << "\n【类本身】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "类定义前不能加存储类说明符！" << endl;
    cout << "  ❌ static class MyClass { };  // 错误！" << endl;
    cout << "  ❌ extern class MyClass { };  // 错误！" << endl;
    cout << "\n但可以在类外声明类：" << endl;
    cout << "  ✅ class MyClass;  // 前向声明" << endl;
}

// ============================================
// 4. 类静态成员演示
// ============================================

class Counter {
public:
    static int totalCount;  // 声明
    int instanceId;
    
    Counter() {
        instanceId = ++totalCount;
    }
    
    static void showTotal() {  // 静态成员函数
        cout << "总共创建了 " << totalCount << " 个对象" << endl;
    }
};

// 静态成员必须在类外定义
int Counter::totalCount = 0;

void demonstrate_class_static() {
    cout << "\n" << string(70, '=') << endl;
    cout << "4. 类静态成员演示" << endl;
    cout << string(70, '=') << endl;
    
    cout << "\n类静态成员的特点：" << endl;
    cout << "  ✅ 所有对象共享同一个变量" << endl;
    cout << "  ✅ 可以通过类名直接访问" << endl;
    cout << "  ✅ 不属于任何单个对象" << endl;
    cout << "  ✅ 必须在类外定义" << endl;
    
    cout << "\n创建对象：" << endl;
    Counter c1;
    cout << "c1.instanceId = " << c1.instanceId << endl;
    
    Counter c2;
    cout << "c2.instanceId = " << c2.instanceId << endl;
    
    Counter c3;
    cout << "c3.instanceId = " << c3.instanceId << endl;
    
    cout << "\n通过类名访问静态成员：" << endl;
    Counter::showTotal();
    
    cout << "\n通过对象访问（也可以，但不推荐）：" << endl;
    cout << "c1.totalCount = " << c1.totalCount << endl;
}

// ============================================
// 5. extern 详解
// ============================================

// extern 声明（变量在其他文件定义）
extern int sharedGlobalVar;  // 只声明，不定义

void explain_extern() {
    cout << "\n" << string(70, '=') << endl;
    cout << "5. extern 详解" << endl;
    cout << string(70, '=') << endl;
    
    cout << "\nextern 的作用：" << endl;
    cout << "  ✅ 声明变量/函数在其他文件定义" << endl;
    cout << "  ✅ 让多个文件共享全局变量" << endl;
    cout << "  ✅ 函数默认就是 extern（可省略）" << endl;
    
    cout << "\n典型用法：" << endl;
    cout << "\n// config.h（头文件）" << endl;
    cout << "extern int maxConnections;  // 声明" << endl;
    cout << "extern void initConfig();   // 声明（可省略 extern）" << endl;
    
    cout << "\n// config.cpp（源文件）" << endl;
    cout << "int maxConnections = 100;   // 定义" << endl;
    cout << "void initConfig() { ... }   // 定义" << endl;
    
    cout << "\n// main.cpp" << endl;
    cout << "#include \"config.h\"" << endl;
    cout << "int main() {" << endl;
    cout << "    maxConnections = 200;  // 使用" << endl;
    cout << "    initConfig();          // 使用" << endl;
    cout << "}" << endl;
    
    cout << "\n关键理解：" << endl;
    cout << "  声明（Declaration）：告诉编译器存在" << endl;
    cout << "  定义（Definition）：  分配内存，只能一次" << endl;
    cout << "  extern 变量：        只声明，不定义" << endl;
    cout << "  extern 函数：        声明（但可省略 extern）" << endl;
}

// ============================================
// 6. 常见误区和陷阱
// ============================================

void explain_common_mistakes() {
    cout << "\n" << string(70, '=') << endl;
    cout << "6. 常见误区和陷阱" << endl;
    cout << string(70, '=') << endl;
    
    cout << "\n【误区1：混淆 static 的三种用法】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "错误理解：static 就是\"静态\"" << endl;
    cout << "正确理解：static 在不同位置，意义完全不同" << endl;
    cout << "  - 函数内：保持值" << endl;
    cout << "  - 文件内：限制作用域" << endl;
    cout << "  - 类内：  共享成员" << endl;
    
    cout << "\n【误区2：const 是存储类】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "错误：把 const 当作存储类" << endl;
    cout << "正确：const 是类型限定符（type qualifier）" << endl;
    cout << "  可以和存储类组合：" << endl;
    cout << "    static const int x = 10;  // static + const" << endl;
    cout << "    extern const int y = 20;  // extern + const" << endl;
    
    cout << "\n【误区3：类可以用存储类修饰】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "❌ static class MyClass { };  // 错误！" << endl;
    cout << "✅ class MyClass { };          // 正确" << endl;
    cout << "\n类本身不能用存储类修饰" << endl;
    cout << "但类的成员可以用 static" << endl;
    
    cout << "\n【误区4：在头文件定义非 inline 变量】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "// myheader.h" << endl;
    cout << "int globalVar = 10;  // ❌ 错误！多次包含会重复定义" << endl;
    cout << "" << endl;
    cout << "正确做法：" << endl;
    cout << "方式1：用 extern" << endl;
    cout << "  // myheader.h" << endl;
    cout << "  extern int globalVar;  // 声明" << endl;
    cout << "  // myheader.cpp" << endl;
    cout << "  int globalVar = 10;    // 定义" << endl;
    cout << "" << endl;
    cout << "方式2：用 inline (C++17)" << endl;
    cout << "  // myheader.h" << endl;
    cout << "  inline int globalVar = 10;  // ✅ 可以在头文件定义" << endl;
    
    cout << "\n【误区5：忘记定义类静态成员】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "class MyClass {" << endl;
    cout << "public:" << endl;
    cout << "    static int count;  // 只是声明" << endl;
    cout << "};" << endl;
    cout << "// 忘记定义！会导致链接错误" << endl;
    cout << "" << endl;
    cout << "正确：" << endl;
    cout << "int MyClass::count = 0;  // 必须在类外定义" << endl;
}

// ============================================
// 7. 学习建议和记忆方法
// ============================================

void learning_tips() {
    cout << "\n" << string(70, '=') << endl;
    cout << "7. 学习建议和记忆方法" << endl;
    cout << string(70, '=') << endl;
    
    cout << "\n【为什么难掌握？问题诊断】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "1. 术语误导：\"存储类\"听起来像一个统一概念" << endl;
    cout << "   实际：是多个不相关机制的集合" << endl;
    cout << "" << endl;
    cout << "2. static 一词多义：同一个关键字，三种不同含义" << endl;
    cout << "   解决：分别学习，根据位置判断" << endl;
    cout << "" << endl;
    cout << "3. 概念混杂：存储类、类型限定符、现代特性混在一起" << endl;
    cout << "   解决：分类学习，明确每个属于哪一类" << endl;
    
    cout << "\n【简化学习路径】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "阶段1：只学这 2 个（覆盖 90% 场景）" << endl;
    cout << "  1. static（文件作用域）：隐藏实现" << endl;
    cout << "  2. extern：           多文件共享" << endl;
    cout << "" << endl;
    cout << "阶段2：类相关" << endl;
    cout << "  3. static（类成员）： 共享成员" << endl;
    cout << "  4. const（类成员）：  只读成员" << endl;
    cout << "" << endl;
    cout << "阶段3：现代 C++（可选）" << endl;
    cout << "  5. inline 变量（C++17）：头文件定义" << endl;
    cout << "  6. constexpr：        编译时常量" << endl;
    cout << "  7. thread_local：     线程局部存储" << endl;
    
    cout << "\n【记忆技巧】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "static 看位置：" << endl;
    cout << "  - 函数内：     \"静态局部\" = 保持不丢" << endl;
    cout << "  - 函数外文件内：\"文件私有\" = 不给别人" << endl;
    cout << "  - 类内：       \"类级共享\" = 大家都有" << endl;
    cout << "" << endl;
    cout << "extern 想声明：" << endl;
    cout << "  - 变量：\"我只是说它存在，真身在别处\"" << endl;
    cout << "  - 函数：\"可加可不加，默认就是 extern\"" << endl;
    
    cout << "\n【实用决策树】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "需要多文件共享全局变量？" << endl;
    cout << "  ├─ 是 → 用 extern 声明 + 一处定义" << endl;
    cout << "  └─ 否 → 不需要存储类" << endl;
    cout << "" << endl;
    cout << "需要隐藏实现细节（函数/变量）？" << endl;
    cout << "  ├─ 是 → 用 static（文件作用域）" << endl;
    cout << "  └─ 否 → 不需要存储类" << endl;
    cout << "" << endl;
    cout << "需要所有对象共享一个变量？" << endl;
    cout << "  ├─ 是 → 用 static（类成员）" << endl;
    cout << "  └─ 否 → 不需要存储类" << endl;
    cout << "" << endl;
    cout << "其他情况？" << endl;
    cout << "  └─ 基本不需要存储类！" << endl;
}

// ============================================
// 8. 实战示例
// ============================================

void practical_examples() {
    cout << "\n" << string(70, '=') << endl;
    cout << "8. 实战示例" << endl;
    cout << string(70, '=') << endl;
    
    cout << "\n【场景1：日志系统】" << endl;
    cout << "需求：多个文件共享日志级别" << endl;
    cout << "\n// logger.h" << endl;
    cout << "extern int logLevel;      // 声明" << endl;
    cout << "void log(const char* msg);" << endl;
    cout << "" << endl;
    cout << "// logger.cpp" << endl;
    cout << "int logLevel = 1;         // 定义" << endl;
    cout << "static int logCount = 0;  // 文件私有" << endl;
    cout << "" << endl;
    cout << "void log(const char* msg) {" << endl;
    cout << "    if (level >= logLevel) {" << endl;
    cout << "        logCount++;  // 内部计数" << endl;
    cout << "        // ..." << endl;
    cout << "    }" << endl;
    cout << "}" << endl;
    
    cout << "\n【场景2：单例模式】" << endl;
    cout << "需求：确保只有一个实例" << endl;
    cout << "\nclass Singleton {" << endl;
    cout << "private:" << endl;
    cout << "    static Singleton* instance;  // 静态成员" << endl;
    cout << "    Singleton() { }              // 私有构造" << endl;
    cout << "public:" << endl;
    cout << "    static Singleton* getInstance() {" << endl;
    cout << "        if (!instance) {" << endl;
    cout << "            instance = new Singleton();" << endl;
    cout << "        }" << endl;
    cout << "        return instance;" << endl;
    cout << "    }" << endl;
    cout << "};" << endl;
    cout << "" << endl;
    cout << "Singleton* Singleton::instance = nullptr;" << endl;
    
    cout << "\n【场景3：计数器】" << endl;
    cout << "需求：跟踪函数调用次数" << endl;
    cout << "\nvoid processData() {" << endl;
    cout << "    static int callCount = 0;  // 静态局部变量" << endl;
    cout << "    callCount++;" << endl;
    cout << "    cout << \"Called \" << callCount << \" times\" << endl;" << endl;
    cout << "}" << endl;
}

// ============================================
// Main
// ============================================

int main() {
    cout << "======================================================================" << endl;
    cout << "  C++ 存储类完全指南 - 系统性理解" << endl;
    cout << "======================================================================" << endl;
    
    explain_storage_class_overview();
    explain_static_three_uses();
    show_applicability_table();
    demonstrate_class_static();
    explain_extern();
    explain_common_mistakes();
    learning_tips();
    practical_examples();
    
    cout << "\n======================================================================" << endl;
    cout << "核心总结" << endl;
    cout << "======================================================================" << endl;
    
    cout << "\n【问题1：C++ 存储类一共有几种？】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "传统说法：4 种（auto, register, static, extern）" << endl;
    cout << "实际情况：只需要掌握 2 种" << endl;
    cout << "  1. static（3 种不同用法）" << endl;
    cout << "  2. extern（多文件协作）" << endl;
    cout << "" << endl;
    cout << "其他的：" << endl;
    cout << "  - auto：已废弃（现在是类型推导）" << endl;
    cout << "  - register：已废弃（编译器自动优化）" << endl;
    
    cout << "\n【问题2：可以用于什么？】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "全局变量：static（文件私有）、extern（多文件共享）" << endl;
    cout << "局部变量：static（保持值）" << endl;
    cout << "函数：    static（文件私有）、extern（默认，可省略）" << endl;
    cout << "类成员：  static（共享成员）" << endl;
    cout << "类本身：  不能用存储类修饰！" << endl;
    
    cout << "\n【问题3：为什么感觉难掌握？】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "根本原因：" << endl;
    cout << "  ❌ \"存储类\"这个术语有误导性" << endl;
    cout << "  ❌ static 一词多义（3 种不同含义）" << endl;
    cout << "  ❌ 概念混杂（存储类 + 类型限定符 + 现代特性）" << endl;
    cout << "" << endl;
    cout << "解决方案：" << endl;
    cout << "  ✅ 分类学习（不要当作统一概念）" << endl;
    cout << "  ✅ 重点掌握 static 和 extern" << endl;
    cout << "  ✅ 根据位置判断 static 的含义" << endl;
    
    cout << "\n【快速决策】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "多文件共享？      → extern" << endl;
    cout << "文件内隐藏？      → static（文件作用域）" << endl;
    cout << "函数内保持值？    → static（局部变量）" << endl;
    cout << "类成员共享？      → static（类成员）" << endl;
    cout << "其他情况？        → 基本不需要！" << endl;
    
    cout << "\n【记忆口诀】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "存储类不多，只需记两个" << endl;
    cout << "static 看位置，意思各不同" << endl;
    cout << "函数内保值，文件外隐藏" << endl;
    cout << "类内表共享，extern 跨文件" << endl;
    cout << "auto 已过时，register 别管它" << endl;
    cout << "const 不算数，那是类型符" << endl;
    cout << "======================================================================" << endl;
    
    return 0;
}

/**
 * 终极总结：
 * 
 * 1. 为什么难？
 *    - "存储类"术语误导
 *    - static 一词多义
 *    - 概念混杂不清
 * 
 * 2. 怎么学？
 *    - 分类学习，不当统一概念
 *    - 重点掌握 static 和 extern
 *    - 通过位置判断 static 含义
 * 
 * 3. 用在哪？
 *    - 全局：static（私有）、extern（共享）
 *    - 局部：static（保值）
 *    - 函数：static（私有）
 *    - 类成员：static（共享）
 *    - 类本身：不能用！
 * 
 * 4. 记住啥？
 *    90% 的情况只需要：
 *    - static（文件作用域）
 *    - extern（多文件）
 *    其他的基本不用！
 */
