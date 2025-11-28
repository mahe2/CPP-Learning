/*
 * 并发 vs 并行 深度解析
 * 问题：为什么时间片切换能提高效率？
 * 答案：并不总是！取决于任务类型
 */

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <atomic>
using namespace std;

// ============================================================================
// 一、概念澄清：并发 vs 并行
// ============================================================================

void explain_concepts() {
    cout << "\n=== 1. 并发 vs 并行（核心区别）===" << endl;
    
    cout << "\n并发（Concurrency）：" << endl;
    cout << "  定义：多个任务在时间段内交替执行" << endl;
    cout << "  关键：看起来同时，实际是快速切换" << endl;
    cout << "  场景：单核CPU" << endl;
    cout << "\n  时间线：" << endl;
    cout << "  时间: 0----1----2----3----4----5----6" << endl;
    cout << "  任务A: [██]......[██]......[██]......" << endl;
    cout << "  任务B: ....[██]......[██]......[██].." << endl;
    cout << "  说明：轮流执行，同一时刻只有一个任务" << endl;
    
    cout << "\n并行（Parallelism）：" << endl;
    cout << "  定义：多个任务真正同时执行" << endl;
    cout << "  关键：物理上同时进行" << endl;
    cout << "  场景：多核CPU" << endl;
    cout << "\n  时间线：" << endl;
    cout << "  时间:   0----1----2----3----4" << endl;
    cout << "  核心1: [████████████████████]" << endl;
    cout << "  核心2: [████████████████████]" << endl;
    cout << "  任务A: [████████████████████]" << endl;
    cout << "  任务B: [████████████████████]" << endl;
    cout << "  说明：真正同时执行" << endl;
    
    cout << "\n你的系统：" << endl;
    cout << "  硬件并发线程数: " << thread::hardware_concurrency() << endl;
    cout << "  支持真正的并行执行！" << endl;
}

// ============================================================================
// 二、纯计算任务：并发 vs 串行
// ============================================================================

// 计算密集型任务（纯CPU）
long long cpu_intensive_task(int id, long long iterations) {
    long long sum = 0;
    for (long long i = 0; i < iterations; i++) {
        sum += i * i;
    }
    return sum;
}

void test_pure_computation() {
    cout << "\n=== 2. 纯计算任务：并发不会更快！ ===" << endl;
    
    const long long iterations = 100000000;  // 1亿次计算
    
    cout << "\n场景：纯CPU计算（无等待）" << endl;
    cout << "任务：计算 1亿次平方和" << endl;
    
    // 串行执行
    cout << "\n方式1: 串行执行（单线程）" << endl;
    auto start = chrono::high_resolution_clock::now();
    
    long long result1 = cpu_intensive_task(1, iterations);
    long long result2 = cpu_intensive_task(2, iterations);
    
    auto end = chrono::high_resolution_clock::now();
    auto serial_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    
    cout << "  耗时: " << serial_time << " ms" << endl;
    
    // 并发执行（单核会更慢，因为有切换开销）
    cout << "\n方式2: 并发执行（多线程，可能在单核上）" << endl;
    start = chrono::high_resolution_clock::now();
    
    thread t1([&]() { cpu_intensive_task(1, iterations); });
    thread t2([&]() { cpu_intensive_task(2, iterations); });
    
    t1.join();
    t2.join();
    
    end = chrono::high_resolution_clock::now();
    auto concurrent_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    
    cout << "  耗时: " << concurrent_time << " ms" << endl;
    
    cout << "\n结论：" << endl;
    if (concurrent_time < serial_time * 0.6) {
        cout << "  ✅ 并发更快 - 你的系统是多核，实现了真正的并行！" << endl;
        cout << "  加速比: " << (double)serial_time / concurrent_time << "x" << endl;
    } else if (concurrent_time > serial_time) {
        cout << "  ❌ 并发更慢 - 系统在单核上切换，有切换开销！" << endl;
        cout << "  减速比: " << (double)concurrent_time / serial_time << "x" << endl;
    } else {
        cout << "  ≈ 差不多 - 并行抵消了切换开销" << endl;
    }
    
    cout << "\n关键点：" << endl;
    cout << "  - 纯计算任务不会因为\"切换\"而变快" << endl;
    cout << "  - 单核上：并发 ≈ 串行 + 切换开销（更慢）" << endl;
    cout << "  - 多核上：并发 = 真正并行（更快）" << endl;
}

// ============================================================================
// 三、有等待的任务：并发的真正优势
// ============================================================================

void io_intensive_task(int id, int sleep_ms) {
    cout << "  [任务" << id << "] 开始执行" << endl;
    
    // 模拟 I/O 等待（网络请求、文件读写等）
    this_thread::sleep_for(chrono::milliseconds(sleep_ms));
    
    cout << "  [任务" << id << "] 完成" << endl;
}

void test_io_bound_tasks() {
    cout << "\n=== 3. I/O 密集型任务：并发的真正优势！ ===" << endl;
    
    cout << "\n场景：有等待的任务（网络请求、文件读写）" << endl;
    cout << "任务：每个任务等待 1000ms" << endl;
    
    const int num_tasks = 5;
    const int wait_time = 1000;  // 每个任务等待1秒
    
    // 串行执行
    cout << "\n方式1: 串行执行" << endl;
    auto start = chrono::high_resolution_clock::now();
    
    for (int i = 1; i <= num_tasks; i++) {
        io_intensive_task(i, wait_time);
    }
    
    auto end = chrono::high_resolution_clock::now();
    auto serial_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    
    cout << "  总耗时: " << serial_time << " ms" << endl;
    cout << "  预期: " << num_tasks * wait_time << " ms (5个任务串行)" << endl;
    
    // 并发执行
    cout << "\n方式2: 并发执行" << endl;
    start = chrono::high_resolution_clock::now();
    
    vector<thread> threads;
    for (int i = 1; i <= num_tasks; i++) {
        threads.emplace_back(io_intensive_task, i, wait_time);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    end = chrono::high_resolution_clock::now();
    auto concurrent_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    
    cout << "  总耗时: " << concurrent_time << " ms" << endl;
    cout << "  预期: ~" << wait_time << " ms (5个任务并发)" << endl;
    
    cout << "\n结论：" << endl;
    cout << "  ✅ 并发快了 " << serial_time / concurrent_time << " 倍！" << endl;
    cout << "  原因：等待期间CPU可以切换到其他任务" << endl;
    
    cout << "\n原理图：" << endl;
    cout << "  串行：" << endl;
    cout << "    任务1: [等待1s]..................." << endl;
    cout << "    任务2: .........[等待1s].........." << endl;
    cout << "    任务3: .................[等待1s].." << endl;
    cout << "    总时间: 5秒" << endl;
    
    cout << "\n  并发：" << endl;
    cout << "    任务1: [等待1s]" << endl;
    cout << "    任务2: [等待1s]  ← 同时进行" << endl;
    cout << "    任务3: [等待1s]  ← 同时进行" << endl;
    cout << "    任务4: [等待1s]  ← 同时进行" << endl;
    cout << "    任务5: [等待1s]  ← 同时进行" << endl;
    cout << "    总时间: ~1秒" << endl;
}

// ============================================================================
// 四、CPU 切换的真相
// ============================================================================

void explain_context_switching() {
    cout << "\n=== 4. CPU 切换的真相 ===" << endl;
    
    cout << "\n什么是时间片切换（Context Switch）？" << endl;
    cout << "  1. 保存当前线程的状态（寄存器、程序计数器等）" << endl;
    cout << "  2. 加载下一个线程的状态" << endl;
    cout << "  3. 继续执行新线程" << endl;
    
    cout << "\n切换开销：" << endl;
    cout << "  - 典型切换时间：1-10 微秒" << endl;
    cout << "  - 看似很小，但频繁切换会累积" << endl;
    cout << "  - 包括：状态保存、缓存失效、TLB 刷新等" << endl;
    
    cout << "\n时间片大小：" << endl;
    cout << "  - Linux：通常 10-100 毫秒" << endl;
    cout << "  - Windows：可变，通常 15-30 毫秒" << endl;
    cout << "  - 太小：切换频繁，开销大" << endl;
    cout << "  - 太大：响应性差" << endl;
    
    cout << "\n示例：单核CPU执行两个任务" << endl;
    cout << "  任务A需要100ms，任务B需要100ms" << endl;
    cout << "  时间片：50ms，切换开销：1ms" << endl;
    cout << "\n  串行：" << endl;
    cout << "    A: [50ms] [50ms]" << endl;
    cout << "    B:             [50ms] [50ms]" << endl;
    cout << "    总时间: 200ms" << endl;
    
    cout << "\n  并发（时间片切换）：" << endl;
    cout << "    A: [50ms]......[50ms]" << endl;
    cout << "    切换: .....[1ms].....[1ms]" << endl;
    cout << "    B: ......[50ms]......[50ms]" << endl;
    cout << "    总时间: 202ms（反而更慢！）" << endl;
    
    cout << "\n结论：纯计算任务，单核上的并发不会更快！" << endl;
}

// ============================================================================
// 五、为什么并发"看起来"快？
// ============================================================================

void why_concurrency_feels_fast() {
    cout << "\n=== 5. 为什么并发\"看起来\"快？ ===" << endl;
    
    cout << "\n原因1: I/O 等待期间可以切换" << endl;
    cout << "  场景：下载文件、数据库查询、用户输入" << endl;
    cout << "  ┌─────────────────────────────────┐" << endl;
    cout << "  │ 任务      │ CPU时间 │ 等待时间  │" << endl;
    cout << "  ├─────────────────────────────────┤" << endl;
    cout << "  │ 下载文件  │ 1ms     │ 1000ms    │" << endl;
    cout << "  │ 读数据库  │ 2ms     │ 500ms     │" << endl;
    cout << "  │ 用户输入  │ 0.1ms   │ ∞         │" << endl;
    cout << "  └─────────────────────────────────┘" << endl;
    cout << "  等待期间CPU空闲 → 切换到其他任务 → 提高利用率！" << endl;
    
    cout << "\n原因2: 多核CPU实现真正并行" << endl;
    cout << "  现代CPU：4核、8核、16核..." << endl;
    cout << "  多个核心可以同时执行不同线程" << endl;
    cout << "  串行: 100ms + 100ms = 200ms" << endl;
    cout << "  并行: max(100ms, 100ms) = 100ms（2倍加速！）" << endl;
    
    cout << "\n原因3: 响应性提升（用户体验）" << endl;
    cout << "  单线程：" << endl;
    cout << "    [长任务===============] 用户界面卡住❌" << endl;
    cout << "\n  多线程：" << endl;
    cout << "    主线程: [UI更新][UI更新][UI更新]... 界面流畅✅" << endl;
    cout << "    后台线程: [长任务===============]" << endl;
    cout << "  总时间不变，但体验更好！" << endl;
    
    cout << "\n原因4: CPU速度远超I/O速度" << endl;
    cout << "  ┌─────────────────┬───────────────┐" << endl;
    cout << "  │ 操作            │ 延迟          │" << endl;
    cout << "  ├─────────────────┼───────────────┤" << endl;
    cout << "  │ CPU指令         │ ~1 纳秒       │" << endl;
    cout << "  │ 内存访问        │ ~100 纳秒     │" << endl;
    cout << "  │ SSD读取         │ ~100 微秒     │" << endl;
    cout << "  │ 网络请求        │ ~10 毫秒      │" << endl;
    cout << "  │ 硬盘读取        │ ~10 毫秒      │" << endl;
    cout << "  └─────────────────┴───────────────┘" << endl;
    cout << "  CPU等I/O = 跑车等牛车，太浪费！" << endl;
    cout << "  解决：等待时切换到其他任务" << endl;
}

// ============================================================================
// 六、实际案例对比
// ============================================================================

void real_world_comparison() {
    cout << "\n=== 6. 实际案例对比 ===" << endl;
    
    cout << "\n案例1: Web服务器" << endl;
    cout << "  单线程：" << endl;
    cout << "    请求1: [处理10ms] [等待数据库100ms]" << endl;
    cout << "    请求2:                              [处理10ms] [等待100ms]" << endl;
    cout << "    结果：每秒处理 ~4 个请求" << endl;
    
    cout << "\n  多线程：" << endl;
    cout << "    请求1: [处理10ms] [等待100ms]" << endl;
    cout << "    请求2: [处理10ms] [等待100ms]  ← 并发" << endl;
    cout << "    请求3: [处理10ms] [等待100ms]  ← 并发" << endl;
    cout << "    ...（可以处理10个并发请求）" << endl;
    cout << "    结果：每秒处理 ~90 个请求（20倍+）" << endl;
    
    cout << "\n案例2: 图像处理" << endl;
    cout << "  任务：处理100张图片，每张需要100ms CPU时间" << endl;
    cout << "  串行：10000ms（100张 × 100ms）" << endl;
    cout << "  并行（8核）：1250ms（100张 ÷ 8核 × 100ms）" << endl;
    cout << "  加速比：8倍" << endl;
    
    cout << "\n案例3: 科学计算" << endl;
    cout << "  任务：矩阵运算，可以拆分成独立子任务" << endl;
    cout << "  串行：1000秒" << endl;
    cout << "  并行（16核）：62.5秒（理想情况）" << endl;
    cout << "  实际：~70秒（考虑通信开销）" << endl;
    cout << "  加速比：~14倍" << endl;
}

// ============================================================================
// 七、核心总结
// ============================================================================

void summary() {
    cout << "\n========================================" << endl;
    cout << "           核心总结" << endl;
    cout << "========================================" << endl;
    
    cout << "\n问题：为什么切换任务时间片可以提高效率？" << endl;
    
    cout << "\n答案：并不总是！取决于任务类型和硬件" << endl;
    
    cout << "\n一、纯计算任务（CPU密集型）：" << endl;
    cout << "  单核：并发 ≈ 串行 + 切换开销（更慢）" << endl;
    cout << "        原因：同一时刻只能执行一个任务" << endl;
    cout << "  多核：并发 = 真正并行（更快）" << endl;
    cout << "        原因：多个核心同时工作" << endl;
    
    cout << "\n二、I/O密集型任务（有等待）：" << endl;
    cout << "  单核：并发 >> 串行（快很多）" << endl;
    cout << "        原因：等待期间切换到其他任务" << endl;
    cout << "  多核：并发 >>> 串行（更快）" << endl;
    cout << "        原因：并行 + 等待期间切换" << endl;
    
    cout << "\n三、CPU速度 vs I/O速度：" << endl;
    cout << "  ┌────────────────┬────────────┐" << endl;
    cout << "  │ 操作           │ 相对速度   │" << endl;
    cout << "  ├────────────────┼────────────┤" << endl;
    cout << "  │ CPU计算        │ 1x         │" << endl;
    cout << "  │ 内存访问       │ 100x慢     │" << endl;
    cout << "  │ SSD读取        │ 100,000x慢 │" << endl;
    cout << "  │ 网络/硬盘      │ 10,000,000x│" << endl;
    cout << "  └────────────────┴────────────┘" << endl;
    cout << "  CPU等I/O = 浪费！并发可以利用等待时间" << endl;
    
    cout << "\n四、并发的真正优势：" << endl;
    cout << "  ✅ I/O等待期间可以执行其他任务" << endl;
    cout << "  ✅ 多核CPU实现真正并行（计算加速）" << endl;
    cout << "  ✅ 提高响应性（用户体验）" << endl;
    cout << "  ✅ 提高资源利用率" << endl;
    cout << "  ❌ 单核纯计算任务不会更快" << endl;
    
    cout << "\n五、何时使用多线程：" << endl;
    cout << "  ✅ I/O密集型：网络请求、文件操作、数据库查询" << endl;
    cout << "  ✅ 计算密集型 + 多核：图像处理、科学计算、编码" << endl;
    cout << "  ✅ 提高响应性：GUI应用、游戏、实时系统" << endl;
    cout << "  ❌ 简单计算 + 单核：反而增加复杂度和开销" << endl;
    
    cout << "\n六、关键理解：" << endl;
    cout << "  1. 并发 ≠ 并行" << endl;
    cout << "     并发：交替执行（看起来同时）" << endl;
    cout << "     并行：真正同时执行（多核）" << endl;
    
    cout << "\n  2. 时间片切换有开销" << endl;
    cout << "     切换本身不会让任务更快完成" << endl;
    cout << "     只是让CPU在等待时不闲着" << endl;
    
    cout << "\n  3. 现代CPU是多核的" << endl;
    cout << "     你的系统: " << thread::hardware_concurrency() << " 个硬件线程" << endl;
    cout << "     多线程可以实现真正并行" << endl;
    
    cout << "\n  4. 任务特性决定效果" << endl;
    cout << "     CPU密集型：需要多核才能加速" << endl;
    cout << "     I/O密集型：单核也能显著提升" << endl;
    
    cout << "\n记忆口诀：" << endl;
    cout << "  并发看起来同时，并行真正同时" << endl;
    cout << "  纯算单核不会快，切换还有小开销" << endl;
    cout << "  等待期间能切换，利用空闲是关键" << endl;
    cout << "  多核并行真给力，计算速度成倍提" << endl;
    cout << "  I/O密集最受益，网络文件数据库" << endl;
    
    cout << "\n========================================" << endl;
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "\n╔═══════════════════════════════════════════════════╗" << endl;
    cout << "║         并发 vs 并行 深度解析                     ║" << endl;
    cout << "║   为什么切换任务时间片可以提高效率？              ║" << endl;
    cout << "╚═══════════════════════════════════════════════════╝" << endl;
    
    // 1. 概念解释
    explain_concepts();
    
    // 2. 纯计算任务测试
    test_pure_computation();
    
    // 3. I/O密集型任务测试
    test_io_bound_tasks();
    
    // 4. CPU切换真相
    explain_context_switching();
    
    // 5. 为什么并发看起来快
    why_concurrency_feels_fast();
    
    // 6. 实际案例
    real_world_comparison();
    
    // 7. 总结
    summary();
    
    return 0;
}
