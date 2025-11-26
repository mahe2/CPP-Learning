/**
 * C++ 函数声明与实现分离 - 深度解析
 * 
 * 核心问题：
 * 1. 为什么 C++ 要将声明和实现分开？
 * 2. 与 Java/Kotlin 的区别是什么？
 * 3. 什么时候必须分开？什么时候可以不分？
 */

#include <iostream>
#include <string>

using namespace std;

// ============================================
// 1. 基本概念：声明 vs 定义
// ============================================

/**
 * 声明（Declaration）：告诉编译器函数的存在
 * - 只有函数签名（名称、参数、返回类型）
 * - 不包含实现代码
 * - 可以声明多次
 */
int add(int a, int b);  // 声明

/**
 * 定义（Definition）：提供函数的实际实现
 * - 包含完整的函数体
 * - 只能定义一次（ODR: One Definition Rule）
 */
int add(int a, int b) {  // 定义
    return a + b;
}

// ============================================
// 2. 为什么要分离？原因分析
// ============================================

/**
 * 原因1: 编译模型不同
 * C++ 是分离编译模型，每个 .cpp 文件独立编译
 * Java/Kotlin 是统一编译模型，编译器能看到所有代码
 */

// 函数 B 需要调用函数 A
void functionB();  // 必须先声明

void functionA() {
    cout << "Function A calling Function B" << endl;
    functionB();  // 调用 B
}

void functionB() {
    cout << "Function B executed" << endl;
}

/**
 * 原因2: 避免循环依赖
 * 两个函数互相调用时，必须先声明
 */

// 前置声明
void funcX();
void funcY();

void funcX() {
    cout << "X calling Y" << endl;
    // funcY();  // 可以调用（已声明）
}

void funcY() {
    cout << "Y calling X" << endl;
    // funcX();  // 可以调用（已声明）
}

/**
 * 原因3: 编译速度优化
 * 头文件只包含声明，实现在 .cpp 中
 * 修改实现不需要重新编译所有依赖文件
 */

// ============================================
// 3. 头文件 (.h) 和源文件 (.cpp) 分离
// ============================================

/**
 * 典型的 C++ 项目结构：
 * 
 * calculator.h (头文件 - 声明)
 * --------------------------------
 * #ifndef CALCULATOR_H
 * #define CALCULATOR_H
 * 
 * int add(int a, int b);
 * int subtract(int a, int b);
 * int multiply(int a, int b);
 * 
 * #endif
 * 
 * 
 * calculator.cpp (源文件 - 实现)
 * --------------------------------
 * #include "calculator.h"
 * 
 * int add(int a, int b) {
 *     return a + b;
 * }
 * 
 * int subtract(int a, int b) {
 *     return a - b;
 * }
 * 
 * int multiply(int a, int b) {
 *     return a * b;
 * }
 */

void explain_header_source_separation() {
    cout << "\n" << string(60, '=') << endl;
    cout << "C++ 头文件和源文件分离" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n为什么要分离？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. 编译速度：" << endl;
    cout << "   - 头文件只有声明，很小" << endl;
    cout << "   - 修改 .cpp 实现不影响其他文件" << endl;
    cout << "   - 只需重新编译修改的文件" << endl;
    
    cout << "\n2. 信息隐藏：" << endl;
    cout << "   - 用户只看到接口（.h）" << endl;
    cout << "   - 实现细节在 .cpp 中" << endl;
    cout << "   - 可以保护商业代码（分发 .h + .lib）" << endl;
    
    cout << "\n3. 减少依赖：" << endl;
    cout << "   - 头文件不需要包含实现依赖" << endl;
    cout << "   - 降低编译依赖关系" << endl;
    
    cout << "\n4. 避免重复定义：" << endl;
    cout << "   - 多个文件可以 #include 同一个 .h" << endl;
    cout << "   - 实现只在 .cpp 中定义一次" << endl;
}

// ============================================
// 4. 什么时候可以不分离？
// ============================================

/**
 * 情况1: inline 函数
 * 内联函数必须在头文件中定义
 */
inline int square(int x) {  // 定义在头文件中
    return x * x;
}

/**
 * 情况2: constexpr 函数
 * constexpr 函数必须在头文件中定义
 */
constexpr int cube(int x) {
    return x * x * x;
}

/**
 * 情况3: 模板函数
 * 模板函数必须在头文件中定义
 */
template<typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
}

/**
 * 情况4: 类的内联成员函数
 * 在类定义内部实现的成员函数自动是 inline
 */
class Calculator {
public:
    // 在类内定义 = inline
    int add(int a, int b) {
        return a + b;
    }
    
    // 在类外声明，在类外定义
    int subtract(int a, int b);
};

// 类外定义
int Calculator::subtract(int a, int b) {
    return a - b;
}

void explain_when_not_separate() {
    cout << "\n" << string(60, '=') << endl;
    cout << "什么时候可以不分离？" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n1. inline 函数：" << endl;
    cout << "   inline int square(int x) { return x * x; }" << endl;
    cout << "   → 必须在头文件中定义" << endl;
    
    cout << "\n2. constexpr 函数：" << endl;
    cout << "   constexpr int cube(int x) { return x * x * x; }" << endl;
    cout << "   → 必须在头文件中定义" << endl;
    
    cout << "\n3. 模板函数/类：" << endl;
    cout << "   template<typename T> T max(T a, T b) { ... }" << endl;
    cout << "   → 必须在头文件中定义" << endl;
    
    cout << "\n4. 小型项目：" << endl;
    cout << "   → 可以全部写在一个 .cpp 文件中" << endl;
    
    cout << "\n5. 类的成员函数（在类内定义）：" << endl;
    cout << "   class A { int get() { return x; } };" << endl;
    cout << "   → 自动成为 inline" << endl;
}

// ============================================
// 5. 与 Java/Kotlin 对比
// ============================================

void compare_with_java_kotlin() {
    cout << "\n" << string(60, '=') << endl;
    cout << "与 Java/Kotlin 对比" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\nJava：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "public class Calculator {" << endl;
    cout << "    public int add(int a, int b) {" << endl;
    cout << "        return a + b;  // 声明和实现在一起" << endl;
    cout << "    }" << endl;
    cout << "}" << endl;
    cout << "\n特点：" << endl;
    cout << "✅ 声明即定义" << endl;
    cout << "✅ 编译器自动处理依赖" << endl;
    cout << "✅ 不需要前置声明" << endl;
    cout << "✅ 不需要头文件" << endl;
    cout << "⚠️  编译单元是类，不是文件" << endl;
    
    cout << "\nKotlin：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "class Calculator {" << endl;
    cout << "    fun add(a: Int, b: Int): Int {" << endl;
    cout << "        return a + b  // 声明和实现在一起" << endl;
    cout << "    }" << endl;
    cout << "}" << endl;
    cout << "\n特点：" << endl;
    cout << "✅ 声明即定义" << endl;
    cout << "✅ 更智能的依赖分析" << endl;
    cout << "✅ 不需要头文件" << endl;
    cout << "✅ 支持顶层函数（不需要类）" << endl;
    
    cout << "\nC++：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "// calculator.h" << endl;
    cout << "class Calculator {" << endl;
    cout << "public:" << endl;
    cout << "    int add(int a, int b);  // 声明" << endl;
    cout << "};" << endl;
    cout << "\n// calculator.cpp" << endl;
    cout << "int Calculator::add(int a, int b) {" << endl;
    cout << "    return a + b;  // 定义" << endl;
    cout << "}" << endl;
    cout << "\n特点：" << endl;
    cout << "✅ 声明和定义可以分离" << endl;
    cout << "✅ 编译速度快（增量编译）" << endl;
    cout << "✅ 信息隐藏（只分发 .h + .lib）" << endl;
    cout << "⚠️  需要手动管理头文件" << endl;
    cout << "⚠️  需要前置声明解决循环依赖" << endl;
}

// ============================================
// 6. 编译过程对比
// ============================================

void compilation_process() {
    cout << "\n" << string(60, '=') << endl;
    cout << "编译过程对比" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\nC++ 编译过程（分离编译）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. 预处理：展开头文件和宏" << endl;
    cout << "   calculator.cpp → calculator.i" << endl;
    cout << "\n2. 编译：.cpp → .o (目标文件)" << endl;
    cout << "   calculator.cpp → calculator.o" << endl;
    cout << "   main.cpp → main.o" << endl;
    cout << "   ⚠️  每个 .cpp 独立编译" << endl;
    cout << "   ⚠️  编译时只看头文件声明" << endl;
    cout << "\n3. 链接：所有 .o → 可执行文件" << endl;
    cout << "   calculator.o + main.o → program" << endl;
    cout << "   ⚠️  链接时才找到实际定义" << endl;
    
    cout << "\n优点：" << endl;
    cout << "  ✅ 修改一个 .cpp 只需重新编译这一个文件" << endl;
    cout << "  ✅ 大项目编译速度快" << endl;
    cout << "  ✅ 可以并行编译多个 .cpp" << endl;
    
    cout << "\n缺点：" << endl;
    cout << "  ❌ 需要手动管理头文件" << endl;
    cout << "  ❌ 头文件被多次包含（编译时间增加）" << endl;
    cout << "  ❌ 链接错误在链接阶段才发现" << endl;
    
    cout << "\nJava/Kotlin 编译过程（统一编译）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. 编译器扫描所有源文件" << endl;
    cout << "   Calculator.java" << endl;
    cout << "   Main.java" << endl;
    cout << "\n2. 建立依赖关系图" << endl;
    cout << "   编译器知道所有类的关系" << endl;
    cout << "\n3. 编译成字节码" << endl;
    cout << "   Calculator.class" << endl;
    cout << "   Main.class" << endl;
    
    cout << "\n优点：" << endl;
    cout << "  ✅ 不需要头文件" << endl;
    cout << "  ✅ 自动处理依赖" << endl;
    cout << "  ✅ 编译错误更早发现" << endl;
    
    cout << "\n缺点：" << endl;
    cout << "  ❌ 大项目编译可能较慢" << endl;
    cout << "  ❌ 增量编译不如 C++ 灵活" << endl;
}

// ============================================
// 7. 实际案例演示
// ============================================

// 案例1: 必须前置声明的情况
class Node;  // 前置声明

class Tree {
private:
    Node* root;  // ✅ 只使用指针，可以前置声明
public:
    void insert(int value);
};

class Node {
private:
    int data;
    Node* left;
    Node* right;
public:
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

void Tree::insert(int value) {
    // 实现...
}

void practical_examples() {
    cout << "\n" << string(60, '=') << endl;
    cout << "实际案例" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n案例1: 类的相互引用" << endl;
    cout << "---------------------------------------" << endl;
    cout << "class Node;  // 前置声明" << endl;
    cout << "\nclass Tree {" << endl;
    cout << "    Node* root;  // 使用指针" << endl;
    cout << "};" << endl;
    cout << "\nclass Node { ... };" << endl;
    cout << "\n→ 必须前置声明才能编译" << endl;
    
    cout << "\n案例2: 大型项目结构" << endl;
    cout << "---------------------------------------" << endl;
    cout << "project/" << endl;
    cout << "├── include/" << endl;
    cout << "│   ├── calculator.h     (声明)" << endl;
    cout << "│   └── math_utils.h     (声明)" << endl;
    cout << "├── src/" << endl;
    cout << "│   ├── calculator.cpp   (实现)" << endl;
    cout << "│   └── math_utils.cpp   (实现)" << endl;
    cout << "└── main.cpp" << endl;
    cout << "\n→ 清晰的接口和实现分离" << endl;
    
    cout << "\n案例3: 库的分发" << endl;
    cout << "---------------------------------------" << endl;
    cout << "分发给用户：" << endl;
    cout << "  ✅ mylib.h       (头文件 - 接口)" << endl;
    cout << "  ✅ mylib.lib/.a  (静态库 - 编译后的代码)" << endl;
    cout << "  ❌ mylib.cpp     (不分发源代码)" << endl;
    cout << "\n→ 保护商业代码，只暴露接口" << endl;
}

// ============================================
// 8. 最佳实践
// ============================================

void best_practices() {
    cout << "\n" << string(60, '=') << endl;
    cout << "最佳实践" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n1. 头文件中放什么？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ 函数声明" << endl;
    cout << "✅ 类定义" << endl;
    cout << "✅ 常量声明（extern const 或 constexpr）" << endl;
    cout << "✅ inline 函数定义" << endl;
    cout << "✅ 模板定义" << endl;
    cout << "❌ 全局变量定义（会导致重复定义）" << endl;
    cout << "❌ 函数实现（除非 inline/constexpr/template）" << endl;
    
    cout << "\n2. 源文件中放什么？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ 函数定义（实现）" << endl;
    cout << "✅ 全局变量定义" << endl;
    cout << "✅ 静态变量定义" << endl;
    
    cout << "\n3. 头文件保护" << endl;
    cout << "---------------------------------------" << endl;
    cout << "// 方法1: Include Guard" << endl;
    cout << "#ifndef MYHEADER_H" << endl;
    cout << "#define MYHEADER_H" << endl;
    cout << "// 内容" << endl;
    cout << "#endif" << endl;
    cout << "\n// 方法2: #pragma once (现代推荐)" << endl;
    cout << "#pragma once" << endl;
    cout << "// 内容" << endl;
    
    cout << "\n4. 前置声明 vs 包含头文件" << endl;
    cout << "---------------------------------------" << endl;
    cout << "优先使用前置声明（如果可以）：" << endl;
    cout << "  class MyClass;  // 前置声明" << endl;
    cout << "  void func(MyClass* ptr);  // 只使用指针/引用" << endl;
    cout << "\n只在需要完整定义时包含头文件：" << endl;
    cout << "  #include \"MyClass.h\"  // 需要访问成员" << endl;
    cout << "  MyClass obj;  // 需要完整定义" << endl;
}

// ============================================
// 9. 总结对比表
// ============================================

void summary_table() {
    cout << "\n" << string(60, '=') << endl;
    cout << "总结对比表" << endl;
    cout << string(60, '=') << endl;
    
    cout << R"(
┌─────────────────┬──────────────┬──────────────┬──────────────┐
│  特性           │  C++         │  Java        │  Kotlin      │
├─────────────────┼──────────────┼──────────────┼──────────────┤
│ 声明/定义       │ 可以分离     │ 必须一起     │ 必须一起     │
├─────────────────┼──────────────┼──────────────┼──────────────┤
│ 头文件          │ ✅ 需要      │ ❌ 不需要    │ ❌ 不需要    │
├─────────────────┼──────────────┼──────────────┼──────────────┤
│ 前置声明        │ ✅ 需要      │ ❌ 不需要    │ ❌ 不需要    │
├─────────────────┼──────────────┼──────────────┼──────────────┤
│ 编译模型        │ 分离编译     │ 统一编译     │ 统一编译     │
│                 │ (每个.cpp)   │ (所有.java)  │ (所有.kt)    │
├─────────────────┼──────────────┼──────────────┼──────────────┤
│ 编译速度        │ ✅ 快        │ ⚠️  中等     │ ⚠️  中等     │
│ (大项目)        │ (增量编译)   │              │              │
├─────────────────┼──────────────┼──────────────┼──────────────┤
│ 循环依赖        │ 前置声明解决 │ 自动处理     │ 自动处理     │
├─────────────────┼──────────────┼──────────────┼──────────────┤
│ 代码保护        │ ✅ 易         │ ⚠️  需混淆   │ ⚠️  需混淆   │
│                 │ (分发.h+.lib)│              │              │
├─────────────────┼──────────────┼──────────────┼──────────────┤
│ 学习曲线        │ ⚠️  陡        │ ✅ 平缓      │ ✅ 平缓      │
└─────────────────┴──────────────┴──────────────┴──────────────┘
)" << endl;
}

// ============================================
// Main
// ============================================

int main() {
    cout << "===========================================" << endl;
    cout << "  C++ 函数声明与实现分离 - 深度解析" << endl;
    cout << "===========================================" << endl;
    
    explain_header_source_separation();
    explain_when_not_separate();
    compare_with_java_kotlin();
    compilation_process();
    practical_examples();
    best_practices();
    summary_table();
    
    cout << "\n===========================================" << endl;
    cout << "核心总结" << endl;
    cout << "===========================================" << endl;
    
    cout << "\n为什么 C++ 要分离声明和定义？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. 编译模型：分离编译，每个 .cpp 独立编译" << endl;
    cout << "2. 编译速度：修改实现不需要重新编译所有文件" << endl;
    cout << "3. 信息隐藏：只暴露接口（.h），隐藏实现" << endl;
    cout << "4. 循环依赖：通过前置声明解决" << endl;
    cout << "5. 代码保护：可以只分发头文件和库文件" << endl;
    
    cout << "\n与 Java/Kotlin 的区别：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "Java/Kotlin: 统一编译，编译器看到所有代码" << endl;
    cout << "  → 不需要头文件" << endl;
    cout << "  → 自动处理依赖" << endl;
    cout << "  → 声明即定义" << endl;
    cout << "\nC++: 分离编译，每个文件独立" << endl;
    cout << "  → 需要头文件声明" << endl;
    cout << "  → 手动管理依赖" << endl;
    cout << "  → 声明和定义可分离" << endl;
    
    cout << "\n什么时候可以不分离？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. inline 函数" << endl;
    cout << "2. constexpr 函数" << endl;
    cout << "3. 模板函数/类" << endl;
    cout << "4. 小型项目（单文件）" << endl;
    cout << "5. 类内定义的成员函数（自动 inline）" << endl;
    
    cout << "\n记忆口诀：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "C++ 分离为编译，Java 统一最便利" << endl;
    cout << "头文件声明是接口，源文件实现藏细节" << endl;
    cout << "inline、constexpr、模板，必须定义在头文" << endl;
    cout << "大项目用分离，小项目可合一" << endl;
    cout << "===========================================" << endl;
    
    return 0;
}

/**
 * 快速决策：什么时候分离？
 * 
 * 分离（.h + .cpp）：
 *   ✅ 大型项目
 *   ✅ 需要信息隐藏
 *   ✅ 需要编译优化
 *   ✅ 普通函数
 *   ✅ 类的成员函数（在类外定义）
 * 
 * 不分离（全在 .h 或 .cpp）：
 *   ✅ inline 函数
 *   ✅ constexpr 函数
 *   ✅ 模板函数/类
 *   ✅ 小型项目
 *   ✅ 单文件程序
 */
