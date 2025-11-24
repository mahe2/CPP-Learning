/**
 * 构造函数初始化方式：内部实现 vs 外部调用
 * 
 * 核心观点：
 * - 内部实现不同（体内赋值 vs 初始化列表）
 * - 外部调用相同（对用户透明）
 */

#include <iostream>
#include <string>

using namespace std;

// ============================================
// 方式1: 体内赋值
// ============================================
class Car_Style1 {
private:
    string brand;
    int year;

public:
    // 内部实现：体内赋值
    Car_Style1(string b, int y) {
        brand = b;
        year = y;
        cout << "Car_Style1 构造（体内赋值）: " << brand << endl;
    }
    
    void show() const {
        cout << "  Brand: " << brand << ", Year: " << year << endl;
    }
};

// ============================================
// 方式2: 初始化列表
// ============================================
class Car_Style2 {
private:
    string brand;
    int year;

public:
    // 内部实现：初始化列表
    Car_Style2(string b, int y) : brand(b), year(y) {
        cout << "Car_Style2 构造（初始化列表）: " << brand << endl;
    }
    
    void show() const {
        cout << "  Brand: " << brand << ", Year: " << year << endl;
    }
};

// ============================================
// 演示：调用方式完全相同
// ============================================
void demo_same_usage() {
    cout << "\n" << string(60, '=') << endl;
    cout << "外部调用：完全相同！" << endl;
    cout << string(60, '=') << endl;
    
    // 调用方式1: 直接初始化
    cout << "\n方式1 - 直接初始化:" << endl;
    Car_Style1 c1("Toyota", 2020);
    Car_Style2 c2("Honda", 2021);
    
    // 调用方式2: 使用 = 号
    cout << "\n方式2 - 等号初始化:" << endl;
    Car_Style1 c3 = Car_Style1("BMW", 2022);
    Car_Style2 c4 = Car_Style2("Audi", 2023);
    
    // 调用方式3: 堆上创建
    cout << "\n方式3 - 堆上创建:" << endl;
    Car_Style1* c5 = new Car_Style1("Tesla", 2024);
    Car_Style2* c6 = new Car_Style2("BYD", 2025);
    
    delete c5;
    delete c6;
    
    cout << "\n结论：外部使用者看不出任何区别！" << endl;
}

// ============================================
// 这是封装的体现
// ============================================
void encapsulation_demo() {
    cout << "\n" << string(60, '=') << endl;
    cout << "封装原则：隐藏实现细节" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n用户视角（外部调用）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "Car_Style1 c1(\"Toyota\", 2020);" << endl;
    cout << "Car_Style2 c2(\"Honda\", 2021);" << endl;
    cout << "                ↑" << endl;
    cout << "          调用方式完全相同" << endl;
    cout << "       用户根本不知道内部怎么实现的！" << endl;
    
    cout << "\n类设计者视角（内部实现）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "Style1: Car_Style1(string b, int y) {" << endl;
    cout << "            brand = b;  // 体内赋值" << endl;
    cout << "        }" << endl;
    cout << "\nStyle2: Car_Style2(string b, int y) : brand(b), year(y) {" << endl;
    cout << "            // 初始化列表" << endl;
    cout << "        }" << endl;
    cout << "                ↑" << endl;
    cout << "         内部实现不同" << endl;
    cout << "    但对外接口（构造函数签名）相同！" << endl;
}

// ============================================
// 与 Java/Kotlin 对比
// ============================================
void compare_with_java_kotlin() {
    cout << "\n" << string(60, '=') << endl;
    cout << "与 Java/Kotlin 对比" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\nJava: 只有体内赋值，但调用方式相同" << endl;
    cout << "---------------------------------------" << endl;
    cout << "// 类定义" << endl;
    cout << "class Car {" << endl;
    cout << "    public Car(String brand, int year) {" << endl;
    cout << "        this.brand = brand;  // 只能这样" << endl;
    cout << "        this.year = year;" << endl;
    cout << "    }" << endl;
    cout << "}" << endl;
    cout << "\n// 调用" << endl;
    cout << "Car c1 = new Car(\"Toyota\", 2020);  // 总是这样调用" << endl;
    
    cout << "\n\nKotlin: 主构造函数自动初始化" << endl;
    cout << "---------------------------------------" << endl;
    cout << "// 类定义（简洁！）" << endl;
    cout << "class Car(val brand: String, val year: Int)" << endl;
    cout << "\n// 调用" << endl;
    cout << "val c1 = Car(\"Toyota\", 2020)  // 调用方式相同" << endl;
    
    cout << "\n\nC++: 内部有两种实现方式，但调用方式相同" << endl;
    cout << "---------------------------------------" << endl;
    cout << "// 类定义方式1" << endl;
    cout << "Car(string b, int y) { brand = b; }" << endl;
    cout << "\n// 类定义方式2" << endl;
    cout << "Car(string b, int y) : brand(b), year(y) { }" << endl;
    cout << "\n// 调用：完全相同！" << endl;
    cout << "Car c1(\"Toyota\", 2020);" << endl;
    cout << "Car c2(\"Honda\", 2021);" << endl;
}

// ============================================
// 实际应用：可以随时切换实现方式
// ============================================
class Product {
private:
    string name;
    double price;
    int stock;

public:
    // 版本1: 体内赋值
    /*
    Product(string n, double p, int s) {
        name = n;
        price = p;
        stock = s;
    }
    */
    
    // 版本2: 初始化列表（改进后）
    // 用户代码不需要改动！
    Product(string n, double p, int s) : name(n), price(p), stock(s) {
        cout << "Product 构造: " << name << endl;
    }
    
    void show() const {
        cout << "  Name: " << name << ", Price: " << price << ", Stock: " << stock << endl;
    }
};

void refactoring_demo() {
    cout << "\n" << string(60, '=') << endl;
    cout << "实际应用：重构内部实现，外部调用不变" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n假设你最初用体内赋值实现 Product 类" << endl;
    cout << "后来学习后，想改用初始化列表（性能更好）" << endl;
    cout << "\n好消息：只需修改类内部，用户代码完全不受影响！" << endl;
    
    cout << "\n用户代码（始终不变）：" << endl;
    cout << "---------------------------------------" << endl;
    Product p1("Laptop", 999.99, 50);
    Product p2("Mouse", 29.99, 200);
    Product p3("Keyboard", 79.99, 150);
    
    p1.show();
    p2.show();
    p3.show();
    
    cout << "\n这就是封装的威力！" << endl;
}

// ============================================
// 构造函数签名决定调用方式
// ============================================
void signature_matters() {
    cout << "\n" << string(60, '=') << endl;
    cout << "构造函数签名决定调用方式" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n关键点：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "构造函数的 '签名' (参数列表) 决定如何调用" << endl;
    cout << "构造函数的 '实现' (体内 vs 列表) 对外不可见" << endl;
    
    cout << "\n示例：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "// 签名：ClassName(string, int)" << endl;
    cout << "// 调用：ClassName obj(\"text\", 123);" << endl;
    cout << "\n// 签名：ClassName()" << endl;
    cout << "// 调用：ClassName obj;" << endl;
    cout << "\n// 签名：ClassName(int)" << endl;
    cout << "// 调用：ClassName obj(456);" << endl;
    
    cout << "\n无论内部用体内赋值还是初始化列表，" << endl;
    cout << "调用方式只看签名！" << endl;
}

int main() {
    cout << "===========================================" << endl;
    cout << "  构造函数：内部实现 vs 外部调用" << endl;
    cout << "===========================================" << endl;
    
    demo_same_usage();
    encapsulation_demo();
    compare_with_java_kotlin();
    refactoring_demo();
    signature_matters();
    
    cout << "\n===========================================" << endl;
    cout << "核心总结" << endl;
    cout << "===========================================" << endl;
    cout << "✅ 两种初始化方式只是内部实现不同" << endl;
    cout << "✅ 外部调用方式完全相同" << endl;
    cout << "✅ 用户无需关心内部如何实现" << endl;
    cout << "✅ 可以随时重构内部实现，不影响用户代码" << endl;
    cout << "\n这就是面向对象的 '封装' 原则！" << endl;
    cout << "===========================================" << endl;
    
    return 0;
}

/**
 * 图解：内部实现 vs 外部调用
 * 
 *        类内部实现（设计者视角）
 *     ┌─────────────────────────────┐
 *     │ 方式1: 体内赋值              │
 *     │ Car(string b, int y) {      │
 *     │     brand = b;              │
 *     │     year = y;               │
 *     │ }                           │
 *     └─────────────┬───────────────┘
 *                   │
 *     ┌─────────────┴───────────────┐
 *     │ 方式2: 初始化列表            │
 *     │ Car(string b, int y)        │
 *     │     : brand(b), year(y) {   │
 *     │ }                           │
 *     └─────────────┬───────────────┘
 *                   │
 *                   │ 对外接口相同
 *                   │ (构造函数签名)
 *                   ↓
 *     ┌─────────────────────────────┐
 *     │   外部调用（用户视角）       │
 *     │                             │
 *     │ Car c1("Toyota", 2020);     │
 *     │ Car c2("Honda", 2021);      │
 *     │                             │
 *     │ 完全相同！看不出区别！       │
 *     └─────────────────────────────┘
 * 
 * 类比：
 * - 就像你点外卖，不关心厨房是用平底锅还是炒锅
 * - 你只关心：点什么菜（构造函数签名）
 * - 厨房内部怎么做（初始化方式），你看不见也不需要知道
 * 
 * 这就是封装！
 */
