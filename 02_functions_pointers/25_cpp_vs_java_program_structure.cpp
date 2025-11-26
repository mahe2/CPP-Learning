/**
 * C++ vs Java 程序结构的本质差异
 * 
 * 核心理解（你的总结）：
 * 1. Java 程序：多对象配合，函数必须存在于对象中
 * 2. C++ 程序：多对象、多文件配合，变量、函数可以独立于对象，存在于文件中
 * 
 * 评价：完全正确！这是两种语言设计哲学的根本差异！
 */

#include <iostream>
#include <string>

using namespace std;

// ============================================
// C++ 独立函数和变量（Java 不允许这样）
// ============================================

// 全局变量 - 独立于任何类
int globalCounter = 0;
const string PROGRAM_NAME = "CPP-Learning";

// 独立函数 - 不属于任何类
void printMessage(const string& msg) {
    cout << msg << endl;
}

int add(int a, int b) {
    return a + b;
}

double calculateArea(double radius) {
    return 3.14159 * radius * radius;
}

// ============================================
// 命名空间 - C++ 的代码组织方式
// ============================================

namespace Utils {
    int version = 1;
    
    void log(const string& msg) {
        cout << "[LOG] " << msg << endl;
    }
}

namespace Math {
    const double PI = 3.14159;
    
    double square(double x) {
        return x * x;
    }
}

// ============================================
// 类（可选）- 类似 Java 的方式
// ============================================

class Calculator {
public:
    int multiply(int a, int b) { 
        return a * b; 
    }
};

// ============================================
// 演示函数
// ============================================

void explain_java_structure() {
    cout << "\n=== Java 程序结构 ===" << endl;
    cout << "\n必须的结构：" << endl;
    cout << "  Package (包)" << endl;
    cout << "    -> Class (类)" << endl;
    cout << "         -> Fields (成员变量)" << endl;
    cout << "         -> Methods (成员方法)" << endl;
    
    cout << "\n强制要求：" << endl;
    cout << "  - 所有代码必须在类中" << endl;
    cout << "  - 所有方法必须属于某个类" << endl;
    cout << "  - 没有独立的全局函数" << endl;
    cout << "  - 没有独立的全局变量" << endl;
    
    cout << "\nJava 示例：" << endl;
    cout << "  public class Calculator {" << endl;
    cout << "      // 必须在类中" << endl;
    cout << "      public static int add(int a, int b) {" << endl;
    cout << "          return a + b;" << endl;
    cout << "      }" << endl;
    cout << "  }" << endl;
}

void explain_cpp_structure() {
    cout << "\n=== C++ 程序结构 ===" << endl;
    cout << "\n灵活的结构：" << endl;
    cout << "  File (文件)" << endl;
    cout << "    -> 全局变量 (可选)" << endl;
    cout << "    -> 全局函数 (可选)" << endl;
    cout << "    -> 命名空间 (可选)" << endl;
    cout << "    -> 类 (可选)" << endl;
    
    cout << "\n灵活性：" << endl;
    cout << "  - 函数可以独立存在（全局函数）" << endl;
    cout << "  - 变量可以独立存在（全局变量）" << endl;
    cout << "  - 类是可选的（不是必须的）" << endl;
    cout << "  - 支持多种编程范式" << endl;
    
    cout << "\nC++ 示例：" << endl;
    cout << "  // 独立函数（不需要类！）" << endl;
    cout << "  int add(int a, int b) {" << endl;
    cout << "      return a + b;" << endl;
    cout << "  }" << endl;
}

void demonstrate_cpp_features() {
    cout << "\n=== C++ 独立函数和变量演示 ===" << endl;
    
    cout << "\n1. 访问全局变量：" << endl;
    cout << "   程序名: " << PROGRAM_NAME << endl;
    cout << "   计数器: " << globalCounter << endl;
    
    cout << "\n2. 调用独立函数：" << endl;
    printMessage("   Hello from independent function!");
    cout << "   10 + 20 = " << add(10, 20) << endl;
    cout << "   圆面积 (r=5): " << calculateArea(5.0) << endl;
    
    cout << "\n3. 使用命名空间：" << endl;
    Utils::log("   Using namespace function");
    cout << "   PI = " << Math::PI << endl;
    cout << "   5 的平方 = " << Math::square(5) << endl;
    
    cout << "\n4. 使用类（可选）：" << endl;
    Calculator calc;
    cout << "   10 * 5 = " << calc.multiply(10, 5) << endl;
    
    cout << "\n关键点：" << endl;
    cout << "  - 这些函数和变量完全独立" << endl;
    cout << "  - 不需要创建对象就能使用" << endl;
    cout << "  - Java/Kotlin 无法这样写！" << endl;
}

void compare_approaches() {
    cout << "\n=== 实现相同功能的对比 ===" << endl;
    
    cout << "\n任务：实现一个简单的加法" << endl;
    
    cout << "\nJava 方式（必须用类）：" << endl;
    cout << "  public class Calculator {" << endl;
    cout << "      public static int add(int a, int b) {" << endl;
    cout << "          return a + b;" << endl;
    cout << "      }" << endl;
    cout << "  }" << endl;
    cout << "  // 调用" << endl;
    cout << "  int sum = Calculator.add(10, 20);" << endl;
    
    cout << "\nC++ 方式1（独立函数）：" << endl;
    cout << "  int add(int a, int b) {" << endl;
    cout << "      return a + b;" << endl;
    cout << "  }" << endl;
    cout << "  // 调用" << endl;
    cout << "  int sum = add(10, 20);  // 更简洁！" << endl;
    
    cout << "\nC++ 方式2（类）：" << endl;
    cout << "  class Calculator {" << endl;
    cout << "  public:" << endl;
    cout << "      int add(int a, int b) { return a + b; }" << endl;
    cout << "  };" << endl;
    cout << "  // 调用" << endl;
    cout << "  Calculator calc;" << endl;
    cout << "  int sum = calc.add(10, 20);" << endl;
    
    cout << "\n结论：C++ 可以选择最合适的方式！" << endl;
}

void explain_design_philosophy() {
    cout << "\n=== 设计哲学差异 ===" << endl;
    
    cout << "\nJava：纯面向对象" << endl;
    cout << "  - 强制使用类" << endl;
    cout << "  - 一切皆对象（几乎）" << endl;
    cout << "  - 统一编程风格" << endl;
    cout << "  - 简化学习曲线" << endl;
    cout << "  - 适合大型企业应用" << endl;
    
    cout << "\nC++：多范式" << endl;
    cout << "  - 过程式编程（函数）" << endl;
    cout << "  - 面向对象编程（类）" << endl;
    cout << "  - 泛型编程（模板）" << endl;
    cout << "  - 函数式编程（lambda）" << endl;
    cout << "  - 灵活但需要更多决策" << endl;
    
    cout << "\n历史原因：" << endl;
    cout << "  Java: 从头设计，可以强制面向对象" << endl;
    cout << "  C++: 基于 C 演化，必须兼容 C" << endl;
}

void show_comparison_table() {
    cout << "\n=== 详细对比表 ===" << endl;
    cout << "\n特性              Java/Kotlin       C++" << endl;
    cout << "----------------------------------------------" << endl;
    cout << "程序组织单位      类(Class)         文件(File)" << endl;
    cout << "代码分组          包(Package)       命名空间(Namespace)" << endl;
    cout << "函数位置          必须在类中        类中或文件中" << endl;
    cout << "变量位置          必须在类中        类中或文件中" << endl;
    cout << "全局函数          不允许            允许" << endl;
    cout << "全局变量          不允许            允许" << endl;
    cout << "主入口            main在类中        main独立存在" << endl;
    cout << "必须创建对象      几乎总是          看情况" << endl;
    cout << "编程范式          纯面向对象        多范式" << endl;
    cout << "----------------------------------------------" << endl;
}

void summarize() {
    cout << "\n=== 核心总结 ===" << endl;
    
    cout << "\n【你的理解（完全正确！）】" << endl;
    cout << "----------------------------------------------" << endl;
    cout << "Java 程序：" << endl;
    cout << "  - 多对象配合" << endl;
    cout << "  - 函数必须存在于对象中" << endl;
    
    cout << "\nC++ 程序：" << endl;
    cout << "  - 多对象、多文件配合" << endl;
    cout << "  - 变量、函数可以独立于对象" << endl;
    cout << "  - 存在于文件中" << endl;
    
    cout << "\n【关键差异】" << endl;
    cout << "----------------------------------------------" << endl;
    cout << "1. 组织方式：" << endl;
    cout << "   Java:  包 -> 类 -> 方法" << endl;
    cout << "   C++:   文件 -> 函数/类/命名空间" << endl;
    
    cout << "\n2. 协作方式：" << endl;
    cout << "   Java:  对象.方法()  (必须通过对象)" << endl;
    cout << "   C++:   函数()  或  对象.方法()  (可选)" << endl;
    
    cout << "\n3. 设计哲学：" << endl;
    cout << "   Java:  纯面向对象，强制统一" << endl;
    cout << "   C++:   多范式，灵活选择" << endl;
    
    cout << "\n【记忆要点】" << endl;
    cout << "----------------------------------------------" << endl;
    cout << "Java: 一切皆对象，函数必须类中藏" << endl;
    cout << "C++: 灵活多范式，函数变量随意放" << endl;
    cout << "对象协作 Java 道，多文件配合 C++ 长" << endl;
    cout << "----------------------------------------------" << endl;
}

// ============================================
// Main
// ============================================

int main() {
    cout << "========================================" << endl;
    cout << "  C++ vs Java 程序结构的本质差异" << endl;
    cout << "========================================" << endl;
    
    explain_java_structure();
    explain_cpp_structure();
    demonstrate_cpp_features();
    compare_approaches();
    explain_design_philosophy();
    show_comparison_table();
    summarize();
    
    cout << "\n========================================" << endl;
    cout << "程序执行完成" << endl;
    cout << "========================================" << endl;
    
    return 0;
}

/**
 * 补充说明：
 * 
 * 1. 现代语言的趋势：
 *    - Kotlin: 支持顶层函数（编译后仍是静态方法）
 *    - Java 8+: Lambda 表达式（本质仍是对象）
 *    - C++: 保持多范式灵活性
 * 
 * 2. C++ 最佳实践：
 *    - 小工具函数：用独立函数
 *    - 需要状态：用类
 *    - 复杂逻辑：用类和继承
 *    - 命名空间避免命名冲突
 * 
 * 3. 从 Java 转 C++ 的建议：
 *    - 不要什么都用类
 *    - 学会使用独立函数
 *    - 理解命名空间
 *    - 拥抱多范式编程
 */
