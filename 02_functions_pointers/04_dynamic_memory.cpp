/*
 * 动态内存分配
 * 
 * C++ 使用 new 和 delete 管理堆内存
 * 类似 Java 的 new，但需要手动释放
 */

#include <iostream>
#include <string>

using namespace std;

int main() {
    // ========== 动态分配单个变量 ==========
    cout << "=== 动态分配单个变量 ===" << endl;
    
    int* ptr = new int;      // 在堆上分配一个 int
    *ptr = 42;
    cout << "动态分配的值: " << *ptr << endl;
    
    delete ptr;              // 释放内存（重要！）
    ptr = nullptr;           // 良好习惯：释放后设为 nullptr
    
    // 分配并初始化
    int* ptr2 = new int(100);  // 分配并初始化为 100
    cout << "初始化的值: " << *ptr2 << endl;
    delete ptr2;
    
    // ========== 动态分配数组 ==========
    cout << "\n=== 动态分配数组 ===" << endl;
    
    int size = 5;
    int* arr = new int[size];  // 动态数组，类似 Java: new int[5]
    
    // 初始化数组
    for (int i = 0; i < size; i++) {
        arr[i] = i * 10;
    }
    
    // 输出数组
    cout << "数组元素: ";
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    
    delete[] arr;  // 释放数组（注意 delete[] 而不是 delete）
    arr = nullptr;
    
    // ========== 动态分配对象 ==========
    cout << "\n=== 动态分配对象 ===" << endl;
    
    string* strPtr = new string("Hello, C++");
    cout << "字符串: " << *strPtr << endl;
    cout << "长度: " << strPtr->length() << endl;  // -> 用于访问指针指向对象的成员
    
    delete strPtr;
    
    // ========== 内存泄漏示例（避免！）==========
    cout << "\n=== 内存泄漏警告 ===" << endl;
    
    // 错误示例：忘记 delete
    /*
    for (int i = 0; i < 1000000; i++) {
        int* leak = new int(i);
        // 忘记 delete leak;  <- 内存泄漏！
    }
    */
    
    // 正确做法
    for (int i = 0; i < 10; i++) {
        int* temp = new int(i);
        cout << *temp << " ";
        delete temp;  // 及时释放
    }
    cout << endl;
    
    // ========== new 失败处理 ==========
    cout << "\n=== 内存分配失败 ===" << endl;
    
    try {
        // 尝试分配超大内存（可能失败）
        // int* huge = new int[1000000000000];  // 可能抛出 bad_alloc 异常
        
        // 使用 nothrow 版本，失败时返回 nullptr
        int* safePtr = new (nothrow) int[100];
        if (safePtr == nullptr) {
            cout << "内存分配失败！" << endl;
        } else {
            cout << "内存分配成功" << endl;
            delete[] safePtr;
        }
    } catch (bad_alloc& e) {
        cout << "捕获异常: " << e.what() << endl;
    }
    
    return 0;
}

/*
 * 对比 Java/Kotlin:
 * 
 * === Java ===
 * // 自动内存管理
 * Integer num = new Integer(42);  // 堆分配
 * int[] arr = new int[5];         // 数组分配
 * String str = new String("Hello");
 * // 不需要 delete，GC 自动回收
 * 
 * === Kotlin ===
 * val num = 42  // 自动装箱
 * val arr = IntArray(5)  // 数组
 * val str = "Hello"
 * // 不需要手动释放
 * 
 * === C++ ===
 * int* num = new int(42);      // 手动分配
 * int* arr = new int[5];       // 数组分配
 * string* str = new string("Hello");
 * 
 * delete num;    // 手动释放
 * delete[] arr;  // 释放数组
 * delete str;    // 释放对象
 * 
 * 
 * 重要规则：
 * 1. new 和 delete 必须配对使用
 * 2. new[] 必须用 delete[] 释放
 * 3. delete 后设置指针为 nullptr
 * 4. 避免重复 delete（未定义行为）
 * 5. 现代 C++ 推荐使用智能指针（后续章节）
 * 
 * 
 * 栈 vs 堆：
 * 栈（Stack）：
 *   - int x = 10;  // 自动管理，作用域结束自动释放
 *   - 快速，但空间有限
 * 
 * 堆（Heap）：
 *   - int* x = new int(10);  // 手动管理
 *   - 空间大，但需要手动释放
 */
