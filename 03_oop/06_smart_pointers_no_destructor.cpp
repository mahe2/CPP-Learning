/**
 * 智能指针 vs 手动内存管理：析构函数的区别
 * 
 * 核心观点：
 * - 使用裸指针（new/delete）：必须写析构函数
 * - 使用智能指针：通常不需要写析构函数（Rule of Zero）
 */

#include <iostream>
#include <string>
#include <memory>
#include <vector>

using namespace std;

// ============================================
// 方式1: 裸指针（需要手动管理内存）❌
// ============================================
class Database_RawPointer {
private:
    string name;
    int* data;        // 裸指针
    size_t size;

public:
    Database_RawPointer(string n, size_t s) : name(n), size(s) {
        // 手动分配内存
        data = new int[size];
        cout << "🔧 Database_RawPointer 构造: " << name 
             << " (手动分配 " << size << " 个int)" << endl;
    }
    
    // ❌ 必须手动写析构函数！
    ~Database_RawPointer() {
        delete[] data;  // 必须手动释放
        cout << "💥 Database_RawPointer 析构: " << name 
             << " (手动释放内存)" << endl;
    }
    
    // ❌ 还需要写拷贝构造函数（Rule of Five）
    Database_RawPointer(const Database_RawPointer& other) 
        : name(other.name + "_copy"), size(other.size) {
        data = new int[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
        cout << "📋 拷贝构造: " << name << endl;
    }
    
    // ❌ 还需要写移动构造函数
    // ❌ 还需要写拷贝赋值运算符
    // ❌ 还需要写移动赋值运算符
    // 总共需要写 5 个特殊函数！（Rule of Five）
};

// ============================================
// 方式2: 智能指针（自动管理内存）✅
// ============================================
class Database_SmartPointer {
private:
    string name;
    unique_ptr<int[]> data;  // 智能指针
    size_t size;

public:
    Database_SmartPointer(string n, size_t s) 
        : name(n), data(make_unique<int[]>(s)), size(s) {
        cout << "🔧 Database_SmartPointer 构造: " << name 
             << " (智能指针自动管理)" << endl;
    }
    
    // ✅ 不需要写析构函数！
    // 智能指针会自动释放内存
    // ~Database_SmartPointer() { }  // 可以不写
    
    // ✅ 不需要写拷贝构造函数
    // ✅ 不需要写移动构造函数
    // ✅ 不需要写拷贝赋值运算符
    // ✅ 不需要写移动赋值运算符
    // 编译器自动生成所有这些函数！（Rule of Zero）
    
    // 当对象销毁时，智能指针自动释放
    ~Database_SmartPointer() {
        cout << "💥 Database_SmartPointer 析构: " << name 
             << " (智能指针自动释放)" << endl;
    }
};

// ============================================
// 方式3: 使用STL容器（更推荐）✅✅
// ============================================
class Database_Vector {
private:
    string name;
    vector<int> data;  // vector 自动管理内存

public:
    Database_Vector(string n, size_t s) 
        : name(n), data(s) {
        cout << "🔧 Database_Vector 构造: " << name 
             << " (vector自动管理)" << endl;
    }
    
    // ✅ 不需要写任何特殊函数！
    // vector 自动管理一切
    
    ~Database_Vector() {
        cout << "💥 Database_Vector 析构: " << name 
             << " (vector自动释放)" << endl;
    }
};

// ============================================
// 复杂示例：多个资源
// ============================================
class ComplexResource_Bad {
private:
    int* buffer1;
    int* buffer2;
    int* buffer3;

public:
    ComplexResource_Bad() {
        buffer1 = new int[100];
        buffer2 = new int[200];
        buffer3 = new int[300];
        cout << "❌ 手动管理3个资源" << endl;
    }
    
    ~ComplexResource_Bad() {
        delete[] buffer1;
        delete[] buffer2;
        delete[] buffer3;
        cout << "❌ 手动释放3个资源（容易遗漏！）" << endl;
    }
};

class ComplexResource_Good {
private:
    unique_ptr<int[]> buffer1;
    unique_ptr<int[]> buffer2;
    unique_ptr<int[]> buffer3;

public:
    ComplexResource_Good() 
        : buffer1(make_unique<int[]>(100)),
          buffer2(make_unique<int[]>(200)),
          buffer3(make_unique<int[]>(300)) {
        cout << "✅ 智能指针管理3个资源" << endl;
    }
    
    // ✅ 不需要写析构函数！
    // 所有智能指针自动释放
    
    ~ComplexResource_Good() {
        cout << "✅ 智能指针自动释放所有资源" << endl;
    }
};

// ============================================
// 什么时候还需要写析构函数？
// ============================================
class FileHandler {
private:
    string filename;
    FILE* file;  // C风格文件指针（不是智能指针能管理的）

public:
    FileHandler(string fname) : filename(fname) {
        file = fopen(filename.c_str(), "w");
        if (file) {
            cout << "📂 文件打开: " << filename << endl;
        }
    }
    
    // ⚠️  需要写析构函数！
    // 因为 FILE* 不是 C++ 对象，智能指针无法自动管理
    ~FileHandler() {
        if (file) {
            fclose(file);
            cout << "📂 文件关闭: " << filename << endl;
        }
    }
};

// 更好的方式：使用 RAII 封装
class FileHandler_Better {
private:
    string filename;
    unique_ptr<FILE, decltype(&fclose)> file;  // 自定义删除器

public:
    FileHandler_Better(string fname) 
        : filename(fname), 
          file(fopen(fname.c_str(), "w"), &fclose) {  // 指定关闭函数
        if (file) {
            cout << "📂 文件打开（智能指针管理）: " << filename << endl;
        }
    }
    
    // ✅ 不需要写析构函数！
    // 智能指针会自动调用 fclose
    
    ~FileHandler_Better() {
        cout << "📂 文件自动关闭: " << filename << endl;
    }
};

// ============================================
// Rule of Zero 示例
// ============================================
class Employee_RuleOfZero {
private:
    string name;
    int id;
    vector<string> projects;           // 自动管理
    unique_ptr<string> address;        // 自动管理
    shared_ptr<string> department;     // 自动管理

public:
    Employee_RuleOfZero(string n, int i) 
        : name(n), id(i), 
          address(make_unique<string>("Unknown")),
          department(make_shared<string>("IT")) {
        cout << "👤 Employee创建: " << name << endl;
    }
    
    // ✅ 完全不需要写析构函数！
    // ✅ 完全不需要写拷贝/移动函数！
    // 所有成员都会自动管理自己
    
    ~Employee_RuleOfZero() {
        cout << "👤 Employee销毁: " << name << " (自动清理)" << endl;
    }
};

// ============================================
// 演示和对比
// ============================================
void demo_memory_management() {
    cout << "\n" << string(60, '=') << endl;
    cout << "内存管理方式对比" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n--- 方式1: 裸指针（需要手动管理）---" << endl;
    {
        Database_RawPointer db1("RawDB", 10);
        // 离开作用域，自动调用析构函数
    }
    
    cout << "\n--- 方式2: 智能指针（自动管理）---" << endl;
    {
        Database_SmartPointer db2("SmartDB", 10);
        // 离开作用域，智能指针自动释放
    }
    
    cout << "\n--- 方式3: STL容器（最推荐）---" << endl;
    {
        Database_Vector db3("VectorDB", 10);
        // 离开作用域，vector自动释放
    }
}

void demo_complex_resources() {
    cout << "\n" << string(60, '=') << endl;
    cout << "多个资源管理对比" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n--- 手动管理（容易出错）---" << endl;
    {
        ComplexResource_Bad bad;
    }
    
    cout << "\n--- 智能指针管理（安全）---" << endl;
    {
        ComplexResource_Good good;
    }
}

void demo_rule_of_zero() {
    cout << "\n" << string(60, '=') << endl;
    cout << "Rule of Zero 演示" << endl;
    cout << string(60, '=') << endl;
    
    {
        Employee_RuleOfZero emp("张三", 1001);
    }
    
    cout << "\n所有资源自动释放，不需要写任何析构/拷贝/移动函数！" << endl;
}

// ============================================
// 最佳实践总结
// ============================================
void best_practices() {
    cout << "\n" << string(60, '=') << endl;
    cout << "最佳实践：何时需要写析构函数？" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n❌ 需要写析构函数的情况（老式代码）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. 使用裸指针 new/delete" << endl;
    cout << "2. 使用 malloc/free" << endl;
    cout << "3. 手动管理资源（文件、网络连接等）" << endl;
    cout << "   → 需要遵循 Rule of Five" << endl;
    
    cout << "\n✅ 不需要写析构函数的情况（现代C++）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. 使用 unique_ptr / shared_ptr" << endl;
    cout << "2. 使用 STL 容器（vector, string, map等）" << endl;
    cout << "3. 使用 RAII 封装的资源" << endl;
    cout << "   → 遵循 Rule of Zero" << endl;
    
    cout << "\n⚠️  特殊情况（仍需写析构函数）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. C风格资源（FILE*, socket等）" << endl;
    cout << "2. 第三方库资源（需要特定清理函数）" << endl;
    cout << "3. 线程、锁等系统资源" << endl;
    cout << "   → 但可以用智能指针+自定义删除器解决" << endl;
}

// ============================================
// 与 Java/Kotlin 对比
// ============================================
void compare_languages() {
    cout << "\n" << string(60, '=') << endl;
    cout << "与 Java/Kotlin 对比" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\nJava/Kotlin：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "- 完全没有析构函数概念" << endl;
    cout << "- GC自动回收内存" << endl;
    cout << "- 文件等资源用 try-with-resources" << endl;
    cout << "\nclass Database {" << endl;
    cout << "    private int[] data;" << endl;
    cout << "    " << endl;
    cout << "    // 不需要析构函数" << endl;
    cout << "    // GC会自动回收 data" << endl;
    cout << "}" << endl;
    
    cout << "\nC++（老式）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "class Database {" << endl;
    cout << "    int* data;" << endl;
    cout << "    " << endl;
    cout << "    ~Database() {" << endl;
    cout << "        delete[] data;  // 必须手动释放" << endl;
    cout << "    }" << endl;
    cout << "};" << endl;
    
    cout << "\nC++（现代）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "class Database {" << endl;
    cout << "    unique_ptr<int[]> data;" << endl;
    cout << "    // 或者" << endl;
    cout << "    vector<int> data;" << endl;
    cout << "    " << endl;
    cout << "    // 不需要写析构函数！" << endl;
    cout << "    // 智能指针/vector自动释放" << endl;
    cout << "};" << endl;
    
    cout << "\n总结：现代C++越来越像Java/Kotlin，不需要手动管理内存" << endl;
}

int main() {
    cout << "===========================================" << endl;
    cout << "  智能指针与析构函数" << endl;
    cout << "===========================================" << endl;
    
    demo_memory_management();
    demo_complex_resources();
    demo_rule_of_zero();
    best_practices();
    compare_languages();
    
    cout << "\n===========================================" << endl;
    cout << "核心结论" << endl;
    cout << "===========================================" << endl;
    cout << "✅ 使用智能指针 → 不需要写析构函数" << endl;
    cout << "✅ 使用STL容器 → 不需要写析构函数" << endl;
    cout << "✅ Rule of Zero → 让编译器自动管理一切" << endl;
    cout << "\n❌ 使用裸指针 → 必须写析构函数（Rule of Five）" << endl;
    cout << "\n现代C++推荐：智能指针 + STL容器" << endl;
    cout << "就像Java/Kotlin一样简单！" << endl;
    cout << "===========================================" << endl;
    
    return 0;
}

/**
 * 快速参考：
 * 
 * ┌────────────────────┬──────────────┬──────────────┬─────────────┐
 * │   内存管理方式      │  析构函数    │  Rule        │  推荐度     │
 * ├────────────────────┼──────────────┼──────────────┼─────────────┤
 * │ 裸指针 int*        │  ✅ 必须写   │  Rule of 5   │  ❌ 不推荐  │
 * │ unique_ptr         │  ❌ 不需要   │  Rule of 0   │  ✅ 推荐    │
 * │ shared_ptr         │  ❌ 不需要   │  Rule of 0   │  ✅ 推荐    │
 * │ vector/string      │  ❌ 不需要   │  Rule of 0   │  ✅✅ 最推荐 │
 * │ C风格资源(FILE*)   │  ⚠️  需要    │  特殊处理     │  ⚠️  慎用   │
 * └────────────────────┴──────────────┴──────────────┴─────────────┘
 * 
 * Rule of Zero (现代C++):
 * - 不要直接管理资源
 * - 使用智能指针和STL容器
 * - 让编译器自动生成所有特殊函数
 * - 不需要写析构函数、拷贝构造、移动构造等
 * 
 * Rule of Five (老式C++):
 * - 如果管理资源（裸指针），必须定义：
 *   1. 析构函数
 *   2. 拷贝构造函数
 *   3. 拷贝赋值运算符
 *   4. 移动构造函数
 *   5. 移动赋值运算符
 */
