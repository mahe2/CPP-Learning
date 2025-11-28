/*
 * C++ 类型转换完整指南
 * 问题: C++ 中可以强制类型转换吗？怎么转？
 * 核心: C++ 有 5 种类型转换方式，比 Java 更强大也更复杂
 */

#include <iostream>
#include <string>
#include <memory>
using namespace std;

// ============================================================================
// 一、C++ 的 5 种类型转换方式
// ============================================================================

class Base {
public:
    virtual void show() { cout << "Base class" << endl; }
    virtual ~Base() {}
};

class Derived : public Base {
public:
    void show() override { cout << "Derived class" << endl; }
    void derivedOnly() { cout << "Derived specific method" << endl; }
};

void overview_of_casting() {
    cout << "\n=== 1. C++ 的 5 种类型转换方式 ===" << endl;
    
    cout << "\n1️⃣ C 风格强制转换 (不推荐)" << endl;
    cout << "   语法: (目标类型)变量" << endl;
    cout << "   示例: int x = (int)3.14;" << endl;
    cout << "   缺点: 不安全，不明确转换意图" << endl;
    
    cout << "\n2️⃣ static_cast - 编译时类型转换 (最常用)" << endl;
    cout << "   语法: static_cast<目标类型>(变量)" << endl;
    cout << "   用途: 基本类型转换、类层次向上转换" << endl;
    cout << "   示例: int x = static_cast<int>(3.14);" << endl;
    
    cout << "\n3️⃣ dynamic_cast - 运行时类型转换 (多态专用)" << endl;
    cout << "   语法: dynamic_cast<目标类型>(变量)" << endl;
    cout << "   用途: 安全的向下转换（父类→子类）" << endl;
    cout << "   特点: 需要虚函数，运行时检查" << endl;
    
    cout << "\n4️⃣ const_cast - 移除/添加 const 属性" << endl;
    cout << "   语法: const_cast<目标类型>(变量)" << endl;
    cout << "   用途: 去除 const 修饰符" << endl;
    cout << "   危险: 修改 const 对象是未定义行为" << endl;
    
    cout << "\n5️⃣ reinterpret_cast - 重新解释内存 (最危险)" << endl;
    cout << "   语法: reinterpret_cast<目标类型>(变量)" << endl;
    cout << "   用途: 指针类型强制转换" << endl;
    cout << "   危险: 跳过类型检查，完全由程序员负责" << endl;
    
    cout << "\n推荐优先级: static_cast > dynamic_cast > const_cast > reinterpret_cast" << endl;
}

// ============================================================================
// 二、static_cast - 静态转换（最常用）
// ============================================================================

void demonstrate_static_cast() {
    cout << "\n=== 2. static_cast - 静态转换（最常用）===" << endl;
    
    // 1. 基本类型转换
    cout << "\n用途1: 基本类型之间转换" << endl;
    double pi = 3.14159;
    int intPi = static_cast<int>(pi);  // double → int
    int value = static_cast<int>(pi);
    cout << "  double pi = " << pi << endl;
    cout << "  int intPi = static_cast<int>(pi) = " << intPi << endl;
    
    float f = 2.5f;
    int i = static_cast<int>(f);
    cout << "  float 2.5 → int = " << i << endl;
    
    // 2. 隐式转换显式化
    cout << "\n用途2: 让隐式转换更明确" << endl;
    int a = 10;
    int b = 3;
    double result = static_cast<double>(a) / b;  // 明确转换，避免整除
    cout << "  " << a << " / " << b << " = " << result << endl;
    cout << "  不加 static_cast: " << (a / b) << " (整除)" << endl;
    
    // 3. 指针类型转换（类层次向上）
    cout << "\n用途3: 类层次向上转换（子类→父类，安全）" << endl;
    Derived* derived = new Derived();
    Base* base = static_cast<Base*>(derived);  // 向上转换，安全
    base->show();
    
    cout << "  ✅ 向上转换（子类→父类）总是安全的" << endl;
    cout << "  ⚠️ 向下转换（父类→子类）用 static_cast 不安全！" << endl;
    
    delete derived;
    
    // 4. void* 转换
    cout << "\n用途4: void* 指针转换" << endl;
    int value = 42;
    void* voidPtr = &value;
    int* intPtr = static_cast<int*>(voidPtr);
    cout << "  void* → int* = " << *intPtr << endl;
    
    cout << "\n✅ static_cast 特点:" << endl;
    cout << "  - 编译时检查类型" << endl;
    cout << "  - 性能无损耗" << endl;
    cout << "  - 最常用的转换方式" << endl;
}

// ============================================================================
// 三、dynamic_cast - 动态转换（运行时检查）
// ============================================================================

void demonstrate_dynamic_cast() {
    cout << "\n=== 3. dynamic_cast - 动态转换（运行时检查）===" << endl;
    
    cout << "\n核心用途: 安全的向下转换（父类→子类）" << endl;
    
    // 场景1: 转换成功
    cout << "\n场景1: 转换成功的情况" << endl;
    Base* basePtr = new Derived();  // 实际指向 Derived 对象
    Derived* derivedPtr = dynamic_cast<Derived*>(basePtr);
    
    if (derivedPtr != nullptr) {
        cout << "  ✅ 转换成功！basePtr 确实指向 Derived 对象" << endl;
        derivedPtr->show();
        derivedPtr->derivedOnly();
    } else {
        cout << "  ❌ 转换失败" << endl;
    }
    delete basePtr;
    
    // 场景2: 转换失败
    cout << "\n场景2: 转换失败的情况" << endl;
    Base* basePtr2 = new Base();  // 实际指向 Base 对象
    Derived* derivedPtr2 = dynamic_cast<Derived*>(basePtr2);
    
    if (derivedPtr2 != nullptr) {
        cout << "  ✅ 转换成功" << endl;
    } else {
        cout << "  ❌ 转换失败！basePtr2 指向的不是 Derived 对象" << endl;
        cout << "  dynamic_cast 返回 nullptr，避免了野指针" << endl;
    }
    delete basePtr2;
    
    // 引用转换（失败会抛异常）
    cout << "\n场景3: 引用转换（失败抛异常）" << endl;
    Derived derivedObj;
    Base& baseRef = derivedObj;
    try {
        Derived& derivedRef = dynamic_cast<Derived&>(baseRef);
        cout << "  ✅ 引用转换成功" << endl;
    } catch (bad_cast& e) {
        cout << "  ❌ 引用转换失败，抛出异常: " << e.what() << endl;
    }
    
    cout << "\n✅ dynamic_cast 特点:" << endl;
    cout << "  - 运行时类型检查（RTTI）" << endl;
    cout << "  - 需要虚函数表（virtual）" << endl;
    cout << "  - 失败返回 nullptr (指针) 或抛异常 (引用)" << endl;
    cout << "  - 有性能开销" << endl;
    
    cout << "\n⚠️ 使用条件:" << endl;
    cout << "  - 类必须有虚函数（多态类）" << endl;
    cout << "  - 编译时开启 RTTI（默认开启）" << endl;
}

// ============================================================================
// 四、const_cast - const 属性转换
// ============================================================================

void modify_const_int(const int* ptr) {
    // 去除 const，允许修改
    int* modifiable = const_cast<int*>(ptr);
    *modifiable = 100;
    cout << "  修改后的值: " << *modifiable << endl;
}

void demonstrate_const_cast() {
    cout << "\n=== 4. const_cast - const 属性转换 ===" << endl;
    
    cout << "\n用途1: 去除 const 修饰符" << endl;
    const int x = 42;
    const int* constPtr = &x;
    
    // int* ptr = constPtr;  // ❌ 编译错误：不能将 const int* 赋给 int*
    int* ptr = const_cast<int*>(constPtr);  // ✅ 去除 const
    cout << "  const_cast 成功去除 const: " << *ptr << endl;
    
    cout << "\n⚠️ 危险操作: 修改原本是 const 的对象" << endl;
    cout << "  如果原对象定义为 const，修改它是未定义行为！" << endl;
    
    // 安全的用法：原对象不是 const
    cout << "\n安全用法: 原对象不是 const" << endl;
    int y = 50;
    const int* constY = &y;  // 指针是 const，但对象不是
    modify_const_int(constY);
    cout << "  原对象 y 的值: " << y << endl;
    
    cout << "\n用途2: 添加 const (少见)" << endl;
    int z = 60;
    int* normalPtr = &z;
    const int* constZ = const_cast<const int*>(normalPtr);
    cout << "  添加 const 后: " << *constZ << endl;
    
    cout << "\n✅ const_cast 特点:" << endl;
    cout << "  - 只能改变 const/volatile 属性" << endl;
    cout << "  - 不能改变类型本身" << endl;
    cout << "  - 谨慎使用，易造成未定义行为" << endl;
}

// ============================================================================
// 五、reinterpret_cast - 重新解释转换（最危险）
// ============================================================================

void demonstrate_reinterpret_cast() {
    cout << "\n=== 5. reinterpret_cast - 重新解释转换（最危险）===" << endl;
    
    cout << "\n用途1: 指针类型强制转换" << endl;
    int num = 0x12345678;
    int* intPtr = &num;
    
    // 将 int* 重新解释为 char*
    char* charPtr = reinterpret_cast<char*>(intPtr);
    cout << "  int 值: 0x" << hex << num << dec << endl;
    cout << "  作为字节读取: ";
    for (int i = 0; i < sizeof(int); i++) {
        cout << "0x" << hex << (int)(unsigned char)charPtr[i] << " ";
    }
    cout << dec << endl;
    
    cout << "\n用途2: 指针和整数之间转换" << endl;
    int value = 100;
    int* ptr = &value;
    
    // 指针转整数
    uintptr_t address = reinterpret_cast<uintptr_t>(ptr);
    cout << "  指针地址: 0x" << hex << address << dec << endl;
    
    // 整数转指针（危险！）
    int* ptr2 = reinterpret_cast<int*>(address);
    cout << "  还原后的值: " << *ptr2 << endl;
    
    cout << "\n用途3: 不相关类型指针转换（极度危险）" << endl;
    double d = 3.14;
    double* dPtr = &d;
    
    // 将 double* 解释为 long long*
    long long* llPtr = reinterpret_cast<long long*>(dPtr);
    cout << "  double 值: " << d << endl;
    cout << "  作为 long long 读取: " << *llPtr << " (内存的原始字节)" << endl;
    
    cout << "\n⚠️ reinterpret_cast 危险性:" << endl;
    cout << "  - 完全跳过类型检查" << endl;
    cout << "  - 只是重新解释内存，不做转换" << endl;
    cout << "  - 容易造成未定义行为" << endl;
    cout << "  - 仅用于底层编程（硬件接口、序列化等）" << endl;
    
    cout << "\n✅ 使用场景:" << endl;
    cout << "  - 硬件寄存器访问" << endl;
    cout << "  - 内存映射 I/O" << endl;
    cout << "  - 序列化/反序列化" << endl;
    cout << "  - 与 C 代码交互" << endl;
}

// ============================================================================
// 六、C 风格转换（不推荐）
// ============================================================================

void demonstrate_c_style_cast() {
    cout << "\n=== 6. C 风格转换（不推荐）===" << endl;
    
    cout << "\n语法: (目标类型)变量" << endl;
    
    // C 风格转换会尝试多种转换方式
    double d = 3.14;
    int i = (int)d;  // 等价于 static_cast<int>(d)
    cout << "  (int)3.14 = " << i << endl;
    
    Derived* derived = new Derived();
    Base* base = (Base*)derived;  // 等价于 static_cast
    
    const int x = 10;
    int* ptr = (int*)&x;  // 等价于 const_cast
    
    cout << "\n❌ C 风格转换的问题:" << endl;
    cout << "  1. 不明确转换意图（是 static_cast 还是 const_cast？）" << endl;
    cout << "  2. 难以搜索和审查代码" << endl;
    cout << "  3. 会尝试多种转换，可能产生意外结果" << endl;
    cout << "  4. 不符合现代 C++ 风格" << endl;
    
    cout << "\n✅ 应该用 C++ 风格转换:" << endl;
    cout << "  - 明确转换意图" << endl;
    cout << "  - 容易搜索（搜索 *_cast）" << endl;
    cout << "  - 编译器检查更严格" << endl;
    
    delete derived;
}

// ============================================================================
// 七、隐式转换 vs 显式转换
// ============================================================================

void demonstrate_implicit_vs_explicit() {
    cout << "\n=== 7. 隐式转换 vs 显式转换 ===" << endl;
    
    cout << "\n隐式转换（编译器自动）:" << endl;
    int a = 10;
    double b = a;  // 隐式转换 int → double，无损
    cout << "  int a = 10;" << endl;
    cout << "  double b = a;  // 隐式转换，b = " << b << endl;
    
    double c = 3.14;
    // int d = c;  // 警告：隐式转换可能丢失数据
    cout << "  double c = 3.14;" << endl;
    cout << "  int d = c;  // 隐式转换，可能有警告" << endl;
    
    cout << "\n显式转换（程序员明确指定）:" << endl;
    double e = 3.14;
    int f = static_cast<int>(e);  // 显式转换，明确意图
    cout << "  int f = static_cast<int>(3.14);  // 显式转换，f = " << f << endl;
    
    cout << "\n建议:" << endl;
    cout << "  ✅ 可能丢失数据的转换，用显式转换" << endl;
    cout << "  ✅ 让代码意图更明确" << endl;
    cout << "  ✅ 避免隐式转换带来的隐患" << endl;
}

// ============================================================================
// 八、与 Java 对比
// ============================================================================

void compare_with_java() {
    cout << "\n=== 8. 与 Java 类型转换对比 ===" << endl;
    
    cout << "\nJava 的类型转换:" << endl;
    cout << "  1. 基本类型转换" << endl;
    cout << "     int i = (int)3.14;  // C 风格" << endl;
    
    cout << "\n  2. 引用类型转换" << endl;
    cout << "     Object obj = \"hello\";" << endl;
    cout << "     String s = (String)obj;  // 向下转换" << endl;
    cout << "     运行时检查，失败抛 ClassCastException" << endl;
    
    cout << "\nC++ 的优势:" << endl;
    cout << "  ✅ 有 4 种专用转换操作符，意图明确" << endl;
    cout << "  ✅ static_cast: 编译时检查，无运行时开销" << endl;
    cout << "  ✅ dynamic_cast: 失败返回 nullptr，不抛异常（指针）" << endl;
    cout << "  ✅ const_cast: 可以操作 const 属性" << endl;
    cout << "  ✅ reinterpret_cast: 底层内存操作" << endl;
    
    cout << "\nJava 的优势:" << endl;
    cout << "  ✅ 简单，只有一种转换语法" << endl;
    cout << "  ✅ 运行时总是检查类型安全" << endl;
    cout << "  ✅ 不能做 reinterpret_cast 那样危险的操作" << endl;
    
    cout << "\n核心区别:" << endl;
    cout << "  Java: 简单安全，但不够灵活" << endl;
    cout << "  C++: 灵活强大，但需要程序员负责" << endl;
}

// ============================================================================
// 九、最佳实践
// ============================================================================

void best_practices() {
    cout << "\n=== 9. 类型转换最佳实践 ===" << endl;
    
    cout << "\n优先级（从高到低）:" << endl;
    cout << "  1️⃣ 避免转换 - 设计时就用正确的类型" << endl;
    cout << "  2️⃣ 隐式转换 - 如果编译器允许且安全" << endl;
    cout << "  3️⃣ static_cast - 日常转换首选" << endl;
    cout << "  4️⃣ dynamic_cast - 多态类型的安全向下转换" << endl;
    cout << "  5️⃣ const_cast - 去除 const（非常谨慎）" << endl;
    cout << "  6️⃣ reinterpret_cast - 仅底层编程使用" << endl;
    cout << "  ❌ C 风格转换 - 避免使用" << endl;
    
    cout << "\n选择指南:" << endl;
    cout << "\n基本类型转换:" << endl;
    cout << "  ✅ 用 static_cast<int>(3.14)" << endl;
    cout << "  ❌ 不用 (int)3.14" << endl;
    
    cout << "\n类层次转换:" << endl;
    cout << "  向上转换（子类→父类）:" << endl;
    cout << "    ✅ 隐式转换（自动安全）" << endl;
    cout << "    ✅ static_cast（显式明确）" << endl;
    
    cout << "\n  向下转换（父类→子类）:" << endl;
    cout << "    ✅ dynamic_cast（有虚函数时）" << endl;
    cout << "    ⚠️ static_cast（确定类型时）" << endl;
    
    cout << "\n去除 const:" << endl;
    cout << "  ⚠️ const_cast（确保原对象不是 const）" << endl;
    cout << "  ❌ 避免修改真正的 const 对象" << endl;
    
    cout << "\n底层操作:" << endl;
    cout << "  ⚠️ reinterpret_cast（完全理解后果）" << endl;
    cout << "  📌 仅用于硬件接口、序列化等场景" << endl;
}

// ============================================================================
// 十、常见错误示例
// ============================================================================

void common_mistakes() {
    cout << "\n=== 10. 常见错误示例 ===" << endl;
    
    cout << "\n❌ 错误1: 向下转换不检查类型" << endl;
    cout << "  Base* base = new Base();" << endl;
    cout << "  Derived* derived = static_cast<Derived*>(base);  // 危险！" << endl;
    cout << "  derived->derivedOnly();  // 未定义行为" << endl;
    
    cout << "\n✅ 正确做法:" << endl;
    cout << "  Base* base = new Base();" << endl;
    cout << "  Derived* derived = dynamic_cast<Derived*>(base);" << endl;
    cout << "  if (derived) { derived->derivedOnly(); }" << endl;
    
    cout << "\n❌ 错误2: 修改真正的 const 对象" << endl;
    cout << "  const int x = 10;" << endl;
    cout << "  int* p = const_cast<int*>(&x);" << endl;
    cout << "  *p = 20;  // 未定义行为！x 定义为 const" << endl;
    
    cout << "\n✅ 正确做法:" << endl;
    cout << "  int x = 10;  // x 不是 const" << endl;
    cout << "  const int* p1 = &x;" << endl;
    cout << "  int* p2 = const_cast<int*>(p1);" << endl;
    cout << "  *p2 = 20;  // OK，x 本身不是 const" << endl;
    
    cout << "\n❌ 错误3: 滥用 reinterpret_cast" << endl;
    cout << "  int x = 10;" << endl;
    cout << "  double* d = reinterpret_cast<double*>(&x);  // 危险！" << endl;
    cout << "  cout << *d;  // 未定义行为，类型大小不同" << endl;
}

// ============================================================================
// 十一、核心总结
// ============================================================================

void summary() {
    cout << "\n========================================" << endl;
    cout << "           核心总结" << endl;
    cout << "========================================" << endl;
    
    cout << "\n问题: C++ 中可以强制类型转换吗？怎么转？" << endl;
    cout << "答案: ✅ 可以！C++ 有 5 种转换方式" << endl;
    
    cout << "\n转换方式对比表:" << endl;
    cout << "┌─────────────────────┬────────────────┬──────────┬────────────┐" << endl;
    cout << "│ 转换方式            │ 用途           │ 安全性   │ 性能开销   │" << endl;
    cout << "├─────────────────────┼────────────────┼──────────┼────────────┤" << endl;
    cout << "│ static_cast         │ 常规转换       │ 编译检查 │ 无         │" << endl;
    cout << "│ dynamic_cast        │ 多态向下转换   │ 运行检查 │ 有         │" << endl;
    cout << "│ const_cast          │ const 属性     │ 危险     │ 无         │" << endl;
    cout << "│ reinterpret_cast    │ 底层内存操作   │ 极危险   │ 无         │" << endl;
    cout << "│ C 风格 (type)value  │ 兼容 C         │ 不推荐   │ 无         │" << endl;
    cout << "└─────────────────────┴────────────────┴──────────┴────────────┘" << endl;
    
    cout << "\n推荐使用顺序:" << endl;
    cout << "  1️⃣ static_cast      - 首选，90% 的情况" << endl;
    cout << "  2️⃣ dynamic_cast     - 多态类型转换" << endl;
    cout << "  3️⃣ const_cast       - 去除 const（谨慎）" << endl;
    cout << "  4️⃣ reinterpret_cast - 底层编程（极少）" << endl;
    cout << "  ❌ C 风格转换       - 避免使用" << endl;
    
    cout << "\n快速选择指南:" << endl;
    cout << "  • 基本类型转换 → static_cast" << endl;
    cout << "  • 向上转换（子→父）→ 隐式转换或 static_cast" << endl;
    cout << "  • 向下转换（父→子）→ dynamic_cast" << endl;
    cout << "  • 去除 const → const_cast" << endl;
    cout << "  • 指针类型强转 → reinterpret_cast（危险）" << endl;
    
    cout << "\n与 Java 对比:" << endl;
    cout << "  Java: (Type)value - 简单统一，运行时检查" << endl;
    cout << "  C++:  4 种专用转换 - 灵活强大，意图明确" << endl;
    
    cout << "\n记忆口诀:" << endl;
    cout << "  static 最常用，编译时就检查" << endl;
    cout << "  dynamic 保安全，运行时才知道" << endl;
    cout << "  const 去修饰，谨慎别乱搞" << endl;
    cout << "  reinterpret 危险，底层才需要" << endl;
    cout << "  C 风格转换，现代不推荐" << endl;
    
    cout << "\n========================================" << endl;
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "\n╔═══════════════════════════════════════════════════╗" << endl;
    cout << "║   C++ 类型转换完整指南                            ║" << endl;
    cout << "║   C++ 中可以强制类型转换吗？怎么转？              ║" << endl;
    cout << "╚═══════════════════════════════════════════════════╝" << endl;
    
    // 1. 概览
    overview_of_casting();
    
    // 2. static_cast
    demonstrate_static_cast();
    
    // 3. dynamic_cast
    demonstrate_dynamic_cast();
    
    // 4. const_cast
    demonstrate_const_cast();
    
    // 5. reinterpret_cast
    demonstrate_reinterpret_cast();
    
    // 6. C 风格转换
    demonstrate_c_style_cast();
    
    // 7. 隐式 vs 显式
    demonstrate_implicit_vs_explicit();
    
    // 8. 与 Java 对比
    compare_with_java();
    
    // 9. 最佳实践
    best_practices();
    
    // 10. 常见错误
    common_mistakes();
    
    // 11. 总结
    summary();
    
    return 0;
}
