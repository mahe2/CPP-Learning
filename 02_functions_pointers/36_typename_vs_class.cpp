/*
 * template<typename T> vs template<class T>
 * 回答: 两种写法含义完全一样，为什么会有两种写法？
 * 核心: 历史原因 + 语义清晰度
 */

#include <iostream>
#include <string>
using namespace std;

// ============================================================================
// 一、两种写法完全等价
// ============================================================================

// 写法1: 使用 class
template<class T>
T add_with_class(T a, T b) {
    cout << "[使用 class] ";
    return a + b;
}

// 写法2: 使用 typename
template<typename T>
T add_with_typename(T a, T b) {
    cout << "[使用 typename] ";
    return a + b;
}

void demonstrate_equivalence() {
    cout << "\n=== 1. 两种写法完全等价 ===" << endl;
    
    // 对于基本类型（int）- 两者都可以
    cout << "\n使用 int (基本类型):" << endl;
    cout << "add_with_class(5, 3) = " << add_with_class(5, 3) << endl;
    cout << "add_with_typename(5, 3) = " << add_with_typename(5, 3) << endl;
    
    // 对于 double - 两者都可以
    cout << "\n使用 double (基本类型):" << endl;
    cout << "add_with_class(1.5, 2.3) = " << add_with_class(1.5, 2.3) << endl;
    cout << "add_with_typename(1.5, 2.3) = " << add_with_typename(1.5, 2.3) << endl;
    
    // 对于 string (类类型) - 两者都可以
    cout << "\n使用 string (类类型):" << endl;
    string s1 = "Hello";
    string s2 = " World";
    cout << "add_with_class(s1, s2) = " << add_with_class(s1, s2) << endl;
    cout << "add_with_typename(s1, s2) = " << add_with_typename(s1, s2) << endl;
    
    cout << "\n结论: 功能完全相同，可以互换使用！" << endl;
}

// ============================================================================
// 二、历史原因
// ============================================================================

void explain_history() {
    cout << "\n=== 2. 为什么会有两种写法？历史原因 ===" << endl;
    
    cout << "\n时间线:" << endl;
    cout << "  1985年: C++ 最初设计时只有 template<class T>" << endl;
    cout << "  原因: 当时 C++ 主要关注面向对象，模板还是新特性" << endl;
    cout << "  问题: 'class' 这个词容易让人误解" << endl;
    
    cout << "\n误解示例:" << endl;
    cout << "  template<class T>  // 看起来 T 必须是一个 class？" << endl;
    cout << "  T add(T a, T b);   // 那基本类型 int、double 能用吗？" << endl;
    
    cout << "\n实际上 class 在这里的含义是\"类型\"(type)，不是\"类\"!" << endl;
    cout << "  template<class T>   // T 可以是 int (✅)" << endl;
    cout << "  template<class T>   // T 可以是 double (✅)" << endl;
    cout << "  template<class T>   // T 可以是 class MyClass (✅)" << endl;
    
    cout << "\n1998年: C++98 标准引入 typename 关键字" << endl;
    cout << "  目的: 提供更清晰的语义" << endl;
    cout << "  typename = \"类型名\"，更准确地表达 T 可以是任何类型" << endl;
    
    cout << "\n现在: 两种写法都保留，完全等价" << endl;
    cout << "  原因: 向后兼容，不破坏旧代码" << endl;
}

// ============================================================================
// 三、同时使用两种写法（可以混用）
// ============================================================================

// 多参数模板：可以混用 class 和 typename
template<class T1, typename T2, class T3>
void mixed_template(T1 a, T2 b, T3 c) {
    cout << "T1 = " << a << ", T2 = " << b << ", T3 = " << c << endl;
}

void demonstrate_mixing() {
    cout << "\n=== 3. 可以混用（但不推荐）===" << endl;
    
    cout << "\n定义: template<class T1, typename T2, class T3>" << endl;
    cout << "调用: mixed_template(42, 3.14, \"hello\")" << endl;
    mixed_template(42, 3.14, "hello");
    
    cout << "\n虽然可以混用，但建议统一使用一种风格" << endl;
}

// ============================================================================
// 四、typename 的另一个用途（这是 class 不能替代的！）
// ============================================================================

template<typename T>
class MyContainer {
public:
    // typename 的第二个用途：告诉编译器这是一个类型
    // 这种情况下 class 不能替代 typename!
    typename T::value_type getValue() {
        // T::value_type 是一个依赖于 T 的类型
        // 必须用 typename 告诉编译器这是类型，不是变量
        return typename T::value_type();
    }
};

class MyType {
public:
    using value_type = int;  // 嵌套类型定义
};

void demonstrate_typename_second_usage() {
    cout << "\n=== 4. typename 的第二个用途（class 不能替代）===" << endl;
    
    cout << "\n在模板内部访问嵌套类型时，必须用 typename:" << endl;
    cout << "  typename T::value_type getValue() { ... }" << endl;
    cout << "  ^^^^^^^^^^^^ 这里的 typename 不能用 class 替代" << endl;
    
    cout << "\n原因: 编译器需要明确知道 T::value_type 是类型还是变量" << endl;
    
    MyContainer<MyType> container;
    cout << "getValue() = " << container.getValue() << endl;
}

// ============================================================================
// 五、实际使用建议
// ============================================================================

void usage_recommendations() {
    cout << "\n=== 5. 实际使用建议 ===" << endl;
    
    cout << "\n推荐使用 typename 的理由:" << endl;
    cout << "  ✅ 语义更清晰：typename = \"类型名\"" << endl;
    cout << "  ✅ 避免误解：不会让人以为只能用 class" << endl;
    cout << "  ✅ 现代代码：C++11 之后的代码多用 typename" << endl;
    cout << "  ✅ 一致性：typename 还有其他用途，统一使用更好" << endl;
    
    cout << "\nclass 也可以用的场景:" << endl;
    cout << "  📌 旧代码：C++98 之前的代码多用 class" << endl;
    cout << "  📌 个人习惯：有些程序员习惯用 class（完全合法）" << endl;
    cout << "  📌 简短：class 比 typename 短 4 个字母" << endl;
    
    cout << "\n现代 C++ 推荐风格:" << endl;
    cout << "  template<typename T>  // ✅ 推荐" << endl;
    cout << "  template<class T>     // ✅ 也可以，但不如 typename 清晰" << endl;
    
    cout << "\n特殊情况（必须用 typename）:" << endl;
    cout << "  typename T::value_type   // ✅ 访问依赖类型，必须用 typename" << endl;
    cout << "  class T::value_type      // ❌ 语法错误" << endl;
}

// ============================================================================
// 六、实际代码示例对比
// ============================================================================

// STL 风格（多用 typename）
template<typename T>
class ModernVector {
    T* data;
    size_t size_;
public:
    ModernVector() : data(nullptr), size_(0) {}
    void push_back(const T& value) { /* ... */ }
};

// 旧风格（多用 class）
template<class T>
class LegacyVector {
    T* data;
    size_t size_;
public:
    LegacyVector() : data(nullptr), size_(0) {}
    void push_back(const T& value) { /* ... */ }
};

void code_style_comparison() {
    cout << "\n=== 6. 代码风格对比 ===" << endl;
    
    cout << "\n现代 STL 风格 (多用 typename):" << endl;
    cout << "  template<typename T>" << endl;
    cout << "  class ModernVector { ... };" << endl;
    
    cout << "\n旧代码风格 (多用 class):" << endl;
    cout << "  template<class T>" << endl;
    cout << "  class LegacyVector { ... };" << endl;
    
    ModernVector<int> v1;
    LegacyVector<int> v2;
    
    cout << "\n两者功能完全相同，只是风格不同" << endl;
}

// ============================================================================
// 七、常见误解澄清
// ============================================================================

void clarify_misconceptions() {
    cout << "\n=== 7. 常见误解澄清 ===" << endl;
    
    cout << "\n误解1: template<class T> 中的 T 必须是 class？" << endl;
    cout << "  ❌ 错误! T 可以是任何类型" << endl;
    cout << "  ✅ int、double、char 等基本类型都可以" << endl;
    
    cout << "\n误解2: typename 比 class 性能更好？" << endl;
    cout << "  ❌ 错误! 编译后完全一样" << endl;
    cout << "  ✅ 只是关键字不同，生成的代码相同" << endl;
    
    cout << "\n误解3: 新代码不能用 class？" << endl;
    cout << "  ❌ 错误! 仍然合法" << endl;
    cout << "  ✅ C++ 标准保证两者永远等价" << endl;
    
    cout << "\n误解4: typename 和 class 可以随意互换？" << endl;
    cout << "  ⚠️ 部分正确!" << endl;
    cout << "  ✅ 在模板参数位置可以互换" << endl;
    cout << "  ❌ 在访问依赖类型时不能互换（必须用 typename）" << endl;
}

// ============================================================================
// 八、核心总结
// ============================================================================

void summary() {
    cout << "\n========================================" << endl;
    cout << "           核心总结" << endl;
    cout << "========================================" << endl;
    
    cout << "\n问题: template<typename T> 与 template<class T> 含义一样吗？" << endl;
    cout << "答案: ✅ 完全一样！可以互换使用" << endl;
    
    cout << "\n为什么有两种写法？" << endl;
    cout << "  1️⃣ 历史原因:" << endl;
    cout << "     - class 是最早的写法（C++85）" << endl;
    cout << "     - typename 是后来加入的（C++98）" << endl;
    cout << "     - 为了向后兼容，两种都保留" << endl;
    
    cout << "\n  2️⃣ 语义清晰度:" << endl;
    cout << "     - class 容易误解为\"只能用类\"" << endl;
    cout << "     - typename 更清晰，表示\"任何类型\"" << endl;
    
    cout << "\n  3️⃣ typename 的额外用途:" << endl;
    cout << "     - 在模板参数: class 和 typename 等价" << endl;
    cout << "     - 访问依赖类型: 必须用 typename (class 不行)" << endl;
    cout << "       例如: typename T::value_type" << endl;
    
    cout << "\n实际建议:" << endl;
    cout << "  📌 新代码推荐: template<typename T>  (更清晰)" << endl;
    cout << "  📌 旧代码常见: template<class T>     (也正确)" << endl;
    cout << "  📌 团队统一: 选一种风格，保持一致" << endl;
    cout << "  📌 访问嵌套类型: 必须用 typename" << endl;
    
    cout << "\n记忆要点:" << endl;
    cout << "  ✅ 在模板参数位置，class = typename" << endl;
    cout << "  ✅ typename 语义更清晰（\"类型名\"）" << endl;
    cout << "  ✅ 编译后代码完全相同，无性能差异" << endl;
    cout << "  ✅ 现代 C++ 推荐用 typename" << endl;
    
    cout << "\n========================================" << endl;
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "\n╔═══════════════════════════════════════════════════════╗" << endl;
    cout << "║   template<typename T> vs template<class T>          ║" << endl;
    cout << "║   含义一样吗？为什么会有两种不同的写法？              ║" << endl;
    cout << "╚═══════════════════════════════════════════════════════╝" << endl;
    
    // 1. 等价性演示
    demonstrate_equivalence();
    
    // 2. 历史原因
    explain_history();
    
    // 3. 混用示例
    demonstrate_mixing();
    
    // 4. typename 的第二个用途
    demonstrate_typename_second_usage();
    
    // 5. 使用建议
    usage_recommendations();
    
    // 6. 代码风格对比
    code_style_comparison();
    
    // 7. 常见误解
    clarify_misconceptions();
    
    // 8. 总结
    summary();
    
    return 0;
}
