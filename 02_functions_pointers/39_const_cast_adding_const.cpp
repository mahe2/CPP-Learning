/*
 * const_cast 详解：如何添加和移除 const
 * 问题: const_cast 如何添加 const 属性？
 * 核心: const_cast 可以双向操作，但添加 const 很少用（可以隐式转换）
 */

#include <iostream>
using namespace std;

// ============================================================================
// 一、const_cast 的两个方向
// ============================================================================

void demonstrate_both_directions() {
    cout << "\n=== 1. const_cast 的两个方向 ===" << endl;
    
    cout << "\n方向1: 移除 const（常用）" << endl;
    const int x = 42;
    const int* constPtr = &x;
    
    // 移除 const
    int* normalPtr = const_cast<int*>(constPtr);
    cout << "  const int* → int*" << endl;
    cout << "  移除 const 后: " << *normalPtr << endl;
    
    cout << "\n方向2: 添加 const（少见）" << endl;
    int y = 100;
    int* ptr = &y;
    
    // 添加 const
    const int* constPtr2 = const_cast<const int*>(ptr);
    cout << "  int* → const int*" << endl;
    cout << "  添加 const 后: " << *constPtr2 << endl;
    
    cout << "\n为什么添加 const 少见？" << endl;
    cout << "  因为可以直接隐式转换（不需要 const_cast）:" << endl;
    const int* constPtr3 = ptr;  // 隐式转换，不需要 const_cast
    cout << "  const int* constPtr3 = ptr;  // ✅ 隐式转换即可" << endl;
}

// ============================================================================
// 二、添加 const 的三种方式
// ============================================================================

void demonstrate_adding_const() {
    cout << "\n=== 2. 添加 const 的三种方式 ===" << endl;
    
    int value = 50;
    int* ptr = &value;
    
    cout << "\n方式1: 隐式转换（推荐）" << endl;
    const int* constPtr1 = ptr;  // 最简单，自动转换
    cout << "  const int* constPtr1 = ptr;" << endl;
    cout << "  值: " << *constPtr1 << endl;
    
    cout << "\n方式2: const_cast（不必要，但可以）" << endl;
    const int* constPtr2 = const_cast<const int*>(ptr);
    cout << "  const int* constPtr2 = const_cast<const int*>(ptr);" << endl;
    cout << "  值: " << *constPtr2 << endl;
    
    cout << "\n方式3: static_cast（也可以）" << endl;
    const int* constPtr3 = static_cast<const int*>(ptr);
    cout << "  const int* constPtr3 = static_cast<const int*>(ptr);" << endl;
    cout << "  值: " << *constPtr3 << endl;
    
    cout << "\n结论: 添加 const 时，直接用隐式转换即可！" << endl;
}

// ============================================================================
// 三、添加 const 的实际用途
// ============================================================================

// 示例：函数重载时可能需要
void process(int* ptr) {
    cout << "  处理非 const 指针: " << *ptr << endl;
}

void process(const int* ptr) {
    cout << "  处理 const 指针: " << *ptr << endl;
}

void demonstrate_use_cases() {
    cout << "\n=== 3. 添加 const 的实际用途 ===" << endl;
    
    cout << "\n用途1: 函数重载选择" << endl;
    int x = 10;
    int* ptr = &x;
    
    process(ptr);  // 调用非 const 版本
    
    // 强制调用 const 版本
    process(const_cast<const int*>(ptr));
    
    cout << "\n用途2: 模板编程（保持 const 正确性）" << endl;
    cout << "  在泛型代码中，可能需要添加 const 来匹配类型" << endl;
    
    cout << "\n用途3: 转换为更严格的类型" << endl;
    int arr[] = {1, 2, 3};
    int* p = arr;
    
    // 传递给只接受 const 指针的函数时
    // const int* cp = const_cast<const int*>(p);  // 可以，但不必要
    const int* cp = p;  // 隐式转换更好
    cout << "  数组首元素: " << *cp << endl;
}

// ============================================================================
// 四、const_cast 添加 const 的完整示例
// ============================================================================

void demonstrate_complete_examples() {
    cout << "\n=== 4. 完整示例 ===" << endl;
    
    // 示例1: 指针
    cout << "\n示例1: 指针添加 const" << endl;
    int a = 10;
    int* pa = &a;
    const int* cpa = const_cast<const int*>(pa);  // 添加 const
    cout << "  int* → const int*: " << *cpa << endl;
    // *cpa = 20;  // ❌ 编译错误：不能通过 const 指针修改
    *pa = 20;      // ✅ 可以通过原指针修改
    cout << "  修改后: " << *cpa << endl;
    
    // 示例2: 引用
    cout << "\n示例2: 引用添加 const" << endl;
    int b = 30;
    int& rb = b;
    const int& crb = const_cast<const int&>(rb);  // 添加 const
    cout << "  int& → const int&: " << crb << endl;
    // crb = 40;   // ❌ 编译错误
    rb = 40;       // ✅ 可以通过原引用修改
    cout << "  修改后: " << crb << endl;
    
    // 示例3: 多级指针
    cout << "\n示例3: 多级指针添加 const" << endl;
    int c = 50;
    int* pc = &c;
    int** ppc = &pc;
    
    // 添加 const 到第一级
    const int** cppc = const_cast<const int**>(ppc);
    cout << "  int** → const int**: " << **cppc << endl;
    
    // 注意：const 位置不同，意义不同
    cout << "\n  const 的位置:" << endl;
    cout << "  const int* p;      // 指向常量的指针" << endl;
    cout << "  int* const p;      // 常量指针" << endl;
    cout << "  const int* const p;// 指向常量的常量指针" << endl;
}

// ============================================================================
// 五、为什么添加 const 通常不用 const_cast
// ============================================================================

void why_not_use_const_cast_for_adding() {
    cout << "\n=== 5. 为什么添加 const 通常不用 const_cast ===" << endl;
    
    int x = 100;
    int* ptr = &x;
    
    cout << "\n原因1: 隐式转换更简洁" << endl;
    // 不推荐
    const int* cp1 = const_cast<const int*>(ptr);
    cout << "  const_cast<const int*>(ptr)  // 太啰嗦" << endl;
    
    // 推荐
    const int* cp2 = ptr;
    cout << "  ptr                          // 简洁明了 ✅" << endl;
    
    cout << "\n原因2: 添加 const 是安全的，编译器自动支持" << endl;
    cout << "  非 const → const: 限制更多，更安全（隐式转换）" << endl;
    cout << "  const → 非 const: 限制更少，危险（需要显式转换）" << endl;
    
    cout << "\n原因3: const_cast 主要用于移除 const" << endl;
    cout << "  const_cast 的设计初衷是处理遗留代码中的 const 问题" << endl;
    cout << "  添加 const 不是它的主要用途" << endl;
    
    cout << "\n类比 Java:" << endl;
    cout << "  子类 → 父类: 隐式转换（向上转型）" << endl;
    cout << "  父类 → 子类: 显式转换（向下转型）" << endl;
    cout << "  非 const → const: 隐式转换（添加限制）" << endl;
    cout << "  const → 非 const: 显式转换（移除限制）" << endl;
}

// ============================================================================
// 六、const_cast 的典型错误
// ============================================================================

void common_mistakes() {
    cout << "\n=== 6. const_cast 的典型错误 ===" << endl;
    
    cout << "\n❌ 错误1: 修改真正的 const 对象" << endl;
    cout << "  const int x = 10;" << endl;
    cout << "  int* p = const_cast<int*>(&x);" << endl;
    cout << "  *p = 20;  // 未定义行为！x 定义为 const" << endl;
    
    cout << "\n✅ 正确做法: 只在对象本身不是 const 时移除 const" << endl;
    int y = 30;
    const int* cy = &y;
    int* py = const_cast<int*>(cy);
    *py = 40;  // OK，y 本身不是 const
    cout << "  y 的新值: " << y << endl;
    
    cout << "\n❌ 错误2: 用 const_cast 添加 const（多此一举）" << endl;
    int z = 50;
    int* pz = &z;
    const int* cpz = const_cast<const int*>(pz);  // 不必要
    cout << "  不如直接: const int* cpz = pz;" << endl;
    
    cout << "\n❌ 错误3: 试图改变类型" << endl;
    cout << "  int* p = ...;" << endl;
    cout << "  double* d = const_cast<double*>(p);  // ❌ 编译错误" << endl;
    cout << "  const_cast 只能改变 const 属性，不能改变类型！" << endl;
}

// ============================================================================
// 七、实用技巧
// ============================================================================

// 示例：const 成员函数中需要修改成员变量
class Counter {
private:
    mutable int accessCount;  // mutable: 允许在 const 函数中修改
    int value;
    
public:
    Counter(int v) : accessCount(0), value(v) {}
    
    // const 成员函数
    int getValue() const {
        accessCount++;  // OK，accessCount 是 mutable
        return value;
    }
    
    // 如果没有 mutable，可以用 const_cast（不推荐）
    int getValueOld() const {
        // 移除 this 的 const
        Counter* self = const_cast<Counter*>(this);
        self->accessCount++;  // 不推荐，应该用 mutable
        return value;
    }
    
    int getAccessCount() const { return accessCount; }
};

void practical_tips() {
    cout << "\n=== 7. 实用技巧 ===" << endl;
    
    cout << "\n技巧1: 优先使用 mutable，而非 const_cast" << endl;
    const Counter counter(100);
    cout << "  值: " << counter.getValue() << endl;
    cout << "  访问次数: " << counter.getAccessCount() << endl;
    cout << "  再次访问: " << counter.getValue() << endl;
    cout << "  访问次数: " << counter.getAccessCount() << endl;
    
    cout << "\n技巧2: 添加 const 用隐式转换" << endl;
    int x = 50;
    int* px = &x;
    const int* cpx = px;  // 隐式转换，清晰简洁
    cout << "  const int* cpx = px;  // ✅" << endl;
    
    cout << "\n技巧3: 移除 const 要谨慎" << endl;
    cout << "  确保原对象不是 const" << endl;
    cout << "  只在必要时使用（如调用遗留代码）" << endl;
}

// ============================================================================
// 八、核心总结
// ============================================================================

void summary() {
    cout << "\n========================================" << endl;
    cout << "           核心总结" << endl;
    cout << "========================================" << endl;
    
    cout << "\n问题: const_cast 如何添加 const？" << endl;
    cout << "答案: const_cast<const int*>(ptr)" << endl;
    
    cout << "\n语法对比:" << endl;
    cout << "  移除 const: int* p = const_cast<int*>(constPtr);" << endl;
    cout << "  添加 const: const int* p = const_cast<const int*>(ptr);" << endl;
    
    cout << "\n但是！添加 const 通常不用 const_cast:" << endl;
    cout << "  ❌ const int* p = const_cast<const int*>(ptr);  // 啰嗦" << endl;
    cout << "  ✅ const int* p = ptr;                         // 简洁" << endl;
    
    cout << "\n为什么？" << endl;
    cout << "  1. 添加 const 是安全的，编译器支持隐式转换" << endl;
    cout << "  2. 移除 const 是危险的，必须显式转换" << endl;
    cout << "  3. const_cast 主要用于移除 const，不是添加" << endl;
    
    cout << "\nconst_cast 的真正用途:" << endl;
    cout << "  ✅ 移除 const（处理遗留代码）" << endl;
    cout << "  ✅ 在确保安全的情况下修改 const 对象" << endl;
    cout << "  ❌ 添加 const（用隐式转换更好）" << endl;
    
    cout << "\n记忆要点:" << endl;
    cout << "  • 非 const → const: 隐式转换（自动，安全）" << endl;
    cout << "  • const → 非 const: const_cast（手动，危险）" << endl;
    cout << "  • 只能改变 const 属性，不能改变类型" << endl;
    cout << "  • 修改真正的 const 对象是未定义行为" << endl;
    
    cout << "\n最佳实践:" << endl;
    cout << "  1️⃣ 添加 const: 直接赋值（隐式转换）" << endl;
    cout << "  2️⃣ 移除 const: 用 const_cast（谨慎）" << endl;
    cout << "  3️⃣ 优先使用 mutable，而非 const_cast" << endl;
    cout << "  4️⃣ 确保原对象不是 const 再移除 const" << endl;
    
    cout << "\n========================================" << endl;
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "\n╔══════════════════════════════════════════════════╗" << endl;
    cout << "║   const_cast 详解：添加和移除 const              ║" << endl;
    cout << "║   重点：如何添加 const 属性？                    ║" << endl;
    cout << "╚══════════════════════════════════════════════════╝" << endl;
    
    // 1. 两个方向
    demonstrate_both_directions();
    
    // 2. 添加 const 的方式
    demonstrate_adding_const();
    
    // 3. 实际用途
    demonstrate_use_cases();
    
    // 4. 完整示例
    demonstrate_complete_examples();
    
    // 5. 为什么不用 const_cast 添加
    why_not_use_const_cast_for_adding();
    
    // 6. 常见错误
    common_mistakes();
    
    // 7. 实用技巧
    practical_tips();
    
    // 8. 总结
    summary();
    
    return 0;
}
