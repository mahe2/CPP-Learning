/*
 * join() 的实际意义
 * 主题：为什么阻塞主线程是有价值的
 * 核心：同步、数据依赖、资源管理
 */

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <numeric>
#include <future>
using namespace std;

// ============================================================================
// 一、join() 的核心价值：同步和等待
// ============================================================================

void explain_join_value() {
    cout << "\n=== 1. join() 的核心价值 ===" << endl;
    
    cout << "\n为什么需要 join()？" << endl;
    cout << "  问题：线程是异步的，主线程不知道何时完成" << endl;
    cout << "  解决：join() 提供同步点，确保任务完成" << endl;
    
    cout << "\n阻塞不是缺点，而是特性：" << endl;
    cout << "  ✅ 阻塞 = 等待 = 同步" << endl;
    cout << "  ✅ 确保线程完成才继续" << endl;
    cout << "  ✅ 保证数据可用性" << endl;
    cout << "  ✅ 避免资源提前释放" << endl;
    
    cout << "\n如果不阻塞会怎样？" << endl;
    cout << "  ❌ 数据可能未就绪" << endl;
    cout << "  ❌ 资源可能被销毁" << endl;
    cout << "  ❌ 程序行为不可预测" << endl;
}

// ============================================================================
// 二、实际场景1：等待计算结果
// ============================================================================

void demonstrate_waiting_for_result() {
    cout << "\n=== 2. 场景1：等待计算结果 ===" << endl;
    
    cout << "\n任务：计算 1 到 1000000 的和" << endl;
    
    // 错误示例：不用 join
    cout << "\n❌ 错误方式：不等待结果" << endl;
    {
        long long result = 0;
        thread t([&result]() {
            this_thread::sleep_for(chrono::milliseconds(100));  // 模拟计算
            for (int i = 1; i <= 1000000; i++) {
                result += i;
            }
        });
        // 不 join，立即继续
        cout << "  结果: " << result << " ❌ 错误！（还没计算完）" << endl;
        t.join();  // 避免程序崩溃
    }
    
    // 正确示例：使用 join
    cout << "\n✅ 正确方式：等待结果" << endl;
    {
        long long result = 0;
        thread t([&result]() {
            this_thread::sleep_for(chrono::milliseconds(100));
            for (int i = 1; i <= 1000000; i++) {
                result += i;
            }
        });
        t.join();  // 等待计算完成
        cout << "  结果: " << result << " ✅ 正确！" << endl;
    }
    
    cout << "\n关键点：" << endl;
    cout << "  join() 确保在使用结果前，计算已经完成" << endl;
    cout << "  阻塞主线程是必要的，否则结果不可靠" << endl;
}

// ============================================================================
// 三、实际场景2：多线程协作
// ============================================================================

void demonstrate_parallel_computation() {
    cout << "\n=== 3. 场景2：多线程并行计算 ===" << endl;
    
    cout << "\n任务：4 个线程分别计算一部分，最后汇总" << endl;
    
    vector<long long> results(4, 0);
    vector<thread> threads;
    
    auto start = chrono::high_resolution_clock::now();
    
    // 创建 4 个线程，每个计算 250000 个数的和
    for (int i = 0; i < 4; i++) {
        threads.emplace_back([&results, i]() {
            long long sum = 0;
            int start = i * 250000 + 1;
            int end = (i + 1) * 250000;
            for (int j = start; j <= end; j++) {
                sum += j;
            }
            results[i] = sum;
            cout << "  [线程 " << i << "] 完成，部分和 = " << sum << endl;
        });
    }
    
    cout << "\n主线程：等待所有线程完成..." << endl;
    
    // ✅ 关键：必须 join 所有线程
    for (auto& t : threads) {
        t.join();  // 阻塞，等待每个线程完成
    }
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    
    // 汇总结果
    long long total = 0;
    for (long long r : results) {
        total += r;
    }
    
    cout << "\n所有线程完成！" << endl;
    cout << "  总和: " << total << endl;
    cout << "  耗时: " << duration.count() << " ms" << endl;
    
    cout << "\n如果不 join 会怎样？" << endl;
    cout << "  ❌ results 数组可能还没填充完" << endl;
    cout << "  ❌ 汇总的结果不完整" << endl;
    cout << "  ❌ 耗时统计不准确" << endl;
}

// ============================================================================
// 四、实际场景3：资源清理
// ============================================================================

void demonstrate_resource_cleanup() {
    cout << "\n=== 4. 场景3：资源清理 ===" << endl;
    
    cout << "\n任务：线程使用对象，主线程需要等待才能销毁" << endl;
    
    class SharedResource {
        string name;
    public:
        SharedResource(string n) : name(n) {
            cout << "  [资源] " << name << " 创建" << endl;
        }
        ~SharedResource() {
            cout << "  [资源] " << name << " 销毁" << endl;
        }
        void use() {
            cout << "  [资源] " << name << " 正在使用..." << endl;
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    };
    
    cout << "\n创建资源和线程：" << endl;
    {
        SharedResource res("数据库连接");
        
        thread t([&res]() {
            res.use();
            this_thread::sleep_for(chrono::milliseconds(200));
            res.use();
        });
        
        cout << "  主线程等待线程完成..." << endl;
        t.join();  // ✅ 必须等待！否则 res 会被提前销毁
        
        cout << "  线程完成，准备销毁资源" << endl;
    }
    cout << "  资源安全销毁" << endl;
    
    cout << "\n如果不 join（或 detach）会怎样？" << endl;
    cout << "  ❌ 程序崩溃（thread 析构时未 join/detach）" << endl;
    cout << "  如果 detach：" << endl;
    cout << "  ❌ res 被销毁，但线程还在使用 → 未定义行为" << endl;
}

// ============================================================================
// 五、实际场景4：顺序依赖
// ============================================================================

void demonstrate_sequential_dependency() {
    cout << "\n=== 5. 场景4：顺序依赖 ===" << endl;
    
    cout << "\n任务：步骤1 → 步骤2 → 步骤3（必须按顺序）" << endl;
    
    string step1_result, step2_result, step3_result;
    
    cout << "\n步骤1：下载数据" << endl;
    thread t1([&step1_result]() {
        this_thread::sleep_for(chrono::milliseconds(100));
        step1_result = "下载的数据";
        cout << "  [步骤1] 完成" << endl;
    });
    t1.join();  // ✅ 必须等待步骤1完成
    
    cout << "\n步骤2：处理数据（依赖步骤1的结果）" << endl;
    thread t2([&step1_result, &step2_result]() {
        this_thread::sleep_for(chrono::milliseconds(100));
        step2_result = step1_result + " → 处理后的数据";
        cout << "  [步骤2] 完成" << endl;
    });
    t2.join();  // ✅ 必须等待步骤2完成
    
    cout << "\n步骤3：保存数据（依赖步骤2的结果）" << endl;
    thread t3([&step2_result, &step3_result]() {
        this_thread::sleep_for(chrono::milliseconds(100));
        step3_result = step2_result + " → 已保存";
        cout << "  [步骤3] 完成" << endl;
    });
    t3.join();  // ✅ 必须等待步骤3完成
    
    cout << "\n最终结果: " << step3_result << endl;
    
    cout << "\n关键点：" << endl;
    cout << "  当任务有依赖关系时，join() 是必需的" << endl;
    cout << "  阻塞确保了正确的执行顺序" << endl;
}

// ============================================================================
// 六、join() vs detach() vs async
// ============================================================================

void compare_synchronization_methods() {
    cout << "\n=== 6. join() vs detach() vs async ===" << endl;
    
    cout << "\n场景对比：需要线程返回结果" << endl;
    
    cout << "\n方式1: thread + join（阻塞但可靠）" << endl;
    {
        int result = 0;
        thread t([&result]() {
            this_thread::sleep_for(chrono::milliseconds(100));
            result = 42;
        });
        t.join();  // 阻塞等待
        cout << "  结果: " << result << " ✅" << endl;
    }
    
    cout << "\n方式2: thread + detach（不阻塞但危险）" << endl;
    cout << "  {" << endl;
    cout << "      int result = 0;" << endl;
    cout << "      thread t([&result]() { result = 42; });" << endl;
    cout << "      t.detach();" << endl;
    cout << "      cout << result;  // ❌ 可能是 0，可能崩溃" << endl;
    cout << "  }" << endl;
    
    cout << "\n方式3: std::async（最佳，自动管理）" << endl;
    {
        auto future = async(launch::async, []() {
            this_thread::sleep_for(chrono::milliseconds(100));
            return 42;
        });
        // 主线程可以做其他事...
        cout << "  主线程继续工作..." << endl;
        this_thread::sleep_for(chrono::milliseconds(50));
        int result = future.get();  // 需要时才阻塞
        cout << "  结果: " << result << " ✅" << endl;
    }
    
    cout << "\n总结：" << endl;
    cout << "  ┌─────────────────┬──────────┬──────────┬──────────┐" << endl;
    cout << "  │ 方式            │ 阻塞     │ 安全性   │ 返回值   │" << endl;
    cout << "  ├─────────────────┼──────────┼──────────┼──────────┤" << endl;
    cout << "  │ thread + join   │ ✅ 阻塞 │ ✅ 安全 │ ⚠️ 间接 │" << endl;
    cout << "  │ thread + detach │ ❌ 不阻塞│ ❌ 危险 │ ❌ 无法 │" << endl;
    cout << "  │ std::async      │ ⚠️ 按需 │ ✅ 安全 │ ✅ 直接 │" << endl;
    cout << "  └─────────────────┴──────────┴──────────┴──────────┘" << endl;
}

// ============================================================================
// 七、join() 不阻塞主线程的替代方案
// ============================================================================

void alternatives_to_blocking() {
    cout << "\n=== 7. 如何减少阻塞影响？===" << endl;
    
    cout << "\n方案1: 主线程在 join 前做其他工作" << endl;
    {
        auto start = chrono::high_resolution_clock::now();
        
        thread t([]() {
            this_thread::sleep_for(chrono::milliseconds(200));
            cout << "  [子线程] 完成耗时任务" << endl;
        });
        
        // 主线程继续工作
        cout << "  [主线程] 做其他事情..." << endl;
        this_thread::sleep_for(chrono::milliseconds(100));
        cout << "  [主线程] 其他工作完成" << endl;
        
        // 然后等待
        cout << "  [主线程] 等待子线程..." << endl;
        t.join();
        
        auto end = chrono::high_resolution_clock::now();
        cout << "  总耗时: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
    }
    
    cout << "\n方案2: 使用 std::async（推荐）" << endl;
    {
        auto start = chrono::high_resolution_clock::now();
        
        auto future = async(launch::async, []() {
            this_thread::sleep_for(chrono::milliseconds(200));
            return "任务完成";
        });
        
        // 主线程继续工作
        cout << "  [主线程] 做其他事情..." << endl;
        this_thread::sleep_for(chrono::milliseconds(100));
        
        // 需要结果时才阻塞
        string result = future.get();
        cout << "  结果: " << result << endl;
        
        auto end = chrono::high_resolution_clock::now();
        cout << "  总耗时: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
    }
    
    cout << "\n方案3: 事件驱动/回调模式" << endl;
    cout << "  // 伪代码" << endl;
    cout << "  thread t([callback]() {" << endl;
    cout << "      auto result = do_work();" << endl;
    cout << "      callback(result);  // 完成时回调" << endl;
    cout << "  });" << endl;
    cout << "  t.detach();  // 不阻塞主线程" << endl;
    
    cout << "\n方案4: 线程池（高级）" << endl;
    cout << "  ThreadPool pool(4);" << endl;
    cout << "  auto future = pool.submit(task);" << endl;
    cout << "  // 主线程继续..." << endl;
    cout << "  auto result = future.get();  // 需要时获取" << endl;
}

// ============================================================================
// 八、核心总结
// ============================================================================

void summary() {
    cout << "\n========================================" << endl;
    cout << "           核心总结" << endl;
    cout << "========================================" << endl;
    
    cout << "\n问题：join() 阻塞主线程，还有意义吗？" << endl;
    cout << "  答案：✅ 非常有意义！阻塞是特性，不是缺点" << endl;
    
    cout << "\njoin() 的核心价值：" << endl;
    cout << "  1️⃣ 同步：确保线程完成" << endl;
    cout << "  2️⃣ 数据可用：保证结果就绪" << endl;
    cout << "  3️⃣ 资源安全：避免提前释放" << endl;
    cout << "  4️⃣ 顺序保证：依赖关系正确" << endl;
    cout << "  5️⃣ 避免崩溃：thread 必须 join 或 detach" << endl;
    
    cout << "\n阻塞不是缺点的原因：" << endl;
    cout << "  ✅ 很多场景需要等待结果" << endl;
    cout << "  ✅ 阻塞 = 同步点 = 可控性" << endl;
    cout << "  ✅ 不阻塞的话，行为不可预测" << endl;
    cout << "  ✅ 主线程可以在 join 前做其他工作" << endl;
    
    cout << "\n实际应用场景：" << endl;
    cout << "  ┌────────────────────────────┬───────────────┐" << endl;
    cout << "  │ 场景                       │ 需要 join?    │" << endl;
    cout << "  ├────────────────────────────┼───────────────┤" << endl;
    cout << "  │ 等待计算结果               │ ✅ 必须       │" << endl;
    cout << "  │ 多线程协作汇总             │ ✅ 必须       │" << endl;
    cout << "  │ 线程使用局部变量           │ ✅ 必须       │" << endl;
    cout << "  │ 任务有依赖关系             │ ✅ 必须       │" << endl;
    cout << "  │ 独立的后台任务（日志等）   │ ⚠️ 可 detach │" << endl;
    cout << "  └────────────────────────────┴───────────────┘" << endl;
    
    cout << "\njoin() vs detach() vs async：" << endl;
    cout << "  join():   阻塞等待，安全可靠，结果可控" << endl;
    cout << "  detach(): 不阻塞，危险，无法获取结果" << endl;
    cout << "  async:    按需阻塞，最灵活，推荐使用" << endl;
    
    cout << "\n如何减少阻塞影响？" << endl;
    cout << "  1. join 前让主线程做其他工作" << endl;
    cout << "  2. 使用 std::async（需要时才阻塞）" << endl;
    cout << "  3. 使用事件驱动/回调模式" << endl;
    cout << "  4. 使用线程池管理" << endl;
    
    cout << "\n类比理解：" << endl;
    cout << "  join() 就像网上购物：" << endl;
    cout << "  - 你下单了（创建线程）" << endl;
    cout << "  - 商家在准备（线程工作）" << endl;
    cout << "  - join() = 等待收货" << endl;
    cout << "  - 不等收货就使用？❌ 没货！" << endl;
    cout << "  - 等到收货再使用？✅ 安全！" << endl;
    
    cout << "\n什么时候 join() 不是最优选择？" << endl;
    cout << "  1. 需要多个线程并发（用线程池）" << endl;
    cout << "  2. 需要取消线程（用 std::future）" << endl;
    cout << "  3. 需要超时机制（用 condition_variable）" << endl;
    cout << "  4. 独立后台任务（用 detach，但要小心）" << endl;
    
    cout << "\n记忆口诀：" << endl;
    cout << "  join 阻塞不是错，同步等待价值多" << endl;
    cout << "  结果未完不能用，资源清理要等待" << endl;
    cout << "  主线程可先做事，需要结果再 join" << endl;
    cout << "  async 更加灵活，按需阻塞最优雅" << endl;
    
    cout << "\n========================================" << endl;
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "\n╔═══════════════════════════════════════════════════╗" << endl;
    cout << "║         join() 的实际意义                         ║" << endl;
    cout << "║   为什么阻塞主线程是有价值的                     ║" << endl;
    cout << "╚═══════════════════════════════════════════════════╝" << endl;
    
    // 1. join() 的核心价值
    explain_join_value();
    
    // 2. 场景1：等待计算结果
    demonstrate_waiting_for_result();
    
    // 3. 场景2：多线程协作
    demonstrate_parallel_computation();
    
    // 4. 场景3：资源清理
    demonstrate_resource_cleanup();
    
    // 5. 场景4：顺序依赖
    demonstrate_sequential_dependency();
    
    // 6. join() vs detach() vs async
    compare_synchronization_methods();
    
    // 7. 如何减少阻塞影响
    alternatives_to_blocking();
    
    // 8. 总结
    summary();
    
    return 0;
}
