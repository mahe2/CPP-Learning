/*
 * 指针基础 - C++ 最重要的概念之一
 * 
 * 指针是存储内存地址的变量
 * Java/Kotlin 没有显式指针，但引用类型本质上是指针
 */

#include <iostream>

using namespace std;

int main() {
    // ========== 基本变量 ==========
    int num = 42;
    
    cout << "=== 变量和指针 ===" << endl;
    cout << "num 的值: " << num << endl;
    cout << "num 的地址: " << &num << endl;  // & 取地址符
    
    // ========== 指针定义 ==========
    int* ptr = &num;  // ptr 是指向 int 的指针，存储 num 的地址
    
    cout << "\n=== 指针操作 ===" << endl;
    cout << "ptr 存储的地址: " << ptr << endl;
    cout << "ptr 指向的值: " << *ptr << endl;  // * 解引用符，获取指针指向的值
    
    // ========== 通过指针修改值 ==========
    *ptr = 100;  // 修改 ptr 指向的值
    cout << "\n修改后 num 的值: " << num << endl;  // num 被改为 100
    
    // ========== 空指针 ==========
    int* nullPtr = nullptr;  // C++11 推荐使用 nullptr（类似 Java null）
    
    if (nullPtr == nullptr) {
        cout << "\n空指针，不能解引用！" << endl;
    }
    
    // ========== 指针和数组 ==========
    cout << "\n=== 指针和数组 ===" << endl;
    int arr[] = {10, 20, 30, 40, 50};
    int* arrPtr = arr;  // 数组名本身就是指向第一个元素的指针
    
    cout << "第一个元素: " << *arrPtr << endl;       // 10
    cout << "第二个元素: " << *(arrPtr + 1) << endl; // 20
    cout << "第三个元素: " << arrPtr[2] << endl;     // 30（等价于 *(arrPtr + 2)）
    
    // 遍历数组
    cout << "数组元素: ";
    for (int i = 0; i < 5; i++) {
        cout << *(arrPtr + i) << " ";  // 指针算术
    }
    cout << endl;
    
    // ========== 指针算术 ==========
    cout << "\n=== 指针算术 ===" << endl;
    int* p = arr;
    cout << "p 指向: " << *p << endl;    // 10
    
    p++;  // 指针移动到下一个元素
    cout << "p++ 后指向: " << *p << endl;  // 20
    
    p += 2;  // 移动两个位置
    cout << "p += 2 后指向: " << *p << endl;  // 40
    
    return 0;
}

/*
 * 对比 Java/Kotlin:
 * 
 * Java 和 Kotlin 没有显式指针，但引用类型本质上是指针：
 * 
 * === Java ===
 * String str = "Hello";  // str 是一个引用（类似 C++ 指针）
 * 
 * int[] arr = {1, 2, 3};  // arr 是数组的引用
 * 
 * === C++ ===
 * string* str = new string("Hello");  // str 是指针
 * delete str;  // 需要手动释放内存
 * 
 * int* arr = new int[3]{1, 2, 3};  // 动态数组
 * delete[] arr;  // 释放数组内存
 * 
 * 
 * 重要区别：
 * 1. Java/Kotlin 自动管理内存（GC）
 * 2. C++ 需要手动管理（或使用智能指针）
 * 3. C++ 可以直接操作内存地址
 * 4. C++ 指针可以进行算术运算
 */
