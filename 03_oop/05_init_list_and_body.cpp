/**
 * 初始化列表 + 构造函数体：分工明确
 * 
 * 核心概念：
 * - 初始化列表：负责成员变量的初始化
 * - 构造函数体：负责额外的逻辑处理
 */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

// ============================================
// 示例1: 初始化后的验证和处理
// ============================================
class BankAccount {
private:
    string accountNumber;
    double balance;
    string owner;
    bool isActive;

public:
    // 初始化列表 + 构造函数体内逻辑
    BankAccount(string accNum, double bal, string own) 
        : accountNumber(accNum), balance(bal), owner(own), isActive(false) {
        
        // ✅ 构造函数体内可以写其他逻辑！
        
        // 1. 数据验证
        if (balance < 0) {
            cout << "⚠️  警告：余额为负数，设置为0" << endl;
            balance = 0;
        }
        
        // 2. 数据处理
        if (accountNumber.length() < 10) {
            accountNumber = "ACC" + accountNumber;
            cout << "ℹ️  账号补全为: " << accountNumber << endl;
        }
        
        // 3. 状态设置
        if (balance >= 100) {
            isActive = true;
            cout << "✅ 账户激活成功" << endl;
        } else {
            cout << "ℹ️  余额不足，账户未激活" << endl;
        }
        
        // 4. 日志记录
        cout << "📝 账户创建: " << owner << ", 余额: " << balance << endl;
    }
    
    void show() const {
        cout << "  账号: " << accountNumber 
             << ", 拥有者: " << owner 
             << ", 余额: " << balance
             << ", 状态: " << (isActive ? "激活" : "未激活") << endl;
    }
};

// ============================================
// 示例2: 复杂计算和初始化
// ============================================
class Rectangle {
private:
    double width;
    double height;
    double area;        // 需要计算得出
    double perimeter;   // 需要计算得出

public:
    // 初始化列表：初始化基础数据
    // 构造函数体：计算派生数据
    Rectangle(double w, double h) : width(w), height(h) {
        // 在构造函数体内计算派生属性
        area = width * height;
        perimeter = 2 * (width + height);
        
        cout << "📐 矩形创建: " << width << " x " << height << endl;
        cout << "   面积: " << area << ", 周长: " << perimeter << endl;
    }
    
    void show() const {
        cout << "  矩形: " << width << "×" << height 
             << ", 面积=" << area << ", 周长=" << perimeter << endl;
    }
};

// ============================================
// 示例3: 资源分配和初始化
// ============================================
class Database {
private:
    string connectionString;
    bool isConnected;
    vector<string> tables;

public:
    Database(string connStr) : connectionString(connStr), isConnected(false) {
        // 构造函数体内：资源分配和连接
        cout << "🔌 正在连接数据库..." << endl;
        
        // 模拟连接过程
        if (!connectionString.empty()) {
            isConnected = true;
            cout << "✅ 数据库连接成功" << endl;
            
            // 加载表信息
            tables.push_back("users");
            tables.push_back("products");
            tables.push_back("orders");
            cout << "📋 加载了 " << tables.size() << " 张表" << endl;
        } else {
            cout << "❌ 连接失败：连接字符串为空" << endl;
        }
    }
    
    ~Database() {
        if (isConnected) {
            cout << "🔌 断开数据库连接" << endl;
        }
    }
    
    void show() const {
        cout << "  数据库连接: " << (isConnected ? "已连接" : "未连接") << endl;
        cout << "  表数量: " << tables.size() << endl;
    }
};

// ============================================
// 示例4: 默认值和条件初始化
// ============================================
class User {
private:
    string username;
    string email;
    int age;
    string role;
    bool emailVerified;

public:
    User(string name, string mail, int userAge) 
        : username(name), email(mail), age(userAge), emailVerified(false) {
        
        // 构造函数体内：根据条件设置其他属性
        
        // 1. 根据年龄设置权限
        if (age < 18) {
            role = "minor";
            cout << "👶 未成年用户，权限受限" << endl;
        } else if (age < 65) {
            role = "adult";
            cout << "👤 成年用户" << endl;
        } else {
            role = "senior";
            cout << "👴 老年用户，享受优惠" << endl;
        }
        
        // 2. 验证邮箱格式
        if (email.find('@') != string::npos) {
            cout << "📧 邮箱格式有效" << endl;
            // 模拟发送验证邮件
            cout << "📤 发送验证邮件到: " << email << endl;
        } else {
            cout << "⚠️  邮箱格式无效" << endl;
        }
        
        // 3. 欢迎消息
        cout << "👋 欢迎, " << username << "!" << endl;
    }
    
    void show() const {
        cout << "  用户: " << username << ", 角色: " << role 
             << ", 年龄: " << age << endl;
    }
};

// ============================================
// 示例5: 错误处理和异常
// ============================================
class FileReader {
private:
    string filename;
    bool isOpen;
    vector<string> lines;

public:
    FileReader(string fname) : filename(fname), isOpen(false) {
        // 构造函数体内：文件操作和错误处理
        cout << "📂 尝试打开文件: " << filename << endl;
        
        // 模拟文件操作
        if (filename.empty()) {
            cout << "❌ 错误：文件名为空" << endl;
            return;
        }
        
        if (filename.find(".txt") == string::npos) {
            cout << "⚠️  警告：不是txt文件" << endl;
        }
        
        // 模拟成功打开
        isOpen = true;
        lines.push_back("第一行数据");
        lines.push_back("第二行数据");
        cout << "✅ 文件打开成功，读取了 " << lines.size() << " 行" << endl;
    }
    
    void show() const {
        cout << "  文件: " << filename << ", 状态: " 
             << (isOpen ? "打开" : "关闭") << endl;
    }
};

// ============================================
// 对比：只用初始化列表 vs 初始化列表+函数体
// ============================================
class Simple {
private:
    int x;
    int y;

public:
    // 只用初始化列表（简单情况）
    Simple(int a, int b) : x(a), y(b) {
        // 空函数体 - 不需要额外逻辑
    }
};

class Complex {
private:
    int x;
    int y;
    int sum;
    int product;

public:
    // 初始化列表 + 函数体（复杂情况）
    Complex(int a, int b) : x(a), y(b) {
        // 函数体内计算派生值
        sum = x + y;
        product = x * y;
        
        // 打印调试信息
        cout << "Complex对象创建: x=" << x << ", y=" << y 
             << ", sum=" << sum << ", product=" << product << endl;
    }
};

// ============================================
// 最佳实践总结
// ============================================
void best_practices() {
    cout << "\n" << string(60, '=') << endl;
    cout << "最佳实践：如何使用初始化列表和构造函数体" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n初始化列表的职责（必须在这里做）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. ✅ 初始化所有成员变量（尤其是const和引用）" << endl;
    cout << "2. ✅ 调用基类构造函数" << endl;
    cout << "3. ✅ 初始化成员对象" << endl;
    
    cout << "\n构造函数体的职责（可以在这里做）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. ✅ 数据验证和边界检查" << endl;
    cout << "2. ✅ 计算派生属性" << endl;
    cout << "3. ✅ 资源分配（打开文件、连接数据库等）" << endl;
    cout << "4. ✅ 日志记录和调试输出" << endl;
    cout << "5. ✅ 复杂的条件逻辑" << endl;
    cout << "6. ✅ 调用其他成员函数" << endl;
    cout << "7. ✅ 错误处理" << endl;
    
    cout << "\n错误示例（不要这样做）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "❌ 在构造函数体内初始化const成员（应该用初始化列表）" << endl;
    cout << "❌ 在构造函数体内初始化引用成员（应该用初始化列表）" << endl;
    cout << "❌ 让成员先默认构造再赋值（应该用初始化列表直接构造）" << endl;
}

// ============================================
// 与 Java/Kotlin 对比
// ============================================
void compare_languages() {
    cout << "\n" << string(60, '=') << endl;
    cout << "与 Java/Kotlin 对比" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\nJava（只能在构造函数体内）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "class BankAccount {" << endl;
    cout << "    public BankAccount(String accNum, double bal) {" << endl;
    cout << "        // 初始化成员" << endl;
    cout << "        this.accountNumber = accNum;" << endl;
    cout << "        this.balance = bal;" << endl;
    cout << "        " << endl;
    cout << "        // 额外逻辑（混在一起）" << endl;
    cout << "        if (balance < 0) {" << endl;
    cout << "            balance = 0;" << endl;
    cout << "        }" << endl;
    cout << "    }" << endl;
    cout << "}" << endl;
    
    cout << "\nKotlin（init块）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "class BankAccount(val accNum: String, var bal: Double) {" << endl;
    cout << "    // 主构造函数自动初始化" << endl;
    cout << "    " << endl;
    cout << "    init {  // 类似C++的构造函数体" << endl;
    cout << "        if (bal < 0) {" << endl;
    cout << "            bal = 0.0" << endl;
    cout << "        }" << endl;
    cout << "        println(\"账户创建\")" << endl;
    cout << "    }" << endl;
    cout << "}" << endl;
    
    cout << "\nC++（分工明确）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "BankAccount(string accNum, double bal)" << endl;
    cout << "    : accountNumber(accNum), balance(bal) {  // 初始化列表" << endl;
    cout << "    " << endl;
    cout << "    // 构造函数体：额外逻辑" << endl;
    cout << "    if (balance < 0) {" << endl;
    cout << "        balance = 0;" << endl;
    cout << "    }" << endl;
    cout << "}" << endl;
    
    cout << "\n对比总结：" << endl;
    cout << "- Java: 初始化和逻辑混在一起" << endl;
    cout << "- Kotlin: 主构造自动初始化 + init块处理逻辑" << endl;
    cout << "- C++: 初始化列表（初始化）+ 构造函数体（逻辑），分工最明确" << endl;
}

int main() {
    cout << "===========================================" << endl;
    cout << "  初始化列表 + 构造函数体" << endl;
    cout << "===========================================" << endl;
    
    cout << "\n--- 示例1: 数据验证和处理 ---" << endl;
    BankAccount acc1("12345", 150.0, "张三");
    acc1.show();
    
    cout << "\n--- 示例2: 复杂计算 ---" << endl;
    Rectangle rect(5.0, 3.0);
    rect.show();
    
    cout << "\n--- 示例3: 资源分配 ---" << endl;
    Database db("mysql://localhost:3306");
    db.show();
    
    cout << "\n--- 示例4: 条件初始化 ---" << endl;
    User user1("Alice", "alice@example.com", 16);
    user1.show();
    
    cout << "\n--- 示例5: 文件操作 ---" << endl;
    FileReader file("data.txt");
    file.show();
    
    cout << "\n--- 示例6: 简单vs复杂 ---" << endl;
    Simple s(10, 20);
    Complex c(10, 20);
    
    best_practices();
    compare_languages();
    
    cout << "\n===========================================" << endl;
    cout << "核心总结" << endl;
    cout << "===========================================" << endl;
    cout << "构造函数 = 初始化列表 + 函数体" << endl;
    cout << "\n初始化列表：负责成员变量的初始化" << endl;
    cout << "  ClassName(params) : member1(val1), member2(val2)" << endl;
    cout << "\n构造函数体：负责额外的逻辑处理" << endl;
    cout << "  {" << endl;
    cout << "      // 验证、计算、资源分配、日志等" << endl;
    cout << "  }" << endl;
    cout << "\n✅ 两者结合使用，分工明确，代码清晰！" << endl;
    cout << "===========================================" << endl;
    
    return 0;
}

/**
 * 完整示例对比：
 * 
 * ┌─────────────────────────────────────────────────────────────┐
 * │ 构造函数的完整结构                                            │
 * ├─────────────────────────────────────────────────────────────┤
 * │                                                             │
 * │  ClassName(参数) : 成员1(值1), 成员2(值2)  ← 初始化列表      │
 * │  {                                          ↑               │
 * │      // 数据验证                            只能在这里初始化  │
 * │      if (成员1 < 0) {                       const和引用成员  │
 * │          成员1 = 0;                                         │
 * │      }                                                      │
 * │                                                             │
 * │      // 计算派生属性                       ← 构造函数体       │
 * │      面积 = 长 * 宽;                        可以写任何逻辑    │
 * │                                                             │
 * │      // 资源分配                                             │
 * │      打开文件();                                             │
 * │                                                             │
 * │      // 日志记录                                             │
 * │      cout << "对象创建";                                     │
 * │  }                                                          │
 * └─────────────────────────────────────────────────────────────┘
 * 
 * 记忆口诀：
 * - 初始化列表：给成员赋初值
 * - 构造函数体：做额外的事
 * - 两者结合：强大又灵活
 */
