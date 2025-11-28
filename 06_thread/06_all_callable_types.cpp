/*
 * C++ 线程支持的所有可调用类型
 * 主题：6种可调用对象（Callable）
 * 核心：普通函数、成员函数、静态成员函数、函数对象、Lambda、std::function
 */

#include <iostream>
#include <thread>
#include <functional>
using namespace std;

// ============================================================================
// 一、普通函数（Free Function）
// ============================================================================

void normal_function() {
    cout << "  [线程 " << this_thread::get_id() 
         << "] 1. 普通函数" << endl;
}

void normal_function_with_params(int a, string b) {
    cout << "  [线程 " << this_thread::get_id() 
         << "] 1. 普通函数（带参数）: " << a << ", " << b << endl;
}

// ============================================================================
// 二、类的成员函数（Member Function）
// ============================================================================

class MyClass {
public:
    void member_function() {
        cout << "  [线程 " << this_thread::get_id() 
             << "] 2. 类的成员函数" << endl;
    }
    
    void member_function_with_params(int x) {
        cout << "  [线程 " << this_thread::get_id() 
             << "] 2. 类的成员函数（带参数）: " << x << endl;
    }
    
    // ============================================================================
    // 三、静态成员函数（Static Member Function）
    // ============================================================================
    
    static void static_member_function() {
        cout << "  [线程 " << this_thread::get_id() 
             << "] 3. 静态成员函数" << endl;
    }
    
    static void static_member_with_params(double d) {
        cout << "  [线程 " << this_thread::get_id() 
             << "] 3. 静态成员函数（带参数）: " << d << endl;
    }
};

// ============================================================================
// 四、函数对象（Functor / Function Object）
// ============================================================================

class SimpleFunctor {
public:
    void operator()() const {
        cout << "  [线程 " << this_thread::get_id() 
             << "] 4. 函数对象（无参数）" << endl;
    }
};

class FunctorWithParams {
public:
    void operator()(int n, string s) const {
        cout << "  [线程 " << this_thread::get_id() 
             << "] 4. 函数对象（带参数）: " << n << ", " << s << endl;
    }
};

class StatefulFunctor {
    int count;
public:
    StatefulFunctor(int c) : count(c) {}
    
    void operator()() const {
        cout << "  [线程 " << this_thread::get_id() 
             << "] 4. 函数对象（带状态）: count = " << count << endl;
    }
};

// ============================================================================
// 五、Lambda 表达式（Lambda Expression）
// ============================================================================

// Lambda 在使用时定义，这里只是示例占位

// ============================================================================
// 六、std::function（通用函数包装器）
// ============================================================================

// std::function 可以包装任何可调用对象

// ============================================================================
// 完整演示
// ============================================================================

void demonstrate_all_callable_types() {
    cout << "\n╔═══════════════════════════════════════════════════╗" << endl;
    cout << "║     C++ 线程支持的 6 种可调用类型                 ║" << endl;
    cout << "╚═══════════════════════════════════════════════════╝" << endl;
    
    // ========================================================================
    // 1. 普通函数
    // ========================================================================
    cout << "\n=== 1. 普通函数（Free Function）===" << endl;
    
    cout << "\n1.1 无参数的普通函数：" << endl;
    thread t1(normal_function);
    t1.join();
    
    cout << "\n1.2 带参数的普通函数：" << endl;
    thread t2(normal_function_with_params, 42, "Hello");
    t2.join();
    
    cout << "\n特点：" << endl;
    cout << "  ✅ 最简单、最常用" << endl;
    cout << "  ✅ 不需要对象" << endl;
    cout << "  ✅ 可以有参数和返回值（通过 std::async）" << endl;
    
    // ========================================================================
    // 2. 类的成员函数
    // ========================================================================
    cout << "\n=== 2. 类的成员函数（Member Function）===" << endl;
    
    MyClass obj;
    
    cout << "\n2.1 无参数的成员函数：" << endl;
    thread t3(&MyClass::member_function, &obj);
    t3.join();
    
    cout << "\n2.2 带参数的成员函数：" << endl;
    thread t4(&MyClass::member_function_with_params, &obj, 100);
    t4.join();
    
    cout << "\n语法：" << endl;
    cout << "  thread t(&Class::method, &object, args...);" << endl;
    cout << "           ↑               ↑        ↑" << endl;
    cout << "        成员函数指针    对象地址  参数" << endl;
    
    cout << "\n特点：" << endl;
    cout << "  ✅ 可以访问对象的成员变量" << endl;
    cout << "  ⚠️ 必须传递对象指针或引用" << endl;
    cout << "  ⚠️ 注意对象生命周期（避免悬空引用）" << endl;
    
    // ========================================================================
    // 3. 静态成员函数
    // ========================================================================
    cout << "\n=== 3. 静态成员函数（Static Member Function）===" << endl;
    
    cout << "\n3.1 无参数的静态成员函数：" << endl;
    thread t5(&MyClass::static_member_function);
    t5.join();
    
    cout << "\n3.2 带参数的静态成员函数：" << endl;
    thread t6(&MyClass::static_member_with_params, 3.14);
    t6.join();
    
    cout << "\n语法：" << endl;
    cout << "  thread t(&Class::static_method, args...);" << endl;
    cout << "           ↑                      ↑" << endl;
    cout << "      静态函数指针              参数（不需要对象）" << endl;
    
    cout << "\n特点：" << endl;
    cout << "  ✅ 不需要对象实例" << endl;
    cout << "  ✅ 类似普通函数，但在类的命名空间内" << endl;
    cout << "  ❌ 不能访问非静态成员变量" << endl;
    
    cout << "\n与普通成员函数的区别：" << endl;
    cout << "  普通成员函数：需要对象，可以访问成员变量" << endl;
    cout << "  静态成员函数：不需要对象，不能访问成员变量" << endl;
    
    // ========================================================================
    // 4. 函数对象（Functor）
    // ========================================================================
    cout << "\n=== 4. 函数对象（Functor / Function Object）===" << endl;
    
    cout << "\n4.1 简单函数对象：" << endl;
    SimpleFunctor functor1;
    thread t7(functor1);
    t7.join();
    
    cout << "\n4.2 带参数的函数对象：" << endl;
    FunctorWithParams functor2;
    thread t8(functor2, 99, "World");
    t8.join();
    
    cout << "\n4.3 带状态的函数对象：" << endl;
    StatefulFunctor functor3(888);
    thread t9(functor3);
    t9.join();
    
    cout << "\n定义：" << endl;
    cout << "  class Functor {" << endl;
    cout << "      void operator()() {  // 重载 () 运算符" << endl;
    cout << "          // 线程执行的代码" << endl;
    cout << "      }" << endl;
    cout << "  };" << endl;
    
    cout << "\n特点：" << endl;
    cout << "  ✅ 可以携带状态（成员变量）" << endl;
    cout << "  ✅ 可以有多个成员函数" << endl;
    cout << "  ✅ 可复用、可测试" << endl;
    cout << "  ⚠️ 需要定义完整的类" << endl;
    
    // ========================================================================
    // 5. Lambda 表达式
    // ========================================================================
    cout << "\n=== 5. Lambda 表达式（Lambda Expression）===" << endl;
    
    cout << "\n5.1 简单 Lambda：" << endl;
    thread t10([]() {
        cout << "  [线程 " << this_thread::get_id() 
             << "] 5. Lambda 表达式（无捕获）" << endl;
    });
    t10.join();
    
    cout << "\n5.2 带参数的 Lambda：" << endl;
    thread t11([](int x, string s) {
        cout << "  [线程 " << this_thread::get_id() 
             << "] 5. Lambda（带参数）: " << x << ", " << s << endl;
    }, 666, "Lambda");
    t11.join();
    
    cout << "\n5.3 捕获变量的 Lambda：" << endl;
    int value = 777;
    string message = "Captured";
    thread t12([value, &message]() {
        cout << "  [线程 " << this_thread::get_id() 
             << "] 5. Lambda（捕获变量）: " << value << ", " << message << endl;
    });
    t12.join();
    
    cout << "\n5.4 捕获所有变量的 Lambda：" << endl;
    int a = 1, b = 2;
    thread t13([=]() {  // 按值捕获所有变量
        cout << "  [线程 " << this_thread::get_id() 
             << "] 5. Lambda（捕获所有）: a=" << a << ", b=" << b << endl;
    });
    t13.join();
    
    cout << "\n语法：" << endl;
    cout << "  []() { }           - 无捕获，无参数" << endl;
    cout << "  [x]() { }          - 按值捕获 x" << endl;
    cout << "  [&x]() { }         - 按引用捕获 x" << endl;
    cout << "  [=]() { }          - 按值捕获所有" << endl;
    cout << "  [&]() { }          - 按引用捕获所有" << endl;
    cout << "  [x, &y]() { }      - 混合捕获" << endl;
    cout << "  [](int a) { }      - 带参数" << endl;
    
    cout << "\n特点：" << endl;
    cout << "  ✅ 最简洁、最灵活（C++11 后最常用）" << endl;
    cout << "  ✅ 可以捕获外部变量" << endl;
    cout << "  ✅ 适合一次性使用的简单逻辑" << endl;
    cout << "  ⚠️ 注意捕获引用的生命周期" << endl;
    
    // ========================================================================
    // 6. std::function（通用函数包装器）
    // ========================================================================
    cout << "\n=== 6. std::function（通用函数包装器）===" << endl;
    
    cout << "\n6.1 包装普通函数：" << endl;
    function<void()> func1 = normal_function;
    thread t14(func1);
    t14.join();
    
    cout << "\n6.2 包装 Lambda：" << endl;
    function<void()> func2 = []() {
        cout << "  [线程 " << this_thread::get_id() 
             << "] 6. std::function（包装Lambda）" << endl;
    };
    thread t15(func2);
    t15.join();
    
    cout << "\n6.3 包装函数对象：" << endl;
    function<void()> func3 = SimpleFunctor();
    thread t16(func3);
    t16.join();
    
    cout << "\n6.4 包装带参数的 Lambda：" << endl;
    function<void(int, string)> func4 = [](int n, string s) {
        cout << "  [线程 " << this_thread::get_id() 
             << "] 6. std::function（带参数）: " << n << ", " << s << endl;
    };
    thread t17(func4, 555, "Function");
    t17.join();
    
    cout << "\n定义：" << endl;
    cout << "  function<返回值(参数类型)> func = ...;" << endl;
    cout << "  例如：" << endl;
    cout << "    function<void()> f1;           // 无参数，无返回值" << endl;
    cout << "    function<int(int)> f2;         // int 参数，int 返回值" << endl;
    cout << "    function<void(int, string)> f3; // 多参数" << endl;
    
    cout << "\n特点：" << endl;
    cout << "  ✅ 可以包装任何可调用对象" << endl;
    cout << "  ✅ 统一的类型（多态性）" << endl;
    cout << "  ✅ 可以存储、传递、赋值" << endl;
    cout << "  ⚠️ 有一定性能开销（虚函数调用）" << endl;
    
    cout << "\n用途：" << endl;
    cout << "  - 需要存储可调用对象（如回调函数）" << endl;
    cout << "  - 需要统一的接口" << endl;
    cout << "  - 需要运行时改变行为" << endl;
}

// ============================================================================
// 对比总结
// ============================================================================

void summary() {
    cout << "\n========================================" << endl;
    cout << "           对比总结" << endl;
    cout << "========================================" << endl;
    
    cout << "\nC++ 线程支持的 6 种可调用类型：" << endl;
    
    cout << "\n┌────────────────────┬─────────────────┬─────────────────┬─────────────────┐" << endl;
    cout << "│ 类型               │ 语法示例        │ 是否需要对象    │ 推荐程度        │" << endl;
    cout << "├────────────────────┼─────────────────┼─────────────────┼─────────────────┤" << endl;
    cout << "│ 1. 普通函数        │ t(func)         │ ❌ 否           │ ⭐⭐⭐⭐        │" << endl;
    cout << "│ 2. 成员函数        │ t(&C::m, &obj)  │ ✅ 是           │ ⭐⭐⭐          │" << endl;
    cout << "│ 3. 静态成员函数    │ t(&C::s)        │ ❌ 否           │ ⭐⭐⭐          │" << endl;
    cout << "│ 4. 函数对象        │ t(functor)      │ ⚠️ 看情况      │ ⭐⭐⭐          │" << endl;
    cout << "│ 5. Lambda          │ t([](){})       │ ❌ 否           │ ⭐⭐⭐⭐⭐      │" << endl;
    cout << "│ 6. std::function   │ t(func_obj)     │ ⚠️ 看包装内容  │ ⭐⭐⭐⭐        │" << endl;
    cout << "└────────────────────┴─────────────────┴─────────────────┴─────────────────┘" << endl;
    
    cout << "\n详细对比：" << endl;
    cout << "┌────────────────┬──────────┬──────────┬──────────┬──────────┬──────────┐" << endl;
    cout << "│ 特性           │ 简洁性   │ 灵活性   │ 可复用性 │ 性能     │ 适用场景 │" << endl;
    cout << "├────────────────┼──────────┼──────────┼──────────┼──────────┼──────────┤" << endl;
    cout << "│ 1. 普通函数    │ ⭐⭐⭐⭐ │ ⭐⭐⭐   │ ⭐⭐⭐⭐ │ ⭐⭐⭐⭐ │ 简单任务 │" << endl;
    cout << "│ 2. 成员函数    │ ⭐⭐⭐   │ ⭐⭐⭐⭐ │ ⭐⭐⭐⭐ │ ⭐⭐⭐⭐ │ 面向对象 │" << endl;
    cout << "│ 3. 静态函数    │ ⭐⭐⭐⭐ │ ⭐⭐     │ ⭐⭐⭐⭐ │ ⭐⭐⭐⭐ │ 工具函数 │" << endl;
    cout << "│ 4. 函数对象    │ ⭐⭐     │ ⭐⭐⭐⭐⭐│ ⭐⭐⭐⭐⭐│ ⭐⭐⭐⭐ │ 复杂逻辑 │" << endl;
    cout << "│ 5. Lambda      │ ⭐⭐⭐⭐⭐│ ⭐⭐⭐⭐⭐│ ⭐⭐     │ ⭐⭐⭐⭐ │ 一次性   │" << endl;
    cout << "│ 6. std::func   │ ⭐⭐⭐   │ ⭐⭐⭐⭐⭐│ ⭐⭐⭐⭐⭐│ ⭐⭐⭐   │ 回调函数 │" << endl;
    cout << "└────────────────┴──────────┴──────────┴──────────┴──────────┴──────────┘" << endl;
    
    cout << "\n使用建议：" << endl;
    cout << "  1️⃣ 简单场景：Lambda（最推荐）" << endl;
    cout << "     thread t([]() { cout << \"Hello\"; });" << endl;
    
    cout << "\n  2️⃣ 需要访问对象成员：成员函数" << endl;
    cout << "     thread t(&MyClass::method, &obj);" << endl;
    
    cout << "\n  3️⃣ 复杂可复用逻辑：函数对象" << endl;
    cout << "     class Worker { void operator()() {...} };" << endl;
    cout << "     thread t(Worker());" << endl;
    
    cout << "\n  4️⃣ 工具函数：普通函数或静态成员函数" << endl;
    cout << "     thread t(utility_function);" << endl;
    
    cout << "\n  5️⃣ 需要统一接口：std::function" << endl;
    cout << "     function<void()> callback = []() {...};" << endl;
    cout << "     thread t(callback);" << endl;
    
    cout << "\n你的理解对比：" << endl;
    cout << "  你说的：普通函数、类成员函数、函数对象、Lambda" << endl;
    cout << "  实际上：" << endl;
    cout << "    ✅ 普通函数" << endl;
    cout << "    ✅ 类成员函数（还包括静态成员函数）" << endl;
    cout << "    ✅ 函数对象" << endl;
    cout << "    ✅ Lambda 表达式" << endl;
    cout << "    ➕ std::function（通用包装器）" << endl;
    
    cout << "\n  所以更准确的说法：" << endl;
    cout << "    C++ 线程支持 6 种可调用类型" << endl;
    cout << "    或者说：支持所有符合 Callable 概念的对象" << endl;
    
    cout << "\n记忆口诀：" << endl;
    cout << "  普通函数最简单，成员函数需对象" << endl;
    cout << "  静态函数不需实例，函数对象可复用" << endl;
    cout << "  Lambda 最灵活常用，function 统一包装" << endl;
    cout << "  六种方式任你选，根据场景来决定" << endl;
    
    cout << "\n========================================" << endl;
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    demonstrate_all_callable_types();
    summary();
    return 0;
}
