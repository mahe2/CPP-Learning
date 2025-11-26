/**
 * RAII 完全指南
 * 
 * RAII = Resource Acquisition Is Initialization
 * 资源获取即初始化
 * 
 * 这是 C++ 最重要的设计模式！
 * Java 没有 RAII（因为有 GC）
 * 
 * 核心思想：
 * - 构造函数获取资源
 * - 析构函数释放资源
 * - 利用栈对象的自动销毁机制
 */

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <mutex>
#include <vector>
#include <chrono>

using namespace std;

// ============================================
// RAII 是什么？
// ============================================

void explain_raii() {
    cout << "\n" << string(70, '=') << endl;
    cout << "RAII - Resource Acquisition Is Initialization" << endl;
    cout << "资源获取即初始化" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【什么是 RAII？】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
RAII 是 C++ 最重要的编程范式！

核心思想：
  1. 构造函数获取资源（Acquire）
  2. 析构函数释放资源（Release）
  3. 利用栈对象的自动销毁

资源包括：
  - 动态内存（new/delete）
  - 文件句柄（open/close）
  - 网络连接（connect/disconnect）
  - 互斥锁（lock/unlock）
  - 数据库连接（connect/disconnect）

RAII 的优势：
  ✅ 自动管理资源（不会忘记释放）
  ✅ 异常安全（即使抛异常，也会释放）
  ✅ 代码简洁（无需手动 try-finally）
  ✅ 不会资源泄漏

Java 对比：
  - Java 没有 RAII
  - Java 用 try-with-resources 或 try-finally
  - Java 依赖 GC（但只管内存，不管其他资源）

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
}

// ============================================
// 反例：不使用 RAII（容易出错）
// ============================================

void bad_example_no_raii() {
    cout << "\n" << string(70, '=') << endl;
    cout << "❌ 反例：不使用 RAII（容易出错）" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【传统 C 风格（容易出错）】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void processFile() {
    FILE* file = fopen("data.txt", "r");
    if (!file) return;  // 错误处理
    
    // 处理文件...
    
    if (error) {
        // ❌ 忘记关闭文件！内存泄漏！
        return;
    }
    
    fclose(file);  // 容易忘记
}

问题：
  1. 容易忘记释放资源
  2. 多个返回点，每个都要释放
  3. 异常时不会释放（C++ 异常）
  4. 代码重复（每个退出点都要写释放代码）

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
    
    cout << "演示：手动管理资源（容易出错）：" << endl;
    
    int* data = new int[100];
    cout << "✅ 分配内存：int* data = new int[100]" << endl;
    
    // 模拟处理...
    bool error = false;
    if (error) {
        // ❌ 忘记释放！内存泄漏！
        cout << "❌ 错误退出，忘记 delete[]！内存泄漏！" << endl;
        return;
    }
    
    delete[] data;
    cout << "✅ 手动释放：delete[] data" << endl;
    cout << "\n问题：如果有多个返回点，容易忘记释放！" << endl;
}

// ============================================
// RAII 示例 1：智能指针（自动管理内存）
// ============================================

void raii_example_smart_pointer() {
    cout << "\n" << string(70, '=') << endl;
    cout << "✅ RAII 示例 1：智能指针（自动管理内存）" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【智能指针 = RAII 的完美体现】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
unique_ptr 原理：
  1. 构造函数：获取资源（new）
  2. 析构函数：释放资源（delete）
  3. 栈对象：自动调用析构函数

代码：
  {
      unique_ptr<int[]> data(new int[100]);
      // 使用 data...
  }  // 作用域结束，自动 delete[]！

优势：
  ✅ 自动释放，不会忘记
  ✅ 异常安全
  ✅ 所有权明确

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
    
    cout << "演示：智能指针自动管理：" << endl;
    
    {
        cout << "\n进入作用域..." << endl;
        auto data = make_unique<int[]>(100);
        cout << "✅ 创建 unique_ptr：auto data = make_unique<int[]>(100)" << endl;
        
        data[0] = 42;
        cout << "✅ 使用数据：data[0] = " << data[0] << endl;
        
        cout << "\n退出作用域..." << endl;
    }  // 自动调用析构函数，delete[]
    
    cout << "✅ unique_ptr 已自动释放内存！无需手动 delete" << endl;
    cout << "\n即使有异常，也会自动释放！" << endl;
}

// ============================================
// RAII 示例 2：文件管理
// ============================================

class FileGuard {
private:
    ofstream file;
    string filename;
    
public:
    // 构造函数：获取资源（打开文件）
    FileGuard(const string& name) : filename(name) {
        file.open(filename);
        cout << "  [RAII] 构造：打开文件 \"" << filename << "\"" << endl;
        if (!file.is_open()) {
            throw runtime_error("无法打开文件");
        }
    }
    
    // 析构函数：释放资源（关闭文件）
    ~FileGuard() {
        if (file.is_open()) {
            file.close();
            cout << "  [RAII] 析构：关闭文件 \"" << filename << "\"" << endl;
        }
    }
    
    // 禁止拷贝
    FileGuard(const FileGuard&) = delete;
    FileGuard& operator=(const FileGuard&) = delete;
    
    void write(const string& data) {
        file << data << endl;
    }
};

void raii_example_file() {
    cout << "\n" << string(70, '=') << endl;
    cout << "✅ RAII 示例 2：文件管理" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【文件管理 RAII】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
传统方式（容易出错）：
  FILE* f = fopen("data.txt", "w");
  // 处理...
  fclose(f);  // 容易忘记

RAII 方式：
  class FileGuard {
      FileGuard(name) { file.open(name); }  // 构造打开
      ~FileGuard() { file.close(); }         // 析构关闭
  };
  
  {
      FileGuard f("data.txt");
      f.write("hello");
  }  // 自动关闭！

优势：
  ✅ 无论如何退出（正常、return、异常），都会关闭文件
  ✅ 不会忘记关闭
  ✅ 异常安全

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
    
    cout << "演示：RAII 文件管理：" << endl;
    
    try {
        cout << "\n进入作用域..." << endl;
        FileGuard file("/tmp/raii_test.txt");
        
        cout << "✅ 写入数据..." << endl;
        file.write("Hello RAII!");
        file.write("Automatic resource management!");
        
        cout << "\n退出作用域..." << endl;
    } catch (const exception& e) {
        cout << "❌ 异常：" << e.what() << endl;
    }
    
    cout << "✅ 文件已自动关闭！" << endl;
}

// ============================================
// RAII 示例 3：互斥锁（线程安全）
// ============================================

class LockGuard {
private:
    mutex& mtx;
    
public:
    // 构造函数：获取锁
    LockGuard(mutex& m) : mtx(m) {
        mtx.lock();
        cout << "  [RAII] 构造：获取锁" << endl;
    }
    
    // 析构函数：释放锁
    ~LockGuard() {
        mtx.unlock();
        cout << "  [RAII] 析构：释放锁" << endl;
    }
    
    // 禁止拷贝
    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;
};

mutex g_mutex;
int g_counter = 0;

void raii_example_lock() {
    cout << "\n" << string(70, '=') << endl;
    cout << "✅ RAII 示例 3：互斥锁管理" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【互斥锁 RAII】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
传统方式（容易死锁）：
  mutex.lock();
  // 处理共享数据...
  if (error) {
      return;  // ❌ 忘记 unlock！死锁！
  }
  mutex.unlock();

RAII 方式（lock_guard / unique_lock）：
  {
      lock_guard<mutex> lock(mtx);  // 构造时 lock
      // 处理共享数据...
  }  // 析构时 unlock，即使异常也会 unlock！

标准库提供：
  - lock_guard<mutex>      // 简单 RAII 锁
  - unique_lock<mutex>     // 灵活 RAII 锁
  - scoped_lock<mutex...>  // C++17，多锁

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
    
    cout << "演示：RAII 锁管理：" << endl;
    
    {
        cout << "\n进入临界区..." << endl;
        LockGuard lock(g_mutex);  // 自动获取锁
        
        cout << "✅ 修改共享数据：g_counter = " << ++g_counter << endl;
        
        cout << "\n退出临界区..." << endl;
    }  // 自动释放锁
    
    cout << "✅ 锁已自动释放！" << endl;
    
    cout << "\n标准库示例：" << endl;
    {
        lock_guard<mutex> lock(g_mutex);  // 标准库的 RAII 锁
        cout << "✅ 使用 lock_guard：g_counter = " << ++g_counter << endl;
    }
}

// ============================================
// RAII 示例 4：计时器
// ============================================

class Timer {
private:
    string name;
    chrono::time_point<chrono::high_resolution_clock> start;
    
public:
    // 构造函数：开始计时
    Timer(const string& n) : name(n) {
        start = chrono::high_resolution_clock::now();
        cout << "  [RAII] 计时器 \"" << name << "\" 开始" << endl;
    }
    
    // 析构函数：结束计时并输出
    ~Timer() {
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "  [RAII] 计时器 \"" << name << "\" 结束: " 
             << duration.count() << " 微秒" << endl;
    }
};

void raii_example_timer() {
    cout << "\n" << string(70, '=') << endl;
    cout << "✅ RAII 示例 4：自动计时器" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【计时器 RAII】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
传统方式：
  auto start = now();
  // 执行代码...
  auto end = now();
  cout << end - start;  // 容易忘记

RAII 方式：
  {
      Timer t("operation");
      // 执行代码...
  }  // 自动输出耗时！

优势：
  ✅ 自动计时
  ✅ 不会忘记
  ✅ 代码简洁

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
    
    cout << "演示：自动计时：" << endl;
    
    {
        Timer timer("排序操作");
        
        // 模拟耗时操作
        vector<int> data(1000);
        for (int i = 0; i < 1000; ++i) {
            data[i] = 1000 - i;
        }
        sort(data.begin(), data.end());
        
    }  // 自动输出耗时
}

// ============================================
// RAII vs Java 对比
// ============================================

void raii_vs_java() {
    cout << "\n" << string(70, '=') << endl;
    cout << "RAII vs Java 资源管理对比" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【C++ RAII vs Java 资源管理】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

C++ RAII：
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
{
    FileGuard file("data.txt");     // 构造时打开
    file.write("hello");
}  // 析构时自动关闭

特点：
  ✅ 利用栈对象自动销毁
  ✅ 构造获取，析构释放
  ✅ 零开销抽象
  ✅ 异常安全
  ✅ 编译器保证调用析构

Java try-with-resources (JDK 7+)：
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
try (FileWriter file = new FileWriter("data.txt")) {
    file.write("hello");
}  // 自动调用 close()

特点：
  ✅ 自动关闭资源（实现 AutoCloseable）
  ✅ 异常安全
  ❌ 需要 try 语法糖
  ❌ 只能管理实现 AutoCloseable 的类
  ❌ 运行时开销

Java 传统方式（JDK 7 之前）：
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
FileWriter file = null;
try {
    file = new FileWriter("data.txt");
    file.write("hello");
} finally {
    if (file != null) {
        file.close();  // 手动关闭
    }
}

特点：
  ❌ 代码冗长
  ❌ 容易出错
  ❌ 需要手动在 finally 中释放

内存管理对比：
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
C++ RAII：
  {
      unique_ptr<int[]> data(new int[1000]);
      // 使用 data...
  }  // 自动 delete[]

Java GC：
  int[] data = new int[1000];
  // 使用 data...
  // GC 自动回收（但不知道何时）

关键区别：
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

C++ RAII:
  ✅ 确定性销毁（离开作用域立即销毁）
  ✅ 可管理任何资源（内存、文件、锁、网络...）
  ✅ 零开销（栈对象）
  ✅ 编译时保证
  ❌ 需要理解栈/堆、构造/析构

Java:
  ✅ 内存自动管理（GC）
  ✅ 简单易用
  ❌ 非确定性销毁（GC 不知道何时运行）
  ❌ 非内存资源需要手动管理（try-with-resources）
  ❌ GC 暂停（STW）
  ❌ 运行时开销

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
}

// ============================================
// RAII 最佳实践
// ============================================

void raii_best_practices() {
    cout << "\n" << string(70, '=') << endl;
    cout << "RAII 最佳实践" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【RAII 最佳实践】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

1️⃣  优先使用标准库的 RAII 类
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
✅ 内存：unique_ptr, shared_ptr, vector, string
✅ 文件：fstream, ifstream, ofstream
✅ 锁：lock_guard, unique_lock, scoped_lock
✅ 线程：thread（需要 join 或 detach）

❌ 避免：
  - 原始指针（new/delete）
  - 手动资源管理
  - malloc/free

2️⃣  自定义 RAII 类的规则
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class ResourceGuard {
public:
    ResourceGuard()  { acquire(); }   // 构造获取
    ~ResourceGuard() { release(); }   // 析构释放
    
    // 禁止拷贝（资源唯一）
    ResourceGuard(const ResourceGuard&) = delete;
    ResourceGuard& operator=(const ResourceGuard&) = delete;
    
    // 可选：支持移动
    ResourceGuard(ResourceGuard&&) noexcept;
};

3️⃣  异常安全
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
✅ 析构函数不应该抛异常
  ~MyClass() noexcept { ... }

✅ 即使构造失败，也不会泄漏
  MyClass::MyClass() {
      ptr1 = new int;  // 如果这里成功
      ptr2 = new int;  // 这里失败，ptr1 会泄漏！
      // 解决：用 unique_ptr
  }

4️⃣  栈优先原则
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
✅ 优先：
  void func() {
      MyClass obj;  // 栈对象，RAII 自动管理
  }

❌ 避免：
  void func() {
      MyClass* obj = new MyClass();  // 需要手动 delete
  }

5️⃣  RAII + 移动语义
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
unique_ptr<File> openFile(const string& name) {
    auto file = make_unique<File>(name);
    return file;  // 移动返回，不拷贝
}

auto file = openFile("data.txt");  // 所有权转移

6️⃣  常见 RAII 模式
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
内存管理：   unique_ptr, shared_ptr
文件管理：   fstream
锁管理：     lock_guard, unique_lock
计时器：     自定义 Timer
作用域保护： 自定义 ScopeGuard
数组管理：   vector（不用原始数组）
字符串：     string（不用 char*）

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
}

// ============================================
// Main
// ============================================

int main() {
    cout << "======================================================================" << endl;
    cout << "  RAII - C++ 最重要的设计模式" << endl;
    cout << "  Resource Acquisition Is Initialization" << endl;
    cout << "======================================================================" << endl;
    
    explain_raii();
    bad_example_no_raii();
    raii_example_smart_pointer();
    raii_example_file();
    raii_example_lock();
    raii_example_timer();
    raii_vs_java();
    raii_best_practices();
    
    cout << "\n======================================================================" << endl;
    cout << "核心总结" << endl;
    cout << "======================================================================" << endl;
    
    cout << R"(
【RAII 核心总结】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

什么是 RAII？
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
RAII = Resource Acquisition Is Initialization
资源获取即初始化

核心思想：
  1. 构造函数：获取资源（new, open, lock）
  2. 析构函数：释放资源（delete, close, unlock）
  3. 栈对象：离开作用域自动调用析构

为什么重要？
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
✅ 自动管理资源（不会忘记释放）
✅ 异常安全（即使异常，也会释放）
✅ 代码简洁（无需 try-finally）
✅ 确定性销毁（离开作用域立即销毁）
✅ 零开销抽象

RAII 实例：
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
1. 智能指针：unique_ptr, shared_ptr
   {
       auto ptr = make_unique<int>(42);
   }  // 自动 delete

2. 文件流：fstream
   {
       ofstream file("data.txt");
       file << "hello";
   }  // 自动 close

3. 互斥锁：lock_guard
   {
       lock_guard<mutex> lock(mtx);
       // 临界区
   }  // 自动 unlock

4. 容器：vector, string
   {
       vector<int> v(1000);
   }  // 自动释放内存

C++ RAII vs Java：
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
C++ RAII:
  - 确定性销毁（立即）
  - 管理任何资源
  - 零开销（栈对象）
  - 编译时保证

Java:
  - GC 管理内存（非确定性）
  - try-with-resources 管理其他资源
  - 运行时开销
  - 需要语法糖

最佳实践：
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
1️⃣  优先栈对象（自动 RAII）
2️⃣  用智能指针（不用原始指针）
3️⃣  用标准容器（不用原始数组）
4️⃣  用 fstream（不用 FILE*）
5️⃣  用 lock_guard（不用手动 lock/unlock）

记忆口诀：
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
"构造获取，析构释放"
"栈对象，自动管理"
"离开作用域，立即销毁"
"异常安全，不会泄漏"

RAII 是 C++ 与众不同的核心特性！
这是 Java 无法模仿的（因为 Java 对象在堆上，依赖 GC）
C++ 因为有栈对象，所以可以实现 RAII！

======================================================================
)" << endl;
    
    return 0;
}

/**
 * 终极总结：
 * 
 * RAII = Resource Acquisition Is Initialization
 * 
 * 为什么 RAII 重要？
 *   1. C++ 最核心的设计模式
 *   2. 自动资源管理（不会忘记释放）
 *   3. 异常安全（即使异常也会释放）
 *   4. 确定性销毁（离开作用域立即销毁）
 *   5. 零开销抽象
 * 
 * Java 为什么没有 RAII？
 *   1. Java 对象总在堆上（无法利用栈的自动销毁）
 *   2. 依赖 GC（非确定性销毁）
 *   3. 用 try-with-resources 替代（语法糖）
 * 
 * C++ RAII 实例：
 *   - 智能指针：unique_ptr, shared_ptr
 *   - 文件流：fstream
 *   - 互斥锁：lock_guard
 *   - 容器：vector, string
 * 
 * 关键理解：
 *   RAII 是 C++ 区别于其他语言的核心特性
 *   因为 C++ 有栈对象 + 确定性析构
 */
