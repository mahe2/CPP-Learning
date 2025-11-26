/**
 * 栈帧的数据结构和创建机制详解
 * 
 * 核心问题：
 * 1. 栈帧有具体的数据结构吗？—— 有！非常明确的结构
 * 2. 栈帧是谁创建的？—— 编译器生成的代码 + CPU 硬件协作
 */

#include <iostream>
#include <iomanip>
#include <cstdint>

using namespace std;

// ============================================
// 1. 栈帧的数据结构
// ============================================

void explain_stack_frame_structure() {
    cout << "\n" << string(70, '=') << endl;
    cout << "1. 栈帧的具体数据结构" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
栈帧（Stack Frame）= 活动记录（Activation Record）

每个函数调用都会在栈上创建一个栈帧，包含：

┌──────────────────────────────────────────┐ ← 高地址（栈底方向）
│  1. 函数参数（Parameters）                │
│     - 调用者传递的参数                    │
│     - 从右到左压栈（C/C++ 调用约定）      │
├──────────────────────────────────────────┤
│  2. 返回地址（Return Address）            │
│     - 函数返回后要执行的下一条指令地址    │
│     - CPU 自动压入（call 指令）           │
├──────────────────────────────────────────┤
│  3. 旧的栈帧指针（Old Frame Pointer）     │ ← EBP/RBP 指向这里
│     - 保存调用者的栈帧基址                │
│     - 用于恢复调用者的栈帧                │
├──────────────────────────────────────────┤
│  4. 局部变量（Local Variables）           │
│     - 函数内部声明的变量                  │
│     - 按声明顺序分配                      │
├──────────────────────────────────────────┤
│  5. 临时变量（Temporaries）               │
│     - 表达式计算的中间结果                │
│     - 编译器自动管理                      │
├──────────────────────────────────────────┤
│  6. 保存的寄存器（Saved Registers）       │ ← ESP/RSP 指向这里
│     - 需要保护的寄存器值                  │
│     - 函数返回时恢复                      │
└──────────────────────────────────────────┘ ← 低地址（栈顶）

关键寄存器：
- ESP/RSP (Stack Pointer)：指向栈顶（当前栈帧的底部）
- EBP/RBP (Base Pointer)：指向栈帧基址（当前栈帧的顶部）

注意：
- x86 (32位): ESP, EBP
- x86-64 (64位): RSP, RBP
)" << endl;
}

// ============================================
// 2. 实际的栈帧布局演示
// ============================================

// 这个函数展示实际的栈帧布局
int add(int a, int b) {
    int result = a + b;
    int temp = result * 2;
    
    cout << "--- add() 函数的栈帧 ---" << endl;
    cout << "参数 a 的地址:        " << &a << endl;
    cout << "参数 b 的地址:        " << &b << endl;
    cout << "局部变量 result 地址: " << &result << endl;
    cout << "局部变量 temp 地址:   " << &temp << endl;
    
    // 观察地址的相对位置
    long diff_a_b = (long)&b - (long)&a;
    long diff_result_a = (long)&a - (long)&result;
    
    cout << "\n地址差分析：" << endl;
    cout << "b - a = " << diff_a_b << " 字节" << endl;
    cout << "a - result = " << diff_result_a << " 字节" << endl;
    cout << "(可以看出参数和局部变量在栈上的布局)" << endl;
    
    return result;
}

void demonstrate_stack_frame_layout() {
    cout << "\n" << string(70, '=') << endl;
    cout << "2. 实际的栈帧布局" << endl;
    cout << string(70, '=') << endl;
    
    int x = 10, y = 20;
    cout << "\nmain() 中的变量：" << endl;
    cout << "x 的地址: " << &x << endl;
    cout << "y 的地址: " << &y << endl;
    
    cout << "\n调用 add(x, y)...\n" << endl;
    int result = add(x, y);
    
    cout << "\n返回后，result 的地址: " << &result << endl;
    cout << "结果值: " << result << endl;
}

// ============================================
// 3. 谁创建栈帧？—— 编译器生成的汇编代码
// ============================================

void explain_who_creates_stack_frame() {
    cout << "\n" << string(70, '=') << endl;
    cout << "3. 谁创建栈帧？" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
答案：编译器生成的汇编代码 + CPU 硬件协作

创建过程分为两部分：

【阶段1：函数调用者（Caller）的工作】
-------------------------------------
调用者执行：call function_name

这条指令做两件事：
1. push 返回地址到栈（硬件自动）
2. 跳转到函数入口（硬件自动）

汇编示例：
    ; 准备参数（从右到左）
    push 20          ; 参数 b
    push 10          ; 参数 a
    
    ; 调用函数
    call add         ; CPU 自动 push 返回地址 + 跳转
    
    ; 返回后清理参数（根据调用约定）
    add esp, 8       ; 弹出 2 个参数（2 * 4 字节）


【阶段2：被调用函数（Callee）的工作】
-------------------------------------
函数入口：设置栈帧（Prologue）

编译器生成的代码：
    ; 保存旧的栈帧基址
    push ebp         ; 保存调用者的栈帧基址
    
    ; 设置新的栈帧基址
    mov ebp, esp     ; 当前栈顶成为新栈帧的基址
    
    ; 为局部变量分配空间
    sub esp, 16      ; 分配 16 字节（局部变量）
    
    ; 现在栈帧已创建，可以使用局部变量


函数出口：销毁栈帧（Epilogue）

编译器生成的代码：
    ; 释放局部变量空间
    mov esp, ebp     ; 恢复栈指针（相当于 add esp, 16）
    
    ; 恢复旧的栈帧基址
    pop ebp          ; 弹出保存的 ebp
    
    ; 返回到调用者
    ret              ; 弹出返回地址 + 跳转


【关键理解】
-------------------------------------
✅ 栈帧是编译器生成的代码创建的
✅ CPU 硬件提供支持（call/ret 指令）
✅ 栈指针寄存器（ESP/RSP）自动维护
✅ 整个过程完全自动化
✅ 程序员无需干预（除非写汇编）

)" << endl;
}

// ============================================
// 4. 模拟栈帧创建过程
// ============================================

void simulate_stack_frame_creation() {
    cout << "\n" << string(70, '=') << endl;
    cout << "4. 模拟栈帧创建过程" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
假设有函数调用：main() -> funcA(10, 20) -> funcB(30)

步骤1：main() 的栈帧（初始状态）
┌────────────────────┐ ← EBP
│ main 局部变量      │
│ ...                │
└────────────────────┘ ← ESP


步骤2：main() 准备调用 funcA(10, 20)
┌────────────────────┐ ← EBP
│ main 局部变量      │
│ ...                │
├────────────────────┤
│ 20 (参数2)         │
├────────────────────┤
│ 10 (参数1)         │
└────────────────────┘ ← ESP


步骤3：执行 call funcA（CPU 自动压入返回地址）
┌────────────────────┐ ← EBP
│ main 局部变量      │
│ ...                │
├────────────────────┤
│ 20 (参数2)         │
├────────────────────┤
│ 10 (参数1)         │
├────────────────────┤
│ 返回地址           │ ← CPU 自动压入
└────────────────────┘ ← ESP


步骤4：funcA 入口执行 push ebp
┌────────────────────┐ ← EBP
│ main 局部变量      │
│ ...                │
├────────────────────┤
│ 20 (参数2)         │
├────────────────────┤
│ 10 (参数1)         │
├────────────────────┤
│ 返回地址           │
├────────────────────┤
│ 旧 EBP             │ ← 保存 main 的 EBP
└────────────────────┘ ← ESP


步骤5：funcA 执行 mov ebp, esp（设置新栈帧基址）
┌────────────────────┐
│ main 局部变量      │
│ ...                │
├────────────────────┤
│ 20 (参数2)         │
├────────────────────┤
│ 10 (参数1)         │
├────────────────────┤
│ 返回地址           │
├────────────────────┤
│ 旧 EBP             │ ← EBP（funcA 的栈帧基址）
└────────────────────┘ ← ESP


步骤6：funcA 执行 sub esp, 16（分配局部变量）
┌────────────────────┐
│ main 局部变量      │
│ ...                │
├────────────────────┤
│ 20 (参数2)         │ ← [EBP + 12]
├────────────────────┤
│ 10 (参数1)         │ ← [EBP + 8]
├────────────────────┤
│ 返回地址           │ ← [EBP + 4]
├────────────────────┤
│ 旧 EBP             │ ← [EBP]（funcA 的栈帧基址）
├────────────────────┤
│ funcA 局部变量1    │ ← [EBP - 4]
├────────────────────┤
│ funcA 局部变量2    │ ← [EBP - 8]
├────────────────────┤
│ funcA 局部变量3    │ ← [EBP - 12]
├────────────────────┤
│ funcA 局部变量4    │ ← [EBP - 16]
└────────────────────┘ ← ESP

现在 funcA 的栈帧完全建立！

访问方式：
- 参数：   [EBP + 8], [EBP + 12], ...
- 局部变量：[EBP - 4], [EBP - 8], ...

)" << endl;
}

// ============================================
// 5. 实际的汇编代码（简化版）
// ============================================

void show_actual_assembly() {
    cout << "\n" << string(70, '=') << endl;
    cout << "5. 实际的汇编代码示例" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
C++ 代码：
---------
int add(int a, int b) {
    int result = a + b;
    return result;
}

int main() {
    int x = add(10, 20);
    return 0;
}


对应的汇编代码（x86-64，简化）：
---------------------------------

add:
    ; === Prologue：创建栈帧 ===
    push    rbp              ; 保存旧的栈帧基址
    mov     rbp, rsp         ; 设置新的栈帧基址
    sub     rsp, 16          ; 为局部变量分配空间
    
    ; 保存参数到栈（可能被编译器优化掉）
    mov     DWORD PTR [rbp-4], edi   ; a 参数（寄存器传参）
    mov     DWORD PTR [rbp-8], esi   ; b 参数
    
    ; === 函数体：执行计算 ===
    mov     eax, DWORD PTR [rbp-4]   ; eax = a
    add     eax, DWORD PTR [rbp-8]   ; eax += b
    mov     DWORD PTR [rbp-12], eax  ; result = eax
    
    ; 准备返回值
    mov     eax, DWORD PTR [rbp-12]  ; 返回值放入 eax
    
    ; === Epilogue：销毁栈帧 ===
    mov     rsp, rbp         ; 释放局部变量（恢复 rsp）
    pop     rbp              ; 恢复旧的栈帧基址
    ret                      ; 返回（弹出返回地址 + 跳转）


main:
    ; === Prologue ===
    push    rbp
    mov     rbp, rsp
    sub     rsp, 16
    
    ; === 调用 add(10, 20) ===
    mov     esi, 20          ; 参数 b（寄存器传参）
    mov     edi, 10          ; 参数 a（寄存器传参）
    call    add              ; 调用函数
    
    ; 保存返回值
    mov     DWORD PTR [rbp-4], eax  ; x = add(...)
    
    ; === Epilogue ===
    mov     eax, 0           ; 返回 0
    mov     rsp, rbp
    pop     rbp
    ret


关键观察：
---------
1. Prologue (push rbp; mov rbp, rsp; sub rsp, N)
   → 这三条指令创建栈帧

2. Epilogue (mov rsp, rbp; pop rbp; ret)
   → 这三条指令销毁栈帧

3. call 指令
   → 自动 push 返回地址 + 跳转

4. ret 指令
   → 自动 pop 返回地址 + 跳转

5. 参数传递
   → 现代 x86-64：前 6 个整数参数用寄存器（rdi, rsi, rdx, rcx, r8, r9）
   → 更多参数：压栈传递
   → 老式 x86：所有参数都压栈

)" << endl;
}

// ============================================
// 6. 栈帧的内存布局实际测试
// ============================================

// 使用 __attribute__ 强制不优化，更清晰地看到栈帧
__attribute__((noinline))
void testFunction(int param1, int param2, int param3) {
    int local1 = 100;
    int local2 = 200;
    int local3 = 300;
    
    cout << "\n=== testFunction 的栈帧布局 ===" << endl;
    cout << "\n参数地址：" << endl;
    cout << "param1: " << &param1 << endl;
    cout << "param2: " << &param2 << endl;
    cout << "param3: " << &param3 << endl;
    
    cout << "\n局部变量地址：" << endl;
    cout << "local1: " << &local1 << endl;
    cout << "local2: " << &local2 << endl;
    cout << "local3: " << &local3 << endl;
    
    // 计算地址差
    cout << "\n地址差分析（字节）：" << endl;
    cout << "param2 - param1 = " << (intptr_t)&param2 - (intptr_t)&param1 << endl;
    cout << "param3 - param2 = " << (intptr_t)&param3 - (intptr_t)&param2 << endl;
    cout << "local1 - local2 = " << (intptr_t)&local1 - (intptr_t)&local2 << endl;
    cout << "local2 - local3 = " << (intptr_t)&local2 - (intptr_t)&local3 << endl;
    
    cout << "\n观察：" << endl;
    cout << "- 参数之间的地址差（通常 4 字节，int 的大小）" << endl;
    cout << "- 局部变量之间的地址差" << endl;
    cout << "- 栈向下增长（参数在高地址，局部变量在低地址）" << endl;
}

void test_stack_frame_memory() {
    cout << "\n" << string(70, '=') << endl;
    cout << "6. 栈帧内存布局实际测试" << endl;
    cout << string(70, '=') << endl;
    
    testFunction(10, 20, 30);
}

// ============================================
// 7. 栈帧的高级特性
// ============================================

void explain_advanced_features() {
    cout << "\n" << string(70, '=') << endl;
    cout << "7. 栈帧的高级特性" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
1. 栈帧省略优化（Frame Pointer Omission）
------------------------------------------
现代编译器在优化时可能省略 EBP/RBP：
- 使用 ESP/RSP 直接访问局部变量
- 节省一个寄存器
- 减少 push/pop 操作
- 优化选项：-fomit-frame-pointer

优化前：
    push ebp
    mov ebp, esp
    sub esp, 16
    mov [ebp-4], eax    ; 通过 ebp 访问

优化后：
    sub esp, 16
    mov [esp+12], eax   ; 直接通过 esp 访问


2. 调用约定（Calling Conventions）
----------------------------------
决定如何传递参数和返回值：

cdecl（C/C++ 默认）：
- 参数从右到左压栈
- 调用者清理栈
- 支持可变参数（printf）

stdcall（Windows API）：
- 参数从右到左压栈
- 被调用者清理栈
- 不支持可变参数

fastcall：
- 前几个参数用寄存器传递
- 剩余参数压栈
- 更快

x86-64 System V ABI（Linux/macOS）：
- 前 6 个整数参数：rdi, rsi, rdx, rcx, r8, r9
- 前 8 个浮点参数：xmm0-xmm7
- 更多参数压栈
- 非常高效


3. 栈对齐（Stack Alignment）
---------------------------
现代 CPU 要求栈对齐到特定边界：
- x86: 4 字节对齐
- x86-64: 16 字节对齐（SSE 指令要求）

编译器自动处理：
    sub rsp, 16      ; 分配 16 字节，保持对齐
    and rsp, -16     ; 强制对齐到 16 字节边界


4. 栈保护（Stack Protection）
----------------------------
防止栈溢出攻击：

Stack Canary（金丝雀值）：
    push ebp
    mov ebp, esp
    sub esp, 20
    mov eax, [gs:0x14]   ; 从 TLS 读取 canary
    mov [ebp-4], eax     ; 放在局部变量后面
    
    ; ... 函数体 ...
    
    mov eax, [ebp-4]     ; 检查 canary
    xor eax, [gs:0x14]
    je .L_ok
    call __stack_chk_fail  ; canary 被破坏！
.L_ok:
    pop ebp
    ret

编译选项：-fstack-protector


5. 变长数组（Variable Length Arrays）
------------------------------------
C99/C++ 支持变长数组：

    void func(int n) {
        int arr[n];  // 大小在运行时决定
    }

栈帧大小动态调整：
    sub rsp, rax     ; rax = n * sizeof(int)

这使得栈帧大小在运行时才能确定！


6. 尾调用优化（Tail Call Optimization）
--------------------------------------
如果函数最后一条语句是调用另一个函数：

    int func1() {
        return func2();  // 尾调用
    }

编译器可以优化为：
    pop rbp          ; 销毁当前栈帧
    jmp func2        ; 直接跳转（不是 call）
    
优点：
- 不创建新栈帧
- 防止栈溢出
- 支持无限递归（变成循环）

)" << endl;
}

// ============================================
// Main
// ============================================

int main() {
    cout << "=====================================================================" << endl;
    cout << "  栈帧的数据结构和创建机制详解" << endl;
    cout << "=====================================================================" << endl;
    
    explain_stack_frame_structure();
    demonstrate_stack_frame_layout();
    explain_who_creates_stack_frame();
    simulate_stack_frame_creation();
    show_actual_assembly();
    test_stack_frame_memory();
    explain_advanced_features();
    
    cout << "\n=====================================================================" << endl;
    cout << "核心总结" << endl;
    cout << "=====================================================================" << endl;
    
    cout << "\n【问题1：栈帧有具体的数据结构吗？】" << endl;
    cout << "---------------------------------------------------------------------" << endl;
    cout << "答：有！非常明确的结构" << endl;
    cout << "\n栈帧包含（从高地址到低地址）：" << endl;
    cout << "1. 函数参数（Parameters）" << endl;
    cout << "2. 返回地址（Return Address）" << endl;
    cout << "3. 旧的栈帧指针（Old Frame Pointer）" << endl;
    cout << "4. 局部变量（Local Variables）" << endl;
    cout << "5. 临时变量（Temporaries）" << endl;
    cout << "6. 保存的寄存器（Saved Registers）" << endl;
    
    cout << "\n【问题2：栈帧是谁创建的？】" << endl;
    cout << "---------------------------------------------------------------------" << endl;
    cout << "答：编译器生成的代码 + CPU 硬件协作" << endl;
    cout << "\n详细过程：" << endl;
    cout << "1. 编译器：生成 Prologue 代码（函数入口）" << endl;
    cout << "   - push rbp       (保存旧栈帧基址)" << endl;
    cout << "   - mov rbp, rsp   (设置新栈帧基址)" << endl;
    cout << "   - sub rsp, N     (分配局部变量空间)" << endl;
    cout << "\n2. CPU 硬件：执行 call 指令时自动" << endl;
    cout << "   - 压入返回地址" << endl;
    cout << "   - 跳转到函数入口" << endl;
    cout << "\n3. 编译器：生成 Epilogue 代码（函数出口）" << endl;
    cout << "   - mov rsp, rbp   (释放局部变量)" << endl;
    cout << "   - pop rbp        (恢复旧栈帧基址)" << endl;
    cout << "\n4. CPU 硬件：执行 ret 指令时自动" << endl;
    cout << "   - 弹出返回地址" << endl;
    cout << "   - 跳转回调用者" << endl;
    
    cout << "\n【关键理解】" << endl;
    cout << "---------------------------------------------------------------------" << endl;
    cout << "✅ 栈帧不是 C++ 语言级别的概念" << endl;
    cout << "✅ 栈帧是编译器和 CPU 协作的产物" << endl;
    cout << "✅ 每个函数调用自动创建栈帧" << endl;
    cout << "✅ 每个函数返回自动销毁栈帧" << endl;
    cout << "✅ 程序员无需关心（除非写汇编或调试）" << endl;
    cout << "✅ 这就是为什么局部变量能自动销毁" << endl;
    
    cout << "\n【类比理解】" << endl;
    cout << "---------------------------------------------------------------------" << endl;
    cout << "栈帧 = 函数的\"工作台\"" << endl;
    cout << "- 函数开始时：搭建工作台（Prologue）" << endl;
    cout << "- 函数执行时：在工作台上操作（使用局部变量）" << endl;
    cout << "- 函数结束时：拆除工作台（Epilogue）" << endl;
    cout << "\n编译器 = 建筑师" << endl;
    cout << "- 设计工作台的图纸（生成汇编代码）" << endl;
    cout << "\nCPU = 施工队" << endl;
    cout << "- 按照图纸搭建和拆除工作台（执行指令）" << endl;
    
    cout << "\n【记忆口诀】" << endl;
    cout << "---------------------------------------------------------------------" << endl;
    cout << "栈帧结构很明确，参数返回加基址" << endl;
    cout << "局部变量紧跟随，临时寄存器保护" << endl;
    cout << "编译器生成码，CPU 硬件执行" << endl;
    cout << "Prologue 来创建，Epilogue 去销毁" << endl;
    cout << "push pop 加 mov，三条指令全搞定" << endl;
    cout << "自动化的艺术，程序员不用管" << endl;
    
    cout << "=====================================================================" << endl;
    
    return 0;
}

/**
 * 补充说明：
 * 
 * 1. 为什么要有栈帧？
 *    - 隔离不同函数的数据
 *    - 支持递归调用
 *    - 快速分配和释放
 *    - 硬件直接支持
 * 
 * 2. 栈帧 vs 堆内存：
 *    - 栈帧：编译时大小固定，自动管理
 *    - 堆内存：运行时大小决定，手动管理
 * 
 * 3. 调试时的应用：
 *    - GDB: info frame, backtrace
 *    - LLDB: frame info, bt
 *    - 可以看到完整的栈帧链
 * 
 * 4. 性能影响：
 *    - 栈帧操作极快（几个 CPU 周期）
 *    - 栈溢出检查可能有开销
 *    - 尾调用优化可以消除栈帧
 * 
 * 5. 安全问题：
 *    - 栈溢出攻击（缓冲区溢出）
 *    - Stack Canary 保护
 *    - ASLR（地址空间随机化）
 */
