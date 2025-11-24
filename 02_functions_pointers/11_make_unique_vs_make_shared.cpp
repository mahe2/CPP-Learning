/**
 * make_unique vs make_shared 区别详解
 * 
 * 核心区别：
 * 1. make_unique: 创建独占所有权的智能指针 (C++14)
 * 2. make_shared: 创建共享所有权的智能指针 (C++11)
 */

#include <iostream>
#include <memory>
#include <string>

using namespace std;

class Person {
public:
    string name;
    int age;
    
    Person(string n, int a) : name(n), age(a) {
        cout << "Person构造: " << name << ", age " << age << endl;
    }
    
    ~Person() {
        cout << "Person析构: " << name << endl;
    }
    
    void introduce() {
        cout << "我是 " << name << ", " << age << " 岁" << endl;
    }
};

// 演示 make_unique
void demo_make_unique() {
    cout << "\n=== make_unique 演示 ===" << endl;
    
    // 创建 unique_ptr - 独占所有权
    auto person1 = make_unique<Person>("张三", 25);
    person1->introduce();
    
    // unique_ptr 不能复制，只能移动
    // auto person2 = person1;  // ❌ 编译错误！unique_ptr不支持拷贝
    auto person2 = move(person1);  // ✅ 可以移动
    
    // person1 现在是空的
    if (person1 == nullptr) {
        cout << "person1 已经是空指针了（所有权已转移）" << endl;
    }
    
    person2->introduce();
    
    // 对比：Java中所有对象引用都像shared_ptr，可以随意复制
    // Kotlin中也是如此，没有"独占所有权"的概念
}

// 演示 make_shared
void demo_make_shared() {
    cout << "\n=== make_shared 演示 ===" << endl;
    
    // 创建 shared_ptr - 共享所有权
    auto person1 = make_shared<Person>("李四", 30);
    cout << "引用计数: " << person1.use_count() << endl;
    
    // shared_ptr 可以复制
    auto person2 = person1;  // ✅ 可以复制！
    cout << "引用计数: " << person1.use_count() << endl;
    
    {
        auto person3 = person1;
        cout << "引用计数: " << person1.use_count() << endl;
        person3->introduce();
    }  // person3 离开作用域，引用计数-1
    
    cout << "引用计数: " << person1.use_count() << endl;
    
    // person1 和 person2 都指向同一个对象
    // 类似于 Java/Kotlin 中的对象引用
}

// 内存分配差异
void memory_allocation_difference() {
    cout << "\n=== 内存分配差异 ===" << endl;
    
    // make_unique: 只分配对象本身
    // 内存布局: [Person对象]
    auto unique_person = make_unique<Person>("unique王五", 35);
    
    // make_shared: 分配对象 + 控制块（引用计数等信息）
    // 内存布局: [控制块(引用计数等) | Person对象]
    // 优点：一次分配，性能更好
    auto shared_person = make_shared<Person>("shared赵六", 40);
    
    cout << "\nmake_unique 内存: 堆上只有Person对象" << endl;
    cout << "make_shared 内存: 堆上有控制块+Person对象（一次分配）" << endl;
}

// 为什么推荐使用 make_xxx 而不是直接 new?
void why_make_functions() {
    cout << "\n=== 为什么使用 make_unique/make_shared? ===" << endl;
    
    // 方式1: 不推荐 - 使用 new
    // unique_ptr<Person> p1(new Person("方式1", 20));
    
    // 方式2: 推荐 - 使用 make_unique
    auto p2 = make_unique<Person>("方式2", 20);
    
    cout << "\n优点:" << endl;
    cout << "1. 异常安全 - 避免内存泄漏" << endl;
    cout << "2. 代码更简洁 - 不用写 new" << endl;
    cout << "3. 性能更好 - make_shared 一次分配内存" << endl;
    cout << "4. 避免重复类型 - auto 自动推导" << endl;
}

// 使用场景对比
void usage_scenarios() {
    cout << "\n=== 使用场景对比 ===" << endl;
    
    cout << "\n使用 make_unique 的场景:" << endl;
    cout << "✅ 对象只有一个所有者" << endl;
    cout << "✅ 需要明确所有权转移（移动语义）" << endl;
    cout << "✅ 工厂函数返回值" << endl;
    cout << "✅ PIMPL 模式" << endl;
    
    cout << "\n使用 make_shared 的场景:" << endl;
    cout << "✅ 多个对象需要共享同一资源" << endl;
    cout << "✅ 容器中存储指针（多个容器共享）" << endl;
    cout << "✅ 回调函数中需要保持对象存活" << endl;
    cout << "✅ 观察者模式、缓存等" << endl;
}

// 性能对比
void performance_comparison() {
    cout << "\n=== 性能对比 ===" << endl;
    
    cout << "\nmake_unique:" << endl;
    cout << "- 内存分配: 1次（只分配对象）" << endl;
    cout << "- 开销: 极小" << endl;
    cout << "- 线程安全: 不需要（独占所有权）" << endl;
    
    cout << "\nmake_shared:" << endl;
    cout << "- 内存分配: 1次（对象+控制块一起）" << endl;
    cout << "- 开销: 略大（维护引用计数）" << endl;
    cout << "- 线程安全: 引用计数是线程安全的（原子操作）" << endl;
    
    cout << "\n直接 new + unique_ptr:" << endl;
    cout << "- 内存分配: 1次" << endl;
    cout << "- 问题: 异常不安全！" << endl;
    
    cout << "\n直接 new + shared_ptr:" << endl;
    cout << "- 内存分配: 2次（对象1次，控制块1次）" << endl;
    cout << "- 问题: 性能差，不推荐！" << endl;
}

// 与 Java/Kotlin 对比
void compare_with_java_kotlin() {
    cout << "\n=== 与 Java/Kotlin 对比 ===" << endl;
    
    cout << "\nJava/Kotlin 的对象引用:" << endl;
    cout << "Person p1 = new Person(\"张三\", 25);" << endl;
    cout << "Person p2 = p1;  // 可以随意复制引用" << endl;
    cout << "// p1 和 p2 指向同一对象，由GC管理" << endl;
    cout << "// 最接近 C++ 的 shared_ptr" << endl;
    
    cout << "\nC++ unique_ptr (Java/Kotlin中没有对应概念):" << endl;
    cout << "auto p1 = make_unique<Person>(\"张三\", 25);" << endl;
    cout << "// auto p2 = p1;  // ❌ 不能复制！" << endl;
    cout << "auto p2 = move(p1);  // ✅ 只能移动" << endl;
    cout << "// p1 变成 nullptr，所有权转移到 p2" << endl;
    
    cout << "\nC++ shared_ptr (类似 Java/Kotlin 引用):" << endl;
    cout << "auto p1 = make_shared<Person>(\"李四\", 30);" << endl;
    cout << "auto p2 = p1;  // ✅ 可以复制" << endl;
    cout << "// p1 和 p2 指向同一对象，引用计数=2" << endl;
    cout << "// 类似 Java/Kotlin，但需要手动管理引用计数" << endl;
}

int main() {
    cout << "===========================================" << endl;
    cout << "  make_unique vs make_shared 完全指南" << endl;
    cout << "===========================================" << endl;
    
    demo_make_unique();
    demo_make_shared();
    memory_allocation_difference();
    why_make_functions();
    usage_scenarios();
    performance_comparison();
    compare_with_java_kotlin();
    
    cout << "\n===========================================" << endl;
    cout << "核心总结:" << endl;
    cout << "===========================================" << endl;
    cout << "1. make_unique: 独占所有权，不能复制，只能移动" << endl;
    cout << "   - 类似 Rust 的所有权系统" << endl;
    cout << "   - Java/Kotlin 中没有对应概念" << endl;
    cout << "\n2. make_shared: 共享所有权，可以复制" << endl;
    cout << "   - 类似 Java/Kotlin 的对象引用" << endl;
    cout << "   - 但使用引用计数而非GC" << endl;
    cout << "\n3. 性能: make_shared 一次分配更高效" << endl;
    cout << "   但有引用计数开销" << endl;
    cout << "\n4. 默认建议: 优先 unique_ptr，需要共享才用 shared_ptr" << endl;
    cout << "   - 明确所有权，减少意外共享" << endl;
    cout << "===========================================" << endl;
    
    return 0;
}

/**
 * 快速参考表：
 * 
 * ┌──────────────┬────────────────┬────────────────┬─────────────────┐
 * │   特性       │  make_unique   │  make_shared   │  Java/Kotlin    │
 * ├──────────────┼────────────────┼────────────────┼─────────────────┤
 * │ 所有权       │  独占           │  共享          │  共享(GC管理)   │
 * │ 可否复制     │  ❌ 不可        │  ✅ 可以       │  ✅ 可以        │
 * │ 可否移动     │  ✅ 可以        │  ✅ 可以       │  N/A            │
 * │ 内存分配     │  1次(对象)      │  1次(对象+控制)│  1次(GC管理)    │
 * │ 引用计数     │  无            │  有            │  GC跟踪         │
 * │ 线程安全     │  N/A           │  计数线程安全   │  GC线程安全     │
 * │ 开销         │  极小          │  略大          │  GC开销         │
 * │ C++版本      │  C++14         │  C++11         │  N/A            │
 * └──────────────┴────────────────┴────────────────┴─────────────────┘
 * 
 * 选择指南:
 * 1. 默认使用 unique_ptr (make_unique)
 *    - 明确所有权，性能最优
 *    - 需要共享时再改为 shared_ptr
 * 
 * 2. 需要共享时使用 shared_ptr (make_shared)
 *    - 多个对象需要访问同一资源
 *    - 但要注意循环引用问题（使用 weak_ptr 解决）
 * 
 * 3. 与 Java/Kotlin 的思维转换:
 *    - Java/Kotlin: 所有对象都是"共享"的（GC管理）
 *    - C++: 默认"独占"，需要时才"共享"
 *    - 这是 C++ 零开销抽象哲学的体现
 */
