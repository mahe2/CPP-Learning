/*
 * 输入输出操作
 * 
 * cin  - 标准输入（从键盘读取）
 * cout - 标准输出（输出到控制台）
 */

#include <iostream>
#include <string>
#include <iomanip>  // 格式化输出

using namespace std;

int main() {
    // ========== 基本输出 ==========
    cout << "Hello, World!" << endl;
    
    // 链式输出（类似 Java 的字符串拼接）
    int age = 25;
    string name = "李四";
    cout << "姓名: " << name << ", 年龄: " << age << endl;
    
    // ========== 基本输入 ==========
    cout << "\n请输入您的姓名: ";
    string userName;
    cin >> userName;  // 读取一个单词（遇到空格停止）
    
    cout << "请输入您的年龄: ";
    int userAge;
    cin >> userAge;
    
    cout << "您好, " << userName << "! 您今年 " << userAge << " 岁。\n";
    
    // ========== 读取整行（包含空格）==========
    cin.ignore();  // 清除缓冲区中的换行符
    
    cout << "\n请输入您的完整地址: ";
    string address;
    getline(cin, address);  // 读取整行，类似 Java Scanner.nextLine()
    cout << "地址: " << address << endl;
    
    // ========== 格式化输出 ==========
    double price = 99.5;
    cout << "\n=== 格式化输出 ===" << endl;
    cout << "原始价格: " << price << endl;
    cout << "保留2位小数: " << fixed << setprecision(2) << price << endl;
    cout << "设置宽度为10: " << setw(10) << price << endl;
    
    // 输出布尔值
    bool isActive = true;
    cout << "布尔值(默认): " << isActive << endl;           // 输出 1
    cout << "布尔值(文本): " << boolalpha << isActive << endl;  // 输出 true
    
    return 0;
}

/*
 * 对比 Java:
 * 
 * Scanner scanner = new Scanner(System.in);
 * String name = scanner.next();      // 相当于 cin >> name
 * String line = scanner.nextLine();  // 相当于 getline(cin, line)
 * int age = scanner.nextInt();       // 相当于 cin >> age
 * 
 * System.out.println("Hello");       // 相当于 cout << "Hello" << endl
 * System.out.print("No newline");    // 相当于 cout << "No newline"
 */
