/*
 * lock_guard 详解：RAII 原理和使用
 * 解答：为什么 lock_guard<mutex> lock(counter_mutex) 能自动加锁和解锁？
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
using namespace std;

// ============================================================================
// 一、传统方式 vs lock_guard 对比
// ============================================================================

mutex demo_mutex;
int shared_value = 0;

void traditional_way() {
    cout << "\n=== 1. 传统方式：手动 lock/unlock ===" << endl;
    
    cout << "\n代码结构：" << endl;
    cout << "  demo_mutex.lock();      // 手动加锁" << endl;
    cout << "  // 临界区代码" << endl;
    cout << "  demo_mutex.unlock();    // 手动解锁" << endl;
    
    cout << "\n实际执行：" << endl;
    demo_mutex.lock();
    cout << "  ✅ 锁已获取，进入临界区" << endl;
    shared_value = 100;
    cout << "  📝 修改共享数据: " << shared_value << endl;
    demo_mutex.unlock();
    cout << "  🔓 手动解锁完成" << endl;
    
    cout << "\n传统方式的问题：" << endl;
    cout << "  ❌ 容易忘记解锁" << endl;
    cout << "  ❌ 异常时可能不会解锁（导致死锁）" << endl;
    cout << "  ❌ 复杂流程中容易出错" << endl;
}

void lock_guard_way() {
    cout << "\n=== 2. lock_guard 方式：自动管理 ===" << endl;
    
    cout << "\n代码结构：" << endl;
    cout << "  {" << endl;
    cout << "      lock_guard<mutex> lock(demo_mutex);  // 构造时自动加锁" << endl;
    cout << "      // 临界区代码" << endl;
    cout << "  }  // 析构时自动解锁" << endl;
    
    cout << "\n实际执行：" << endl;
    {
        cout << "  🔒 创建 lock_guard 对象..." << endl;
        lock_guard<mutex> lock(demo_mutex);  // 构造函数自动调用 demo_mutex.lock()
        cout << "  ✅ lock_guard 构造完成，锁已获取" << endl;
        
        shared_value = 200;
        cout << "  📝 修改共享数据: " << shared_value << endl;
        
        cout << "  🏁 即将离开作用域..." << endl;
    }  // lock_guard 析构函数自动调用 demo_mutex.unlock()
    cout << "  🔓 lock_guard 析构完成，锁已释放" << endl;
}

// ============================================================================
// 二、RAII 原理详解
// ============================================================================

class MyLockGuard {
    mutex& m_mutex;
    
public:
    // 构造函数：获取资源（加锁）
    MyLockGuard(mutex& mtx) : m_mutex(mtx) {
        cout << "    [MyLockGuard] 构造函数：获取锁" << endl;
        m_mutex.lock();
    }
    
    // 析构函数：释放资源（解锁）
    ~MyLockGuard() {
        cout << "    [MyLockGuard] 析构函数：释放锁" << endl;
        m_mutex.unlock();
    }
    
    // 禁止拷贝（防止重复解锁）
    MyLockGuard(const MyLockGuard&) = delete;
    MyLockGuard& operator=(const MyLockGuard&) = delete;
};

void demonstrate_raii_principle() {
    cout << "\n=== 3. RAII 原理详解 ===" << endl;
    
    cout << "\nRAII = Resource Acquisition Is Initialization" << endl;
    cout << "  💡 核心思想：" << endl;
    cout << "    - 构造函数获取资源" << endl;
    cout << "    - 析构函数释放资源" << endl;
    cout << "    - C++ 保证对象离开作用域时自动调用析构函数" << endl;
    
    cout << "\n自定义 lock_guard 演示：" << endl;
    cout << "  代码: MyLockGuard guard(demo_mutex);" << endl;
    
    {
        cout << "  🚀 开始创建 MyLockGuard 对象" << endl;
        MyLockGuard guard(demo_mutex);  // 构造时加锁
        cout << "  ✅ MyLockGuard 对象创建完成，锁已获取" << endl;
        
        shared_value = 300;
        cout << "  📝 在临界区修改数据: " << shared_value << endl;
        
        cout << "  🏁 即将离开作用域，对象即将析构" << endl;
    }  // guard 对象析构，自动解锁
    cout << "  🔓 MyLockGuard 对象已析构，锁已释放" << endl;
}

// ============================================================================
// 三、对象生命周期详解
// ============================================================================

void demonstrate_object_lifecycle() {
    cout << "\n=== 4. 对象生命周期详解 ===" << endl;
    
    cout << "\n关键概念：作用域（Scope）" << endl;
    cout << "  作用域：用 { } 包围的代码块" << endl;
    cout << "  对象在作用域结束时自动析构" << endl;
    
    cout << "\n示例 1：基本作用域" << endl;
    cout << "  {  // <-- 作用域开始" << endl;
    {
        cout << "    创建局部变量..." << endl;
        int local_var = 42;
        lock_guard<mutex> lock(demo_mutex);
        cout << "    lock_guard 已创建，锁已获取" << endl;
        cout << "    local_var = " << local_var << endl;
    }  // <-- 作用域结束，所有局部对象（包括 lock_guard）自动析构
    cout << "    作用域结束，lock_guard 已析构，锁已释放" << endl;
    
    cout << "\n示例 2：函数作用域" << endl;
    auto func = []() {
        cout << "    函数开始，创建 lock_guard" << endl;
        lock_guard<mutex> lock(demo_mutex);
        cout << "    锁已获取" << endl;
        return 42;
        // 函数结束时，lock_guard 自动析构，锁自动释放
    };
    
    cout << "  调用函数..." << endl;
    int result = func();
    cout << "  函数返回: " << result << "，lock_guard 已在函数结束时析构" << endl;
}

// ============================================================================
// 四、异常安全性演示
// ============================================================================

void demonstrate_exception_safety() {
    cout << "\n=== 5. 异常安全性演示 ===" << endl;
    
    cout << "\n问题：传统方式遇到异常" << endl;
    cout << "  demo_mutex.lock();" << endl;
    cout << "  // 如果这里抛出异常..." << endl;
    cout << "  throw runtime_error(\"出错了\");" << endl;
    cout << "  demo_mutex.unlock();  // ❌ 永远不会执行！锁永远不会释放！" << endl;
    
    cout << "\n演示传统方式的问题：" << endl;
    try {
        demo_mutex.lock();
        cout << "  🔒 手动获取锁" << endl;
        
        // 模拟异常
        throw runtime_error("模拟异常");
        
        demo_mutex.unlock();  // ❌ 这行永远不会执行
    } catch (const exception& e) {
        cout << "  ❌ 捕获异常: " << e.what() << endl;
        cout << "  ❌ 锁没有释放！其他线程会永远等待！" << endl;
        demo_mutex.unlock();  // 手动修复，但很容易忘记
    }
    
    cout << "\n解决：lock_guard 的异常安全" << endl;
    try {
        cout << "  🔒 使用 lock_guard 获取锁" << endl;
        lock_guard<mutex> lock(demo_mutex);
        cout << "  ✅ 锁已获取" << endl;
        
        // 模拟异常
        throw runtime_error("模拟异常");
        
        // 即使这里有更多代码，也不会执行
    } catch (const exception& e) {
        cout << "  ❌ 捕获异常: " << e.what() << endl;
        cout << "  ✅ 但是！lock_guard 会在栈展开时自动析构，锁会自动释放！" << endl;
    }
    // lock_guard 在异常处理过程中已经自动析构并释放锁
}

// ============================================================================
// 五、使用场景对比
// ============================================================================

int counter = 0;
mutex counter_mtx;

void increment_traditional(int times) {
    for (int i = 0; i < times; i++) {
        counter_mtx.lock();
        counter++;
        counter_mtx.unlock();
    }
}

void increment_lock_guard(int times) {
    for (int i = 0; i < times; i++) {
        lock_guard<mutex> lock(counter_mtx);  // 自动加锁
        counter++;
        // 自动解锁（作用域结束）
    }
}

void increment_complex_logic_traditional(int times) {
    for (int i = 0; i < times; i++) {
        counter_mtx.lock();
        
        if (counter % 2 == 0) {
            counter += 2;
        } else {
            if (counter > 100) {
                counter_mtx.unlock();  // ❌ 容易忘记在这里解锁
                return;
            }
            counter++;
        }
        
        counter_mtx.unlock();
    }
}

void increment_complex_logic_lock_guard(int times) {
    for (int i = 0; i < times; i++) {
        lock_guard<mutex> lock(counter_mtx);  // 自动加锁
        
        if (counter % 2 == 0) {
            counter += 2;
        } else {
            if (counter > 100) {
                return;  // ✅ lock_guard 会自动析构解锁
            }
            counter++;
        }
        // 自动解锁，不管怎么退出这个作用域
    }
}

void demonstrate_usage_scenarios() {
    cout << "\n=== 6. 使用场景对比 ===" << endl;
    
    cout << "\n场景 1：简单临界区" << endl;
    cout << "  传统方式：" << endl;
    cout << "    mutex.lock();" << endl;
    cout << "    // 临界区" << endl;
    cout << "    mutex.unlock();" << endl;
    cout << "  " << endl;
    cout << "  lock_guard 方式：" << endl;
    cout << "    {" << endl;
    cout << "        lock_guard<mutex> lock(mutex);" << endl;
    cout << "        // 临界区" << endl;
    cout << "    }  // 自动解锁" << endl;
    
    cout << "\n场景 2：复杂逻辑（多个 return 路径）" << endl;
    cout << "  传统方式问题：" << endl;
    cout << "    - 每个 return 前都要记得 unlock()" << endl;
    cout << "    - 很容易遗漏，导致死锁" << endl;
    cout << "  " << endl;
    cout << "  lock_guard 优势：" << endl;
    cout << "    - 无论如何退出函数/作用域，都会自动解锁" << endl;
    cout << "    - 异常安全" << endl;
    
    cout << "\n性能测试：" << endl;
    counter = 0;
    
    auto start = chrono::high_resolution_clock::now();
    
    vector<thread> threads1;
    for (int i = 0; i < 4; i++) {
        threads1.emplace_back(increment_traditional, 10000);
    }
    for (auto& t : threads1) {
        t.join();
    }
    
    auto mid = chrono::high_resolution_clock::now();
    int result1 = counter;
    
    counter = 0;
    
    vector<thread> threads2;
    for (int i = 0; i < 4; i++) {
        threads2.emplace_back(increment_lock_guard, 10000);
    }
    for (auto& t : threads2) {
        t.join();
    }
    
    auto end = chrono::high_resolution_clock::now();
    int result2 = counter;
    
    auto time1 = chrono::duration_cast<chrono::microseconds>(mid - start).count();
    auto time2 = chrono::duration_cast<chrono::microseconds>(end - mid).count();
    
    cout << "  传统方式: " << result1 << " (" << time1 << " μs)" << endl;
    cout << "  lock_guard: " << result2 << " (" << time2 << " μs)" << endl;
    cout << "  性能差异: " << abs(time2 - time1) << " μs (基本相同)" << endl;
}

// ============================================================================
// 六、常见误解和正确理解
// ============================================================================

void demonstrate_common_misunderstandings() {
    cout << "\n=== 7. 常见误解和正确理解 ===" << endl;
    
    cout << "\n❌ 误解 1：认为 lock_guard 很复杂" << endl;
    cout << "  错误想法：lock_guard<mutex> lock(counter_mutex) 不直观" << endl;
    cout << "  ✅ 正确理解：" << endl;
    cout << "    这就是创建一个对象，对象名叫 lock" << endl;
    cout << "    类型是 lock_guard<mutex>" << endl;
    cout << "    构造参数是 counter_mutex" << endl;
    cout << "    和 int x(5) 创建整数是一样的道理" << endl;
    
    cout << "\n❌ 误解 2：不知道什么时候解锁" << endl;
    cout << "  错误想法：不知道锁什么时候释放" << endl;
    cout << "  ✅ 正确理解：" << endl;
    cout << "    C++ 规则：对象离开作用域时自动调用析构函数" << endl;
    cout << "    lock_guard 的析构函数会自动调用 mutex.unlock()" << endl;
    
    cout << "\n❌ 误解 3：认为 lock_guard 是魔法" << endl;
    cout << "  错误想法：不理解它怎么自动加锁解锁" << endl;
    cout << "  ✅ 正确理解：" << endl;
    cout << "    没有魔法，就是普通的构造函数和析构函数" << endl;
    cout << "    构造函数调用 mutex.lock()" << endl;
    cout << "    析构函数调用 mutex.unlock()" << endl;
    
    cout << "\n简化理解：" << endl;
    cout << "  把 lock_guard<mutex> lock(counter_mutex) 理解为：" << endl;
    cout << "  \"创建一个智能锁管家，把 counter_mutex 交给它管理\"" << endl;
    cout << "  管家上班时（构造）：锁门" << endl;
    cout << "  管家下班时（析构）：开门" << endl;
}

// ============================================================================
// 七、实践演示
// ============================================================================

void demonstrate_practical_examples() {
    cout << "\n=== 8. 实践演示 ===" << endl;
    
    cout << "\n示例：银行账户转账" << endl;
    
    class BankAccount {
        mutable mutex mtx;  // mutable 允许在 const 方法中修改
        int balance;
        string name;
        
    public:
        BankAccount(string n, int initial) : name(n), balance(initial) {}
        
        bool transfer_traditional(BankAccount& to, int amount) {
            // ❌ 传统方式：容易出错
            mtx.lock();
            if (balance < amount) {
                mtx.unlock();  // 记得解锁
                return false;
            }
            
            to.mtx.lock();  // 可能死锁！
            balance -= amount;
            to.balance += amount;
            
            to.mtx.unlock();
            mtx.unlock();
            return true;
        }
        
        bool transfer_lock_guard(BankAccount& to, int amount) {
            // ✅ lock_guard 方式：安全
            lock_guard<mutex> lock1(mtx);
            if (balance < amount) {
                return false;  // 自动解锁
            }
            
            lock_guard<mutex> lock2(to.mtx);  // 仍可能死锁，需要其他技术解决
            balance -= amount;
            to.balance += amount;
            return true;
            // 所有锁自动释放
        }
        
        int get_balance() const {
            lock_guard<mutex> lock(mtx);  // mutable mutex 可以在 const 方法中使用
            return balance;
        }
    };
    
    cout << "  关键点：" << endl;
    cout << "    1. lock_guard 自动管理锁的生命周期" << endl;
    cout << "    2. 无论函数如何退出，锁都会被释放" << endl;
    cout << "    3. 异常安全：即使抛出异常也会正确解锁" << endl;
}

// ============================================================================
// 八、核心总结
// ============================================================================

void summary() {
    cout << "\n========================================" << endl;
    cout << "        lock_guard 核心总结" << endl;
    cout << "========================================" << endl;
    
    cout << "\n一、工作原理：" << endl;
    cout << "  lock_guard<mutex> lock(counter_mutex);" << endl;
    cout << "  ↓" << endl;
    cout << "  创建 lock_guard 对象，构造函数调用 counter_mutex.lock()" << endl;
    cout << "  ↓" << endl;
    cout << "  使用临界区资源" << endl;
    cout << "  ↓" << endl;
    cout << "  对象离开作用域，析构函数调用 counter_mutex.unlock()" << endl;
    
    cout << "\n二、RAII 原理：" << endl;
    cout << "  Resource Acquisition Is Initialization" << endl;
    cout << "  🏗️  构造函数 = 获取资源（加锁）" << endl;
    cout << "  🗑️  析构函数 = 释放资源（解锁）" << endl;
    cout << "  🔄 C++ 保证对象离开作用域时自动析构" << endl;
    
    cout << "\n三、对比总结：" << endl;
    cout << "  ┌─────────────────┬──────────────┬──────────────┐" << endl;
    cout << "  │ 方面            │ 传统 lock()  │ lock_guard   │" << endl;
    cout << "  ├─────────────────┼──────────────┼──────────────┤" << endl;
    cout << "  │ 加锁            │ 手动         │ 自动         │" << endl;
    cout << "  │ 解锁            │ 手动         │ 自动         │" << endl;
    cout << "  │ 异常安全        │ ❌ 危险      │ ✅ 安全      │" << endl;
    cout << "  │ 忘记解锁        │ ❌ 可能      │ ✅ 不可能    │" << endl;
    cout << "  │ 复杂逻辑        │ ❌ 容易出错  │ ✅ 简单      │" << endl;
    cout << "  │ 性能开销        │ 无           │ 几乎无       │" << endl;
    cout << "  └─────────────────┴──────────────┴──────────────┘" << endl;
    
    cout << "\n四、记忆方法：" << endl;
    cout << "  1. 把 lock_guard 当作\"智能锁管家\"" << endl;
    cout << "  2. 管家上班（构造）时锁门" << endl;
    cout << "  3. 管家下班（析构）时开门" << endl;
    cout << "  4. 管家很负责任，绝不会忘记开门" << endl;
    
    cout << "\n五、使用建议：" << endl;
    cout << "  ✅ 优先使用 lock_guard（99% 的情况）" << endl;
    cout << "  ✅ 简单、安全、异常安全" << endl;
    cout << "  ⚠️ 需要手动控制锁时才用传统方式" << endl;
    cout << "  ⚠️ 复杂场景可以考虑 unique_lock" << endl;
    
    cout << "\n六、核心理解：" << endl;
    cout << "  lock_guard<mutex> lock(counter_mutex) 就是：" << endl;
    cout << "  \"创建一个对象来自动管理锁\"" << endl;
    cout << "  不是魔法，就是构造函数和析构函数的应用！" << endl;
    
    cout << "========================================" << endl;
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "\n╔═══════════════════════════════════════════════════╗" << endl;
    cout << "║              lock_guard 详解                     ║" << endl;
    cout << "║     RAII 原理和自动锁管理机制                     ║" << endl;
    cout << "╚═══════════════════════════════════════════════════╝" << endl;
    
    // 1. 传统方式 vs lock_guard
    traditional_way();
    lock_guard_way();
    
    // 2. RAII 原理详解
    demonstrate_raii_principle();
    
    // 3. 对象生命周期
    demonstrate_object_lifecycle();
    
    // 4. 异常安全性
    demonstrate_exception_safety();
    
    // 5. 使用场景对比
    demonstrate_usage_scenarios();
    
    // 6. 常见误解
    demonstrate_common_misunderstandings();
    
    // 7. 实践演示
    demonstrate_practical_examples();
    
    // 8. 总结
    summary();
    
    return 0;
}