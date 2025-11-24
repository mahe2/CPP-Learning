/*
 * 指针 vs 引用 - 深入理解
 * 
 * 这是 C++ 与 Java/Kotlin 最大的区别之一
 */

#include <iostream>
#include <string>

using namespace std;

// ========== 第一部分：指针基础 ==========
void pointerBasics() {
    cout << "=== 指针基础 ===" << endl;
    
    // 1. 什么是指针？指针就是地址！
    int num = 42;
    int* ptr = &num;  // ptr 存储 num 的地址
    
    cout << "变量 num 的值: " << num << endl;
    cout << "变量 num 的地址: " << &num << endl;
    cout << "指针 ptr 存储的地址: " << ptr << endl;
    cout << "通过指针访问的值: " << *ptr << endl;
    
    // 2. 指针可以修改指向
    int another = 100;
    ptr = &another;  // ✅ 指针可以重新指向其他变量
    cout << "\n指针重新指向后: " << *ptr << endl;
    
    // 3. 空指针
    int* nullPtr = nullptr;  // C++11 推荐
    // int* oldNull = NULL;   // 旧写法
    
    if (nullPtr == nullptr) {
        cout << "这是一个空指针" << endl;
    }
    
    // 4. 指针的指针
    int value = 10;
    int* p1 = &value;
    int** p2 = &p1;  // 指针的指针
    
    cout << "\n指针的指针：" << endl;
    cout << "value = " << value << endl;
    cout << "*p1 = " << *p1 << endl;
    cout << "**p2 = " << **p2 << endl;
}

// ========== 第二部分：引用基础 ==========
void referenceBasics() {
    cout << "\n\n=== 引用基础 ===" << endl;
    
    // 1. 什么是引用？引用是变量的别名！
    int num = 42;
    int& ref = num;  // ref 是 num 的别名
    
    cout << "变量 num: " << num << endl;
    cout << "引用 ref: " << ref << endl;
    cout << "num 的地址: " << &num << endl;
    cout << "ref 的地址: " << &ref << endl;  // 地址相同！
    
    // 2. 通过引用修改原变量
    ref = 100;
    cout << "\n修改 ref 后, num = " << num << endl;  // num 也变了
    
    // 3. 引用必须初始化，且不能重新绑定
    // int& invalidRef;  // ❌ 错误！引用必须初始化
    
    int another = 200;
    ref = another;  // ❌ 这不是重新绑定！而是把 another 的值赋给 num
    cout << "执行 ref = another 后:" << endl;
    cout << "num = " << num << endl;      // 200（被修改了）
    cout << "another = " << another << endl;  // 200
}

// ========== 第三部分：指针 vs 引用 对比 ==========
void pointerVsReference() {
    cout << "\n\n=== 指针 vs 引用 对比 ===" << endl;
    
    int x = 10, y = 20;
    
    // 指针
    int* ptr = &x;
    cout << "指针初始指向 x: " << *ptr << endl;
    ptr = &y;  // ✅ 可以重新指向
    cout << "指针重新指向 y: " << *ptr << endl;
    ptr = nullptr;  // ✅ 可以为空
    
    // 引用
    int& ref = x;
    cout << "\n引用初始绑定 x: " << ref << endl;
    ref = y;  // ❌ 这不是重新绑定！而是把 y 的值赋给 x
    cout << "执行 ref = y 后, x = " << x << endl;  // x 变成了 20
    // ref = nullptr;  // ❌ 引用不能为空
    
    // 语法差异
    cout << "\n语法对比：" << endl;
    int value = 100;
    int* p = &value;
    int& r = value;
    
    cout << "指针访问值: " << *p << " (需要解引用 *)" << endl;
    cout << "引用访问值: " << r << " (直接使用，像变量一样)" << endl;
}

// ========== 第四部分：函数参数传递 ==========
// 按值传递
void passByValue(int x) {
    x = 999;  // 不影响外部
}

// 按指针传递
void passByPointer(int* ptr) {
    *ptr = 999;  // 修改指针指向的值
}

// 按引用传递
void passByReference(int& ref) {
    ref = 999;  // 直接修改原变量
}

void parameterPassing() {
    cout << "\n\n=== 函数参数传递 ===" << endl;
    
    // 1. 按值传递
    int val1 = 10;
    passByValue(val1);
    cout << "按值传递后: " << val1 << endl;  // 10（不变）
    
    // 2. 按指针传递
    int val2 = 10;
    passByPointer(&val2);
    cout << "按指针传递后: " << val2 << endl;  // 999（改变）
    
    // 3. 按引用传递
    int val3 = 10;
    passByReference(val3);
    cout << "按引用传递后: " << val3 << endl;  // 999（改变）
}

// ========== 第五部分：为什么 Java/Kotlin 只有引用？ ==========
void whyJavaKotlinDifferent() {
    cout << "\n\n=== 为什么 Java/Kotlin 只有引用？ ===" << endl;
    
    cout << "\n【设计理念】" << endl;
    cout << "C++：" << endl;
    cout << "  - 低级控制：允许直接操作内存地址" << endl;
    cout << "  - 手动管理：程序员负责内存管理" << endl;
    cout << "  - 性能优先：指针操作快速，零抽象开销" << endl;
    cout << "  - 危险但强大：可能出现野指针、内存泄漏" << endl;
    
    cout << "\nJava/Kotlin：" << endl;
    cout << "  - 安全优先：隐藏内存细节，避免指针错误" << endl;
    cout << "  - 自动管理：垃圾回收器（GC）自动管理内存" << endl;
    cout << "  - 简化编程：不需要考虑内存分配/释放" << endl;
    cout << "  - 牺牲性能：GC 有一定性能开销" << endl;
    
    cout << "\n【实际例子】" << endl;
    cout << "C++ 指针可能的问题：" << endl;
    cout << "  int* ptr = new int(10);" << endl;
    cout << "  delete ptr;              // 必须手动释放" << endl;
    cout << "  *ptr = 20;               // 悬空指针！未定义行为" << endl;
    
    cout << "\nJava 引用的安全性：" << endl;
    cout << "  Integer obj = new Integer(10);" << endl;
    cout << "  obj = null;              // 安全" << endl;
    cout << "  // GC 自动回收，不会有悬空指针" << endl;
}

// ========== 第六部分：Java/Kotlin 引用 vs C++ 引用 ==========
class Person {
public:
    string name;
    Person(string n) : name(n) {}
};

void javaStyleReference() {
    cout << "\n\n=== Java/Kotlin 风格的引用 ===" << endl;
    
    // Java/Kotlin 的对象引用类似 C++ 的指针
    Person* p1 = new Person("张三");  // 类似 Java: Person p1 = new Person("张三");
    Person* p2 = p1;                  // 引用复制，都指向同一个对象
    
    p2->name = "李四";
    cout << "p1->name: " << p1->name << endl;  // 李四（两个指针指向同一对象）
    cout << "p2->name: " << p2->name << endl;  // 李四
    
    delete p1;  // C++ 需要手动释放，Java/Kotlin 自动回收
    p1 = nullptr;
    p2 = nullptr;
    
    cout << "\n对比：" << endl;
    cout << "Java:   Person p = new Person(\"张三\");  // p 是引用" << endl;
    cout << "C++:    Person* p = new Person(\"张三\");  // p 是指针" << endl;
    cout << "效果相同，但 Java 不需要 delete！" << endl;
}

// ========== 第七部分：实际应用场景 ==========
void practicalExamples() {
    cout << "\n\n=== 实际应用场景 ===" << endl;
    
    // 1. 动态数组（类似 Java ArrayList 的底层实现）
    cout << "【1. 动态数组】" << endl;
    int* arr = new int[5];  // 动态分配数组
    for (int i = 0; i < 5; i++) {
        arr[i] = i * 10;
    }
    
    cout << "数组元素: ";
    for (int i = 0; i < 5; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    delete[] arr;  // 释放数组内存
    
    // 2. 链表节点（指针的经典应用）
    cout << "\n【2. 链表节点】" << endl;
    struct Node {
        int data;
        Node* next;  // 指向下一个节点的指针
    };
    
    Node* head = new Node{1, nullptr};
    head->next = new Node{2, nullptr};
    head->next->next = new Node{3, nullptr};
    
    cout << "链表: ";
    Node* current = head;
    while (current != nullptr) {
        cout << current->data << " -> ";
        current = current->next;
    }
    cout << "null" << endl;
    
    // 清理链表
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    
    // 3. const 引用传递大对象（性能优化）
    cout << "\n【3. 高效参数传递】" << endl;
    string longString = "这是一个很长的字符串...";
    
    auto printByValue = [](string s) {  // 按值传递：复制整个字符串
        cout << "按值传递（会复制）: " << s.substr(0, 10) << "..." << endl;
    };
    
    auto printByRef = [](const string& s) {  // 按引用传递：不复制
        cout << "按引用传递（不复制）: " << s.substr(0, 10) << "..." << endl;
    };
    
    printByValue(longString);  // 慢（有复制开销）
    printByRef(longString);    // 快（无复制开销）
}

int main() {
    cout << "=== C++ 指针与引用完全指南 ===" << endl;
    
    // 1. 指针基础
    pointerBasics();
    
    // 2. 引用基础
    referenceBasics();
    
    // 3. 指针 vs 引用对比
    pointerVsReference();
    
    // 4. 函数参数传递
    parameterPassing();
    
    // 5. 为什么 Java/Kotlin 不同
    whyJavaKotlinDifferent();
    
    // 6. Java 风格的引用
    javaStyleReference();
    
    // 7. 实际应用
    practicalExamples();
    
    return 0;
}

/*
 * ========== 核心概念总结 ==========
 * 
 * 1. 指针是什么？
 *    - 指针 = 存储内存地址的变量
 *    - 类型：int* ptr（指向 int 的指针）
 *    - 操作：& 取地址，* 解引用
 * 
 * 2. 引用是什么？
 *    - 引用 = 变量的别名
 *    - 类型：int& ref（int 的引用）
 *    - 特点：必须初始化，不能重新绑定
 * 
 * 3. 指针 vs 引用
 *    ┌──────────────┬─────────────────┬─────────────────┐
 *    │   特性       │     指针        │     引用        │
 *    ├──────────────┼─────────────────┼─────────────────┤
 *    │ 本质         │ 存储地址的变量  │ 变量的别名      │
 *    │ 可否为空     │ ✅ 可以nullptr  │ ❌ 不能为空     │
 *    │ 可否重新指向 │ ✅ 可以         │ ❌ 不能         │
 *    │ 必须初始化   │ ❌ 不强制       │ ✅ 必须         │
 *    │ 语法         │ *ptr 解引用     │ 直接使用        │
 *    │ 安全性       │ 较低（危险）    │ 较高（安全）    │
 *    │ 灵活性       │ 高              │ 低              │
 *    └──────────────┴─────────────────┴─────────────────┘
 * 
 * 
 * ========== C++ vs Java/Kotlin ==========
 * 
 * C++:
 *   - 有指针：int* ptr = &num;
 *   - 有引用：int& ref = num;
 *   - 可直接操作内存地址
 *   - 手动内存管理（new/delete）
 * 
 * Java:
 *   - 没有显式指针
 *   - 对象都是引用（类似 C++ 指针）
 *   - Integer obj = new Integer(10);  // obj 是引用
 *   - 自动垃圾回收（GC）
 * 
 * Kotlin:
 *   - 同 Java，没有显式指针
 *   - val obj = Person("张三")  // obj 是引用
 *   - 自动垃圾回收（GC）
 * 
 * 
 * ========== 为什么设计不同？ ==========
 * 
 * C++ 的设计理念：
 *   - 性能优先："零成本抽象"
 *   - 底层控制：可直接操作硬件
 *   - 灵活性：程序员决定一切
 *   - 代价：容易出错（野指针、内存泄漏）
 * 
 * Java/Kotlin 的设计理念：
 *   - 安全优先：避免指针错误
 *   - 简单易用：不需要考虑内存管理
 *   - 跨平台：JVM 抽象层
 *   - 代价：性能开销（GC）、灵活性降低
 * 
 * 
 * ========== 对比示例 ==========
 * 
 * 【示例1：对象创建】
 * C++:
 *   Person p1("张三");           // 栈上创建，自动管理
 *   Person* p2 = new Person("李四");  // 堆上创建，手动 delete
 * 
 * Java:
 *   Person p1 = new Person("张三");  // 都在堆上，GC 自动回收
 * 
 * 【示例2：数组】
 * C++:
 *   int arr[5];              // 栈上，固定大小
 *   int* arr = new int[5];   // 堆上，手动 delete[]
 * 
 * Java:
 *   int[] arr = new int[5];  // 堆上，GC 自动回收
 * 
 * 【示例3：函数传递】
 * C++:
 *   void func(int x);        // 按值（复制）
 *   void func(int* x);       // 按指针（可修改）
 *   void func(int& x);       // 按引用（可修改）
 *   void func(const int& x); // 按引用（只读，高效）
 * 
 * Java:
 *   void func(int x);        // 基本类型按值
 *   void func(Person p);     // 对象按引用（实际是引用的值）
 * 
 * 
 * ========== 使用建议 ==========
 * 
 * 何时使用指针：
 *   ✅ 需要可选值（可以为 nullptr）
 *   ✅ 需要重新指向不同对象
 *   ✅ 数组、动态内存分配
 *   ✅ 多态（基类指针指向派生类）
 *   ✅ 数据结构（链表、树）
 * 
 * 何时使用引用：
 *   ✅ 函数参数（避免复制）
 *   ✅ 函数返回值（返回成员变量）
 *   ✅ 操作符重载
 *   ✅ 不需要为空的情况
 *   ✅ 不需要重新绑定的情况
 * 
 * 现代 C++ 建议：
 *   - 优先使用引用
 *   - 需要指针时用智能指针（unique_ptr, shared_ptr）
 *   - 避免裸指针（raw pointer）
 * 
 * 
 * ========== 记忆口诀 ==========
 * 
 * 指针是地址，可空可改向
 * 引用是别名，绑定到底不改
 * Java 只引用，安全又简单
 * C++ 两都有，灵活要小心
 */
