/*
 * 内存地址高低详解
 * 理解为什么栈在高地址，堆在低地址
 */

#include <iostream>
#include <iomanip>

using namespace std;

// ========== 第一部分：什么是内存地址？==========

void whatIsMemoryAddress() {
    cout << "=== 什么是内存地址？ ===" << endl;
    
    /*
     * 内存地址 = 内存中的位置编号
     * 
     * 类比：内存就像一栋大楼
     * 
     *    高地址 (0xFFFF...)
     *    ┌─────────────────┐
     *    │  栈区           │ ← 顶楼（高地址）
     *    │  (Stack)        │
     *    ├─────────────────┤
     *    │                 │
     *    │  空闲空间       │
     *    │                 │
     *    ├─────────────────┤
     *    │  堆区           │
     *    │  (Heap)         │
     *    ├─────────────────┤
     *    │  全局/静态区    │
     *    ├─────────────────┤
     *    │  常量区         │
     *    ├─────────────────┤
     *    │  代码区         │ ← 1 楼（低地址）
     *    └─────────────────┘
     *    低地址 (0x0000...)
     */
    
    cout << "\n内存地址就像房间号：" << endl;
    cout << "  低地址 = 1 楼 101 室（如 0x00001000）" << endl;
    cout << "  高地址 = 10 楼 1001 室（如 0x00010000）" << endl;
    
    cout << "\n为什么要有高低之分？" << endl;
    cout << "  - 操作系统按地址组织内存" << endl;
    cout << "  - 不同区域分配不同地址范围" << endl;
    cout << "  - 便于管理和保护" << endl;
}

// ========== 第二部分：实际地址对比 ==========

int globalVar = 100;  // 全局区

void compareAddresses() {
    cout << "\n\n=== 实际内存地址对比 ===" << endl;
    
    // 栈变量
    int stackVar1 = 1;
    int stackVar2 = 2;
    int stackVar3 = 3;
    
    // 堆变量
    int* heapVar1 = new int(10);
    int* heapVar2 = new int(20);
    int* heapVar3 = new int(30);
    
    // 常量
    const char* str = "Hello";
    
    cout << "\n使用十六进制显示地址（更直观）：" << endl;
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    
    // 设置十六进制输出
    cout << hex << uppercase << showbase;
    
    cout << "\n【代码区】（最低）" << endl;
    cout << "  函数地址:         " << (void*)&compareAddresses << endl;
    
    cout << "\n【常量区】" << endl;
    cout << "  字符串常量:       " << (void*)str << endl;
    
    cout << "\n【全局/静态区】" << endl;
    cout << "  全局变量:         " << &globalVar << endl;
    
    cout << "\n【堆区】（向上增长 ↑）" << endl;
    cout << "  heapVar1:         " << heapVar1 << endl;
    cout << "  heapVar2:         " << heapVar2 << endl;
    cout << "  heapVar3:         " << heapVar3 << endl;
    cout << "  观察：后分配的地址更大（向上增长）" << endl;
    
    cout << "\n【栈区】（最高，向下增长 ↓）" << endl;
    cout << "  stackVar1:        " << &stackVar1 << endl;
    cout << "  stackVar2:        " << &stackVar2 << endl;
    cout << "  stackVar3:        " << &stackVar3 << endl;
    cout << "  观察：后定义的地址更小（向下增长）" << endl;
    
    cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    
    // 恢复十进制输出
    cout << dec << nouppercase << noshowbase;
    
    // 计算地址差
    long stackDiff = (long)&stackVar1 - (long)&stackVar2;
    long heapDiff = (long)heapVar2 - (long)heapVar1;
    
    cout << "\n地址差异：" << endl;
    cout << "  栈变量地址差: " << stackDiff << " 字节";
    if (stackDiff < 0) {
        cout << " (负数，向下增长)";
    }
    cout << endl;
    
    cout << "  堆变量地址差: " << heapDiff << " 字节";
    if (heapDiff > 0) {
        cout << " (正数，向上增长)";
    }
    cout << endl;
    
    delete heapVar1;
    delete heapVar2;
    delete heapVar3;
}

// ========== 第三部分：为什么栈向下、堆向上？==========

void whyStackDownHeapUp() {
    cout << "\n\n=== 为什么栈向下增长、堆向上增长？ ===" << endl;
    
    cout << "\n【历史原因】" << endl;
    cout << "  早期计算机内存很小，需要充分利用：" << endl;
    cout << "  " << endl;
    cout << "    高地址" << endl;
    cout << "    ┌─────────────┐" << endl;
    cout << "    │   栈   ↓    │ ← 从上往下长" << endl;
    cout << "    ├─────────────┤" << endl;
    cout << "    │             │" << endl;
    cout << "    │  空闲空间   │ ← 栈和堆可以共用这部分" << endl;
    cout << "    │             │" << endl;
    cout << "    ├─────────────┤" << endl;
    cout << "    │   堆   ↑    │ ← 从下往上长" << endl;
    cout << "    └─────────────┘" << endl;
    cout << "    低地址" << endl;
    
    cout << "\n【现代系统】" << endl;
    cout << "  - 内存很大（GB 级别），不太需要节省" << endl;
    cout << "  - 但保留了这个设计（兼容性）" << endl;
    cout << "  - 栈和堆各自独立的地址空间" << endl;
    
    cout << "\n【设计优势】" << endl;
    cout << "  1. 最大化可用空间" << endl;
    cout << "     栈和堆从两端向中间增长，充分利用" << endl;
    cout << "  " << endl;
    cout << "  2. 防止冲突" << endl;
    cout << "     如果栈和堆碰撞 → 栈溢出（Stack Overflow）" << endl;
    cout << "  " << endl;
    cout << "  3. 简化管理" << endl;
    cout << "     栈：只需移动栈指针（SP）" << endl;
    cout << "     堆：从低地址分配，便于追踪" << endl;
}

// ========== 第四部分：栈增长演示 ==========

void demonstrateStackGrowth() {
    cout << "\n\n=== 栈增长演示 ===" << endl;
    
    int a = 1;
    int b = 2;
    int c = 3;
    
    cout << "\n定义顺序：a → b → c" << endl;
    cout << "地址：" << endl;
    cout << "  a: " << &a << endl;
    cout << "  b: " << &b << endl;
    cout << "  c: " << &c << endl;
    
    if ((long)&a > (long)&b && (long)&b > (long)&c) {
        cout << "\n结论：先定义的在高地址，后定义的在低地址" << endl;
        cout << "      说明栈向下增长（从高地址到低地址）" << endl;
    } else {
        cout << "\n注意：编译器优化可能改变顺序" << endl;
    }
    
    cout << "\n栈的生长过程：" << endl;
    cout << "  1. 进入函数 → 栈指针向下移动（地址减小）" << endl;
    cout << "  2. 分配变量 → 在低地址处分配" << endl;
    cout << "  3. 退出函数 → 栈指针向上移动（地址增大）" << endl;
}

// ========== 第五部分：函数调用与栈帧 ==========

void innerFunction() {
    int inner = 999;
    cout << "  内层函数变量地址: " << &inner << endl;
}

void middleFunction() {
    int middle = 888;
    cout << "  中层函数变量地址: " << &middle << endl;
    innerFunction();
}

void outerFunction() {
    int outer = 777;
    cout << "  外层函数变量地址: " << &outer << endl;
    middleFunction();
}

void demonstrateStackFrames() {
    cout << "\n\n=== 函数调用与栈帧 ===" << endl;
    
    cout << "\n调用链：outer → middle → inner" << endl;
    cout << "观察地址变化：" << endl;
    
    outerFunction();
    
    cout << "\n解释：" << endl;
    cout << "  - 每次函数调用，栈向下增长（地址减小）" << endl;
    cout << "  - 内层函数的变量地址更小" << endl;
    cout << "  - 函数返回时，栈帧被释放" << endl;
}

// ========== 第六部分：堆增长演示 ==========

void demonstrateHeapGrowth() {
    cout << "\n\n=== 堆增长演示 ===" << endl;
    
    int* p1 = new int(1);
    int* p2 = new int(2);
    int* p3 = new int(3);
    
    cout << "\n分配顺序：p1 → p2 → p3" << endl;
    cout << "地址：" << endl;
    cout << "  p1: " << p1 << endl;
    cout << "  p2: " << p2 << endl;
    cout << "  p3: " << p3 << endl;
    
    if ((long)p2 > (long)p1 && (long)p3 > (long)p2) {
        cout << "\n结论：后分配的在高地址" << endl;
        cout << "      说明堆向上增长（从低地址到高地址）" << endl;
    } else {
        cout << "\n注意：内存分配器可能不是连续分配" << endl;
    }
    
    cout << "\n堆的增长过程：" << endl;
    cout << "  1. new 请求 → 从堆的空闲区域分配" << endl;
    cout << "  2. 向高地址方向寻找可用空间" << endl;
    cout << "  3. delete 释放 → 空间还给堆管理器" << endl;
    
    delete p1;
    delete p2;
    delete p3;
}

// ========== 第七部分：地址高低的实际意义 ==========

void practicalMeaning() {
    cout << "\n\n=== 地址高低的实际意义 ===" << endl;
    
    cout << "\n【1. 栈溢出检测】" << endl;
    cout << "  如果栈不断向下增长（递归太深）" << endl;
    cout << "  最终会碰到堆区 → 栈溢出错误" << endl;
    cout << "  " << endl;
    cout << "  示例：" << endl;
    cout << "    void recursion() {" << endl;
    cout << "        recursion();  // 无限递归" << endl;
    cout << "    }" << endl;
    cout << "    → 栈不断向下长，直到越界" << endl;
    
    cout << "\n【2. 指针比较】" << endl;
    cout << "  可以比较两个变量的相对位置：" << endl;
    
    int var1 = 10;
    int var2 = 20;
    
    if (&var1 > &var2) {
        cout << "  var1 在高地址，先定义" << endl;
    } else {
        cout << "  var2 在高地址，先定义" << endl;
    }
    
    cout << "\n【3. 内存布局调试】" << endl;
    cout << "  通过地址判断变量在哪个区：" << endl;
    cout << "  - 地址很低 (0x000...) → 代码区/常量区" << endl;
    cout << "  - 地址中等 (0x00F...) → 堆区" << endl;
    cout << "  - 地址很高 (0x7FF...) → 栈区" << endl;
    
    cout << "\n【4. 安全问题】" << endl;
    cout << "  - 缓冲区溢出：向低地址写数据" << endl;
    cout << "  - 可能覆盖返回地址，造成安全漏洞" << endl;
    cout << "  - 现代系统有保护机制（ASLR、Stack Canary）" << endl;
}

// ========== 第八部分：总结 ==========

void summarize() {
    cout << "\n\n=== 总结：地址高低的关键点 ===" << endl;
    
    cout << "\n【地址分布】" << endl;
    cout << "  从低到高：代码区 < 常量区 < 全局区 < 堆区 < 栈区" << endl;
    
    cout << "\n【增长方向】" << endl;
    cout << "  栈区：向下增长（从高地址到低地址）↓" << endl;
    cout << "  堆区：向上增长（从低地址到高地址）↑" << endl;
    
    cout << "\n【为什么这样设计？】" << endl;
    cout << "  1. 历史原因：节省内存空间" << endl;
    cout << "  2. 防止冲突：两端向中间长" << endl;
    cout << "  3. 简化管理：栈用指针，堆用分配器" << endl;
    
    cout << "\n【实际意义】" << endl;
    cout << "  ✅ 理解栈溢出原因" << endl;
    cout << "  ✅ 调试内存问题" << endl;
    cout << "  ✅ 理解程序内存布局" << endl;
    cout << "  ⚠️  但日常编程很少直接用到" << endl;
    
    cout << "\n【记忆口诀】" << endl;
    cout << "  代码常量全局低，" << endl;
    cout << "  堆在中间向上长，" << endl;
    cout << "  栈在顶部向下长，" << endl;
    cout << "  两头相会溢出伤。" << endl;
}

int main() {
    cout << "=== 内存地址高低详解 ===" << endl;
    
    // 1. 什么是内存地址
    whatIsMemoryAddress();
    
    // 2. 实际地址对比
    compareAddresses();
    
    // 3. 为什么栈向下、堆向上
    whyStackDownHeapUp();
    
    // 4. 栈增长演示
    demonstrateStackGrowth();
    
    // 5. 函数调用与栈帧
    demonstrateStackFrames();
    
    // 6. 堆增长演示
    demonstrateHeapGrowth();
    
    // 7. 实际意义
    practicalMeaning();
    
    // 8. 总结
    summarize();
    
    return 0;
}

/*
 * ========== 核心要点总结 ==========
 * 
 * 1. 内存地址 = 内存位置的编号
 *    - 就像门牌号：1 楼 101 室 vs 10 楼 1001 室
 *    - 数值越大 = 地址越高 = "楼层"越高
 * 
 * 2. 内存布局（从低到高）
 *    
 *    高地址 (0x7FFF...)
 *    ┌──────────────┐
 *    │   栈区  ↓    │ ← 最高，向下增长
 *    ├──────────────┤
 *    │   空闲       │
 *    ├──────────────┤
 *    │   堆区  ↑    │ ← 向上增长
 *    ├──────────────┤
 *    │   全局/静态  │
 *    ├──────────────┤
 *    │   常量区     │
 *    ├──────────────┤
 *    │   代码区     │ ← 最低
 *    └──────────────┘
 *    低地址 (0x0000...)
 * 
 * 3. 为什么栈向下、堆向上？
 *    - 历史原因：早期内存小，两端向中间长，节省空间
 *    - 现代原因：保持兼容性，便于管理
 *    - 好处：最大化利用空间，防止冲突
 * 
 * 4. 栈的增长
 *    - 进入函数 → 栈向下长（地址减小）
 *    - 离开函数 → 栈收缩（地址增大）
 *    - 后定义的变量在低地址
 * 
 * 5. 堆的增长
 *    - new 分配 → 堆向上长（地址增大）
 *    - delete 释放 → 还给堆管理器
 *    - 后分配的通常在高地址（但不绝对）
 * 
 * 6. 实际意义
 *    ✅ 理解栈溢出（Stack Overflow）
 *    ✅ 调试内存问题
 *    ✅ 理解缓冲区溢出攻击
 *    ⚠️  日常编程不太需要关心具体地址
 * 
 * 7. 与 Java/Kotlin 对比
 *    Java/Kotlin:
 *      - 程序员不需要关心地址
 *      - JVM 自动管理内存布局
 *      - 简单但失去底层控制
 *    
 *    C++:
 *      - 可以看到实际内存地址
 *      - 理解底层内存布局
 *      - 复杂但完全控制
 * 
 * 8. 记忆技巧
 *    把内存想象成大楼：
 *      - 1 楼 = 代码区（低地址）
 *      - 中间楼层 = 全局区、堆区
 *      - 顶楼 = 栈区（高地址）
 *    
 *    栈从顶楼往下长（客人从上往下住）
 *    堆从低楼往上长（货物从下往上堆）
 * 
 * 9. 重要提醒
 *    ⚠️  地址的具体数值不重要
 *    ✅ 重要的是理解：
 *       - 不同区域的相对位置
 *       - 栈向下、堆向上的原理
 *       - 为什么会栈溢出
 * 
 * 10. 实用建议
 *     初学阶段：
 *       - 知道有高低之分即可
 *       - 理解栈向下、堆向上
 *       - 不需要记具体地址
 *     
 *     实际开发：
 *       - 很少直接用到地址高低
 *       - 主要用于调试和理解错误
 *       - 现代工具会帮你检测问题
 */
