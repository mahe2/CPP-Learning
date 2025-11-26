/**
 * C++ 类型限定符和现代特性详解
 * 
 * Java 中没有的 C++ 独特特性：
 * 1. 类型限定符：const, volatile, mutable
 * 2. 现代特性：constexpr, inline 变量, thread_local
 * 
 * 为什么 Java 没有？设计哲学不同！
 */

#include <iostream>
#include <string>
#include <thread>
#include <vector>

using namespace std;

// ============================================
// 1. const - 只读保证（Java 的 final 不同）
// ============================================

void explain_const() {
    cout << "\n" << string(70, '=') << endl;
    cout << "1. const - 只读保证" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【C++ const vs Java final】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Java final:
  final int x = 10;
  x = 20;  // ❌ 编译错误

C++ const（功能更强大）:
  const int x = 10;
  x = 20;  // ❌ 编译错误
  
  const int* p1 = &x;        // 指向常量的指针
  int* const p2 = &y;        // 常量指针
  const int* const p3 = &x;  // 常量指针指向常量
  
  // Java 没有这些复杂的指针常量语义！


【const 的多种用法】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
    
    // 1. const 变量
    const int MAX_SIZE = 100;
    cout << "1. const 变量（基础）：" << endl;
    cout << "   const int MAX_SIZE = 100;" << endl;
    cout << "   值：" << MAX_SIZE << endl;
    // MAX_SIZE = 200;  // ❌ 错误
    
    // 2. const 指针（4 种组合）
    cout << "\n2. const 指针（Java 没有！）：" << endl;
    
    int a = 10, b = 20;
    
    // 指向常量的指针：数据不能改，指针可以改
    const int* ptr1 = &a;
    cout << "   const int* ptr1 = &a;  // 指向常量的指针" << endl;
    cout << "   *ptr1 = " << *ptr1 << endl;
    // *ptr1 = 30;  // ❌ 错误：不能修改指向的数据
    ptr1 = &b;      // ✅ 可以：可以改变指向
    cout << "   ptr1 = &b;  // 可以改变指向" << endl;
    
    // 常量指针：数据可以改，指针不能改
    int* const ptr2 = &a;
    cout << "\n   int* const ptr2 = &a;  // 常量指针" << endl;
    *ptr2 = 30;     // ✅ 可以：可以修改指向的数据
    cout << "   *ptr2 = 30;  // 可以修改数据" << endl;
    // ptr2 = &b;   // ❌ 错误：不能改变指向
    
    // 常量指针指向常量：都不能改
    const int* const ptr3 = &a;
    cout << "\n   const int* const ptr3 = &a;  // 都不能改" << endl;
    // *ptr3 = 40;  // ❌ 错误
    // ptr3 = &b;   // ❌ 错误
    
    // 3. const 引用（Java 没有引用的概念）
    cout << "\n3. const 引用（Java 没有！）：" << endl;
    const int& ref = a;
    cout << "   const int& ref = a;" << endl;
    cout << "   ref = " << ref << endl;
    // ref = 50;  // ❌ 错误：不能通过 const 引用修改
    
    cout << "\n【关键理解】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "Java final：" << endl;
    cout << "  - 只能修饰变量、方法、类" << endl;
    cout << "  - 变量：不能重新赋值" << endl;
    cout << "  - 方法：不能重写" << endl;
    cout << "  - 类：不能继承" << endl;
    cout << "\nC++ const：" << endl;
    cout << "  - 可以修饰变量、指针、引用、函数参数、成员函数" << endl;
    cout << "  - 指针有 4 种组合" << endl;
    cout << "  - 成员函数可以 const（承诺不修改对象）" << endl;
    cout << "  - 编译时检查，零运行时开销" << endl;
}

// ============================================
// 2. const 成员函数（Java 没有）
// ============================================

class Point {
private:
    int x, y;
    mutable int accessCount;  // mutable 稍后解释
    
public:
    Point(int x, int y) : x(x), y(y), accessCount(0) {}
    
    // const 成员函数：承诺不修改对象状态
    int getX() const {
        accessCount++;  // mutable 成员可以修改
        return x;
    }
    
    int getY() const {
        return y;
    }
    
    // 非 const 成员函数
    void setX(int newX) {
        x = newX;
    }
    
    void print() const {
        cout << "(" << x << ", " << y << ")" << endl;
    }
};

void demonstrate_const_member_function() {
    cout << "\n" << string(70, '=') << endl;
    cout << "2. const 成员函数（Java 没有！）" << endl;
    cout << string(70, '=') << endl;
    
    cout << "\n什么是 const 成员函数？" << endl;
    cout << "  - 成员函数后面加 const" << endl;
    cout << "  - 承诺不会修改对象的状态" << endl;
    cout << "  - const 对象只能调用 const 成员函数" << endl;
    
    cout << "\n示例代码：" << endl;
    cout << "  class Point {" << endl;
    cout << "      int getX() const { return x; }  // const 成员函数" << endl;
    cout << "      void setX(int v) { x = v; }     // 非 const" << endl;
    cout << "  };" << endl;
    
    cout << "\n实际演示：" << endl;
    Point p(10, 20);
    cout << "普通对象：" << endl;
    cout << "  p.getX() = " << p.getX() << endl;  // ✅ 可以
    p.setX(30);                                     // ✅ 可以
    cout << "  p.setX(30)" << endl;
    
    const Point cp(100, 200);
    cout << "\nconst 对象：" << endl;
    cout << "  cp.getX() = " << cp.getX() << endl;  // ✅ 可以调用 const 函数
    // cp.setX(300);  // ❌ 错误：不能调用非 const 函数
    cout << "  cp.setX(300);  // ❌ 错误！" << endl;
    
    cout << "\nJava 为什么没有？" << endl;
    cout << "  - Java 的方法默认可以修改对象" << endl;
    cout << "  - 没有 const 对象的概念" << endl;
    cout << "  - 依赖编程约定，而不是编译器强制" << endl;
}

// ============================================
// 3. volatile - 禁止优化（Java 有，但语义不同）
// ============================================

volatile int volatileVar = 0;  // 告诉编译器不要优化

void explain_volatile() {
    cout << "\n" << string(70, '=') << endl;
    cout << "3. volatile - 禁止优化" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【C++ volatile vs Java volatile】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
C++ volatile:
  - 告诉编译器：这个变量可能被外部改变
  - 禁止编译器优化对它的访问
  - 主要用于：硬件寄存器、信号处理、内存映射 I/O
  - 不保证多线程安全（❌ 不是线程同步！）

Java volatile:
  - 保证可见性（线程间可见）
  - 保证有序性（禁止指令重排）
  - 提供基本的线程同步
  - ✅ 是线程同步机制的一部分！

完全不同的概念！
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
    
    cout << "C++ volatile 的用途：" << endl;
    cout << "\n1. 硬件寄存器访问：" << endl;
    cout << "   volatile int* hwReg = (int*)0x40000000;" << endl;
    cout << "   *hwReg = 0xFF;  // 必须真实写入，不能优化掉" << endl;
    
    cout << "\n2. 信号处理：" << endl;
    cout << "   volatile sig_atomic_t flag = 0;" << endl;
    cout << "   void signal_handler() { flag = 1; }" << endl;
    
    cout << "\n3. 内存映射 I/O：" << endl;
    cout << "   volatile char* buffer = mmap(...);" << endl;
    
    cout << "\n【重要警告】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "❌ C++ volatile 不能用于多线程同步！" << endl;
    cout << "   volatile int counter = 0;  // ❌ 不是线程安全的！" << endl;
    cout << "   counter++;  // 多线程下会有问题" << endl;
    cout << "\n✅ 多线程请使用 std::atomic：" << endl;
    cout << "   std::atomic<int> counter(0);  // ✅ 线程安全" << endl;
    cout << "   counter++;  // 原子操作" << endl;
    
    cout << "\n为什么 C++ 不像 Java？" << endl;
    cout << "  - C++ 设计时多线程还不成熟" << endl;
    cout << "  - C++11 引入了 std::atomic 替代" << endl;
    cout << "  - volatile 保留用于硬件访问" << endl;
}

// ============================================
// 4. mutable - const 中的例外（Java 完全没有）
// ============================================

class Cache {
private:
    string data;
    mutable int hits;      // mutable：const 函数中可修改
    mutable bool cached;
    
public:
    Cache(const string& d) : data(d), hits(0), cached(false) {}
    
    // const 函数，但可以修改 mutable 成员
    string getData() const {
        hits++;  // ✅ 可以修改 mutable 成员
        cached = true;
        return data;
    }
    
    int getHits() const { return hits; }
};

void explain_mutable() {
    cout << "\n" << string(70, '=') << endl;
    cout << "4. mutable - const 中的例外" << endl;
    cout << string(70, '=') << endl;
    
    cout << "\nmutable 关键字的作用：" << endl;
    cout << "  - 允许 const 成员函数修改某些成员" << endl;
    cout << "  - 用于不影响对象逻辑状态的成员" << endl;
    cout << "  - 典型用途：缓存、统计信息、互斥锁" << endl;
    
    cout << "\n示例代码：" << endl;
    cout << "  class Cache {" << endl;
    cout << "      mutable int hits;  // 可以在 const 函数中修改" << endl;
    cout << "      string getData() const {" << endl;
    cout << "          hits++;  // ✅ 允许！" << endl;
    cout << "      }" << endl;
    cout << "  };" << endl;
    
    cout << "\n实际演示：" << endl;
    const Cache cache("important data");
    cout << "const Cache cache(\"important data\");" << endl;
    
    cout << "\n调用 const 函数 3 次：" << endl;
    cache.getData();
    cache.getData();
    cache.getData();
    
    cout << "缓存命中次数：" << cache.getHits() << endl;
    
    cout << "\n为什么需要 mutable？" << endl;
    cout << "  - 缓存不影响对象的逻辑状态" << endl;
    cout << "  - 统计信息不改变对象本质" << endl;
    cout << "  - 互斥锁需要在 const 函数中锁定" << endl;
    
    cout << "\nJava 为什么没有？" << endl;
    cout << "  - Java 没有 const 成员函数的概念" << endl;
    cout << "  - 方法默认可以修改对象" << endl;
    cout << "  - 不需要这种例外机制" << endl;
}

// ============================================
// 5. constexpr - 编译时计算（Java 没有）
// ============================================

// constexpr 变量：编译时常量
constexpr int BUFFER_SIZE = 1024;
constexpr double PI = 3.14159;

// constexpr 函数：可以在编译时执行
constexpr int square(int x) {
    return x * x;
}

constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

void explain_constexpr() {
    cout << "\n" << string(70, '=') << endl;
    cout << "5. constexpr - 编译时计算" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【constexpr vs const】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
const:
  - 运行时常量（值可能在运行时确定）
  - 只保证不可修改
  - const int x = getValue();  // 运行时才知道值

constexpr:
  - 编译时常量（必须在编译时确定）
  - 可以用于数组大小、模板参数
  - constexpr int x = 10;  // 编译时就知道
  - 更强的保证：既是 const，又是编译时常量

)" << endl;
    
    cout << "constexpr 变量：" << endl;
    cout << "  constexpr int BUFFER_SIZE = 1024;" << endl;
    cout << "  可以用作数组大小：int arr[BUFFER_SIZE];" << endl;
    
    // 编译时计算
    int arr[BUFFER_SIZE];  // ✅ 可以，因为是编译时常量
    cout << "  数组大小：" << sizeof(arr) / sizeof(int) << endl;
    
    cout << "\nconstexpr 函数（编译时执行）：" << endl;
    cout << "  constexpr int square(int x) { return x * x; }" << endl;
    
    // 编译时计算
    constexpr int result = square(10);  // 编译时计算
    cout << "  square(10) = " << result << " （编译时计算）" << endl;
    
    // 也可以运行时调用
    int runtime_value = 20;
    int runtime_result = square(runtime_value);  // 运行时计算
    cout << "  square(20) = " << runtime_result << " （运行时计算）" << endl;
    
    cout << "\n递归 constexpr 函数：" << endl;
    constexpr int fact5 = factorial(5);
    cout << "  factorial(5) = " << fact5 << " （编译时计算）" << endl;
    
    cout << "\nconstexpr 的优势：" << endl;
    cout << "  ✅ 编译时计算，零运行时开销" << endl;
    cout << "  ✅ 类型安全（不像宏）" << endl;
    cout << "  ✅ 可调试（是真正的函数）" << endl;
    cout << "  ✅ 可以用于模板元编程" << endl;
    
    cout << "\nJava 为什么没有？" << endl;
    cout << "  - Java 是纯运行时语言" << endl;
    cout << "  - JIT 编译器在运行时优化" << endl;
    cout << "  - 没有编译时计算的概念" << endl;
}

// ============================================
// 6. inline 变量（C++17，Java 没有）
// ============================================

// inline 变量可以在头文件中定义
inline int globalCounter = 0;
inline const string APP_NAME = "MyApp";

void explain_inline_variable() {
    cout << "\n" << string(70, '=') << endl;
    cout << "6. inline 变量（C++17）" << endl;
    cout << string(70, '=') << endl;
    
    cout << "\ninline 变量解决的问题：" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "传统 C++ 的问题：" << endl;
    cout << "  头文件中定义变量会导致重复定义错误" << endl;
    cout << "\n  // config.h" << endl;
    cout << "  int maxSize = 100;  // ❌ 错误！包含多次会重复定义" << endl;
    cout << "\n传统解决方案：" << endl;
    cout << "  // config.h" << endl;
    cout << "  extern int maxSize;  // 声明" << endl;
    cout << "  // config.cpp" << endl;
    cout << "  int maxSize = 100;   // 定义" << endl;
    cout << "\nC++17 inline 变量：" << endl;
    cout << "  // config.h" << endl;
    cout << "  inline int maxSize = 100;  // ✅ 可以在头文件定义！" << endl;
    
    cout << "\n实际演示：" << endl;
    cout << "  inline int globalCounter = 0;" << endl;
    cout << "  inline const string APP_NAME = \"MyApp\";" << endl;
    globalCounter++;
    cout << "  globalCounter = " << globalCounter << endl;
    cout << "  APP_NAME = " << APP_NAME << endl;
    
    cout << "\ninline 变量的特点：" << endl;
    cout << "  ✅ 可以在头文件中定义" << endl;
    cout << "  ✅ 多次包含不会重复定义" << endl;
    cout << "  ✅ 只有一个实例（所有文件共享）" << endl;
    cout << "  ✅ 替代 extern 的现代方式" << endl;
    
    cout << "\nJava 为什么不需要？" << endl;
    cout << "  - Java 没有头文件的概念" << endl;
    cout << "  - 用 public static 变量实现共享" << endl;
    cout << "  - 类加载器自动处理" << endl;
}

// ============================================
// 7. thread_local - 线程局部存储（Java 有类似的）
// ============================================

thread_local int threadId = 0;

void threadFunction(int id) {
    threadId = id;  // 每个线程有自己的副本
    cout << "线程 " << id << " 的 threadId = " << threadId << endl;
}

void explain_thread_local() {
    cout << "\n" << string(70, '=') << endl;
    cout << "7. thread_local - 线程局部存储" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【C++ thread_local vs Java ThreadLocal】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
C++ thread_local:
  thread_local int counter = 0;
  - 每个线程有自己的副本
  - 语言级别的支持
  - 类似全局变量，但每个线程独立

Java ThreadLocal:
  ThreadLocal<Integer> counter = new ThreadLocal<>();
  counter.set(0);
  - 每个线程有自己的副本
  - 库级别的支持（泛型类）
  - 需要 get/set 方法访问

相似但实现方式不同！
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
    
    cout << "thread_local 的作用：" << endl;
    cout << "  - 每个线程有自己独立的变量副本" << endl;
    cout << "  - 避免多线程竞争" << endl;
    cout << "  - 不需要加锁" << endl;
    
    cout << "\n示例代码：" << endl;
    cout << "  thread_local int threadId = 0;" << endl;
    cout << "  每个线程设置自己的 threadId，互不影响" << endl;
    
    cout << "\n创建 3 个线程演示：" << endl;
    vector<thread> threads;
    for (int i = 1; i <= 3; i++) {
        threads.emplace_back(threadFunction, i);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    cout << "\n主线程的 threadId = " << threadId << " （未被修改）" << endl;
    
    cout << "\n典型用途：" << endl;
    cout << "  - 线程 ID" << endl;
    cout << "  - 随机数生成器（每个线程独立种子）" << endl;
    cout << "  - 性能计数器" << endl;
    cout << "  - 数据库连接（每个线程独立连接）" << endl;
}

// ============================================
// 8. 综合对比表
// ============================================

void comparison_table() {
    cout << "\n" << string(70, '=') << endl;
    cout << "8. C++ vs Java 特性对比" << endl;
    cout << string(70, '=') << endl;
    
    cout << "\n特性对比表：" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "特性            C++              Java              说明" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "只读变量        const            final             C++功能更强" << endl;
    cout << "const指针       有               无                Java没指针" << endl;
    cout << "const成员函数   有               无                Java没这概念" << endl;
    cout << "禁止优化        volatile         无对应            C++用于硬件" << endl;
    cout << "线程可见性      atomic           volatile          语义不同" << endl;
    cout << "const例外       mutable          无                Java不需要" << endl;
    cout << "编译时常量      constexpr        无                Java运行时" << endl;
    cout << "头文件变量      inline(C++17)    无                Java无头文件" << endl;
    cout << "线程局部        thread_local     ThreadLocal       实现方式不同" << endl;
    cout << "---------------------------------------------------------------" << endl;
    
    cout << "\n为什么 Java 没有这些特性？" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "1. 设计哲学不同：" << endl;
    cout << "   C++: 零开销抽象，编译时优化，完全控制" << endl;
    cout << "   Java: 简化编程，运行时优化，自动管理" << endl;
    
    cout << "\n2. 内存模型不同：" << endl;
    cout << "   C++: 直接操作内存，指针，引用" << endl;
    cout << "   Java: 对象引用，垃圾回收，无指针" << endl;
    
    cout << "\n3. 编译模型不同：" << endl;
    cout << "   C++: 编译到机器码，头文件分离" << endl;
    cout << "   Java: 编译到字节码，运行时加载" << endl;
    
    cout << "\n4. 目标领域不同：" << endl;
    cout << "   C++: 系统编程，嵌入式，性能关键" << endl;
    cout << "   Java: 企业应用，Web，跨平台" << endl;
}

// ============================================
// 9. 实用建议
// ============================================

void practical_advice() {
    cout << "\n" << string(70, '=') << endl;
    cout << "9. 实用建议" << endl;
    cout << string(70, '=') << endl;
    
    cout << "\n【学习优先级】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "必须掌握：" << endl;
    cout << "  1. const 变量和指针        - 日常编程必用" << endl;
    cout << "  2. const 成员函数          - 类设计必用" << endl;
    cout << "  3. constexpr              - 性能优化必用" << endl;
    
    cout << "\n了解即可：" << endl;
    cout << "  4. mutable                - 特定场景" << endl;
    cout << "  5. inline 变量（C++17）    - 现代 C++" << endl;
    cout << "  6. thread_local           - 多线程编程" << endl;
    
    cout << "\n基本不用：" << endl;
    cout << "  7. volatile               - 嵌入式/硬件编程" << endl;
    
    cout << "\n【最佳实践】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "1. 尽可能使用 const：" << endl;
    cout << "   - 函数参数：const string& name" << endl;
    cout << "   - 成员函数：int getValue() const" << endl;
    cout << "   - 局部变量：const int MAX = 100" << endl;
    
    cout << "\n2. 编译时计算用 constexpr：" << endl;
    cout << "   - 常量：constexpr int SIZE = 1024" << endl;
    cout << "   - 函数：constexpr int square(int x)" << endl;
    
    cout << "\n3. 多线程不用 volatile：" << endl;
    cout << "   ❌ volatile int counter;  // 不是线程安全！" << endl;
    cout << "   ✅ atomic<int> counter;   // 线程安全" << endl;
    
    cout << "\n4. 头文件变量用 inline：" << endl;
    cout << "   inline const string VERSION = \"1.0\";" << endl;
    
    cout << "\n【从 Java 转 C++ 的注意点】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "1. const 不等于 final：" << endl;
    cout << "   - const 功能更强大（指针、引用、成员函数）" << endl;
    cout << "   - 需要理解指针的 const 语义" << endl;
    
    cout << "\n2. 没有 GC，但有 RAII：" << endl;
    cout << "   - 用 const 引用避免拷贝" << endl;
    cout << "   - 用智能指针管理内存" << endl;
    
    cout << "\n3. 编译时计算很强大：" << endl;
    cout << "   - constexpr 可以在编译时执行复杂计算" << endl;
    cout << "   - 性能提升巨大（零运行时开销）" << endl;
}

// ============================================
// Main
// ============================================

int main() {
    cout << "======================================================================" << endl;
    cout << "  C++ 类型限定符和现代特性详解" << endl;
    cout << "  （Java 中没有的概念）" << endl;
    cout << "======================================================================" << endl;
    
    explain_const();
    demonstrate_const_member_function();
    explain_volatile();
    explain_mutable();
    explain_constexpr();
    explain_inline_variable();
    explain_thread_local();
    comparison_table();
    practical_advice();
    
    cout << "\n======================================================================" << endl;
    cout << "核心总结" << endl;
    cout << "======================================================================" << endl;
    
    cout << "\n【类型限定符】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "1. const - 只读保证：" << endl;
    cout << "   - 比 Java final 强大得多" << endl;
    cout << "   - 支持指针、引用、成员函数" << endl;
    cout << "   - 编译时检查，零开销" << endl;
    
    cout << "\n2. volatile - 禁止优化：" << endl;
    cout << "   - 用于硬件寄存器访问" << endl;
    cout << "   - 与 Java volatile 完全不同" << endl;
    cout << "   - 多线程请用 atomic" << endl;
    
    cout << "\n3. mutable - const 例外：" << endl;
    cout << "   - 允许 const 函数修改某些成员" << endl;
    cout << "   - Java 没有这个概念" << endl;
    
    cout << "\n【现代特性】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "1. constexpr - 编译时计算：" << endl;
    cout << "   - 零运行时开销" << endl;
    cout << "   - Java 是纯运行时语言，没有这概念" << endl;
    
    cout << "\n2. inline 变量（C++17）：" << endl;
    cout << "   - 可以在头文件定义变量" << endl;
    cout << "   - Java 没有头文件" << endl;
    
    cout << "\n3. thread_local：" << endl;
    cout << "   - 线程局部存储" << endl;
    cout << "   - Java 有 ThreadLocal（类似但实现不同）" << endl;
    
    cout << "\n【为什么 Java 没有？】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "根本原因：设计哲学不同" << endl;
    cout << "  C++: 零开销抽象 + 完全控制 + 编译时优化" << endl;
    cout << "  Java: 简化编程 + 自动管理 + 运行时优化" << endl;
    
    cout << "\n【记忆要点】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "必须掌握：const, const 成员函数, constexpr" << endl;
    cout << "了解即可：mutable, inline 变量, thread_local" << endl;
    cout << "基本不用：volatile（多线程用 atomic）" << endl;
    
    cout << "======================================================================" << endl;
    
    return 0;
}

/**
 * 终极总结：
 * 
 * 1. 为什么 Java 没有这些特性？
 *    - 设计哲学：简化 vs 完全控制
 *    - 内存模型：GC vs 手动管理
 *    - 编译模型：字节码 vs 机器码
 * 
 * 2. C++ 独特优势：
 *    - const：编译时类型安全
 *    - constexpr：编译时计算
 *    - 零运行时开销
 * 
 * 3. 从 Java 转 C++：
 *    - const ≠ final
 *    - volatile ≠ 线程同步
 *    - 理解编译时 vs 运行时
 */
