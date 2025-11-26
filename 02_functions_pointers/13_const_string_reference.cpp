/**
 * 函数参数传递方式对比：const string& vs 其他方式
 * 
 * 核心问题：为什么 addStudent(const string& name) 而不是其他方式？
 */

#include <iostream>
#include <string>
#include <chrono>

using namespace std;
using namespace chrono;

// ============================================
// 方式1: 按值传递 (Pass by Value)
// ============================================
class Student1 {
private:
    string name;
public:
    // ❌ 不推荐：按值传递
    Student1(string n) : name(n) {
        cout << "Student1 构造（按值传递）" << endl;
    }
};

void addStudent1(string name) {  // 按值传递
    cout << "方式1: 按值传递 - " << name << endl;
}

// ============================================
// 方式2: 按引用传递 (Pass by Reference)
// ============================================
class Student2 {
private:
    string name;
public:
    // ⚠️  可以但不安全：非const引用
    Student2(string& n) : name(n) {
        cout << "Student2 构造（引用传递）" << endl;
    }
};

void addStudent2(string& name) {  // 引用传递（可以修改）
    cout << "方式2: 引用传递 - " << name << endl;
    // name = "Modified";  // 可以修改原字符串！危险！
}

// ============================================
// 方式3: const引用传递 (Pass by const Reference) ✅
// ============================================
class Student3 {
private:
    string name;
public:
    // ✅ 推荐：const引用
    Student3(const string& n) : name(n) {
        cout << "Student3 构造（const引用）" << endl;
    }
};

void addStudent3(const string& name) {  // const引用（不能修改）
    cout << "方式3: const引用传递 - " << name << endl;
    // name = "Modified";  // ❌ 编译错误！不能修改
}

// ============================================
// 方式4: 指针传递 (Pass by Pointer)
// ============================================
void addStudent4(const string* name) {  // 指针传递
    if (name) {
        cout << "方式4: 指针传递 - " << *name << endl;
    }
}

// ============================================
// 详细对比和分析
// ============================================
void compare_methods() {
    cout << "\n" << string(60, '=') << endl;
    cout << "各种传递方式的对比" << endl;
    cout << string(60, '=') << endl;
    
    string testName = "张三";
    
    cout << "\n1. 按值传递（string name）：" << endl;
    cout << "---------------------------------------" << endl;
    addStudent1(testName);
    cout << "原字符串: " << testName << endl;
    cout << "\n问题：" << endl;
    cout << "❌ 会复制整个字符串（性能差）" << endl;
    cout << "❌ 如果字符串很长，复制开销大" << endl;
    cout << "❌ 浪费内存和时间" << endl;
    
    cout << "\n2. 引用传递（string& name）：" << endl;
    cout << "---------------------------------------" << endl;
    addStudent2(testName);
    cout << "原字符串: " << testName << endl;
    cout << "\n问题：" << endl;
    cout << "⚠️  可以修改原字符串（不安全）" << endl;
    cout << "⚠️  不能传递临时对象（如 addStudent2(\"李四\")）" << endl;
    cout << "⚠️  调用者可能不希望参数被修改" << endl;
    
    cout << "\n3. const引用传递（const string& name）：✅" << endl;
    cout << "---------------------------------------" << endl;
    addStudent3(testName);
    cout << "原字符串: " << testName << endl;
    cout << "\n优点：" << endl;
    cout << "✅ 不复制，效率高（只传引用）" << endl;
    cout << "✅ 不能修改原字符串（安全）" << endl;
    cout << "✅ 可以传递临时对象" << endl;
    cout << "✅ 可以传递const对象" << endl;
    
    cout << "\n4. 指针传递（const string* name）：" << endl;
    cout << "---------------------------------------" << endl;
    addStudent4(&testName);
    cout << "\n问题：" << endl;
    cout << "⚠️  需要检查空指针" << endl;
    cout << "⚠️  语法不如引用简洁" << endl;
    cout << "⚠️  调用时需要取地址符 &" << endl;
}

// ============================================
// 性能对比测试
// ============================================
void performance_test() {
    cout << "\n" << string(60, '=') << endl;
    cout << "性能对比测试（100万次调用）" << endl;
    cout << string(60, '=') << endl;
    
    // 创建一个长字符串
    string longString(1000, 'A');  // 1000个字符
    const int iterations = 1000000;
    
    // 测试1: 按值传递
    auto start1 = high_resolution_clock::now();
    for (int i = 0; i < iterations; i++) {
        addStudent1(longString);
    }
    auto end1 = high_resolution_clock::now();
    auto duration1 = duration_cast<milliseconds>(end1 - start1);
    
    // 测试2: const引用传递
    auto start2 = high_resolution_clock::now();
    for (int i = 0; i < iterations; i++) {
        addStudent3(longString);
    }
    auto end2 = high_resolution_clock::now();
    auto duration2 = duration_cast<milliseconds>(end2 - start2);
    
    cout << "\n按值传递耗时: " << duration1.count() << " ms" << endl;
    cout << "const引用传递耗时: " << duration2.count() << " ms" << endl;
    cout << "性能提升: " << (duration1.count() - duration2.count()) << " ms" << endl;
    cout << "提升比例: " << (double)duration1.count() / duration2.count() << "x" << endl;
}

// ============================================
// 使用场景演示
// ============================================
void usage_scenarios() {
    cout << "\n" << string(60, '=') << endl;
    cout << "使用场景演示" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n场景1: 传递字符串字面量" << endl;
    cout << "---------------------------------------" << endl;
    // addStudent2("临时字符串");  // ❌ 编译错误！非const引用不能绑定临时对象
    addStudent3("临时字符串");     // ✅ 可以！const引用可以绑定临时对象
    
    cout << "\n场景2: 传递const字符串" << endl;
    cout << "---------------------------------------" << endl;
    const string constName = "const名字";
    // addStudent2(constName);  // ❌ 编译错误！非const引用不能绑定const对象
    addStudent3(constName);     // ✅ 可以！
    
    cout << "\n场景3: 传递函数返回的临时字符串" << endl;
    cout << "---------------------------------------" << endl;
    auto getName = []() { return string("返回的字符串"); };
    // addStudent2(getName());  // ❌ 编译错误！
    addStudent3(getName());     // ✅ 可以！
}

// ============================================
// 内存分析
// ============================================
void memory_analysis() {
    cout << "\n" << string(60, '=') << endl;
    cout << "内存和拷贝分析" << endl;
    cout << string(60, '=') << endl;
    
    string original = "Original String";
    
    cout << "\n按值传递的内存行为：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. 调用 addStudent1(original)" << endl;
    cout << "2. 创建 original 的副本" << endl;
    cout << "3. 副本传入函数" << endl;
    cout << "4. 函数结束，副本销毁" << endl;
    cout << "\n内存操作：" << endl;
    cout << "  - 分配新内存" << endl;
    cout << "  - 复制字符串内容" << endl;
    cout << "  - 释放内存" << endl;
    cout << "→ 开销大！" << endl;
    
    cout << "\n\nconst引用传递的内存行为：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. 调用 addStudent3(original)" << endl;
    cout << "2. 传递 original 的引用（地址）" << endl;
    cout << "3. 函数通过引用访问原对象" << endl;
    cout << "4. 函数结束，引用失效（原对象不变）" << endl;
    cout << "\n内存操作：" << endl;
    cout << "  - 只传递地址（8字节）" << endl;
    cout << "  - 不复制内容" << endl;
    cout << "  - 不分配/释放内存" << endl;
    cout << "→ 开销极小！" << endl;
}

// ============================================
// 最佳实践建议
// ============================================
void best_practices() {
    cout << "\n" << string(60, '=') << endl;
    cout << "最佳实践建议" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n何时使用 const string&？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ 函数参数接收字符串（推荐）" << endl;
    cout << "✅ 不需要修改参数" << endl;
    cout << "✅ 字符串可能较长" << endl;
    cout << "✅ 需要高性能" << endl;
    
    cout << "\n何时使用 string（按值）？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "⚠️  需要修改局部副本" << endl;
    cout << "⚠️  字符串很短（如1-2个字符）" << endl;
    cout << "⚠️  需要存储参数（移动语义）" << endl;
    
    cout << "\n何时使用 string&（非const引用）？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "⚠️  明确需要修改原字符串" << endl;
    cout << "⚠️  输出参数（out parameter）" << endl;
    
    cout << "\n推荐模式：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "// ✅ 推荐：const引用" << endl;
    cout << "void addStudent(const string& name, int id) {" << endl;
    cout << "    // name 不能被修改，效率高" << endl;
    cout << "}" << endl;
    cout << "\n// ⚠️  仅在需要时：按值" << endl;
    cout << "void processString(string name) {" << endl;
    cout << "    name += \" processed\";  // 修改副本" << endl;
    cout << "}" << endl;
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
    cout << "void addStudent(String name) {" << endl;
    cout << "    // String 是不可变的（immutable）" << endl;
    cout << "    // 传递的是引用（类似指针）" << endl;
    cout << "    // 但不能修改原字符串" << endl;
    cout << "}" << endl;
    cout << "\n→ Java的String天然就像const string&" << endl;
    
    cout << "\nKotlin：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "fun addStudent(name: String) {" << endl;
    cout << "    // String 也是不可变的" << endl;
    cout << "    // 传递引用" << endl;
    cout << "}" << endl;
    cout << "\n→ Kotlin的String也类似const string&" << endl;
    
    cout << "\nC++：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "void addStudent(const string& name) {" << endl;
    cout << "    // 显式指定const引用" << endl;
    cout << "    // 需要程序员控制" << endl;
    cout << "}" << endl;
    cout << "\n→ C++需要手动选择传递方式，更灵活但更复杂" << endl;
}

int main() {
    cout << "===========================================" << endl;
    cout << "  const string& 详解" << endl;
    cout << "===========================================" << endl;
    
    compare_methods();
    // performance_test();  // 取消注释查看性能测试
    usage_scenarios();
    memory_analysis();
    best_practices();
    compare_with_java_kotlin();
    
    cout << "\n===========================================" << endl;
    cout << "总结" << endl;
    cout << "===========================================" << endl;
    cout << "void addStudent(const string& name)" << endl;
    cout << "                 ^^^^^^^^^^^^^^" << endl;
    cout << "\nconst: 不能修改（安全）" << endl;
    cout << "&:     引用传递（高效，不复制）" << endl;
    cout << "\n这是C++中传递字符串的最佳实践！" << endl;
    cout << "既安全又高效！✅" << endl;
    cout << "===========================================" << endl;
    
    return 0;
}

/**
 * 快速参考表：
 * 
 * ┌──────────────────┬────────┬────────┬──────────┬────────────┐
 * │  传递方式         │  拷贝  │  修改  │  临时对象 │  推荐度    │
 * ├──────────────────┼────────┼────────┼──────────┼────────────┤
 * │ string name      │  是    │  副本  │  可以    │  ❌ 不推荐  │
 * │ string& name     │  否    │  原值  │  不可以  │  ⚠️  慎用   │
 * │ const string& n  │  否    │  不可  │  可以    │  ✅ 推荐   │
 * │ string* name     │  否    │  原值  │  不可以  │  ⚠️  慎用   │
 * └──────────────────┴────────┴────────┴──────────┴────────────┘
 * 
 * 记忆口诀：
 * - 传字符串，用const引用
 * - 既安全，又高效
 * - 不复制，不修改
 * - C++最佳实践
 */
