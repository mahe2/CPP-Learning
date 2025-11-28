/*
 * thread.detach() 详解
 * 主题：detach 的含义和危险性
 * 核心：线程对象 vs 线程本身，生命周期管理
 */

#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

// ============================================================================
// 一、thread 对象 vs 线程本身
// ============================================================================

void explain_thread_object_vs_thread() {
    cout << "\n=== 1. thread 对象 vs 线程本身 ===" << endl;
    
    cout << "\n核心概念：" << endl;
    cout << "  thread 对象：C++ 中的管理工具（管理者）" << endl;
    cout << "  线程本身：操作系统中的执行单元（被管理者）" << endl;
    
    cout << "\n类比：" << endl;
    cout << "  ┌─────────────────┬──────────────────┬──────────────────┐" << endl;
    cout << "  │ 概念            │ thread 对象      │ 线程本身         │" << endl;
    cout << "  ├─────────────────┼──────────────────┼──────────────────┤" << endl;
    cout << "  │ 是什么          │ 遥控器           │ 电视机           │" << endl;
    cout << "  │ 在哪里          │ C++ 代码中       │ 操作系统中       │" << endl;
    cout << "  │ 作用            │ 管理和控制       │ 实际执行代码     │" << endl;
    cout << "  │ detach 后       │ 失去控制         │ 继续运行         │" << endl;
    cout << "  └─────────────────┴──────────────────┴──────────────────┘" << endl;
    
    cout << "\n示例：" << endl;
    cout << "  thread t([]() {" << endl;
    cout << "      // 这是线程本身在执行" << endl;
    cout << "  });" << endl;
    cout << "  // t 是管理这个线程的对象" << endl;
    
    cout << "\n重要理解：" << endl;
    cout << "  销毁 thread 对象 ≠ 终止线程" << endl;
    cout << "  thread 对象只是管理工具，不是线程本身" << endl;
}

// ============================================================================
// 二、detach() 的含义
// ============================================================================

void explain_detach_meaning() {
    cout << "\n=== 2. detach() 的含义 ===" << endl;
    
    cout << "\ndetach() 做了什么？" << endl;
    cout << "  1. 断开 thread 对象和线程的绑定关系" << endl;
    cout << "  2. thread 对象不再管理线程" << endl;
    cout << "  3. 线程成为\"守护线程\"（daemon thread）" << endl;
    cout << "  4. 线程独立运行，不受 thread 对象影响" << endl;
    
    cout << "\n图示：" << endl;
    cout << "  detach 之前：" << endl;
    cout << "    thread 对象 [t] ←――管理――→ [线程 123]" << endl;
    cout << "         ↓                        ↓" << endl;
    cout << "    可以 join()              等待中..." << endl;
    
    cout << "\n  t.detach() 执行：" << endl;
    cout << "    thread 对象 [t]    ✗    [线程 123]" << endl;
    cout << "         ↓                        ↓" << endl;
    cout << "    失去管理权              独立运行" << endl;
    
    cout << "\n  detach 之后：" << endl;
    cout << "    thread 对象 [t] - 空壳（不代表任何线程）" << endl;
    cout << "    线程 [123]     - 继续运行（直到完成或主线程结束）" << endl;
    
    cout << "\n\"不再代表任何线程\"的含义：" << endl;
    cout << "  - t.joinable() 返回 false" << endl;
    cout << "  - 不能再 t.join() （会抛异常）" << endl;
    cout << "  - 不能再 t.detach() （会抛异常）" << endl;
    cout << "  - t 变成了一个空壳对象" << endl;
}

// ============================================================================
// 三、detach 实际演示
// ============================================================================

void demonstrate_detach() {
    cout << "\n=== 3. detach 实际演示 ===" << endl;
    
    cout << "\n创建线程：" << endl;
    thread t([]() {
        for (int i = 1; i <= 5; i++) {
            this_thread::sleep_for(chrono::milliseconds(200));
            cout << "    [子线程] 工作中... " << i << endl;
        }
        cout << "    [子线程] 完成！" << endl;
    });
    
    cout << "  线程 ID: " << t.get_id() << endl;
    cout << "  joinable? " << (t.joinable() ? "是" : "否") << endl;
    
    cout << "\n调用 detach()：" << endl;
    t.detach();
    
    cout << "  detach 后 - 线程 ID: " << (t.get_id() == thread::id() ? "空ID" : "有ID") << endl;
    cout << "  detach 后 - joinable? " << (t.joinable() ? "是" : "否") << endl;
    
    cout << "\n主线程继续工作：" << endl;
    for (int i = 1; i <= 3; i++) {
        this_thread::sleep_for(chrono::milliseconds(150));
        cout << "  [主线程] 工作中... " << i << endl;
    }
    
    cout << "\n主线程即将结束..." << endl;
    this_thread::sleep_for(chrono::milliseconds(500));  // 等一下让子线程完成
    cout << "  主线程结束" << endl;
}

// ============================================================================
// 四、detach vs join 对比
// ============================================================================

void compare_detach_vs_join() {
    cout << "\n=== 4. detach vs join 对比 ===" << endl;
    
    cout << "\n方式1: join() - 等待线程" << endl;
    {
        cout << "  创建线程..." << endl;
        thread t1([]() {
            this_thread::sleep_for(chrono::milliseconds(200));
            cout << "    [join 线程] 完成" << endl;
        });
        
        cout << "  主线程等待中..." << endl;
        t1.join();  // 阻塞，直到线程结束
        cout << "  主线程继续（线程已结束）" << endl;
    }
    
    cout << "\n方式2: detach() - 分离线程" << endl;
    {
        cout << "  创建线程..." << endl;
        thread t2([]() {
            this_thread::sleep_for(chrono::milliseconds(200));
            cout << "    [detach 线程] 完成" << endl;
        });
        
        cout << "  调用 detach()..." << endl;
        t2.detach();
        cout << "  主线程立即继续（不等待）" << endl;
        this_thread::sleep_for(chrono::milliseconds(300));  // 等待让线程完成
    }
    
    cout << "\n对比总结：" << endl;
    cout << "  ┌──────────────────┬────────────────┬────────────────┐" << endl;
    cout << "  │ 特性             │ join()         │ detach()       │" << endl;
    cout << "  ├──────────────────┼────────────────┼────────────────┤" << endl;
    cout << "  │ 主线程是否等待   │ ✅ 阻塞等待   │ ❌ 立即返回   │" << endl;
    cout << "  │ thread 对象状态  │ 有效 → 无效   │ 立即无效       │" << endl;
    cout << "  │ 线程关系         │ 父子关系       │ 独立关系       │" << endl;
    cout << "  │ 生命周期管理     │ 主线程控制     │ 自己控制       │" << endl;
    cout << "  │ 适用场景         │ 需要等待结果   │ 后台任务       │" << endl;
    cout << "  └──────────────────┴────────────────┴────────────────┘" << endl;
}

// ============================================================================
// 五、detach 后和主线程的关系
// ============================================================================

void explain_relationship_after_detach() {
    cout << "\n=== 5. detach 后和主线程的关系 ===" << endl;
    
    cout << "\n问题：detach 后，和主线程无关了吗？" << endl;
    cout << "  答案：❌ 不是！仍然有关系" << endl;
    
    cout << "\n关键关系：" << endl;
    cout << "  1. 主线程结束 → 进程结束 → 所有线程强制终止" << endl;
    cout << "  2. detach 线程仍属于同一进程" << endl;
    cout << "  3. 共享进程资源（内存、文件描述符等）" << endl;
    cout << "  4. 只是 thread 对象不再管理，不是完全独立" << endl;
    
    cout << "\n生命周期关系：" << endl;
    cout << "  ┌────────────────────────────────────────────┐" << endl;
    cout << "  │            进程生命周期                    │" << endl;
    cout << "  │  ┌──────────────────────────────────┐      │" << endl;
    cout << "  │  │      主线程生命周期              │      │" << endl;
    cout << "  │  │  ┌────────────────────────┐      │      │" << endl;
    cout << "  │  │  │  detach 线程生命周期   │      │      │" << endl;
    cout << "  │  │  │                        │      │      │" << endl;
    cout << "  │  │  └────────────────────────┘      │      │" << endl;
    cout << "  │  │    ↑ 如果主线程结束，detach    │      │" << endl;
    cout << "  │  │      线程也会被强制终止         │      │" << endl;
    cout << "  │  └──────────────────────────────────┘      │" << endl;
    cout << "  └────────────────────────────────────────────┘" << endl;
    
    cout << "\n具体含义：" << endl;
    cout << "  ✅ 管理关系断开：thread 对象不再控制线程" << endl;
    cout << "  ✅ 不能 join：无法等待线程结束" << endl;
    cout << "  ✅ 独立运行：线程按自己的节奏执行" << endl;
    cout << "  ❌ 不是完全独立：仍在同一进程中" << endl;
    cout << "  ❌ 不是守护进程：进程结束时会被杀死" << endl;
}

// ============================================================================
// 六、detach 的危险性
// ============================================================================

void demonstrate_detach_danger() {
    cout << "\n=== 6. detach 的危险性 ===" << endl;
    
    cout << "\n⚠️ 危险1: 主线程提前结束" << endl;
    cout << "  问题：主线程结束 → 进程结束 → detach 线程被强制杀死" << endl;
    cout << "  示例：" << endl;
    cout << "    {" << endl;
    cout << "        thread t([]() {" << endl;
    cout << "            sleep(10);  // 需要 10 秒" << endl;
    cout << "        });" << endl;
    cout << "        t.detach();" << endl;
    cout << "    }  // main 结束 → 线程只执行了一部分就被杀" << endl;
    
    cout << "\n⚠️ 危险2: 访问已销毁的变量" << endl;
    cout << "  问题：detach 线程可能访问栈上的局部变量" << endl;
    cout << "  示例代码：" << endl;
    
    // 危险示例（注释掉，避免真的执行）
    cout << "    void dangerous_function() {" << endl;
    cout << "        int x = 10;  // 栈上的变量" << endl;
    cout << "        thread t([&x]() {  // ❌ 引用捕获" << endl;
    cout << "            sleep(1);" << endl;
    cout << "            cout << x;  // ❌ x 已经被销毁！" << endl;
    cout << "        });" << endl;
    cout << "        t.detach();" << endl;
    cout << "    }  // x 被销毁，但线程还在运行" << endl;
    
    cout << "\n⚠️ 危险3: 资源泄漏" << endl;
    cout << "  问题：detach 后无法知道线程是否完成" << endl;
    cout << "  - 无法确认资源是否释放" << endl;
    cout << "  - 无法获取返回值" << endl;
    cout << "  - 无法处理异常" << endl;
    
    cout << "\n⚠️ 危险4: 程序退出时的不确定性" << endl;
    cout << "  问题：detach 线程可能在 main 返回后仍在运行" << endl;
    cout << "  - 可能访问已销毁的全局对象" << endl;
    cout << "  - 可能在静态对象析构时执行" << endl;
    cout << "  - 行为未定义" << endl;
}

// ============================================================================
// 七、安全使用 detach
// ============================================================================

void safe_use_of_detach() {
    cout << "\n=== 7. 安全使用 detach ===" << endl;
    
    cout << "\n✅ 规则1: 确保主线程足够长寿" << endl;
    cout << "  方法：主线程等待足够长的时间或使用信号等待" << endl;
    
    cout << "\n✅ 规则2: 不要捕获局部变量的引用" << endl;
    cout << "  ❌ 错误：[&x]() { use(x); }" << endl;
    cout << "  ✅ 正确：[x]() { use(x); }  // 按值捕获" << endl;
    cout << "  ✅ 正确：使用 new 分配，线程内 delete" << endl;
    
    cout << "\n✅ 规则3: 只用于真正独立的后台任务" << endl;
    cout << "  适合：日志记录、监控、清理任务" << endl;
    cout << "  不适合：需要返回结果的任务" << endl;
    
    cout << "\n✅ 规则4: 使用 std::async 替代" << endl;
    cout << "  auto future = async(launch::async, task);" << endl;
    cout << "  // 更安全，可以获取返回值" << endl;
    
    cout << "\n安全的 detach 示例：" << endl;
    cout << "  void log_to_file(string message) {" << endl;
    cout << "      thread t([message]() {  // 按值捕获" << endl;
    cout << "          // 写入日志文件" << endl;
    cout << "      });" << endl;
    cout << "      t.detach();" << endl;
    cout << "  }" << endl;
    cout << "  // 日志任务独立，不需要返回值" << endl;
}

// ============================================================================
// 八、实际演示：主线程结束的影响
// ============================================================================

void demonstrate_main_thread_exit() {
    cout << "\n=== 8. 实际演示：主线程结束的影响 ===" << endl;
    
    cout << "\n场景1: 主线程等待，detach 线程完成" << endl;
    {
        thread t([]() {
            for (int i = 1; i <= 3; i++) {
                this_thread::sleep_for(chrono::milliseconds(100));
                cout << "    [detach线程] " << i << "/3" << endl;
            }
            cout << "    [detach线程] ✅ 完成" << endl;
        });
        t.detach();
        
        cout << "  主线程等待 500ms..." << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << "  主线程结束" << endl;
    }
    
    cout << "\n场景2: 主线程不等待，detach 线程被中断（模拟）" << endl;
    cout << "  说明：如果主线程立即返回，detach 线程会被杀死" << endl;
    cout << "  {" << endl;
    cout << "      thread t([]() {" << endl;
    cout << "          sleep(10);  // 需要 10 秒" << endl;
    cout << "      });" << endl;
    cout << "      t.detach();" << endl;
    cout << "  }  // ❌ main 立即返回，线程被杀死" << endl;
}

// ============================================================================
// 九、核心总结
// ============================================================================

void summary() {
    cout << "\n========================================" << endl;
    cout << "           核心总结" << endl;
    cout << "========================================" << endl;
    
    cout << "\n问题1: \"线程对象不再代表任何线程\"是什么意思？" << endl;
    cout << "  答案：" << endl;
    cout << "    - thread 对象是管理工具，不是线程本身" << endl;
    cout << "    - detach() 断开管理关系" << endl;
    cout << "    - 对象变成空壳，线程继续运行" << endl;
    cout << "    - 类比：遥控器失效，电视继续播放" << endl;
    
    cout << "\n问题2: detach 后，就和主线程无关了吗？" << endl;
    cout << "  答案：❌ 不是！" << endl;
    cout << "    - 仍在同一进程中" << endl;
    cout << "    - 主线程结束 → 进程结束 → detach 线程被杀" << endl;
    cout << "    - 共享进程资源" << endl;
    cout << "    - 只是 thread 对象不再管理" << endl;
    
    cout << "\ndetach() 的含义：" << endl;
    cout << "  1. 断开 thread 对象和线程的绑定" << endl;
    cout << "  2. thread 对象失去管理权" << endl;
    cout << "  3. 线程独立运行" << endl;
    cout << "  4. 但仍属于同一进程" << endl;
    
    cout << "\ndetach vs join：" << endl;
    cout << "  ┌──────────────────┬────────────────┬────────────────┐" << endl;
    cout << "  │ 特性             │ join()         │ detach()       │" << endl;
    cout << "  ├──────────────────┼────────────────┼────────────────┤" << endl;
    cout << "  │ 主线程           │ 阻塞等待       │ 立即返回       │" << endl;
    cout << "  │ 管理关系         │ 保持到结束     │ 立即断开       │" << endl;
    cout << "  │ 线程独立性       │ 受管理         │ 独立运行       │" << endl;
    cout << "  │ 主线程结束影响   │ 必须先join     │ 线程被杀       │" << endl;
    cout << "  │ 安全性           │ ✅ 更安全     │ ⚠️ 危险       │" << endl;
    cout << "  └──────────────────┴────────────────┴────────────────┘" << endl;
    
    cout << "\ndetach 的危险性：" << endl;
    cout << "  ⚠️ 主线程提前结束 → 线程被杀" << endl;
    cout << "  ⚠️ 访问已销毁的变量 → 未定义行为" << endl;
    cout << "  ⚠️ 无法获取返回值" << endl;
    cout << "  ⚠️ 无法处理异常" << endl;
    
    cout << "\n安全使用 detach：" << endl;
    cout << "  ✅ 确保主线程足够长寿" << endl;
    cout << "  ✅ 按值捕获变量，不用引用" << endl;
    cout << "  ✅ 只用于独立的后台任务" << endl;
    cout << "  ✅ 考虑使用 std::async 代替" << endl;
    
    cout << "\n推荐做法：" << endl;
    cout << "  1. 默认使用 join()（更安全）" << endl;
    cout << "  2. 需要后台任务时考虑 detach()" << endl;
    cout << "  3. 需要返回值时使用 std::async" << endl;
    cout << "  4. 复杂场景使用线程池" << endl;
    
    cout << "\n类比记忆：" << endl;
    cout << "  thread 对象 = 遥控器" << endl;
    cout << "  线程本身 = 电视机" << endl;
    cout << "  " << endl;
    cout << "  join():   遥控器一直有效，等电视节目结束" << endl;
    cout << "  detach(): 扔掉遥控器，电视继续播放" << endl;
    cout << "            但关掉电源（主线程结束），电视也关" << endl;
    
    cout << "\n记忆口诀：" << endl;
    cout << "  对象管理线程体，detach 断开关系" << endl;
    cout << "  线程继续独立跑，进程结束才终止" << endl;
    cout << "  对象不代表线程，只是失去控制" << endl;
    cout << "  主线程要等待，否则子线程夭折" << endl;
    
    cout << "\n========================================" << endl;
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "\n╔═══════════════════════════════════════════════════╗" << endl;
    cout << "║         thread.detach() 详解                      ║" << endl;
    cout << "║   线程对象 vs 线程本身，生命周期管理             ║" << endl;
    cout << "╚═══════════════════════════════════════════════════╝" << endl;
    
    // 1. thread 对象 vs 线程本身
    explain_thread_object_vs_thread();
    
    // 2. detach() 的含义
    explain_detach_meaning();
    
    // 3. detach 实际演示
    demonstrate_detach();
    
    // 4. detach vs join 对比
    compare_detach_vs_join();
    
    // 5. detach 后和主线程的关系
    explain_relationship_after_detach();
    
    // 6. detach 的危险性
    demonstrate_detach_danger();
    
    // 7. 安全使用 detach
    safe_use_of_detach();
    
    // 8. 实际演示：主线程结束的影响
    demonstrate_main_thread_exit();
    
    // 9. 总结
    summary();
    
    return 0;
}
