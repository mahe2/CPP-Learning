/**
 * const成员函数调用规则详解
 * 
 * 核心规则：
 * - const成员函数可以调用其他const成员函数
 * - const成员函数不能调用非const成员函数
 */

#include <iostream>
#include <string>

using namespace std;

class Person {
private:
    string name;
    mutable int accessCount;  // mutable允许在const函数中修改

public:
    Person(string n) : name(n), accessCount(0) {}
    
    // ============================================
    // 非const成员函数
    // ============================================
    void setName(string n) {
        name = n;
        cout << "setName() 被调用（非const函数）" << endl;
    }
    
    void increaseCount() {
        accessCount++;
        cout << "increaseCount() 被调用（非const函数）" << endl;
    }
    
    // ============================================
    // const成员函数
    // ============================================
    string getName() const {
        accessCount++;  // mutable成员可以修改
        cout << "getName() 被调用（const函数）" << endl;
        return name;
    }
    
    void display() const {
        cout << "display() 被调用（const函数）" << endl;
        cout << "Name: " << name << endl;
    }
    
    // ============================================
    // const函数调用其他函数
    // ============================================
    void constFunction() const {
        cout << "\n--- constFunction() 开始 ---" << endl;
        
        // ✅ 可以：调用其他const函数
        display();
        string n = getName();
        
        // ❌ 不可以：调用非const函数（编译错误）
        // setName("New Name");      // 编译错误！
        // increaseCount();          // 编译错误！
        
        cout << "--- constFunction() 结束 ---\n" << endl;
    }
    
    // ============================================
    // 非const函数可以调用任何函数
    // ============================================
    void nonConstFunction() {
        cout << "\n--- nonConstFunction() 开始 ---" << endl;
        
        // ✅ 可以：调用const函数
        display();
        string n = getName();
        
        // ✅ 可以：调用非const函数
        setName("Updated Name");
        increaseCount();
        
        cout << "--- nonConstFunction() 结束 ---\n" << endl;
    }
};

// ============================================
// 为什么const函数不能调用非const函数？
// ============================================
void explain_why() {
    cout << "\n" << string(60, '=') << endl;
    cout << "为什么const函数不能调用非const函数？" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n原因：逻辑一致性保证" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. const成员函数承诺：不修改对象状态" << endl;
    cout << "2. 非const成员函数可能：修改对象状态" << endl;
    cout << "3. 如果const函数调用非const函数" << endl;
    cout << "   → 就可能通过非const函数修改对象" << endl;
    cout << "   → 违反了const的承诺！" << endl;
    
    cout << "\n类比：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "const函数 = 只读模式" << endl;
    cout << "非const函数 = 读写模式" << endl;
    cout << "\n只读模式不能调用读写模式的函数" << endl;
    cout << "因为读写函数可能会修改数据！" << endl;
}

// ============================================
// 示例演示
// ============================================
void demo() {
    cout << "\n" << string(60, '=') << endl;
    cout << "示例演示" << endl;
    cout << string(60, '=') << endl;
    
    Person p("Alice");
    
    // 非const对象可以调用任何函数
    cout << "\n1. 非const对象 p:" << endl;
    p.display();           // ✅ 可以调用const函数
    p.setName("Bob");      // ✅ 可以调用非const函数
    p.nonConstFunction();  // ✅ 可以调用非const函数
    
    // const对象只能调用const函数
    cout << "\n2. const对象 cp:" << endl;
    const Person cp("Charlie");
    cp.display();          // ✅ 可以调用const函数
    cp.getName();          // ✅ 可以调用const函数
    // cp.setName("David"); // ❌ 编译错误！不能调用非const函数
    // cp.nonConstFunction(); // ❌ 编译错误！不能调用非const函数
    cp.constFunction();    // ✅ 可以调用const函数
}

// ============================================
// 编译错误示例（注释掉的）
// ============================================
class BadExample {
private:
    int value;

public:
    BadExample() : value(0) {}
    
    // 非const函数
    void modify() {
        value = 100;
    }
    
    // const函数尝试调用非const函数
    void constFunc() const {
        // modify();  // ❌ 编译错误！
        // error: passing 'const BadExample' as 'this' argument 
        // discards qualifiers
        
        cout << "constFunc不能调用modify()" << endl;
    }
};

// ============================================
// 总结规则
// ============================================
void summary_rules() {
    cout << "\n" << string(60, '=') << endl;
    cout << "const成员函数调用规则总结" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n规则表：" << endl;
    cout << "┌────────────────┬──────────────┬──────────────┐" << endl;
    cout << "│  调用者类型    │  const函数   │  非const函数 │" << endl;
    cout << "├────────────────┼──────────────┼──────────────┤" << endl;
    cout << "│ const函数      │  ✅ 可以     │  ❌ 不可以   │" << endl;
    cout << "│ 非const函数    │  ✅ 可以     │  ✅ 可以     │" << endl;
    cout << "│ const对象      │  ✅ 可以     │  ❌ 不可以   │" << endl;
    cout << "│ 非const对象    │  ✅ 可以     │  ✅ 可以     │" << endl;
    cout << "└────────────────┴──────────────┴──────────────┘" << endl;
    
    cout << "\n记忆口诀：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "const函数只能调const函数" << endl;
    cout << "非const函数随便调" << endl;
    cout << "const对象只能用const函数" << endl;
    cout << "非const对象随便用" << endl;
}

// ============================================
// 第10题详解
// ============================================
void question_10_explanation() {
    cout << "\n" << string(60, '=') << endl;
    cout << "第10题详解" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n题目：关于const成员函数，以下说法错误的是？" << endl;
    cout << "\nA. const成员函数不能修改成员变量 ✅ 正确" << endl;
    cout << "   → const函数不能修改普通成员变量" << endl;
    cout << "\nB. const对象只能调用const成员函数 ✅ 正确" << endl;
    cout << "   → const对象不能调用非const函数" << endl;
    cout << "\nC. const成员函数不能调用非const成员函数 ✅ 正确" << endl;
    cout << "   → const函数只能调用const函数" << endl;
    cout << "\nD. const成员函数可以有重载版本 ✅ 正确" << endl;
    cout << "   → 可以有const和非const两个版本" << endl;
    
    cout << "\n等等！所有选项都是正确的？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "题目问的是'错误的是哪个'" << endl;
    cout << "如果所有都正确，那题目本身可能有问题" << endl;
    cout << "\n让我检查D选项..." << endl;
}

// ============================================
// 重载版本示例
// ============================================
class OverloadExample {
private:
    int value;

public:
    OverloadExample(int v) : value(v) {}
    
    // const版本
    int getValue() const {
        cout << "调用const版本的getValue()" << endl;
        return value;
    }
    
    // 非const版本（重载）
    int getValue() {
        cout << "调用非const版本的getValue()" << endl;
        return value;
    }
};

void demo_overload() {
    cout << "\n" << string(60, '=') << endl;
    cout << "const函数重载示例" << endl;
    cout << string(60, '=') << endl;
    
    OverloadExample obj(42);
    const OverloadExample constObj(100);
    
    cout << "\n非const对象调用：" << endl;
    obj.getValue();      // 调用非const版本
    
    cout << "\nconst对象调用：" << endl;
    constObj.getValue(); // 调用const版本
    
    cout << "\n✅ 所以选项D也是正确的！const函数可以有重载版本" << endl;
}

int main() {
    cout << "===========================================" << endl;
    cout << "  const成员函数调用规则" << endl;
    cout << "===========================================" << endl;
    
    explain_why();
    demo();
    summary_rules();
    question_10_explanation();
    demo_overload();
    
    cout << "\n===========================================" << endl;
    cout << "结论" << endl;
    cout << "===========================================" << endl;
    cout << "选项C：'const成员函数不能调用非const成员函数'" << endl;
    cout << "这个说法是 ✅ 正确的！" << endl;
    cout << "\n但题目问的是'错误的是哪个'..." << endl;
    cout << "可能题目设计有误，或者D选项有特殊情况" << endl;
    cout << "\n标准答案应该是：所有选项都正确" << endl;
    cout << "或者题目想问的是其他意思" << endl;
    cout << "===========================================" << endl;
    
    return 0;
}
