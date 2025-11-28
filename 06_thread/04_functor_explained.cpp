/*
 * 函数对象（Functor）详解
 * 主题：什么是函数对象，与普通对象的区别
 * 核心：operator() 重载、函数对象的灵活性
 */

#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
using namespace std;

// ============================================================================
// 一、什么是函数对象（Functor）？
// ============================================================================

void explain_functor() {
    cout << "\n=== 1. 什么是函数对象？===" << endl;
    
    cout << "\n定义：" << endl;
    cout << "  函数对象（Functor）= 重载了 operator() 的类对象" << endl;
    cout << "  本质：是一个对象，但可以像函数一样被调用" << endl;
    
    cout << "\n核心：operator() 重载" << endl;
    cout << "  class MyFunctor {" << endl;
    cout << "      void operator()() {  // 重载 () 运算符" << endl;
    cout << "          // 函数体" << endl;
    cout << "      }" << endl;
    cout << "  };" << endl;
    
    cout << "\n使用：" << endl;
    cout << "  MyFunctor func;  // 创建对象" << endl;
    cout << "  func();          // 像函数一样调用！" << endl;
    
    cout << "\n与普通对象的区别：" << endl;
    cout << "  ┌─────────────────┬─────────────────┬─────────────────┐" << endl;
    cout << "  │ 特性            │ 普通对象        │ 函数对象        │" << endl;
    cout << "  ├─────────────────┼─────────────────┼─────────────────┤" << endl;
    cout << "  │ 定义方式        │ class A {}      │ class A {}      │" << endl;
    cout << "  │ operator() 重载 │ ❌ 无           │ ✅ 有           │" << endl;
    cout << "  │ 调用方式        │ obj.method()    │ obj() 或 method │" << endl;
    cout << "  │ 可以有成员变量  │ ✅ 可以         │ ✅ 可以         │" << endl;
    cout << "  │ 可以有其他方法  │ ✅ 可以         │ ✅ 可以         │" << endl;
    cout << "  └─────────────────┴─────────────────┴─────────────────┘" << endl;
}

// ============================================================================
// 二、函数对象 vs 普通对象
// ============================================================================

// 普通对象
class NormalClass {
    int value;
public:
    NormalClass(int v) : value(v) {}
    
    void print() {
        cout << "    普通方法: value = " << value << endl;
    }
    
    int getValue() { return value; }
};

// 函数对象
class FunctorClass {
    int value;
public:
    FunctorClass(int v) : value(v) {}
    
    // ✅ 重载了 operator()，可以像函数一样调用
    void operator()() {
        cout << "    函数对象调用: value = " << value << endl;
    }
    
    // ✅ 也可以有普通方法
    void print() {
        cout << "    普通方法: value = " << value << endl;
    }
    
    int getValue() { return value; }
};

void compare_normal_vs_functor() {
    cout << "\n=== 2. 函数对象 vs 普通对象 ===" << endl;
    
    cout << "\n普通对象的使用：" << endl;
    NormalClass normal(100);
    cout << "  创建对象: NormalClass normal(100);" << endl;
    cout << "  调用方法: normal.print();" << endl;
    normal.print();
    // normal();  // ❌ 错误！普通对象不能这样调用
    
    cout << "\n函数对象的使用：" << endl;
    FunctorClass functor(200);
    cout << "  创建对象: FunctorClass functor(200);" << endl;
    
    cout << "  方式1 - 像函数一样调用: functor();" << endl;
    functor();  // ✅ 调用 operator()
    
    cout << "  方式2 - 调用普通方法: functor.print();" << endl;
    functor.print();  // ✅ 也可以调用普通方法
    
    cout << "\n关键区别：" << endl;
    cout << "  普通对象：只能 obj.method() 调用" << endl;
    cout << "  函数对象：可以 obj() 调用（因为重载了 operator()）" << endl;
}

// ============================================================================
// 三、operator() 可以有参数和返回值
// ============================================================================

// 无参数的函数对象
class PrintFunctor {
public:
    void operator()() const {
        cout << "    无参数的 operator()" << endl;
    }
};

// 有参数的函数对象
class AddFunctor {
public:
    void operator()(int a, int b) const {
        cout << "    operator(int, int): " << a << " + " << b << " = " << (a + b) << endl;
    }
};

// 有返回值的函数对象
class MultiplyFunctor {
public:
    int operator()(int a, int b) const {
        return a * b;
    }
};

// 多个参数和返回值
class CalculatorFunctor {
public:
    double operator()(double a, double b, char op) const {
        switch(op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': return b != 0 ? a / b : 0;
            default: return 0;
        }
    }
};

void demonstrate_operator_overloading() {
    cout << "\n=== 3. operator() 可以有参数和返回值 ===" << endl;
    
    cout << "\n情况1: 无参数" << endl;
    cout << "  void operator()() { ... }" << endl;
    PrintFunctor print;
    print();
    
    cout << "\n情况2: 有参数" << endl;
    cout << "  void operator()(int a, int b) { ... }" << endl;
    AddFunctor add;
    add(10, 20);
    
    cout << "\n情况3: 有返回值" << endl;
    cout << "  int operator()(int a, int b) { ... }" << endl;
    MultiplyFunctor multiply;
    int result = multiply(5, 6);
    cout << "    返回值: " << result << endl;
    
    cout << "\n情况4: 多个参数和返回值" << endl;
    cout << "  double operator()(double a, double b, char op) { ... }" << endl;
    CalculatorFunctor calc;
    cout << "    10 + 5 = " << calc(10, 5, '+') << endl;
    cout << "    10 * 5 = " << calc(10, 5, '*') << endl;
    
    cout << "\n结论：operator() 完全灵活！" << endl;
    cout << "  ✅ 可以无参数、有参数、多个参数" << endl;
    cout << "  ✅ 可以无返回值（void）、有返回值（任意类型）" << endl;
    cout << "  ✅ 可以有多个重载版本" << endl;
}

// ============================================================================
// 四、函数对象可以有状态（成员变量）
// ============================================================================

// 计数器函数对象 - 带状态
class Counter {
    int count = 0;
    string name;
    
public:
    Counter(string n) : name(n) {}
    
    // operator() 可以修改成员变量
    void operator()() {
        count++;
        cout << "    [" << name << "] 调用次数: " << count << endl;
    }
    
    int getCount() const { return count; }
};

// 累加器函数对象 - 带状态
class Accumulator {
    int sum = 0;
    
public:
    void operator()(int value) {
        sum += value;
    }
    
    int getSum() const { return sum; }
};

void demonstrate_stateful_functor() {
    cout << "\n=== 4. 函数对象可以有状态（成员变量）===" << endl;
    
    cout << "\n示例1: 计数器" << endl;
    Counter counter("计数器");
    cout << "  调用 counter() 多次：" << endl;
    counter();
    counter();
    counter();
    cout << "  总调用次数: " << counter.getCount() << endl;
    
    cout << "\n示例2: 累加器" << endl;
    Accumulator acc;
    cout << "  累加: 1, 2, 3, 4, 5" << endl;
    acc(1);
    acc(2);
    acc(3);
    acc(4);
    acc(5);
    cout << "  总和: " << acc.getSum() << endl;
    
    cout << "\n优势：" << endl;
    cout << "  ✅ 函数对象可以保存状态（成员变量）" << endl;
    cout << "  ✅ 普通函数无法保存状态（除非用全局变量）" << endl;
    cout << "  ✅ 每个函数对象实例有独立的状态" << endl;
}

// ============================================================================
// 五、函数对象可以有多个方法和成员
// ============================================================================

class ComplexFunctor {
    int threshold;
    int count = 0;
    vector<int> history;
    
public:
    ComplexFunctor(int t) : threshold(t) {}
    
    // ✅ operator() - 主要功能
    bool operator()(int value) {
        count++;
        history.push_back(value);
        return value > threshold;
    }
    
    // ✅ 其他方法
    void printStats() const {
        cout << "    调用次数: " << count << endl;
        cout << "    阈值: " << threshold << endl;
        cout << "    历史记录: ";
        for (int v : history) {
            cout << v << " ";
        }
        cout << endl;
    }
    
    void reset() {
        count = 0;
        history.clear();
    }
    
    int getCount() const { return count; }
    void setThreshold(int t) { threshold = t; }
};

void demonstrate_complex_functor() {
    cout << "\n=== 5. 函数对象可以有多个方法和成员 ===" << endl;
    
    cout << "\n函数对象不限制成员数量：" << endl;
    cout << "  ✅ 可以有多个成员变量" << endl;
    cout << "  ✅ 可以有多个成员方法" << endl;
    cout << "  ✅ operator() 只是其中一个特殊方法" << endl;
    
    cout << "\n示例：复杂的函数对象" << endl;
    ComplexFunctor checker(50);
    
    cout << "  测试数据: 30, 60, 40, 80, 20" << endl;
    cout << "  结果: " << endl;
    cout << "    30 > 50? " << (checker(30) ? "是" : "否") << endl;
    cout << "    60 > 50? " << (checker(60) ? "是" : "否") << endl;
    cout << "    40 > 50? " << (checker(40) ? "是" : "否") << endl;
    cout << "    80 > 50? " << (checker(80) ? "是" : "否") << endl;
    cout << "    20 > 50? " << (checker(20) ? "是" : "否") << endl;
    
    cout << "\n  统计信息：" << endl;
    checker.printStats();
}

// ============================================================================
// 六、函数对象的实际应用
// ============================================================================

// 应用1: 与 STL 算法配合
class IsEven {
public:
    bool operator()(int n) const {
        return n % 2 == 0;
    }
};

class IsGreaterThan {
    int threshold;
public:
    IsGreaterThan(int t) : threshold(t) {}
    
    bool operator()(int n) const {
        return n > threshold;
    }
};

// 应用2: 与线程配合
class ThreadWorker {
    string name;
    int task_count;
    
public:
    ThreadWorker(string n, int count) : name(n), task_count(count) {}
    
    void operator()() {
        for (int i = 0; i < task_count; i++) {
            cout << "    [" << name << "] 执行任务 " << (i + 1) << endl;
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    }
};

void demonstrate_functor_usage() {
    cout << "\n=== 6. 函数对象的实际应用 ===" << endl;
    
    cout << "\n应用1: 与 STL 算法配合" << endl;
    vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    cout << "  原始数据: ";
    for (int n : numbers) cout << n << " ";
    cout << endl;
    
    // 使用函数对象过滤偶数
    cout << "  偶数: ";
    for (int n : numbers) {
        if (IsEven()(n)) {  // 创建临时对象并调用
            cout << n << " ";
        }
    }
    cout << endl;
    
    // 使用函数对象过滤大于5的数
    IsGreaterThan gt5(5);
    cout << "  大于5: ";
    for (int n : numbers) {
        if (gt5(n)) {
            cout << n << " ";
        }
    }
    cout << endl;
    
    cout << "\n应用2: 与线程配合" << endl;
    ThreadWorker worker("工作线程", 3);
    thread t(worker);  // 函数对象可以直接传给 thread
    t.join();
    
    cout << "\n应用3: 与 std::sort 配合" << endl;
    vector<int> data = {5, 2, 8, 1, 9, 3};
    
    // 自定义排序函数对象
    class DescendingSort {
    public:
        bool operator()(int a, int b) const {
            return a > b;  // 降序
        }
    };
    
    cout << "  排序前: ";
    for (int n : data) cout << n << " ";
    cout << endl;
    
    sort(data.begin(), data.end(), DescendingSort());
    
    cout << "  降序后: ";
    for (int n : data) cout << n << " ";
    cout << endl;
}

// ============================================================================
// 七、函数对象 vs Lambda vs 函数指针
// ============================================================================

// 函数指针方式
bool is_positive(int n) {
    return n > 0;
}

void compare_three_approaches() {
    cout << "\n=== 7. 函数对象 vs Lambda vs 函数指针 ===" << endl;
    
    vector<int> numbers = {-2, 3, -1, 5, 0, -7, 8};
    
    cout << "\n数据: ";
    for (int n : numbers) cout << n << " ";
    cout << endl;
    
    // 方式1: 函数指针
    cout << "\n方式1: 函数指针" << endl;
    cout << "  正数个数: " << count_if(numbers.begin(), numbers.end(), is_positive) << endl;
    
    // 方式2: 函数对象
    cout << "\n方式2: 函数对象" << endl;
    class IsPositive {
    public:
        bool operator()(int n) const { return n > 0; }
    };
    cout << "  正数个数: " << count_if(numbers.begin(), numbers.end(), IsPositive()) << endl;
    
    // 方式3: Lambda
    cout << "\n方式3: Lambda 表达式（C++11）" << endl;
    cout << "  正数个数: " << count_if(numbers.begin(), numbers.end(), [](int n) { return n > 0; }) << endl;
    
    cout << "\n对比：" << endl;
    cout << "  ┌─────────────────┬────────────┬────────────┬────────────┐" << endl;
    cout << "  │ 特性            │ 函数指针   │ 函数对象   │ Lambda     │" << endl;
    cout << "  ├─────────────────┼────────────┼────────────┼────────────┤" << endl;
    cout << "  │ 可内联优化      │ ❌         │ ✅         │ ✅         │" << endl;
    cout << "  │ 可以有状态      │ ❌         │ ✅         │ ✅         │" << endl;
    cout << "  │ 代码简洁性      │ ⭐⭐       │ ⭐⭐       │ ⭐⭐⭐⭐⭐ │" << endl;
    cout << "  │ 可复用性        │ ⭐⭐⭐     │ ⭐⭐⭐⭐   │ ⭐         │" << endl;
    cout << "  │ 灵活性          │ ⭐         │ ⭐⭐⭐⭐⭐ │ ⭐⭐⭐     │" << endl;
    cout << "  └─────────────────┴────────────┴────────────┴────────────┘" << endl;
    
    cout << "\n选择建议：" << endl;
    cout << "  ✅ 简单场景：Lambda（最简洁）" << endl;
    cout << "  ✅ 需要状态：函数对象" << endl;
    cout << "  ✅ 需要复用：函数对象" << endl;
    cout << "  ✅ C兼容：函数指针" << endl;
}

// ============================================================================
// 八、Java 中的对比
// ============================================================================

void compare_with_java() {
    cout << "\n=== 8. 与 Java 的对比 ===" << endl;
    
    cout << "\nJava 没有函数对象的概念，但有类似机制：" << endl;
    
    cout << "\n1. Java 的方式（C++ 函数对象之前）：" << endl;
    cout << "   // Java 接口 + 匿名类" << endl;
    cout << "   interface Comparator<T> {" << endl;
    cout << "       int compare(T a, T b);" << endl;
    cout << "   }" << endl;
    cout << "   " << endl;
    cout << "   Collections.sort(list, new Comparator<Integer>() {" << endl;
    cout << "       public int compare(Integer a, Integer b) {" << endl;
    cout << "           return b - a;  // 降序" << endl;
    cout << "       }" << endl;
    cout << "   });" << endl;
    
    cout << "\n2. C++ 的方式（函数对象）：" << endl;
    cout << "   class Comparator {" << endl;
    cout << "       bool operator()(int a, int b) const {" << endl;
    cout << "           return a > b;  // 降序" << endl;
    cout << "       }" << endl;
    cout << "   };" << endl;
    cout << "   " << endl;
    cout << "   sort(vec.begin(), vec.end(), Comparator());" << endl;
    
    cout << "\n3. Java 8+ Lambda：" << endl;
    cout << "   Collections.sort(list, (a, b) -> b - a);" << endl;
    
    cout << "\n4. C++11+ Lambda：" << endl;
    cout << "   sort(vec.begin(), vec.end(), [](int a, int b) { return a > b; });" << endl;
    
    cout << "\n核心区别：" << endl;
    cout << "  ┌──────────────────────┬──────────────────┬──────────────────┐" << endl;
    cout << "  │ 特性                 │ C++ 函数对象     │ Java             │" << endl;
    cout << "  ├──────────────────────┼──────────────────┼──────────────────┤" << endl;
    cout << "  │ 本质                 │ operator() 重载  │ 接口 + 方法      │" << endl;
    cout << "  │ 语法                 │ obj()            │ obj.method()     │" << endl;
    cout << "  │ 性能                 │ 可内联，更快     │ 虚函数调用       │" << endl;
    cout << "  │ 现代替代             │ Lambda           │ Lambda           │" << endl;
    cout << "  └──────────────────────┴──────────────────┴──────────────────┘" << endl;
    
    cout << "\nJava 的等价概念：" << endl;
    cout << "  C++ 函数对象 ≈ Java 函数式接口 + 实现类" << endl;
    cout << "  例如：Runnable, Callable, Comparator, Predicate 等" << endl;
}

// ============================================================================
// 九、核心总结
// ============================================================================

void summary() {
    cout << "\n========================================" << endl;
    cout << "           核心总结" << endl;
    cout << "========================================" << endl;
    
    cout << "\n一、什么是函数对象？" << endl;
    cout << "  - 重载了 operator() 的类" << endl;
    cout << "  - 对象可以像函数一样被调用" << endl;
    cout << "  - 语法: obj() 等价于 obj.operator()()" << endl;
    
    cout << "\n二、函数对象 vs 普通对象：" << endl;
    cout << "  相同点：" << endl;
    cout << "    ✅ 都用 class 声明" << endl;
    cout << "    ✅ 都可以有成员变量" << endl;
    cout << "    ✅ 都可以有成员方法" << endl;
    cout << "    ✅ 都可以有构造函数、析构函数" << endl;
    
    cout << "\n  不同点：" << endl;
    cout << "    函数对象：重载了 operator()，可以 obj() 调用" << endl;
    cout << "    普通对象：没有 operator()，只能 obj.method() 调用" << endl;
    
    cout << "\n三、operator() 的灵活性：" << endl;
    cout << "  ✅ 可以无参数: void operator()() { }" << endl;
    cout << "  ✅ 可以有参数: void operator()(int a, int b) { }" << endl;
    cout << "  ✅ 可以有返回值: int operator()(int x) { return x*2; }" << endl;
    cout << "  ✅ 可以有多个重载版本" << endl;
    cout << "  ✅ 参数和返回值类型任意" << endl;
    
    cout << "\n四、函数对象的优势：" << endl;
    cout << "  1. 可以保存状态（成员变量）" << endl;
    cout << "  2. 可以有多个方法和成员" << endl;
    cout << "  3. 可以内联优化，性能好" << endl;
    cout << "  4. 可以作为模板参数" << endl;
    cout << "  5. 代码可复用" << endl;
    
    cout << "\n五、常见应用场景：" << endl;
    cout << "  ✅ STL 算法: sort, find_if, count_if 等" << endl;
    cout << "  ✅ 线程创建: thread(functor)" << endl;
    cout << "  ✅ 回调函数" << endl;
    cout << "  ✅ 策略模式" << endl;
    
    cout << "\n六、三种可调用对象对比：" << endl;
    cout << "  ┌─────────────┬────────────┬────────────┬────────────┐" << endl;
    cout << "  │ 特性        │ 函数指针   │ 函数对象   │ Lambda     │" << endl;
    cout << "  ├─────────────┼────────────┼────────────┼────────────┤" << endl;
    cout << "  │ 语法        │ func       │ Functor()  │ [](){}     │" << endl;
    cout << "  │ 有状态      │ ❌         │ ✅         │ ✅         │" << endl;
    cout << "  │ 可内联      │ ❌         │ ✅         │ ✅         │" << endl;
    cout << "  │ 简洁性      │ ⭐⭐       │ ⭐⭐       │ ⭐⭐⭐⭐⭐ │" << endl;
    cout << "  │ 复用性      │ ⭐⭐⭐     │ ⭐⭐⭐⭐⭐ │ ⭐         │" << endl;
    cout << "  └─────────────┴────────────┴────────────┴────────────┘" << endl;
    
    cout << "\n七、与 Java 的对比：" << endl;
    cout << "  C++ 函数对象 ≈ Java 函数式接口 + 实现" << endl;
    cout << "  " << endl;
    cout << "  C++: class F { void operator()() {} };" << endl;
    cout << "       F f; f();" << endl;
    cout << "  " << endl;
    cout << "  Java: interface R { void run(); }" << endl;
    cout << "        R r = new R() { void run() {} };" << endl;
    cout << "        r.run();" << endl;
    
    cout << "\n八、现代 C++ 的选择：" << endl;
    cout << "  C++11 之前：函数对象是主力" << endl;
    cout << "  C++11 之后：Lambda 更常用（更简洁）" << endl;
    cout << "  " << endl;
    cout << "  何时用函数对象？" << endl;
    cout << "    ✅ 需要复用的复杂逻辑" << endl;
    cout << "    ✅ 需要维护状态" << endl;
    cout << "    ✅ 需要多个方法" << endl;
    cout << "  " << endl;
    cout << "  何时用 Lambda？" << endl;
    cout << "    ✅ 简单的一次性逻辑" << endl;
    cout << "    ✅ 代码简洁性优先" << endl;
    
    cout << "\n九、记忆口诀：" << endl;
    cout << "  函数对象很特别，重载括号可调用" << endl;
    cout << "  本质还是普通类，成员方法随意加" << endl;
    cout << "  operator 参数灵活，返回类型随心写" << endl;
    cout << "  状态保存是优势，Lambda 出现后少用" << endl;
    cout << "  Java 没有这概念，接口实现是替代" << endl;
    
    cout << "\n========================================" << endl;
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "\n╔═══════════════════════════════════════════════════╗" << endl;
    cout << "║         函数对象（Functor）详解                   ║" << endl;
    cout << "║   operator() 重载、与普通对象的区别              ║" << endl;
    cout << "╚═══════════════════════════════════════════════════╝" << endl;
    
    // 1. 什么是函数对象
    explain_functor();
    
    // 2. 函数对象 vs 普通对象
    compare_normal_vs_functor();
    
    // 3. operator() 可以有参数和返回值
    demonstrate_operator_overloading();
    
    // 4. 函数对象可以有状态
    demonstrate_stateful_functor();
    
    // 5. 函数对象可以有多个方法和成员
    demonstrate_complex_functor();
    
    // 6. 函数对象的实际应用
    demonstrate_functor_usage();
    
    // 7. 函数对象 vs Lambda vs 函数指针
    compare_three_approaches();
    
    // 8. 与 Java 的对比
    compare_with_java();
    
    // 9. 总结
    summary();
    
    return 0;
}
