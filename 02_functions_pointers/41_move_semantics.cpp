/*
 * C++ 移动语义详解
 * 问题: C++ 中的移动语义是指什么？Java 中是否有对应的概念？
 * 答案: 移动语义是 C++11 引入的性能优化特性，Java 没有对应概念
 * 核心: 通过"转移资源所有权"代替"拷贝资源"，避免不必要的开销
 */

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <chrono>
using namespace std;

// ============================================================================
// 一、什么是移动语义？
// ============================================================================

void what_is_move_semantics() {
    cout << "\n=== 1. 什么是移动语义？===" << endl;
    
    cout << "\n核心概念：" << endl;
    cout << "  移动语义（Move Semantics）= 转移资源所有权，而不是拷贝资源" << endl;
    
    cout << "\n为什么需要移动语义？" << endl;
    cout << "  问题：拷贝大对象（如动态数组、大字符串）开销很大" << endl;
    cout << "  - 需要分配新内存" << endl;
    cout << "  - 需要复制所有数据" << endl;
    cout << "  - 对于临时对象，拷贝后立即销毁，纯属浪费" << endl;
    
    cout << "\n解决方案：移动语义" << endl;
    cout << "  - 不拷贝数据，直接\"偷\"走资源指针" << endl;
    cout << "  - 原对象变为空壳（nullptr）" << endl;
    cout << "  - 性能提升：O(1) vs O(n)" << endl;
    
    cout << "\n类比：搬家" << endl;
    cout << "  拷贝语义 = 复制所有家具到新家（耗时耗力）" << endl;
    cout << "  移动语义 = 直接把家具搬到新家（只改地址）" << endl;
}

// ============================================================================
// 二、左值 vs 右值
// ============================================================================

void demonstrate_lvalue_rvalue() {
    cout << "\n=== 2. 左值 vs 右值（理解移动语义的基础）===" << endl;
    
    cout << "\n左值（lvalue）：" << endl;
    cout << "  - 有名字、有地址的表达式" << endl;
    cout << "  - 可以取地址（&x 合法）" << endl;
    cout << "  - 生命周期较长，超出表达式" << endl;
    
    int x = 10;
    int* ptr = &x;  // x 是左值，可以取地址
    cout << "  int x = 10;        // x 是左值" << endl;
    cout << "  int* ptr = &x;     // 可以取地址" << endl;
    
    cout << "\n右值（rvalue）：" << endl;
    cout << "  - 临时对象、字面量、表达式计算结果" << endl;
    cout << "  - 不能取地址" << endl;
    cout << "  - 生命周期短，表达式结束就销毁" << endl;
    
    // int* ptr2 = &10;     // ❌ 编译错误：10 是右值，不能取地址
    // int* ptr3 = &(x+1);  // ❌ 编译错误：x+1 是右值
    cout << "  int y = x + 1;     // x+1 是右值" << endl;
    cout << "  int z = 42;        // 42 是右值" << endl;
    cout << "  // &10;            // ❌ 编译错误：不能取右值的地址" << endl;
    
    cout << "\n右值引用（C++11 新特性）：" << endl;
    cout << "  语法：Type&& name" << endl;
    cout << "  作用：绑定到右值，延长其生命周期" << endl;
    
    int&& rvalueRef = 10;     // 右值引用，绑定到字面量
    int&& rvalueRef2 = x + 1; // 右值引用，绑定到临时结果
    cout << "  int&& ref = 10;    // 右值引用" << endl;
    cout << "  ref 的值: " << rvalueRef << endl;
    
    cout << "\n对比：" << endl;
    cout << "  int& lref = x;     // 左值引用，绑定左值" << endl;
    cout << "  int&& rref = 10;   // 右值引用，绑定右值" << endl;
    cout << "  // int& lref2 = 10; // ❌ 左值引用不能绑定右值" << endl;
}

// ============================================================================
// 三、自定义类：拷贝 vs 移动
// ============================================================================

class MyString {
private:
    char* data;
    size_t length;

public:
    // 构造函数
    MyString(const char* str = "") {
        length = strlen(str);
        data = new char[length + 1];
        strcpy(data, str);
        cout << "  [构造] 创建字符串: \"" << data << "\"" << endl;
    }
    
    // 拷贝构造函数（深拷贝）
    MyString(const MyString& other) {
        length = other.length;
        data = new char[length + 1];  // 分配新内存
        strcpy(data, other.data);     // 拷贝数据
        cout << "  [拷贝构造] 拷贝字符串: \"" << data << "\" (开销大！)" << endl;
    }
    
    // 移动构造函数（C++11）
    MyString(MyString&& other) noexcept {
        data = other.data;      // 直接"偷"走指针
        length = other.length;
        
        other.data = nullptr;   // 原对象变空
        other.length = 0;
        
        cout << "  [移动构造] 移动字符串: \"" << data << "\" (零开销！)" << endl;
    }
    
    // 拷贝赋值运算符
    MyString& operator=(const MyString& other) {
        if (this != &other) {
            delete[] data;
            length = other.length;
            data = new char[length + 1];
            strcpy(data, other.data);
            cout << "  [拷贝赋值] 拷贝字符串: \"" << data << "\"" << endl;
        }
        return *this;
    }
    
    // 移动赋值运算符（C++11）
    MyString& operator=(MyString&& other) noexcept {
        if (this != &other) {
            delete[] data;
            
            data = other.data;
            length = other.length;
            
            other.data = nullptr;
            other.length = 0;
            
            cout << "  [移动赋值] 移动字符串: \"" << data << "\"" << endl;
        }
        return *this;
    }
    
    // 析构函数
    ~MyString() {
        if (data) {
            cout << "  [析构] 释放字符串: \"" << data << "\"" << endl;
            delete[] data;
        } else {
            cout << "  [析构] 释放空对象" << endl;
        }
    }
    
    const char* c_str() const { return data ? data : ""; }
};

void demonstrate_copy_vs_move() {
    cout << "\n=== 3. 拷贝 vs 移动（核心区别）===" << endl;
    
    cout << "\n场景1: 拷贝构造（深拷贝，开销大）" << endl;
    MyString s1("Hello");
    MyString s2(s1);  // 调用拷贝构造函数
    cout << "  s1: " << s1.c_str() << endl;
    cout << "  s2: " << s2.c_str() << endl;
    
    cout << "\n场景2: 移动构造（转移所有权，零开销）" << endl;
    MyString s3("World");
    MyString s4(std::move(s3));  // 调用移动构造函数
    cout << "  s3: " << s3.c_str() << " (已被掏空)" << endl;
    cout << "  s4: " << s4.c_str() << endl;
    
    cout << "\n场景3: 从函数返回（自动触发移动）" << endl;
    auto createString = []() {
        MyString temp("Temporary");
        return temp;  // 返回时自动移动，不拷贝！
    };
    MyString s5 = createString();
    
    cout << "\n对比：" << endl;
    cout << "  拷贝构造：分配内存 + 拷贝数据（O(n)）" << endl;
    cout << "  移动构造：转移指针（O(1)）" << endl;
}

// ============================================================================
// 四、std::move 详解
// ============================================================================

void demonstrate_std_move() {
    cout << "\n=== 4. std::move 详解 ===" << endl;
    
    cout << "\nstd::move 的作用：" << endl;
    cout << "  - 将左值转换为右值引用" << endl;
    cout << "  - 告诉编译器：这个对象可以被\"掏空\"" << endl;
    cout << "  - 本身不移动任何东西，只是类型转换" << endl;
    
    cout << "\n示例1: 手动触发移动" << endl;
    MyString s1("Original");
    MyString s2 = std::move(s1);  // s1 转为右值引用，触发移动构造
    cout << "  移动后 s1: \"" << s1.c_str() << "\" (空)" << endl;
    cout << "  移动后 s2: \"" << s2.c_str() << "\"" << endl;
    
    cout << "\n示例2: 不用 std::move（拷贝）" << endl;
    MyString s3("Copy Test");
    MyString s4 = s3;  // 没有 std::move，触发拷贝构造
    cout << "  拷贝后 s3: \"" << s3.c_str() << "\" (保留)" << endl;
    cout << "  拷贝后 s4: \"" << s4.c_str() << "\"" << endl;
    
    cout << "\n⚠️ 使用 std::move 后：" << endl;
    cout << "  - 原对象处于\"有效但未定义\"状态" << endl;
    cout << "  - 可以销毁、可以赋新值" << endl;
    cout << "  - 不要再使用其内容！" << endl;
}

// ============================================================================
// 五、移动语义的应用场景
// ============================================================================

void demonstrate_use_cases() {
    cout << "\n=== 5. 移动语义的应用场景 ===" << endl;
    
    cout << "\n场景1: 容器中存储大对象" << endl;
    vector<MyString> vec;
    MyString s1("String 1");
    MyString s2("String 2");
    
    vec.push_back(std::move(s1));  // 移动，不拷贝
    vec.push_back(std::move(s2));  // 移动，不拷贝
    cout << "  容器大小: " << vec.size() << endl;
    
    cout << "\n场景2: 函数返回大对象（自动优化）" << endl;
    auto makeString = []() {
        MyString local("Local String");
        return local;  // 返回值优化（RVO）或移动
    };
    MyString result = makeString();
    
    cout << "\n场景3: swap 操作（性能提升巨大）" << endl;
    cout << "  传统 swap（3次拷贝）：" << endl;
    cout << "    temp = a; (拷贝)" << endl;
    cout << "    a = b;    (拷贝)" << endl;
    cout << "    b = temp; (拷贝)" << endl;
    
    cout << "\n  移动语义 swap（3次移动）：" << endl;
    cout << "    temp = move(a); (移动)" << endl;
    cout << "    a = move(b);    (移动)" << endl;
    cout << "    b = move(temp); (移动)" << endl;
}

// ============================================================================
// 六、性能对比：拷贝 vs 移动
// ============================================================================

class BigData {
private:
    int* data;
    size_t size;

public:
    BigData(size_t n) : size(n) {
        data = new int[size];
        for (size_t i = 0; i < size; i++) {
            data[i] = i;
        }
    }
    
    // 拷贝构造
    BigData(const BigData& other) : size(other.size) {
        data = new int[size];
        memcpy(data, other.data, size * sizeof(int));
    }
    
    // 移动构造
    BigData(BigData&& other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
    }
    
    ~BigData() { delete[] data; }
    
    size_t getSize() const { return size; }
};

void performance_comparison() {
    cout << "\n=== 6. 性能对比：拷贝 vs 移动 ===" << endl;
    
    const size_t dataSize = 10000000;  // 1000万个整数
    
    cout << "\n测试：移动 1000 万个整数的数组" << endl;
    
    // 测试拷贝
    auto start = chrono::high_resolution_clock::now();
    BigData original(dataSize);
    BigData copied = original;  // 拷贝
    auto end = chrono::high_resolution_clock::now();
    auto copyTime = chrono::duration_cast<chrono::microseconds>(end - start).count();
    
    cout << "  拷贝耗时: " << copyTime << " 微秒" << endl;
    cout << "  操作: 分配内存 + 拷贝 " << (dataSize * sizeof(int) / 1024 / 1024) << " MB 数据" << endl;
    
    // 测试移动
    start = chrono::high_resolution_clock::now();
    BigData original2(dataSize);
    BigData moved = std::move(original2);  // 移动
    end = chrono::high_resolution_clock::now();
    auto moveTime = chrono::duration_cast<chrono::microseconds>(end - start).count();
    
    cout << "\n  移动耗时: " << moveTime << " 微秒" << endl;
    cout << "  操作: 只转移指针，零拷贝" << endl;
    
    cout << "\n性能提升: " << (copyTime / (double)moveTime) << " 倍" << endl;
    cout << "  拷贝: O(n) 时间复杂度" << endl;
    cout << "  移动: O(1) 时间复杂度" << endl;
}

// ============================================================================
// 七、与 Java 对比
// ============================================================================

void compare_with_java() {
    cout << "\n=== 7. 与 Java 对比 ===" << endl;
    
    cout << "\nJava 的对象传递：" << endl;
    cout << "  - 所有对象都是引用类型" << endl;
    cout << "  - 传递对象 = 拷贝引用（指针），不拷贝对象" << endl;
    cout << "  - 相当于 C++ 的指针传递" << endl;
    
    cout << "\n示例（Java）：" << endl;
    cout << "  ArrayList<String> list1 = new ArrayList<>();" << endl;
    cout << "  list1.add(\"Hello\");" << endl;
    cout << "  ArrayList<String> list2 = list1;  // 拷贝引用，不拷贝数据" << endl;
    cout << "  list2.add(\"World\");              // list1 也受影响" << endl;
    
    cout << "\nC++ 的对象传递：" << endl;
    cout << "  - 对象可以是值类型或引用类型" << endl;
    cout << "  - 按值传递 = 拷贝整个对象（深拷贝）" << endl;
    cout << "  - 移动语义 = 转移所有权（浅拷贝+清空）" << endl;
    
    cout << "\n示例（C++）：" << endl;
    cout << "  vector<string> v1 = {\"Hello\"};" << endl;
    cout << "  vector<string> v2 = v1;         // 深拷贝，v1 和 v2 独立" << endl;
    cout << "  vector<string> v3 = move(v1);   // 移动，v1 被掏空" << endl;
    
    cout << "\n核心区别：" << endl;
    cout << "  ┌──────────────┬──────────────────┬──────────────────┐" << endl;
    cout << "  │ 特性         │ Java             │ C++              │" << endl;
    cout << "  ├──────────────┼──────────────────┼──────────────────┤" << endl;
    cout << "  │ 对象语义     │ 引用语义         │ 值语义           │" << endl;
    cout << "  │ 赋值操作     │ 拷贝引用         │ 拷贝对象/移动    │" << endl;
    cout << "  │ 内存管理     │ 垃圾回收         │ 手动/RAII        │" << endl;
    cout << "  │ 移动语义     │ ❌ 不需要        │ ✅ C++11 引入    │" << endl;
    cout << "  │ 性能控制     │ 有限             │ 精确             │" << endl;
    cout << "  └──────────────┴──────────────────┴──────────────────┘" << endl;
    
    cout << "\nJava 为什么不需要移动语义？" << endl;
    cout << "  1. 所有对象都是引用，传递已经是\"轻量级\"的" << endl;
    cout << "  2. 垃圾回收器自动管理内存" << endl;
    cout << "  3. 没有值语义，不存在\"拷贝整个对象\"的问题" << endl;
    
    cout << "\nC++ 为什么需要移动语义？" << endl;
    cout << "  1. 支持值语义，对象可以在栈上分配" << endl;
    cout << "  2. 拷贝整个对象开销可能很大" << endl;
    cout << "  3. 需要精确控制性能" << endl;
    cout << "  4. 避免临时对象的拷贝开销" << endl;
}

// ============================================================================
// 八、完美转发（Perfect Forwarding）
// ============================================================================

void process(int& x) { cout << "    处理左值: " << x << endl; }
void process(int&& x) { cout << "    处理右值: " << x << endl; }

template<typename T>
void forwardWrapper(T&& arg) {
    cout << "  转发参数..." << endl;
    process(std::forward<T>(arg));  // 完美转发
}

void demonstrate_perfect_forwarding() {
    cout << "\n=== 8. 完美转发（Perfect Forwarding）===" << endl;
    
    cout << "\n什么是完美转发？" << endl;
    cout << "  - 在模板函数中，保持参数的左值/右值属性" << endl;
    cout << "  - 使用 std::forward 实现" << endl;
    
    cout << "\n示例：" << endl;
    int x = 10;
    forwardWrapper(x);       // 左值，转发为左值
    forwardWrapper(20);      // 右值，转发为右值
    
    cout << "\n应用场景：" << endl;
    cout << "  - 工厂函数" << endl;
    cout << "  - 包装器函数" << endl;
    cout << "  - 容器的 emplace 系列函数" << endl;
}

// ============================================================================
// 九、Rule of Five（五法则）
// ============================================================================

void rule_of_five() {
    cout << "\n=== 9. Rule of Five（五法则）===" << endl;
    
    cout << "\n规则：如果定义了以下任意一个，通常需要定义全部五个：" << endl;
    cout << "  1️⃣ 析构函数              ~ClassName()" << endl;
    cout << "  2️⃣ 拷贝构造函数          ClassName(const ClassName&)" << endl;
    cout << "  3️⃣ 拷贝赋值运算符        operator=(const ClassName&)" << endl;
    cout << "  4️⃣ 移动构造函数          ClassName(ClassName&&)" << endl;
    cout << "  5️⃣ 移动赋值运算符        operator=(ClassName&&)" << endl;
    
    cout << "\n为什么需要五个？" << endl;
    cout << "  - 管理资源（内存、文件句柄等）的类需要自定义这些函数" << endl;
    cout << "  - 防止浅拷贝导致的双重释放、悬空指针等问题" << endl;
    
    cout << "\n示例框架：" << endl;
    cout << "  class Resource {" << endl;
    cout << "  private:" << endl;
    cout << "      int* data;" << endl;
    cout << "  public:" << endl;
    cout << "      // 1. 析构函数" << endl;
    cout << "      ~Resource() { delete data; }" << endl;
    cout << "      " << endl;
    cout << "      // 2. 拷贝构造函数" << endl;
    cout << "      Resource(const Resource& other) {" << endl;
    cout << "          data = new int(*other.data);" << endl;
    cout << "      }" << endl;
    cout << "      " << endl;
    cout << "      // 3. 拷贝赋值运算符" << endl;
    cout << "      Resource& operator=(const Resource& other) {" << endl;
    cout << "          if (this != &other) {" << endl;
    cout << "              delete data;" << endl;
    cout << "              data = new int(*other.data);" << endl;
    cout << "          }" << endl;
    cout << "          return *this;" << endl;
    cout << "      }" << endl;
    cout << "      " << endl;
    cout << "      // 4. 移动构造函数" << endl;
    cout << "      Resource(Resource&& other) noexcept {" << endl;
    cout << "          data = other.data;" << endl;
    cout << "          other.data = nullptr;" << endl;
    cout << "      }" << endl;
    cout << "      " << endl;
    cout << "      // 5. 移动赋值运算符" << endl;
    cout << "      Resource& operator=(Resource&& other) noexcept {" << endl;
    cout << "          if (this != &other) {" << endl;
    cout << "              delete data;" << endl;
    cout << "              data = other.data;" << endl;
    cout << "              other.data = nullptr;" << endl;
    cout << "          }" << endl;
    cout << "          return *this;" << endl;
    cout << "      }" << endl;
    cout << "  };" << endl;
    
    cout << "\n注意：移动函数应该标记为 noexcept" << endl;
    cout << "  - 让容器知道移动是安全的" << endl;
    cout << "  - 提升性能（容器可以安全地使用移动）" << endl;
}

// ============================================================================
// 十、最佳实践
// ============================================================================

void best_practices() {
    cout << "\n=== 10. 最佳实践 ===" << endl;
    
    cout << "\n✅ 何时使用移动语义：" << endl;
    cout << "  1. 返回大对象时（自动触发）" << endl;
    cout << "  2. 容器操作（push_back, insert 等）" << endl;
    cout << "  3. swap、sort 等算法" << endl;
    cout << "  4. 确定不再需要原对象时" << endl;
    
    cout << "\n⚠️ 使用 std::move 的注意事项：" << endl;
    cout << "  1. 移动后不要再使用原对象的内容" << endl;
    cout << "  2. 移动后的对象可以赋新值、可以销毁" << endl;
    cout << "  3. 不要对 const 对象使用 std::move" << endl;
    cout << "  4. 返回局部变量时不需要 std::move（RVO）" << endl;
    
    cout << "\n✅ 实现移动函数的原则：" << endl;
    cout << "  1. 标记为 noexcept" << endl;
    cout << "  2. 转移所有资源" << endl;
    cout << "  3. 将源对象置为有效的空状态" << endl;
    cout << "  4. 检查自赋值（移动赋值运算符）" << endl;
    
    cout << "\n❌ 常见错误：" << endl;
    cout << "  1. 返回时错误使用 std::move" << endl;
    cout << "     return std::move(local);  // ❌ 阻碍 RVO" << endl;
    cout << "     return local;             // ✅ 编译器自动优化" << endl;
    
    cout << "\n  2. 对 const 对象使用 std::move" << endl;
    cout << "     const string s = \"hello\";" << endl;
    cout << "     string s2 = std::move(s);  // ❌ 退化为拷贝" << endl;
    
    cout << "\n  3. 移动后继续使用对象" << endl;
    cout << "     vector<int> v1 = {1,2,3};" << endl;
    cout << "     vector<int> v2 = std::move(v1);" << endl;
    cout << "     cout << v1.size();  // ❌ v1 已被掏空" << endl;
}

// ============================================================================
// 十一、核心总结
// ============================================================================

void summary() {
    cout << "\n========================================" << endl;
    cout << "           核心总结" << endl;
    cout << "========================================" << endl;
    
    cout << "\n问题: C++ 中的移动语义是指什么？Java 中是否有对应的概念？" << endl;
    
    cout << "\n答案：" << endl;
    cout << "  C++: 移动语义 = 转移资源所有权，避免拷贝" << endl;
    cout << "  Java: ❌ 没有对应概念（所有对象都是引用）" << endl;
    
    cout << "\n移动语义的核心：" << endl;
    cout << "  传统拷贝: 分配新内存 + 拷贝数据（O(n)）" << endl;
    cout << "  移动语义: 转移指针 + 清空源对象（O(1)）" << endl;
    
    cout << "\n关键概念：" << endl;
    cout << "  1️⃣ 左值 vs 右值" << endl;
    cout << "     左值：有名字、有地址、生命周期长" << endl;
    cout << "     右值：临时对象、字面量、表达式结果" << endl;
    
    cout << "\n  2️⃣ 右值引用（&&）" << endl;
    cout << "     Type&& ref = ...;  // 绑定到右值" << endl;
    
    cout << "\n  3️⃣ std::move" << endl;
    cout << "     将左值转换为右值引用，触发移动" << endl;
    
    cout << "\n  4️⃣ 移动构造/赋值函数" << endl;
    cout << "     ClassName(ClassName&& other) noexcept;" << endl;
    cout << "     ClassName& operator=(ClassName&& other) noexcept;" << endl;
    
    cout << "\n性能提升：" << endl;
    cout << "  拷贝 1000 万个整数: ~数十毫秒" << endl;
    cout << "  移动 1000 万个整数: ~几微秒" << endl;
    cout << "  提升: 数千倍（对大对象）" << endl;
    
    cout << "\nC++ vs Java：" << endl;
    cout << "  ┌────────────┬─────────────────┬─────────────────┐" << endl;
    cout << "  │ 特性       │ C++             │ Java            │" << endl;
    cout << "  ├────────────┼─────────────────┼─────────────────┤" << endl;
    cout << "  │ 对象语义   │ 值语义          │ 引用语义        │" << endl;
    cout << "  │ 赋值开销   │ 拷贝对象/移动   │ 拷贝引用        │" << endl;
    cout << "  │ 移动语义   │ ✅ 必需         │ ❌ 不需要       │" << endl;
    cout << "  │ 性能控制   │ 精确            │ 有限            │" << endl;
    cout << "  └────────────┴─────────────────┴─────────────────┘" << endl;
    
    cout << "\nJava 为什么不需要？" << endl;
    cout << "  - 所有对象都是引用，传递本身已经\"轻量级\"" << endl;
    cout << "  - 没有值语义，不存在拷贝整个对象的问题" << endl;
    
    cout << "\nC++ 为什么需要？" << endl;
    cout << "  - 支持值语义，对象拷贝开销可能很大" << endl;
    cout << "  - 需要精确的性能控制" << endl;
    cout << "  - 避免临时对象的不必要拷贝" << endl;
    
    cout << "\nRule of Five（五法则）：" << endl;
    cout << "  管理资源的类需要定义：" << endl;
    cout << "  1. 析构函数" << endl;
    cout << "  2. 拷贝构造函数" << endl;
    cout << "  3. 拷贝赋值运算符" << endl;
    cout << "  4. 移动构造函数     ← C++11 新增" << endl;
    cout << "  5. 移动赋值运算符   ← C++11 新增" << endl;
    
    cout << "\n记忆口诀：" << endl;
    cout << "  左值有名字，右值是临时" << endl;
    cout << "  移动偷指针，拷贝复制数据" << endl;
    cout << "  std::move 转换，触发移动语义" << endl;
    cout << "  noexcept 标记，容器更高效" << endl;
    cout << "  Java 全引用，不需要移动" << endl;
    cout << "  C++ 值语义，移动提性能" << endl;
    
    cout << "\n========================================" << endl;
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "\n╔═══════════════════════════════════════════════════╗" << endl;
    cout << "║         C++ 移动语义详解                          ║" << endl;
    cout << "║   C++ 中的移动语义是指什么？                      ║" << endl;
    cout << "║   Java 中是否有对应的概念？                       ║" << endl;
    cout << "╚═══════════════════════════════════════════════════╝" << endl;
    
    // 1. 什么是移动语义
    what_is_move_semantics();
    
    // 2. 左值 vs 右值
    demonstrate_lvalue_rvalue();
    
    // 3. 拷贝 vs 移动
    demonstrate_copy_vs_move();
    
    // 4. std::move
    demonstrate_std_move();
    
    // 5. 应用场景
    demonstrate_use_cases();
    
    // 6. 性能对比
    performance_comparison();
    
    // 7. 与 Java 对比
    compare_with_java();
    
    // 8. 完美转发
    demonstrate_perfect_forwarding();
    
    // 9. Rule of Five
    rule_of_five();
    
    // 10. 最佳实践
    best_practices();
    
    // 11. 总结
    summary();
    
    return 0;
}
