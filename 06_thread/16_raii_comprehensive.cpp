/*
 * C++ RAII 模式详解
 * 证明：所有 RAII 都基于"局部变量离开作用域自动销毁"机制
 * 包括：智能指针、文件管理、锁管理、内存管理等
 */

#include <iostream>
#include <memory>
#include <fstream>
#include <vector>
#include <mutex>
#include <thread>
#include <chrono>
using namespace std;

// ============================================================================
// 1. 自定义文件 RAII 管理器
// ============================================================================
class FileRAII {
    FILE* file;
    string filename;
    
public:
    // 构造函数：获取资源（打开文件）
    FileRAII(const string& name, const string& mode) : filename(name) {
        cout << "  🏗️  [FileRAII] 构造：打开文件 " << filename << endl;
        file = fopen(name.c_str(), mode.c_str());
        if (!file) {
            throw runtime_error("无法打开文件: " + name);
        }
    }
    
    // 析构函数：释放资源（关闭文件）
    ~FileRAII() {
        if (file) {
            cout << "  🗑️  [FileRAII] 析构：关闭文件 " << filename << endl;
            fclose(file);
        }
    }
    
    // 禁止拷贝（防止重复释放）
    FileRAII(const FileRAII&) = delete;
    FileRAII& operator=(const FileRAII&) = delete;
    
    // 提供访问接口
    FILE* get() { return file; }
    
    void write(const string& content) {
        if (file) {
            fprintf(file, "%s", content.c_str());
        }
    }
};

// ============================================================================
// 2. 自定义内存 RAII 管理器
// ============================================================================
class MemoryRAII {
    void* memory;
    size_t size;
    
public:
    // 构造函数：获取资源（分配内存）
    MemoryRAII(size_t bytes) : size(bytes) {
        cout << "  🏗️  [MemoryRAII] 构造：分配 " << size << " 字节内存" << endl;
        memory = malloc(size);
        if (!memory) {
            throw bad_alloc();
        }
    }
    
    // 析构函数：释放资源（释放内存）
    ~MemoryRAII() {
        if (memory) {
            cout << "  🗑️  [MemoryRAII] 析构：释放 " << size << " 字节内存" << endl;
            free(memory);
        }
    }
    
    // 禁止拷贝
    MemoryRAII(const MemoryRAII&) = delete;
    MemoryRAII& operator=(const MemoryRAII&) = delete;
    
    // 提供访问接口
    void* get() { return memory; }
    template<typename T>
    T* as() { return static_cast<T*>(memory); }
};

// ============================================================================
// 3. 自定义线程 RAII 管理器
// ============================================================================
class ThreadRAII {
    thread t;
    string name;
    
public:
    // 构造函数：获取资源（创建线程）
    template<typename F>
    ThreadRAII(const string& thread_name, F&& func) : name(thread_name), t(forward<F>(func)) {
        cout << "  🏗️  [ThreadRAII] 构造：创建线程 " << name << endl;
    }
    
    // 析构函数：释放资源（等待线程结束）
    ~ThreadRAII() {
        if (t.joinable()) {
            cout << "  🗑️  [ThreadRAII] 析构：等待线程 " << name << " 结束" << endl;
            t.join();
        }
    }
    
    // 禁止拷贝
    ThreadRAII(const ThreadRAII&) = delete;
    ThreadRAII& operator=(const ThreadRAII&) = delete;
};

// ============================================================================
// 4. 自定义计时器 RAII 管理器
// ============================================================================
class TimerRAII {
    string operation_name;
    chrono::high_resolution_clock::time_point start_time;
    
public:
    // 构造函数：开始计时
    TimerRAII(const string& name) : operation_name(name) {
        cout << "  🏗️  [TimerRAII] 构造：开始计时 " << operation_name << endl;
        start_time = chrono::high_resolution_clock::now();
    }
    
    // 析构函数：结束计时并报告
    ~TimerRAII() {
        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
        cout << "  🗑️  [TimerRAII] 析构：" << operation_name 
             << " 耗时 " << duration.count() << " 微秒" << endl;
    }
};

// ============================================================================
// 5. 自定义资源清理器 RAII
// ============================================================================
template<typename CleanupFunc>
class ScopeGuard {
    CleanupFunc cleanup;
    bool dismissed = false;
    
public:
    // 构造函数：保存清理函数
    ScopeGuard(CleanupFunc func) : cleanup(func) {
        cout << "  🏗️  [ScopeGuard] 构造：注册清理函数" << endl;
    }
    
    // 析构函数：执行清理
    ~ScopeGuard() {
        if (!dismissed) {
            cout << "  🗑️  [ScopeGuard] 析构：执行清理函数" << endl;
            cleanup();
        }
    }
    
    // 可以取消清理
    void dismiss() { dismissed = true; }
    
    // 禁止拷贝
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
};

// 辅助函数创建 ScopeGuard
template<typename F>
auto make_scope_guard(F&& func) {
    return ScopeGuard<F>(forward<F>(func));
}

// ============================================================================
// 演示函数
// ============================================================================

void demonstrate_file_raii() {
    cout << "\n=== 1. 文件 RAII 演示 ===" << endl;
    
    cout << "\n进入作用域..." << endl;
    {
        FileRAII file("test.txt", "w");
        file.write("Hello RAII!\n");
        cout << "文件操作完成" << endl;
    }  // 文件在这里自动关闭
    cout << "离开作用域，文件已自动关闭" << endl;
}

void demonstrate_memory_raii() {
    cout << "\n=== 2. 内存 RAII 演示 ===" << endl;
    
    cout << "\n进入作用域..." << endl;
    {
        MemoryRAII mem(1024);
        int* array = mem.as<int>();
        
        // 使用内存
        for (int i = 0; i < 10; i++) {
            array[i] = i * i;
        }
        cout << "内存使用完成" << endl;
    }  // 内存在这里自动释放
    cout << "离开作用域，内存已自动释放" << endl;
}

void demonstrate_thread_raii() {
    cout << "\n=== 3. 线程 RAII 演示 ===" << endl;
    
    cout << "\n进入作用域..." << endl;
    {
        ThreadRAII worker("WorkerThread", []() {
            this_thread::sleep_for(chrono::milliseconds(100));
            cout << "    线程工作完成" << endl;
        });
        cout << "线程已启动" << endl;
    }  // 线程在这里自动 join
    cout << "离开作用域，线程已自动等待结束" << endl;
}

void demonstrate_timer_raii() {
    cout << "\n=== 4. 计时器 RAII 演示 ===" << endl;
    
    cout << "\n进入作用域..." << endl;
    {
        TimerRAII timer("复杂计算");
        
        // 模拟一些计算
        volatile int sum = 0;
        for (int i = 0; i < 100000; i++) {
            sum += i;
        }
        cout << "计算完成，结果: " << sum << endl;
    }  // 计时器在这里自动报告时间
    cout << "离开作用域，计时已自动结束" << endl;
}

void demonstrate_scope_guard() {
    cout << "\n=== 5. ScopeGuard RAII 演示 ===" << endl;
    
    cout << "\n进入作用域..." << endl;
    {
        int* temp_data = new int[10];
        cout << "分配了临时数据" << endl;
        
        // 注册清理函数
        auto guard = make_scope_guard([temp_data]() {
            delete[] temp_data;
            cout << "    清理函数：删除临时数据" << endl;
        });
        
        // 使用数据
        for (int i = 0; i < 10; i++) {
            temp_data[i] = i;
        }
        cout << "数据使用完成" << endl;
    }  // ScopeGuard 在这里自动执行清理函数
    cout << "离开作用域，数据已自动清理" << endl;
}

void demonstrate_standard_raii() {
    cout << "\n=== 6. 标准库 RAII 演示 ===" << endl;
    
    cout << "\n6.1 unique_ptr（智能指针）" << endl;
    {
        cout << "创建 unique_ptr..." << endl;
        unique_ptr<int[]> arr(new int[100]);
        
        // 使用数组
        for (int i = 0; i < 10; i++) {
            arr[i] = i;
        }
        cout << "数组使用完成" << endl;
    }  // unique_ptr 析构时自动 delete[]
    cout << "unique_ptr 自动释放了内存" << endl;
    
    cout << "\n6.2 shared_ptr（共享指针）" << endl;
    {
        cout << "创建 shared_ptr..." << endl;
        shared_ptr<vector<int>> vec = make_shared<vector<int>>(100);
        
        // 使用 vector
        (*vec)[0] = 42;
        cout << "vector 使用完成，大小: " << vec->size() << endl;
    }  // shared_ptr 引用计数为0时自动释放
    cout << "shared_ptr 自动释放了内存" << endl;
    
    cout << "\n6.3 lock_guard（锁管理）" << endl;
    mutex mtx;
    {
        cout << "创建 lock_guard..." << endl;
        lock_guard<mutex> lock(mtx);
        
        cout << "在临界区内操作" << endl;
    }  // lock_guard 析构时自动解锁
    cout << "lock_guard 自动释放了锁" << endl;
    
    cout << "\n6.4 fstream（文件流）" << endl;
    {
        cout << "创建 ofstream..." << endl;
        ofstream file("test2.txt");
        
        file << "Hello RAII from fstream!" << endl;
        cout << "文件写入完成" << endl;
    }  // ofstream 析构时自动关闭文件
    cout << "ofstream 自动关闭了文件" << endl;
}

void demonstrate_exception_safety() {
    cout << "\n=== 7. 异常安全演示 ===" << endl;
    
    cout << "\n测试异常情况下的自动清理：" << endl;
    try {
        FileRAII file("test3.txt", "w");
        MemoryRAII mem(512);
        TimerRAII timer("异常测试");
        
        auto guard = make_scope_guard([]() {
            cout << "    清理函数：处理异常后的清理工作" << endl;
        });
        
        cout << "即将抛出异常..." << endl;
        throw runtime_error("测试异常");
        
    } catch (const exception& e) {
        cout << "捕获异常: " << e.what() << endl;
    }
    // 所有 RAII 对象都会在异常处理过程中自动析构
    cout << "所有资源都已自动清理完成" << endl;
}

void demonstrate_raii_principles() {
    cout << "\n=== 8. RAII 核心原理总结 ===" << endl;
    
    cout << "\n🎯 核心机制：" << endl;
    cout << "  所有 RAII 都基于同一个 C++ 语言保证：" << endl;
    cout << "  \"局部变量离开作用域时必须自动销毁\"" << endl;
    
    cout << "\n🏗️  RAII 模式的统一结构：" << endl;
    cout << "  class ResourceManager {" << endl;
    cout << "      ResourceType resource;" << endl;
    cout << "  public:" << endl;
    cout << "      ResourceManager() {" << endl;
    cout << "          // 构造函数：获取资源" << endl;
    cout << "      }" << endl;
    cout << "      ~ResourceManager() {" << endl;
    cout << "          // 析构函数：释放资源" << endl;
    cout << "      }" << endl;
    cout << "  };" << endl;
    
    cout << "\n📋 RAII 应用场景：" << endl;
    cout << "  ✅ 内存管理：unique_ptr, shared_ptr, vector, string" << endl;
    cout << "  ✅ 文件管理：fstream, FileRAII" << endl;
    cout << "  ✅ 锁管理：lock_guard, unique_lock, shared_lock" << endl;
    cout << "  ✅ 线程管理：ThreadRAII, jthread (C++20)" << endl;
    cout << "  ✅ 网络连接：socket RAII 包装" << endl;
    cout << "  ✅ 数据库连接：connection RAII 包装" << endl;
    cout << "  ✅ 临时状态：ScopeGuard, finally 模式" << endl;
    cout << "  ✅ 计时统计：TimerRAII, ProfilerRAII" << endl;
    
    cout << "\n🔄 生命周期绑定：" << endl;
    cout << "  对象生命周期 ←→ 资源生命周期" << endl;
    cout << "  对象创建 = 资源获取" << endl;
    cout << "  对象销毁 = 资源释放" << endl;
    
    cout << "\n⚡ 为什么如此可靠？" << endl;
    cout << "  C++ 标准保证：" << endl;
    cout << "    1. 局部对象离开作用域时必须析构" << endl;
    cout << "    2. 异常时会正确进行栈展开" << endl;
    cout << "    3. 析构函数必须被调用" << endl;
    cout << "  这是语言规范，不是实现细节！" << endl;
    
    cout << "\n🎁 RAII 的优势：" << endl;
    cout << "  ✅ 自动管理：无需手动释放资源" << endl;
    cout << "  ✅ 异常安全：即使发生异常也会正确清理" << endl;
    cout << "  ✅ 简洁代码：减少样板代码" << endl;
    cout << "  ✅ 防止泄漏：资源泄漏几乎不可能" << endl;
    cout << "  ✅ 明确语义：资源生命周期一目了然" << endl;
}

int main() {
    cout << "\n╔═══════════════════════════════════════════════════╗" << endl;
    cout << "║              RAII 模式详解                       ║" << endl;
    cout << "║     基于局部变量作用域的资源自动管理              ║" << endl;
    cout << "╚═══════════════════════════════════════════════════╝" << endl;
    
    demonstrate_file_raii();
    demonstrate_memory_raii();
    demonstrate_thread_raii();
    demonstrate_timer_raii();
    demonstrate_scope_guard();
    demonstrate_standard_raii();
    demonstrate_exception_safety();
    demonstrate_raii_principles();
    
    cout << "\n🎯 结论：" << endl;
    cout << "  是的！所有 RAII 都基于同一个核心机制：" << endl;
    cout << "  \"局部变量离开作用域自动销毁\"" << endl;
    cout << "  这是 C++ 最强大的资源管理特性之一！" << endl;
    
    return 0;
}