/*
 * 从子线程切换到主线程执行的方法
 * 核心：线程间通信、任务队列、回调机制
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <future>
#include <chrono>
#include <atomic>
using namespace std;

// ============================================================================
// 一、核心概念：什么是"切换到主线程执行"
// ============================================================================

void explain_thread_switching() {
    cout << "\n=== 1. 什么是\"切换到主线程执行\" ===" << endl;
    
    cout << "\n核心理解：" << endl;
    cout << "  ❌ C++ 没有直接的线程切换 API（不像 JavaScript 的 async/await）" << endl;
    cout << "  ✅ 但可以通过消息传递、任务队列实现类似效果" << endl;
    
    cout << "\n常见场景：" << endl;
    cout << "  1. GUI 程序：子线程下载数据，主线程更新界面" << endl;
    cout << "  2. 游戏：子线程加载资源，主线程渲染" << endl;
    cout << "  3. 网络：子线程接收数据，主线程处理" << endl;
    
    cout << "\n实现思路：" << endl;
    cout << "  1. 子线程完成任务后，将结果或回调放入队列" << endl;
    cout << "  2. 主线程定期检查队列，执行任务" << endl;
    cout << "  3. 使用 condition_variable 或 future 通知" << endl;
}

// ============================================================================
// 二、方法1：任务队列（最常用，推荐）
// ============================================================================

class TaskQueue {
private:
    queue<function<void()>> tasks;
    mutex mtx;
    
public:
    // 添加任务（子线程调用）
    void post(function<void()> task) {
        lock_guard<mutex> lock(mtx);
        tasks.push(task);
        cout << "    [子线程] 任务已提交到主线程队列" << endl;
    }
    
    // 执行所有任务（主线程调用）
    void execute_all() {
        lock_guard<mutex> lock(mtx);
        while (!tasks.empty()) {
            auto task = tasks.front();
            tasks.pop();
            
            // 解锁后执行，避免长时间持有锁
            mtx.unlock();
            task();  // ✅ 在主线程执行
            mtx.lock();
        }
    }
    
    bool has_tasks() {
        lock_guard<mutex> lock(mtx);
        return !tasks.empty();
    }
};

void demonstrate_task_queue() {
    cout << "\n=== 2. 方法1：任务队列（推荐）===" << endl;
    
    TaskQueue queue;
    
    cout << "\n场景：子线程下载数据，主线程更新界面" << endl;
    
    // 子线程：模拟下载
    thread worker([&queue]() {
        cout << "  [子线程] 开始下载数据..." << endl;
        this_thread::sleep_for(chrono::milliseconds(200));
        
        string data = "下载的数据";
        
        // 将更新界面的任务提交到主线程
        queue.post([data]() {
            cout << "    [主线程] 更新界面: " << data << endl;
        });
        
        cout << "  [子线程] 下载完成" << endl;
    });
    
    // 主线程：模拟事件循环
    cout << "[主线程] 运行事件循环..." << endl;
    
    for (int i = 0; i < 10; i++) {
        // 模拟主线程的其他工作
        this_thread::sleep_for(chrono::milliseconds(50));
        
        // 执行队列中的任务
        queue.execute_all();
    }
    
    worker.join();
    
    cout << "\n优点：" << endl;
    cout << "  ✅ 模式清晰，易于理解" << endl;
    cout << "  ✅ 线程安全" << endl;
    cout << "  ✅ 主线程完全控制执行时机" << endl;
    cout << "  ✅ GUI 框架常用模式（Qt、wxWidgets）" << endl;
}

// ============================================================================
// 三、方法2：带通知的任务队列（阻塞等待）
// ============================================================================

class BlockingTaskQueue {
private:
    queue<function<void()>> tasks;
    mutex mtx;
    condition_variable cv;
    atomic<bool> stop{false};
    
public:
    // 子线程：提交任务
    void post(function<void()> task) {
        {
            lock_guard<mutex> lock(mtx);
            tasks.push(task);
        }
        cv.notify_one();  // ✅ 通知主线程
        cout << "    [子线程] 任务已提交，并通知主线程" << endl;
    }
    
    // 主线程：等待并执行任务
    void wait_and_execute() {
        unique_lock<mutex> lock(mtx);
        
        // 等待任务到来
        cv.wait(lock, [this]() { 
            return !tasks.empty() || stop; 
        });
        
        if (stop) return;
        
        auto task = tasks.front();
        tasks.pop();
        lock.unlock();
        
        task();  // ✅ 在主线程执行
    }
    
    void shutdown() {
        stop = true;
        cv.notify_all();
    }
};

void demonstrate_blocking_queue() {
    cout << "\n=== 3. 方法2：带通知的任务队列 ===" << endl;
    
    BlockingTaskQueue queue;
    
    cout << "\n场景：主线程等待子线程的任务" << endl;
    
    // 子线程：模拟耗时操作
    thread worker([&queue]() {
        cout << "  [子线程] 开始计算..." << endl;
        this_thread::sleep_for(chrono::milliseconds(300));
        
        int result = 42;
        
        // 提交任务到主线程
        queue.post([result]() {
            cout << "    [主线程] 收到结果: " << result << endl;
        });
        
        cout << "  [子线程] 计算完成" << endl;
    });
    
    // 主线程：阻塞等待任务
    cout << "[主线程] 等待任务..." << endl;
    queue.wait_and_execute();  // ✅ 阻塞等待，有任务时执行
    cout << "[主线程] 任务执行完毕" << endl;
    
    queue.shutdown();
    worker.join();
    
    cout << "\n优点：" << endl;
    cout << "  ✅ 实时响应，无需轮询" << endl;
    cout << "  ✅ CPU 友好（阻塞时不消耗 CPU）" << endl;
}

// ============================================================================
// 四、方法3：回调函数（简单场景）
// ============================================================================

void demonstrate_callback() {
    cout << "\n=== 4. 方法3：回调函数 ===" << endl;
    
    cout << "\n场景：子线程完成后调用回调" << endl;
    
    // 定义回调类型
    using Callback = function<void(int)>;
    
    // 异步计算函数
    auto async_compute = [](int n, Callback callback) {
        thread([n, callback]() {
            cout << "  [子线程] 开始计算..." << endl;
            this_thread::sleep_for(chrono::milliseconds(200));
            
            int result = n * n;
            
            // ⚠️ 注意：回调仍在子线程执行！
            cout << "  [子线程] 调用回调（仍在子线程）" << endl;
            callback(result);
            
        }).detach();
    };
    
    cout << "[主线程] 启动异步计算" << endl;
    
    async_compute(10, [](int result) {
        cout << "    [回调] 结果: " << result << endl;
        cout << "    [回调] 线程 ID: " << this_thread::get_id() << endl;
    });
    
    cout << "[主线程] ID: " << this_thread::get_id() << endl;
    cout << "[主线程] 继续工作..." << endl;
    
    // 等待子线程完成
    this_thread::sleep_for(chrono::milliseconds(300));
    
    cout << "\n⚠️ 注意：" << endl;
    cout << "  回调虽然看起来像切换到主线程，但实际仍在子线程执行！" << endl;
    cout << "  如需在主线程执行，需要配合任务队列" << endl;
}

// ============================================================================
// 五、方法4：回调 + 任务队列（推荐）
// ============================================================================

void demonstrate_callback_with_queue() {
    cout << "\n=== 5. 方法4：回调 + 任务队列（推荐）===" << endl;
    
    TaskQueue main_queue;
    
    cout << "\n场景：子线程完成后，回调在主线程执行" << endl;
    
    // 异步计算，回调在主线程执行
    auto async_compute_on_main = [&main_queue](int n, function<void(int)> callback) {
        thread([n, callback, &main_queue]() {
            cout << "  [子线程] 开始计算..." << endl;
            this_thread::sleep_for(chrono::milliseconds(200));
            
            int result = n * n;
            
            // 将回调提交到主线程队列
            main_queue.post([callback, result]() {
                cout << "    [主线程] 执行回调" << endl;
                callback(result);
            });
            
        }).detach();
    };
    
    cout << "[主线程] 启动异步计算" << endl;
    
    async_compute_on_main(10, [](int result) {
        cout << "    [回调] 结果: " << result << " ✅ 在主线程执行" << endl;
    });
    
    // 主线程事件循环
    cout << "[主线程] 运行事件循环..." << endl;
    for (int i = 0; i < 10; i++) {
        this_thread::sleep_for(chrono::milliseconds(50));
        main_queue.execute_all();
    }
    
    cout << "\n✅ 完美方案：" << endl;
    cout << "  子线程执行耗时操作" << endl;
    cout << "  回调在主线程安全执行" << endl;
}

// ============================================================================
// 六、方法5：std::async + future（简单但功能有限）
// ============================================================================

void demonstrate_async_future() {
    cout << "\n=== 6. 方法5：std::async + future ===" << endl;
    
    cout << "\n场景：异步计算，主线程获取结果" << endl;
    
    // 启动异步任务
    auto fut = async(launch::async, []() {
        cout << "  [子线程] 开始计算..." << endl;
        this_thread::sleep_for(chrono::milliseconds(200));
        return 42;
    });
    
    cout << "[主线程] 继续工作..." << endl;
    this_thread::sleep_for(chrono::milliseconds(100));
    
    cout << "[主线程] 获取结果（会阻塞）" << endl;
    int result = fut.get();  // ✅ 在主线程获取结果
    
    cout << "[主线程] 结果: " << result << endl;
    
    cout << "\n优点：" << endl;
    cout << "  ✅ 代码简洁" << endl;
    cout << "  ✅ 自动管理线程" << endl;
    
    cout << "\n缺点：" << endl;
    cout << "  ⚠️ 只能获取一次结果" << endl;
    cout << "  ⚠️ 无法实现复杂的通信模式" << endl;
}

// ============================================================================
// 七、方法6：完整的事件循环（生产级）
// ============================================================================

class EventLoop {
private:
    queue<function<void()>> tasks;
    mutex mtx;
    condition_variable cv;
    atomic<bool> running{true};
    
public:
    // 提交任务到主线程
    void post(function<void()> task) {
        {
            lock_guard<mutex> lock(mtx);
            tasks.push(task);
        }
        cv.notify_one();
    }
    
    // 主线程运行事件循环
    void run() {
        cout << "[EventLoop] 启动主线程事件循环" << endl;
        
        while (running) {
            unique_lock<mutex> lock(mtx);
            
            // 等待任务或超时（100ms）
            cv.wait_for(lock, chrono::milliseconds(100), [this]() {
                return !tasks.empty() || !running;
            });
            
            // 执行所有任务
            while (!tasks.empty() && running) {
                auto task = tasks.front();
                tasks.pop();
                
                lock.unlock();
                task();  // ✅ 在主线程执行
                lock.lock();
            }
        }
        
        cout << "[EventLoop] 事件循环结束" << endl;
    }
    
    void stop() {
        running = false;
        cv.notify_all();
    }
};

void demonstrate_event_loop() {
    cout << "\n=== 7. 方法6：完整的事件循环（生产级）===" << endl;
    
    EventLoop loop;
    
    // 启动主线程事件循环（在单独线程模拟）
    thread main_thread([&loop]() {
        loop.run();
    });
    
    cout << "\n模拟多个子线程提交任务" << endl;
    
    // 子线程1
    thread worker1([&loop]() {
        this_thread::sleep_for(chrono::milliseconds(100));
        
        loop.post([]() {
            cout << "  [主线程] 执行任务1" << endl;
        });
    });
    
    // 子线程2
    thread worker2([&loop]() {
        this_thread::sleep_for(chrono::milliseconds(150));
        
        loop.post([]() {
            cout << "  [主线程] 执行任务2" << endl;
        });
    });
    
    // 子线程3
    thread worker3([&loop]() {
        this_thread::sleep_for(chrono::milliseconds(200));
        
        loop.post([]() {
            cout << "  [主线程] 执行任务3" << endl;
        });
    });
    
    // 等待所有工作完成
    worker1.join();
    worker2.join();
    worker3.join();
    
    this_thread::sleep_for(chrono::milliseconds(100));
    
    // 停止事件循环
    loop.stop();
    main_thread.join();
    
    cout << "\n✅ 生产级方案特点：" << endl;
    cout << "  完整的事件循环机制" << endl;
    cout << "  支持多个子线程提交任务" << endl;
    cout << "  自动处理通知和唤醒" << endl;
    cout << "  可以优雅关闭" << endl;
}

// ============================================================================
// 八、实际应用：GUI 更新示例
// ============================================================================

void demonstrate_gui_pattern() {
    cout << "\n=== 8. 实际应用：GUI 更新模式 ===" << endl;
    
    TaskQueue ui_queue;
    
    cout << "\n模拟 GUI 程序：后台下载，主线程更新界面" << endl;
    
    // 模拟下载任务
    auto download_file = [&ui_queue](const string& url) {
        thread([url, &ui_queue]() {
            cout << "  [下载线程] 下载: " << url << endl;
            
            // 更新进度：0%
            ui_queue.post([url]() {
                cout << "    [UI 线程] 更新进度: " << url << " - 0%" << endl;
            });
            
            this_thread::sleep_for(chrono::milliseconds(100));
            
            // 更新进度：50%
            ui_queue.post([url]() {
                cout << "    [UI 线程] 更新进度: " << url << " - 50%" << endl;
            });
            
            this_thread::sleep_for(chrono::milliseconds(100));
            
            // 更新进度：100%
            ui_queue.post([url]() {
                cout << "    [UI 线程] 更新进度: " << url << " - 100%" << endl;
            });
            
        }).detach();
    };
    
    // 启动多个下载
    download_file("file1.zip");
    download_file("file2.zip");
    
    // UI 主循环
    cout << "[UI 线程] 运行主循环..." << endl;
    for (int i = 0; i < 15; i++) {
        this_thread::sleep_for(chrono::milliseconds(50));
        ui_queue.execute_all();  // ✅ 执行所有 UI 更新
    }
    
    cout << "\n关键点：" << endl;
    cout << "  ✅ 下载在子线程进行（不阻塞 UI）" << endl;
    cout << "  ✅ UI 更新在主线程执行（线程安全）" << endl;
    cout << "  ✅ 进度实时反馈给用户" << endl;
}

// ============================================================================
// 九、对比各种方法
// ============================================================================

void compare_methods() {
    cout << "\n=== 9. 各种方法对比 ===" << endl;
    
    cout << "\n┌─────────────────────┬──────────┬──────────┬──────────┬──────────┐" << endl;
    cout << "│ 方法                │ 易用性   │ 实时性   │ 灵活性   │ 推荐度   │" << endl;
    cout << "├─────────────────────┼──────────┼──────────┼──────────┼──────────┤" << endl;
    cout << "│ 1. 任务队列         │ ⭐⭐⭐⭐ │ ⭐⭐⭐   │ ⭐⭐⭐⭐ │ ⭐⭐⭐⭐⭐│" << endl;
    cout << "│ 2. 带通知的队列     │ ⭐⭐⭐   │ ⭐⭐⭐⭐⭐│ ⭐⭐⭐⭐ │ ⭐⭐⭐⭐ │" << endl;
    cout << "│ 3. 回调函数         │ ⭐⭐⭐⭐⭐│ ⭐⭐⭐⭐ │ ⭐⭐     │ ⭐⭐⭐   │" << endl;
    cout << "│ 4. 回调+队列        │ ⭐⭐⭐⭐ │ ⭐⭐⭐   │ ⭐⭐⭐⭐⭐│ ⭐⭐⭐⭐⭐│" << endl;
    cout << "│ 5. async/future     │ ⭐⭐⭐⭐⭐│ ⭐⭐     │ ⭐⭐     │ ⭐⭐⭐   │" << endl;
    cout << "│ 6. 完整事件循环     │ ⭐⭐⭐   │ ⭐⭐⭐⭐⭐│ ⭐⭐⭐⭐⭐│ ⭐⭐⭐⭐⭐│" << endl;
    cout << "└─────────────────────┴──────────┴──────────┴──────────┴──────────┘" << endl;
    
    cout << "\n推荐方案：" << endl;
    cout << "  🥇 简单场景：任务队列（方法1）" << endl;
    cout << "  🥈 需要回调：回调 + 队列（方法4）" << endl;
    cout << "  🥉 生产环境：完整事件循环（方法6）" << endl;
}

// ============================================================================
// 十、核心总结
// ============================================================================

void summary() {
    cout << "\n========================================" << endl;
    cout << "           核心总结" << endl;
    cout << "========================================" << endl;
    
    cout << "\n问题：C++ 怎么从子线程切换到主线程执行？" << endl;
    cout << "  答案：通过任务队列 + 主线程事件循环" << endl;
    
    cout << "\n核心思路：" << endl;
    cout << "  1. 子线程完成工作后，将任务放入队列" << endl;
    cout << "  2. 主线程定期检查队列，执行任务" << endl;
    cout << "  3. 使用 mutex 保证线程安全" << endl;
    cout << "  4. 使用 condition_variable 实现通知" << endl;
    
    cout << "\n6 种实现方法：" << endl;
    cout << "  1. 任务队列（轮询）- 推荐 ⭐⭐⭐⭐⭐" << endl;
    cout << "  2. 带通知的队列（阻塞）- 推荐 ⭐⭐⭐⭐" << endl;
    cout << "  3. 回调函数（仍在子线程）- 不推荐 ⭐⭐" << endl;
    cout << "  4. 回调 + 队列（完美）- 推荐 ⭐⭐⭐⭐⭐" << endl;
    cout << "  5. async/future（简单但受限）- 适合简单场景 ⭐⭐⭐" << endl;
    cout << "  6. 完整事件循环（生产级）- 推荐 ⭐⭐⭐⭐⭐" << endl;
    
    cout << "\n典型应用场景：" << endl;
    cout << "  ✅ GUI 程序：子线程下载，主线程更新界面" << endl;
    cout << "  ✅ 游戏开发：子线程加载，主线程渲染" << endl;
    cout << "  ✅ 网络编程：子线程接收，主线程处理" << endl;
    cout << "  ✅ 数据处理：子线程计算，主线程显示" << endl;
    
    cout << "\n关键要点：" << endl;
    cout << "  1. C++ 没有直接的线程切换 API" << endl;
    cout << "  2. 通过消息传递机制实现" << endl;
    cout << "  3. 主线程必须有事件循环" << endl;
    cout << "  4. 使用 mutex 保证线程安全" << endl;
    cout << "  5. GUI 框架已内置此机制（Qt、wxWidgets）" << endl;
    
    cout << "\n实现模板：" << endl;
    cout << "  class TaskQueue {" << endl;
    cout << "      queue<function<void()>> tasks;" << endl;
    cout << "      mutex mtx;" << endl;
    cout << "      " << endl;
    cout << "      void post(function<void()> task) {" << endl;
    cout << "          lock_guard<mutex> lock(mtx);" << endl;
    cout << "          tasks.push(task);" << endl;
    cout << "      }" << endl;
    cout << "      " << endl;
    cout << "      void execute_all() {" << endl;
    cout << "          lock_guard<mutex> lock(mtx);" << endl;
    cout << "          while (!tasks.empty()) {" << endl;
    cout << "              tasks.front()();  // 在主线程执行" << endl;
    cout << "              tasks.pop();" << endl;
    cout << "          }" << endl;
    cout << "      }" << endl;
    cout << "  };" << endl;
    
    cout << "\n使用示例：" << endl;
    cout << "  TaskQueue queue;" << endl;
    cout << "  " << endl;
    cout << "  // 子线程提交任务" << endl;
    cout << "  thread([&queue]() {" << endl;
    cout << "      auto result = compute();" << endl;
    cout << "      queue.post([result]() {" << endl;
    cout << "          cout << result;  // 在主线程执行" << endl;
    cout << "      });" << endl;
    cout << "  }).detach();" << endl;
    cout << "  " << endl;
    cout << "  // 主线程事件循环" << endl;
    cout << "  while (running) {" << endl;
    cout << "      queue.execute_all();" << endl;
    cout << "      this_thread::sleep_for(16ms);  // 60 FPS" << endl;
    cout << "  }" << endl;
    
    cout << "\n与其他语言对比：" << endl;
    cout << "  JavaScript: setTimeout(() => {}, 0) 切换到事件循环" << endl;
    cout << "  Python:     asyncio.run_in_executor() 切换线程" << endl;
    cout << "  C#:         Dispatcher.Invoke() 切换到 UI 线程" << endl;
    cout << "  C++:        任务队列 + 事件循环（手动实现）" << endl;
    
    cout << "\n记忆口诀：" << endl;
    cout << "  子线程完成任务后，结果放入队列里" << endl;
    cout << "  主线程轮询执行它，安全又清晰" << endl;
    cout << "  GUI 更新要主线程，千万别忘记" << endl;
    cout << "  任务队列是核心，生产级工具" << endl;
    
    cout << "\n========================================" << endl;
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "\n╔═══════════════════════════════════════════════════╗" << endl;
    cout << "║     从子线程切换到主线程执行的方法            ║" << endl;
    cout << "║   任务队列、事件循环、线程间通信              ║" << endl;
    cout << "╚═══════════════════════════════════════════════════╝" << endl;
    
    // 1. 核心概念
    explain_thread_switching();
    
    // 2. 方法1：任务队列
    demonstrate_task_queue();
    
    // 3. 方法2：带通知的队列
    demonstrate_blocking_queue();
    
    // 4. 方法3：回调函数
    demonstrate_callback();
    
    // 5. 方法4：回调 + 队列
    demonstrate_callback_with_queue();
    
    // 6. 方法5：async/future
    demonstrate_async_future();
    
    // 7. 方法6：完整事件循环
    demonstrate_event_loop();
    
    // 8. 实际应用：GUI 模式
    demonstrate_gui_pattern();
    
    // 9. 对比各种方法
    compare_methods();
    
    // 10. 总结
    summary();
    
    return 0;
}
