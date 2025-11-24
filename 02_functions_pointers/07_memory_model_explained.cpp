/*
 * C++ 内存模型 - 通俗易懂版
 * 用生活中的类比帮助理解和记忆
 */

#include <iostream>
#include <string>

using namespace std;

// ========== 用图书馆类比理解内存模型 ==========

/*
 * 把程序想象成一个图书馆：
 * 
 * ┌─────────────────────────────────────────────────────┐
 * │  1. 代码区（Code/Text）                              │
 * │     类比：图书馆的书籍                               │
 * │     作用：存储程序代码（函数、类定义等）              │
 * │     特点：只读，不能修改                             │
 * │     例子：你写的 main() 函数、类的成员函数           │
 * ├─────────────────────────────────────────────────────┤
 * │  2. 全局/静态存储区（Global/Static）                 │
 * │     类比：图书馆的公告栏                             │
 * │     作用：存储全局变量、静态变量                      │
 * │     特点：程序启动时分配，结束时释放                  │
 * │     例子：全局变量、static 变量                      │
 * ├─────────────────────────────────────────────────────┤
 * │  3. 常量区（Constant）                               │
 * │     类比：图书馆的标语牌                             │
 * │     作用：存储字符串常量、const 常量                  │
 * │     特点：只读，不能修改                             │
 * │     例子："Hello World"、const 常量                  │
 * ├─────────────────────────────────────────────────────┤
 * │  4. 栈区（Stack）↓                                   │
 * │     类比：图书馆的借书登记本                          │
 * │     作用：存储局部变量、函数参数、函数调用信息         │
 * │     特点：自动分配和释放（LIFO：后进先出）            │
 * │     例子：函数内的 int x = 10;                       │
 * ├─────────────────────────────────────────────────────┤
 * │  5. 堆区（Heap）↑                                    │
 * │     类比：图书馆的储藏室                             │
 * │     作用：存储动态分配的对象                          │
 * │     特点：手动分配（new）和释放（delete）             │
 * │     例子：int* p = new int(10);                      │
 * └─────────────────────────────────────────────────────┘
 */

// ========== 第一部分：5 个区域的详细解释 ==========

// 全局变量 → 存在全局/静态区
int globalCounter = 0;
const int MAX_SIZE = 100;  // 存在常量区

void explainMemoryRegions() {
    cout << "=== 5 个内存区域详解 ===" << endl;
    
    // ===== 1. 代码区 =====
    cout << "\n【1. 代码区（Code/Text Segment）】" << endl;
    cout << "作用：存储程序代码（机器指令）" << endl;
    cout << "特点：" << endl;
    cout << "  - 只读（不能在运行时修改代码）" << endl;
    cout << "  - 所有函数、类的成员函数都在这里" << endl;
    cout << "  - 程序启动时加载到内存" << endl;
    cout << "例子：" << endl;
    cout << "  void func() { ... }  // 这个函数的代码在代码区" << endl;
    cout << "  class A { void f(); }  // f() 的代码在代码区" << endl;
    
    cout << "\n类比：" << endl;
    cout << "  就像书的内容，你可以阅读（执行），但不能修改" << endl;
    
    // ===== 2. 全局/静态区 =====
    cout << "\n\n【2. 全局/静态存储区（Global/Static）】" << endl;
    cout << "作用：存储全局变量和静态变量" << endl;
    cout << "特点：" << endl;
    cout << "  - 程序启动时分配，结束时释放" << endl;
    cout << "  - 整个程序期间都存在" << endl;
    cout << "  - 未初始化的变量自动初始化为 0" << endl;
    
    static int staticVar = 0;  // 存在静态区
    staticVar++;
    
    cout << "例子：" << endl;
    cout << "  int globalCounter = 0;  // 全局变量" << endl;
    cout << "  static int staticVar;    // 静态变量" << endl;
    cout << "  当前 staticVar = " << staticVar << endl;
    
    cout << "\n类比：" << endl;
    cout << "  就像公告栏，从开馆到闭馆一直在那里" << endl;
    
    // ===== 3. 常量区 =====
    cout << "\n\n【3. 常量区（Constant/Read-Only）】" << endl;
    cout << "作用：存储字符串常量和 const 全局常量" << endl;
    cout << "特点：" << endl;
    cout << "  - 只读，不能修改" << endl;
    cout << "  - 字符串字面量在这里" << endl;
    
    const char* str = "Hello";  // "Hello" 存在常量区
    
    cout << "例子：" << endl;
    cout << "  const char* str = \"Hello\";  // \"Hello\" 在常量区" << endl;
    cout << "  const int MAX = 100;          // MAX 在常量区" << endl;
    cout << "  str 指向常量区：" << (void*)str << endl;
    
    cout << "\n类比：" << endl;
    cout << "  就像墙上的标语牌，只能看不能改" << endl;
    
    // ===== 4. 栈区 =====
    cout << "\n\n【4. 栈区（Stack）】" << endl;
    cout << "作用：存储局部变量、函数参数、返回地址" << endl;
    cout << "特点：" << endl;
    cout << "  - 自动分配和释放（进入/离开作用域）" << endl;
    cout << "  - 后进先出（LIFO）" << endl;
    cout << "  - 速度快，但空间有限（1-8 MB）" << endl;
    
    int stackVar = 42;  // 栈上
    
    cout << "例子：" << endl;
    cout << "  int x = 10;        // 栈上" << endl;
    cout << "  string s = \"abc\";  // 对象本身在栈" << endl;
    cout << "  stackVar 地址：" << &stackVar << endl;
    
    cout << "\n类比：" << endl;
    cout << "  就像借书登记本，借书时写上，还书时擦掉" << endl;
    
    // ===== 5. 堆区 =====
    cout << "\n\n【5. 堆区（Heap）】" << endl;
    cout << "作用：存储动态分配的对象（new）" << endl;
    cout << "特点：" << endl;
    cout << "  - 手动分配（new）和释放（delete）" << endl;
    cout << "  - 空间大，但速度较慢" << endl;
    cout << "  - 容易内存泄漏" << endl;
    
    int* heapVar = new int(99);  // 堆上
    
    cout << "例子：" << endl;
    cout << "  int* p = new int(10);  // 堆上" << endl;
    cout << "  heapVar 指向地址：" << heapVar << endl;
    cout << "  必须手动 delete！" << endl;
    
    delete heapVar;
    
    cout << "\n类比：" << endl;
    cout << "  就像储藏室，你自己管理物品进出" << endl;
}

// ========== 第二部分：实际代码演示 ==========

// 全局变量 - 全局/静态区
int g_var = 100;

// 全局常量 - 常量区
const int GLOBAL_CONST = 200;

void demonstrateMemoryWithCode() {
    cout << "\n\n=== 实际代码演示 ===" << endl;
    
    // 静态变量 - 全局/静态区
    static int s_var = 300;
    
    // 局部变量 - 栈区
    int local_var = 400;
    
    // 动态分配 - 堆区
    int* heap_var = new int(500);
    
    // 字符串常量 - 常量区
    const char* str_const = "Hello World";
    
    cout << "\n各区域的实际地址：" << endl;
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    
    // 代码区（函数地址）
    cout << "【代码区】" << endl;
    cout << "  函数地址:          " << (void*)&demonstrateMemoryWithCode << endl;
    cout << "  explainMemoryRegions: " << (void*)&explainMemoryRegions << endl;
    
    cout << "\n【全局/静态区】" << endl;
    cout << "  全局变量 g_var:    " << &g_var << endl;
    cout << "  静态变量 s_var:    " << &s_var << endl;
    
    cout << "\n【常量区】" << endl;
    cout << "  全局常量:          " << &GLOBAL_CONST << endl;
    cout << "  字符串常量:        " << (void*)str_const << endl;
    
    cout << "\n【栈区】" << endl;
    cout << "  局部变量 local_var:" << &local_var << endl;
    
    cout << "\n【堆区】" << endl;
    cout << "  动态变量 heap_var: " << heap_var << endl;
    
    cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    cout << "观察：地址从低到高排列，符合内存布局！" << endl;
    
    delete heap_var;
}

// ========== 第三部分：记忆技巧 ==========

void memoryMnemonics() {
    cout << "\n\n=== 记忆技巧 ===" << endl;
    
    cout << "\n【方法1：生活类比】" << endl;
    cout << "把程序想象成一个图书馆：" << endl;
    cout << "  代码区 = 书架上的书（只能读）" << endl;
    cout << "  常量区 = 墙上的标语（只能看）" << endl;
    cout << "  全局区 = 公告栏（一直在）" << endl;
    cout << "  栈区   = 借书登记本（临时的）" << endl;
    cout << "  堆区   = 储藏室（自己管理）" << endl;
    
    cout << "\n【方法2：按生命周期记】" << endl;
    cout << "  程序全程：代码区、常量区、全局/静态区" << endl;
    cout << "  作用域内：栈区（自动）" << endl;
    cout << "  手动管理：堆区（new/delete）" << endl;
    
    cout << "\n【方法3：按用途记】" << endl;
    cout << "  代码存哪？   → 代码区" << endl;
    cout << "  常量存哪？   → 常量区" << endl;
    cout << "  全局变量？   → 全局/静态区" << endl;
    cout << "  局部变量？   → 栈区" << endl;
    cout << "  动态分配？   → 堆区" << endl;
    
    cout << "\n【方法4：口诀】" << endl;
    cout << "  代码只读不可改，" << endl;
    cout << "  常量字符串不变，" << endl;
    cout << "  全局静态伴始终，" << endl;
    cout << "  栈上自动进出快，" << endl;
    cout << "  堆上手动要小心。" << endl;
}

// ========== 第四部分：对比 Java/Kotlin ==========

void compareWithJavaKotlin() {
    cout << "\n\n=== 对比 Java/Kotlin ===" << endl;
    
    cout << "\n【Java/Kotlin 的内存模型（简化版）】" << endl;
    cout << "┌────────────────────────┐" << endl;
    cout << "│  方法区（Method Area）  │ ← 类信息、常量池" << endl;
    cout << "├────────────────────────┤" << endl;
    cout << "│  堆（Heap）             │ ← 所有对象" << endl;
    cout << "├────────────────────────┤" << endl;
    cout << "│  栈（Stack）            │ ← 局部变量、方法调用" << endl;
    cout << "└────────────────────────┘" << endl;
    
    cout << "\n【主要区别】" << endl;
    cout << "C++:" << endl;
    cout << "  - 对象可以在栈或堆上" << endl;
    cout << "  - 程序员决定内存位置" << endl;
    cout << "  - 手动管理堆内存" << endl;
    
    cout << "\nJava/Kotlin:" << endl;
    cout << "  - 对象总是在堆上" << endl;
    cout << "  - JVM 自动决定" << endl;
    cout << "  - GC 自动回收" << endl;
    
    cout << "\n【代码对比】" << endl;
    cout << "Java:" << endl;
    cout << "  int x = 10;              // 栈（基本类型）" << endl;
    cout << "  Person p = new Person(); // 堆（对象）" << endl;
    cout << "  // GC 自动回收" << endl;
    
    cout << "\nC++:" << endl;
    cout << "  int x = 10;              // 栈" << endl;
    cout << "  Person p1;               // 栈（对象本身）" << endl;
    cout << "  Person* p2 = new Person();  // 堆" << endl;
    cout << "  delete p2;  // 必须手动释放" << endl;
}

// ========== 第五部分：常见问题 ==========

void commonQuestions() {
    cout << "\n\n=== 常见问题 ===" << endl;
    
    cout << "\n【Q1：为什么要分这么多区？】" << endl;
    cout << "A: 不同数据有不同的生命周期和访问方式" << endl;
    cout << "   代码：永远不变，可以共享" << endl;
    cout << "   全局：整个程序都要用" << endl;
    cout << "   栈：临时数据，自动管理快速" << endl;
    cout << "   堆：灵活但需要手动管理" << endl;
    
    cout << "\n【Q2：string 存在哪里？】" << endl;
    cout << "A: string 对象本身在栈或堆（看怎么创建）" << endl;
    cout << "   但内部存储的字符数据在堆上" << endl;
    
    string s1 = "hello";           // s1 对象在栈，数据在堆
    string* s2 = new string("hi"); // s2 指针在栈，对象在堆，数据也在堆
    
    cout << "   string s1: 对象在栈，数据在堆" << endl;
    cout << "   string* s2 = new string(): 对象在堆，数据也在堆" << endl;
    
    delete s2;
    
    cout << "\n【Q3：vector 存在哪里？】" << endl;
    cout << "A: 同理，vector 对象可以在栈或堆" << endl;
    cout << "   但内部数组总是在堆上" << endl;
    
    cout << "\n【Q4：函数内的 static 变量存在哪？】" << endl;
    cout << "A: 全局/静态区（不是栈！）" << endl;
    
    static int count = 0;
    count++;
    
    cout << "   static int count; // 在全局/静态区" << endl;
    cout << "   调用 " << count << " 次，值会保留" << endl;
    
    cout << "\n【Q5：const 变量存在哪？】" << endl;
    cout << "A: 要看是哪种 const：" << endl;
    cout << "   const int x = 10;           // 栈（局部）" << endl;
    cout << "   const int GLOBAL = 100;     // 常量区（全局）" << endl;
    cout << "   const char* s = \"hello\";   // s 在栈，\"hello\" 在常量区" << endl;
}

// ========== 第六部分：实战练习 ==========

void practiceExercises() {
    cout << "\n\n=== 实战练习 ===" << endl;
    
    cout << "\n【练习：判断下列变量在哪个区】" << endl;
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    
    // 1. 全局变量
    cout << "\n1. int globalVar = 10;" << endl;
    cout << "   答案：全局/静态区" << endl;
    
    // 2. 局部变量
    cout << "\n2. void func() {" << endl;
    cout << "       int localVar = 20;" << endl;
    cout << "   }" << endl;
    cout << "   答案：栈区" << endl;
    
    // 3. 动态分配
    cout << "\n3. int* p = new int(30);" << endl;
    cout << "   答案：p 在栈区，*p（30）在堆区" << endl;
    
    // 4. 字符串常量
    cout << "\n4. const char* str = \"Hello\";" << endl;
    cout << "   答案：str 在栈区，\"Hello\" 在常量区" << endl;
    
    // 5. 静态局部变量
    cout << "\n5. void func() {" << endl;
    cout << "       static int count = 0;" << endl;
    cout << "   }" << endl;
    cout << "   答案：全局/静态区（不是栈！）" << endl;
    
    // 6. 对象
    cout << "\n6. string s = \"test\";" << endl;
    cout << "   答案：s 对象在栈，内部数据在堆" << endl;
    
    cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
}

int main() {
    cout << "=== C++ 内存模型 - 通俗易懂版 ===" << endl;
    
    // 1. 5 个区域详解
    explainMemoryRegions();
    
    // 2. 实际代码演示
    demonstrateMemoryWithCode();
    
    // 3. 记忆技巧
    memoryMnemonics();
    
    // 4. 对比 Java/Kotlin
    compareWithJavaKotlin();
    
    // 5. 常见问题
    commonQuestions();
    
    // 6. 实战练习
    practiceExercises();
    
    return 0;
}

/*
 * ========== 最终总结 ==========
 * 
 * 1. 五大内存区域
 * 
 *    ┌─────────────┬──────────────┬──────────────┬─────────────┐
 *    │   区域      │   存储内容   │   生命周期   │   可修改？  │
 *    ├─────────────┼──────────────┼──────────────┼─────────────┤
 *    │ 代码区      │ 程序代码     │ 程序运行期间 │ ❌ 只读     │
 *    │ 常量区      │ 字符串常量   │ 程序运行期间 │ ❌ 只读     │
 *    │ 全局/静态区 │ 全局/静态变量│ 程序运行期间 │ ✅ 可修改   │
 *    │ 栈区        │ 局部变量     │ 作用域内     │ ✅ 可修改   │
 *    │ 堆区        │ new 分配     │ delete 前    │ ✅ 可修改   │
 *    └─────────────┴──────────────┴──────────────┴─────────────┘
 * 
 * 
 * 2. 生活类比（最好记！）
 * 
 *    图书馆类比：
 *    代码区 = 书架上的书（只能读）
 *    常量区 = 墙上的标语（只能看）
 *    全局区 = 公告栏（一直在）
 *    栈区   = 借书登记本（临时的）
 *    堆区   = 储藏室（自己管理）
 * 
 * 
 * 3. 记忆口诀
 * 
 *    代码只读不可改，（代码区）
 *    常量字符串不变，（常量区）
 *    全局静态伴始终，（全局/静态区）
 *    栈上自动进出快，（栈区）
 *    堆上手动要小心。（堆区）
 * 
 * 
 * 4. 快速判断法
 * 
 *    看到什么          →  在哪个区
 *    ────────────────────────────────
 *    函数、类的方法    →  代码区
 *    "Hello"           →  常量区
 *    const 全局变量    →  常量区
 *    全局变量          →  全局/静态区
 *    static 变量       →  全局/静态区
 *    int x = 10;       →  栈区（函数内）
 *    new ...           →  堆区
 * 
 * 
 * 5. 与 Java/Kotlin 对比
 * 
 *    C++:
 *      ✅ 5 个区域，完全控制
 *      ✅ 对象可以在栈或堆
 *      ⚠️  手动管理，容易出错
 * 
 *    Java/Kotlin:
 *      ✅ 简化为 3 个区域
 *      ✅ 对象总是在堆
 *      ✅ GC 自动管理
 *      ⚠️  失去部分控制权
 * 
 * 
 * 6. 实用建议
 * 
 *    初学阶段：
 *      - 理解 5 个区域的作用
 *      - 重点掌握栈和堆的区别
 *      - 能判断变量在哪个区
 * 
 *    实际开发：
 *      - 优先使用栈（自动管理）
 *      - 必须用堆时用智能指针
 *      - 避免全局变量（不好维护）
 */
