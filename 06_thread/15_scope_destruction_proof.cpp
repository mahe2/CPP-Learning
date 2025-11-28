/*
 * 验证 lock_guard 的自动销毁原理
 * 证明：局部变量离开作用域自动销毁 + 构造析构函数配对调用
 */

#include <iostream>
#include <mutex>
#include <thread>
using namespace std;

// 自定义 lock_guard 来观察构造和析构过程
class MyLockGuard {
    mutex& m_mutex;
    string m_name;
    
public:
    // 构造函数：获取资源
    MyLockGuard(mutex& mtx, const string& name) : m_mutex(mtx), m_name(name) {
        cout << "  🏗️  [" << m_name << "] 构造函数调用 - 获取锁" << endl;
        m_mutex.lock();
    }
    
    // 析构函数：释放资源
    ~MyLockGuard() {
        cout << "  🗑️  [" << m_name << "] 析构函数调用 - 释放锁" << endl;
        m_mutex.unlock();
    }
};

mutex demo_mutex;

void demonstrate_scope_destruction() {
    cout << "\n=== 1. 作用域自动销毁验证 ===" << endl;
    
    cout << "\n示例1: 基本作用域" << endl;
    {
        cout << "进入作用域..." << endl;
        MyLockGuard guard(demo_mutex, "Guard1");
        cout << "在作用域内执行代码..." << endl;
    }  // 离开作用域，guard 自动销毁
    cout << "已离开作用域" << endl;
    
    cout << "\n示例2: 函数作用域" << endl;
    auto func = []() {
        cout << "进入函数..." << endl;
        MyLockGuard guard(demo_mutex, "Guard2");
        cout << "函数内执行..." << endl;
        return 42;
        // 函数结束，guard 自动销毁
    };
    
    int result = func();
    cout << "函数返回: " << result << endl;
}

void demonstrate_early_return() {
    cout << "\n=== 2. 提前return的自动销毁 ===" << endl;
    
    auto func = [](bool early_return) {
        cout << "函数开始..." << endl;
        MyLockGuard guard(demo_mutex, "EarlyReturn");
        
        if (early_return) {
            cout << "提前返回..." << endl;
            return;  // 提前返回，guard 也会自动析构
        }
        
        cout << "正常执行..." << endl;
        // 正常结束，guard 自动析构
    };
    
    cout << "\n测试提前返回：" << endl;
    func(true);
    
    cout << "\n测试正常返回：" << endl;
    func(false);
}

void demonstrate_exception_safety() {
    cout << "\n=== 3. 异常情况的自动销毁 ===" << endl;
    
    auto func = [](bool throw_exception) {
        cout << "函数开始..." << endl;
        
        try {
            MyLockGuard guard(demo_mutex, "Exception");
            
            if (throw_exception) {
                cout << "即将抛出异常..." << endl;
                throw runtime_error("测试异常");
            }
            
            cout << "正常执行完成" << endl;
        } catch (const exception& e) {
            cout << "捕获异常: " << e.what() << endl;
        }
        // guard 在异常处理过程中已经自动析构
    };
    
    cout << "\n测试异常情况：" << endl;
    func(true);
    
    cout << "\n测试正常情况：" << endl;
    func(false);
}

void demonstrate_nested_scopes() {
    cout << "\n=== 4. 嵌套作用域的销毁顺序 ===" << endl;
    
    cout << "外层作用域开始..." << endl;
    {
        MyLockGuard outer_guard(demo_mutex, "Outer");
        cout << "创建外层 guard" << endl;
        
        {
            MyLockGuard inner_guard(demo_mutex, "Inner");  // 注意：这会导致死锁，仅用于演示销毁顺序
            cout << "创建内层 guard" << endl;
        }  // 内层先销毁
        cout << "内层作用域结束" << endl;
        
    }  // 外层后销毁
    cout << "外层作用域结束" << endl;
}

void demonstrate_standard_lock_guard() {
    cout << "\n=== 5. 标准 lock_guard 的工作原理 ===" << endl;
    
    cout << "\n原理总结：" << endl;
    cout << "  1. lock_guard<mutex> lock(counter_mutex);" << endl;
    cout << "     ↓ 创建局部变量 'lock'" << endl;
    cout << "     ↓ 调用构造函数 lock_guard(counter_mutex)" << endl;
    cout << "     ↓ 构造函数内部调用 counter_mutex.lock()" << endl;
    cout << "  " << endl;
    cout << "  2. 使用临界区资源..." << endl;
    cout << "  " << endl;
    cout << "  3. } // 离开作用域" << endl;
    cout << "     ↓ C++ 自动销毁局部变量 'lock'" << endl;
    cout << "     ↓ 调用析构函数 ~lock_guard()" << endl;
    cout << "     ↓ 析构函数内部调用 counter_mutex.unlock()" << endl;
    
    cout << "\n实际演示：" << endl;
    {
        cout << "创建 lock_guard..." << endl;
        lock_guard<mutex> lock(demo_mutex);
        cout << "lock_guard 已创建，锁已获取" << endl;
        cout << "使用临界区..." << endl;
    }
    cout << "lock_guard 已自动销毁，锁已释放" << endl;
}

void demonstrate_why_it_works() {
    cout << "\n=== 6. 为什么这个机制如此可靠？ ===" << endl;
    
    cout << "\nC++ 语言保证：" << endl;
    cout << "  ✅ 局部变量在离开作用域时**必须**销毁" << endl;
    cout << "  ✅ 对象销毁时**必须**调用析构函数" << endl;
    cout << "  ✅ 即使发生异常，栈展开过程中也会正确销毁局部变量" << endl;
    cout << "  ✅ 这是 C++ 语言规范，不是实现细节" << endl;
    
    cout << "\nRAII 模式的威力：" << endl;
    cout << "  🎯 Resource Acquisition Is Initialization" << endl;
    cout << "  🏗️  构造 = 获取资源（加锁）" << endl;
    cout << "  🗑️  析构 = 释放资源（解锁）" << endl;
    cout << "  🔄 生命周期绑定 = 资源管理自动化" << endl;
    
    cout << "\n与手动管理的对比：" << endl;
    cout << "  ❌ 手动: mutex.lock() → 可能忘记 unlock()" << endl;
    cout << "  ❌ 手动: 异常时可能不会 unlock()" << endl;
    cout << "  ❌ 手动: 多个返回路径容易遗漏 unlock()" << endl;
    cout << "  " << endl;
    cout << "  ✅ RAII: 构造时自动 lock()" << endl;
    cout << "  ✅ RAII: 析构时自动 unlock()" << endl;
    cout << "  ✅ RAII: 无论如何离开作用域都会 unlock()" << endl;
    cout << "  ✅ RAII: 异常安全，零失误" << endl;
}

int main() {
    cout << "\n╔═══════════════════════════════════════════════════╗" << endl;
    cout << "║         验证 lock_guard 自动销毁原理              ║" << endl;
    cout << "║    局部变量作用域 + 构造析构函数配对调用           ║" << endl;
    cout << "╚═══════════════════════════════════════════════════╝" << endl;
    
    demonstrate_scope_destruction();
    demonstrate_early_return();
    demonstrate_exception_safety();
    // demonstrate_nested_scopes();  // 注释掉，因为会死锁
    demonstrate_standard_lock_guard();
    demonstrate_why_it_works();
    
    cout << "\n🎯 结论确认：" << endl;
    cout << "  你的理解完全正确！" << endl;
    cout << "  lock_guard<mutex> lock(counter_mutex) 确实是：" << endl;
    cout << "    1. 创建局部变量" << endl;
    cout << "    2. 利用作用域自动销毁" << endl;
    cout << "    3. 构造函数获取资源（加锁）" << endl;
    cout << "    4. 析构函数释放资源（解锁）" << endl;
    cout << "  这就是 RAII 模式的经典应用！" << endl;
    
    return 0;
}