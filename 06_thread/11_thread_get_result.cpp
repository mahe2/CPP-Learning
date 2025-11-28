/*
 * thread + join 获取执行结果的多种方式
 * 核心：thread 不直接返回结果，但有多种方法获取
 * 对比：thread vs async 的结果获取方式
 */

#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include <functional>
#include <chrono>
using namespace std;

// ============================================================================
// 一、thread + join 获取结果的 5 种方式
// ============================================================================

void explain_result_methods() {
    cout << "\n=== 1. thread + join 获取结果的方式 ===" << endl;
    
    cout << "\n核心理解：" << endl;
    cout << "  ❌ thread 构造函数不支持返回值" << endl;
    cout << "  ✅ 但可以通过其他方式获取结果！" << endl;
    
    cout << "\n5 种获取结果的方式：" << endl;
    cout << "  1. 引用参数（最常用）" << endl;
    cout << "  2. 指针参数" << endl;
    cout << "  3. Lambda 捕获引用" << endl;
    cout << "  4. 共享对象（成员变量）" << endl;
    cout << "  5. promise/future（最灵活）" << endl;
    
    cout << "\n对比 async：" << endl;
    cout << "  async: 直接返回 future，非常方便" << endl;
    cout << "  thread: 需要手动传递结果存储位置" << endl;
}

// ============================================================================
// 二、方式1：引用参数（最常用）
// ============================================================================

void demonstrate_reference_parameter() {
    cout << "\n=== 2. 方式1：引用参数 ===" << endl;
    
    cout << "\n示例：计算 1 到 N 的和" << endl;
    
    // 存储结果的变量
    long long result = 0;
    
    // 通过引用传递结果
    thread t([](long long& res, int n) {
        for (int i = 1; i <= n; i++) {
            res += i;
        }
    }, ref(result), 1000000);  // ✅ 必须用 std::ref
    
    cout << "主线程：等待计算完成..." << endl;
    t.join();
    
    cout << "结果: " << result << endl;
    
    cout << "\n关键点：" << endl;
    cout << "  1. 参数必须是引用类型: long long&" << endl;
    cout << "  2. 传递时必须用 std::ref(result)" << endl;
    cout << "  3. join 后才能安全访问结果" << endl;
}

// ============================================================================
// 三、方式2：指针参数
// ============================================================================

void demonstrate_pointer_parameter() {
    cout << "\n=== 3. 方式2：指针参数 ===" << endl;
    
    cout << "\n示例：查找最大值" << endl;
    
    vector<int> data = {3, 7, 2, 9, 1, 5};
    int max_value = 0;
    
    // 通过指针传递结果
    thread t([](vector<int>* arr, int* result) {
        *result = 0;
        for (int val : *arr) {
            if (val > *result) {
                *result = val;
            }
        }
    }, &data, &max_value);
    
    t.join();
    
    cout << "最大值: " << max_value << endl;
    
    cout << "\n优点：" << endl;
    cout << "  ✅ 不需要 std::ref，直接传地址" << endl;
    cout << "\n缺点：" << endl;
    cout << "  ⚠️ 需要手动管理指针，容易出错" << endl;
}

// ============================================================================
// 四、方式3：Lambda 捕获引用（推荐）
// ============================================================================

void demonstrate_lambda_capture() {
    cout << "\n=== 4. 方式3：Lambda 捕获引用（推荐）===" << endl;
    
    cout << "\n示例：并发计算多个结果" << endl;
    
    int sum = 0;
    int product = 1;
    string status = "";
    
    // Lambda 捕获引用，非常方便
    thread t([&sum, &product, &status]() {
        // 计算和
        for (int i = 1; i <= 10; i++) {
            sum += i;
        }
        
        // 计算积
        for (int i = 1; i <= 5; i++) {
            product *= i;
        }
        
        status = "完成";
    });
    
    t.join();
    
    cout << "和: " << sum << endl;
    cout << "积: " << product << endl;
    cout << "状态: " << status << endl;
    
    cout << "\n优点：" << endl;
    cout << "  ✅ 语法简洁，不需要 std::ref" << endl;
    cout << "  ✅ 可以捕获多个变量" << endl;
    cout << "  ✅ 推荐使用！" << endl;
    
    cout << "\n⚠️ 注意：必须用引用捕获 [&]，不能用值捕获 [=]" << endl;
}

// ============================================================================
// 五、方式4：共享对象（成员变量）
// ============================================================================

class Calculator {
    int result = 0;
    bool done = false;
    
public:
    void compute(int n) {
        thread t([this, n]() {  // ✅ 捕获 this 指针
            result = 0;
            for (int i = 1; i <= n; i++) {
                result += i;
            }
            done = true;
        });
        t.join();
    }
    
    int get_result() const { return result; }
    bool is_done() const { return done; }
};

void demonstrate_shared_object() {
    cout << "\n=== 5. 方式4：共享对象 ===" << endl;
    
    cout << "\n示例：封装在类中" << endl;
    
    Calculator calc;
    calc.compute(100);
    
    cout << "结果: " << calc.get_result() << endl;
    cout << "完成: " << (calc.is_done() ? "是" : "否") << endl;
    
    cout << "\n优点：" << endl;
    cout << "  ✅ 封装性好，易于管理" << endl;
    cout << "  ✅ 适合面向对象设计" << endl;
}

// ============================================================================
// 六、方式5：promise/future（最灵活，推荐）
// ============================================================================

void demonstrate_promise_future() {
    cout << "\n=== 6. 方式5：promise/future（最灵活）===" << endl;
    
    cout << "\n示例：使用 promise 传递结果" << endl;
    
    // 创建 promise
    promise<int> prom;
    // 获取 future
    future<int> fut = prom.get_future();
    
    // 在线程中设置结果
    thread t([](promise<int> p) {
        this_thread::sleep_for(chrono::milliseconds(100));
        
        // 计算结果
        int result = 0;
        for (int i = 1; i <= 100; i++) {
            result += i;
        }
        
        // 设置结果（传递给 future）
        p.set_value(result);  // ✅ 设置结果
        
    }, move(prom));  // ⚠️ promise 必须 move
    
    cout << "主线程：继续工作..." << endl;
    
    // 从 future 获取结果（会阻塞直到结果就绪）
    int result = fut.get();  // ✅ 获取结果
    
    cout << "结果: " << result << endl;
    
    t.join();
    
    cout << "\n优点：" << endl;
    cout << "  ✅ 最接近 async 的方式" << endl;
    cout << "  ✅ 可以传递异常" << endl;
    cout << "  ✅ 支持延迟获取（future.get()）" << endl;
    
    cout << "\n缺点：" << endl;
    cout << "  ⚠️ 代码稍微复杂一点" << endl;
}

// ============================================================================
// 七、方式6：回调函数（callback）
// ============================================================================

void demonstrate_callback() {
    cout << "\n=== 7. 方式6：回调函数（callback）===" << endl;
    
    cout << "\n示例：计算完成后调用回调" << endl;
    
    // 定义回调类型
    using Callback = function<void(int)>;
    
    // 执行任务并回调
    auto compute_with_callback = [](int n, Callback callback) {
        thread t([n, callback]() {
            // 计算结果
            int result = 0;
            for (int i = 1; i <= n; i++) {
                result += i;
            }
            
            // 调用回调
            callback(result);  // ✅ 将结果传给回调
        });
        t.detach();  // ⚠️ 注意：这里用 detach
    };
    
    // 使用回调
    cout << "主线程：启动异步计算" << endl;
    
    compute_with_callback(100, [](int result) {
        cout << "  [回调] 收到结果: " << result << endl;
    });
    
    cout << "主线程：继续工作..." << endl;
    
    // 等待回调执行
    this_thread::sleep_for(chrono::milliseconds(100));
    
    cout << "\n特点：" << endl;
    cout << "  ✅ 异步风格，不阻塞主线程" << endl;
    cout << "  ⚠️ 通常配合 detach 使用（需要注意生命周期）" << endl;
    cout << "  💡 JavaScript/Node.js 常用模式" << endl;
}

// ============================================================================
// 八、多线程返回多个结果
// ============================================================================

void demonstrate_multiple_results() {
    cout << "\n=== 8. 多线程返回多个结果 ===" << endl;
    
    cout << "\n场景：4 个线程分别计算，汇总结果" << endl;
    
    // 存储每个线程的结果
    vector<int> results(4, 0);
    vector<thread> threads;
    
    // 创建 4 个线程
    for (int i = 0; i < 4; i++) {
        threads.emplace_back([&results, i]() {
            int sum = 0;
            int start = i * 250 + 1;
            int end = (i + 1) * 250;
            
            for (int j = start; j <= end; j++) {
                sum += j;
            }
            
            results[i] = sum;  // ✅ 每个线程写入自己的位置
        });
    }
    
    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }
    
    // 汇总结果
    int total = 0;
    for (int i = 0; i < 4; i++) {
        cout << "线程 " << i << " 结果: " << results[i] << endl;
        total += results[i];
    }
    
    cout << "总和: " << total << endl;
    
    cout << "\n技巧：" << endl;
    cout << "  ✅ 每个线程写入独立位置，避免竞争" << endl;
    cout << "  ✅ 使用 vector 存储多个结果" << endl;
}

// ============================================================================
// 九、对比：thread vs async 获取结果
// ============================================================================

void compare_thread_vs_async() {
    cout << "\n=== 9. 对比：thread vs async ===" << endl;
    
    auto compute = [](int n) {
        int sum = 0;
        for (int i = 1; i <= n; i++) {
            sum += i;
        }
        return sum;
    };
    
    cout << "\n方式1：thread + join + 引用参数" << endl;
    {
        int result = 0;
        
        thread t([&result, compute]() {
            result = compute(100);
        });
        
        t.join();
        cout << "  结果: " << result << endl;
    }
    
    cout << "\n方式2：thread + promise/future" << endl;
    {
        promise<int> prom;
        future<int> fut = prom.get_future();
        
        thread t([compute](promise<int> p) {
            p.set_value(compute(100));
        }, move(prom));
        
        int result = fut.get();
        cout << "  结果: " << result << endl;
        
        t.join();
    }
    
    cout << "\n方式3：async（最简洁）" << endl;
    {
        auto fut = async(launch::async, compute, 100);
        int result = fut.get();
        cout << "  结果: " << result << endl;
    }
    
    cout << "\n对比表格：" << endl;
    cout << "  ┌─────────────────────┬────────────┬──────────────┬──────────┐" << endl;
    cout << "  │ 方式                │ 代码行数   │ 易用性       │ 推荐度   │" << endl;
    cout << "  ├─────────────────────┼────────────┼──────────────┼──────────┤" << endl;
    cout << "  │ thread + 引用       │ 4-5 行     │ ⭐⭐         │ ⭐⭐⭐   │" << endl;
    cout << "  │ thread + promise    │ 6-7 行     │ ⭐⭐⭐       │ ⭐⭐⭐⭐ │" << endl;
    cout << "  │ async               │ 2 行       │ ⭐⭐⭐⭐⭐   │ ⭐⭐⭐⭐⭐│" << endl;
    cout << "  └─────────────────────┴────────────┴──────────────┴──────────┘" << endl;
}

// ============================================================================
// 十、实际场景示例：并发下载
// ============================================================================

// 模拟下载函数
struct DownloadResult {
    string url;
    int size;
    bool success;
};

DownloadResult download_file(const string& url) {
    this_thread::sleep_for(chrono::milliseconds(100));  // 模拟下载
    return {url, 1024 * (rand() % 100), true};
}

void demonstrate_practical_example() {
    cout << "\n=== 10. 实际场景：并发下载文件 ===" << endl;
    
    vector<string> urls = {
        "https://example.com/file1.zip",
        "https://example.com/file2.zip",
        "https://example.com/file3.zip"
    };
    
    cout << "\n方式1：thread + Lambda 捕获（推荐）" << endl;
    {
        vector<DownloadResult> results(urls.size());
        vector<thread> threads;
        
        for (size_t i = 0; i < urls.size(); i++) {
            threads.emplace_back([&results, &urls, i]() {
                results[i] = download_file(urls[i]);
            });
        }
        
        // 等待所有下载完成
        for (auto& t : threads) {
            t.join();
        }
        
        // 显示结果
        for (const auto& res : results) {
            cout << "  文件: " << res.url << endl;
            cout << "    大小: " << res.size << " bytes" << endl;
            cout << "    状态: " << (res.success ? "成功" : "失败") << endl;
        }
    }
    
    cout << "\n方式2：async（更简洁）" << endl;
    {
        vector<future<DownloadResult>> futures;
        
        // 启动所有下载
        for (const auto& url : urls) {
            futures.push_back(async(launch::async, download_file, url));
        }
        
        // 获取所有结果
        for (auto& fut : futures) {
            auto res = fut.get();
            cout << "  文件: " << res.url << " (" << res.size << " bytes)" << endl;
        }
    }
}

// ============================================================================
// 十一、核心总结
// ============================================================================

void summary() {
    cout << "\n========================================" << endl;
    cout << "           核心总结" << endl;
    cout << "========================================" << endl;
    
    cout << "\n问题：thread + join 能否获取结果？" << endl;
    cout << "  答案：✅ 可以！有 6 种方式！" << endl;
    
    cout << "\n6 种获取结果的方式：" << endl;
    cout << "  1️⃣ 引用参数：thread(func, ref(result))" << endl;
    cout << "     - 需要 std::ref" << endl;
    cout << "     - 比较常用" << endl;
    
    cout << "\n  2️⃣ 指针参数：thread(func, &result)" << endl;
    cout << "     - 不需要 std::ref" << endl;
    cout << "     - 但需要管理指针" << endl;
    
    cout << "\n  3️⃣ Lambda 捕获：[&result]() { result = ... }" << endl;
    cout << "     - 最简洁！推荐！✅" << endl;
    cout << "     - 可以捕获多个变量" << endl;
    
    cout << "\n  4️⃣ 共享对象：[this]() { this->result = ... }" << endl;
    cout << "     - 适合面向对象设计" << endl;
    cout << "     - 封装性好" << endl;
    
    cout << "\n  5️⃣ promise/future：最灵活！✅" << endl;
    cout << "     - 最接近 async" << endl;
    cout << "     - 可以传递异常" << endl;
    cout << "     - 支持延迟获取" << endl;
    
    cout << "\n  6️⃣ 回调函数：callback(result)" << endl;
    cout << "     - 异步风格" << endl;
    cout << "     - 配合 detach 使用" << endl;
    
    cout << "\n推荐方式排序：" << endl;
    cout << "  🥇 Lambda 捕获（最简洁）" << endl;
    cout << "  🥈 promise/future（最灵活）" << endl;
    cout << "  🥉 引用参数（传统方式）" << endl;
    
    cout << "\nthread vs async：" << endl;
    cout << "  thread 获取结果：" << endl;
    cout << "    ⚠️ 需要手动传递结果存储位置" << endl;
    cout << "    ⚠️ 代码稍微复杂" << endl;
    cout << "    ✅ 但可以做到！" << endl;
    
    cout << "\n  async 获取结果：" << endl;
    cout << "    ✅ 直接返回 future" << endl;
    cout << "    ✅ 代码最简洁" << endl;
    cout << "    ✅ 首选方案！" << endl;
    
    cout << "\n什么时候用 thread + join？" << endl;
    cout << "  1. 需要精确控制线程（优先级、亲和性等）" << endl;
    cout << "  2. 需要管理线程生命周期" << endl;
    cout << "  3. 实现自定义线程池" << endl;
    cout << "  4. 与旧代码兼容" << endl;
    
    cout << "\n什么时候用 async？" << endl;
    cout << "  1. 只关心结果，不关心线程细节（99% 场景）" << endl;
    cout << "  2. 需要返回值" << endl;
    cout << "  3. 想要最简洁的代码" << endl;
    cout << "  4. 不确定用哪个 → 用 async！" << endl;
    
    cout << "\n记忆口诀：" << endl;
    cout << "  thread 无返回，但法子很多" << endl;
    cout << "  Lambda 捕获简洁，promise 最灵活" << endl;
    cout << "  async 返回 future，代码最简短" << endl;
    cout << "  不知道选哪个，首选用 async" << endl;
    
    cout << "\n代码示例对比：" << endl;
    cout << "\n  // ⚠️ thread + 引用（4 行）" << endl;
    cout << "  int result = 0;" << endl;
    cout << "  thread t([&result]() { result = compute(); });" << endl;
    cout << "  t.join();" << endl;
    cout << "  cout << result;" << endl;
    
    cout << "\n  // ✅ async（2 行）" << endl;
    cout << "  auto fut = async(launch::async, compute);" << endl;
    cout << "  cout << fut.get();" << endl;
    
    cout << "\n========================================" << endl;
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "\n╔═══════════════════════════════════════════════════╗" << endl;
    cout << "║     thread + join 获取执行结果的方法          ║" << endl;
    cout << "║   6 种方式 + async 对比                           ║" << endl;
    cout << "╚═══════════════════════════════════════════════════╝" << endl;
    
    // 1. 说明获取结果的方式
    explain_result_methods();
    
    // 2. 方式1：引用参数
    demonstrate_reference_parameter();
    
    // 3. 方式2：指针参数
    demonstrate_pointer_parameter();
    
    // 4. 方式3：Lambda 捕获
    demonstrate_lambda_capture();
    
    // 5. 方式4：共享对象
    demonstrate_shared_object();
    
    // 6. 方式5：promise/future
    demonstrate_promise_future();
    
    // 7. 方式6：回调函数
    demonstrate_callback();
    
    // 8. 多线程多结果
    demonstrate_multiple_results();
    
    // 9. thread vs async 对比
    compare_thread_vs_async();
    
    // 10. 实际场景示例
    demonstrate_practical_example();
    
    // 11. 总结
    summary();
    
    return 0;
}
