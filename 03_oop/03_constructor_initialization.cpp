/**
 * 构造函数的两种初始化方式：
 * 1. 构造函数体内赋值
 * 2. 成员初始化列表（推荐）
 */

#include <iostream>
#include <string>

using namespace std;

// ============================================
// 方式1: 构造函数体内赋值（不推荐）
// ============================================
class Person_Style1 {
private:
    string name;
    int age;

public:
    // 构造函数体内赋值
    Person_Style1(string n, int a) {
        name = n;  // 赋值操作
        age = a;   // 赋值操作
        cout << "Style1: 体内赋值 - " << name << endl;
    }
    
    void show() const {
        cout << "  Name: " << name << ", Age: " << age << endl;
    }
};

// ============================================
// 方式2: 成员初始化列表（推荐）⭐
// ============================================
class Person_Style2 {
private:
    string name;
    int age;

public:
    // 成员初始化列表（冒号后面）
    Person_Style2(string n, int a) : name(n), age(a) {
        cout << "Style2: 初始化列表 - " << name << endl;
    }
    
    void show() const {
        cout << "  Name: " << name << ", Age: " << age << endl;
    }
};

// ============================================
// 两者的区别演示
// ============================================
class Demo {
private:
    string text;

public:
    // 方式1: 体内赋值
    Demo(string t) {
        cout << "  → Demo默认构造 string (空)" << endl;
        text = t;  // 赋值
        cout << "  → 然后赋值: " << t << endl;
    }
};

class Demo2 {
private:
    string text;

public:
    // 方式2: 初始化列表
    Demo2(string t) : text(t) {
        cout << "  → Demo2直接初始化 string: " << t << endl;
    }
};

void compare_initialization() {
    cout << "\n" << string(60, '=') << endl;
    cout << "两种方式的执行过程对比" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n方式1 - 体内赋值（两步）：" << endl;
    cout << "---------------------------------------" << endl;
    Demo d1("Hello");
    
    cout << "\n方式2 - 初始化列表（一步）：" << endl;
    cout << "---------------------------------------" << endl;
    Demo2 d2("World");
}

// ============================================
// 必须使用初始化列表的情况 ⭐⭐⭐
// ============================================
class MustUseInitList {
private:
    const int id;           // const 成员
    int& ref;               // 引用成员
    string name;            // 普通成员

public:
    // ✅ 正确：必须用初始化列表
    MustUseInitList(int i, int& r, string n) : id(i), ref(r), name(n) {
        // const 和引用必须在初始化列表中初始化
        // id = i;  // ❌ 错误！const 不能赋值
        // ref = r; // ❌ 错误！引用不能重新绑定
    }
    
    void show() const {
        cout << "  ID: " << id << ", Ref: " << ref << ", Name: " << name << endl;
    }
};

// ============================================
// 性能对比：复杂对象初始化
// ============================================
class ComplexObject {
private:
    string data;

public:
    ComplexObject() {
        cout << "    ComplexObject默认构造" << endl;
    }
    
    ComplexObject(string d) : data(d) {
        cout << "    ComplexObject参数构造: " << d << endl;
    }
    
    ComplexObject& operator=(const string& d) {
        data = d;
        cout << "    ComplexObject赋值: " << d << endl;
        return *this;
    }
};

class Container_Style1 {
private:
    ComplexObject obj;

public:
    Container_Style1(string s) {
        // 体内赋值：先默认构造，再赋值（低效）
        obj = s;
    }
};

class Container_Style2 {
private:
    ComplexObject obj;

public:
    Container_Style2(string s) : obj(s) {
        // 初始化列表：直接构造（高效）
    }
};

void performance_comparison() {
    cout << "\n" << string(60, '=') << endl;
    cout << "性能对比：复杂对象初始化" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n方式1 - 体内赋值（先默认构造，再赋值）：" << endl;
    cout << "---------------------------------------" << endl;
    Container_Style1 c1("Data1");
    
    cout << "\n方式2 - 初始化列表（直接构造）：" << endl;
    cout << "---------------------------------------" << endl;
    Container_Style2 c2("Data2");
}

// ============================================
// 初始化顺序：按声明顺序，不是初始化列表顺序！⚠️
// ============================================
class InitOrder {
private:
    int a;
    int b;
    int c;

public:
    // 初始化顺序：a -> b -> c（按声明顺序）
    // 不是 c -> b -> a（初始化列表顺序）
    InitOrder(int x) : c(x), b(c + 1), a(b + 1) {
        cout << "初始化顺序演示:" << endl;
        cout << "  a = " << a << " (未定义行为！a先初始化，但此时b还没初始化)" << endl;
        cout << "  b = " << b << " (未定义行为！b第二初始化，但此时c还没初始化)" << endl;
        cout << "  c = " << c << " (c最后初始化)" << endl;
    }
    
    // ✅ 正确写法：按声明顺序初始化
    InitOrder() : a(1), b(a + 1), c(b + 1) {
        cout << "正确的初始化:" << endl;
        cout << "  a = " << a << endl;
        cout << "  b = " << b << endl;
        cout << "  c = " << c << endl;
    }
};

// ============================================
// 与 Java/Kotlin 对比
// ============================================
void compare_with_java_kotlin() {
    cout << "\n" << string(60, '=') << endl;
    cout << "与 Java/Kotlin 对比" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\nJava（只有一种方式）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "class Person {" << endl;
    cout << "    private String name;" << endl;
    cout << "    private int age;" << endl;
    cout << "    " << endl;
    cout << "    public Person(String n, int a) {" << endl;
    cout << "        name = n;  // 只能在构造函数体内赋值" << endl;
    cout << "        age = a;" << endl;
    cout << "    }" << endl;
    cout << "}" << endl;
    
    cout << "\nKotlin（主构造函数自动初始化）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "class Person(val name: String, val age: Int) {" << endl;
    cout << "    // 自动初始化，不需要手动写" << endl;
    cout << "}" << endl;
    cout << "\n// 或者次构造函数：" << endl;
    cout << "class Person {" << endl;
    cout << "    val name: String" << endl;
    cout << "    val age: Int" << endl;
    cout << "    " << endl;
    cout << "    constructor(n: String, a: Int) {" << endl;
    cout << "        name = n  // 体内赋值" << endl;
    cout << "        age = a" << endl;
    cout << "    }" << endl;
    cout << "}" << endl;
    
    cout << "\nC++（两种方式）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "// 方式1: 体内赋值（类似 Java）" << endl;
    cout << "Person(string n, int a) {" << endl;
    cout << "    name = n;" << endl;
    cout << "    age = a;" << endl;
    cout << "}" << endl;
    cout << "\n// 方式2: 初始化列表（推荐，更高效）⭐" << endl;
    cout << "Person(string n, int a) : name(n), age(a) {" << endl;
    cout << "    // 已经初始化完成" << endl;
    cout << "}" << endl;
}

// ============================================
// 最佳实践建议
// ============================================
void best_practices() {
    cout << "\n" << string(60, '=') << endl;
    cout << "最佳实践建议" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n✅ 总是优先使用初始化列表：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. 性能更好（直接初始化，不是先默认构造再赋值）" << endl;
    cout << "2. const 成员必须用初始化列表" << endl;
    cout << "3. 引用成员必须用初始化列表" << endl;
    cout << "4. 没有默认构造函数的成员必须用初始化列表" << endl;
    cout << "5. 基类构造函数必须在初始化列表中调用" << endl;
    
    cout << "\n⚠️  注意事项：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. 初始化顺序由成员声明顺序决定，不是初始化列表顺序" << endl;
    cout << "2. 建议初始化列表顺序与声明顺序一致，避免混淆" << endl;
    cout << "3. 复杂逻辑可以在构造函数体内完成" << endl;
    
    cout << "\n❌ 体内赋值的问题：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. 性能差（两步：默认构造 + 赋值）" << endl;
    cout << "2. const 和引用成员无法使用" << endl;
    cout << "3. 对于复杂对象，会调用两次函数（构造 + 赋值）" << endl;
}

int main() {
    cout << "===========================================" << endl;
    cout << "  C++ 构造函数初始化方式对比" << endl;
    cout << "===========================================" << endl;
    
    // 基本对比
    cout << "\n--- 基本用法对比 ---" << endl;
    Person_Style1 p1("Alice", 25);
    p1.show();
    
    Person_Style2 p2("Bob", 30);
    p2.show();
    
    // 执行过程对比
    compare_initialization();
    
    // 必须使用初始化列表的情况
    cout << "\n" << string(60, '=') << endl;
    cout << "必须使用初始化列表的情况" << endl;
    cout << string(60, '=') << endl;
    int external = 100;
    MustUseInitList m(1, external, "Required");
    m.show();
    
    // 性能对比
    performance_comparison();
    
    // 初始化顺序警告
    cout << "\n" << string(60, '=') << endl;
    cout << "⚠️  初始化顺序陷阱" << endl;
    cout << string(60, '=') << endl;
    cout << "\n错误示例（初始化列表顺序与声明顺序不同）：" << endl;
    // InitOrder io1(10);  // 未定义行为！
    
    cout << "\n正确示例（按声明顺序初始化）：" << endl;
    InitOrder io2;
    
    // 与其他语言对比
    compare_with_java_kotlin();
    
    // 最佳实践
    best_practices();
    
    cout << "\n===========================================" << endl;
    cout << "总结" << endl;
    cout << "===========================================" << endl;
    cout << "Person(string n, int a) {         ← 体内赋值（不推荐）" << endl;
    cout << "    name = n;                     → 先默认构造，再赋值" << endl;
    cout << "}" << endl;
    cout << "\nPerson(string n, int a) : name(n), age(a) {  ← 初始化列表（推荐）⭐" << endl;
    cout << "    // 已初始化完成            → 直接构造，高效" << endl;
    cout << "}" << endl;
    cout << "\n记住：初始化列表是 C++ 的推荐写法！" << endl;
    cout << "===========================================" << endl;
    
    return 0;
}

/**
 * 快速参考：
 * 
 * ┌────────────────────┬──────────────┬──────────────┬─────────────┐
 * │     特性           │  体内赋值     │  初始化列表   │  必须用后者  │
 * ├────────────────────┼──────────────┼──────────────┼─────────────┤
 * │ 普通成员变量       │  ✅ 可以      │  ✅ 推荐     │  ❌         │
 * │ const 成员         │  ❌ 不可      │  ✅ 必须     │  ✅         │
 * │ 引用成员           │  ❌ 不可      │  ✅ 必须     │  ✅         │
 * │ 无默认构造的成员    │  ❌ 不可      │  ✅ 必须     │  ✅         │
 * │ 性能               │  较差（2步）   │  更好（1步）  │  ✅         │
 * │ 初始化顺序         │  写的顺序     │  声明顺序     │  ⚠️  注意   │
 * └────────────────────┴──────────────┴──────────────┴─────────────┘
 * 
 * 记忆要点：
 * 1. 初始化列表：冒号后面 : name(n), age(a)
 * 2. 体内赋值：大括号里面 { name = n; age = a; }
 * 3. 推荐：总是用初始化列表！
 * 4. Java 只有体内赋值，Kotlin 主构造自动初始化
 */
