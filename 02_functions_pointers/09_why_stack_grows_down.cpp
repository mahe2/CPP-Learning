/*
 * 为什么栈从高地址向低地址分配？
 * 深入理解栈的工作原理和栈溢出
 */

#include <iostream>
#include <iomanip>

using namespace std;

// ========== 第一部分：栈的分配机制 ==========

void stackAllocationMechanism() {
    cout << "=== 栈的分配机制 ===" << endl;
    
    /*
     * 关键理解：操作系统预先分配了一块栈空间
     * 
     * 操作系统给每个程序的栈分配：
     *   - 起始地址（高地址）：0x7fff0000（例如）
     *   - 结束地址（低地址）：0x7fe00000（例如）
     *   - 栈大小：通常 1-8 MB
     * 
     * 可视化：
     * 
     *   高地址 0x7fff0000  ← 栈底（Stack Bottom）
     *   ┌────────────────┐
     *   │                │
     *   │   未使用空间   │  ← 程序刚启动时，整个栈是空的
     *   │                │
     *   │                │
     *   ├────────────────┤
     *   │  栈指针 SP →   │  ← 当前栈顶位置（Stack Pointer）
     *   │                │
     *   │   已使用空间   │  ← 已分配的变量在这里
     *   │                │
     *   └────────────────┘
     *   低地址 0x7fe00000  ← 栈的底线（Stack Limit）
     */
    
    cout << "\n【操作系统分配栈空间】" << endl;
    cout << "  1. 程序启动时，OS 分配一块固定大小的栈空间" << endl;
    cout << "     例如：1 MB 或 8 MB" << endl;
    cout << "  " << endl;
    cout << "  2. 栈有两个边界：" << endl;
    cout << "     - 栈底（高地址）：0x7fff0000（假设）" << endl;
    cout << "     - 栈限（低地址）：0x7fe00000（假设）" << endl;
    cout << "  " << endl;
    cout << "  3. 栈指针（SP）从栈底开始，向下移动" << endl;
    cout << "     - 分配变量 → SP 向下移（地址减小）" << endl;
    cout << "     - 释放变量 → SP 向上移（地址增大）" << endl;
}

// ========== 第二部分：为什么从高到低？==========

void whyHighToLow() {
    cout << "\n\n=== 为什么从高地址到低地址？ ===" << endl;
    
    cout << "\n【原因 1：历史设计】" << endl;
    cout << "  早期计算机内存布局：" << endl;
    cout << "  " << endl;
    cout << "  高地址 0xFFFF" << endl;
    cout << "  ┌────────────┐" << endl;
    cout << "  │  栈  ↓     │ ← 从顶部向下长" << endl;
    cout << "  ├────────────┤" << endl;
    cout << "  │            │" << endl;
    cout << "  │  空闲空间  │ ← 栈和堆共享这块空间" << endl;
    cout << "  │            │" << endl;
    cout << "  ├────────────┤" << endl;
    cout << "  │  堆  ↑     │ ← 从底部向上长" << endl;
    cout << "  ├────────────┤" << endl;
    cout << "  │  全局区    │" << endl;
    cout << "  ├────────────┤" << endl;
    cout << "  │  代码区    │" << endl;
    cout << "  └────────────┘" << endl;
    cout << "  低地址 0x0000" << endl;
    
    cout << "\n  优势：" << endl;
    cout << "    - 栈和堆从两端向中间增长" << endl;
    cout << "    - 最大化利用空间" << endl;
    cout << "    - 只有当两者相遇才会溢出" << endl;
    
    cout << "\n【原因 2：硬件支持】" << endl;
    cout << "  - CPU 有专门的栈指针寄存器（SP/ESP/RSP）" << endl;
    cout << "  - PUSH 指令：SP 减小（向下）" << endl;
    cout << "  - POP 指令：SP 增大（向上）" << endl;
    cout << "  - 硬件层面就是这样设计的" << endl;
    
    cout << "\n【原因 3：自然适配】" << endl;
    cout << "  - 代码区在低地址（固定不变）" << endl;
    cout << "  - 堆从低地址向上长（动态分配）" << endl;
    cout << "  - 栈从高地址向下长（函数调用）" << endl;
    cout << "  - 各司其职，互不干扰" << endl;
}

// ========== 第三部分：栈溢出详解 ==========

void stackOverflowExplained() {
    cout << "\n\n=== 栈溢出详解 ===" << endl;
    
    cout << "\n你的理解完全正确！" << endl;
    cout << "「分到最小地址，岂不是栈溢出了」" << endl;
    cout << "→ 是的！这就是栈溢出（Stack Overflow）的原因！" << endl;
    
    cout << "\n【栈溢出的过程】" << endl;
    cout << "  " << endl;
    cout << "  栈底（高地址）0x7fff0000" << endl;
    cout << "  ┌──────────────────┐" << endl;
    cout << "  │  SP 初始位置     │" << endl;
    cout << "  ├──────────────────┤" << endl;
    cout << "  │  函数 A 的变量   │ ← SP 向下移" << endl;
    cout << "  ├──────────────────┤" << endl;
    cout << "  │  函数 B 的变量   │ ← SP 继续向下" << endl;
    cout << "  ├──────────────────┤" << endl;
    cout << "  │  函数 C 的变量   │ ← SP 继续向下" << endl;
    cout << "  ├──────────────────┤" << endl;
    cout << "  │  ...递归调用...  │ ← SP 不断向下" << endl;
    cout << "  ├──────────────────┤" << endl;
    cout << "  │  ❌ 超出栈限！   │ ← 栈溢出！" << endl;
    cout << "  └──────────────────┘" << endl;
    cout << "  栈限（低地址）0x7fe00000" << endl;
    
    cout << "\n【栈溢出的原因】" << endl;
    cout << "  1. 递归太深（无限递归）" << endl;
    cout << "     void f() { f(); }  ← 无限调用" << endl;
    cout << "  " << endl;
    cout << "  2. 局部数组太大" << endl;
    cout << "     int arr[1000000];  ← 4 MB，可能超过栈大小" << endl;
    cout << "  " << endl;
    cout << "  3. 函数嵌套太深" << endl;
    cout << "     A() → B() → C() → ... → 超出栈空间" << endl;
}

// ========== 第四部分：实际演示 ==========

void demonstrateStackPointer() {
    cout << "\n\n=== 栈指针移动演示 ===" << endl;
    
    int a = 1;
    int b = 2;
    int c = 3;
    
    cout << "\n定义顺序：a → b → c" << endl;
    cout << "地址变化：" << endl;
    cout << "  a: " << &a << "  ← 栈指针从这里开始" << endl;
    cout << "  b: " << &b << "  ← 栈指针向下移动" << endl;
    cout << "  c: " << &c << "  ← 栈指针继续向下" << endl;
    
    long diff_ab = (long)&a - (long)&b;
    long diff_bc = (long)&b - (long)&c;
    
    cout << "\n地址差：" << endl;
    cout << "  a - b = " << diff_ab << " 字节";
    if (diff_ab > 0) {
        cout << "  ✅ a 在高地址，先分配" << endl;
    }
    
    cout << "  b - c = " << diff_bc << " 字节";
    if (diff_bc > 0) {
        cout << "  ✅ b 在高地址，先分配" << endl;
    }
    
    cout << "\n结论：" << endl;
    cout << "  栈指针从高地址开始，每次分配变量都向低地址移动" << endl;
}

// ========== 第五部分：栈大小和限制 ==========

void stackSizeAndLimits() {
    cout << "\n\n=== 栈大小和限制 ===" << endl;
    
    cout << "\n【不同系统的栈大小】" << endl;
    cout << "  Linux:   默认 8 MB" << endl;
    cout << "  macOS:   默认 8 MB" << endl;
    cout << "  Windows: 默认 1 MB" << endl;
    
    cout << "\n【如何查看栈大小？】" << endl;
    cout << "  Linux/macOS:" << endl;
    cout << "    ulimit -s        # 查看栈大小（KB）" << endl;
    cout << "    ulimit -s 16384  # 设置为 16 MB" << endl;
    
    cout << "\n【栈的地址范围（示例）】" << endl;
    cout << "  假设栈大小 8 MB = 8,388,608 字节" << endl;
    cout << "  " << endl;
    cout << "  栈底（高地址）：0x7fff0000 = 2,147,418,112" << endl;
    cout << "  栈限（低地址）：0x7f7f0000 = 2,139,029,504" << endl;
    cout << "  差值：         8,388,608 字节 = 8 MB" << endl;
    cout << "  " << endl;
    cout << "  如果栈指针超过 0x7f7f0000 → 栈溢出！" << endl;
}

// ========== 第六部分：为什么不从低到高？==========

void whyNotLowToHigh() {
    cout << "\n\n=== 为什么不从低地址到高地址分配？ ===" << endl;
    
    cout << "\n【如果栈从低到高会怎样？】" << endl;
    cout << "  " << endl;
    cout << "  低地址 0x0000" << endl;
    cout << "  ┌────────────┐" << endl;
    cout << "  │  代码区    │" << endl;
    cout << "  ├────────────┤" << endl;
    cout << "  │  全局区    │" << endl;
    cout << "  ├────────────┤" << endl;
    cout << "  │  堆  ↑     │ ← 向上长" << endl;
    cout << "  ├────────────┤" << endl;
    cout << "  │  栈  ↑     │ ← 也向上长？" << endl;
    cout << "  └────────────┘" << endl;
    cout << "  高地址 0xFFFF" << endl;
    
    cout << "\n【问题】" << endl;
    cout << "  ❌ 堆和栈都向上长 → 很快就会碰撞" << endl;
    cout << "  ❌ 需要固定分界线 → 不灵活" << endl;
    cout << "  ❌ 无法充分利用空间" << endl;
    
    cout << "\n【现在的设计更好】" << endl;
    cout << "  ✅ 堆向上、栈向下 → 从两端向中间长" << endl;
    cout << "  ✅ 没有固定分界 → 灵活分配" << endl;
    cout << "  ✅ 只有真正用完才溢出 → 最大化利用" << endl;
}

// ========== 第七部分：实际栈溢出示例 ==========

// 注意：这些函数会导致程序崩溃，仅作示例说明

void causeStackOverflowByRecursion() {
    cout << "\n\n=== 栈溢出示例（仅演示，不运行）===" << endl;
    
    cout << "\n【示例 1：无限递归】" << endl;
    cout << "  void infiniteRecursion() {" << endl;
    cout << "      infiniteRecursion();  // 无限调用自己" << endl;
    cout << "  }" << endl;
    cout << "  " << endl;
    cout << "  结果：" << endl;
    cout << "    - 每次调用占用栈空间" << endl;
    cout << "    - 栈指针不断向下移动" << endl;
    cout << "    - 最终超过栈限 → Segmentation Fault" << endl;
    
    cout << "\n【示例 2：大数组】" << endl;
    cout << "  void largeArray() {" << endl;
    cout << "      int arr[1000000];  // 4 MB" << endl;
    cout << "  }" << endl;
    cout << "  " << endl;
    cout << "  结果：" << endl;
    cout << "    - 如果栈只有 1 MB → 立即溢出" << endl;
    cout << "    - 如果栈是 8 MB → 可能成功" << endl;
    
    cout << "\n【示例 3：深度递归】" << endl;
    cout << "  int fibonacci(int n) {" << endl;
    cout << "      if (n <= 1) return n;" << endl;
    cout << "      return fibonacci(n-1) + fibonacci(n-2);" << endl;
    cout << "  }" << endl;
    cout << "  fibonacci(1000000);  // 递归太深" << endl;
    cout << "  " << endl;
    cout << "  结果：栈溢出" << endl;
}

// ========== 第八部分：总结 ==========

void summarize() {
    cout << "\n\n=== 核心总结 ===" << endl;
    
    cout << "\n【你的问题回答】" << endl;
    cout << "  Q1: 地址不应该是从小向大分吗？" << endl;
    cout << "  A1: 栈是特殊的，从大到小分配" << endl;
    cout << "      堆才是从小到大分配" << endl;
    
    cout << "\n  Q2: 是从程序的栈内存最大地址开始分？" << endl;
    cout << "  A2: 完全正确！" << endl;
    cout << "      OS 分配栈空间：0x7fff0000 ~ 0x7fe00000（例如）" << endl;
    cout << "      栈指针从 0x7fff0000（最高）开始，向下移动" << endl;
    
    cout << "\n  Q3: 分到最小地址，岂不是栈溢出了？" << endl;
    cout << "  A3: 对！这就是栈溢出的原理！" << endl;
    cout << "      当栈指针超过 0x7fe00000（最低）→ Stack Overflow" << endl;
    
    cout << "\n【为什么这样设计？】" << endl;
    cout << "  1. 历史原因：节省空间（栈和堆从两端向中间长）" << endl;
    cout << "  2. 硬件支持：CPU 指令就是这样设计的" << endl;
    cout << "  3. 灵活高效：最大化利用内存" << endl;
    
    cout << "\n【关键理解】" << endl;
    cout << "  ✅ 栈有固定的地址范围（例如 8 MB）" << endl;
    cout << "  ✅ 从高地址向低地址分配" << endl;
    cout << "  ✅ 超过低地址边界 = 栈溢出" << endl;
    cout << "  ✅ 堆从低向高，栈从高向低，充分利用空间" << endl;
}

int main() {
    cout << "=== 为什么栈从高地址向低地址分配？ ===" << endl;
    
    // 1. 栈的分配机制
    stackAllocationMechanism();
    
    // 2. 为什么从高到低
    whyHighToLow();
    
    // 3. 栈溢出详解
    stackOverflowExplained();
    
    // 4. 实际演示
    demonstrateStackPointer();
    
    // 5. 栈大小和限制
    stackSizeAndLimits();
    
    // 6. 为什么不从低到高
    whyNotLowToHigh();
    
    // 7. 栈溢出示例
    causeStackOverflowByRecursion();
    
    // 8. 总结
    summarize();
    
    return 0;
}

/*
 * ========== 终极总结 ==========
 * 
 * 问题 1：地址不应该是从小向大分吗？
 * 回答：
 *   - 堆：从小到大（0x1000 → 0x2000 → ...）
 *   - 栈：从大到小（0x7fff → 0x7ffe → ...）
 *   - 栈是特殊的！
 * 
 * 
 * 问题 2：是从程序的栈内存最大地址开始分？
 * 回答：
 *   ✅ 完全正确！
 *   
 *   操作系统分配栈空间：
 *     栈底（Stack Bottom）：0x7fff0000（最高地址）
 *     栈限（Stack Limit）： 0x7fe00000（最低地址）
 *     栈大小：8 MB
 *   
 *   栈指针（SP）从 0x7fff0000 开始：
 *     分配变量 → SP 向下移（0x7fff0000 → 0x7ffefffc → ...）
 *     释放变量 → SP 向上移（0x7ffefffc → 0x7fff0000 → ...）
 * 
 * 
 * 问题 3：分到最小地址，岂不是栈溢出了？
 * 回答：
 *   ✅ 完全正确！这就是栈溢出的原理！
 *   
 *   栈溢出的过程：
 *     1. 栈指针从 0x7fff0000 开始
 *     2. 不断分配变量，SP 向下移
 *     3. SP 移动到 0x7fe00000（栈限）
 *     4. 继续分配 → 超过栈限 → Stack Overflow！
 *   
 *   触发栈溢出的原因：
 *     - 无限递归
 *     - 局部数组太大
 *     - 函数嵌套太深
 * 
 * 
 * 为什么要从高到低？
 * 
 *   历史原因：
 *     早期内存小，栈和堆从两端向中间长，最大化利用空间
 *     
 *     高地址
 *     ┌──────────┐
 *     │  栈  ↓   │ ← 从上往下
 *     ├──────────┤
 *     │  空闲    │ ← 共享空间
 *     ├──────────┤
 *     │  堆  ↑   │ ← 从下往上
 *     └──────────┘
 *     低地址
 *   
 *   硬件支持：
 *     CPU 的 PUSH/POP 指令天然支持向下增长
 *   
 *   设计优势：
 *     ✅ 灵活：没有固定分界线
 *     ✅ 高效：只有真正用完才溢出
 *     ✅ 简单：栈用指针，堆用分配器
 * 
 * 
 * 记忆要点：
 * 
 *   1. 栈有固定的地址范围（例如 8 MB）
 *      例如：0x7fff0000（高）~ 0x7fe00000（低）
 *   
 *   2. 栈指针从最高地址开始，向下移动
 *      分配变量：SP 减小（向下）
 *      释放变量：SP 增大（向上）
 *   
 *   3. 超过最低地址 = 栈溢出
 *      SP < 0x7fe00000 → Stack Overflow
 *   
 *   4. 栈和堆从两端向中间长
 *      充分利用内存空间
 * 
 * 
 * 类比记忆：
 * 
 *   栈就像一栋楼：
 *     - OS 分配了 1-10 楼给你（栈空间）
 *     - 你从 10 楼开始住（高地址）
 *     - 每来一个人，往下分配一层（9 楼、8 楼...）
 *     - 如果住满了 1 楼还有人来 → 栈溢出！
 */
