/*
 * C++ 线程基础详解
 * 主题：C++11 引入的多线程支持
 * 核心：std::thread、线程创建、线程管理、线程同步
 */

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>
#include <atomic>
#include <future>
#include <condition_variable>
using namespace std;

// ============================================================================
// 一、什么是线程？
// ============================================================================

void what_is_thread() {
    cout << "\n=== 1. 什么是线程？===" << endl;
    
    cout << "\n基本概念：" << endl;
    cout << "  进程（Process）：" << endl;
    cout << "    - 操作系统资源分配的基本单位" << endl;
    cout << "    - 拥有独立的内存空间" << endl;
    cout << "    - 进程间通信（IPC）较复杂" << endl;
    
    cout << "\n  线程（Thread）：" << endl;
    cout << "    - 程序执行的最小单位" << endl;
    cout << "    - 共享进程的内存空间" << endl;
    cout << "    - 线程间通信简单，但需要同步" << endl;
    cout << "    - 轻量级，创建和切换开销小" << endl;
    
    cout << "\nC++ 线程发展：" << endl;
    cout << "  C++98/03: ❌ 没有标准线程库" << endl;
    cout << "    - 依赖操作系统 API（pthread、Windows Thread）" << endl;
    cout << "    - 不可移植" << endl;
    
    cout << "\n  C++11: ✅ 引入 std::thread" << endl;
    cout << "    - 跨平台标准线程库" << endl;
    cout << "    - #include <thread>" << endl;
    cout << "    - 提供线程、互斥锁、条件变量等" << endl;
    
    cout << "\n硬件信息：" << endl;
    cout << "  当前系统支持的并发线程数: " 
         << thread::hardware_concurrency() << endl;
}

// ============================================================================
// 二、创建线程的多种方式
// ============================================================================

// 方式1: 普通函数
void print_hello() {
    cout << "  [线程 " << this_thread::get_id() << "] Hello from thread!" << endl;
}

// 方式2: 带参数的函数
void print_number(int n, const string& message) {
    cout << "  [线程 " << this_thread::get_id() 
         << "] Number: " << n << ", Message: " << message << endl;
}

// 方式3: 类的成员函数
class Counter {
    int count = 0;
public:
    void increment(int times) {
        for (int i = 0; i < times; i++) {
            count++;
        }
        cout << "  [线程 " << this_thread::get_id() 
             << "] Count: " << count << endl;
    }
    
    int get_count() const { return count; }
};

// 方式4: 函数对象（Functor）
class PrintFunctor {
    string message;
public:
    PrintFunctor(string msg) : message(msg) {}
    
    void operator()() {
        cout << "  [线程 " << this_thread::get_id() 
             << "] Functor: " << message << endl;
    }
};

void demonstrate_thread_creation() {
    cout << "\n=== 2. 创建线程的多种方式 ===" << endl;
    
    cout << "\n方式1: 普通函数" << endl;
    thread t1(print_hello);
    t1.join();  // 等待线程结束
    
    cout << "\n方式2: 带参数的函数" << endl;
    thread t2(print_number, 42, "测试");
    t2.join();
    
    cout << "\n方式3: 类的成员函数" << endl;
    Counter counter;
    thread t3(&Counter::increment, &counter, 100);
    t3.join();
    cout << "  最终计数: " << counter.get_count() << endl;
    
    cout << "\n方式4: 函数对象（Functor）" << endl;
    PrintFunctor functor("Hello from functor!");
    thread t4(functor);
    t4.join();
    
    cout << "\n方式5: Lambda 表达式（最常用）" << endl;
    thread t5([]() {
        cout << "  [线程 " << this_thread::get_id() 
             << "] Lambda: Hello from lambda!" << endl;
    });
    t5.join();
    
    cout << "\n方式6: Lambda 捕获变量" << endl;
    int value = 100;
    thread t6([value]() {
        cout << "  [线程 " << this_thread::get_id() 
             << "] Lambda captured: " << value << endl;
    });
    t6.join();
}

// ============================================================================
// 三、线程的生命周期管理
// ============================================================================

void demonstrate_thread_lifecycle() {
    cout << "\n=== 3. 线程的生命周期管理 ===" << endl;
    
    cout << "\n3.1 join() - 等待线程结束" << endl;
    cout << "  作用：主线程等待子线程执行完毕" << endl;
    cout << "  阻塞：会阻塞主线程" << endl;
    
    thread t1([]() {
        this_thread::sleep_for(chrono::milliseconds(100));
        cout << "    子线程执行完毕" << endl;
    });
    
    cout << "  主线程等待中..." << endl;
    t1.join();
    cout << "  主线程继续执行" << endl;
    
    cout << "\n3.2 detach() - 分离线程" << endl;
    cout << "  作用：线程独立运行，不再受主线程控制" << endl;
    cout << "  注意：线程对象不再代表任何线程" << endl;
    
    thread t2([]() {
        this_thread::sleep_for(chrono::milliseconds(50));
        cout << "    分离的线程可能会执行..." << endl;
    });
    
    t2.detach();
    cout << "  主线程不等待分离的线程" << endl;
    this_thread::sleep_for(chrono::milliseconds(100));  // 等待一下看效果
    
    cout << "\n3.3 joinable() - 检查是否可以 join" << endl;
    thread t3([]() {});
    
    cout << "  join 前 joinable: " << (t3.joinable() ? "是" : "否") << endl;
    t3.join();
    cout << "  join 后 joinable: " << (t3.joinable() ? "是" : "否") << endl;
    
    cout << "\n⚠️ 重要：线程必须 join 或 detach，否则析构时会调用 std::terminate()" << endl;
}

// ============================================================================
// 四、线程参数传递
// ============================================================================

void thread_with_ref(int& value) {
    value = 100;
    cout << "  线程内修改: " << value << endl;
}

void demonstrate_thread_arguments() {
    cout << "\n=== 4. 线程参数传递 ===" << endl;
    
    cout << "\n4.1 按值传递（默认）" << endl;
    int x = 10;
    thread t1([](int val) {
        val = 20;
        cout << "    线程内: " << val << endl;
    }, x);
    t1.join();
    cout << "  主线程: x = " << x << " (未改变)" << endl;
    
    cout << "\n4.2 按引用传递（需要 std::ref）" << endl;
    int y = 10;
    thread t2(thread_with_ref, ref(y));  // 必须用 std::ref
    t2.join();
    cout << "  主线程: y = " << y << " (已改变)" << endl;
    
    cout << "\n4.3 传递指针" << endl;
    int z = 10;
    thread t3([](int* ptr) {
        *ptr = 30;
        cout << "    线程内: " << *ptr << endl;
    }, &z);
    t3.join();
    cout << "  主线程: z = " << z << " (已改变)" << endl;
    
    cout << "\n4.4 移动语义（转移所有权）" << endl;
    vector<int> vec = {1, 2, 3};
    cout << "  移动前大小: " << vec.size() << endl;
    
    thread t4([](vector<int> v) {
        cout << "    线程内大小: " << v.size() << endl;
    }, std::move(vec));  // 移动，避免拷贝
    
    cout << "  移动后大小: " << vec.size() << " (已被掏空)" << endl;
    t4.join();
}

// ============================================================================
// 五、线程同步：互斥锁（Mutex）
// ============================================================================

int shared_counter = 0;
mutex counter_mutex;

void increment_without_lock(int times) {
    for (int i = 0; i < times; i++) {
        shared_counter++;  // ❌ 数据竞争！
    }
}

void increment_with_lock(int times) {
    for (int i = 0; i < times; i++) {
        counter_mutex.lock();
        shared_counter++;  // ✅ 安全
        counter_mutex.unlock();
    }
}

void increment_with_lock_guard(int times) {
    for (int i = 0; i < times; i++) {
        lock_guard<mutex> lock(counter_mutex);  // RAII，自动加锁/解锁
        shared_counter++;
    }
}

void demonstrate_mutex() {
    cout << "\n=== 5. 线程同步：互斥锁（Mutex）===" << endl;
    
    cout << "\n5.1 数据竞争问题（Race Condition）" << endl;
    shared_counter = 0;
    
    thread t1(increment_without_lock, 10000);
    thread t2(increment_without_lock, 10000);
    
    t1.join();
    t2.join();
    
    cout << "  预期结果: 20000" << endl;
    cout << "  实际结果: " << shared_counter << " ❌ 错误！" << endl;
    
    cout << "\n5.2 使用 mutex.lock() 和 unlock()" << endl;
    shared_counter = 0;
    
    thread t3(increment_with_lock, 10000);
    thread t4(increment_with_lock, 10000);
    
    t3.join();
    t4.join();
    
    cout << "  结果: " << shared_counter << " ✅ 正确！" << endl;
    
    cout << "\n5.3 使用 lock_guard（推荐，RAII）" << endl;
    shared_counter = 0;
    
    thread t5(increment_with_lock_guard, 10000);
    thread t6(increment_with_lock_guard, 10000);
    
    t5.join();
    t6.join();
    
    cout << "  结果: " << shared_counter << " ✅ 正确！" << endl;
    
    cout << "\nlock_guard 优点：" << endl;
    cout << "  ✅ 自动加锁和解锁（构造时加锁，析构时解锁）" << endl;
    cout << "  ✅ 异常安全（即使抛出异常也会解锁）" << endl;
    cout << "  ✅ 防止忘记解锁" << endl;
}

// ============================================================================
// 六、原子操作（Atomic）
// ============================================================================

atomic<int> atomic_counter(0);

void increment_atomic(int times) {
    for (int i = 0; i < times; i++) {
        atomic_counter++;  // 原子操作，无需锁
    }
}

void demonstrate_atomic() {
    cout << "\n=== 6. 原子操作（Atomic）===" << endl;
    
    cout << "\n什么是原子操作？" << endl;
    cout << "  - 不可分割的操作" << endl;
    cout << "  - 硬件层面保证线程安全" << endl;
    cout << "  - 性能优于 mutex（无锁编程）" << endl;
    
    cout << "\n示例：" << endl;
    atomic_counter = 0;
    
    thread t1(increment_atomic, 100000);
    thread t2(increment_atomic, 100000);
    
    t1.join();
    t2.join();
    
    cout << "  结果: " << atomic_counter << " ✅ 正确！" << endl;
    
    cout << "\n适用场景：" << endl;
    cout << "  ✅ 简单的计数器" << endl;
    cout << "  ✅ 标志位（flag）" << endl;
    cout << "  ✅ 单个变量的读写" << endl;
    cout << "  ❌ 复杂的临界区（需要 mutex）" << endl;
    
    cout << "\n常用原子类型：" << endl;
    cout << "  atomic<int>, atomic<bool>, atomic<long>, etc." << endl;
}

// ============================================================================
// 七、条件变量（Condition Variable）
// ============================================================================

mutex cv_mutex;
condition_variable cv;
bool ready = false;
int shared_data = 0;

void producer() {
    this_thread::sleep_for(chrono::milliseconds(100));
    
    {
        lock_guard<mutex> lock(cv_mutex);
        shared_data = 42;
        ready = true;
        cout << "  [生产者] 数据准备完毕: " << shared_data << endl;
    }
    
    cv.notify_one();  // 通知等待的线程
}

void consumer() {
    cout << "  [消费者] 等待数据..." << endl;
    
    unique_lock<mutex> lock(cv_mutex);
    cv.wait(lock, []{ return ready; });  // 等待条件满足
    
    cout << "  [消费者] 收到数据: " << shared_data << endl;
}

void demonstrate_condition_variable() {
    cout << "\n=== 7. 条件变量（Condition Variable）===" << endl;
    
    cout << "\n作用：" << endl;
    cout << "  - 线程间同步：等待某个条件满足" << endl;
    cout << "  - 避免忙等待（busy waiting）" << endl;
    cout << "  - 生产者-消费者模式" << endl;
    
    cout << "\n示例：生产者-消费者" << endl;
    
    ready = false;
    shared_data = 0;
    
    thread t1(consumer);
    thread t2(producer);
    
    t1.join();
    t2.join();
    
    cout << "\n关键点：" << endl;
    cout << "  1. wait() 会自动释放锁，等待被唤醒" << endl;
    cout << "  2. notify_one() 唤醒一个等待的线程" << endl;
    cout << "  3. notify_all() 唤醒所有等待的线程" << endl;
}

// ============================================================================
// 八、异步任务（std::async 和 std::future）
// ============================================================================

int calculate_sum(int n) {
    int sum = 0;
    for (int i = 1; i <= n; i++) {
        sum += i;
    }
    return sum;
}

void demonstrate_async_future() {
    cout << "\n=== 8. 异步任务（std::async 和 std::future）===" << endl;
    
    cout << "\nstd::async 作用：" << endl;
    cout << "  - 更高层的异步编程接口" << endl;
    cout << "  - 自动管理线程" << endl;
    cout << "  - 可以获取返回值（通过 future）" << endl;
    
    cout << "\n示例1: 获取异步任务的返回值" << endl;
    
    future<int> result = async(launch::async, calculate_sum, 100);
    
    cout << "  主线程继续执行其他任务..." << endl;
    this_thread::sleep_for(chrono::milliseconds(10));
    
    cout << "  获取结果: " << result.get() << endl;  // 阻塞直到结果可用
    
    cout << "\n示例2: 多个异步任务" << endl;
    
    auto f1 = async(launch::async, calculate_sum, 100);
    auto f2 = async(launch::async, calculate_sum, 200);
    auto f3 = async(launch::async, calculate_sum, 300);
    
    cout << "  结果1: " << f1.get() << endl;
    cout << "  结果2: " << f2.get() << endl;
    cout << "  结果3: " << f3.get() << endl;
    
    cout << "\nlaunch 策略：" << endl;
    cout << "  launch::async    - 立即创建新线程" << endl;
    cout << "  launch::deferred - 延迟执行，调用 get() 时才执行" << endl;
    
    cout << "\nstd::async vs std::thread：" << endl;
    cout << "  ┌─────────────────┬──────────────┬──────────────┐" << endl;
    cout << "  │ 特性            │ std::thread  │ std::async   │" << endl;
    cout << "  ├─────────────────┼──────────────┼──────────────┤" << endl;
    cout << "  │ 返回值          │ ❌ 无        │ ✅ future    │" << endl;
    cout << "  │ 异常处理        │ ⚠️ 复杂     │ ✅ 简单      │" << endl;
    cout << "  │ 线程管理        │ 手动         │ 自动         │" << endl;
    cout << "  │ 使用难度        │ 较难         │ 简单         │" << endl;
    cout << "  └─────────────────┴──────────────┴──────────────┘" << endl;
}

// ============================================================================
// 九、线程池概念
// ============================================================================

void demonstrate_thread_pool_concept() {
    cout << "\n=== 9. 线程池概念 ===" << endl;
    
    cout << "\n什么是线程池？" << endl;
    cout << "  - 预先创建一组线程" << endl;
    cout << "  - 复用线程，避免频繁创建/销毁" << endl;
    cout << "  - 控制并发数量" << endl;
    
    cout << "\n为什么需要线程池？" << endl;
    cout << "  问题：频繁创建线程开销大" << endl;
    cout << "    - 创建线程需要系统调用" << endl;
    cout << "    - 线程过多会耗尽系统资源" << endl;
    
    cout << "\n  解决方案：线程池" << endl;
    cout << "    ✅ 线程复用，减少创建开销" << endl;
    cout << "    ✅ 限制并发数，避免资源耗尽" << endl;
    cout << "    ✅ 任务队列，统一管理" << endl;
    
    cout << "\n简单示例（模拟线程池思想）：" << endl;
    
    const int num_threads = 4;
    vector<thread> thread_pool;
    
    cout << "  创建 " << num_threads << " 个工作线程..." << endl;
    
    for (int i = 0; i < num_threads; i++) {
        thread_pool.emplace_back([i]() {
            cout << "    [工作线程 " << i << "] 执行任务" << endl;
            this_thread::sleep_for(chrono::milliseconds(50));
        });
    }
    
    cout << "  等待所有线程完成..." << endl;
    for (auto& t : thread_pool) {
        t.join();
    }
    
    cout << "  完成！" << endl;
    
    cout << "\n注意：C++ 标准库不提供线程池，需要：" << endl;
    cout << "  - 自己实现" << endl;
    cout << "  - 使用第三方库（如 Boost.Asio）" << endl;
}

// ============================================================================
// 十、常见陷阱和最佳实践
// ============================================================================

void demonstrate_pitfalls_and_best_practices() {
    cout << "\n=== 10. 常见陷阱和最佳实践 ===" << endl;
    
    cout << "\n⚠️ 陷阱1: 忘记 join 或 detach" << endl;
    cout << "  错误：thread 对象析构时如果未 join 或 detach，程序会终止" << endl;
    cout << "  {" << endl;
    cout << "      thread t([](){});" << endl;
    cout << "      // 忘记 join 或 detach" << endl;
    cout << "  }  // ❌ std::terminate() 被调用！" << endl;
    
    cout << "\n⚠️ 陷阱2: 数据竞争" << endl;
    cout << "  多个线程同时访问共享数据，且至少有一个写操作" << endl;
    cout << "  解决：使用 mutex、atomic、或避免共享数据" << endl;
    
    cout << "\n⚠️ 陷阱3: 死锁（Deadlock）" << endl;
    cout << "  两个线程互相等待对方释放锁" << endl;
    cout << "  线程A: lock(m1) → 等待 lock(m2)" << endl;
    cout << "  线程B: lock(m2) → 等待 lock(m1)" << endl;
    cout << "  解决：统一锁的顺序、使用 std::lock()" << endl;
    
    cout << "\n⚠️ 陷阱4: 引用悬空" << endl;
    cout << "  线程使用局部变量的引用，但变量已销毁" << endl;
    cout << "  void func() {" << endl;
    cout << "      int x = 10;" << endl;
    cout << "      thread t([&x]() { ... });  // ❌ x 可能已销毁" << endl;
    cout << "      t.detach();" << endl;
    cout << "  }  // x 销毁，但线程还在运行" << endl;
    
    cout << "\n✅ 最佳实践：" << endl;
    cout << "  1. 优先使用 std::async 而不是手动管理线程" << endl;
    cout << "  2. 使用 RAII 管理锁（lock_guard、unique_lock）" << endl;
    cout << "  3. 尽量减少共享数据，优先使用消息传递" << endl;
    cout << "  4. 简单计数用 atomic，复杂临界区用 mutex" << endl;
    cout << "  5. 避免在持有锁时调用用户代码或阻塞操作" << endl;
    cout << "  6. 线程数量不要超过 hardware_concurrency()" << endl;
    cout << "  7. 使用 thread 时，确保 join 或 detach" << endl;
}

// ============================================================================
// 十一、核心总结
// ============================================================================

void summary() {
    cout << "\n========================================" << endl;
    cout << "           核心总结" << endl;
    cout << "========================================" << endl;
    
    cout << "\n主题: C++ 线程编程" << endl;
    
    cout << "\n一、基础概念：" << endl;
    cout << "  线程：程序执行的最小单位" << endl;
    cout << "  并发：多个任务交替执行" << endl;
    cout << "  并行：多个任务同时执行（多核）" << endl;
    
    cout << "\n二、创建线程：" << endl;
    cout << "  1. std::thread t(function);" << endl;
    cout << "  2. std::thread t(lambda);" << endl;
    cout << "  3. std::thread t(&Class::method, &obj);" << endl;
    
    cout << "\n三、线程管理：" << endl;
    cout << "  join()    - 等待线程结束（阻塞）" << endl;
    cout << "  detach()  - 分离线程（独立运行）" << endl;
    cout << "  joinable() - 检查是否可以 join" << endl;
    
    cout << "\n四、线程同步：" << endl;
    cout << "  ┌─────────────────┬──────────────────┬─────────────┐" << endl;
    cout << "  │ 工具            │ 用途             │ 性能        │" << endl;
    cout << "  ├─────────────────┼──────────────────┼─────────────┤" << endl;
    cout << "  │ mutex           │ 互斥访问         │ 中等        │" << endl;
    cout << "  │ lock_guard      │ RAII 锁管理      │ 中等        │" << endl;
    cout << "  │ unique_lock     │ 灵活的锁管理     │ 中等        │" << endl;
    cout << "  │ atomic          │ 原子操作         │ 高（无锁）  │" << endl;
    cout << "  │ condition_var   │ 条件等待         │ 低（阻塞）  │" << endl;
    cout << "  └─────────────────┴──────────────────┴─────────────┘" << endl;
    
    cout << "\n五、异步编程：" << endl;
    cout << "  std::async  - 高层异步接口" << endl;
    cout << "  std::future - 获取异步结果" << endl;
    cout << "  优点：自动管理线程、可获取返回值" << endl;
    
    cout << "\n六、使用场景：" << endl;
    cout << "  ✅ 简单任务：std::async" << endl;
    cout << "  ✅ 需要精确控制：std::thread" << endl;
    cout << "  ✅ 简单计数：atomic" << endl;
    cout << "  ✅ 保护临界区：mutex + lock_guard" << endl;
    cout << "  ✅ 等待条件：condition_variable" << endl;
    
    cout << "\n七、性能对比：" << endl;
    cout << "  atomic > mutex > condition_variable" << endl;
    cout << "  但要根据具体场景选择合适的工具" << endl;
    
    cout << "\n八、常见问题：" << endl;
    cout << "  ❌ 数据竞争：多线程无保护访问共享数据" << endl;
    cout << "  ❌ 死锁：多个线程互相等待" << endl;
    cout << "  ❌ 忘记 join/detach：程序终止" << endl;
    cout << "  ❌ 引用悬空：线程使用已销毁的变量" << endl;
    
    cout << "\n九、最佳实践：" << endl;
    cout << "  1. 优先使用高层抽象（async）" << endl;
    cout << "  2. 使用 RAII 管理资源" << endl;
    cout << "  3. 减少共享数据" << endl;
    cout << "  4. 正确选择同步工具" << endl;
    cout << "  5. 避免常见陷阱" << endl;
    
    cout << "\n十、与 Java 对比：" << endl;
    cout << "  ┌──────────────────┬────────────────┬────────────────┐" << endl;
    cout << "  │ 特性             │ C++            │ Java           │" << endl;
    cout << "  ├──────────────────┼────────────────┼────────────────┤" << endl;
    cout << "  │ 线程创建         │ std::thread    │ new Thread()   │" << endl;
    cout << "  │ 互斥锁           │ mutex          │ synchronized   │" << endl;
    cout << "  │ 条件变量         │ condition_var  │ wait/notify    │" << endl;
    cout << "  │ 异步编程         │ async/future   │ Future/Exec... │" << endl;
    cout << "  │ 线程池           │ 需自己实现     │ ExecutorServ...│" << endl;
    cout << "  └──────────────────┴────────────────┴────────────────┘" << endl;
    
    cout << "\n记忆口诀：" << endl;
    cout << "  线程创建 thread 起，join detach 二选一" << endl;
    cout << "  共享数据要保护，mutex atomic 来守护" << endl;
    cout << "  lock_guard 自动锁，RAII 安全又简洁" << endl;
    cout << "  条件等待 condition，生产消费好搭档" << endl;
    cout << "  async future 高层次，简单任务首选它" << endl;
    
    cout << "\n========================================" << endl;
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "\n╔═══════════════════════════════════════════════════╗" << endl;
    cout << "║         C++ 线程编程基础                          ║" << endl;
    cout << "║   std::thread, mutex, atomic, async               ║" << endl;
    cout << "╚═══════════════════════════════════════════════════╝" << endl;
    
    // 1. 什么是线程
    what_is_thread();
    
    // 2. 创建线程
    demonstrate_thread_creation();
    
    // 3. 线程生命周期
    demonstrate_thread_lifecycle();
    
    // 4. 线程参数传递
    demonstrate_thread_arguments();
    
    // 5. 互斥锁
    demonstrate_mutex();
    
    // 6. 原子操作
    demonstrate_atomic();
    
    // 7. 条件变量
    demonstrate_condition_variable();
    
    // 8. 异步任务
    demonstrate_async_future();
    
    // 9. 线程池概念
    demonstrate_thread_pool_concept();
    
    // 10. 陷阱和最佳实践
    demonstrate_pitfalls_and_best_practices();
    
    // 11. 总结
    summary();
    
    return 0;
}
