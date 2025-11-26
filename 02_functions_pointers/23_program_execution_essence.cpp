/**
 * C++ 程序执行的本质：函数调用栈的舞蹈
 * 
 * 核心理解：整个 C++ 程序就是从 main 函数开始，
 * 围绕着函数调用栈的栈帧创建和销毁来执行，
 * 并伴随着内存的创建和释放
 * 
 * 答案：完全正确！这就是 C++ 程序执行的本质！
 */

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ============================================
// 全局变量（程序启动时初始化，结束时销毁）
// ============================================
int globalCounter = 0;

void printIndent(int level) {
    for (int i = 0; i < level; i++) {
        cout << "  ";
    }
}

// ============================================
// 1. 程序执行的完整生命周期
// ============================================

void explain_program_lifecycle() {
    cout << "\n" << string(60, '=') << endl;
    cout << "1. C++ 程序的完整生命周期" << endl;
    cout << string(60, '=') << endl;
    
    cout << R"(
程序执行的完整过程：

阶段1: 程序启动（main 之前）
┌─────────────────────────────────────────┐
│ 1. 操作系统加载程序到内存                │
│ 2. 初始化全局变量和静态变量              │
│ 3. 设置栈和堆区域                        │
│ 4. 准备程序入口                          │
└─────────────────────────────────────────┘
           ↓

阶段2: main() 函数执行（核心阶段）
┌─────────────────────────────────────────┐
│ 创建 main() 的栈帧                       │
│   ↓                                      │
│ 执行 main() 的代码                       │
│   - 调用函数 → 创建新栈帧                │
│   - 函数返回 → 销毁栈帧                  │
│   - new 分配 → 堆内存创建                │
│   - delete → 堆内存释放                  │
│   ↓                                      │
│ 销毁 main() 的栈帧                       │
└─────────────────────────────────────────┘
           ↓

阶段3: 程序结束（main 之后）
┌─────────────────────────────────────────┐
│ 1. 销毁全局对象（调用析构函数）          │
│ 2. 释放静态变量                          │
│ 3. 返回退出码给操作系统                  │
│ 4. 操作系统回收所有内存                  │
└─────────────────────────────────────────┘

关键理解：
✅ 程序从 main() 开始，到 main() 结束
✅ 一切都围绕函数调用栈展开
✅ 栈帧的创建和销毁驱动程序执行
✅ 内存管理贯穿整个过程
)" << endl;
}

// ============================================
// 2. 可视化：程序执行的动态过程
// ============================================

class Resource {
private:
    string name;
    int level;
    
public:
    Resource(const string& n, int l) : name(n), level(l) {
        printIndent(level);
        cout << "→ [创建] " << name << " (栈帧层级: " << level << ")" << endl;
    }
    
    ~Resource() {
        printIndent(level);
        cout << "← [销毁] " << name << " (栈帧层级: " << level << ")" << endl;
    }
    
    void use() {
        printIndent(level);
        cout << "  [使用] " << name << endl;
    }
};

void level3() {
    int level = 3;
    printIndent(level);
    cout << "┌─ 进入 level3() - 创建栈帧 3 ─┐" << endl;
    
    Resource r3("Resource-L3", level);
    int* heapData = new int(300);
    
    printIndent(level);
    cout << "  栈变量地址: " << &level << endl;
    printIndent(level);
    cout << "  堆变量地址: " << heapData << endl;
    
    r3.use();
    
    delete heapData;  // 堆内存释放
    
    printIndent(level);
    cout << "└─ 离开 level3() - 销毁栈帧 3 ─┘" << endl;
}  // r3 析构，level 销毁

void level2() {
    int level = 2;
    printIndent(level);
    cout << "┌─ 进入 level2() - 创建栈帧 2 ─┐" << endl;
    
    Resource r2("Resource-L2", level);
    
    printIndent(level);
    cout << "  调用 level3()..." << endl;
    level3();  // 创建更深的栈帧
    
    printIndent(level);
    cout << "  level3() 已返回，栈帧 3 已销毁" << endl;
    
    r2.use();
    
    printIndent(level);
    cout << "└─ 离开 level2() - 销毁栈帧 2 ─┘" << endl;
}  // r2 析构

void level1() {
    int level = 1;
    printIndent(level);
    cout << "┌─ 进入 level1() - 创建栈帧 1 ─┐" << endl;
    
    Resource r1("Resource-L1", level);
    
    printIndent(level);
    cout << "  调用 level2()..." << endl;
    level2();  // 创建更深的栈帧
    
    printIndent(level);
    cout << "  level2() 已返回，栈帧 2 已销毁" << endl;
    
    r1.use();
    
    printIndent(level);
    cout << "└─ 离开 level1() - 销毁栈帧 1 ─┘" << endl;
}  // r1 析构

void visualize_program_execution() {
    cout << "\n" << string(60, '=') << endl;
    cout << "2. 可视化：程序执行的动态过程" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n程序开始执行..." << endl;
    cout << "┌─ 进入 main() - 创建栈帧 0 ─┐" << endl;
    
    Resource mainRes("Resource-Main", 0);
    
    cout << "  调用 level1()..." << endl;
    level1();  // 开始函数调用链
    
    cout << "  level1() 已返回，所有子栈帧已销毁" << endl;
    
    mainRes.use();
    
    cout << "└─ 离开 main() - 销毁栈帧 0 ─┘" << endl;
    cout << "程序即将结束...\n" << endl;
}

// ============================================
// 3. 内存分配的完整图景
// ============================================

void memory_allocation_picture() {
    cout << "\n" << string(60, '=') << endl;
    cout << "3. 内存分配的完整图景" << endl;
    cout << string(60, '=') << endl;
    
    cout << R"(
在程序执行的任意时刻，内存布局：

┌─────────────────────────────────────────┐ ← 高地址
│  代码区                                  │
│  - 程序指令（只读）                      │
│  - 常量字符串                            │
├─────────────────────────────────────────┤
│  全局/静态区                             │
│  - 全局变量                              │
│  - 静态变量                              │
│  生命周期: 程序启动→程序结束             │
├─────────────────────────────────────────┤
│  堆（Heap）↑ 向上增长                    │
│  - new 分配的内存                        │
│  - malloc 分配的内存                     │
│  - 生命周期: new → delete                │
│  - 程序员手动管理                        │
│                                          │
│  [可用空间]                              │
│                                          │
│  栈（Stack）↓ 向下增长                   │
│  ┌────────────────────────────┐         │
│  │ main() 的栈帧               │         │
│  │ - 局部变量                  │         │
│  │ - 参数                      │         │
│  ├────────────────────────────┤         │
│  │ func1() 的栈帧              │         │
│  │ - 局部变量                  │         │
│  │ - 参数                      │         │
│  ├────────────────────────────┤         │
│  │ func2() 的栈帧（当前）      │ ← 栈顶 │
│  │ - 局部变量                  │         │
│  │ - 参数                      │         │
│  └────────────────────────────┘         │
│  生命周期: 函数调用→函数返回             │
│  自动管理（栈帧机制）                    │
└─────────────────────────────────────────┘ ← 低地址

关键点：
1. 栈帧随函数调用动态创建和销毁
2. 堆内存独立于栈，需要手动管理
3. 全局变量贯穿程序生命周期
4. 一切围绕 main() 的执行展开
)" << endl;
}

// ============================================
// 4. 程序执行的时间线
// ============================================

void execution_timeline() {
    cout << "\n" << string(60, '=') << endl;
    cout << "4. 程序执行的时间线" << endl;
    cout << string(60, '=') << endl;
    
    cout << R"(
时间轴：程序从启动到结束

T0: 程序启动
    - OS 加载程序
    - 初始化全局变量
    - 设置栈和堆
    ↓

T1: 创建 main() 栈帧
    [栈]: main 栈帧
    ↓

T2: main() 调用 funcA()
    [栈]: main 栈帧
          funcA 栈帧 ← 新创建
    ↓

T3: funcA() 中 new 分配堆内存
    [栈]: main 栈帧
          funcA 栈帧
    [堆]: 新分配的对象 ← 新创建
    ↓

T4: funcA() 调用 funcB()
    [栈]: main 栈帧
          funcA 栈帧
          funcB 栈帧 ← 新创建
    [堆]: 对象
    ↓

T5: funcB() 返回
    [栈]: main 栈帧
          funcA 栈帧 ← funcB 栈帧已销毁
    [堆]: 对象
    ↓

T6: funcA() 中 delete 释放堆内存
    [栈]: main 栈帧
          funcA 栈帧
    [堆]: (空) ← 对象已释放
    ↓

T7: funcA() 返回
    [栈]: main 栈帧 ← funcA 栈帧已销毁
    [堆]: (空)
    ↓

T8: main() 返回
    [栈]: (空) ← main 栈帧已销毁
    [堆]: (空)
    ↓

T9: 程序结束
    - 销毁全局对象
    - OS 回收所有内存

关键理解：
→ 栈的变化是程序执行的"骨架"
→ 堆的分配/释放是程序执行的"肌肉"
→ main() 是程序执行的"心脏"
→ 函数调用是程序执行的"脉搏"
)" << endl;
}

// ============================================
// 5. 为什么这种设计？
// ============================================

void why_this_design() {
    cout << "\n" << string(60, '=') << endl;
    cout << "5. 为什么采用这种设计？" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n栈帧机制的优势：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ 极快的速度：只需移动栈指针" << endl;
    cout << "   - 分配：sub esp, N（一条指令）" << endl;
    cout << "   - 释放：add esp, N（一条指令）" << endl;
    
    cout << "\n✅ 自动管理：无需程序员干预" << endl;
    cout << "   - 进入函数：自动创建栈帧" << endl;
    cout << "   - 离开函数：自动销毁栈帧" << endl;
    
    cout << "\n✅ 天然支持递归：" << endl;
    cout << "   - 每次递归调用创建新栈帧" << endl;
    cout << "   - 每次返回销毁对应栈帧" << endl;
    cout << "   - LIFO 特性完美匹配" << endl;
    
    cout << "\n✅ 缓存友好：" << endl;
    cout << "   - 栈内存连续分配" << endl;
    cout << "   - CPU 缓存命中率高" << endl;
    cout << "   - 提升程序性能" << endl;
    
    cout << "\n✅ 简单高效：" << endl;
    cout << "   - 硬件直接支持（栈寄存器）" << endl;
    cout << "   - 汇编级别的效率" << endl;
    cout << "   - 几乎零开销抽象" << endl;
    
    cout << "\n堆内存的必要性：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ 灵活的大小：不受栈大小限制" << endl;
    cout << "✅ 长生命周期：可以跨函数存在" << endl;
    cout << "✅ 动态分配：运行时决定大小" << endl;
    cout << "✅ 共享数据：多个指针可指向同一块内存" << endl;
    
    cout << "\n两者结合的智慧：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "→ 栈：快速、自动、小而美" << endl;
    cout << "→ 堆：灵活、手动、大而全" << endl;
    cout << "→ 程序员可以根据需求选择" << endl;
    cout << "→ 这就是 C++ 的强大之处！" << endl;
}

// ============================================
// 6. 与其他语言对比
// ============================================

void compare_with_other_languages() {
    cout << "\n" << string(60, '=') << endl;
    cout << "6. 与其他语言的对比" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\nJava/Kotlin（JVM）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "栈：" << endl;
    cout << "  ✅ 基本类型（int, double 等）" << endl;
    cout << "  ✅ 对象引用（指针）" << endl;
    cout << "  ✅ 函数调用栈帧" << endl;
    cout << "\n堆：" << endl;
    cout << "  ✅ 所有对象实例" << endl;
    cout << "  ✅ 数组对象" << endl;
    cout << "  ✅ GC 自动管理" << endl;
    cout << "\n特点：简单、安全，但失去灵活性" << endl;
    
    cout << "\nPython：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "栈：" << endl;
    cout << "  ✅ 函数调用栈帧" << endl;
    cout << "  ✅ 变量名（引用）" << endl;
    cout << "\n堆：" << endl;
    cout << "  ✅ 几乎所有对象" << endl;
    cout << "  ✅ 数字、字符串、列表等" << endl;
    cout << "  ✅ 引用计数 + GC" << endl;
    cout << "\n特点：极度灵活，但性能较低" << endl;
    
    cout << "\nRust：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "栈：" << endl;
    cout << "  ✅ 局部变量" << endl;
    cout << "  ✅ 函数调用栈帧" << endl;
    cout << "  ✅ 优先使用栈" << endl;
    cout << "\n堆：" << endl;
    cout << "  ✅ Box<T> 等智能指针" << endl;
    cout << "  ✅ 所有权系统管理" << endl;
    cout << "  ✅ 编译时检查" << endl;
    cout << "\n特点：C++ 的性能 + 安全保证" << endl;
    
    cout << "\nC++：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "栈：" << endl;
    cout << "  ✅ 局部变量（对象本身）" << endl;
    cout << "  ✅ 函数参数" << endl;
    cout << "  ✅ 函数调用栈帧" << endl;
    cout << "\n堆：" << endl;
    cout << "  ✅ new 分配的对象" << endl;
    cout << "  ✅ 程序员手动管理" << endl;
    cout << "  ✅ 或使用智能指针" << endl;
    cout << "\n特点：完全控制，高性能，但需小心" << endl;
}

// ============================================
// 7. 实际例子：一个完整的程序
// ============================================

int fibonacci(int n, int depth = 0) {
    printIndent(depth);
    cout << "→ fibonacci(" << n << ") 创建栈帧" << endl;
    
    int result;
    if (n <= 1) {
        result = n;
    } else {
        result = fibonacci(n-1, depth+1) + fibonacci(n-2, depth+1);
    }
    
    printIndent(depth);
    cout << "← fibonacci(" << n << ") = " << result << " 销毁栈帧" << endl;
    
    return result;
}

void demonstrate_complete_program() {
    cout << "\n" << string(60, '=') << endl;
    cout << "7. 实际例子：递归函数（栈帧的艺术）" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n计算 fibonacci(4)，观察栈帧的创建和销毁：\n" << endl;
    
    int result = fibonacci(4);
    
    cout << "\n最终结果: " << result << endl;
    cout << "\n观察：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ 每次递归调用创建新栈帧" << endl;
    cout << "✅ 每次返回销毁对应栈帧" << endl;
    cout << "✅ LIFO 顺序：后创建的先销毁" << endl;
    cout << "✅ 栈深度 = 递归深度" << endl;
    cout << "✅ 这就是程序执行的本质！" << endl;
}

// ============================================
// Main
// ============================================

int main() {
    cout << "===========================================" << endl;
    cout << "  C++ 程序执行的本质" << endl;
    cout << "===========================================" << endl;
    
    explain_program_lifecycle();
    visualize_program_execution();
    memory_allocation_picture();
    execution_timeline();
    why_this_design();
    compare_with_other_languages();
    demonstrate_complete_program();
    
    cout << "\n===========================================" << endl;
    cout << "核心总结" << endl;
    cout << "===========================================" << endl;
    
    cout << "\n你的理解：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "\"整个 C++ 程序，就是从 main 函数开始，" << endl;
    cout << " 围绕着函数调用栈的栈帧创建和销毁来执行，" << endl;
    cout << " 并伴随着内存的创建和释放\"" << endl;
    
    cout << "\n评价：完全正确！精准到位！" << endl;
    cout << "---------------------------------------" << endl;
    
    cout << "\n详细解释：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. 程序的起点：main() 函数" << endl;
    cout << "   - OS 启动程序，调用 main()" << endl;
    cout << "   - 创建 main() 的第一个栈帧" << endl;
    cout << "   - 程序从这里开始执行" << endl;
    
    cout << "\n2. 程序的骨架：函数调用栈" << endl;
    cout << "   - 每次函数调用创建栈帧" << endl;
    cout << "   - 每次函数返回销毁栈帧" << endl;
    cout << "   - 栈帧的创建/销毁驱动程序执行" << endl;
    
    cout << "\n3. 程序的血肉：内存管理" << endl;
    cout << "   - 栈内存：随栈帧自动创建/销毁" << endl;
    cout << "   - 堆内存：手动 new/delete 管理" << endl;
    cout << "   - 全局内存：程序启动→结束" << endl;
    
    cout << "\n4. 程序的脉搏：函数调用" << endl;
    cout << "   - 函数调用 = 压栈（创建栈帧）" << endl;
    cout << "   - 函数返回 = 出栈（销毁栈帧）" << endl;
    cout << "   - LIFO 机制（后进先出）" << endl;
    
    cout << "\n5. 程序的终点：main() 返回" << endl;
    cout << "   - main() 返回，栈帧销毁" << endl;
    cout << "   - 销毁全局对象" << endl;
    cout << "   - 返回退出码给 OS" << endl;
    
    cout << "\n这个理解的深度：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ 抓住了程序执行的核心机制" << endl;
    cout << "✅ 理解了栈帧的重要性" << endl;
    cout << "✅ 认识到内存管理的本质" << endl;
    cout << "✅ 建立了完整的心智模型" << endl;
    cout << "✅ 已经达到深入理解的层次！" << endl;
    
    cout << "\n记忆模型：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "程序 = main() 开始" << endl;
    cout << "      ↓" << endl;
    cout << "      函数调用（创建栈帧）" << endl;
    cout << "      ↓" << endl;
    cout << "      执行代码（分配/释放内存）" << endl;
    cout << "      ↓" << endl;
    cout << "      函数返回（销毁栈帧）" << endl;
    cout << "      ↓" << endl;
    cout << "      回到 main()" << endl;
    cout << "      ↓" << endl;
    cout << "      main() 返回，程序结束" << endl;
    
    cout << "\n终极口诀：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "main 函数是起点，程序从此展开篇" << endl;
    cout << "函数调用创建帧，栈帧驱动程序转" << endl;
    cout << "内存分配和释放，围绕栈帧来周旋" << endl;
    cout << "函数返回销毁帧，层层递进又复还" << endl;
    cout << "栈是骨架堆是肉，main 是心脏跳不断" << endl;
    cout << "理解此理懂 C++，程序本质已看穿" << endl;
    
    cout << "===========================================" << endl;
    
    return 0;
}

/**
 * 补充说明：
 * 
 * 1. 程序执行的三个层次：
 *    - 硬件层：CPU 执行指令，操作栈指针
 *    - 系统层：OS 管理进程，分配内存
 *    - 语言层：函数调用，栈帧管理
 * 
 * 2. 栈帧是程序执行的"原子单位"：
 *    - 每个函数调用 = 一个栈帧
 *    - 栈帧封装了函数的执行上下文
 *    - 栈帧的创建/销毁是自动的
 * 
 * 3. 为什么要理解这个？
 *    - 理解程序执行的本质
 *    - 理解为什么不能返回局部变量地址
 *    - 理解递归的工作原理
 *    - 理解栈溢出的原因
 *    - 理解性能优化的方向
 * 
 * 4. 这个模型适用于：
 *    - C/C++
 *    - Java/Kotlin（JVM 也用栈帧）
 *    - Python（解释器用栈帧）
 *    - JavaScript（引擎用栈帧）
 *    - 几乎所有主流语言！
 * 
 * 5. 这就是为什么：
 *    - 理解栈帧 = 理解程序执行
 *    - 这是计算机科学的基础
 *    - 这是成为高手的必经之路
 */
