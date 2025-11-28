/*
 * 成员函数指针详解
 * 主题：&Class::method 的含义和用法
 * 核心：成员函数指针、对象传递方式
 */

#include <iostream>
#include <thread>
#include <memory>
using namespace std;

class Counter {
    int count = 0;
    string name;
    
public:
    Counter(string n) : name(n) {}
    
    void increment(int times) {
        for (int i = 0; i < times; i++) {
            count++;
        }
        cout << "  [" << name << "] Count: " << count << endl;
    }
    
    void print_info() const {
        cout << "  [" << name << "] 当前计数: " << count << endl;
    }
    
    int get_count() const { return count; }
};

// ============================================================================
// 一、&Class::method 的含义
// ============================================================================

void explain_member_function_pointer() {
    cout << "\n=== 1. &Class::method 的含义 ===" << endl;
    
    cout << "\n这是什么？" << endl;
    cout << "  &Counter::increment - 成员函数指针（Member Function Pointer）" << endl;
    cout << "  不是引用！是指向成员函数的指针" << endl;
    
    cout << "\n语法规则：" << endl;
    cout << "  &ClassName::method_name" << endl;
    cout << "  这是 C++ 获取成员函数地址的唯一标准写法" << endl;
    
    cout << "\n为什么需要 & ？" << endl;
    cout << "  - 普通函数：函数名本身就是地址" << endl;
    cout << "    void func() {};" << endl;
    cout << "    func;  // 等价于 &func" << endl;
    
    cout << "\n  - 成员函数：必须显式使用 &" << endl;
    cout << "    class A { void method() {} };" << endl;
    cout << "    &A::method;  // ✅ 正确" << endl;
    cout << "    A::method;   // ❌ 错误，语法不允许" << endl;
    
    cout << "\n成员函数指针的类型：" << endl;
    cout << "  void (Counter::*ptr)(int);" << endl;
    cout << "       ↑       ↑      ↑" << endl;
    cout << "    返回类型  类名  参数类型" << endl;
    
    // 实际声明一个成员函数指针
    void (Counter::*func_ptr)(int) = &Counter::increment;
    cout << "\n  声明: void (Counter::*func_ptr)(int) = &Counter::increment;" << endl;
    cout << "  这个指针指向 Counter 类的 increment 成员函数" << endl;
}

// ============================================================================
// 二、std::thread 使用成员函数的方式
// ============================================================================

void demonstrate_thread_with_member_function() {
    cout << "\n=== 2. std::thread 使用成员函数 ===" << endl;
    
    cout << "\n完整语法：" << endl;
    cout << "  thread t(&Class::method, object, args...);" << endl;
    cout << "            ↑              ↑       ↑" << endl;
    cout << "        成员函数指针    对象    参数" << endl;
    
    cout << "\n方式1: 传递对象指针（推荐）" << endl;
    {
        Counter counter1("对象指针");
        thread t(&Counter::increment, &counter1, 5);
        //         ↑                   ↑          ↑
        //     函数指针            对象地址     参数
        t.join();
        cout << "  说明：&counter1 传递对象的地址" << endl;
    }
    
    cout << "\n方式2: 传递对象引用（使用 ref）" << endl;
    {
        Counter counter2("对象引用");
        thread t(&Counter::increment, ref(counter2), 5);
        //         ↑                   ↑              ↑
        //     函数指针           引用包装器        参数
        t.join();
        cout << "  说明：ref(counter2) 避免拷贝对象" << endl;
    }
    
    cout << "\n方式3: 传递对象拷贝（不推荐，会拷贝整个对象）" << endl;
    {
        Counter counter3("对象拷贝");
        thread t(&Counter::increment, counter3, 5);
        //         ↑                   ↑         ↑
        //     函数指针            对象拷贝    参数
        t.join();
        cout << "  说明：counter3 会被拷贝一份到线程中" << endl;
        cout << "  原对象不受影响: ";
        counter3.print_info();
    }
    
    cout << "\n方式4: 使用智能指针" << endl;
    {
        auto counter4 = make_shared<Counter>("智能指针");
        thread t(&Counter::increment, counter4, 5);
        //         ↑                   ↑          ↑
        //     函数指针         shared_ptr     参数
        t.join();
        cout << "  说明：shared_ptr 自动管理生命周期" << endl;
    }
}

// ============================================================================
// 三、对比：成员函数指针 vs 函数调用
// ============================================================================

void compare_pointer_vs_call() {
    cout << "\n=== 3. 成员函数指针 vs 函数调用 ===" << endl;
    
    Counter counter("比较");
    
    cout << "\n情况1: 获取成员函数指针（不执行）" << endl;
    cout << "  代码: &Counter::increment" << endl;
    cout << "  含义: 获取 increment 函数的地址" << endl;
    cout << "  结果: 一个指针值，指向函数代码" << endl;
    cout << "  用途: 传递给 thread、bind 等" << endl;
    
    cout << "\n情况2: 调用成员函数（立即执行）" << endl;
    cout << "  代码: counter.increment(3)" << endl;
    cout << "  含义: 在当前线程立即执行函数" << endl;
    cout << "  结果: ";
    counter.increment(3);
    
    cout << "\n情况3: 通过指针调用" << endl;
    void (Counter::*ptr)(int) = &Counter::increment;
    cout << "  代码: (counter.*ptr)(2);" << endl;
    cout << "  含义: 通过成员函数指针调用" << endl;
    cout << "  结果: ";
    (counter.*ptr)(2);
    
    cout << "\n语法总结：" << endl;
    cout << "  ┌──────────────────────┬────────────────┬────────────┐" << endl;
    cout << "  │ 写法                 │ 类型           │ 用途       │" << endl;
    cout << "  ├──────────────────────┼────────────────┼────────────┤" << endl;
    cout << "  │ &Counter::increment  │ 函数指针       │ 传递函数   │" << endl;
    cout << "  │ counter.increment    │ 函数调用       │ 执行函数   │" << endl;
    cout << "  │ (obj.*ptr)(args)     │ 指针调用       │ 间接调用   │" << endl;
    cout << "  └──────────────────────┴────────────────┴────────────┘" << endl;
}

// ============================================================================
// 四、为什么这样设计？
// ============================================================================

void explain_why_this_syntax() {
    cout << "\n=== 4. 为什么需要这种语法？===" << endl;
    
    cout << "\n问题：成员函数需要对象才能调用" << endl;
    cout << "  class Counter {" << endl;
    cout << "      void increment() { count++; }  // 需要 this 指针" << endl;
    cout << "  };" << endl;
    
    cout << "\n  普通调用：" << endl;
    cout << "    Counter c;" << endl;
    cout << "    c.increment();  // 编译器自动传递 this = &c" << endl;
    
    cout << "\n  线程中调用：" << endl;
    cout << "    thread t(&Counter::increment, &c);" << endl;
    cout << "             ↑                    ↑" << endl;
    cout << "          函数是什么          对象是谁" << endl;
    
    cout << "\n解决方案：分开传递" << endl;
    cout << "  1. &Counter::increment - 告诉 thread 要调用哪个函数" << endl;
    cout << "  2. &counter           - 告诉 thread 在哪个对象上调用" << endl;
    cout << "  3. args...            - 传递函数参数" << endl;
    
    cout << "\nthread 内部实现类似：" << endl;
    cout << "  auto func_ptr = &Counter::increment;" << endl;
    cout << "  auto obj_ptr = &counter;" << endl;
    cout << "  (obj_ptr->*func_ptr)(args...);  // 调用成员函数" << endl;
    
    cout << "\n类比：" << endl;
    cout << "  就像快递配送，需要两个信息：" << endl;
    cout << "  - 函数指针 = 配送员（知道怎么送）" << endl;
    cout << "  - 对象     = 地址（知道送到哪里）" << endl;
    cout << "  两者缺一不可！" << endl;
}

// ============================================================================
// 五、其他传递成员函数的方式
// ============================================================================

void other_ways_to_pass_member_function() {
    cout << "\n=== 5. 其他传递成员函数的方式 ===" << endl;
    
    Counter counter("其他方式");
    
    cout << "\n方式1: 直接使用成员函数指针（我们一直在用）" << endl;
    cout << "  thread t(&Counter::increment, &counter, 3);" << endl;
    thread t1(&Counter::increment, &counter, 3);
    t1.join();
    
    cout << "\n方式2: 使用 Lambda 包装（更灵活）" << endl;
    cout << "  thread t([&counter]() {" << endl;
    cout << "      counter.increment(3);" << endl;
    cout << "  });" << endl;
    thread t2([&counter]() {
        counter.increment(3);
    });
    t2.join();
    cout << "  优点：可以调用多个成员函数" << endl;
    
    cout << "\n方式3: 使用 std::bind（C++11）" << endl;
    cout << "  auto bound = bind(&Counter::increment, &counter, 3);" << endl;
    cout << "  thread t(bound);" << endl;
    auto bound = bind(&Counter::increment, &counter, 3);
    thread t3(bound);
    t3.join();
    cout << "  说明：bind 预先绑定对象和参数" << endl;
    
    cout << "\n方式4: 使用 std::mem_fn（很少用）" << endl;
    cout << "  auto mem_fn_obj = mem_fn(&Counter::increment);" << endl;
    cout << "  thread t([&]() { mem_fn_obj(counter, 3); });" << endl;
    auto mem_fn_obj = mem_fn(&Counter::increment);
    thread t4([&]() { mem_fn_obj(counter, 3); });
    t4.join();
    
    cout << "\n推荐程度：" << endl;
    cout << "  ⭐⭐⭐⭐⭐ 方式1（直接用指针）- 最简洁、最常用" << endl;
    cout << "  ⭐⭐⭐⭐   方式2（Lambda）   - 最灵活、可读性好" << endl;
    cout << "  ⭐⭐⭐     方式3（bind）     - 复杂、不推荐" << endl;
    cout << "  ⭐⭐       方式4（mem_fn）   - 很少用" << endl;
}

// ============================================================================
// 六、常见错误
// ============================================================================

void common_mistakes() {
    cout << "\n=== 6. 常见错误 ===" << endl;
    
    cout << "\n❌ 错误1: 忘记 &" << endl;
    cout << "  thread t(Counter::increment, &counter, 5);" << endl;
    cout << "  编译错误：成员函数必须用 &Class::method" << endl;
    
    cout << "\n❌ 错误2: 忘记传对象" << endl;
    cout << "  thread t(&Counter::increment, 5);" << endl;
    cout << "  编译错误：成员函数需要对象才能调用" << endl;
    
    cout << "\n❌ 错误3: 对象生命周期问题" << endl;
    cout << "  {" << endl;
    cout << "      Counter c;" << endl;
    cout << "      thread t(&Counter::increment, &c, 5);" << endl;
    cout << "      t.detach();  // ❌ 危险！" << endl;
    cout << "  }  // c 被销毁，但线程还在运行" << endl;
    
    cout << "\n❌ 错误4: 传值导致意外拷贝" << endl;
    cout << "  Counter c;" << endl;
    cout << "  thread t(&Counter::increment, c, 5);  // c 被拷贝" << endl;
    cout << "  t.join();" << endl;
    cout << "  cout << c.get_count();  // 0（原对象未改变）" << endl;
    
    cout << "\n✅ 正确写法：" << endl;
    cout << "  Counter c;" << endl;
    cout << "  thread t(&Counter::increment, &c, 5);  // 传地址" << endl;
    cout << "  // 或" << endl;
    cout << "  thread t(&Counter::increment, ref(c), 5);  // 传引用" << endl;
}

// ============================================================================
// 七、核心总结
// ============================================================================

void summary() {
    cout << "\n========================================" << endl;
    cout << "           核心总结" << endl;
    cout << "========================================" << endl;
    
    cout << "\n一、&Counter::increment 是什么？" << endl;
    cout << "  - 成员函数指针（不是引用！）" << endl;
    cout << "  - 指向 Counter 类的 increment 成员函数" << endl;
    cout << "  - 这是获取成员函数地址的唯一标准写法" << endl;
    
    cout << "\n二、为什么需要 & ？" << endl;
    cout << "  - 普通函数：函数名 = 地址" << endl;
    cout << "  - 成员函数：必须显式 &Class::method" << endl;
    cout << "  - 这是 C++ 语法规则" << endl;
    
    cout << "\n三、std::thread 的完整语法：" << endl;
    cout << "  thread t(&Class::method, object, args...);" << endl;
    cout << "           ↓               ↓       ↓" << endl;
    cout << "        成员函数指针    对象    参数" << endl;
    
    cout << "\n四、传递对象的方式：" << endl;
    cout << "  ┌────────────────┬──────────────┬──────────┐" << endl;
    cout << "  │ 写法           │ 含义         │ 推荐度   │" << endl;
    cout << "  ├────────────────┼──────────────┼──────────┤" << endl;
    cout << "  │ &counter       │ 传地址       │ ⭐⭐⭐⭐⭐ │" << endl;
    cout << "  │ ref(counter)   │ 传引用       │ ⭐⭐⭐⭐⭐ │" << endl;
    cout << "  │ counter        │ 传拷贝       │ ⭐        │" << endl;
    cout << "  │ shared_ptr     │ 智能指针     │ ⭐⭐⭐⭐   │" << endl;
    cout << "  └────────────────┴──────────────┴──────────┘" << endl;
    
    cout << "\n五、其他传递方式：" << endl;
    cout << "  1. 直接用指针：t(&Class::method, &obj, args)" << endl;
    cout << "     ⭐⭐⭐⭐⭐ 最简洁、最常用" << endl;
    
    cout << "\n  2. Lambda 包装：t([&obj]() { obj.method(args); })" << endl;
    cout << "     ⭐⭐⭐⭐ 最灵活、可读性好" << endl;
    
    cout << "\n  3. std::bind：t(bind(&Class::method, &obj, args))" << endl;
    cout << "     ⭐⭐⭐ 复杂、不推荐（Lambda 更好）" << endl;
    
    cout << "\n六、对比：" << endl;
    cout << "  ┌──────────────────────┬────────────┬──────────┐" << endl;
    cout << "  │ 写法                 │ 含义       │ 何时执行 │" << endl;
    cout << "  ├──────────────────────┼────────────┼──────────┤" << endl;
    cout << "  │ &Counter::increment  │ 函数指针   │ 不执行   │" << endl;
    cout << "  │ counter.increment(5) │ 函数调用   │ 立即执行 │" << endl;
    cout << "  │ (obj.*ptr)(args)     │ 指针调用   │ 立即执行 │" << endl;
    cout << "  └──────────────────────┴────────────┴──────────┘" << endl;
    
    cout << "\n七、记忆口诀：" << endl;
    cout << "  成员函数要指针，& 号不能省" << endl;
    cout << "  对象地址跟着传，ref 包装防拷贝" << endl;
    cout << "  thread 内部调用时，需要知道在哪调" << endl;
    cout << "  函数指针加对象，两个参数缺一不可" << endl;
    
    cout << "\n八、类比理解：" << endl;
    cout << "  &Counter::increment - 食谱（告诉你怎么做菜）" << endl;
    cout << "  &counter           - 厨房（告诉你在哪做菜）" << endl;
    cout << "  args...            - 食材（告诉你用什么做菜）" << endl;
    cout << "  三者缺一不可才能做出菜！" << endl;
    
    cout << "\n========================================" << endl;
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "\n╔═══════════════════════════════════════════════════╗" << endl;
    cout << "║         成员函数指针详解                          ║" << endl;
    cout << "║   &Class::method 的含义和用法                     ║" << endl;
    cout << "╚═══════════════════════════════════════════════════╝" << endl;
    
    // 1. &Class::method 的含义
    explain_member_function_pointer();
    
    // 2. std::thread 使用成员函数
    demonstrate_thread_with_member_function();
    
    // 3. 成员函数指针 vs 函数调用
    compare_pointer_vs_call();
    
    // 4. 为什么这样设计
    explain_why_this_syntax();
    
    // 5. 其他传递成员函数的方式
    other_ways_to_pass_member_function();
    
    // 6. 常见错误
    common_mistakes();
    
    // 7. 总结
    summary();
    
    return 0;
}
