/*
 * const vs constexpr 深入对比
 * 
 * 详细展示两者的区别和使用场景
 */

#include <iostream>
#include <array>

using namespace std;

// ========== constexpr 函数 ==========
// constexpr 函数可以在编译时求值
constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

constexpr int square(int x) {
    return x * x;
}

// ========== 类中的常量 ==========
class Config {
public:
    static const int MAX_USERS = 100;           // ✅ 类内初始化
    static constexpr double PI = 3.14159;       // ✅ 编译时常量
    static constexpr int BUFFER_SIZE = 1024;    // ✅ 可用于数组大小
    
    void demo() const {
        cout << "  Config 演示 - PI: " << PI << ", 缓冲区: " << BUFFER_SIZE << endl;
    }
};

// ========== const 函数（普通函数）==========
int getRandomNumber() {
    return 42;  // 模拟运行时计算
}

int main() {
    cout << "=== const vs constexpr 深入对比 ===" << endl;
    
    // ========== 1. 初始化时机 ==========
    cout << "\n【1. 初始化时机】" << endl;
    
    // const 可以用运行时值初始化
    int runtime_value = 100;
    const int const_runtime = runtime_value;  // ✅ 运行时初始化
    cout << "const 运行时初始化: " << const_runtime << endl;
    
    // constexpr 必须用常量表达式初始化
    constexpr int constexpr_compile = 100;    // ✅ 编译时初始化
    // constexpr int error = runtime_value;   // ❌ 错误！
    cout << "constexpr 编译时初始化: " << constexpr_compile << endl;
    
    // ========== 2. 数组大小 ==========
    cout << "\n【2. 数组大小（必须编译时常量）】" << endl;
    
    constexpr int SIZE = 5;
    int array1[SIZE];  // ✅ constexpr 可以
    cout << "使用 constexpr 定义数组，大小: " << SIZE << endl;
    
    const int size1 = 10;  // 编译时常量（字面量初始化）
    int array2[size1];     // ✅ 通常可以（编译器优化）
    cout << "使用 const 定义数组（字面量），大小: " << size1 << endl;
    
    const int size2 = runtime_value;  // 运行时常量
    // int array3[size2];              // ❌ 不允许！size2 不是编译时常量
    cout << "const 运行时常量不能用于数组大小" << endl;
    
    // ========== 3. constexpr 函数 ==========
    cout << "\n【3. constexpr 函数（编译时计算）】" << endl;
    
    // 编译时计算阶乘
    constexpr int fact5 = factorial(5);  // 编译时计算
    cout << "5! = " << fact5 << " (编译时计算)" << endl;
    
    // 也可以运行时调用
    int n = 6;
    int fact6 = factorial(n);  // 运行时计算
    cout << "6! = " << fact6 << " (运行时计算)" << endl;
    
    // 编译时计算平方
    constexpr int sq = square(10);  // 编译时
    int array_sq[sq];  // ✅ 可以用作数组大小
    cout << "10² = " << sq << " (用作数组大小)" << endl;
    
    // ========== 4. std::array 使用 ==========
    cout << "\n【4. std::array（需要编译时大小）】" << endl;
    
    constexpr int ARRAY_SIZE = 3;
    array<int, ARRAY_SIZE> arr = {1, 2, 3};  // ✅ constexpr
    
    // const int SIZE_CONST = runtime_value;
    // array<int, SIZE_CONST> arr2;  // ❌ 错误！需要编译时常量
    
    cout << "std::array 大小: " << arr.size() << endl;
    
    // ========== 5. 性能对比 ==========
    cout << "\n【5. 性能对比】" << endl;
    
    // constexpr - 编译时计算，零运行时开销
    constexpr int result1 = factorial(10);
    cout << "constexpr 结果: " << result1 << " (编译时已计算)" << endl;
    
    // 普通函数 - 运行时计算
    int result2 = getRandomNumber();
    cout << "运行时结果: " << result2 << " (运行时计算)" << endl;
    
    // ========== 6. 指针和引用 ==========
    cout << "\n【6. 指针和引用】" << endl;
    
    int value = 42;
    
    // const 指针（指向的值不能改）
    const int* ptr1 = &value;
    cout << "const 指针指向的值: " << *ptr1 << endl;
    // *ptr1 = 100;  // ❌ 错误！不能修改
    ptr1 = nullptr;  // ✅ 可以修改指针本身
    
    // 指针常量（指针本身不能改）
    int* const ptr2 = &value;
    *ptr2 = 100;     // ✅ 可以修改值
    // ptr2 = nullptr;  // ❌ 错误！不能修改指针
    
    // 常量指针常量（都不能改）
    const int* const ptr3 = &value;
    // *ptr3 = 200;     // ❌ 错误！
    // ptr3 = nullptr;  // ❌ 错误！
    
    // ========== 7. 类成员 ==========
    cout << "\n【7. 类中的使用】" << endl;
    
    Config config;
    cout << "  最大用户数: " << Config::MAX_USERS << endl;
    cout << "  PI 值: " << Config::PI << endl;
    config.demo();
    
    // ========== 8. 实际应用建议 ==========
    cout << "\n【8. 何时使用】" << endl;
    cout << "✅ 使用 constexpr：" << endl;
    cout << "   - 数组大小、模板参数" << endl;
    cout << "   - 编译时计算（性能优化）" << endl;
    cout << "   - 常量表达式" << endl;
    cout << "\n✅ 使用 const：" << endl;
    cout << "   - 保护变量不被修改" << endl;
    cout << "   - 函数参数（const 引用）" << endl;
    cout << "   - 成员函数（不修改对象状态）" << endl;
    
    return 0;
}

/*
 * 总结：
 * 
 * ┌──────────────┬─────────────────┬─────────────────┐
 * │   特性       │      const      │    constexpr    │
 * ├──────────────┼─────────────────┼─────────────────┤
 * │ 求值时机     │ 运行时/编译时   │ 编译时          │
 * │ 初始化       │ 任何表达式      │ 常量表达式      │
 * │ 数组大小     │ 不一定可以      │ 可以            │
 * │ 模板参数     │ 不一定可以      │ 可以            │
 * │ 性能         │ 可能有开销      │ 零开销          │
 * │ 类成员       │ 需要类外定义*   │ 类内直接定义    │
 * │ 函数         │ 运行时函数      │ 编译时可求值    │
 * └──────────────┴─────────────────┴─────────────────┘
 * 
 * * C++17 后 const 静态成员也可以类内初始化
 * 
 * 
 * Java/Kotlin 对比：
 * 
 * === Java ===
 * final int x = 10;           // 类似 const
 * static final int Y = 20;    // 类似 constexpr（但不完全相同）
 * 
 * === Kotlin ===
 * val x = 10                  // 类似 const（运行时不可变）
 * const val Y = 20            // 类似 constexpr（编译时常量）
 * 
 * === C++ ===
 * const int x = 10;           // 类似 Java final 和 Kotlin val
 * constexpr int Y = 20;       // 类似 Kotlin const val
 * 
 * 
 * 口诀记忆：
 * - const：值不变，运行也行
 * - constexpr：编译时算，性能最强
 */
