/*
 * 变量和数据类型
 * 
 * C++ 是强类型语言，类似 Java 和 TypeScript
 */

#include <iostream>
#include <string>  // 使用 string 需要引入

using namespace std;

int main() {
    // ========== 基本数据类型 ==========
    
    // 整数类型
    int age = 37;                 // 32位整数，类似 Java int
    long long bigNumber = 123456789L; // 64位整数，类似 Java long
    short smallNum = 100;             // 16位整数
    
    // 浮点类型
    float price = 99.99f;             // 单精度，类似 Java float
    double pi = 3.14159;              // 双精度，类似 Java double
    
    // 字符和布尔
    char grade = 'A';                 // 单字符，类似 Java char
    bool isStudent = true;            // 布尔值，类似 Java boolean
    
    // 字符串（C++ 风格）
    string name = "张三";              // 类似 Java String
    string tencent = "腾讯";
    
    // ========== C++11 自动类型推导 ==========
    auto x = 42;           // 编译器自动推导为 int，类似 Kotlin 的 val x = 42
    auto y = 3.14;         // 推导为 double
    auto msg = "Hello";    // 推导为 const char*
    
    // ========== 常量 ==========
    const int MAX_SIZE = 100;         // 常量，类似 Java final 或 Kotlin val
    const int MIN_SIZE = 0;
    constexpr double E = 2.71828;     // 编译期常量
    constexpr int age = 20;
    
    // const vs constexpr 详细对比
    cout << "\n=== const vs constexpr ===" << endl;
    
    // 1. const - 运行时常量
    int input = 10;
    const int runtime_const = input * 2;  // ✅ 可以，运行时求值
    cout << "运行时常量: " << runtime_const << endl;
    
    // 2. constexpr - 编译期常量
    constexpr int compile_const = 20 * 2;  // ✅ 可以，编译时求值
    // constexpr int error = input * 2;    // ❌ 错误！input 不是编译期常量
    cout << "编译期常量: " << compile_const << endl;
    
    // 3. 使用场景差异
    // const 可用于运行时值
    const int arr_size1 = input;  // 运行时确定
    
    // constexpr 用于必须编译时确定的地方
    constexpr int arr_size2 = 5;
    int array1[arr_size2];  // ✅ 可以，编译期常量
    // int array2[arr_size1]; // ❌ 某些编译器不允许（C++标准要求编译期常量）
    
    // 4. constexpr 函数
    auto square = [](int n) constexpr { return n * n; };
    constexpr int result = square(5);  // 编译时计算
    cout << "5的平方（编译时计算）: " << result << endl;
    
    // ========== 输出变量 ==========
    cout << "姓名: " << name << endl;
    cout << "年龄: " << age << endl;
    cout << "是否学生: " << (isStudent ? "是" : "否") << endl;
    cout << "成绩: " << grade << endl;
    cout << "价格: " << price << endl;
    
    // 查看变量占用字节数
    cout << "\n=== 数据类型大小 ===" << endl;
    cout << "int: " << sizeof(int) << " 字节" << endl;
    cout << "long long: " << sizeof(long long) << " 字节" << endl;
    cout << "float: " << sizeof(float) << " 字节" << endl;
    cout << "double: " << sizeof(double) << " 字节" << endl;
    cout << "char: " << sizeof(char) << " 字节" << endl;
    cout << "bool: " << sizeof(bool) << " 字节" << endl;
    
    return 0;
}

/*
 * 对比 Java/Kotlin:
 * 
 * Java:
 * int age = 25;
 * String name = "张三";
 * final int MAX_SIZE = 100;
 * 
 * Kotlin:
 * val age: Int = 25        // 不可变
 * var name: String = "张三" // 可变
 * const val MAX_SIZE = 100
 * 
 * TypeScript:
 * let age: number = 25;
 * const name: string = "张三";
 * const MAX_SIZE: number = 100;
 * 
 * 
 * ========== const vs constexpr 详解 ==========
 * 
 * 1. 求值时机：
 *    const     - 可以是运行时常量（如 const int x = input;）
 *    constexpr - 必须是编译时常量（如 constexpr int x = 10;）
 * 
 * 2. 使用限制：
 *    const     - 值初始化后不能修改，但初始化可以用运行时值
 *    constexpr - 必须用常量表达式初始化，保证编译时求值
 * 
 * 3. 应用场景：
 *    const     - 保护变量不被修改（类似 Java final）
 *    constexpr - 编译时计算、数组大小、模板参数、性能优化
 * 
 * 4. 示例对比：
 *    // const 示例
 *    int x = 10;
 *    const int a = x;           // ✅ 运行时常量
 *    const int b = 20;          // ✅ 编译时常量（编译器优化）
 *    int arr1[a];               // ❌ 可能不允许（取决于编译器）
 * 
 *    // constexpr 示例
 *    constexpr int c = 20;      // ✅ 编译时常量
 *    // constexpr int d = x;    // ❌ 错误！x 不是常量表达式
 *    int arr2[c];               // ✅ 可以，c 是编译时常量
 * 
 * 5. 性能差异：
 *    constexpr 在编译时计算，零运行时开销
 *    const 如果是运行时常量，有轻微运行时开销
 * 
 * 6. Java/Kotlin 对比：
 *    Java:   final int x = 10;        // 类似 const
 *            (没有直接对应 constexpr 的特性)
 *    
 *    Kotlin: val x = 10               // 类似 const（运行时不可变）
 *            const val x = 10         // 类似 constexpr（编译时常量）
 * 
 * 7. 何时使用：
 *    - 需要编译时常量？用 constexpr（数组大小、模板参数）
 *    - 只需要保护变量不被修改？用 const
 *    - 不确定？默认用 const，需要时改为 constexpr
 */
