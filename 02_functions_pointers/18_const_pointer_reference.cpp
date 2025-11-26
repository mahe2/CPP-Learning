/**
 * C++ const 详解：const auto&、指针常量、常量指针
 * 
 * 核心区分：
 * 1. const auto& → const引用（不是指针）
 * 2. const int* → 常量指针（指向的值不能改）
 * 3. int* const → 指针常量（指针本身不能改）
 * 4. const int* const → 两者都不能改
 */

#include <iostream>
#include <string>
#include <memory>
#include <vector>

using namespace std;

class Student {
public:
    string name;
    int age;
    
    Student(string n, int a) : name(n), age(a) {}
    void display() const {
        cout << "姓名: " << name << ", 年龄: " << age << endl;
    }
};

// ============================================
// const auto& 详解
// ============================================
void const_auto_ref() {
    cout << "\n" << string(60, '=') << endl;
    cout << "const auto& 详解" << endl;
    cout << string(60, '=') << endl;
    
    vector<shared_ptr<Student>> students;
    students.push_back(make_shared<Student>("张三", 20));
    students.push_back(make_shared<Student>("李四", 25));
    
    cout << "\nfor (const auto& student : students)" << endl;
    cout << "---------------------------------------" << endl;
    
    for (const auto& student : students) {
        cout << "\nstudent 的类型: const shared_ptr<Student>&" << endl;
        cout << "       ^^^^^ ^^^^^^^^^^^^^^^^^^^ ^" << endl;
        cout << "         |            |           |" << endl;
        cout << "         |            |           引用（不是指针！）" << endl;
        cout << "         |            智能指针类型" << endl;
        cout << "         const 修饰整个智能指针" << endl;
        
        cout << "\n可以做什么？" << endl;
        cout << "✅ student->name（访问对象成员）: " << student->name << endl;
        cout << "✅ student->display()（调用方法）: ";
        student->display();
        
        cout << "\n不能做什么？" << endl;
        cout << "❌ student = other_ptr（不能让它指向其他对象）" << endl;
        cout << "❌ student.reset()（不能调用非const方法）" << endl;
        
        cout << "\n这不是指针常量！这是 const 引用！" << endl;
        break;  // 只演示一次
    }
}

// ============================================
// 指针常量 vs 常量指针
// ============================================
void pointer_const_comparison() {
    cout << "\n" << string(60, '=') << endl;
    cout << "指针常量 vs 常量指针" << endl;
    cout << string(60, '=') << endl;
    
    int x = 10;
    int y = 20;
    
    cout << "\n1. 常量指针（Pointer to Constant）" << endl;
    cout << "---------------------------------------" << endl;
    cout << "const int* ptr1 = &x;" << endl;
    cout << "或 int const* ptr1 = &x;" << endl;
    const int* ptr1 = &x;
    
    cout << "\nconst 修饰 *ptr（指向的值）" << endl;
    cout << "       ^^^^^ ^^^^" << endl;
    cout << "         |    |" << endl;
    cout << "         |    指向的值" << endl;
    cout << "         不能修改" << endl;
    
    cout << "\n可以做什么？" << endl;
    cout << "*ptr1 的值: " << *ptr1 << endl;
    cout << "✅ ptr1 = &y;（可以改变指向）" << endl;
    ptr1 = &y;
    cout << "现在 *ptr1 = " << *ptr1 << endl;
    
    cout << "\n不能做什么？" << endl;
    cout << "❌ *ptr1 = 100;（不能修改指向的值）" << endl;
    // *ptr1 = 100;  // 编译错误！
    
    cout << "\n记忆：const 在 * 左边 → 值不能改" << endl;
    
    cout << "\n" << string(60, '-') << endl;
    
    cout << "\n2. 指针常量（Constant Pointer）" << endl;
    cout << "---------------------------------------" << endl;
    cout << "int* const ptr2 = &x;" << endl;
    int* const ptr2 = &x;
    
    cout << "\nconst 修饰 ptr（指针本身）" << endl;
    cout << "       ^^^^ ^^^^" << endl;
    cout << "         |   |" << endl;
    cout << "         |   指针本身" << endl;
    cout << "         不能改变指向" << endl;
    
    cout << "\n可以做什么？" << endl;
    cout << "✅ *ptr2 = 100;（可以修改值）" << endl;
    *ptr2 = 100;
    cout << "现在 x = " << x << endl;
    
    cout << "\n不能做什么？" << endl;
    cout << "❌ ptr2 = &y;（不能改变指向）" << endl;
    // ptr2 = &y;  // 编译错误！
    
    cout << "\n记忆：const 在 * 右边 → 指针不能改" << endl;
    
    cout << "\n" << string(60, '-') << endl;
    
    cout << "\n3. 指向常量的常量指针" << endl;
    cout << "---------------------------------------" << endl;
    cout << "const int* const ptr3 = &x;" << endl;
    const int* const ptr3 = &x;
    
    cout << "\n两个 const：" << endl;
    cout << "const int* const ptr3" << endl;
    cout << "^^^^^ ^^^  ^^^^^ ^^^^" << endl;
    cout << "  |    |     |    |" << endl;
    cout << "  |    |     |    指针本身" << endl;
    cout << "  |    |     不能改变指向" << endl;
    cout << "  |    指向的值" << endl;
    cout << "  不能修改值" << endl;
    
    cout << "\n可以做什么？" << endl;
    cout << "✅ 读取 *ptr3: " << *ptr3 << endl;
    
    cout << "\n不能做什么？" << endl;
    cout << "❌ *ptr3 = 200;（不能修改值）" << endl;
    cout << "❌ ptr3 = &y;（不能改变指向）" << endl;
    
    cout << "\n记忆：两个 const → 啥都不能改" << endl;
}

// ============================================
// 记忆技巧：从右往左读
// ============================================
void memory_trick() {
    cout << "\n" << string(60, '=') << endl;
    cout << "记忆技巧：从右往左读" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n1. const int* p" << endl;
    cout << "---------------------------------------" << endl;
    cout << "从右往左读：" << endl;
    cout << "p → 是一个指针" << endl;
    cout << "* → 指向" << endl;
    cout << "int → 整数" << endl;
    cout << "const → 常量" << endl;
    cout << "\n结果：p 是指向常量整数的指针" << endl;
    cout << "→ 指向的值不能改" << endl;
    
    cout << "\n2. int* const p" << endl;
    cout << "---------------------------------------" << endl;
    cout << "从右往左读：" << endl;
    cout << "p → 是一个常量" << endl;
    cout << "const → 常量" << endl;
    cout << "* → 指针" << endl;
    cout << "int → 指向整数" << endl;
    cout << "\n结果：p 是一个常量指针，指向整数" << endl;
    cout << "→ 指针本身不能改" << endl;
    
    cout << "\n3. const int* const p" << endl;
    cout << "---------------------------------------" << endl;
    cout << "从右往左读：" << endl;
    cout << "p → 是一个常量指针" << endl;
    cout << "const → 常量" << endl;
    cout << "* → 指针" << endl;
    cout << "int → 指向整数" << endl;
    cout << "const → 常量" << endl;
    cout << "\n结果：p 是常量指针，指向常量整数" << endl;
    cout << "→ 指针和值都不能改" << endl;
}

// ============================================
// 看 const 位置的技巧
// ============================================
void const_position_trick() {
    cout << "\n" << string(60, '=') << endl;
    cout << "看 const 位置的技巧" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n规则：看 const 和 * 的相对位置" << endl;
    cout << "---------------------------------------" << endl;
    
    cout << "\nconst 在 * 左边（左 const）：" << endl;
    cout << "  const int* p" << endl;
    cout << "  int const* p  （等价）" << endl;
    cout << "  ^^^^^ ^^" << endl;
    cout << "    |   |" << endl;
    cout << "    const 修饰 *p（指向的值）" << endl;
    cout << "  → 常量指针（值不能改）" << endl;
    
    cout << "\nconst 在 * 右边（右 const）：" << endl;
    cout << "  int* const p" << endl;
    cout << "      ^^^^^ ^" << endl;
    cout << "        |   |" << endl;
    cout << "        const 修饰 p（指针本身）" << endl;
    cout << "  → 指针常量（指针不能改）" << endl;
    
    cout << "\nconst 在两边：" << endl;
    cout << "  const int* const p" << endl;
    cout << "  ^^^^^     ^^^^^ ^" << endl;
    cout << "    |         |   |" << endl;
    cout << "    |         const 修饰 p（指针本身）" << endl;
    cout << "    const 修饰 *p（指向的值）" << endl;
    cout << "  → 都不能改" << endl;
}

// ============================================
// 引用的 const
// ============================================
void reference_const() {
    cout << "\n" << string(60, '=') << endl;
    cout << "引用的 const" << endl;
    cout << string(60, '=') << endl;
    
    int x = 100;
    
    cout << "\n1. 普通引用" << endl;
    cout << "---------------------------------------" << endl;
    cout << "int& ref = x;" << endl;
    int& ref = x;
    cout << "ref = " << ref << endl;
    cout << "✅ ref = 200;（可以修改）" << endl;
    ref = 200;
    cout << "x 现在 = " << x << endl;
    
    cout << "\n2. const 引用" << endl;
    cout << "---------------------------------------" << endl;
    cout << "const int& cref = x;" << endl;
    const int& cref = x;
    cout << "cref = " << cref << endl;
    cout << "❌ cref = 300;（不能修改）" << endl;
    // cref = 300;  // 编译错误！
    
    cout << "\n注意：引用本身就不能改变指向！" << endl;
    cout << "所以引用只有一种 const：" << endl;
    cout << "  const int& → const 引用（不能修改值）" << endl;
    cout << "  没有「引用常量」的概念！" << endl;
    
    cout << "\n" << string(60, '-') << endl;
    
    cout << "\n3. 对象的 const 引用" << endl;
    cout << "---------------------------------------" << endl;
    Student s("王五", 30);
    const Student& sref = s;
    
    cout << "const Student& sref = s;" << endl;
    cout << "\n可以做什么？" << endl;
    cout << "✅ sref.name（读取成员）: " << sref.name << endl;
    cout << "✅ sref.display()（调用 const 方法）" << endl;
    
    cout << "\n不能做什么？" << endl;
    cout << "❌ sref.name = \"新名字\";（不能修改）" << endl;
    // sref.name = "新名字";  // 如果是 const，会编译错误
    
    cout << "\n对比智能指针的 const 引用：" << endl;
    cout << "---------------------------------------" << endl;
    auto ptr = make_shared<Student>("赵六", 35);
    const auto& pref = ptr;
    
    cout << "const auto& pref = ptr;" << endl;
    cout << "类型: const shared_ptr<Student>&" << endl;
    cout << "\n✅ pref->name（可以访问）: " << pref->name << endl;
    cout << "❌ pref = other_ptr（不能改变引用）" << endl;
    cout << "❌ pref.reset()（不能调用非 const 方法）" << endl;
}

// ============================================
// 完整对比表
// ============================================
void comparison_table() {
    cout << "\n" << string(60, '=') << endl;
    cout << "完整对比表" << endl;
    cout << string(60, '=') << endl;
    
    cout << R"(
┌──────────────────────┬──────────────┬──────────────┬──────────┐
│  声明                 │  名称         │  指针能改？  │  值能改？│
├──────────────────────┼──────────────┼──────────────┼──────────┤
│ int* p               │  普通指针     │  ✅          │  ✅      │
├──────────────────────┼──────────────┼──────────────┼──────────┤
│ const int* p         │  常量指针     │  ✅          │  ❌      │
│ int const* p         │  （同上）     │  ✅          │  ❌      │
├──────────────────────┼──────────────┼──────────────┼──────────┤
│ int* const p         │  指针常量     │  ❌          │  ✅      │
├──────────────────────┼──────────────┼──────────────┼──────────┤
│ const int* const p   │  两者都是常量 │  ❌          │  ❌      │
├──────────────────────┼──────────────┼──────────────┼──────────┤
│ int& r               │  普通引用     │  不适用*     │  ✅      │
├──────────────────────┼──────────────┼──────────────┼──────────┤
│ const int& r         │  const引用    │  不适用*     │  ❌      │
└──────────────────────┴──────────────┴──────────────┴──────────┘

* 引用初始化后就不能改变指向，所以没有"引用能改？"这个问题

记忆口诀：
- const 在 * 左边 → 值不能改（常量指针）
- const 在 * 右边 → 指针不能改（指针常量）
- const 在两边 → 都不能改
- 引用加 const → 值不能改
)" << endl;
}

// ============================================
// 实际应用场景
// ============================================
void practical_usage() {
    cout << "\n" << string(60, '=') << endl;
    cout << "实际应用场景" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n场景1: 函数参数（推荐 const 引用）" << endl;
    cout << "---------------------------------------" << endl;
    cout << "void printStudent(const Student& s) {" << endl;
    cout << "    // s 不会被修改，也不会被复制" << endl;
    cout << "}" << endl;
    cout << "✅ 高效且安全" << endl;
    
    cout << "\n场景2: 遍历容器（推荐 const auto&）" << endl;
    cout << "---------------------------------------" << endl;
    cout << "for (const auto& item : container) {" << endl;
    cout << "    // item 不会被修改，也不会被复制" << endl;
    cout << "}" << endl;
    cout << "✅ 现代 C++ 最佳实践" << endl;
    
    cout << "\n场景3: 指向字符串字面量（必须用常量指针）" << endl;
    cout << "---------------------------------------" << endl;
    cout << "const char* str = \"Hello\";  // ✅ 正确" << endl;
    cout << "// char* str = \"Hello\";    // ⚠️  不推荐（某些编译器会警告）" << endl;
    
    cout << "\n场景4: 固定指针位置（用指针常量）" << endl;
    cout << "---------------------------------------" << endl;
    cout << "int* const ptr = get_buffer();  // 指针不能改变" << endl;
    cout << "*ptr = 100;  // 但可以修改内容" << endl;
    cout << "用于：固定的内存地址操作" << endl;
}

// ============================================
// 常见错误
// ============================================
void common_mistakes() {
    cout << "\n" << string(60, '=') << endl;
    cout << "常见错误" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n错误1: 混淆常量指针和指针常量" << endl;
    cout << "---------------------------------------" << endl;
    cout << "const int* p;  → 常量指针（值不能改）" << endl;
    cout << "int* const p;  → 指针常量（指针不能改）" << endl;
    cout << "记住：const 靠近谁，谁就不能改" << endl;
    
    cout << "\n错误2: 认为引用可以改变指向" << endl;
    cout << "---------------------------------------" << endl;
    cout << "int& ref = x;" << endl;
    cout << "ref = y;  // ❌ 这不是改变指向，是修改 x 的值为 y" << endl;
    cout << "引用初始化后永远绑定同一个对象！" << endl;
    
    cout << "\n错误3: 混淆 const auto& 和指针常量" << endl;
    cout << "---------------------------------------" << endl;
    cout << "const auto& ref = ptr;  → const 引用" << endl;
    cout << "auto* const p = &x;     → 指针常量" << endl;
    cout << "这是两个完全不同的概念！" << endl;
}

int main() {
    cout << "===========================================" << endl;
    cout << "  const auto& 和指针常量详解" << endl;
    cout << "===========================================" << endl;
    
    const_auto_ref();
    pointer_const_comparison();
    memory_trick();
    const_position_trick();
    reference_const();
    comparison_table();
    practical_usage();
    common_mistakes();
    
    cout << "\n===========================================" << endl;
    cout << "总结" << endl;
    cout << "===========================================" << endl;
    cout << "\n回答你的问题：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "const auto& student → const 引用（不是指针！）" << endl;
    cout << "  - 引用本身就不能改变绑定" << endl;
    cout << "  - const 表示不能通过它修改对象" << endl;
    cout << "\n指针常量 vs 常量指针：" << endl;
    cout << "  const int* p  → 常量指针（值不能改）" << endl;
    cout << "  int* const p  → 指针常量（指针不能改）" << endl;
    cout << "\n记忆技巧：" << endl;
    cout << "  const 在 * 左边 → 左值不能改" << endl;
    cout << "  const 在 * 右边 → 右指针不能改" << endl;
    cout << "===========================================" << endl;
    
    return 0;
}

/**
 * 快速参考卡片：
 * 
 * ┌────────────────────────────────────────┐
 * │  const 位置                             │
 * ├────────────────────────────────────────┤
 * │  const int* p     常量指针（值不能改） │
 * │        ^^^^ ^                          │
 * │         |   |                          │
 * │         const 在 * 左边                │
 * ├────────────────────────────────────────┤
 * │  int* const p     指针常量（指针不能改）│
 * │      ^^^^^ ^                           │
 * │        |   |                           │
 * │        const 在 * 右边                 │
 * ├────────────────────────────────────────┤
 * │  const int* const p   都不能改         │
 * │        ^^^^ ^^^^^ ^                    │
 * │         |     |                        │
 * │         两个 const                     │
 * └────────────────────────────────────────┘
 * 
 * 引用的 const：
 * const int& ref → const 引用（值不能改）
 * 引用本身永远不能改变绑定！
 * 
 * const auto& → const 引用（不是指针常量！）
 */
