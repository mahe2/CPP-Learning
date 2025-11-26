/**
 * 迭代器解引用详解：为什么需要 (*it)->getName()？
 * 
 * 理解迭代器、智能指针的多层解引用
 */

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>

using namespace std;

class Student {
private:
    string name;
    int id;

public:
    Student(string n, int i) : name(n), id(i) {
        cout << "创建学生: " << name << endl;
    }
    
    string getName() const { return name; }
    int getId() const { return id; }
};

// ============================================
// 迭代器解引用详解
// ============================================
void iterator_basics() {
    cout << "\n" << string(60, '=') << endl;
    cout << "迭代器基础" << endl;
    cout << string(60, '=') << endl;
    
    vector<shared_ptr<Student>> students;
    students.push_back(make_shared<Student>("张三", 1001));
    students.push_back(make_shared<Student>("李四", 1002));
    
    cout << "\n数据结构层次：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "vector<shared_ptr<Student>> students" << endl;
    cout << "  ^       ^                ^" << endl;
    cout << "  |       |                |" << endl;
    cout << "  |       |                学生对象" << endl;
    cout << "  |       智能指针" << endl;
    cout << "  容器" << endl;
    
    cout << "\n迭代器指向什么？" << endl;
    cout << "---------------------------------------" << endl;
    auto it = students.begin();
    cout << "auto it = students.begin();" << endl;
    cout << "\nit 的类型: vector<shared_ptr<Student>>::iterator" << endl;
    cout << "it 指向: vector 中的一个元素" << endl;
    cout << "vector 中的元素类型: shared_ptr<Student>" << endl;
    
    cout << "\n所以：" << endl;
    cout << "  it         → 迭代器（指向 shared_ptr）" << endl;
    cout << "  *it        → shared_ptr<Student>" << endl;
    cout << "  (*it)->... → 通过智能指针访问 Student 对象" << endl;
}

// ============================================
// 逐步解引用演示
// ============================================
void step_by_step() {
    cout << "\n" << string(60, '=') << endl;
    cout << "逐步解引用" << endl;
    cout << string(60, '=') << endl;
    
    vector<shared_ptr<Student>> students;
    students.push_back(make_shared<Student>("王五", 1003));
    
    auto it = students.begin();
    
    cout << "\n步骤1: 获取迭代器" << endl;
    cout << "---------------------------------------" << endl;
    cout << "auto it = students.begin();" << endl;
    cout << "类型: vector<shared_ptr<Student>>::iterator" << endl;
    
    cout << "\n步骤2: 解引用迭代器得到智能指针" << endl;
    cout << "---------------------------------------" << endl;
    cout << "auto ptr = *it;" << endl;
    auto ptr = *it;
    cout << "类型: shared_ptr<Student>" << endl;
    cout << "引用计数: " << ptr.use_count() << endl;
    
    cout << "\n步骤3: 通过智能指针访问对象" << endl;
    cout << "---------------------------------------" << endl;
    cout << "ptr->getName()" << endl;
    cout << "结果: " << ptr->getName() << endl;
    
    cout << "\n一步到位（合并步骤2和3）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "(*it)->getName()" << endl;
    cout << "       ^^    ^^^^^" << endl;
    cout << "       ||    |" << endl;
    cout << "       ||    调用方法" << endl;
    cout << "       |智能指针的 ->" << endl;
    cout << "       解引用迭代器" << endl;
    cout << "结果: " << (*it)->getName() << endl;
}

// ============================================
// 为什么需要括号？
// ============================================
void why_parentheses() {
    cout << "\n" << string(60, '=') << endl;
    cout << "为什么需要括号？" << endl;
    cout << string(60, '=') << endl;
    
    vector<shared_ptr<Student>> students;
    students.push_back(make_shared<Student>("赵六", 1004));
    auto it = students.begin();
    
    cout << "\n运算符优先级问题：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "-> 的优先级 > * 的优先级" << endl;
    
    cout << "\n错误写法（会编译失败）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "*it->getName()" << endl;
    cout << "会被解释为: *(it->getName())" << endl;
    cout << "           意思是: 先对 it 调用 ->getName()" << endl;
    cout << "           但 it 是迭代器，没有 getName() 方法！" << endl;
    cout << "           ❌ 编译错误！" << endl;
    
    cout << "\n正确写法（加括号）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "(*it)->getName()" << endl;
    cout << "^^^^^" << endl;
    cout << "  |" << endl;
    cout << "  括号强制先解引用" << endl;
    cout << "\n执行顺序：" << endl;
    cout << "1. (*it)        → 解引用迭代器，得到 shared_ptr<Student>" << endl;
    cout << "2. (*it)->      → 对智能指针使用 ->" << endl;
    cout << "3. (*it)->getName() → 调用 Student 的 getName()" << endl;
    cout << "✅ 正确！" << endl;
    
    cout << "\n实际输出: " << (*it)->getName() << endl;
}

// ============================================
// 多种写法对比
// ============================================
void different_ways() {
    cout << "\n" << string(60, '=') << endl;
    cout << "多种等价写法" << endl;
    cout << string(60, '=') << endl;
    
    vector<shared_ptr<Student>> students;
    students.push_back(make_shared<Student>("孙七", 1005));
    auto it = students.begin();
    
    cout << "\n写法1: (*it)->getName() [推荐]" << endl;
    cout << "---------------------------------------" << endl;
    cout << "结果: " << (*it)->getName() << endl;
    cout << "特点: 简洁、常用" << endl;
    
    cout << "\n写法2: 分步骤" << endl;
    cout << "---------------------------------------" << endl;
    auto ptr = *it;
    cout << "auto ptr = *it;" << endl;
    cout << "ptr->getName()" << endl;
    cout << "结果: " << ptr->getName() << endl;
    cout << "特点: 清晰、适合调试" << endl;
    
    cout << "\n写法3: it->get()->getName()" << endl;
    cout << "---------------------------------------" << endl;
    cout << "结果: " << it->get()->getName() << endl;
    cout << "说明: it->... 访问智能指针的方法" << endl;
    cout << "      get() 获取原始指针" << endl;
    cout << "      然后 -> 访问对象" << endl;
    cout << "特点: 冗长、不推荐" << endl;
    
    cout << "\n写法4: (**it).getName()" << endl;
    cout << "---------------------------------------" << endl;
    cout << "结果: " << (**it).getName() << endl;
    cout << "说明: *it 得到智能指针" << endl;
    cout << "      **it 再解引用得到对象" << endl;
    cout << "      .getName() 调用方法" << endl;
    cout << "特点: 少见、不直观" << endl;
}

// ============================================
// 不同容器类型对比
// ============================================
void different_containers() {
    cout << "\n" << string(60, '=') << endl;
    cout << "不同容器类型的解引用" << endl;
    cout << string(60, '=') << endl;
    
    // 情况1: vector<Student>（不用智能指针）
    cout << "\n情况1: vector<Student>" << endl;
    cout << "---------------------------------------" << endl;
    vector<Student> students1;
    students1.emplace_back("张三", 1001);
    auto it1 = students1.begin();
    cout << "vector<Student> students;" << endl;
    cout << "auto it = students.begin();" << endl;
    cout << "\nit 指向: Student 对象" << endl;
    cout << "*it → Student 对象" << endl;
    cout << "it->getName() 或 (*it).getName()" << endl;
    cout << "结果: " << it1->getName() << endl;
    cout << "✅ 不需要 (*it)-> 因为只有一层！" << endl;
    
    // 情况2: vector<Student*>（原始指针）
    cout << "\n情况2: vector<Student*>" << endl;
    cout << "---------------------------------------" << endl;
    vector<Student*> students2;
    students2.push_back(new Student("李四", 1002));
    auto it2 = students2.begin();
    cout << "vector<Student*> students;" << endl;
    cout << "auto it = students.begin();" << endl;
    cout << "\nit 指向: Student* (原始指针)" << endl;
    cout << "*it → Student* (原始指针)" << endl;
    cout << "(*it)->getName()" << endl;
    cout << "结果: " << (*it2)->getName() << endl;
    cout << "✅ 需要 (*it)-> 因为有两层！" << endl;
    delete students2[0];  // 手动释放
    
    // 情况3: vector<shared_ptr<Student>>（智能指针）
    cout << "\n情况3: vector<shared_ptr<Student>>" << endl;
    cout << "---------------------------------------" << endl;
    vector<shared_ptr<Student>> students3;
    students3.push_back(make_shared<Student>("王五", 1003));
    auto it3 = students3.begin();
    cout << "vector<shared_ptr<Student>> students;" << endl;
    cout << "auto it = students.begin();" << endl;
    cout << "\nit 指向: shared_ptr<Student>" << endl;
    cout << "*it → shared_ptr<Student>" << endl;
    cout << "(*it)->getName()" << endl;
    cout << "结果: " << (*it3)->getName() << endl;
    cout << "✅ 需要 (*it)-> 因为有两层！" << endl;
}

// ============================================
// 实际应用场景
// ============================================
void practical_examples() {
    cout << "\n" << string(60, '=') << endl;
    cout << "实际应用场景" << endl;
    cout << string(60, '=') << endl;
    
    vector<shared_ptr<Student>> students;
    students.push_back(make_shared<Student>("张三", 1001));
    students.push_back(make_shared<Student>("李四", 1002));
    students.push_back(make_shared<Student>("王五", 1003));
    
    cout << "\n场景1: find_if 查找后使用" << endl;
    cout << "---------------------------------------" << endl;
    int targetId = 1002;
    auto it = find_if(students.begin(), students.end(),
        [targetId](const shared_ptr<Student>& s) {
            return s->getId() == targetId;
        }
    );
    
    if (it != students.end()) {
        cout << "找到学生: " << (*it)->getName() << endl;
        cout << "学号: " << (*it)->getId() << endl;
    }
    
    cout << "\n场景2: 遍历并修改" << endl;
    cout << "---------------------------------------" << endl;
    cout << "for (auto it = students.begin(); it != students.end(); ++it) {" << endl;
    cout << "    cout << (*it)->getName();" << endl;
    cout << "}" << endl;
    for (auto it = students.begin(); it != students.end(); ++it) {
        cout << "  - " << (*it)->getName() << endl;
    }
    
    cout << "\n场景3: 删除特定元素" << endl;
    cout << "---------------------------------------" << endl;
    cout << "auto it = find_if(...);" << endl;
    cout << "if (it != students.end()) {" << endl;
    cout << "    cout << \"删除: \" << (*it)->getName();" << endl;
    cout << "    students.erase(it);" << endl;
    cout << "}" << endl;
}

// ============================================
// 常见错误
// ============================================
void common_mistakes() {
    cout << "\n" << string(60, '=') << endl;
    cout << "常见错误" << endl;
    cout << string(60, '=') << endl;
    
    vector<shared_ptr<Student>> students;
    students.push_back(make_shared<Student>("错误示例", 9999));
    auto it = students.begin();
    
    cout << "\n错误1: 忘记解引用" << endl;
    cout << "---------------------------------------" << endl;
    cout << "// it->getName()  ❌ 错误！" << endl;
    cout << "问题: it 是迭代器，不是智能指针" << endl;
    cout << "      迭代器没有 getName() 方法" << endl;
    
    cout << "\n错误2: 缺少括号" << endl;
    cout << "---------------------------------------" << endl;
    cout << "// *it->getName()  ❌ 错误！" << endl;
    cout << "问题: 运算符优先级问题" << endl;
    cout << "      会被理解为 *(it->getName())" << endl;
    
    cout << "\n错误3: 多余解引用" << endl;
    cout << "---------------------------------------" << endl;
    cout << "// (*it).getName()  ❌ 错误！" << endl;
    cout << "问题: *it 是智能指针，不是对象" << endl;
    cout << "      智能指针没有 getName() 方法" << endl;
    cout << "      应该用 -> 而不是 ." << endl;
    
    cout << "\n正确写法：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ (*it)->getName()" << endl;
    cout << "✅ it->get()->getName()  (冗长但正确)" << endl;
    cout << "✅ (**it).getName()      (少见但正确)" << endl;
}

// ============================================
// 与 Java/Kotlin 对比
// ============================================
void compare_with_java_kotlin() {
    cout << "\n" << string(60, '=') << endl;
    cout << "与 Java/Kotlin 对比" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\nJava（使用 Iterator）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "Iterator<Student> it = students.iterator();" << endl;
    cout << "while (it.hasNext()) {" << endl;
    cout << "    Student s = it.next();  // next() 直接返回对象" << endl;
    cout << "    s.getName();            // 直接调用" << endl;
    cout << "}" << endl;
    cout << "\n→ Java 的迭代器 next() 直接给你对象" << endl;
    
    cout << "\nKotlin（增强for循环）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "for (s in students) {" << endl;
    cout << "    s.name  // 直接访问" << endl;
    cout << "}" << endl;
    cout << "\n→ Kotlin 完全隐藏了迭代器细节" << endl;
    
    cout << "\nC++（迭代器）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "auto it = students.begin();" << endl;
    cout << "(*it)->getName();  // 需要手动解引用" << endl;
    cout << "\n→ C++ 更底层，需要理解迭代器和指针" << endl;
    
    cout << "\nC++（range-based for）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "for (const auto& s : students) {" << endl;
    cout << "    s->getName();  // s 是 shared_ptr<Student>" << endl;
    cout << "}" << endl;
    cout << "\n→ 使用 range-based for 可以避免手动解引用！" << endl;
}

// ============================================
// 内存布局图示
// ============================================
void memory_layout() {
    cout << "\n" << string(60, '=') << endl;
    cout << "内存布局示意" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\nvector<shared_ptr<Student>> students" << endl;
    cout << "---------------------------------------" << endl;
    cout << "\n内存结构：" << endl;
    cout << R"(
    students (vector)
    ┌────────────────┐
    │ [0]: ptr1      │───→ shared_ptr<Student> ───→ Student对象1
    │ [1]: ptr2      │───→ shared_ptr<Student> ───→ Student对象2
    │ [2]: ptr3      │───→ shared_ptr<Student> ───→ Student对象3
    └────────────────┘
         ↑
         │
    iterator (it)
    )" << endl;
    
    cout << "解引用过程：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "it              → 指向 vector 的某个位置" << endl;
    cout << "*it             → 得到该位置的元素（shared_ptr）" << endl;
    cout << "(*it)->getName() → 通过智能指针访问 Student 对象" << endl;
}

int main() {
    cout << "===========================================" << endl;
    cout << "  (*it)-> 详解" << endl;
    cout << "===========================================" << endl;
    
    iterator_basics();
    step_by_step();
    why_parentheses();
    different_ways();
    different_containers();
    practical_examples();
    common_mistakes();
    compare_with_java_kotlin();
    memory_layout();
    
    cout << "\n===========================================" << endl;
    cout << "总结" << endl;
    cout << "===========================================" << endl;
    cout << "\n为什么需要 (*it)-> ？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. it 是迭代器，指向容器中的元素" << endl;
    cout << "2. 容器是 vector<shared_ptr<Student>>" << endl;
    cout << "3. 元素类型是 shared_ptr<Student>" << endl;
    cout << "4. *it 解引用得到 shared_ptr<Student>" << endl;
    cout << "5. (*it)-> 通过智能指针访问 Student 对象" << endl;
    cout << "\n记住：" << endl;
    cout << "  it            → 迭代器" << endl;
    cout << "  *it           → shared_ptr<Student>" << endl;
    cout << "  (*it)->       → 访问 Student 对象成员" << endl;
    cout << "\n括号是必须的，因为 -> 优先级高于 * ！" << endl;
    cout << "===========================================" << endl;
    
    return 0;
}

/**
 * 快速参考：
 * 
 * vector<shared_ptr<Student>> students;
 * auto it = find_if(students.begin(), students.end(), ...);
 * 
 * 正确写法：
 * ✅ (*it)->getName()
 * ✅ it->get()->getName()  (冗长)
 * ✅ (**it).getName()      (少见)
 * 
 * 错误写法：
 * ❌ it->getName()         (it 是迭代器)
 * ❌ *it->getName()        (优先级错误)
 * ❌ (*it).getName()       (*it 是指针)
 * 
 * 记忆技巧：
 * - vector 容器有一层
 * - shared_ptr 又有一层
 * - 所以需要两次"穿透"：先 * 再 ->
 * - 括号保证顺序正确
 */
