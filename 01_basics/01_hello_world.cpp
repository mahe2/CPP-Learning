/*
 * Hello World - C++入门第一个程序
 * 
 * 编译运行：
 * g++ -std=c++17 01_hello_world.cpp -o hello_world
 * ./hello_world
 */

#include <iostream>  // 输入输出流库，类似 Java 的 System.out

using namespace std;  // 使用标准命名空间，避免写 std::cout

int main() {
    // cout 相当于 Java 的 System.out.println
    cout << "Hello C++!" << endl;

    cout << "欢迎来到C++的世界" << endl;
    
    return 0;  // 返回0表示程序正常退出
}

/*
 * 对比 Java:
 * public class HelloWorld {
 *     public static void main(String[] args) {
 *         System.out.println("Hello, Java!");
 *     }
 * }
 * 
 * 对比 Kotlin:
 * fun main() {
 *     println("Hello, Kotlin!")
 * }
 */
