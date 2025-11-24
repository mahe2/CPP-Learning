/*
 * 函数基础
 * 
 * C++ 函数与 Java/Kotlin 方法类似
 */

#include <iostream>
#include <string>

using namespace std;

// ========== 函数声明（可选，类似 Java 的方法签名）==========
void greet();
int add(int a, int b);
double multiply(double x, double y);
void printInfo(string name, int age = 18);  // 默认参数

// ========== 主函数 ==========
int main() {
    cout << "=== 函数基础示例 ===" << endl;
    
    // 调用无返回值函数
    greet();
    
    // 调用有返回值函数
    int sum = add(10, 20);
    cout << "10 + 20 = " << sum << endl;
    
    double product = multiply(3.5, 2.0);
    cout << "3.5 × 2.0 = " << product << endl;
    
    // 使用默认参数
    printInfo("Alice");        // 使用默认年龄 18
    printInfo("Bob", 25);      // 指定年龄 25
    
    return 0;
}

// ========== 函数定义 ==========

// 无返回值函数（类似 Java void 方法）
void greet() {
    cout << "Hello, C++!" << endl;
}

// 有返回值函数
int add(int a, int b) {
    return a + b;
}

double multiply(double x, double y) {
    return x * y;
}

// 带默认参数的函数（类似 Kotlin 的默认参数）
void printInfo(string name, int age) {
    cout << "姓名: " << name << ", 年龄: " << age << endl;
}

/*
 * 对比 Java:
 * 
 * public static void greet() {
 *     System.out.println("Hello!");
 * }
 * 
 * public static int add(int a, int b) {
 *     return a + b;
 * }
 * 
 * 对比 Kotlin:
 * 
 * fun greet() {
 *     println("Hello!")
 * }
 * 
 * fun add(a: Int, b: Int): Int {
 *     return a + b
 * }
 * 
 * fun printInfo(name: String, age: Int = 18) {  // 默认参数
 *     println("姓名: $name, 年龄: $age")
 * }
 */
