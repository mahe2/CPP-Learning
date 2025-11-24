/*
 * C++ 内存模型详解
 * 
 * 这是 C++ 与 Java/Kotlin 的重要区别！
 * Java/Kotlin: 所有对象在堆上，GC 自动管理
 * C++: 栈、堆、全局区都可以，程序员手动控制
 */

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ========== 全局变量（全局/静态存储区）==========
int globalVar = 100;            // 全局变量
static int staticGlobalVar = 200;  // 静态全局变量（只在当前文件可见）
const int CONSTANT = 300;       // 全局常量

// ========== 第一部分：C++ 内存布局 ==========
void memoryLayout() {
    cout << "=== C++ 内存布局 ===" << endl;
    
    /*
     * C++ 程序内存分为 5 个区域：
     * 
     * ┌─────────────────────────────────────────┐
     * │  1. 代码区（Code/Text Segment）         │ ← 存储程序代码（只读）
     * ├─────────────────────────────────────────┤
     * │  2. 全局/静态存储区（Global/Static）    │ ← 全局变量、静态变量
     * │     - 已初始化数据段（.data）           │
     * │     - 未初始化数据段（.bss）            │
     * ├─────────────────────────────────────────┤
     * │  3. 常量区（Constant）                  │ ← 字符串常量、const 常量
     * ├─────────────────────────────────────────┤
     * │  4. 栈区（Stack）↓                      │ ← 局部变量、函数参数
     * │     [向下增长]                           │    自动分配/释放
     * │     ...                                 │
     * │     [向上增长]                           │
     * │  5. 堆区（Heap）↑                       │ ← new/malloc 动态分配
     * └─────────────────────────────────────────┘    程序员手动管理
     */
    
    // 局部变量（栈）
    int stackVar = 10;
    
    // 动态分配（堆）
    int* heapVar = new int(20);
    
    cout << "\n内存地址对比：" << endl;
    cout << "全局变量地址: " << &globalVar << endl;
    cout << "栈变量地址:   " << &stackVar << endl;
    cout << "堆变量地址:   " << heapVar << endl;
    
    // 观察：地址数值大小
    // 通常：全局区 < 堆区 < 栈区（但不绝对，取决于系统）
    
    delete heapVar;  // 释放堆内存
}

// ========== 第二部分：栈内存（Stack）==========
void stackMemory() {
    cout << "\n\n=== 栈内存（Stack） ===" << endl;
    
    /*
     * 栈内存特点：
     * ✅ 自动管理：变量离开作用域自动销毁
     * ✅ 速度快：只需移动栈指针
     * ✅ 大小固定：通常 1-8 MB（系统限制）
     * ⚠️  空间有限：大数组可能栈溢出
     */
    
    int a = 10;          // 栈上分配
    int arr[100];        // 栈上分配数组
    string str = "abc";  // 对象本身在栈，内部数据可能在堆
    
    cout << "栈变量 a 的地址: " << &a << endl;
    cout << "栈数组 arr 的地址: " << arr << endl;
    
    // 作用域演示
    {
        int b = 20;  // b 在内部作用域
        cout << "内部作用域 b = " << b << endl;
    }  // b 离开作用域，自动销毁
    
    // cout << b;  // ❌ 错误！b 已经不存在了
    
    cout << "\n栈内存的生命周期：" << endl;
    cout << "- 进入作用域 { 时分配" << endl;
    cout << "- 离开作用域 } 时释放" << endl;
    cout << "- 完全自动，不需要 delete" << endl;
}

// ========== 第三部分：堆内存（Heap）==========
void heapMemory() {
    cout << "\n\n=== 堆内存（Heap） ===" << endl;
    
    /*
     * 堆内存特点：
     * ✅ 大小灵活：可动态分配任意大小
     * ✅ 生命周期长：手动控制
     * ✅ 空间大：受系统内存限制（GB 级别）
     * ⚠️  速度较慢：需要系统分配
     * ❌ 手动管理：必须 delete，否则内存泄漏
     */
    
    // 1. 单个对象
    int* p1 = new int(42);        // 分配单个 int
    cout << "堆上分配的值: " << *p1 << endl;
    delete p1;                    // 必须手动释放
    
    // 2. 数组
    int* arr = new int[1000];     // 分配 1000 个 int
    arr[0] = 100;
    cout << "堆上数组第一个元素: " << arr[0] << endl;
    delete[] arr;                 // 注意：数组用 delete[]
    
    // 3. 对象
    string* str = new string("Hello");
    cout << "堆上字符串: " << *str << endl;
    delete str;
    
    // 4. 内存泄漏示例（错误示范）
    cout << "\n⚠️  内存泄漏示例：" << endl;
    int* leak = new int(999);
    // 忘记 delete leak;  // ❌ 内存泄漏！
    leak = nullptr;  // 地址丢失，内存永远无法释放
    cout << "内存泄漏：分配了内存但没有 delete" << endl;
}

// ========== 第四部分：全局/静态存储区 ==========
int globalCounter = 0;

void globalStaticMemory() {
    cout << "\n\n=== 全局/静态存储区 ===" << endl;
    
    /*
     * 全局/静态存储区特点：
     * ✅ 程序启动时分配，结束时释放
     * ✅ 全局变量：整个程序可见
     * ✅ 静态变量：保持值，但作用域受限
     */
    
    // 静态局部变量
    static int staticLocal = 0;  // 只初始化一次
    staticLocal++;
    globalCounter++;
    
    cout << "全局变量: " << globalCounter << endl;
    cout << "静态局部变量: " << staticLocal << endl;
    
    // 多次调用，观察差异
    static int callCount = 0;
    callCount++;
    cout << "函数被调用 " << callCount << " 次" << endl;
}

// ========== 第五部分：对比 Java/Kotlin ==========
class Person {
public:
    string name;
    int age;
    
    Person(string n, int a) : name(n), age(a) {
        cout << "Person 构造: " << name << endl;
    }
    
    ~Person() {
        cout << "Person 析构: " << name << endl;
    }
};

void compareWithJava() {
    cout << "\n\n=== C++ vs Java/Kotlin 内存模型 ===" << endl;
    
    cout << "\n【Java/Kotlin】" << endl;
    cout << "Person p1 = new Person(\"张三\", 25);" << endl;
    cout << "  - 对象总是在堆上" << endl;
    cout << "  - p1 是引用（类似指针）" << endl;
    cout << "  - GC 自动回收，不需要管理" << endl;
    
    cout << "\n【C++ 方式1：栈上创建】" << endl;
    {
        Person p1("张三", 25);  // 栈上创建
        cout << "  - 对象在栈上" << endl;
        cout << "  - p1 是对象本身（不是引用）" << endl;
        cout << "  - 离开作用域自动销毁" << endl;
    }  // p1 自动调用析构函数
    
    cout << "\n【C++ 方式2：堆上创建】" << endl;
    Person* p2 = new Person("李四", 30);  // 堆上创建
    cout << "  - 对象在堆上" << endl;
    cout << "  - p2 是指针（类似 Java 引用）" << endl;
    cout << "  - 必须手动 delete" << endl;
    delete p2;  // 手动销毁
}

// ========== 第六部分：实际应用场景 ==========
void practicalScenarios() {
    cout << "\n\n=== 实际应用场景 ===" << endl;
    
    // 场景1：小对象用栈（推荐）
    cout << "\n【场景1：小对象优先用栈】" << endl;
    {
        int x = 10;              // ✅ 栈：快速，自动管理
        string name = "Alice";   // ✅ 栈：方便，无泄漏
        vector<int> vec{1,2,3};  // ✅ 栈（对象本身），内部数据在堆
        
        cout << "小对象用栈：快速、安全、自动管理" << endl;
    }  // 自动释放
    
    // 场景2：大对象或生命周期长，用堆
    cout << "\n【场景2：大对象或需要长生命周期，用堆】" << endl;
    int* bigArray = new int[1000000];  // 100 万个 int，栈放不下
    cout << "大数组必须用堆，栈会溢出" << endl;
    delete[] bigArray;
    
    // 场景3：返回对象
    cout << "\n【场景3：函数返回对象】" << endl;
    auto createPerson = []() -> Person* {
        // Person p("local", 20);
        // return &p;  // ❌ 错误！返回栈对象地址（悬空指针）
        
        return new Person("heap", 20);  // ✅ 正确：堆对象可以返回
    };
    
    Person* p = createPerson();
    cout << "返回堆对象是安全的" << endl;
    delete p;
    
    // 场景4：动态数组（vector 更好）
    cout << "\n【场景4：动态数组】" << endl;
    
    // 方式1：手动管理（老式，不推荐）
    int* arr1 = new int[10];
    // ... 使用
    delete[] arr1;
    
    // 方式2：智能指针（现代 C++）
    // unique_ptr<int[]> arr2(new int[10]);  // 自动释放
    
    // 方式3：STL 容器（最推荐）
    vector<int> arr3(10);  // 自动管理，最安全
    cout << "推荐使用 vector，自动管理内存" << endl;
}

// ========== 第七部分：常见错误 ==========
void commonMistakes() {
    cout << "\n\n=== 常见内存错误 ===" << endl;
    
    // 1. 内存泄漏
    cout << "\n【错误1：内存泄漏】" << endl;
    cout << "int* p = new int(10);" << endl;
    cout << "// 忘记 delete p;  ❌ 内存泄漏！" << endl;
    
    // 2. 重复释放
    cout << "\n【错误2：重复释放】" << endl;
    cout << "int* p = new int(10);" << endl;
    cout << "delete p;" << endl;
    cout << "delete p;  ❌ 重复 delete，崩溃！" << endl;
    cout << "正确做法：delete p; p = nullptr;" << endl;
    
    // 3. 悬空指针
    cout << "\n【错误3：悬空指针】" << endl;
    cout << "int* p = new int(10);" << endl;
    cout << "delete p;" << endl;
    cout << "*p = 20;  ❌ 使用已释放的内存！" << endl;
    
    // 4. 栈溢出
    cout << "\n【错误4：栈溢出】" << endl;
    cout << "int arr[10000000];  ❌ 数组太大，栈溢出！" << endl;
    cout << "应该用：int* arr = new int[10000000];" << endl;
    
    // 5. delete 和 delete[] 混用
    cout << "\n【错误5：delete 和 delete[] 混用】" << endl;
    cout << "int* arr = new int[10];" << endl;
    cout << "delete arr;  ❌ 应该用 delete[] arr;" << endl;
}

// ========== 第八部分：内存地址演示 ==========
void memoryAddressDemo() {
    cout << "\n\n=== 内存地址实际演示 ===" << endl;
    
    // 全局变量
    static int staticVar = 1;
    
    // 栈变量
    int stackVar1 = 2;
    int stackVar2 = 3;
    
    // 堆变量
    int* heapVar1 = new int(4);
    int* heapVar2 = new int(5);
    
    // 字符串常量
    const char* str = "Hello";
    
    cout << "\n各区域内存地址：" << endl;
    cout << "全局变量: " << &globalVar << endl;
    cout << "静态变量: " << &staticVar << endl;
    cout << "常量字符串: " << (void*)str << endl;
    cout << "---" << endl;
    cout << "栈变量1: " << &stackVar1 << endl;
    cout << "栈变量2: " << &stackVar2 << endl;
    cout << "  (地址相近，顺序分配)" << endl;
    cout << "---" << endl;
    cout << "堆变量1: " << heapVar1 << endl;
    cout << "堆变量2: " << heapVar2 << endl;
    cout << "  (地址相近，但可能不连续)" << endl;
    
    // 观察栈变量的地址差
    long diff = (long)&stackVar2 - (long)&stackVar1;
    cout << "\n栈变量地址差: " << diff << " 字节" << endl;
    cout << "  (通常等于变量大小，如 int 是 4 字节)" << endl;
    
    delete heapVar1;
    delete heapVar2;
}

int main() {
    cout << "=== C++ 内存模型完全指南 ===" << endl;
    
    // 1. 内存布局
    memoryLayout();
    
    // 2. 栈内存
    stackMemory();
    
    // 3. 堆内存
    heapMemory();
    
    // 4. 全局/静态存储区
    globalStaticMemory();
    globalStaticMemory();  // 调用第二次，观察静态变量
    
    // 5. 对比 Java/Kotlin
    compareWithJava();
    
    // 6. 实际应用场景
    practicalScenarios();
    
    // 7. 常见错误
    commonMistakes();
    
    // 8. 内存地址演示
    memoryAddressDemo();
    
    return 0;
}

/*
 * ========== 核心总结 ==========
 * 
 * 1. C++ 内存分为 5 个区域：
 * 
 *    ┌──────────────────┬──────────────┬──────────────────┐
 *    │   区域           │   存储内容   │   生命周期       │
 *    ├──────────────────┼──────────────┼──────────────────┤
 *    │ 代码区           │ 程序代码     │ 程序运行期间     │
 *    │ 全局/静态区      │ 全局/静态变量│ 程序运行期间     │
 *    │ 常量区           │ 字符串常量等 │ 程序运行期间     │
 *    │ 栈区（Stack）    │ 局部变量     │ 作用域结束自动释放│
 *    │ 堆区（Heap）     │ new 分配     │ 手动 delete      │
 *    └──────────────────┴──────────────┴──────────────────┘
 * 
 * 
 * 2. 栈 vs 堆
 * 
 *    ┌──────────┬─────────────────┬─────────────────┐
 *    │   特性   │     栈（Stack） │     堆（Heap）  │
 *    ├──────────┼─────────────────┼─────────────────┤
 *    │ 分配     │ 自动（编译器）  │ 手动（new）     │
 *    │ 释放     │ 自动（}结束）   │ 手动（delete）  │
 *    │ 速度     │ 快（微秒级）    │ 慢（毫秒级）    │
 *    │ 大小     │ 小（1-8 MB）    │ 大（GB 级别）   │
 *    │ 安全性   │ 高（自动管理）  │ 低（易泄漏）    │
 *    │ 使用场景 │ 小对象、局部变量│ 大对象、长生命周期│
 *    └──────────┴─────────────────┴─────────────────┘
 * 
 * 
 * 3. C++ vs Java/Kotlin 内存模型
 * 
 *    Java/Kotlin:
 *      Person p = new Person();  // 总是在堆上
 *      - 对象总是在堆上
 *      - p 是引用（类似 C++ 指针）
 *      - GC 自动回收
 *      - 简单但失去控制权
 * 
 *    C++:
 *      Person p1;                // 栈上
 *      Person* p2 = new Person(); // 堆上
 *      - 可以选择栈或堆
 *      - p1 是对象本身，p2 是指针
 *      - 手动管理（delete）
 *      - 复杂但完全控制
 * 
 * 
 * 4. 使用原则
 * 
 *    ✅ 优先使用栈：
 *      - 小对象（< 1KB）
 *      - 生命周期明确的局部变量
 *      - 性能关键的代码
 * 
 *    ✅ 使用堆的场景：
 *      - 大对象（大数组、大结构）
 *      - 生命周期超出作用域
 *      - 动态大小（运行时决定）
 *      - 需要返回的对象
 * 
 *    ✅ 现代 C++ 最佳实践：
 *      - 优先用栈
 *      - 必须用堆时用智能指针（unique_ptr, shared_ptr）
 *      - 用 STL 容器（vector, string）而不是原始数组
 *      - 避免手动 new/delete
 * 
 * 
 * 5. 常见错误
 * 
 *    ❌ 内存泄漏：new 后忘记 delete
 *    ❌ 重复释放：delete 同一个指针两次
 *    ❌ 悬空指针：使用已 delete 的指针
 *    ❌ 栈溢出：栈上分配过大的数组
 *    ❌ delete/delete[] 混用
 * 
 * 
 * ========== 记忆口诀 ==========
 * 
 * 栈上快又安全，自动分配释放
 * 堆上大而灵活，手动管理要记
 * Java 全在堆上，GC 帮你收拾
 * C++ 任你选择，灵活但要小心
 * 
 * 小对象用栈，大对象用堆
 * 能用栈就栈，减少内存累
 * 智能指针护航，避免泄漏后悔
 * STL 容器为王，现代 C++ 最美
 * 
 * 
 * ========== 与 Java/Kotlin 对比表 ==========
 * 
 * ┌──────────────┬───────────────────┬──────────────────┐
 * │   特性       │   C++             │   Java/Kotlin    │
 * ├──────────────┼───────────────────┼──────────────────┤
 * │ 对象位置     │ 栈或堆（可选）    │ 堆（固定）       │
 * │ 内存管理     │ 手动（new/delete）│ 自动（GC）       │
 * │ 对象创建     │ Person p;         │ val p = Person() │
 * │              │ Person* p = new..;│ （总是堆上）     │
 * │ 指针/引用    │ 指针 + 引用       │ 只有引用         │
 * │ 生命周期     │ 作用域或手动管理  │ GC 决定          │
 * │ 性能         │ 高（零开销）      │ 较低（GC 开销）  │
 * │ 安全性       │ 低（易出错）      │ 高（GC 保证）    │
 * │ 灵活性       │ 高（完全控制）    │ 低（受限于 GC）  │
 * │ 学习曲线     │ 陡峭              │ 平缓             │
 * └──────────────┴───────────────────┴──────────────────┘
 * 
 * 
 * ========== 学习建议 ==========
 * 
 * 1. 初学阶段（现在）：
 *    - 优先使用栈（局部变量）
 *    - 理解栈和堆的区别
 *    - 避免手动 new/delete
 * 
 * 2. 进阶阶段：
 *    - 学习智能指针（unique_ptr, shared_ptr）
 *    - 理解 RAII（资源获取即初始化）
 *    - 使用 STL 容器管理动态内存
 * 
 * 3. 实际项目：
 *    - 能用栈就用栈
 *    - 必须用堆时用智能指针
 *    - 使用 valgrind 等工具检测内存泄漏
 */
