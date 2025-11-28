/*
 * 线程执行时机详解
 * 核心：线程在创建时就开始执行，join() 只是等待结束
 */

#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

// ============================================================================
// 一、证明：线程在创建时就开始执行
// ============================================================================

void prove_immediate_execution() {
    cout << "\n=== 1. 证明：线程在创建时就开始执行 ===" << endl;
    
    cout << "\n实验：创建线程后延迟 join" << endl;
    
    auto start = chrono::high_resolution_clock::now();
    
    cout << "[主线程] 准备创建线程..." << endl;
    
    // 创建线程：线程立即开始执行！
    thread t([]() {
        cout << "  [子线程] 开始执行！（线程已经开始工作）" << endl;
        this_thread::sleep_for(chrono::seconds(2));
        cout << "  [子线程] 工作完成！" << endl;
    });
    
    cout << "[主线程] 线程已创建，但还没 join" << endl;
    
    // 主线程做其他事（模拟延迟）
    cout << "[主线程] 先做其他事情..." << endl;
    this_thread::sleep_for(chrono::seconds(1));
    cout << "[主线程] 其他事情完成，现在 join" << endl;
    
    // join：等待线程结束
    t.join();
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(end - start);
    
    cout << "[主线程] join 返回" << endl;
    cout << "\n总耗时: " << duration.count() << " 秒" << endl;
    cout << "分析：总耗时约 2 秒（不是 3 秒！）" << endl;
    cout << "      证明：线程创建时就开始工作，主线程工作和子线程工作是并发的" << endl;
}

// ============================================================================
// 二、详细时间线分析
// ============================================================================

void detailed_timeline() {
    cout << "\n=== 2. 详细时间线分析 ===" << endl;
    
    auto start = chrono::high_resolution_clock::now();
    
    auto print_time = [start]() {
        auto now = chrono::high_resolution_clock::now();
        auto ms = chrono::duration_cast<chrono::milliseconds>(now - start).count();
        cout << "[" << ms << "ms] ";
    };
    
    print_time(); cout << "主线程：开始" << endl;
    
    // 创建线程
    thread t([start, print_time]() {
        print_time(); cout << "子线程：立即开始执行！" << endl;
        
        this_thread::sleep_for(chrono::milliseconds(300));
        print_time(); cout << "子线程：工作中..." << endl;
        
        this_thread::sleep_for(chrono::milliseconds(300));
        print_time(); cout << "子线程：即将完成" << endl;
        
        this_thread::sleep_for(chrono::milliseconds(200));
        print_time(); cout << "子线程：完成！" << endl;
    });
    
    print_time(); cout << "主线程：线程已创建（子线程已经在跑了）" << endl;
    
    // 主线程做其他事
    this_thread::sleep_for(chrono::milliseconds(100));
    print_time(); cout << "主线程：做其他事情..." << endl;
    
    this_thread::sleep_for(chrono::milliseconds(200));
    print_time(); cout << "主线程：准备 join" << endl;
    
    // join：等待子线程结束
    t.join();
    
    print_time(); cout << "主线程：join 返回（子线程已结束）" << endl;
    
    cout << "\n观察结果：" << endl;
    cout << "  子线程在 0ms 附近就开始了（创建后立即执行）" << endl;
    cout << "  主线程在 300ms 才 join" << endl;
    cout << "  但总耗时约 800ms（子线程工作时间）" << endl;
    cout << "  说明：主线程和子线程是并发执行的" << endl;
}

// ============================================================================
// 三、join() 的真正作用
// ============================================================================

void explain_join_role() {
    cout << "\n=== 3. join() 的真正作用 ===" << endl;
    
    cout << "\n核心理解：" << endl;
    cout << "  thread t(task);  ← 线程创建并立即开始执行" << endl;
    cout << "  t.join();        ← 等待线程结束（阻塞）" << endl;
    
    cout << "\njoin() 不是启动线程，而是等待线程！" << endl;
    
    cout << "\n详细过程：" << endl;
    cout << "  1. thread t(task);" << endl;
    cout << "     ↓" << endl;
    cout << "     操作系统创建新线程" << endl;
    cout << "     ↓" << endl;
    cout << "     新线程立即开始执行 task" << endl;
    cout << "     ↓" << endl;
    cout << "     主线程继续往下走（不等待）" << endl;
    cout << "     ↓" << endl;
    cout << "  2. 主线程做其他事..." << endl;
    cout << "     ↓" << endl;
    cout << "  3. t.join();" << endl;
    cout << "     ↓" << endl;
    cout << "     检查线程是否完成？" << endl;
    cout << "     ├─ 已完成 → 立即返回" << endl;
    cout << "     └─ 未完成 → 阻塞等待，直到完成" << endl;
}

// ============================================================================
// 四、对比：如果 join() 立即调用 vs 延迟调用
// ============================================================================

void compare_join_timing() {
    cout << "\n=== 4. 对比：立即 join vs 延迟 join ===" << endl;
    
    cout << "\n方式1：立即 join（无并发效果）" << endl;
    {
        auto start = chrono::high_resolution_clock::now();
        
        thread t([]() {
            this_thread::sleep_for(chrono::milliseconds(200));
        });
        t.join();  // ❌ 立即等待，主线程被阻塞
        
        // 主线程做其他事
        this_thread::sleep_for(chrono::milliseconds(200));
        
        auto duration = chrono::duration_cast<chrono::milliseconds>(
            chrono::high_resolution_clock::now() - start
        );
        cout << "  总耗时: " << duration.count() << " ms（约 400ms）" << endl;
        cout << "  分析：串行执行，没有利用并发" << endl;
    }
    
    cout << "\n方式2：延迟 join（有并发效果）" << endl;
    {
        auto start = chrono::high_resolution_clock::now();
        
        thread t([]() {
            this_thread::sleep_for(chrono::milliseconds(200));
        });
        
        // 主线程先做其他事（子线程同时在运行）
        this_thread::sleep_for(chrono::milliseconds(200));
        
        t.join();  // ✅ 这时子线程可能已经完成了
        
        auto duration = chrono::duration_cast<chrono::milliseconds>(
            chrono::high_resolution_clock::now() - start
        );
        cout << "  总耗时: " << duration.count() << " ms（约 200ms）" << endl;
        cout << "  分析：并发执行，充分利用了多线程" << endl;
    }
    
    cout << "\n结论：" << endl;
    cout << "  join() 的位置很重要！" << endl;
    cout << "  ✅ 延迟 join 可以让主线程和子线程并发工作" << endl;
    cout << "  ❌ 立即 join 会退化成串行执行" << endl;
}

// ============================================================================
// 五、常见误区
// ============================================================================

void common_misconceptions() {
    cout << "\n=== 5. 常见误区 ===" << endl;
    
    cout << "\n❌ 误区1：join() 启动线程" << endl;
    cout << "   错误理解：thread t(task); 只是准备，join() 才启动" << endl;
    cout << "   正确理解：thread t(task); 立即启动，join() 只是等待" << endl;
    
    cout << "\n❌ 误区2：线程创建不执行，等 join 才执行" << endl;
    cout << "   错误理解：thread t(task); 什么都不做，t.join(); 开始执行" << endl;
    cout << "   正确理解：thread t(task); 已经在执行了，t.join(); 等待结束" << endl;
    
    cout << "\n❌ 误区3：不 join 线程就不会运行" << endl;
    cout << "   错误理解：必须 join 线程才会工作" << endl;
    cout << "   正确理解：线程立即工作，join 只是为了同步和资源管理" << endl;
    
    cout << "\n✅ 正确理解：" << endl;
    cout << "   thread t(task);  ← 创建 + 立即启动" << endl;
    cout << "   // 此时：主线程和子线程并发运行" << endl;
    cout << "   t.join();        ← 等待子线程结束（可能已经结束了）" << endl;
}

// ============================================================================
// 六、实际演示：join 时线程可能已经完成
// ============================================================================

void demonstrate_thread_may_finish_before_join() {
    cout << "\n=== 6. 演示：join 时线程可能已经完成 ===" << endl;
    
    cout << "\n场景：快速任务" << endl;
    
    thread t([]() {
        cout << "  [子线程] 快速任务开始" << endl;
        this_thread::sleep_for(chrono::milliseconds(50));
        cout << "  [子线程] 快速任务完成" << endl;
    });
    
    cout << "[主线程] 线程已创建" << endl;
    
    // 主线程做一个慢任务
    cout << "[主线程] 做一个慢任务..." << endl;
    this_thread::sleep_for(chrono::milliseconds(200));
    cout << "[主线程] 慢任务完成" << endl;
    
    cout << "[主线程] 准备 join（子线程可能已经完成了）" << endl;
    t.join();  // 很可能立即返回，因为子线程早就完成了
    cout << "[主线程] join 返回（立即返回，因为子线程早就完成了）" << endl;
    
    cout << "\n关键点：" << endl;
    cout << "  如果线程在 join 之前就完成了，join 会立即返回" << endl;
    cout << "  join 不是唤醒线程，而是等待线程（如果需要的话）" << endl;
}

// ============================================================================
// 七、对比其他语言
// ============================================================================

void compare_with_other_languages() {
    cout << "\n=== 7. 对比其他语言 ===" << endl;
    
    cout << "\nJava：" << endl;
    cout << "  Thread t = new Thread(task);" << endl;
    cout << "  t.start();  ← ✅ 明确调用 start() 才启动" << endl;
    cout << "  t.join();   ← 等待结束" << endl;
    
    cout << "\nC++：" << endl;
    cout << "  thread t(task);  ← ✅ 创建时就启动（没有 start）" << endl;
    cout << "  t.join();        ← 等待结束" << endl;
    
    cout << "\n区别：" << endl;
    cout << "  Java：创建和启动是分离的（new + start）" << endl;
    cout << "  C++：创建即启动（构造函数就启动）" << endl;
    
    cout << "\nC++ 为什么这样设计？" << endl;
    cout << "  1. RAII 原则：资源获取即初始化" << endl;
    cout << "  2. 简化接口：减少一步操作" << endl;
    cout << "  3. 避免遗忘：不会忘记 start" << endl;
}

// ============================================================================
// 八、实际应用场景
// ============================================================================

void practical_scenarios() {
    cout << "\n=== 8. 实际应用场景 ===" << endl;
    
    cout << "\n场景1：充分利用并发" << endl;
    cout << "  // ✅ 好的做法" << endl;
    cout << "  thread t1(task1);" << endl;
    cout << "  thread t2(task2);  ← t1 和 t2 已经在跑了" << endl;
    cout << "  thread t3(task3);  ← 三个线程并发执行" << endl;
    cout << "  do_main_work();    ← 主线程也在工作" << endl;
    cout << "  t1.join();" << endl;
    cout << "  t2.join();" << endl;
    cout << "  t3.join();" << endl;
    
    cout << "\n场景2：批量任务" << endl;
    cout << "  vector<thread> threads;" << endl;
    cout << "  for (int i = 0; i < 10; i++) {" << endl;
    cout << "      threads.emplace_back(task, i);  ← 每个线程立即开始" << endl;
    cout << "  }" << endl;
    cout << "  // 此时 10 个线程都在并发执行" << endl;
    cout << "  for (auto& t : threads) {" << endl;
    cout << "      t.join();  ← 等待所有线程完成" << endl;
    cout << "  }" << endl;
    
    cout << "\n场景3：异步计算" << endl;
    {
        cout << "  实际演示：" << endl;
        
        int result = 0;
        thread t([&result]() {
            // 计算立即开始
            for (int i = 1; i <= 1000; i++) {
                result += i;
            }
        });
        
        cout << "    主线程：线程已创建，计算已经开始..." << endl;
        
        // 主线程做其他事
        cout << "    主线程：我先做其他事..." << endl;
        this_thread::sleep_for(chrono::milliseconds(10));
        
        // 需要结果时才 join
        cout << "    主线程：现在需要结果，join..." << endl;
        t.join();
        
        cout << "    结果: " << result << endl;
    }
}

// ============================================================================
// 九、核心总结
// ============================================================================

void summary() {
    cout << "\n========================================" << endl;
    cout << "           核心总结" << endl;
    cout << "========================================" << endl;
    
    cout << "\n问题：join() 之前，线程就开始执行了吗？" << endl;
    cout << "  答案：✅ 是的！线程在创建时就立即开始执行！" << endl;
    
    cout << "\n问题：join() 只是等待结果？" << endl;
    cout << "  答案：✅ 是的！join() 只是等待线程结束，不启动线程！" << endl;
    
    cout << "\n完整执行流程：" << endl;
    cout << "  ┌─────────────────────────────────────────┐" << endl;
    cout << "  │ 1. thread t(task);                      │" << endl;
    cout << "  │    ↓                                    │" << endl;
    cout << "  │    操作系统创建新线程                    │" << endl;
    cout << "  │    ↓                                    │" << endl;
    cout << "  │    ✅ 线程立即开始执行 task             │" << endl;
    cout << "  │    ↓                                    │" << endl;
    cout << "  │    主线程继续往下走（不等待）            │" << endl;
    cout << "  │                                         │" << endl;
    cout << "  │ 2. 主线程做其他事...                    │" << endl;
    cout << "  │    （此时主线程和子线程并发执行）        │" << endl;
    cout << "  │                                         │" << endl;
    cout << "  │ 3. t.join();                            │" << endl;
    cout << "  │    ↓                                    │" << endl;
    cout << "  │    检查子线程是否完成？                  │" << endl;
    cout << "  │    ├─ 已完成 → 立即返回                 │" << endl;
    cout << "  │    └─ 未完成 → 阻塞等待直到完成         │" << endl;
    cout << "  └─────────────────────────────────────────┘" << endl;
    
    cout << "\n关键理解：" << endl;
    cout << "  1️⃣ 线程创建 = 线程启动（C++ 特性）" << endl;
    cout << "  2️⃣ join() = 等待线程结束（不是启动）" << endl;
    cout << "  3️⃣ join() 之前线程已经在执行了" << endl;
    cout << "  4️⃣ join() 可能立即返回（如果线程已完成）" << endl;
    cout << "  5️⃣ join() 可能阻塞等待（如果线程未完成）" << endl;
    
    cout << "\nC++ vs Java：" << endl;
    cout << "  ┌────────────┬────────────────────┬──────────────┐" << endl;
    cout << "  │ 语言       │ 创建               │ 启动         │" << endl;
    cout << "  ├────────────┼────────────────────┼──────────────┤" << endl;
    cout << "  │ Java       │ new Thread(task)   │ t.start()    │" << endl;
    cout << "  │ C++        │ thread t(task)     │ （立即启动） │" << endl;
    cout << "  └────────────┴────────────────────┴──────────────┘" << endl;
    
    cout << "\n实际应用技巧：" << endl;
    cout << "  ✅ 创建所有线程（它们立即开始并发执行）" << endl;
    cout << "  ✅ 主线程做其他工作（充分利用并发）" << endl;
    cout << "  ✅ 需要结果时才 join（同步点）" << endl;
    
    cout << "\n常见误区：" << endl;
    cout << "  ❌ 误区：join() 启动线程" << endl;
    cout << "  ✅ 正确：构造函数启动线程，join() 等待结束" << endl;
    
    cout << "\n记忆口诀：" << endl;
    cout << "  创建线程即启动，不用等到调 join" << endl;
    cout << "  join 只是做等待，线程早已在狂奔" << endl;
    cout << "  主线程子线程，同时在执行" << endl;
    cout << "  需要结果再 join，同步点上来会合" << endl;
    
    cout << "\n类比理解：" << endl;
    cout << "  线程就像外卖员：" << endl;
    cout << "  - thread t(送外卖) → 外卖员接单后立即出发" << endl;
    cout << "  - 你继续做其他事 → 你在家做自己的事" << endl;
    cout << "  - t.join() → 等外卖员到达（可能已经到了）" << endl;
    
    cout << "\n========================================" << endl;
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "\n╔═══════════════════════════════════════════════════╗" << endl;
    cout << "║         线程执行时机详解                          ║" << endl;
    cout << "║   线程在创建时就开始，join() 只是等待            ║" << endl;
    cout << "╚═══════════════════════════════════════════════════╝" << endl;
    
    // 1. 证明线程立即执行
    prove_immediate_execution();
    
    // 2. 详细时间线分析
    detailed_timeline();
    
    // 3. join() 的真正作用
    explain_join_role();
    
    // 4. 对比立即 join vs 延迟 join
    compare_join_timing();
    
    // 5. 常见误区
    common_misconceptions();
    
    // 6. 演示 join 时线程可能已完成
    demonstrate_thread_may_finish_before_join();
    
    // 7. 对比其他语言
    compare_with_other_languages();
    
    // 8. 实际应用场景
    practical_scenarios();
    
    // 9. 总结
    summary();
    
    return 0;
}
