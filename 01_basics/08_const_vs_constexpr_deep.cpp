/**
 * const（运行时常量）vs constexpr（编译时常量）深度解析
 * 
 * 核心问题：
 * 1. 常量和编译时常量有什么区别？
 * 2. 使用场景是什么？
 * 3. 为什么需要编译时常量？
 */

#include <iostream>
#include <array>
#include <chrono>

using namespace std;
using namespace chrono;

// ============================================
// 1. 基本区别演示
// ============================================
void basic_difference() {
    cout << "\n" << string(60, '=') << endl;
    cout << "1. 基本区别" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\nconst（运行时常量）：" << endl;
    cout << "---------------------------------------" << endl;
    int input = 100;
    const int a = input;  // ✅ 可以用运行时值初始化
    const int b = 10 + 20;  // ✅ 可以用表达式
    const int c = time(nullptr);  // ✅ 可以用运行时函数
    
    cout << "const int a = input;        // " << a << " (运行时确定)" << endl;
    cout << "const int b = 10 + 20;      // " << b << " (编译时可能优化)" << endl;
    cout << "const int c = time(nullptr);// " << c << " (运行时确定)" << endl;
    
    cout << "\nconstexpr（编译时常量）：" << endl;
    cout << "---------------------------------------" << endl;
    constexpr int x = 100;  // ✅ 字面量
    constexpr int y = 10 + 20;  // ✅ 常量表达式
    constexpr int z = x * y;  // ✅ 使用其他 constexpr
    // constexpr int error = input;  // ❌ 编译错误！
    // constexpr int error2 = time(nullptr);  // ❌ 编译错误！
    
    cout << "constexpr int x = 100;      // " << x << " (编译时确定)" << endl;
    cout << "constexpr int y = 10 + 20;  // " << y << " (编译时计算)" << endl;
    cout << "constexpr int z = x * y;    // " << z << " (编译时计算)" << endl;
    cout << "// constexpr int error = input;  ❌ 编译错误！" << endl;
}

// ============================================
// 2. 关键区别：能否用于编译时需求
// ============================================
void compile_time_requirements() {
    cout << "\n" << string(60, '=') << endl;
    cout << "2. 编译时需求（const vs constexpr）" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n需求1: 数组大小（必须编译时确定）" << endl;
    cout << "---------------------------------------" << endl;
    
    constexpr int SIZE1 = 10;
    int arr1[SIZE1];  // ✅ constexpr 可以
    cout << "int arr1[SIZE1];  // SIZE1 是 constexpr，✅ 成功" << endl;
    
    const int SIZE2 = 10;  // 字面量初始化
    int arr2[SIZE2];  // ✅ 通常可以（编译器识别为常量）
    cout << "int arr2[SIZE2];  // SIZE2 是 const（字面量），✅ 成功" << endl;
    
    int runtime_val = 10;
    const int SIZE3 = runtime_val;  // 运行时初始化
    // int arr3[SIZE3];  // ❌ 编译错误！SIZE3 不是编译时常量
    cout << "// int arr3[SIZE3]; // SIZE3 是运行时 const，❌ 失败" << endl;
    
    cout << "\n需求2: 模板参数（必须编译时确定）" << endl;
    cout << "---------------------------------------" << endl;
    
    constexpr int N1 = 5;
    array<int, N1> arr_a;  // ✅ constexpr
    cout << "array<int, N1> arr_a;  // N1 是 constexpr，✅ 成功" << endl;
    
    const int N2 = 5;  // 字面量
    array<int, N2> arr_b;  // ✅ 通常可以
    cout << "array<int, N2> arr_b;  // N2 是 const（字面量），✅ 成功" << endl;
    
    const int N3 = runtime_val;
    // array<int, N3> arr_c;  // ❌ 编译错误！
    cout << "// array<int, N3> arr_c; // N3 是运行时 const，❌ 失败" << endl;
    
    cout << "\n需求3: switch-case 标签（必须编译时确定）" << endl;
    cout << "---------------------------------------" << endl;
    
    constexpr int CASE_A = 1;
    const int CASE_B = 2;  // 字面量
    const int CASE_C = runtime_val;  // 运行时
    
    int choice = 1;
    switch (choice) {
        case CASE_A:  // ✅ constexpr
            cout << "case CASE_A: ✅" << endl;
            break;
        case CASE_B:  // ✅ const（字面量）
            cout << "case CASE_B: ✅" << endl;
            break;
        // case CASE_C:  // ❌ 运行时 const，编译错误
        //     break;
    }
}

// ============================================
// 3. constexpr 函数
// ============================================
constexpr int fibonacci(int n) {
    return (n <= 1) ? n : fibonacci(n-1) + fibonacci(n-2);
}

int fibonacci_runtime(int n) {
    return (n <= 1) ? n : fibonacci_runtime(n-1) + fibonacci_runtime(n-2);
}

void constexpr_functions() {
    cout << "\n" << string(60, '=') << endl;
    cout << "3. constexpr 函数（编译时计算）" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\nconstexpr 函数的特点：" << endl;
    cout << "---------------------------------------" << endl;
    
    // 编译时计算
    constexpr int fib10 = fibonacci(10);
    cout << "constexpr int fib10 = fibonacci(10);" << endl;
    cout << "  结果: " << fib10 << " (编译时已计算，零运行时开销)" << endl;
    
    // 可用于数组大小
    int arr[fibonacci(5)];  // ✅ 编译时计算
    cout << "\nint arr[fibonacci(5)];  // 数组大小: " << fibonacci(5) << endl;
    
    // 也可以运行时调用
    int n = 10;
    int result = fibonacci(n);  // 运行时计算
    cout << "\nint result = fibonacci(n);  // 运行时计算: " << result << endl;
    
    cout << "\n普通函数的对比：" << endl;
    cout << "---------------------------------------" << endl;
    int fib10_rt = fibonacci_runtime(10);
    cout << "int fib10_rt = fibonacci_runtime(10);" << endl;
    cout << "  结果: " << fib10_rt << " (运行时计算，有开销)" << endl;
    
    // int arr2[fibonacci_runtime(5)];  // ❌ 编译错误！
    cout << "// int arr2[fibonacci_runtime(5)]; ❌ 不能用作数组大小" << endl;
}

// ============================================
// 4. 性能对比
// ============================================
constexpr long long compute_factorial(int n) {
    long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

long long compute_factorial_runtime(int n) {
    long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

void performance_comparison() {
    cout << "\n" << string(60, '=') << endl;
    cout << "4. 性能对比" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n编译时计算 (constexpr)：" << endl;
    cout << "---------------------------------------" << endl;
    constexpr long long fact20 = compute_factorial(20);
    cout << "constexpr long long fact20 = compute_factorial(20);" << endl;
    cout << "  20! = " << fact20 << endl;
    cout << "  ✅ 编译时已计算完成，运行时直接使用" << endl;
    cout << "  ✅ 零运行时开销" << endl;
    cout << "  ✅ 相当于直接写: const long long fact20 = 2432902008176640000LL;" << endl;
    
    cout << "\n运行时计算 (普通函数)：" << endl;
    cout << "---------------------------------------" << endl;
    auto start = high_resolution_clock::now();
    long long fact20_rt = compute_factorial_runtime(20);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(end - start);
    
    cout << "long long fact20_rt = compute_factorial_runtime(20);" << endl;
    cout << "  20! = " << fact20_rt << endl;
    cout << "  ⚠️  运行时计算" << endl;
    cout << "  ⚠️  耗时: " << duration.count() << " 纳秒" << endl;
    cout << "  ⚠️  每次调用都要重新计算" << endl;
    
    cout << "\n结论：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "constexpr 把运行时的工作转移到编译时！" << endl;
    cout << "程序体积可能稍大，但运行速度更快！" << endl;
}

// ============================================
// 5. 使用场景总结
// ============================================
void usage_scenarios() {
    cout << "\n" << string(60, '=') << endl;
    cout << "5. 使用场景" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n何时使用 const？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ 保护变量不被修改" << endl;
    cout << "   const int MAX_VALUE = getValue();" << endl;
    cout << "\n✅ 函数参数（避免复制和修改）" << endl;
    cout << "   void func(const string& str);" << endl;
    cout << "\n✅ const 成员函数（不修改对象状态）" << endl;
    cout << "   int getValue() const;" << endl;
    cout << "\n✅ 指针和引用的常量性" << endl;
    cout << "   const int* ptr;  // 常量指针" << endl;
    cout << "   int* const ptr;  // 指针常量" << endl;
    
    cout << "\n何时使用 constexpr？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ 数组大小" << endl;
    cout << "   constexpr int SIZE = 100;" << endl;
    cout << "   int arr[SIZE];" << endl;
    cout << "\n✅ 模板参数" << endl;
    cout << "   constexpr int N = 10;" << endl;
    cout << "   array<int, N> arr;" << endl;
    cout << "\n✅ 编译时计算（性能优化）" << endl;
    cout << "   constexpr int result = heavyComputation();" << endl;
    cout << "\n✅ 常量表达式" << endl;
    cout << "   constexpr double PI = 3.14159;" << endl;
    cout << "   constexpr int MAX = 1000;" << endl;
    cout << "\n✅ switch-case 标签" << endl;
    cout << "   constexpr int CASE_A = 1;" << endl;
    cout << "   case CASE_A: ..." << endl;
}

// ============================================
// 6. 为什么需要编译时常量？
// ============================================
void why_compile_time_constants() {
    cout << "\n" << string(60, '=') << endl;
    cout << "6. 为什么需要编译时常量？" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n理由1: 类型系统要求" << endl;
    cout << "---------------------------------------" << endl;
    cout << "C++ 的类型系统需要在编译时确定很多信息：" << endl;
    cout << "  - 数组大小: int arr[N]" << endl;
    cout << "  - 模板参数: array<int, N>" << endl;
    cout << "  - 枚举值: enum { VALUE = N }" << endl;
    cout << "  - case 标签: case N: ..." << endl;
    cout << "\n这些地方运行时常量不行，必须编译时确定！" << endl;
    
    cout << "\n理由2: 性能优化" << endl;
    cout << "---------------------------------------" << endl;
    cout << "编译时计算 = 零运行时开销" << endl;
    cout << "例如：constexpr int x = 2 + 3 * 4;" << endl;
    cout << "  编译后相当于: const int x = 14;" << endl;
    cout << "  直接使用 14，不需要运行时计算！" << endl;
    
    cout << "\n理由3: 内存优化" << endl;
    cout << "---------------------------------------" << endl;
    cout << "constexpr 值可以直接内联到代码中" << endl;
    cout << "不需要在内存中分配存储空间" << endl;
    cout << "（编译器优化）" << endl;
    
    cout << "\n理由4: 编译时错误检查" << endl;
    cout << "---------------------------------------" << endl;
    cout << "constexpr 函数会在编译时检查" << endl;
    cout << "如果有错误，编译时就能发现" << endl;
    cout << "而不是运行时才崩溃！" << endl;
    
    cout << "\n理由5: 常量折叠（Constant Folding）" << endl;
    cout << "---------------------------------------" << endl;
    cout << "编译器可以优化 constexpr 表达式：" << endl;
    cout << "  constexpr int a = 10;" << endl;
    cout << "  constexpr int b = 20;" << endl;
    cout << "  constexpr int c = a + b * 2;" << endl;
    cout << "  编译后 c 直接是 50，不需要计算！" << endl;
}

// ============================================
// 7. 实际案例
// ============================================
void practical_examples() {
    cout << "\n" << string(60, '=') << endl;
    cout << "7. 实际案例" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n案例1: 配置常量" << endl;
    cout << "---------------------------------------" << endl;
    constexpr int MAX_CONNECTIONS = 100;
    constexpr int BUFFER_SIZE = 4096;
    constexpr double TIMEOUT_SECONDS = 30.0;
    
    cout << "constexpr int MAX_CONNECTIONS = 100;" << endl;
    cout << "constexpr int BUFFER_SIZE = 4096;" << endl;
    cout << "→ 用于数组大小、模板参数等" << endl;
    
    cout << "\n案例2: 数学常量" << endl;
    cout << "---------------------------------------" << endl;
    constexpr double PI = 3.141592653589793;
    constexpr double E = 2.718281828459045;
    constexpr double GOLDEN_RATIO = 1.618033988749895;
    
    cout << "constexpr double PI = 3.141592653589793;" << endl;
    cout << "→ 编译时就确定，高精度，零开销" << endl;
    
    cout << "\n案例3: 编译时计算" << endl;
    cout << "---------------------------------------" << endl;
    constexpr int DAYS_IN_YEAR = 365;
    constexpr int HOURS_IN_DAY = 24;
    constexpr int MINUTES_IN_HOUR = 60;
    constexpr int SECONDS_IN_MINUTE = 60;
    constexpr int SECONDS_IN_YEAR = DAYS_IN_YEAR * HOURS_IN_DAY * 
                                     MINUTES_IN_HOUR * SECONDS_IN_MINUTE;
    
    cout << "constexpr int SECONDS_IN_YEAR = 365 * 24 * 60 * 60;" << endl;
    cout << "  结果: " << SECONDS_IN_YEAR << " (编译时计算)" << endl;
    
    cout << "\n案例4: 类型安全的枚举" << endl;
    cout << "---------------------------------------" << endl;
    constexpr int STATUS_OK = 0;
    constexpr int STATUS_ERROR = 1;
    constexpr int STATUS_PENDING = 2;
    
    cout << "constexpr int STATUS_OK = 0;" << endl;
    cout << "→ 可以用在 switch-case 中" << endl;
}

// ============================================
// 8. 对比表格
// ============================================
void comparison_table() {
    cout << "\n" << string(60, '=') << endl;
    cout << "8. 完整对比表" << endl;
    cout << string(60, '=') << endl;
    
    cout << R"(
┌──────────────────┬─────────────────┬─────────────────┐
│  特性             │  const          │  constexpr      │
├──────────────────┼─────────────────┼─────────────────┤
│ 求值时机          │ 运行时/编译时   │ 编译时（强制）  │
├──────────────────┼─────────────────┼─────────────────┤
│ 初始化方式        │ 任何表达式      │ 常量表达式      │
│                  │ const int a=f();│ constexpr int b │
├──────────────────┼─────────────────┼─────────────────┤
│ 数组大小          │ 不一定可以      │ ✅ 可以         │
│                  │ (看编译器)      │                 │
├──────────────────┼─────────────────┼─────────────────┤
│ 模板参数          │ 不一定可以      │ ✅ 可以         │
├──────────────────┼─────────────────┼─────────────────┤
│ switch-case 标签  │ 不一定可以      │ ✅ 可以         │
├──────────────────┼─────────────────┼─────────────────┤
│ 性能              │ 可能有开销      │ 零运行时开销    │
├──────────────────┼─────────────────┼─────────────────┤
│ 函数              │ 运行时函数      │ 编译时可求值    │
│                  │ const返回值     │ constexpr 函数  │
├──────────────────┼─────────────────┼─────────────────┤
│ 指针/引用         │ ✅ 常用         │ ⚠️  较少用      │
│                  │ const T* ptr    │ constexpr T* ptr│
├──────────────────┼─────────────────┼─────────────────┤
│ 类成员            │ ✅ 常用         │ ✅ 常用         │
│                  │ const成员函数   │ static constexpr│
└──────────────────┴─────────────────┴─────────────────┘
)" << endl;
}

// ============================================
// 9. Java/Kotlin 对比
// ============================================
void language_comparison() {
    cout << "\n" << string(60, '=') << endl;
    cout << "9. 与 Java/Kotlin 对比" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\nJava：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "final int x = 10;              // 类似 const" << endl;
    cout << "  → 运行时不可变" << endl;
    cout << "  → 可以用运行时值初始化" << endl;
    cout << "\nstatic final int Y = 20;       // 类似 constexpr" << endl;
    cout << "  → 但没有强制编译时求值" << endl;
    cout << "  → 主要用于常量定义" << endl;
    
    cout << "\nKotlin：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "val x = getValue()             // 类似 const" << endl;
    cout << "  → 运行时不可变" << endl;
    cout << "  → 可以是运行时值" << endl;
    cout << "\nconst val Y = 20               // 类似 constexpr" << endl;
    cout << "  → 编译时常量" << endl;
    cout << "  → 必须是基本类型或 String" << endl;
    cout << "  → 必须在顶层或 object 中声明" << endl;
    
    cout << "\nC++：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "const int x = getValue();      // 运行时常量" << endl;
    cout << "constexpr int y = 20;          // 编译时常量" << endl;
    cout << "  → 更精确的控制" << endl;
    cout << "  → constexpr 函数可编译时求值" << endl;
}

int main() {
    cout << "===========================================" << endl;
    cout << "  const vs constexpr 深度解析" << endl;
    cout << "===========================================" << endl;
    
    basic_difference();
    compile_time_requirements();
    constexpr_functions();
    performance_comparison();
    usage_scenarios();
    why_compile_time_constants();
    practical_examples();
    comparison_table();
    language_comparison();
    
    cout << "\n===========================================" << endl;
    cout << "总结" << endl;
    cout << "===========================================" << endl;
    cout << "\n核心答案：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. const = 运行时常量（值不可变）" << endl;
    cout << "   constexpr = 编译时常量（编译时确定）" << endl;
    cout << "\n2. 使用场景：" << endl;
    cout << "   const: 保护变量、函数参数、成员函数" << endl;
    cout << "   constexpr: 数组大小、模板参数、性能优化" << endl;
    cout << "\n3. 为什么需要编译时常量？" << endl;
    cout << "   - 类型系统要求（数组、模板）" << endl;
    cout << "   - 性能优化（零运行时开销）" << endl;
    cout << "   - 编译时错误检查" << endl;
    cout << "   - 常量折叠优化" << endl;
    cout << "\n记忆口诀：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "const 保安全，运行也能算" << endl;
    cout << "constexpr 真常量，编译就确定" << endl;
    cout << "数组模板要 constexpr" << endl;
    cout << "性能优化找 constexpr" << endl;
    cout << "===========================================" << endl;
    
    return 0;
}

/**
 * 快速决策树：
 * 
 * 需要在编译时确定值？
 *   ├─ 是 → 用 constexpr
 *   │   ├─ 数组大小
 *   │   ├─ 模板参数
 *   │   ├─ switch-case 标签
 *   │   └─ 性能关键的常量
 *   │
 *   └─ 否 → 用 const
 *       ├─ 保护变量不被修改
 *       ├─ 函数参数（const&）
 *       ├─ const 成员函数
 *       └─ 运行时初始化的常量
 */
