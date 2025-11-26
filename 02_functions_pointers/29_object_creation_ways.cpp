/**
 * C++ 创建对象的多种方式详解
 * 
 * Java 只有一种方式：new Object()（在堆上）
 * C++ 有 6 种主要方式，非常灵活但也复杂！
 * 
 * 核心区别：
 * - Java: 对象总是在堆上，自动 GC
 * - C++: 可以在栈上或堆上，手动或自动管理
 */

#include <iostream>
#include <memory>
#include <vector>
#include <string>

using namespace std;

// ============================================
// 测试类：观察对象创建和销毁
// ============================================

class Person {
private:
    string name;
    int age;
    
public:
    // 默认构造函数
    Person() : name("Unknown"), age(0) {
        cout << "  [构造] 默认构造: " << name << endl;
    }
    
    // 参数构造函数
    Person(const string& n, int a) : name(n), age(a) {
        cout << "  [构造] 参数构造: " << name << ", age=" << age << endl;
    }
    
    // 拷贝构造函数
    Person(const Person& other) : name(other.name), age(other.age) {
        cout << "  [构造] 拷贝构造: " << name << " (从 " << &other << ")" << endl;
    }
    
    // 移动构造函数（C++11）
    Person(Person&& other) noexcept : name(move(other.name)), age(other.age) {
        cout << "  [构造] 移动构造: " << name << endl;
        other.age = 0;
    }
    
    // 析构函数
    ~Person() {
        cout << "  [析构] 销毁: " << name << ", age=" << age << endl;
    }
    
    void display() const {
        cout << "    Person(" << name << ", " << age << ")" << endl;
    }
    
    string getName() const { return name; }
    int getAge() const { return age; }
};

// ============================================
// 方式 1: 栈上创建（Java 没有！）
// ============================================

void method1_stack_object() {
    cout << "\n" << string(70, '=') << endl;
    cout << "方式 1: 栈上创建对象（自动存储）" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【C++ 栈对象 vs Java 没有对应概念】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Java（只有一种方式）:
  Person p = new Person("Alice", 25);  // 总是在堆上
  // p 是引用，指向堆上的对象
  // 垃圾回收器负责释放

C++（栈对象）:
  Person p("Alice", 25);  // 在栈上！
  // p 是真正的对象，不是指针/引用
  // 函数结束自动销毁（调用析构函数）

关键区别：
  - Java: 对象总在堆，需要 GC
  - C++: 对象可以在栈，自动管理（RAII）
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
    
    cout << "创建栈对象：" << endl;
    cout << "  Person p1(\"Alice\", 25);" << endl;
    Person p1("Alice", 25);
    
    cout << "\n  Person p2(\"Bob\", 30);" << endl;
    Person p2("Bob", 30);
    
    cout << "\n显示对象：" << endl;
    p1.display();
    p2.display();
    
    cout << "\n函数结束，栈对象自动销毁（RAII）：" << endl;
    // 自动调用析构函数，无需 delete
}

// ============================================
// 方式 2: 堆上创建（原始指针，类似 Java new）
// ============================================

void method2_heap_raw_pointer() {
    cout << "\n" << string(70, '=') << endl;
    cout << "方式 2: 堆上创建对象（原始指针）" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【C++ new vs Java new】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Java:
  Person p = new Person("Alice", 25);
  // 自动 GC，不需要手动释放

C++（原始指针，老式方式）:
  Person* p = new Person("Alice", 25);
  delete p;  // ❌ 必须手动释放！

问题：
  - 容易忘记 delete → 内存泄漏
  - 异常安全问题
  - 所有权不明确

现代 C++：❌ 避免使用原始指针！
应该用：✅ 智能指针（unique_ptr, shared_ptr）
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
    
    cout << "创建堆对象（原始指针）：" << endl;
    cout << "  Person* p1 = new Person(\"Charlie\", 28);" << endl;
    Person* p1 = new Person("Charlie", 28);
    
    cout << "\n访问成员：" << endl;
    cout << "  p1->display();" << endl;
    p1->display();
    
    cout << "\n手动释放内存：" << endl;
    cout << "  delete p1;  // 必须！" << endl;
    delete p1;  // 调用析构函数并释放内存
    
    cout << "\n⚠️  如果忘记 delete，就会内存泄漏！" << endl;
    cout << "⚠️  现代 C++ 应该避免原始指针！" << endl;
}

// ============================================
// 方式 3: unique_ptr（独占所有权）
// ============================================

void method3_unique_ptr() {
    cout << "\n" << string(70, '=') << endl;
    cout << "方式 3: unique_ptr（独占所有权智能指针）" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【unique_ptr - 独占所有权】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Java 没有完全对应的概念（GC 自动管理）

C++ unique_ptr:
  unique_ptr<Person> p = make_unique<Person>("David", 32);
  // 自动管理内存
  // 离开作用域自动 delete
  // 不能拷贝，只能移动（独占所有权）

特点：
  ✅ 零开销（和原始指针一样快）
  ✅ 异常安全
  ✅ 所有权明确（独占）
  ❌ 不能拷贝（只能移动）

适用场景：
  - 单一所有者
  - 函数返回对象
  - 容器存储对象
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
    
    cout << "创建 unique_ptr：" << endl;
    cout << "  auto p1 = make_unique<Person>(\"David\", 32);" << endl;
    auto p1 = make_unique<Person>("David", 32);
    
    cout << "\n访问成员：" << endl;
    p1->display();
    
    cout << "\n尝试拷贝：" << endl;
    cout << "  // auto p2 = p1;  // ❌ 编译错误！不能拷贝" << endl;
    // auto p2 = p1;  // 编译错误
    
    cout << "\n移动所有权：" << endl;
    cout << "  auto p2 = move(p1);  // ✅ 移动所有权" << endl;
    auto p2 = move(p1);  // p1 变为 nullptr
    
    cout << "  p1 现在是 nullptr: " << (p1 == nullptr ? "是" : "否") << endl;
    cout << "  p2 拥有对象: " << (p2 != nullptr ? "是" : "否") << endl;
    
    cout << "\n函数结束，unique_ptr 自动释放：" << endl;
}

// ============================================
// 方式 4: shared_ptr（共享所有权）
// ============================================

void method4_shared_ptr() {
    cout << "\n" << string(70, '=') << endl;
    cout << "方式 4: shared_ptr（共享所有权智能指针）" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【shared_ptr - 共享所有权】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
类似 Java 的对象引用（但需要手动使用 shared_ptr）

C++ shared_ptr:
  shared_ptr<Person> p1 = make_shared<Person>("Eve", 27);
  shared_ptr<Person> p2 = p1;  // 共享所有权
  // 引用计数：2
  // 最后一个 shared_ptr 销毁时，对象才释放

特点：
  ✅ 可以拷贝（共享所有权）
  ✅ 自动管理内存（引用计数）
  ✅ 线程安全的引用计数
  ❌ 有开销（引用计数 + 控制块）
  ❌ 可能循环引用（需要 weak_ptr）

适用场景：
  - 多个所有者
  - 容器共享对象
  - 回调函数
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
    
    cout << "创建 shared_ptr：" << endl;
    cout << "  auto p1 = make_shared<Person>(\"Eve\", 27);" << endl;
    auto p1 = make_shared<Person>("Eve", 27);
    cout << "  引用计数: " << p1.use_count() << endl;
    
    cout << "\n拷贝 shared_ptr（共享所有权）：" << endl;
    cout << "  auto p2 = p1;  // 可以拷贝" << endl;
    auto p2 = p1;
    cout << "  p1 引用计数: " << p1.use_count() << endl;
    cout << "  p2 引用计数: " << p2.use_count() << endl;
    
    {
        cout << "\n内部作用域：" << endl;
        cout << "  auto p3 = p1;" << endl;
        auto p3 = p1;
        cout << "  引用计数: " << p1.use_count() << endl;
        cout << "\n  作用域结束，p3 销毁..." << endl;
    }
    
    cout << "  p1 引用计数: " << p1.use_count() << endl;
    
    cout << "\n函数结束，所有 shared_ptr 销毁，对象释放：" << endl;
}

// ============================================
// 方式 5: 拷贝构造（值语义）
// ============================================

void method5_copy() {
    cout << "\n" << string(70, '=') << endl;
    cout << "方式 5: 拷贝构造（值语义）" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【C++ 值语义 vs Java 引用语义】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Java（引用语义）:
  Person p1 = new Person("Frank", 35);
  Person p2 = p1;  // p2 和 p1 指向同一个对象
  p2.setAge(40);   // p1 也变成 40

C++（值语义，默认行为）:
  Person p1("Frank", 35);
  Person p2 = p1;  // 拷贝构造，创建新对象！
  // p2 是 p1 的完整副本
  // 修改 p2 不影响 p1

关键区别：
  - Java: 赋值拷贝引用（浅拷贝）
  - C++: 赋值拷贝对象（深拷贝）
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
    
    cout << "创建原始对象：" << endl;
    cout << "  Person p1(\"Frank\", 35);" << endl;
    Person p1("Frank", 35);
    
    cout << "\n拷贝构造：" << endl;
    cout << "  Person p2 = p1;  // 调用拷贝构造函数" << endl;
    Person p2 = p1;  // 拷贝构造
    
    cout << "\n它们是不同的对象：" << endl;
    cout << "  p1 地址: " << &p1 << endl;
    cout << "  p2 地址: " << &p2 << endl;
    
    cout << "\n函数结束，两个对象都会销毁：" << endl;
}

// ============================================
// 方式 6: 移动构造（C++11，性能优化）
// ============================================

Person createPerson(const string& name, int age) {
    cout << "  函数内创建对象..." << endl;
    return Person(name, age);  // 返回临时对象
}

void method6_move() {
    cout << "\n" << string(70, '=') << endl;
    cout << "方式 6: 移动构造（C++11 性能优化）" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【移动语义 - C++11 的重大创新】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Java 没有移动语义（不需要，因为是引用）

C++ 移动构造:
  Person p = createPerson("Grace", 29);
  // 不拷贝，直接"窃取"临时对象的资源
  // 临时对象变成空壳

为什么需要？
  - 避免不必要的拷贝（性能）
  - 大对象（如 vector）拷贝很慢
  - 资源所有权转移（如文件句柄）

拷贝 vs 移动：
  拷贝: 复制所有数据（慢）
  移动: 转移所有权（快）

编译器优化（RVO）:
  - 现代编译器可能直接在目标位置构造
  - 甚至不调用移动构造
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
    
    cout << "从函数返回对象（可能触发移动）：" << endl;
    cout << "  Person p = createPerson(\"Grace\", 29);" << endl;
    Person p = createPerson("Grace", 29);
    
    cout << "\n显示对象：" << endl;
    p.display();
    
    cout << "\n函数结束，对象销毁：" << endl;
}

// ============================================
// 方式 7: 就地构造（emplace，容器专用）
// ============================================

void method7_emplace() {
    cout << "\n" << string(70, '=') << endl;
    cout << "方式 7: 就地构造（emplace）" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
【emplace - 容器内直接构造】
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Java（ArrayList）:
  list.add(new Person("Harry", 31));  // 创建对象后添加

C++ push_back vs emplace_back:
  vector<Person> v;
  
  // 方式 1: push_back（先创建，后移动）
  v.push_back(Person("Harry", 31));
  
  // 方式 2: emplace_back（直接在容器内构造）
  v.emplace_back("Harry", 31);  // 更高效！

emplace 的优势：
  ✅ 避免临时对象
  ✅ 避免移动/拷贝
  ✅ 直接在最终位置构造

现代 C++ 最佳实践：
  - 容器添加元素用 emplace_back
  - map 插入用 emplace
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
)" << endl;
    
    cout << "创建 vector：" << endl;
    vector<Person> people;
    
    cout << "\n使用 push_back（创建临时对象）：" << endl;
    cout << "  people.push_back(Person(\"Harry\", 31));" << endl;
    people.push_back(Person("Harry", 31));
    
    cout << "\n使用 emplace_back（直接构造）：" << endl;
    cout << "  people.emplace_back(\"Iris\", 26);  // 更高效！" << endl;
    people.emplace_back("Iris", 26);
    
    cout << "\n显示所有人：" << endl;
    for (const auto& p : people) {
        p.display();
    }
    
    cout << "\n函数结束，vector 销毁，所有对象销毁：" << endl;
}

// ============================================
// 综合对比表
// ============================================

void comparison_table() {
    cout << "\n" << string(70, '=') << endl;
    cout << "C++ vs Java 对象创建对比" << endl;
    cout << string(70, '=') << endl;
    
    cout << "\n" << string(110, '-') << endl;
    cout << "方式                创建方式                      内存位置    生命周期管理    性能      Java 对应" << endl;
    cout << string(110, '-') << endl;
    cout << "1. 栈对象          Person p(\"A\",25)             栈         自动（RAII）    最快      ❌ 无" << endl;
    cout << "2. 原始指针        Person* p = new Person()      堆         手动 delete      快        类似 new" << endl;
    cout << "3. unique_ptr      make_unique<Person>()         堆         自动            快        ❌ 无" << endl;
    cout << "4. shared_ptr      make_shared<Person>()         堆         自动（引用计数） 较慢      类似引用" << endl;
    cout << "5. 拷贝构造        Person p2 = p1                栈/堆      自动            中等      ❌ 无（Java是引用）" << endl;
    cout << "6. 移动构造        Person p = move(other)        栈/堆      自动            快        ❌ 无" << endl;
    cout << "7. emplace         vec.emplace_back(\"A\",25)     堆         自动            最快      类似 add" << endl;
    cout << string(110, '-') << endl;
    
    cout << "\n【关键区别总结】" << endl;
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    cout << "\nJava:" << endl;
    cout << "  ✓ 只有一种方式：new（堆上）" << endl;
    cout << "  ✓ 对象总是引用语义" << endl;
    cout << "  ✓ 自动垃圾回收" << endl;
    cout << "  ✓ 简单，不容易出错" << endl;
    cout << "  ✗ 无法控制内存位置" << endl;
    cout << "  ✗ GC 暂停" << endl;
    
    cout << "\nC++:" << endl;
    cout << "  ✓ 7 种方式，灵活" << endl;
    cout << "  ✓ 可以选择栈或堆" << endl;
    cout << "  ✓ 值语义或指针语义" << endl;
    cout << "  ✓ RAII 自动管理（栈对象）" << endl;
    cout << "  ✓ 智能指针（堆对象）" << endl;
    cout << "  ✓ 性能可控" << endl;
    cout << "  ✗ 复杂，容易出错" << endl;
}

// ============================================
// 最佳实践
// ============================================

void best_practices() {
    cout << "\n" << string(70, '=') << endl;
    cout << "最佳实践和使用建议" << endl;
    cout << string(70, '=') << endl;
    
    cout << "\n【选择指南】" << endl;
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    
    cout << "\n1️⃣  优先使用栈对象（最简单、最快）：" << endl;
    cout << "  ✅ void func() {" << endl;
    cout << "      Person p(\"Alice\", 25);  // 栈对象，自动管理" << endl;
    cout << "      p.display();" << endl;
    cout << "  }  // 自动销毁，RAII" << endl;
    
    cout << "\n  何时使用：" << endl;
    cout << "    - 局部变量" << endl;
    cout << "    - 生命周期明确" << endl;
    cout << "    - 不需要动态分配" << endl;
    
    cout << "\n2️⃣  需要堆对象时，用 unique_ptr（独占所有权）：" << endl;
    cout << "  ✅ auto p = make_unique<Person>(\"Bob\", 30);" << endl;
    cout << "     return p;  // 可以返回，转移所有权" << endl;
    
    cout << "\n  何时使用：" << endl;
    cout << "    - 需要动态大小" << endl;
    cout << "    - 工厂函数返回" << endl;
    cout << "    - 多态（基类指针指向派生类）" << endl;
    
    cout << "\n3️⃣  多个所有者时，用 shared_ptr（共享所有权）：" << endl;
    cout << "  ✅ auto p = make_shared<Person>(\"Charlie\", 28);" << endl;
    cout << "     vec.push_back(p);  // 多个地方共享" << endl;
    cout << "     map[\"key\"] = p;   // 引用计数管理" << endl;
    
    cout << "\n  何时使用：" << endl;
    cout << "    - 多个容器共享对象" << endl;
    cout << "    - 回调函数" << endl;
    cout << "    - 观察者模式" << endl;
    
    cout << "\n4️⃣  容器中创建对象，用 emplace：" << endl;
    cout << "  ✅ vector<Person> v;" << endl;
    cout << "     v.emplace_back(\"David\", 32);  // 直接构造，最高效" << endl;
    
    cout << "\n5️⃣  ❌ 避免原始指针（除非特殊场景）：" << endl;
    cout << "  ❌ Person* p = new Person();  // 容易忘记 delete" << endl;
    cout << "  ❌ delete p;  // 容易遗漏" << endl;
    
    cout << "\n【从 Java 到 C++ 的思维转变】" << endl;
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    
    cout << "\nJava 思维：" << endl;
    cout << "  Person p = new Person();  // 总是这样" << endl;
    cout << "  // 忘掉它，让 GC 处理" << endl;
    
    cout << "\nC++ 思维（需要考虑）：" << endl;
    cout << "  1. 对象在栈还是堆？" << endl;
    cout << "     - 栈：简单、快速、自动管理" << endl;
    cout << "     - 堆：灵活、动态、需要智能指针" << endl;
    cout << "\n  2. 所有权关系？" << endl;
    cout << "     - 独占：unique_ptr" << endl;
    cout << "     - 共享：shared_ptr" << endl;
    cout << "     - 观察：weak_ptr" << endl;
    cout << "\n  3. 拷贝还是移动？" << endl;
    cout << "     - 需要副本：拷贝" << endl;
    cout << "     - 转移所有权：移动" << endl;
    
    cout << "\n【性能考虑】" << endl;
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    cout << "性能排序（从快到慢）：" << endl;
    cout << "  1. 栈对象                  - 最快（无堆分配）" << endl;
    cout << "  2. unique_ptr + emplace    - 很快（零开销抽象）" << endl;
    cout << "  3. 移动构造                - 快（转移所有权）" << endl;
    cout << "  4. shared_ptr              - 较慢（引用计数开销）" << endl;
    cout << "  5. 拷贝构造                - 慢（复制数据）" << endl;
}

// ============================================
// 实际例子：学生管理系统
// ============================================

void real_world_example() {
    cout << "\n" << string(70, '=') << endl;
    cout << "实际例子：如何选择创建方式" << endl;
    cout << string(70, '=') << endl;
    
    cout << "\n【场景 1：临时计算（栈对象）】" << endl;
    cout << "代码：" << endl;
    cout << "  void calculateAverage() {" << endl;
    cout << "      Person temp(\"Temp\", 0);  // 栈对象" << endl;
    cout << "      // 使用 temp..." << endl;
    cout << "  }  // 自动销毁" << endl;
    
    cout << "\n【场景 2：容器存储（智能指针）】" << endl;
    cout << "代码：" << endl;
    cout << "  class StudentManager {" << endl;
    cout << "      vector<shared_ptr<Person>> students;  // 共享所有权" << endl;
    cout << "      " << endl;
    cout << "      void addStudent(const string& name, int age) {" << endl;
    cout << "          students.push_back(make_shared<Person>(name, age));" << endl;
    cout << "      }" << endl;
    cout << "  };" << endl;
    
    cout << "\n【场景 3：工厂函数（unique_ptr）】" << endl;
    cout << "代码：" << endl;
    cout << "  unique_ptr<Person> createPerson(int type) {" << endl;
    cout << "      if (type == 1)" << endl;
    cout << "          return make_unique<Student>();  // 派生类" << endl;
    cout << "      else" << endl;
    cout << "          return make_unique<Teacher>();  // 多态" << endl;
    cout << "  }" << endl;
    
    cout << "\n【场景 4：值类型（栈对象 + 拷贝）】" << endl;
    cout << "代码：" << endl;
    cout << "  struct Point { int x, y; };  // 小对象，值语义" << endl;
    cout << "  " << endl;
    cout << "  Point p1{10, 20};" << endl;
    cout << "  Point p2 = p1;  // 拷贝，符合直觉" << endl;
}

// ============================================
// Main
// ============================================

int main() {
    cout << "======================================================================" << endl;
    cout << "  C++ 创建对象的 7 种方式" << endl;
    cout << "  （与 Java 的重大区别）" << endl;
    cout << "======================================================================" << endl;
    
    method1_stack_object();
    method2_heap_raw_pointer();
    method3_unique_ptr();
    method4_shared_ptr();
    method5_copy();
    method6_move();
    method7_emplace();
    
    comparison_table();
    best_practices();
    real_world_example();
    
    cout << "\n======================================================================" << endl;
    cout << "核心总结" << endl;
    cout << "======================================================================" << endl;
    
    cout << "\n【Java vs C++ 对象创建】" << endl;
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    
    cout << "\nJava（简单）：" << endl;
    cout << "  Person p = new Person();  // 唯一方式" << endl;
    cout << "  - 总是在堆上" << endl;
    cout << "  - 总是引用语义" << endl;
    cout << "  - GC 自动管理" << endl;
    
    cout << "\nC++（灵活但复杂）：" << endl;
    cout << "  1. Person p(...);             // 栈对象（最常用）" << endl;
    cout << "  2. Person* p = new ...;       // 原始指针（避免）" << endl;
    cout << "  3. make_unique<Person>(...);  // 独占所有权" << endl;
    cout << "  4. make_shared<Person>(...);  // 共享所有权" << endl;
    cout << "  5. Person p2 = p1;            // 拷贝（值语义）" << endl;
    cout << "  6. Person p = move(other);    // 移动（性能）" << endl;
    cout << "  7. vec.emplace_back(...);     // 就地构造" << endl;
    
    cout << "\n【记忆要点】" << endl;
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    cout << "优先级：" << endl;
    cout << "  1️⃣  栈对象（最简单、最快）" << endl;
    cout << "  2️⃣  unique_ptr（独占所有权）" << endl;
    cout << "  3️⃣  shared_ptr（共享所有权）" << endl;
    cout << "  4️⃣  emplace（容器内构造）" << endl;
    cout << "  ❌ 避免原始指针" << endl;
    
    cout << "\n关键区别：" << endl;
    cout << "  - Java: 对象总在堆，GC 管理" << endl;
    cout << "  - C++: 栈（RAII）或堆（智能指针）" << endl;
    cout << "  - Java: 引用语义（赋值拷贝引用）" << endl;
    cout << "  - C++: 值语义（赋值拷贝对象）" << endl;
    
    cout << "\n从 Java 到 C++：" << endl;
    cout << "  - 局部变量：用栈对象，不用 new" << endl;
    cout << "  - 容器存储：用 shared_ptr" << endl;
    cout << "  - 返回对象：用 unique_ptr 或直接返回" << endl;
    cout << "  - 忘掉 new/delete：用智能指针" << endl;
    
    cout << "======================================================================" << endl;
    
    return 0;
}

/**
 * 终极总结：
 * 
 * Java: 简单但不灵活
 *   - 只有一种方式：new（堆上）
 *   - 自动 GC
 *   - 引用语义
 * 
 * C++: 灵活但复杂
 *   - 7 种创建方式
 *   - 栈（RAII）+ 堆（智能指针）
 *   - 值语义 + 指针语义
 *   - 完全控制生命周期和性能
 * 
 * 最佳实践：
 *   1. 优先栈对象
 *   2. 需要堆用 unique_ptr
 *   3. 共享用 shared_ptr
 *   4. 避免原始指针
 */
