/*
 * C++ vs Java 对象模型对比
 * 主题：C++ 没有统一基类、运算符重载
 * 核心：operator() 重载的是函数调用运算符
 */

#include <iostream>
#include <string>
using namespace std;

// ============================================================================
// 一、C++ vs Java 对象模型
// ============================================================================

void explain_object_model() {
    cout << "\n=== 1. C++ vs Java 对象模型 ===" << endl;
    
    cout << "\nJava 对象模型：" << endl;
    cout << "  ┌─────────────────────────┐" << endl;
    cout << "  │      Object (基类)      │  ← 所有类的根" << endl;
    cout << "  │  - toString()           │" << endl;
    cout << "  │  - equals()             │" << endl;
    cout << "  │  - hashCode()           │" << endl;
    cout << "  │  - getClass()           │" << endl;
    cout << "  └─────────────────────────┘" << endl;
    cout << "            ↑   ↑   ↑" << endl;
    cout << "            │   │   │" << endl;
    cout << "      ┌─────┘   │   └─────┐" << endl;
    cout << "      │         │         │" << endl;
    cout << "   MyClass1  MyClass2  MyClass3" << endl;
    cout << "   (隐式继承 Object)" << endl;
    
    cout << "\n  特点：" << endl;
    cout << "    ✅ 所有类都继承自 Object" << endl;
    cout << "    ✅ 统一的方法：toString(), equals() 等" << endl;
    cout << "    ✅ 多态性：Object obj = new MyClass()" << endl;
    cout << "    ✅ 反射：getClass() 获取类型信息" << endl;
    
    cout << "\n\nC++ 对象模型：" << endl;
    cout << "  MyClass1    MyClass2    MyClass3" << endl;
    cout << "     │           │           │" << endl;
    cout << "  (独立)     (独立)     (独立)" << endl;
    cout << "  没有共同基类！" << endl;
    
    cout << "\n  特点：" << endl;
    cout << "    ❌ 没有统一基类" << endl;
    cout << "    ❌ 没有统一的方法" << endl;
    cout << "    ✅ 更灵活，没有继承开销" << endl;
    cout << "    ✅ 可以选择性继承" << endl;
    
    cout << "\n对比总结：" << endl;
    cout << "  ┌────────────────────┬─────────────────┬─────────────────┐" << endl;
    cout << "  │ 特性               │ Java            │ C++             │" << endl;
    cout << "  ├────────────────────┼─────────────────┼─────────────────┤" << endl;
    cout << "  │ 统一基类           │ ✅ Object       │ ❌ 无           │" << endl;
    cout << "  │ 所有类都是对象     │ ✅ 是           │ ❌ 否           │" << endl;
    cout << "  │ toString()         │ ✅ 有           │ ❌ 无（可自定）│" << endl;
    cout << "  │ equals()           │ ✅ 有           │ ❌ 无（可重载==│" << endl;
    cout << "  │ 多态基类指针       │ Object*         │ 需自定义基类    │" << endl;
    cout << "  │ 性能开销           │ 虚表开销        │ 无额外开销      │" << endl;
    cout << "  └────────────────────┴─────────────────┴─────────────────┘" << endl;
}

// ============================================================================
// 二、C++ 中类的完全独立性
// ============================================================================

class ClassA {
    int value = 10;
public:
    void printA() {
        cout << "    ClassA: value = " << value << endl;
    }
};

class ClassB {
    string name = "ClassB";
public:
    void printB() {
        cout << "    ClassB: name = " << name << endl;
    }
};

void demonstrate_independent_classes() {
    cout << "\n=== 2. C++ 中类的完全独立性 ===" << endl;
    
    cout << "\n示例：两个完全独立的类" << endl;
    ClassA a;
    ClassB b;
    
    a.printA();
    b.printB();
    
    cout << "\n关键点：" << endl;
    cout << "  - ClassA 和 ClassB 没有任何关系" << endl;
    cout << "  - 不能用统一的指针/引用指向它们" << endl;
    cout << "  - 没有共同的方法（如 toString()）" << endl;
    
    // 以下代码在 Java 中可行，但 C++ 中不行：
    // Object obj1 = new ClassA();  // Java: ✅
    // Object obj2 = new ClassB();  // Java: ✅
    // C++: ❌ 没有统一的 Object 类型
    
    cout << "\nJava 等价代码：" << endl;
    cout << "  Object obj1 = new ClassA();  // ✅ 可以" << endl;
    cout << "  Object obj2 = new ClassB();  // ✅ 可以" << endl;
    cout << "  obj1.toString();              // ✅ 所有对象都有" << endl;
    
    cout << "\nC++ 中如果需要类似功能，必须自己设计基类：" << endl;
    cout << "  class Base { virtual void print() = 0; };" << endl;
    cout << "  class ClassA : public Base { ... };" << endl;
    cout << "  class ClassB : public Base { ... };" << endl;
    cout << "  Base* obj1 = new ClassA();   // ✅ 现在可以" << endl;
}

// ============================================================================
// 三、operator() 重载的是什么？
// ============================================================================

void explain_operator_overloading() {
    cout << "\n=== 3. operator() 重载的是什么？===" << endl;
    
    cout << "\n核心概念：" << endl;
    cout << "  operator() 不是从某个基类重载来的" << endl;
    cout << "  而是重载 C++ 的函数调用运算符 ()" << endl;
    
    cout << "\nC++ 可重载的运算符：" << endl;
    cout << "  ┌──────────────┬──────────────┬──────────────┐" << endl;
    cout << "  │ 算术运算符   │ 比较运算符   │ 其他运算符   │" << endl;
    cout << "  ├──────────────┼──────────────┼──────────────┤" << endl;
    cout << "  │ +  -  *  /   │ ==  !=       │ []  (下标)   │" << endl;
    cout << "  │ %  ++  --    │ <   >        │ ()  (函数调用│" << endl;
    cout << "  │ +=  -=  *=   │ <=  >=       │ ->  (成员访问│" << endl;
    cout << "  │              │              │ <<  >> (流)  │" << endl;
    cout << "  └──────────────┴──────────────┴──────────────┘" << endl;
    
    cout << "\noperator() 的特殊性：" << endl;
    cout << "  - () 是函数调用运算符" << endl;
    cout << "  - 重载它让对象可以像函数一样调用" << endl;
    cout << "  - 不是从任何基类继承或重载来的" << endl;
    cout << "  - 是你自己定义的新功能" << endl;
    
    cout << "\n类比：" << endl;
    cout << "  class MyInt {" << endl;
    cout << "      int value;" << endl;
    cout << "  public:" << endl;
    cout << "      // 重载 + 运算符" << endl;
    cout << "      MyInt operator+(const MyInt& other) {" << endl;
    cout << "          return MyInt(value + other.value);" << endl;
    cout << "      }" << endl;
    cout << "      " << endl;
    cout << "      // 重载 () 运算符" << endl;
    cout << "      void operator()() {" << endl;
    cout << "          cout << \"Called!\" << endl;" << endl;
    cout << "      }" << endl;
    cout << "  };" << endl;
    cout << "  " << endl;
    cout << "  MyInt a, b;" << endl;
    cout << "  a + b;   // 使用重载的 +" << endl;
    cout << "  a();     // 使用重载的 ()" << endl;
}

// ============================================================================
// 四、实际演示运算符重载
// ============================================================================

class Point {
    int x, y;
    
public:
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    
    // 重载 + 运算符
    Point operator+(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }
    
    // 重载 == 运算符
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    
    // 重载 << 运算符（友元函数）
    friend ostream& operator<<(ostream& os, const Point& p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }
    
    // 重载 () 运算符（函数调用）
    void operator()() const {
        cout << "    Point called: (" << x << ", " << y << ")" << endl;
    }
    
    // 重载 [] 运算符（下标访问）
    int operator[](int index) const {
        return index == 0 ? x : y;
    }
};

void demonstrate_various_operators() {
    cout << "\n=== 4. 实际演示运算符重载 ===" << endl;
    
    Point p1(1, 2);
    Point p2(3, 4);
    
    cout << "\n重载 + 运算符：" << endl;
    Point p3 = p1 + p2;
    cout << "  p1 + p2 = " << p3 << endl;
    
    cout << "\n重载 == 运算符：" << endl;
    cout << "  p1 == p2? " << (p1 == p2 ? "是" : "否") << endl;
    
    cout << "\n重载 << 运算符：" << endl;
    cout << "  cout << p1 输出: " << p1 << endl;
    
    cout << "\n重载 () 运算符（函数调用）：" << endl;
    cout << "  p1() 调用：" << endl;
    p1();
    
    cout << "\n重载 [] 运算符（下标访问）：" << endl;
    cout << "  p1[0] = " << p1[0] << endl;
    cout << "  p1[1] = " << p1[1] << endl;
    
    cout << "\n关键理解：" << endl;
    cout << "  - 所有这些 operator 都不是从基类继承的" << endl;
    cout << "  - 它们是你自己定义的新功能" << endl;
    cout << "  - 让自定义类型可以使用运算符" << endl;
}

// ============================================================================
// 五、operator() 的多种形式
// ============================================================================

class Calculator {
public:
    // 无参数版本
    void operator()() const {
        cout << "    Calculator: 无参数调用" << endl;
    }
    
    // 一个参数版本
    int operator()(int x) const {
        cout << "    Calculator: 一个参数 " << x << endl;
        return x * 2;
    }
    
    // 两个参数版本
    int operator()(int x, int y) const {
        cout << "    Calculator: 两个参数 " << x << ", " << y << endl;
        return x + y;
    }
    
    // 三个参数版本
    int operator()(int x, int y, int z) const {
        cout << "    Calculator: 三个参数 " << x << ", " << y << ", " << z << endl;
        return x + y + z;
    }
};

void demonstrate_operator_call_overloads() {
    cout << "\n=== 5. operator() 的多种形式 ===" << endl;
    
    cout << "\noperator() 可以有多个重载版本：" << endl;
    Calculator calc;
    
    cout << "\n无参数：" << endl;
    calc();
    
    cout << "\n一个参数：" << endl;
    int result1 = calc(5);
    cout << "  返回值: " << result1 << endl;
    
    cout << "\n两个参数：" << endl;
    int result2 = calc(3, 4);
    cout << "  返回值: " << result2 << endl;
    
    cout << "\n三个参数：" << endl;
    int result3 = calc(1, 2, 3);
    cout << "  返回值: " << result3 << endl;
    
    cout << "\n结论：" << endl;
    cout << "  ✅ operator() 可以有任意数量的参数" << endl;
    cout << "  ✅ operator() 可以有任意返回值类型" << endl;
    cout << "  ✅ operator() 可以有多个重载版本" << endl;
    cout << "  ✅ 完全由你自己定义，不受基类约束" << endl;
}

// ============================================================================
// 六、Java vs C++ 详细对比
// ============================================================================

void detailed_java_cpp_comparison() {
    cout << "\n=== 6. Java vs C++ 详细对比 ===" << endl;
    
    cout << "\n场景1: 统一基类" << endl;
    cout << "  Java:" << endl;
    cout << "    class MyClass { }  // 隐式继承 Object" << endl;
    cout << "    Object obj = new MyClass();  // ✅ 可以" << endl;
    cout << "    obj.toString();  // ✅ 所有对象都有" << endl;
    
    cout << "\n  C++:" << endl;
    cout << "    class MyClass { };  // 不继承任何类" << endl;
    cout << "    // 没有统一的 Object 类型" << endl;
    cout << "    // 需要自己设计基类或使用模板" << endl;
    
    cout << "\n场景2: 多态性" << endl;
    cout << "  Java:" << endl;
    cout << "    Object[] objects = new Object[3];" << endl;
    cout << "    objects[0] = new String(\"hello\");" << endl;
    cout << "    objects[1] = new Integer(42);" << endl;
    cout << "    objects[2] = new MyClass();" << endl;
    
    cout << "\n  C++:" << endl;
    cout << "    // 需要自己设计基类" << endl;
    cout << "    class Base { virtual ~Base() {} };" << endl;
    cout << "    class MyClass : public Base { };" << endl;
    cout << "    Base* objects[3];" << endl;
    cout << "    objects[0] = new MyClass();" << endl;
    
    cout << "\n场景3: 反射" << endl;
    cout << "  Java:" << endl;
    cout << "    Object obj = new MyClass();" << endl;
    cout << "    Class<?> clazz = obj.getClass();  // ✅ 反射" << endl;
    cout << "    String name = clazz.getName();" << endl;
    
    cout << "\n  C++:" << endl;
    cout << "    MyClass obj;" << endl;
    cout << "    // ❌ 没有内置反射" << endl;
    cout << "    // 需要使用 typeid 或 RTTI（功能有限）" << endl;
    cout << "    // 或使用第三方库" << endl;
    
    cout << "\n场景4: 运算符重载" << endl;
    cout << "  Java:" << endl;
    cout << "    // ❌ 不支持运算符重载（除了 String 的 +）" << endl;
    cout << "    // 只能定义方法" << endl;
    cout << "    class MyClass {" << endl;
    cout << "        public void call() { }  // 普通方法" << endl;
    cout << "    }" << endl;
    cout << "    obj.call();  // 只能这样调用" << endl;
    
    cout << "\n  C++:" << endl;
    cout << "    // ✅ 支持运算符重载" << endl;
    cout << "    class MyClass {" << endl;
    cout << "        void operator()() { }  // 重载 ()" << endl;
    cout << "    };" << endl;
    cout << "    MyClass obj;" << endl;
    cout << "    obj();  // 像函数一样调用！" << endl;
    
    cout << "\n总结对比：" << endl;
    cout << "  ┌─────────────────┬─────────────────┬─────────────────┐" << endl;
    cout << "  │ 特性            │ Java            │ C++             │" << endl;
    cout << "  ├─────────────────┼─────────────────┼─────────────────┤" << endl;
    cout << "  │ 统一基类        │ ✅ Object       │ ❌ 无           │" << endl;
    cout << "  │ 反射            │ ✅ 强大         │ ⚠️ 有限(RTTI)  │" << endl;
    cout << "  │ 运算符重载      │ ❌ 不支持       │ ✅ 支持         │" << endl;
    cout << "  │ 多重继承        │ ❌ 不支持       │ ✅ 支持         │" << endl;
    cout << "  │ 性能开销        │ 虚表+GC开销     │ 可选择性开销    │" << endl;
    cout << "  │ 灵活性          │ 受限于Object    │ 完全自由        │" << endl;
    cout << "  └─────────────────┴─────────────────┴─────────────────┘" << endl;
}

// ============================================================================
// 七、核心总结
// ============================================================================

void summary() {
    cout << "\n========================================" << endl;
    cout << "           核心总结" << endl;
    cout << "========================================" << endl;
    
    cout << "\n问题1: C++ 有统一基类吗？" << endl;
    cout << "  答案：❌ 没有！" << endl;
    cout << "  " << endl;
    cout << "  Java: 所有类都继承 Object" << endl;
    cout << "    class MyClass { }  // 隐式继承 Object" << endl;
    cout << "  " << endl;
    cout << "  C++: 类完全独立" << endl;
    cout << "    class MyClass { }  // 不继承任何类" << endl;
    
    cout << "\n问题2: operator() 是从哪重载来的？" << endl;
    cout << "  答案：不是从基类重载来的！" << endl;
    cout << "  " << endl;
    cout << "  operator() 重载的是 C++ 的函数调用运算符 ()" << endl;
    cout << "  就像重载 +、-、* 等运算符一样" << endl;
    cout << "  是你自己定义的新功能，不是继承来的" << endl;
    
    cout << "\n问题3: 重载的是哪个运算符？" << endl;
    cout << "  答案：函数调用运算符 ()" << endl;
    cout << "  " << endl;
    cout << "  class Functor {" << endl;
    cout << "      void operator()() {  // 重载 () 运算符" << endl;
    cout << "          cout << \"called\" << endl;" << endl;
    cout << "      }" << endl;
    cout << "  };" << endl;
    cout << "  " << endl;
    cout << "  Functor f;" << endl;
    cout << "  f();  // 调用重载的 () 运算符" << endl;
    
    cout << "\n核心理解：" << endl;
    cout << "  1. C++ 没有统一基类" << endl;
    cout << "     - 类之间完全独立" << endl;
    cout << "     - 没有共同的方法（如 toString）" << endl;
    cout << "     - 更灵活，但需要自己设计继承体系" << endl;
    
    cout << "\n  2. operator() 不是继承来的" << endl;
    cout << "     - 是重载 C++ 的函数调用运算符" << endl;
    cout << "     - 让对象可以像函数一样调用" << endl;
    cout << "     - 完全由你自己定义" << endl;
    
    cout << "\n  3. C++ 运算符重载" << endl;
    cout << "     - 可以重载 +、-、*、/、()、[]、<< 等" << endl;
    cout << "     - 不是从基类继承的功能" << endl;
    cout << "     - 是 C++ 的语言特性" << endl;
    
    cout << "\n  4. Java vs C++ 设计哲学" << endl;
    cout << "     Java: 统一、简单、受限" << endl;
    cout << "       - 统一基类 Object" << endl;
    cout << "       - 不支持运算符重载" << endl;
    cout << "       - 不支持多重继承" << endl;
    cout << "     " << endl;
    cout << "     C++: 灵活、复杂、自由" << endl;
    cout << "       - 没有统一基类" << endl;
    cout << "       - 支持运算符重载" << endl;
    cout << "       - 支持多重继承" << endl;
    
    cout << "\n类比记忆：" << endl;
    cout << "  Java 的 Object：" << endl;
    cout << "    就像政府统一管理，所有人都有身份证（Object）" << endl;
    cout << "    方便管理，但限制自由" << endl;
    cout << "  " << endl;
    cout << "  C++ 的独立类：" << endl;
    cout << "    就像自由市场，每个公司独立运作" << endl;
    cout << "    完全自由，但需要自己建立规则" << endl;
    
    cout << "\n记忆口诀：" << endl;
    cout << "  Java 有 Object，C++ 无统一" << endl;
    cout << "  operator() 不继承，重载运算符新功能" << endl;
    cout << "  括号运算符特殊，对象当函数来使用" << endl;
    cout << "  C++ 更灵活自由，Java 更统一简单" << endl;
    
    cout << "\n========================================" << endl;
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "\n╔═══════════════════════════════════════════════════╗" << endl;
    cout << "║      C++ vs Java 对象模型对比                     ║" << endl;
    cout << "║   统一基类、运算符重载、operator()               ║" << endl;
    cout << "╚═══════════════════════════════════════════════════╝" << endl;
    
    // 1. C++ vs Java 对象模型
    explain_object_model();
    
    // 2. C++ 中类的完全独立性
    demonstrate_independent_classes();
    
    // 3. operator() 重载的是什么
    explain_operator_overloading();
    
    // 4. 实际演示运算符重载
    demonstrate_various_operators();
    
    // 5. operator() 的多种形式
    demonstrate_operator_call_overloads();
    
    // 6. Java vs C++ 详细对比
    detailed_java_cpp_comparison();
    
    // 7. 总结
    summary();
    
    return 0;
}
