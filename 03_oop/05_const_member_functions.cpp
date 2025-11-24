/*
 * const 成员函数详解
 * 
 * 函数名后的 const 表示：这个函数不会修改对象的成员变量
 */

#include <iostream>
#include <string>

using namespace std;

class Person {
private:
    string name;
    int age;
    mutable int accessCount;  // mutable 关键字：即使在 const 函数中也可以修改
    
public:
    Person(string n, int a) : name(n), age(a), accessCount(0) {}
    
    // ========== const 成员函数 ==========
    // 承诺不修改对象状态
    void display() const {
        cout << "姓名: " << name << ", 年龄: " << age << endl;
        
        // ✅ 可以读取成员变量
        cout << "访问次数: " << accessCount << endl;
        
        // ✅ 可以修改 mutable 成员
        accessCount++;  // mutable 成员例外
        
        // ❌ 不能修改普通成员变量
        // name = "新名字";  // 编译错误！
        // age = 100;        // 编译错误！
    }
    
    string getName() const {
        return name;  // ✅ 只读取，不修改
    }
    
    int getAge() const {
        return age;   // ✅ 只读取，不修改
    }
    
    // 计算方法，不修改状态
    bool isAdult() const {
        return age >= 18;  // ✅ 只读取，不修改
    }
    
    // ========== 非 const 成员函数 ==========
    // 可以修改对象状态
    void setName(const string& n) {
        name = n;  // ✅ 可以修改
    }
    
    void setAge(int a) {
        age = a;  // ✅ 可以修改
    }
    
    void celebrateBirthday() {
        age++;  // ✅ 修改年龄
        cout << name << " 过生日了！现在 " << age << " 岁" << endl;
    }
    
    // ========== const 重载 ==========
    // 可以根据对象是否为 const 提供不同版本
    
    // 非 const 版本：返回可修改的引用
    string& getNameRef() {
        cout << "调用非 const 版本" << endl;
        return name;
    }
    
    // const 版本：返回只读引用
    const string& getNameRef() const {
        cout << "调用 const 版本" << endl;
        return name;
    }
};

// ========== 演示 const 对象 ==========
void demonstrateConstObject() {
    cout << "\n=== const 对象演示 ===" << endl;
    
    // 普通对象
    Person p1("张三", 25);
    p1.display();           // ✅ 可以调用 const 函数
    p1.setName("李四");      // ✅ 可以调用非 const 函数
    p1.celebrateBirthday(); // ✅ 可以调用非 const 函数
    
    cout << "\n--- const 对象 ---" << endl;
    
    // const 对象
    const Person p2("王五", 30);
    p2.display();           // ✅ 可以调用 const 函数
    cout << "年龄: " << p2.getAge() << endl;  // ✅ 可以调用 const 函数
    
    // ❌ 不能调用非 const 函数
    // p2.setName("赵六");      // 编译错误！
    // p2.celebrateBirthday(); // 编译错误！
}

// ========== const 重载演示 ==========
void demonstrateConstOverload() {
    cout << "\n=== const 重载演示 ===" << endl;
    
    Person p1("Alice", 20);
    const Person p2("Bob", 25);
    
    // 普通对象调用非 const 版本
    string& ref1 = p1.getNameRef();
    ref1 = "Modified Alice";  // ✅ 可以修改
    p1.display();
    
    // const 对象调用 const 版本
    const string& ref2 = p2.getNameRef();
    cout << "Bob 的名字: " << ref2 << endl;
    // ref2 = "Modified Bob";  // ❌ 编译错误！const 引用不能修改
}

// ========== const 参数传递 ==========
// const 引用参数：高效且安全
void printPerson(const Person& p) {  // const 引用：不复制，不修改
    cout << "\n打印人员信息：" << endl;
    p.display();        // ✅ 可以调用 const 函数
    // p.setAge(100);   // ❌ 编译错误！不能调用非 const 函数
}

// ========== 实际应用示例 ==========
class BankAccount {
private:
    string accountNumber;
    double balance;
    mutable int queryCount;  // 查询次数（统计用，不算状态改变）
    
public:
    BankAccount(string num, double bal) 
        : accountNumber(num), balance(bal), queryCount(0) {}
    
    // const 函数：查询操作，不修改账户状态
    double getBalance() const {
        queryCount++;  // mutable 成员可以修改
        return balance;
    }
    
    string getAccountNumber() const {
        return accountNumber;
    }
    
    bool canWithdraw(double amount) const {
        return balance >= amount;  // 只读取，不修改
    }
    
    void displayInfo() const {
        cout << "账号: " << accountNumber 
             << ", 余额: " << balance 
             << ", 查询次数: " << queryCount << endl;
    }
    
    // 非 const 函数：修改操作
    void deposit(double amount) {
        balance += amount;  // 修改余额
    }
    
    bool withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;  // 修改余额
            return true;
        }
        return false;
    }
};

// ========== 类比 Java/Kotlin ==========
void javaKotlinComparison() {
    cout << "\n=== Java/Kotlin 对比 ===" << endl;
    
    cout << "C++:" << endl;
    cout << "  void display() const { }  // 承诺不修改对象" << endl;
    
    cout << "\nJava:" << endl;
    cout << "  // 没有直接对应的语法" << endl;
    cout << "  // 需要通过代码规范或注释说明" << endl;
    
    cout << "\nKotlin:" << endl;
    cout << "  val name: String  // 不可变属性（类似效果）" << endl;
    cout << "  fun getName() = name  // 只读，不修改" << endl;
}

int main() {
    cout << "=== const 成员函数详解 ===" << endl;
    
    // 1. const 对象演示
    demonstrateConstObject();
    
    // 2. const 重载演示
    demonstrateConstOverload();
    
    // 3. const 引用参数
    Person p("Charlie", 35);
    printPerson(p);
    
    // 4. 实际应用：银行账户
    cout << "\n=== 银行账户示例 ===" << endl;
    BankAccount account("123456", 1000.0);
    
    // const 函数：查询
    account.displayInfo();
    cout << "余额: " << account.getBalance() << endl;
    cout << "可以取款500吗? " << (account.canWithdraw(500) ? "是" : "否") << endl;
    
    // 非 const 函数：修改
    account.deposit(500);
    account.withdraw(200);
    account.displayInfo();
    
    // const 对象只能调用 const 函数
    const BankAccount constAccount("789012", 2000.0);
    constAccount.displayInfo();  // ✅ const 函数
    // constAccount.deposit(100); // ❌ 编译错误！
    
    // 5. Java/Kotlin 对比
    javaKotlinComparison();
    
    return 0;
}

/*
 * ========== 总结 ==========
 * 
 * 1. const 成员函数语法：
 *    void functionName() const { }
 *    ^                   ^^^^^
 *    返回类型             const 修饰符
 * 
 * 2. const 成员函数的作用：
 *    - 承诺不修改对象的成员变量
 *    - 允许 const 对象调用
 *    - 提高代码安全性
 *    - 表达设计意图
 * 
 * 3. const 成员函数的规则：
 *    ✅ 可以读取成员变量
 *    ✅ 可以修改 mutable 成员
 *    ✅ 可以调用其他 const 函数
 *    ❌ 不能修改普通成员变量
 *    ❌ 不能调用非 const 函数
 * 
 * 4. const 对象的限制：
 *    const Person p("张三", 25);
 *    p.display();     // ✅ 可以调用 const 函数
 *    p.setAge(30);    // ❌ 不能调用非 const 函数
 * 
 * 5. mutable 关键字：
 *    mutable int count;  // 即使在 const 函数中也可以修改
 *    用于：缓存、统计、调试信息等辅助数据
 * 
 * 6. const 重载：
 *    string& getName();        // 非 const 版本
 *    const string& getName() const;  // const 版本
 *    根据对象是否为 const 调用不同版本
 * 
 * 
 * ========== 使用建议 ==========
 * 
 * 1. 何时使用 const 成员函数：
 *    - Getter 方法（只读取，不修改）
 *    - 查询方法（如 isEmpty(), size()）
 *    - 计算方法（基于现有数据计算，不改变状态）
 *    - 显示/打印方法
 * 
 * 2. 何时不用 const：
 *    - Setter 方法（修改成员变量）
 *    - 改变对象状态的方法
 * 
 * 3. 最佳实践：
 *    - 尽可能多地使用 const（const-correctness）
 *    - 所有不修改对象的函数都应该加 const
 *    - 参数传递优先使用 const 引用
 * 
 * 
 * ========== 对比表 ==========
 * 
 * ┌────────────────────┬──────────────┬──────────────────┐
 * │      特性          │  const 函数  │   非 const 函数  │
 * ├────────────────────┼──────────────┼──────────────────┤
 * │ 修改成员变量       │  ❌          │  ✅              │
 * │ const 对象调用     │  ✅          │  ❌              │
 * │ 普通对象调用       │  ✅          │  ✅              │
 * │ 调用 const 函数    │  ✅          │  ✅              │
 * │ 调用非 const 函数  │  ❌          │  ✅              │
 * │ 修改 mutable 成员  │  ✅          │  ✅              │
 * └────────────────────┴──────────────┴──────────────────┘
 * 
 * 
 * ========== Java/Kotlin 对比 ==========
 * 
 * C++:
 * class Person {
 *     int age;
 * public:
 *     int getAge() const { return age; }  // 明确标记不修改
 *     void setAge(int a) { age = a; }     // 可以修改
 * };
 * 
 * Java:
 * class Person {
 *     private int age;
 *     
 *     public int getAge() { return age; }  // 约定：getter 不修改
 *     public void setAge(int age) { this.age = age; }
 * }
 * // Java 没有 const 成员函数，靠命名约定
 * 
 * Kotlin:
 * class Person(var age: Int) {
 *     fun getAge() = age  // 默认不修改（函数式风格）
 * }
 * // Kotlin 的 val 属性类似 const，但是属性级别的
 * 
 * 
 * ========== 记忆口诀 ==========
 * 
 * const 在后面，承诺不改变
 * 只能看不能摸，const 对象安全多
 * getter 加 const，表达设计好
 */
