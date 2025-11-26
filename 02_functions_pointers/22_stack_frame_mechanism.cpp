/**
 * 函数调用栈和栈帧（Stack Frame）详解
 * 
 * 核心问题：栈上的内存可以自动释放，是否是因为栈变量
 * 存储在函数调用栈的栈帧中，随着函数出栈而自动销毁？
 * 
 * 答案：完全正确！这就是栈内存自动管理的核心原理！
 */

#include <iostream>
#include <string>

using namespace std;

// ============================================
// 1. 函数调用栈和栈帧的基本概念
// ============================================

void explain_stack_frame() {
    cout << "\n" << string(60, '=') << endl;
    cout << "1. 函数调用栈和栈帧的基本概念" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n什么是函数调用栈（Call Stack）？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "函数调用栈是一个 LIFO（后进先出）数据结构" << endl;
    cout << "用于管理函数调用和局部变量" << endl;
    
    cout << "\n什么是栈帧（Stack Frame）？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "每个函数调用都会创建一个栈帧" << endl;
    cout << "栈帧包含：" << endl;
    cout << "  ✅ 函数参数" << endl;
    cout << "  ✅ 返回地址（调用者地址）" << endl;
    cout << "  ✅ 局部变量" << endl;
    cout << "  ✅ 寄存器保存区" << endl;
    cout << "  ✅ 临时数据" << endl;
    
    cout << "\n栈帧的生命周期：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. 函数被调用 → 创建栈帧（压栈 push）" << endl;
    cout << "2. 执行函数体 → 使用栈帧中的变量" << endl;
    cout << "3. 函数返回 → 销毁栈帧（出栈 pop）" << endl;
    cout << "4. 局部变量自动销毁（栈指针回退）" << endl;
}

// ============================================
// 2. 栈帧的详细结构
// ============================================

void show_stack_frame_structure() {
    cout << "\n" << string(60, '=') << endl;
    cout << "2. 栈帧的详细结构" << endl;
    cout << string(60, '=') << endl;
    
    cout << R"(
函数调用栈的增长方向（从高地址到低地址）：

┌─────────────────────────────────────────────┐ ← 栈底（高地址）
│  main() 的栈帧                               │
│  ┌────────────────────────────────┐         │
│  │ main 的局部变量                │         │
│  │ int x = 10;                    │         │
│  │ string name = "Alice";         │         │
│  └────────────────────────────────┘         │
├─────────────────────────────────────────────┤
│  funcA() 的栈帧（main 调用）                 │
│  ┌────────────────────────────────┐         │
│  │ 返回地址（回到 main）           │         │
│  │ 参数                            │         │
│  │ funcA 的局部变量                │         │
│  │ int a = 20;                    │         │
│  │ double b = 3.14;               │         │
│  └────────────────────────────────┘         │
├─────────────────────────────────────────────┤
│  funcB() 的栈帧（funcA 调用）                │
│  ┌────────────────────────────────┐         │
│  │ 返回地址（回到 funcA）          │         │
│  │ 参数                            │         │
│  │ funcB 的局部变量                │         │
│  │ char c = 'X';                  │         │
│  └────────────────────────────────┘         │
├─────────────────────────────────────────────┤ ← 栈顶（低地址）
│  [未使用的栈空间]                            │
│  ↓ 继续向下增长                              │
└─────────────────────────────────────────────┘

关键点：
1. 栈从高地址向低地址增长（向下增长）
2. 每次函数调用，栈指针（SP）向下移动
3. 函数返回时，栈指针向上回退
4. 局部变量随栈帧一起创建和销毁
)" << endl;
}

// ============================================
// 3. 函数调用过程演示
// ============================================

void level3() {
    cout << "\n    [进入 level3]" << endl;
    
    int var3 = 30;
    cout << "    level3 局部变量 var3 地址: " << &var3 << endl;
    cout << "    level3 执行中..." << endl;
    
    cout << "    [level3 即将返回]" << endl;
}

void level2() {
    cout << "\n  [进入 level2]" << endl;
    
    int var2 = 20;
    cout << "  level2 局部变量 var2 地址: " << &var2 << endl;
    cout << "  level2 调用 level3..." << endl;
    
    level3();  // 创建 level3 的栈帧
    
    cout << "  [level2 从 level3 返回]" << endl;
    cout << "  var2 仍然有效: " << var2 << endl;
    cout << "  [level2 即将返回]" << endl;
}

void level1() {
    cout << "\n[进入 level1]" << endl;
    
    int var1 = 10;
    cout << "level1 局部变量 var1 地址: " << &var1 << endl;
    cout << "level1 调用 level2..." << endl;
    
    level2();  // 创建 level2 的栈帧
    
    cout << "[level1 从 level2 返回]" << endl;
    cout << "var1 仍然有效: " << var1 << endl;
    cout << "[level1 即将返回]" << endl;
}

void demonstrate_call_stack() {
    cout << "\n" << string(60, '=') << endl;
    cout << "3. 函数调用过程演示" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n调用链：main → level1 → level2 → level3\n" << endl;
    
    level1();  // 创建 level1 的栈帧
    
    cout << "\n所有函数已返回，所有栈帧已销毁" << endl;
    cout << "所有局部变量已自动释放" << endl;
}

// ============================================
// 4. 栈帧创建和销毁的详细过程
// ============================================

void detailed_stack_operations() {
    cout << "\n" << string(60, '=') << endl;
    cout << "4. 栈帧创建和销毁的详细过程" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n函数调用时（创建栈帧）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. 保存当前栈指针（Base Pointer/Frame Pointer）" << endl;
    cout << "2. 将参数压入栈" << endl;
    cout << "3. 压入返回地址（下一条指令地址）" << endl;
    cout << "4. 移动栈指针，为局部变量分配空间" << endl;
    cout << "5. 初始化局部变量" << endl;
    
    cout << "\n用汇编伪代码表示：" << endl;
    cout << "  push ebp          ; 保存旧栈帧指针" << endl;
    cout << "  mov ebp, esp      ; 设置新栈帧指针" << endl;
    cout << "  sub esp, 16       ; 为局部变量分配 16 字节" << endl;
    cout << "  ; 执行函数体..." << endl;
    
    cout << "\n函数返回时（销毁栈帧）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. 准备返回值（通常放在寄存器中）" << endl;
    cout << "2. 恢复栈指针（回退到调用前位置）" << endl;
    cout << "3. 弹出返回地址" << endl;
    cout << "4. 跳转回调用者" << endl;
    cout << "5. 局部变量空间被'释放'（栈指针回退）" << endl;
    
    cout << "\n用汇编伪代码表示：" << endl;
    cout << "  mov esp, ebp      ; 恢复栈指针" << endl;
    cout << "  pop ebp           ; 恢复旧栈帧指针" << endl;
    cout << "  ret               ; 返回（弹出返回地址并跳转）" << endl;
    
    cout << "\n关键理解：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ 局部变量的'销毁'只是栈指针回退" << endl;
    cout << "✅ 不需要逐个析构（对于 POD 类型）" << endl;
    cout << "✅ 对于类对象，会先调用析构函数" << endl;
    cout << "✅ 这就是为什么栈分配如此快速！" << endl;
}

// ============================================
// 5. 为什么栈变量会自动销毁？
// ============================================

void why_auto_destruction() {
    cout << "\n" << string(60, '=') << endl;
    cout << "5. 为什么栈变量会自动销毁？" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n答案：因为栈帧机制！" << endl;
    cout << "---------------------------------------" << endl;
    
    cout << "\n步骤1：函数调用，创建栈帧" << endl;
    cout << "  void func() {" << endl;
    cout << "    int x = 10;  // x 分配在当前栈帧中" << endl;
    cout << "  }" << endl;
    cout << "\n  调用 func() 时：" << endl;
    cout << "  1. 栈指针下移（分配空间）" << endl;
    cout << "  2. x 被存储在新栈帧中" << endl;
    cout << "  3. x 的地址 = 栈指针 + 偏移量" << endl;
    
    cout << "\n步骤2：函数返回，销毁栈帧" << endl;
    cout << "  func() 返回时：" << endl;
    cout << "  1. 栈指针上移（回到调用前位置）" << endl;
    cout << "  2. x 所在的内存空间被'释放'" << endl;
    cout << "  3. 下次函数调用会覆盖这块内存" << endl;
    
    cout << "\n关键点：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ 栈变量的地址在栈帧内" << endl;
    cout << "✅ 栈帧随函数调用创建" << endl;
    cout << "✅ 栈帧随函数返回销毁" << endl;
    cout << "✅ 因此栈变量自动销毁！" << endl;
    
    cout << "\n这就是为什么：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "❌ 不能返回局部变量的地址/引用" << endl;
    cout << "   因为栈帧销毁后，地址无效！" << endl;
    cout << "\n✅ 可以返回堆上对象的地址" << endl;
    cout << "   因为堆不受栈帧影响！" << endl;
}

// ============================================
// 6. 实际示例：观察栈帧地址
// ============================================

void observe_addresses() {
    cout << "\n" << string(60, '=') << endl;
    cout << "6. 实际示例：观察栈帧地址" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n在不同函数中观察变量地址：\n" << endl;
    
    auto func1 = []() {
        int a = 1;
        int b = 2;
        cout << "func1:" << endl;
        cout << "  &a = " << &a << endl;
        cout << "  &b = " << &b << " (相差 " 
             << ((char*)&a - (char*)&b) << " 字节)" << endl;
    };
    
    auto func2 = []() {
        int x = 10;
        int y = 20;
        cout << "\nfunc2:" << endl;
        cout << "  &x = " << &x << endl;
        cout << "  &y = " << &y << " (相差 " 
             << ((char*)&x - (char*)&y) << " 字节)" << endl;
    };
    
    func1();  // func1 的栈帧
    func2();  // func2 的栈帧（可能复用 func1 的空间）
    
    cout << "\n观察：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. 同一函数内，变量地址连续" << endl;
    cout << "2. 不同函数的变量可能在相同地址范围" << endl;
    cout << "3. 这说明栈帧被复用了！" << endl;
}

// ============================================
// 7. 类对象的析构函数调用
// ============================================

class Resource {
private:
    int id;
    
public:
    Resource(int i) : id(i) {
        cout << "  [构造] Resource " << id 
             << " (地址: " << this << ")" << endl;
    }
    
    ~Resource() {
        cout << "  [析构] Resource " << id 
             << " (地址: " << this << ")" << endl;
    }
};

void demonstrate_destructor_order() {
    cout << "\n" << string(60, '=') << endl;
    cout << "7. 类对象的析构函数调用顺序" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n栈变量的析构顺序（LIFO，后进先出）：\n" << endl;
    
    cout << "进入作用域 {" << endl;
    {
        Resource r1(1);
        Resource r2(2);
        Resource r3(3);
        
        cout << "\n所有对象已构造，在栈帧中" << endl;
        cout << "离开作用域 }" << endl;
    }  // 析构顺序：r3 → r2 → r1 (LIFO)
    
    cout << "\n析构完成！" << endl;
    
    cout << "\n关键理解：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ 栈变量按构造的逆序析构" << endl;
    cout << "✅ 这是栈的 LIFO 特性决定的" << endl;
    cout << "✅ 编译器自动插入析构函数调用" << endl;
    cout << "✅ 然后栈指针回退，释放栈帧" << endl;
}

// ============================================
// 8. 栈 vs 堆的对比
// ============================================

void stack_vs_heap_lifetime() {
    cout << "\n" << string(60, '=') << endl;
    cout << "8. 栈 vs 堆的生命周期对比" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n栈变量（自动存储期）：" << endl;
    cout << "---------------------------------------" << endl;
    {
        cout << "进入作用域 {" << endl;
        int stackVar = 10;
        cout << "  int stackVar = 10;" << endl;
        cout << "  栈变量在栈帧中" << endl;
        cout << "  地址: " << &stackVar << endl;
        cout << "离开作用域 }" << endl;
    }  // stackVar 自动销毁（栈帧销毁）
    cout << "→ stackVar 已销毁（栈指针回退）\n" << endl;
    
    cout << "堆变量（动态存储期）：" << endl;
    cout << "---------------------------------------" << endl;
    {
        cout << "进入作用域 {" << endl;
        int* heapVar = new int(20);
        cout << "  int* heapVar = new int(20);" << endl;
        cout << "  heapVar 指针在栈帧中: " << &heapVar << endl;
        cout << "  数据在堆上: " << heapVar << endl;
        cout << "离开作用域 }" << endl;
        delete heapVar;  // 必须手动释放
    }  // heapVar 指针销毁，但如果忘记 delete，堆内存泄漏
    cout << "→ heapVar 指针已销毁（栈帧销毁）" << endl;
    cout << "→ 堆数据已释放（手动 delete）\n" << endl;
    
    cout << "对比总结：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "栈变量：随栈帧创建和销毁，完全自动" << endl;
    cout << "堆数据：独立于栈帧，需要手动管理" << endl;
    cout << "指针变量：在栈上，但指向堆数据" << endl;
}

// ============================================
// 9. 可视化演示
// ============================================

void visual_demonstration() {
    cout << "\n" << string(60, '=') << endl;
    cout << "9. 可视化演示：栈帧的动态变化" << endl;
    cout << string(60, '=') << endl;
    
    cout << R"(
时间线：函数调用过程

T1: main() 开始
┌─────────────────┐
│ main 的栈帧      │
│ int x = 1;      │ ← 栈顶
└─────────────────┘

T2: main() 调用 funcA()
┌─────────────────┐
│ main 的栈帧      │
│ int x = 1;      │
├─────────────────┤
│ funcA 的栈帧     │
│ int a = 2;      │ ← 栈顶（向下增长）
└─────────────────┘

T3: funcA() 调用 funcB()
┌─────────────────┐
│ main 的栈帧      │
│ int x = 1;      │
├─────────────────┤
│ funcA 的栈帧     │
│ int a = 2;      │
├─────────────────┤
│ funcB 的栈帧     │
│ int b = 3;      │ ← 栈顶
└─────────────────┘

T4: funcB() 返回
┌─────────────────┐
│ main 的栈帧      │
│ int x = 1;      │
├─────────────────┤
│ funcA 的栈帧     │
│ int a = 2;      │ ← 栈顶（向上回退）
└─────────────────┘
   ↑ funcB 的栈帧已销毁，b 自动释放

T5: funcA() 返回
┌─────────────────┐
│ main 的栈帧      │
│ int x = 1;      │ ← 栈顶
└─────────────────┘
   ↑ funcA 的栈帧已销毁，a 自动释放

T6: main() 返回
[栈为空]
   ↑ main 的栈帧已销毁，x 自动释放

关键理解：
- 每个函数调用创建新栈帧（压栈）
- 函数返回销毁栈帧（出栈）
- 局部变量随栈帧一起创建和销毁
- 这就是"自动存储期"的本质！
)" << endl;
}

// ============================================
// Main
// ============================================

int main() {
    cout << "===========================================" << endl;
    cout << "  函数调用栈和栈帧机制详解" << endl;
    cout << "===========================================" << endl;
    
    explain_stack_frame();
    show_stack_frame_structure();
    demonstrate_call_stack();
    detailed_stack_operations();
    why_auto_destruction();
    observe_addresses();
    demonstrate_destructor_order();
    stack_vs_heap_lifetime();
    visual_demonstration();
    
    cout << "\n===========================================" << endl;
    cout << "核心答案" << endl;
    cout << "===========================================" << endl;
    
    cout << "\n问题：栈上的内存可以自动释放，是否是因为" << endl;
    cout << "      栈变量存储在函数调用栈的栈帧中，" << endl;
    cout << "      随着函数出栈而自动销毁？" << endl;
    
    cout << "\n答案：完全正确！这就是核心原理！" << endl;
    cout << "---------------------------------------" << endl;
    
    cout << "\n详细解释：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. 每个函数调用创建一个栈帧" << endl;
    cout << "   - 栈帧包含：参数、返回地址、局部变量" << endl;
    cout << "   - 栈帧在栈内存中分配" << endl;
    
    cout << "\n2. 局部变量存储在栈帧中" << endl;
    cout << "   - 变量地址 = 栈帧基址 + 偏移量" << endl;
    cout << "   - 随栈帧一起分配" << endl;
    
    cout << "\n3. 函数返回时，栈帧被销毁" << endl;
    cout << "   - 栈指针回退到调用前位置" << endl;
    cout << "   - 栈帧空间被'释放'（可复用）" << endl;
    cout << "   - 局部变量随之销毁" << endl;
    
    cout << "\n4. 这就是'自动存储期'的本质" << endl;
    cout << "   - 不需要程序员手动 delete" << endl;
    cout << "   - 编译器自动管理生命周期" << endl;
    cout << "   - 基于硬件栈指针机制" << endl;
    
    cout << "\n关键优势：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ 极快：只需移动栈指针" << endl;
    cout << "✅ 安全：不会内存泄漏" << endl;
    cout << "✅ 简单：程序员无需管理" << endl;
    cout << "✅ 高效：CPU 缓存友好（连续内存）" << endl;
    
    cout << "\n这就是为什么：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ C++ 推荐优先使用栈（局部变量）" << endl;
    cout << "✅ Java/Kotlin 基本类型也用栈" << endl;
    cout << "✅ 函数式编程偏好栈（不可变值）" << endl;
    cout << "✅ 现代语言都有'自动存储期'概念" << endl;
    
    cout << "\n记忆模型：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "函数调用 = 压入栈帧（push frame）" << endl;
    cout << "  → 局部变量随栈帧创建" << endl;
    cout << "\n函数返回 = 弹出栈帧（pop frame）" << endl;
    cout << "  → 局部变量随栈帧销毁" << endl;
    cout << "\n栈帧 = 函数的'工作空间'" << endl;
    cout << "  → 进入函数时分配" << endl;
    cout << "  → 离开函数时回收" << endl;
    
    cout << "\n记忆口诀：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "函数调用创建帧，局部变量帧中安" << endl;
    cout << "函数返回销毁帧，变量随之自动散" << endl;
    cout << "栈帧机制是关键，自动管理不用管" << endl;
    cout << "理解栈帧懂内存，C++ 精通在眼前" << endl;
    
    cout << "===========================================" << endl;
    
    return 0;
}

/**
 * 补充说明：
 * 
 * 1. 栈帧（Stack Frame）也称为：
 *    - Activation Record（活动记录）
 *    - Call Frame（调用帧）
 * 
 * 2. 栈指针寄存器：
 *    - SP (Stack Pointer)：指向栈顶
 *    - BP/FP (Base/Frame Pointer)：指向当前栈帧基址
 * 
 * 3. 栈的增长方向：
 *    - 多数架构：向低地址增长（向下）
 *    - 少数架构：向高地址增长（向上）
 * 
 * 4. 为什么栈这么快？
 *    - 只需要移动指针（1-2 条指令）
 *    - 内存连续，缓存友好
 *    - 不需要复杂的内存分配算法
 * 
 * 5. 栈的大小限制：
 *    - Linux: 通常 8 MB (ulimit -s 查看)
 *    - Windows: 通常 1 MB
 *    - 可以通过编译器选项调整
 * 
 * 6. 递归与栈：
 *    - 每次递归调用创建新栈帧
 *    - 深度递归可能导致栈溢出
 *    - 尾递归优化可以避免栈增长
 */
