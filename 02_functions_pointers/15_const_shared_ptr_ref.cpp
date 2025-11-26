/**
 * const shared_ptr<Student>& 详解
 * 
 * 理解：const引用 + 智能指针的组合
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class Student {
private:
    string name;
    int id;
    
public:
    Student(string n, int i) : name(n), id(i) {
        cout << "创建学生: " << name << endl;
    }
    
    ~Student() {
        cout << "销毁学生: " << name << endl;
    }
    
    string getName() const { return name; }
    int getId() const { return id; }
    
    void setName(string n) { name = n; }
};

// ============================================
// 各种参数传递方式对比
// ============================================

// 方式1: 按值传递智能指针
void method1(shared_ptr<Student> s) {
    cout << "\n方式1: shared_ptr<Student> s（按值）" << endl;
    cout << "  引用计数: " << s.use_count() << endl;
    cout << "  问题：会复制智能指针，引用计数+1" << endl;
}

// 方式2: 按引用传递智能指针
void method2(shared_ptr<Student>& s) {
    cout << "\n方式2: shared_ptr<Student>& s（引用）" << endl;
    cout << "  引用计数: " << s.use_count() << endl;
    cout << "  可以修改智能指针本身" << endl;
    // s = nullptr;  // 可以！会影响外部
}

// 方式3: 按const引用传递智能指针 ✅
void method3(const shared_ptr<Student>& s) {
    cout << "\n方式3: const shared_ptr<Student>& s（const引用）✅" << endl;
    cout << "  引用计数: " << s.use_count() << endl;
    cout << "  不能修改智能指针本身" << endl;
    // s = nullptr;  // ❌ 编译错误！
}

// 方式4: 按原始指针传递
void method4(Student* s) {
    cout << "\n方式4: Student* s（原始指针）" << endl;
    cout << "  需要检查空指针" << endl;
    if (s) {
        cout << "  学生: " << s->getName() << endl;
    }
}

void compare_methods() {
    cout << "\n" << string(60, '=') << endl;
    cout << "各种传递方式对比" << endl;
    cout << string(60, '=') << endl;
    
    auto student = make_shared<Student>("张三", 1001);
    cout << "\n初始引用计数: " << student.use_count() << endl;
    
    method1(student);  // 引用计数会变成2
    cout << "调用后引用计数: " << student.use_count() << endl;
    
    method2(student);  // 引用计数仍是1
    cout << "调用后引用计数: " << student.use_count() << endl;
    
    method3(student);  // 引用计数仍是1（推荐）
    cout << "调用后引用计数: " << student.use_count() << endl;
    
    method4(student.get());  // 获取原始指针
}

// ============================================
// const 的三层含义
// ============================================
void const_three_levels() {
    cout << "\n" << string(60, '=') << endl;
    cout << "const 的三层理解" << endl;
    cout << string(60, '=') << endl;
    
    auto s = make_shared<Student>("李四", 1002);
    
    cout << "\n1. const shared_ptr<Student>& s" << endl;
    cout << "---------------------------------------" << endl;
    cout << "完整类型：const (shared_ptr<Student>) &" << endl;
    cout << "           ^^^^^                       ^" << endl;
    cout << "             |                         |" << endl;
    cout << "             |                         引用（不复制）" << endl;
    cout << "             const修饰整个智能指针" << endl;
    
    cout << "\n含义拆解：" << endl;
    cout << "  shared_ptr<Student>  → 指向Student的智能指针" << endl;
    cout << "  &                    → 引用传递（不复制智能指针）" << endl;
    cout << "  const                → 不能修改这个智能指针" << endl;
    
    cout << "\n2. 可以做什么？" << endl;
    cout << "---------------------------------------" << endl;
    const shared_ptr<Student>& ref = s;
    cout << "✅ 读取学生信息: " << ref->getName() << endl;
    cout << "✅ 调用const方法: ID = " << ref->getId() << endl;
    cout << "✅ 修改学生内容: ";
    ref->setName("李四改名");
    cout << ref->getName() << endl;
    
    cout << "\n3. 不能做什么？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "❌ 不能让智能指针指向其他对象" << endl;
    cout << "   // ref = make_shared<Student>(...); // 编译错误！" << endl;
    cout << "❌ 不能置空" << endl;
    cout << "   // ref = nullptr; // 编译错误！" << endl;
    cout << "❌ 不能调用非const的智能指针方法" << endl;
    cout << "   // ref.reset(); // 编译错误！" << endl;
}

// ============================================
// 实际使用场景
// ============================================
void practical_usage() {
    cout << "\n" << string(60, '=') << endl;
    cout << "实际使用场景" << endl;
    cout << string(60, '=') << endl;
    
    vector<shared_ptr<Student>> students;
    students.push_back(make_shared<Student>("张三", 1001));
    students.push_back(make_shared<Student>("李四", 1002));
    students.push_back(make_shared<Student>("王五", 1003));
    
    cout << "\n场景1: 遍历容器（只读）" << endl;
    cout << "---------------------------------------" << endl;
    cout << "for (const auto& s : students) {" << endl;
    cout << "    // s 的类型就是 const shared_ptr<Student>&" << endl;
    cout << "}" << endl;
    for (const auto& s : students) {
        cout << "  - " << s->getName() << endl;
    }
    
    cout << "\n场景2: Lambda 捕获（只读）" << endl;
    cout << "---------------------------------------" << endl;
    cout << "[id](const shared_ptr<Student>& s) {" << endl;
    cout << "    return s->getId() == id;" << endl;
    cout << "}" << endl;
    int targetId = 1002;
    auto it = find_if(students.begin(), students.end(),
        [targetId](const shared_ptr<Student>& s) {
            return s->getId() == targetId;
        }
    );
    if (it != students.end()) {
        cout << "找到: " << (*it)->getName() << endl;
    }
    
    cout << "\n场景3: 函数参数（只读访问）" << endl;
    cout << "---------------------------------------" << endl;
    auto displayStudent = [](const shared_ptr<Student>& s) {
        cout << "学生: " << s->getName() << ", ID: " << s->getId() << endl;
    };
    displayStudent(students[0]);
}

// ============================================
// 性能分析
// ============================================
void performance_analysis() {
    cout << "\n" << string(60, '=') << endl;
    cout << "性能分析" << endl;
    cout << string(60, '=') << endl;
    
    auto student = make_shared<Student>("性能测试", 9999);
    
    cout << "\n按值传递 shared_ptr<Student>：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. 复制智能指针对象" << endl;
    cout << "2. 引用计数原子操作 +1" << endl;
    cout << "3. 函数结束时引用计数 -1" << endl;
    cout << "→ 有开销（虽然不大，但可避免）" << endl;
    
    cout << "\n按引用传递 const shared_ptr<Student>&：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. 只传递引用（8字节地址）" << endl;
    cout << "2. 不复制智能指针" << endl;
    cout << "3. 引用计数不变" << endl;
    cout << "→ 零开销！✅" << endl;
    
    cout << "\n引用计数演示：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "初始: use_count = " << student.use_count() << endl;
    
    {
        shared_ptr<Student> copy = student;  // 按值复制
        cout << "按值复制后: use_count = " << student.use_count() << endl;
    }
    cout << "离开作用域: use_count = " << student.use_count() << endl;
    
    {
        const shared_ptr<Student>& ref = student;  // 引用
        cout << "引用传递: use_count = " << student.use_count() 
             << " （不变！）" << endl;
    }
}

// ============================================
// 常见错误示例
// ============================================
void common_mistakes() {
    cout << "\n" << string(60, '=') << endl;
    cout << "常见错误" << endl;
    cout << string(60, '=') << endl;
    
    auto s = make_shared<Student>("错误示例", 8888);
    
    cout << "\n错误1: 按值传递（不必要的复制）" << endl;
    cout << "---------------------------------------" << endl;
    cout << "❌ void func(shared_ptr<Student> s) { ... }" << endl;
    cout << "问题：每次调用都会复制智能指针" << endl;
    
    cout << "\n错误2: 使用非const引用（可能误修改）" << endl;
    cout << "---------------------------------------" << endl;
    cout << "⚠️  void func(shared_ptr<Student>& s) {" << endl;
    cout << "       s = nullptr;  // 可能不小心置空！" << endl;
    cout << "   }" << endl;
    
    cout << "\n正确做法：const引用" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ void func(const shared_ptr<Student>& s) { ... }" << endl;
    cout << "优点：高效 + 安全" << endl;
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
    cout << "void processStudent(Student s) {" << endl;
    cout << "    // s 是对象引用（类似指针）" << endl;
    cout << "    // 传递引用本身，不复制对象" << endl;
    cout << "    // 可以修改对象内容" << endl;
    cout << "    // 可以让 s 指向其他对象: s = otherStudent;" << endl;
    cout << "}" << endl;
    
    cout << "\nJava（final 参数）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "void processStudent(final Student s) {" << endl;
    cout << "    // s 是final，不能重新赋值" << endl;
    cout << "    // s = otherStudent; // ❌ 编译错误" << endl;
    cout << "    // 类似 C++ 的 const shared_ptr<Student>&" << endl;
    cout << "}" << endl;
    
    cout << "\nKotlin：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "fun processStudent(s: Student) {" << endl;
    cout << "    // 默认就是不可变引用（类似final）" << endl;
    cout << "    // s = otherStudent // ❌ 编译错误" << endl;
    cout << "}" << endl;
    
    cout << "\nC++：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "void processStudent(const shared_ptr<Student>& s) {" << endl;
    cout << "    // const: 不能修改智能指针本身" << endl;
    cout << "    // &: 引用传递（不复制）" << endl;
    cout << "    // 最接近 Kotlin 的默认行为" << endl;
    cout << "}" << endl;
    
    cout << "\n总结：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "Java final Student     ≈  C++ const shared_ptr<Student>&" << endl;
    cout << "Kotlin Student         ≈  C++ const shared_ptr<Student>&" << endl;
    cout << "C++ 需要显式指定，更底层，更灵活" << endl;
}

// ============================================
// 完整对比表
// ============================================
void comparison_table() {
    cout << "\n" << string(60, '=') << endl;
    cout << "完整对比表" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n┌──────────────────────────────┬─────┬────────┬──────────┐" << endl;
    cout << "│  类型                         │复制 │引用计数 │修改指针  │" << endl;
    cout << "├──────────────────────────────┼─────┼────────┼──────────┤" << endl;
    cout << "│ shared_ptr<Student> s        │ 是  │  +1    │  可以    │" << endl;
    cout << "│ shared_ptr<Student>& s       │ 否  │  不变  │  可以    │" << endl;
    cout << "│ const shared_ptr<Student>& s │ 否  │  不变  │  不可以  │✅" << endl;
    cout << "│ Student* s                   │ 否  │  无    │  可以    │" << endl;
    cout << "└──────────────────────────────┴─────┴────────┴──────────┘" << endl;
    
    cout << "\n推荐使用场景：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ 只读访问 → const shared_ptr<Student>&" << endl;
    cout << "✅ 需要修改指针本身 → shared_ptr<Student>&" << endl;
    cout << "⚠️  需要转移所有权 → shared_ptr<Student> (按值)" << endl;
    cout << "⚠️  已有智能指针，临时访问 → Student* 或 Student&" << endl;
}

int main() {
    cout << "===========================================" << endl;
    cout << "  const shared_ptr<Student>& 详解" << endl;
    cout << "===========================================" << endl;
    
    compare_methods();
    const_three_levels();
    practical_usage();
    performance_analysis();
    common_mistakes();
    compare_with_java_kotlin();
    comparison_table();
    
    cout << "\n===========================================" << endl;
    cout << "总结" << endl;
    cout << "===========================================" << endl;
    cout << "\nconst shared_ptr<Student>& s" << endl;
    cout << "^^^^^ ^^^^^^^^^^^^^^^^^^^ ^" << endl;
    cout << "  |           |            |" << endl;
    cout << "  |           |            └─ 引用（不复制智能指针）" << endl;
    cout << "  |           └─ 智能指针类型" << endl;
    cout << "  └─ 不能修改指针本身" << endl;
    cout << "\n这是传递智能指针的最佳实践！" << endl;
    cout << "高效（不复制）+ 安全（不能误修改）✅" << endl;
    cout << "===========================================" << endl;
    
    return 0;
}

/**
 * 快速记忆：
 * 
 * const shared_ptr<Student>& s
 * 
 * 从右往左读：
 * 1. & → 引用（不复制）
 * 2. shared_ptr<Student> → 智能指针
 * 3. const → 智能指针本身不可修改
 * 
 * 类比 Java：
 * final Student s  ≈  const shared_ptr<Student>& s
 * 
 * 使用场景：
 * - Lambda 参数
 * - 遍历容器
 * - 函数参数（只读）
 * 
 * 这是现代 C++ 的标准写法！
 */
