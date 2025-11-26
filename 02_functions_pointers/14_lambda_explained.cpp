/**
 * Lambda 表达式详解
 * 解释学生管理系统中的 Lambda 用法
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
    double score;

public:
    Student(string n, int i, double s) : name(n), id(i), score(s) {}
    
    int getId() const { return id; }
    string getName() const { return name; }
    double getScore() const { return score; }
    
    void display() const {
        cout << "学号: " << id << ", 姓名: " << name 
             << ", 成绩: " << score << endl;
    }
};

// ============================================
// 方法1: Lambda 表达式（现代 C++ 推荐）✅
// ============================================
void removeStudent_Lambda(vector<shared_ptr<Student>>& students, int id) {
    cout << "\n方法1: 使用 Lambda" << endl;
    cout << "---------------------------------------" << endl;
    
    auto it = find_if(students.begin(), students.end(),
        [id](const shared_ptr<Student>& s) {  // Lambda 表达式
            return s->getId() == id;
        }
    );
    
    if (it != students.end()) {
        cout << "找到学生: " << (*it)->getName() << endl;
    }
}

// ============================================
// 方法2: 函数对象（Functor）- 传统方式
// ============================================
class StudentIdMatcher {
private:
    int targetId;  // 要查找的学号

public:
    // 构造函数，保存要查找的 id
    StudentIdMatcher(int id) : targetId(id) {}
    
    // 重载 () 运算符，使对象可以像函数一样调用
    bool operator()(const shared_ptr<Student>& s) const {
        return s->getId() == targetId;
    }
};

void removeStudent_Functor(vector<shared_ptr<Student>>& students, int id) {
    cout << "\n方法2: 使用 Functor（函数对象）" << endl;
    cout << "---------------------------------------" << endl;
    
    StudentIdMatcher matcher(id);  // 创建函数对象
    auto it = find_if(students.begin(), students.end(), matcher);
    
    if (it != students.end()) {
        cout << "找到学生: " << (*it)->getName() << endl;
    }
}

// ============================================
// 方法3: 普通函数 + 全局变量（不推荐）❌
// ============================================
int g_targetId;  // 全局变量（危险！）

bool matchStudentId(const shared_ptr<Student>& s) {
    return s->getId() == g_targetId;
}

void removeStudent_GlobalFunction(vector<shared_ptr<Student>>& students, int id) {
    cout << "\n方法3: 使用全局函数（不推荐）" << endl;
    cout << "---------------------------------------" << endl;
    
    g_targetId = id;  // 设置全局变量
    auto it = find_if(students.begin(), students.end(), matchStudentId);
    
    if (it != students.end()) {
        cout << "找到学生: " << (*it)->getName() << endl;
    }
}

// ============================================
// Lambda 详细解析
// ============================================
void lambda_details() {
    cout << "\n" << string(60, '=') << endl;
    cout << "Lambda 表达式详解" << endl;
    cout << string(60, '=') << endl;
    
    int targetId = 1001;
    vector<shared_ptr<Student>> students;
    students.push_back(make_shared<Student>("张三", 1001, 85.5));
    students.push_back(make_shared<Student>("李四", 1002, 92.0));
    
    cout << "\nLambda 语法：" << endl;
    cout << "[捕获列表](参数列表) { 函数体 }" << endl;
    cout << "\n具体例子：" << endl;
    cout << "[id](const shared_ptr<Student>& s) { return s->getId() == id; }" << endl;
    cout << " ↑        ↑                              ↑" << endl;
    cout << " |        |                              |" << endl;
    cout << " |        |                              函数体" << endl;
    cout << " |        参数" << endl;
    cout << " 捕获外部变量" << endl;
    
    cout << "\n执行过程：" << endl;
    cout << "1. find_if 遍历每个学生" << endl;
    cout << "2. 对每个学生调用 Lambda" << endl;
    cout << "3. Lambda 比较学生的 id 和捕获的 targetId" << endl;
    cout << "4. 返回 true/false" << endl;
    cout << "5. find_if 找到第一个返回 true 的元素" << endl;
}

// ============================================
// 捕获方式详解
// ============================================
void capture_modes() {
    cout << "\n" << string(60, '=') << endl;
    cout << "Lambda 捕获方式" << endl;
    cout << string(60, '=') << endl;
    
    int x = 10;
    int y = 20;
    string name = "张三";
    
    cout << "\n1. [=] - 按值捕获所有外部变量" << endl;
    cout << "---------------------------------------" << endl;
    auto lambda1 = [=]() {
        cout << "x = " << x << ", y = " << y << ", name = " << name << endl;
        // x = 100;  // ❌ 编译错误！按值捕获是const的
    };
    lambda1();
    cout << "特点：复制所有变量，不能修改" << endl;
    
    cout << "\n2. [&] - 按引用捕获所有外部变量" << endl;
    cout << "---------------------------------------" << endl;
    auto lambda2 = [&]() {
        cout << "修改前: x = " << x << endl;
        x = 100;  // ✅ 可以修改
        cout << "修改后: x = " << x << endl;
    };
    lambda2();
    cout << "外部 x 现在 = " << x << endl;
    cout << "特点：引用所有变量，可以修改" << endl;
    
    cout << "\n3. [x] - 按值捕获特定变量 x" << endl;
    cout << "---------------------------------------" << endl;
    auto lambda3 = [x]() {
        cout << "只捕获 x = " << x << endl;
        // cout << y;  // ❌ 编译错误！没有捕获 y
    };
    lambda3();
    cout << "特点：只捕获指定变量，按值" << endl;
    
    cout << "\n4. [&x] - 按引用捕获特定变量 x" << endl;
    cout << "---------------------------------------" << endl;
    auto lambda4 = [&x]() {
        x = 200;
        cout << "修改 x = " << x << endl;
    };
    lambda4();
    cout << "外部 x 现在 = " << x << endl;
    cout << "特点：只捕获指定变量，按引用" << endl;
    
    cout << "\n5. [=, &x] - 混合捕获" << endl;
    cout << "---------------------------------------" << endl;
    x = 10;  // 重置
    auto lambda5 = [=, &x]() {  // x按引用，其他按值
        x = 300;
        cout << "x(引用) = " << x << ", y(值) = " << y << endl;
    };
    lambda5();
    cout << "外部 x = " << x << ", y = " << y << endl;
    cout << "特点：默认按值，x按引用" << endl;
    
    cout << "\n6. [this] - 捕获当前对象指针（在类中）" << endl;
    cout << "---------------------------------------" << endl;
    cout << "用于在成员函数中使用 Lambda" << endl;
}

// ============================================
// 实际应用场景
// ============================================
void practical_examples() {
    cout << "\n" << string(60, '=') << endl;
    cout << "Lambda 实际应用" << endl;
    cout << string(60, '=') << endl;
    
    vector<shared_ptr<Student>> students;
    students.push_back(make_shared<Student>("张三", 1001, 85.5));
    students.push_back(make_shared<Student>("李四", 1002, 92.0));
    students.push_back(make_shared<Student>("王五", 1003, 78.5));
    students.push_back(make_shared<Student>("赵六", 1004, 88.0));
    
    cout << "\n场景1: 查找学号为 1002 的学生" << endl;
    cout << "---------------------------------------" << endl;
    int targetId = 1002;
    auto it = find_if(students.begin(), students.end(),
        [targetId](const shared_ptr<Student>& s) {
            return s->getId() == targetId;
        }
    );
    if (it != students.end()) {
        (*it)->display();
    }
    
    cout << "\n场景2: 统计成绩 >= 85 的学生数量" << endl;
    cout << "---------------------------------------" << endl;
    double threshold = 85.0;
    int count = count_if(students.begin(), students.end(),
        [threshold](const shared_ptr<Student>& s) {
            return s->getScore() >= threshold;
        }
    );
    cout << "成绩 >= " << threshold << " 的学生: " << count << " 人" << endl;
    
    cout << "\n场景3: 按成绩升序排序" << endl;
    cout << "---------------------------------------" << endl;
    sort(students.begin(), students.end(),
        [](const shared_ptr<Student>& a, const shared_ptr<Student>& b) {
            return a->getScore() < b->getScore();
        }
    );
    cout << "排序后：" << endl;
    for (const auto& s : students) {
        s->display();
    }
    
    cout << "\n场景4: 遍历并修改（增加5分奖励）" << endl;
    cout << "---------------------------------------" << endl;
    for_each(students.begin(), students.end(),
        [](shared_ptr<Student>& s) {
            // 注意：这里需要修改，所以不能用 const
            cout << s->getName() << " 加分前: " << s->getScore();
            // 假设有 setScore 方法
            cout << " → 加分后: " << s->getScore() + 5 << endl;
        }
    );
}

// ============================================
// Lambda vs 其他方式对比
// ============================================
void compare_approaches() {
    cout << "\n" << string(60, '=') << endl;
    cout << "Lambda vs 其他方式" << endl;
    cout << string(60, '=') << endl;
    
    vector<shared_ptr<Student>> students;
    students.push_back(make_shared<Student>("张三", 1001, 85.5));
    students.push_back(make_shared<Student>("李四", 1002, 92.0));
    
    int id = 1001;
    
    removeStudent_Lambda(students, id);      // 简洁、现代
    removeStudent_Functor(students, id);     // 传统、冗长
    removeStudent_GlobalFunction(students, id);  // 不安全
    
    cout << "\n总结：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ Lambda:  简洁、安全、现代 C++ 推荐" << endl;
    cout << "⚠️  Functor: 需要定义类，代码冗长" << endl;
    cout << "❌ 全局函数: 使用全局变量，不安全" << endl;
}

int main() {
    cout << "===========================================" << endl;
    cout << "  Lambda 表达式完全解析" << endl;
    cout << "===========================================" << endl;
    
    lambda_details();
    capture_modes();
    practical_examples();
    compare_approaches();
    
    cout << "\n===========================================" << endl;
    cout << "总结" << endl;
    cout << "===========================================" << endl;
    cout << "\nLambda 表达式 = 匿名函数" << endl;
    cout << "\n你代码中的：" << endl;
    cout << "[id](const shared_ptr<Student>& s) {" << endl;
    cout << "    return s->getId() == id;" << endl;
    cout << "}" << endl;
    cout << "\n含义：" << endl;
    cout << "创建一个临时函数，用于判断学生的 id 是否等于指定值" << endl;
    cout << "\n[id]   → 捕获外部变量 id（按值）" << endl;
    cout << "(...)  → 参数：每个学生的智能指针" << endl;
    cout << "{...}  → 函数体：比较并返回 true/false" << endl;
    cout << "\n这是现代 C++ 的精华！简洁又强大！✅" << endl;
    cout << "===========================================" << endl;
    
    return 0;
}

/**
 * 快速参考：
 * 
 * Lambda 语法：
 * [捕获列表](参数列表) -> 返回类型 { 函数体 }
 * 
 * 捕获方式：
 * []        - 不捕获任何变量
 * [=]       - 按值捕获所有变量
 * [&]       - 按引用捕获所有变量
 * [x]       - 按值捕获 x
 * [&x]      - 按引用捕获 x
 * [=, &x]   - x 按引用，其他按值
 * [&, x]    - x 按值，其他按引用
 * [this]    - 捕获当前对象指针
 * 
 * 常用场景：
 * - find_if()    查找
 * - count_if()   统计
 * - sort()       排序
 * - for_each()   遍历
 * - remove_if()  删除
 * 
 * 与 Java/Kotlin 对比：
 * Java:   (s) -> s.getId() == id
 * Kotlin: { s -> s.id == id }
 * C++:    [id](const auto& s) { return s->getId() == id; }
 */
