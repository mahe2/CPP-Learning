/**
 * C++ 指针：* 和 -> 的使用场景详解
 * 
 * 核心规则：
 * * 用于解引用（获取指针指向的值）
 * -> 用于访问指针指向对象的成员
 */

#include <iostream>
#include <string>
#include <memory>

using namespace std;

class Student {
public:
    string name;
    int age;
    
    Student(string n, int a) : name(n), age(a) {}
    
    void display() {
        cout << "姓名: " << name << ", 年龄: " << age << endl;
    }
    
    string getName() const { return name; }
};

// ============================================
// 基础规则演示
// ============================================
void basic_rules() {
    cout << "\n" << string(60, '=') << endl;
    cout << "基础规则" << endl;
    cout << string(60, '=') << endl;
    
    // 普通变量
    int x = 100;
    int* ptr = &x;  // ptr 是指向 int 的指针
    
    cout << "\n1. 基本类型指针" << endl;
    cout << "---------------------------------------" << endl;
    cout << "int x = 100;" << endl;
    cout << "int* ptr = &x;" << endl;
    cout << "\nptr     → 指针本身（地址）: " << ptr << endl;
    cout << "*ptr    → 解引用（获取值）: " << *ptr << endl;
    cout << "\n使用 * 解引用获取值" << endl;
    
    // 修改值
    *ptr = 200;
    cout << "\n*ptr = 200;  // 通过指针修改值" << endl;
    cout << "x 现在 = " << x << endl;
    
    cout << "\n" << string(60, '-') << endl;
    
    // 对象指针
    Student s("张三", 20);
    Student* sptr = &s;
    
    cout << "\n2. 对象指针" << endl;
    cout << "---------------------------------------" << endl;
    cout << "Student s(\"张三\", 20);" << endl;
    cout << "Student* sptr = &s;" << endl;
    cout << "\nsptr       → 指针本身（地址）: " << sptr << endl;
    cout << "*sptr      → 解引用（获取对象）" << endl;
    cout << "sptr->name → 访问成员: " << sptr->name << endl;
    cout << "sptr->age  → 访问成员: " << sptr->age << endl;
}

// ============================================
// * 的三种用法
// ============================================
void star_operator_uses() {
    cout << "\n" << string(60, '=') << endl;
    cout << "* 号的三种用法" << endl;
    cout << string(60, '=') << endl;
    
    int x = 100;
    Student s("李四", 25);
    
    cout << "\n用法1: 声明指针类型" << endl;
    cout << "---------------------------------------" << endl;
    cout << "int* ptr;           // ptr 是指向 int 的指针" << endl;
    cout << "Student* sptr;      // sptr 是指向 Student 的指针" << endl;
    cout << "char* str;          // str 是指向 char 的指针" << endl;
    int* ptr;
    Student* sptr;
    
    cout << "\n用法2: 解引用（获取指针指向的值）" << endl;
    cout << "---------------------------------------" << endl;
    ptr = &x;
    cout << "ptr = &x;" << endl;
    cout << "*ptr = " << *ptr << "  // 获取 x 的值" << endl;
    
    cout << "\n用法3: 解引用对象指针（获取整个对象）" << endl;
    cout << "---------------------------------------" << endl;
    sptr = &s;
    cout << "sptr = &s;" << endl;
    cout << "(*sptr).name = " << (*sptr).name << "  // 获取对象，再访问成员" << endl;
    cout << "(*sptr).age = " << (*sptr).age << endl;
    cout << "\n注意：需要括号！因为 . 优先级高于 *" << endl;
}

// ============================================
// -> 的用法
// ============================================
void arrow_operator_uses() {
    cout << "\n" << string(60, '=') << endl;
    cout << "-> 的用法" << endl;
    cout << string(60, '=') << endl;
    
    Student s("王五", 30);
    Student* ptr = &s;
    
    cout << "\n-> 是什么？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "-> 是 (*ptr). 的简写" << endl;
    cout << "专门用于通过指针访问对象成员" << endl;
    
    cout << "\n两种等价写法：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "写法1（繁琐）: (*ptr).name = " << (*ptr).name << endl;
    cout << "写法2（简洁）: ptr->name    = " << ptr->name << endl;
    cout << "\n✅ 推荐使用 ptr->name（更简洁）" << endl;
    
    cout << "\n调用成员函数：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "(*ptr).display():  ";
    (*ptr).display();
    cout << "ptr->display():    ";
    ptr->display();
    cout << "\n✅ 推荐使用 ptr->display()（更简洁）" << endl;
}

// ============================================
// 使用场景对比
// ============================================
void usage_scenarios() {
    cout << "\n" << string(60, '=') << endl;
    cout << "使用场景对比" << endl;
    cout << string(60, '=') << endl;
    
    int x = 100;
    int* intPtr = &x;
    
    Student s("赵六", 35);
    Student* objPtr = &s;
    
    cout << "\n场景1: 基本类型指针 → 只用 *" << endl;
    cout << "---------------------------------------" << endl;
    cout << "int* ptr = &x;" << endl;
    cout << "*ptr              // 获取值: " << *intPtr << endl;
    cout << "*ptr = 200;       // 修改值" << endl;
    *intPtr = 200;
    cout << "x 现在 = " << x << endl;
    cout << "\n基本类型没有成员，不能用 ->" << endl;
    
    cout << "\n场景2: 对象指针 → * 和 -> 都可以" << endl;
    cout << "---------------------------------------" << endl;
    cout << "Student* ptr = &s;" << endl;
    cout << "\n访问成员变量：" << endl;
    cout << "  (*ptr).name  = " << (*objPtr).name << "  // 繁琐" << endl;
    cout << "  ptr->name    = " << objPtr->name << "  // 简洁 ✅" << endl;
    cout << "\n调用成员函数：" << endl;
    cout << "  (*ptr).display()  // 繁琐" << endl;
    cout << "  ptr->display()    // 简洁 ✅" << endl;
    
    cout << "\n场景3: 获取整个对象 → 用 *" << endl;
    cout << "---------------------------------------" << endl;
    cout << "Student s2 = *ptr;  // 复制整个对象" << endl;
    Student s2 = *objPtr;
    cout << "s2.name = " << s2.name << endl;
    cout << "\n这里 * 获取了整个对象（会复制）" << endl;
}

// ============================================
// 智能指针的特殊情况
// ============================================
void smart_pointer_scenarios() {
    cout << "\n" << string(60, '=') << endl;
    cout << "智能指针的使用" << endl;
    cout << string(60, '=') << endl;
    
    auto ptr = make_shared<Student>("孙七", 40);
    
    cout << "\nshared_ptr 的用法：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "auto ptr = make_shared<Student>(\"孙七\", 40);" << endl;
    cout << "\nptr         → 智能指针本身" << endl;
    cout << "ptr->name   → 访问成员: " << ptr->name << endl;
    cout << "*ptr        → 解引用得到对象" << endl;
    cout << "(*ptr).name → 访问成员: " << (*ptr).name << endl;
    
    cout << "\n✅ 推荐：ptr->name（简洁）" << endl;
    cout << "⚠️  少用：(*ptr).name（繁琐）" << endl;
    
    cout << "\n调用方法：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "ptr->display():  ";
    ptr->display();
    cout << "(*ptr).display():  ";
    (*ptr).display();
    cout << "\n✅ 推荐：ptr->display()（简洁）" << endl;
}

// ============================================
// 迭代器的使用（综合案例）
// ============================================
void iterator_scenarios() {
    cout << "\n" << string(60, '=') << endl;
    cout << "迭代器场景（综合应用）" << endl;
    cout << string(60, '=') << endl;
    
    vector<shared_ptr<Student>> students;
    students.push_back(make_shared<Student>("张三", 20));
    students.push_back(make_shared<Student>("李四", 25));
    
    auto it = students.begin();
    
    cout << "\nvector<shared_ptr<Student>> students;" << endl;
    cout << "auto it = students.begin();" << endl;
    cout << "\n数据结构：" << endl;
    cout << "it       → 迭代器（指向 vector 元素）" << endl;
    cout << "*it      → shared_ptr<Student>（解引用迭代器）" << endl;
    cout << "(*it)->  → 通过智能指针访问对象" << endl;
    
    cout << "\n访问成员：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "(*it)->name  = " << (*it)->name << endl;
    cout << "(*it)->age   = " << (*it)->age << endl;
    
    cout << "\n为什么是 (*it)-> ？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. it 是迭代器" << endl;
    cout << "2. *it 解引用得到 shared_ptr<Student>" << endl;
    cout << "3. (*it)-> 通过智能指针访问 Student 对象" << endl;
    cout << "4. 括号必须，因为 -> 优先级高于 *" << endl;
}

// ============================================
// 常见错误
// ============================================
void common_mistakes() {
    cout << "\n" << string(60, '=') << endl;
    cout << "常见错误" << endl;
    cout << string(60, '=') << endl;
    
    Student s("错误示例", 99);
    Student* ptr = &s;
    
    cout << "\n错误1: 对基本类型用 ->" << endl;
    cout << "---------------------------------------" << endl;
    cout << "int* ptr = &x;" << endl;
    cout << "ptr->something  ❌ 错误！int 没有成员" << endl;
    cout << "*ptr            ✅ 正确！获取 int 值" << endl;
    
    cout << "\n错误2: 忘记括号" << endl;
    cout << "---------------------------------------" << endl;
    cout << "*ptr.name       ❌ 错误！优先级问题" << endl;
    cout << "(*ptr).name     ✅ 正确！" << endl;
    cout << "ptr->name       ✅ 更好！" << endl;
    
    cout << "\n错误3: 混淆对象和指针" << endl;
    cout << "---------------------------------------" << endl;
    cout << "Student s;      // 对象" << endl;
    cout << "Student* ptr;   // 指针" << endl;
    cout << "\ns.name          ✅ 对象用 ." << endl;
    cout << "ptr->name       ✅ 指针用 ->" << endl;
    cout << "\ns->name         ❌ 错误！s 不是指针" << endl;
    cout << "ptr.name        ❌ 错误！ptr 是指针" << endl;
}

// ============================================
// 决策树
// ============================================
void decision_tree() {
    cout << "\n" << string(60, '=') << endl;
    cout << "如何选择 * 还是 -> ?" << endl;
    cout << string(60, '=') << endl;
    
    cout << R"(
决策树：
┌─────────────────────────────────────────────────┐
│ 我有一个指针 ptr                                 │
└─────────────────┬───────────────────────────────┘
                  │
         ┌────────┴────────┐
         │                 │
    是基本类型？       是对象类型？
    (int*, double*)    (Student*, string*)
         │                 │
         │                 │
    只能用 *            ┌──┴──┐
         │              │     │
    例如：*ptr     要访问成员？ 要整个对象？
                       │         │
                       │         │
                   用 ->     用 *
                       │         │
                ptr->name   *ptr
                ptr->age    Student s = *ptr
                ptr->func()

特殊情况：智能指针
shared_ptr<Student> ptr
    ├─ ptr->name   ✅ 推荐（访问成员）
    ├─ (*ptr).name ✅ 可以但繁琐
    └─ *ptr        ✅ 获取对象引用
)" << endl;
}

// ============================================
// 与 Java/Kotlin 对比
// ============================================
void compare_with_java_kotlin() {
    cout << "\n" << string(60, '=') << endl;
    cout << "与 Java/Kotlin 对比" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\nJava：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "Student s = new Student(\"张三\", 20);" << endl;
    cout << "s.name         // 永远用 ." << endl;
    cout << "s.display()    // 永远用 ." << endl;
    cout << "\n→ Java 的对象变量都是引用（类似指针）" << endl;
    cout << "→ 但语法上统一用 ." << endl;
    cout << "→ 没有显式的指针概念" << endl;
    
    cout << "\nKotlin：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "val s = Student(\"张三\", 20)" << endl;
    cout << "s.name         // 永远用 ." << endl;
    cout << "s.display()    // 永远用 ." << endl;
    cout << "\n→ 和 Java 一样，统一用 ." << endl;
    
    cout << "\nC++：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "// 情况1: 对象" << endl;
    cout << "Student s(\"张三\", 20);" << endl;
    cout << "s.name         // 对象用 ." << endl;
    cout << "s.display()    // 对象用 ." << endl;
    cout << "\n// 情况2: 指针" << endl;
    cout << "Student* ptr = &s;" << endl;
    cout << "ptr->name      // 指针用 ->" << endl;
    cout << "ptr->display() // 指针用 ->" << endl;
    cout << "\n→ C++ 区分对象和指针" << endl;
    cout << "→ 对象用 . ，指针用 ->" << endl;
    cout << "→ 需要程序员明确知道是哪种" << endl;
}

// ============================================
// 完整对比表
// ============================================
void comparison_table() {
    cout << "\n" << string(60, '=') << endl;
    cout << "完整对比表" << endl;
    cout << string(60, '=') << endl;
    
    cout << R"(
┌───────────────────────┬──────────────┬──────────────────┐
│  场景                  │  使用 *      │  使用 ->         │
├───────────────────────┼──────────────┼──────────────────┤
│ 基本类型指针           │  ✅          │  ❌              │
│ int* ptr = &x;        │  *ptr        │  不适用          │
├───────────────────────┼──────────────┼──────────────────┤
│ 对象指针-访问成员      │  ⚠️          │  ✅ 推荐         │
│ Student* ptr = &s;    │  (*ptr).name │  ptr->name       │
├───────────────────────┼──────────────┼──────────────────┤
│ 对象指针-获取对象      │  ✅          │  ❌              │
│ Student* ptr = &s;    │  *ptr        │  不适用          │
├───────────────────────┼──────────────┼──────────────────┤
│ 智能指针-访问成员      │  ⚠️          │  ✅ 推荐         │
│ shared_ptr<T> ptr;    │  (*ptr).name │  ptr->name       │
├───────────────────────┼──────────────┼──────────────────┤
│ 迭代器-容器有指针      │  ✅          │  ✅              │
│ vector<shared_ptr<T>> │  *it         │  (*it)->member   │
└───────────────────────┴──────────────┴──────────────────┘

记忆规则：
1. * 用于解引用（获取值或对象）
2. -> 用于通过指针访问成员（简化版的 (*ptr).）
3. 对象用 . ，指针用 ->
4. 基本类型只能用 * ，对象两者都行但 -> 更简洁
)" << endl;
}

int main() {
    cout << "===========================================" << endl;
    cout << "  C++ 指针：* 和 -> 详解" << endl;
    cout << "===========================================" << endl;
    
    basic_rules();
    star_operator_uses();
    arrow_operator_uses();
    usage_scenarios();
    smart_pointer_scenarios();
    iterator_scenarios();
    common_mistakes();
    decision_tree();
    compare_with_java_kotlin();
    comparison_table();
    
    cout << "\n===========================================" << endl;
    cout << "总结" << endl;
    cout << "===========================================" << endl;
    cout << "\n核心规则：" << endl;
    cout << "1. * 用于解引用（获取指针指向的值/对象）" << endl;
    cout << "2. -> 用于通过指针访问对象成员" << endl;
    cout << "3. -> 等价于 (*ptr).，但更简洁" << endl;
    cout << "\n简单记忆：" << endl;
    cout << "  对象.成员      → 用 ." << endl;
    cout << "  指针->成员     → 用 ->" << endl;
    cout << "  *指针          → 解引用获取值" << endl;
    cout << "\n最佳实践：" << endl;
    cout << "  ptr->name      ✅ 推荐（简洁）" << endl;
    cout << "  (*ptr).name    ⚠️  可以但繁琐" << endl;
    cout << "===========================================" << endl;
    
    return 0;
}

/**
 * 快速参考卡片：
 * 
 * ┌─────────────────────────────────────┐
 * │  * 的用法                            │
 * ├─────────────────────────────────────┤
 * │ 1. 声明指针：int* ptr               │
 * │ 2. 解引用：*ptr（获取值）            │
 * │ 3. 获取对象：*objPtr（获取整个对象） │
 * └─────────────────────────────────────┘
 * 
 * ┌─────────────────────────────────────┐
 * │  -> 的用法                           │
 * ├─────────────────────────────────────┤
 * │ 1. 访问成员：ptr->name              │
 * │ 2. 调用方法：ptr->display()        │
 * │ 3. 等价于：(*ptr).name              │
 * └─────────────────────────────────────┘
 * 
 * 记忆口诀：
 * - 星号解引用，获取值
 * - 箭头访问，更简洁
 * - 对象点，指针箭
 */
