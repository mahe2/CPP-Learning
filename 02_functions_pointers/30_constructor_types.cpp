/**
 * C++ 构造函数完全指南
 * 
 * C++ 有 6 种构造函数（Java 只有 1-2 种）
 * 
 * 为什么这么多？
 * - C++ 需要精确控制对象的创建、拷贝、移动
 * - 性能优化（避免不必要的拷贝）
 * - 资源管理（RAII）
 * 
 * Java 为什么少？
 * - Java 对象总是引用，赋值只拷贝引用
 * - GC 自动管理内存
 * - 不需要关心拷贝/移动的性能问题
 */

#include <iostream>
#include <string>
#include <vector>
#include <cstring>

using namespace std;

// ============================================
// 演示类：包含所有 6 种构造函数
// ============================================

class MyString {
private:
    char* data;      // 动态分配的数据
    size_t length;
    
    void printInfo(const string& msg) const {
        cout << "  [" << msg << "] ";
        if (data) {
            cout << "data=\"" << data << "\", length=" << length;
            cout << ", address=" << (void*)data;
        } else {
            cout << "data=nullptr, length=" << length;
        }
        cout << endl;
    }
    
public:
    // ============================================
    // 1️⃣ 默认构造函数（无参数）
    // ============================================
    MyString() : data(nullptr), length(0) {
        cout << "\n✅ 1. 默认构造函数" << endl;
        printInfo("构造后");
    }
    
    // ============================================
    // 2️⃣ 参数构造函数（最常用）
    // ============================================
    MyString(const char* str) {
        cout << "\n✅ 2. 参数构造函数: \"" << str << "\"" << endl;
        length = strlen(str);
        data = new char[length + 1];  // 分配内存
        strcpy(data, str);
        printInfo("构造后");
    }
    
    // ============================================
    // 3️⃣ 拷贝构造函数（深拷贝）
    // ============================================
    MyString(const MyString& other) {
        cout << "\n✅ 3. 拷贝构造函数（深拷贝）" << endl;
        cout << "  从对象拷贝: ";
        other.printInfo("源对象");
        
        // 深拷贝：分配新内存，拷贝数据
        length = other.length;
        if (other.data) {
            data = new char[length + 1];
            strcpy(data, other.data);
        } else {
            data = nullptr;
        }
        printInfo("新对象");
    }
    
    // ============================================
    // 4️⃣ 移动构造函数（C++11，性能优化）
    // ============================================
    MyString(MyString&& other) noexcept {
        cout << "\n✅ 4. 移动构造函数（转移所有权）" << endl;
        cout << "  从临时对象移动: ";
        other.printInfo("源对象");
        
        // 移动：直接"窃取"资源，不拷贝数据
        data = other.data;
        length = other.length;
        
        // 将源对象置为空
        other.data = nullptr;
        other.length = 0;
        
        printInfo("新对象");
        other.printInfo("源对象已清空");
    }
    
    // ============================================
    // 5️⃣ 委托构造函数（C++11）
    // ============================================
    MyString(size_t len) : MyString() {  // 委托给默认构造函数
        cout << "\n✅ 5. 委托构造函数: 创建指定长度的空字符串" << endl;
        data = new char[len + 1];
        length = len;
        memset(data, 'A', len);
        data[len] = '\0';
        printInfo("构造后");
    }
    
    // ============================================
    // 6️⃣ 转换构造函数（隐式转换）
    // ============================================
    // 实际上就是单参数的构造函数
    // 可以用 explicit 禁止隐式转换
    
    // 析构函数
    ~MyString() {
        if (data) {
            cout << "  [析构] 释放内存: \"" << data << "\"" << endl;
            delete[] data;
        } else {
            cout << "  [析构] 无需释放（data=nullptr）" << endl;
        }
    }
    
    // 辅助方法
    const char* c_str() const { return data ? data : ""; }
    size_t size() const { return length; }
    
    void print() const {
        cout << "  MyString: \"" << c_str() << "\" (len=" << length << ")" << endl;
    }
};

// ============================================
// 详细解释每种构造函数
// ============================================

void explain_constructors() {
    cout << "\n" << string(70, '=') << endl;
    cout << "C++ 的 6 种构造函数详解" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【C++ 6 种构造函数 vs Java】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
1️⃣  默认构造函数（无参数）
   C++: MyString s;             // 栈上创建
   Java: String s = new String();  // 堆上创建
   
2️⃣  参数构造函数
   C++: MyString s("hello");
   Java: String s = new String("hello");
   
3️⃣  拷贝构造函数（深拷贝）
   C++: MyString s2 = s1;       // 拷贝整个对象！
   Java: String s2 = s1;        // 只拷贝引用！
   
4️⃣  移动构造函数（C++11，Java 没有）
   C++: MyString s = createString();  // 移动，不拷贝
   Java: 不需要，因为是引用
   
5️⃣  委托构造函数（C++11）
   C++: MyString(int len) : MyString() { ... }
   Java: this() 调用其他构造函数
   
6️⃣  转换构造函数
   C++: MyString s = "hello";   // 隐式转换
   Java: 没有隐式转换（必须显式 new）

关键区别：
  - Java: 只需要 1-2 种构造函数（默认 + 参数）
  - C++: 需要 6 种，因为要精确控制对象的创建、拷贝、移动
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
}

// ============================================
// 1. 默认构造函数演示
// ============================================

void demo_default_constructor() {
    cout << "\n" << string(70, '=') << endl;
    cout << "演示 1：默认构造函数" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【什么是默认构造函数？】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
定义：无参数的构造函数

作用：
  - 创建"空"对象
  - 初始化成员变量为默认值
  - 容器需要（vector、array）

何时使用：
  ✅ 创建数组：MyString arr[10];  // 每个元素都调用默认构造
  ✅ 容器：vector<MyString> v(5);  // 5 个默认构造的对象
  ✅ 延迟初始化：先创建，后赋值

Java 对比：
  - Java 也有默认构造函数
  - 但 Java 对象总在堆上：new MyClass()
  - C++ 可以在栈上：MyClass obj;
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
    
    cout << "创建默认对象：" << endl;
    MyString s1;  // 调用默认构造函数
    s1.print();
}

// ============================================
// 2. 参数构造函数演示
// ============================================

void demo_parameter_constructor() {
    cout << "\n" << string(70, '=') << endl;
    cout << "演示 2：参数构造函数" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【什么是参数构造函数？】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
定义：带参数的构造函数

作用：
  - 用给定的值初始化对象
  - 最常用的构造方式

何时使用：
  ✅ 创建有意义的对象：MyString s("hello");
  ✅ 避免两步初始化（先创建，后赋值）

Java 对比：
  - 完全一样
  - Java: String s = new String("hello");
  - C++: MyString s("hello");  // 栈上
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
    
    cout << "创建带参数的对象：" << endl;
    MyString s2("Hello World");
    s2.print();
}

// ============================================
// 3. 拷贝构造函数演示
// ============================================

void demo_copy_constructor() {
    cout << "\n" << string(70, '=') << endl;
    cout << "演示 3：拷贝构造函数（深拷贝）" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【什么是拷贝构造函数？】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
定义：用一个对象创建另一个对象

语法：MyString(const MyString& other)

作用：
  - 创建对象的副本
  - 深拷贝（拷贝数据，不是指针）

何时调用：
  ✅ MyString s2 = s1;          // 拷贝构造
  ✅ MyString s2(s1);            // 拷贝构造
  ✅ func(s1);                   // 值传递，拷贝
  ✅ return s;                   // 返回值，拷贝（可能优化掉）

为什么需要？
  - 默认拷贝是浅拷贝（只拷贝指针）
  - 浅拷贝会导致两个对象共享同一块内存
  - 析构时会 double free（崩溃）

Java 对比：
  - Java 没有拷贝构造函数！
  - Java 赋值只拷贝引用（浅拷贝）
  - String s2 = s1;  // s1 和 s2 指向同一个对象
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
    
    cout << "创建原始对象：" << endl;
    MyString s1("Original");
    s1.print();
    
    cout << "\n拷贝对象（深拷贝）：" << endl;
    MyString s2 = s1;  // 调用拷贝构造函数
    s2.print();
    
    cout << "\n两个对象是独立的（不同内存地址）" << endl;
}

// ============================================
// 4. 移动构造函数演示（重点！）
// ============================================

MyString createString(const char* str) {
    cout << "  [函数内] 创建临时对象..." << endl;
    return MyString(str);  // 返回临时对象
}

void demo_move_constructor() {
    cout << "\n" << string(70, '=') << endl;
    cout << "演示 4：移动构造函数（C++11 重大创新）" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【什么是移动构造函数？为什么需要？】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
问题场景：
  MyString createString() {
      MyString temp("hello");  // 1. 创建临时对象
      return temp;              // 2. 拷贝返回（慢！）
  }
  MyString s = createString();  // 3. 又拷贝一次（慢！）

传统方式（C++98）：
  - 需要拷贝 2 次（函数内 + 函数外）
  - 每次拷贝都分配内存、复制数据
  - 临时对象马上就销毁，拷贝浪费性能

移动构造函数（C++11）：
  - 不拷贝数据，直接"窃取"临时对象的资源
  - 把临时对象的指针赋给新对象
  - 将临时对象置空（防止析构时释放）
  - 零拷贝，超快！

语法：MyString(MyString&& other)
  - && 是右值引用（临时对象）

何时调用：
  ✅ 返回局部对象：return temp;
  ✅ 容器操作：vec.push_back(MyString("hi"));
  ✅ 显式移动：MyString s2 = move(s1);

Java 为什么没有？
  - Java 对象是引用，赋值本身就是"移动"引用
  - 不需要拷贝数据
  - 垃圾回收器管理内存

C++ 为什么需要？
  - C++ 对象有值语义，默认拷贝数据
  - 大对象拷贝慢（如 vector<int>(100万)）
  - 移动构造可以避免拷贝，大幅提升性能
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
    
    cout << "从函数返回对象（触发移动）：" << endl;
    MyString s3 = createString("Moved String");
    s3.print();
    
    cout << "\n显式移动：" << endl;
    MyString s4("Source");
    MyString s5 = move(s4);  // 显式调用移动构造
    cout << "s4（源对象）：";
    s4.print();
    cout << "s5（目标对象）：";
    s5.print();
}

// ============================================
// 5. 委托构造函数演示
// ============================================

void demo_delegating_constructor() {
    cout << "\n" << string(70, '=') << endl;
    cout << "演示 5：委托构造函数（C++11）" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【什么是委托构造函数？】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
定义：一个构造函数调用另一个构造函数

作用：
  - 避免代码重复
  - 共享初始化逻辑

语法：
  MyString(int len) : MyString() {  // 先调用默认构造
      // 再执行自己的初始化
  }

Java 对比：
  - Java 也有：this() 调用其他构造函数
  - 用法类似
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
    
    cout << "创建指定长度的字符串（委托构造）：" << endl;
    MyString s6(5);  // 调用委托构造函数
    s6.print();
}

// ============================================
// 6. 转换构造函数演示
// ============================================

void demo_conversion_constructor() {
    cout << "\n" << string(70, '=') << endl;
    cout << "演示 6：转换构造函数（隐式转换）" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【什么是转换构造函数？】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
定义：单参数构造函数可以进行隐式类型转换

例子：
  MyString(const char* str);  // 单参数构造函数
  
  MyString s = "hello";  // 隐式转换：const char* → MyString

问题：
  - 可能导致意外的类型转换
  - 难以发现的 bug

解决：用 explicit 禁止隐式转换
  explicit MyString(const char* str);
  MyString s = "hello";     // ❌ 错误
  MyString s("hello");      // ✅ 可以
  MyString s = MyString("hello");  // ✅ 可以

Java 对比：
  - Java 没有隐式转换
  - 必须显式 new
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
    
    cout << "隐式转换（单参数构造）：" << endl;
    MyString s7 = "Implicit Conversion";  // 隐式调用参数构造函数
    s7.print();
}

// ============================================
// 性能对比：拷贝 vs 移动
// ============================================

void performance_comparison() {
    cout << "\n" << string(70, '=') << endl;
    cout << "性能对比：拷贝构造 vs 移动构造" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【拷贝 vs 移动性能对比】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
拷贝构造（慢）：
  - 分配新内存
  - 复制所有数据
  - 大对象拷贝很慢（O(n)）
  
  例子：
  vector<int> v1(1000000);  // 100万元素
  vector<int> v2 = v1;      // 拷贝100万个int（慢！）

移动构造（快）：
  - 不分配内存
  - 不复制数据
  - 只转移指针（O(1)）
  
  例子：
  vector<int> v1(1000000);
  vector<int> v2 = move(v1);  // 只移动指针（快！）

性能差异：
  - 小对象：差别不大
  - 大对象：移动快 1000+ 倍！
  - 资源对象（文件、网络）：移动是唯一选择
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
    
    cout << "\n实际演示：" << endl;
    
    cout << "\n1. 拷贝大字符串（慢）：" << endl;
    MyString big1("This is a very long string that needs to be copied");
    MyString big2 = big1;  // 拷贝
    
    cout << "\n2. 移动大字符串（快）：" << endl;
    MyString big3("This is a very long string that will be moved");
    MyString big4 = move(big3);  // 移动
}

// ============================================
// 使用场景总结
// ============================================

void usage_summary() {
    cout << "\n" << string(70, '=') << endl;
    cout << "使用场景总结" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【6 种构造函数使用场景】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

1️⃣  默认构造函数（无参数）
   何时用：
     - 数组：MyString arr[10];
     - 容器：vector<MyString> v(5);
     - 延迟初始化
   
   必须提供：
     - 如果要用容器存储
     - 如果要创建数组

2️⃣  参数构造函数
   何时用：
     - 最常用！创建有意义的对象
     - MyString s("hello");
   
   必须提供：
     - 几乎总是需要

3️⃣  拷贝构造函数
   何时用：
     - 创建对象副本：MyString s2 = s1;
     - 值传递：func(obj);
     - 容器操作：vec.push_back(obj);
   
   必须提供：
     - 如果类管理动态内存（指针成员）
     - 如果类管理资源（文件、网络）
     - 否则会浅拷贝，导致 double free

4️⃣  移动构造函数（C++11）
   何时用：
     - 从临时对象创建
     - 函数返回对象
     - 容器操作（提升性能）
     - 显式移动：move(obj)
   
   必须提供：
     - 如果对象很大（如 vector）
     - 如果对象不可拷贝（如 unique_ptr）
     - 性能关键代码

5️⃣  委托构造函数（C++11）
   何时用：
     - 多个构造函数有共同初始化逻辑
     - 避免代码重复
   
   可选：
     - 代码优化，非必须

6️⃣  转换构造函数
   何时用：
     - 单参数构造函数自动成为转换构造
     - 用 explicit 禁止隐式转换
   
   建议：
     - 除非明确需要隐式转换，否则用 explicit

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
}

// ============================================
// 最佳实践
// ============================================

void best_practices() {
    cout << "\n" << string(70, '=') << endl;
    cout << "最佳实践" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【构造函数最佳实践】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

1️⃣  Rule of Three（三法则，C++98）
   如果定义了以下任何一个，通常需要定义全部三个：
     - 析构函数
     - 拷贝构造函数
     - 拷贝赋值运算符
   
   原因：管理资源（动态内存、文件句柄等）

2️⃣  Rule of Five（五法则，C++11）
   在 Rule of Three 基础上，还要定义：
     - 移动构造函数
     - 移动赋值运算符
   
   原因：性能优化

3️⃣  Rule of Zero（零法则，现代 C++）
   最佳实践：不要手动管理资源！
     - 用智能指针（unique_ptr, shared_ptr）
     - 用标准容器（vector, string）
     - 编译器自动生成所有构造函数
   
   例子：
     class Good {
         string name;           // 自动管理
         vector<int> data;      // 自动管理
         unique_ptr<int> ptr;   // 自动管理
         // 不需要定义任何构造函数！
     };

4️⃣  使用 explicit
   单参数构造函数应该用 explicit（除非明确需要隐式转换）：
     explicit MyString(const char* str);

5️⃣  使用 noexcept
   移动构造函数应该标记 noexcept：
     MyString(MyString&& other) noexcept;
   
   原因：容器（如 vector）重新分配时，如果移动构造不是 noexcept，
         会退化为拷贝（性能损失）

6️⃣  优先移动而非拷贝
   返回对象时，编译器会自动移动（RVO）：
     MyString createString() {
         MyString temp("hello");
         return temp;  // 自动移动，不拷贝
     }

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
}

// ============================================
// Java 对比
// ============================================

void java_comparison() {
    cout << "\n" << string(70, '=') << endl;
    cout << "C++ vs Java 构造函数对比" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【C++ vs Java 构造函数】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Java（简单）：
  1. 默认构造函数：MyClass()
  2. 参数构造函数：MyClass(String name)
  3. 委托构造：this()
  
  仅此而已！

C++（复杂但强大）：
  1. 默认构造函数：MyClass()
  2. 参数构造函数：MyClass(const string& name)
  3. 拷贝构造函数：MyClass(const MyClass& other)  // Java 没有
  4. 移动构造函数：MyClass(MyClass&& other)       // Java 没有
  5. 委托构造函数：MyClass(int) : MyClass() {}
  6. 转换构造函数：MyClass = "string"              // Java 没有

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

为什么 Java 简单？
  - 对象总是引用，赋值只拷贝引用
  - 垃圾回收自动管理内存
  - 不需要关心拷贝/移动性能

为什么 C++ 复杂？
  - 对象有值语义，需要精确控制拷贝
  - 手动管理内存（或 RAII）
  - 性能优化（避免不必要拷贝）

从 Java 到 C++ 的思维转变：
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Java 思维：
  MyClass obj1 = new MyClass("data");
  MyClass obj2 = obj1;  // 拷贝引用，指向同一对象

C++ 思维：
  MyClass obj1("data");
  MyClass obj2 = obj1;  // 拷贝整个对象（深拷贝）！
  
  如果想要引用语义：
  MyClass* ptr1 = new MyClass("data");
  MyClass* ptr2 = ptr1;  // 拷贝指针（类似 Java）

关键理解：
  - Java: 赋值 = 拷贝引用（浅拷贝）
  - C++: 赋值 = 拷贝对象（深拷贝，除非用指针）

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
}

// ============================================
// Main
// ============================================

int main() {
    cout << "======================================================================" << endl;
    cout << "  C++ 构造函数完全指南" << endl;
    cout << "  （6 种构造函数 vs Java 的 2 种）" << endl;
    cout << "======================================================================" << endl;
    
    explain_constructors();
    
    demo_default_constructor();
    demo_parameter_constructor();
    demo_copy_constructor();
    demo_move_constructor();
    demo_delegating_constructor();
    demo_conversion_constructor();
    
    performance_comparison();
    usage_summary();
    best_practices();
    java_comparison();
    
    cout << "\n======================================================================" << endl;
    cout << "核心总结" << endl;
    cout << "======================================================================" << endl;
    
    cout << R"(
【C++ 6 种构造函数】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
1️⃣  默认构造：MyString()
   - 无参数，创建空对象
   - 数组、容器需要

2️⃣  参数构造：MyString(const char* str)
   - 最常用
   - 用给定值初始化

3️⃣  拷贝构造：MyString(const MyString& other)
   - 深拷贝，创建副本
   - 管理资源时必须提供

4️⃣  移动构造：MyString(MyString&& other)
   - C++11 重大创新
   - 转移所有权，不拷贝数据
   - 性能优化关键

5️⃣  委托构造：MyString(int) : MyString() {}
   - 调用其他构造函数
   - 避免代码重复

6️⃣  转换构造：单参数构造，可隐式转换
   - 建议用 explicit 禁止

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

【为什么移动构造函数这么重要？】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
问题：
  - C++ 默认拷贝对象（值语义）
  - 大对象拷贝很慢（如 vector<int>(100万)）
  - 临时对象马上销毁，拷贝浪费性能

解决：
  - 移动构造：转移资源，不拷贝数据
  - 性能提升：1000+ 倍（大对象）
  - 零开销抽象

Java 为什么不需要：
  - Java 对象是引用
  - 赋值本身就是"移动"引用
  - GC 管理内存

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

【记忆要点】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
必须掌握：
  1. 默认构造（数组、容器）
  2. 参数构造（最常用）
  3. 拷贝构造（深拷贝，管理资源）
  4. 移动构造（性能优化）

Rule of Five：
  如果管理资源，定义全部 5 个：
    - 析构函数
    - 拷贝构造 + 拷贝赋值
    - 移动构造 + 移动赋值

Rule of Zero（最佳）：
  用智能指针和标准容器，不手动管理资源
  编译器自动生成所有构造函数

从 Java 到 C++：
  - Java: 赋值拷贝引用（浅拷贝）
  - C++: 赋值拷贝对象（深拷贝）
  - 理解值语义 vs 引用语义

======================================================================
)" << endl;
    
    return 0;
}

/**
 * 终极总结：
 * 
 * 为什么 C++ 有这么多构造函数？
 *   1. 值语义：需要精确控制对象拷贝
 *   2. 性能优化：移动构造避免拷贝
 *   3. 资源管理：RAII 需要正确的构造/析构
 * 
 * Java 为什么简单？
 *   1. 引用语义：赋值只拷贝引用
 *   2. 垃圾回收：自动管理内存
 *   3. 不需要关心拷贝/移动性能
 * 
 * 移动构造为什么重要？
 *   1. C++11 重大创新
 *   2. 避免不必要的拷贝
 *   3. 大对象性能提升 1000+ 倍
 *   4. 现代 C++ 的核心
 */
