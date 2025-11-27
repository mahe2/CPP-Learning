/*
 * 右值引用 vs const 左值引用对比
 * 问题: MyString&& other 和 const MyString& other 区别在哪？
 * 核心: 右值引用允许修改参数（移动），const左值引用只能读取（拷贝）
 */

#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

// ============================================================================
// 一、核心区别概述
// ============================================================================

void core_difference() {
    cout << "\n=== 1. MyString&& 和 const MyString& 的核心区别 ===" << endl;
    
    cout << "\nMyString&& other（右值引用）：" << endl;
    cout << "  类型：右值引用" << endl;
    cout << "  绑定：只能绑定到右值（临时对象、std::move的结果）" << endl;
    cout << "  可修改性：✅ 可以修改 other（可以\"偷\"走资源）" << endl;
    cout << "  用途：移动构造函数、移动赋值运算符" << endl;
    cout << "  语义：转移所有权，\"掏空\"原对象" << endl;
    
    cout << "\nconst MyString& other（const 左值引用）：" << endl;
    cout << "  类型：const 左值引用" << endl;
    cout << "  绑定：可以绑定到左值和右值（万能引用特性）" << endl;
    cout << "  可修改性：❌ 不能修改 other（只能读取）" << endl;
    cout << "  用途：拷贝构造函数、拷贝赋值运算符" << endl;
    cout << "  语义：保留原对象，深拷贝数据" << endl;
    
    cout << "\n关键区别：" << endl;
    cout << "  ┌────────────────┬─────────────────┬─────────────────┐" << endl;
    cout << "  │ 特性           │ MyString&&      │ const MyString& │" << endl;
    cout << "  ├────────────────┼─────────────────┼─────────────────┤" << endl;
    cout << "  │ 绑定对象       │ 只能右值        │ 左值+右值       │" << endl;
    cout << "  │ 可修改性       │ ✅ 可修改       │ ❌ 不可修改     │" << endl;
    cout << "  │ 资源操作       │ 转移（偷）      │ 拷贝（复制）    │" << endl;
    cout << "  │ 原对象状态     │ 被掏空          │ 保持不变        │" << endl;
    cout << "  │ 性能           │ O(1)            │ O(n)            │" << endl;
    cout << "  └────────────────┴─────────────────┴─────────────────┘" << endl;
}

// ============================================================================
// 二、实际示例：完整的类定义
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
        cout << "  [构造] \"" << data << "\"" << endl;
    }
    
    // 拷贝构造函数：参数是 const MyString&
    MyString(const MyString& other) {
        cout << "  [拷贝构造] 参数类型: const MyString&" << endl;
        
        // ❌ other.data = nullptr;  // 编译错误！other 是 const，不能修改
        
        // ✅ 只能读取 other，必须深拷贝
        length = other.length;
        data = new char[length + 1];  // 分配新内存
        strcpy(data, other.data);     // 拷贝数据
        
        cout << "     操作：分配新内存 + 拷贝数据" << endl;
        cout << "     结果：原对象保持不变，拷贝了 \"" << data << "\"" << endl;
    }
    
    // 移动构造函数：参数是 MyString&&
    MyString(MyString&& other) noexcept {
        cout << "  [移动构造] 参数类型: MyString&&" << endl;
        
        // ✅ other 是右值引用，可以修改（"偷"走资源）
        data = other.data;      // 直接拿走指针
        length = other.length;
        
        other.data = nullptr;   // 修改 other，清空它
        other.length = 0;
        
        cout << "     操作：转移指针 + 清空原对象" << endl;
        cout << "     结果：原对象被掏空，移动了 \"" << data << "\"" << endl;
    }
    
    // 拷贝赋值运算符：参数是 const MyString&
    MyString& operator=(const MyString& other) {
        cout << "  [拷贝赋值] 参数类型: const MyString&" << endl;
        
        if (this != &other) {
            delete[] data;
            
            // ❌ other.data = nullptr;  // 编译错误！
            
            // ✅ 深拷贝
            length = other.length;
            data = new char[length + 1];
            strcpy(data, other.data);
            
            cout << "     操作：深拷贝数据" << endl;
        }
        return *this;
    }
    
    // 移动赋值运算符：参数是 MyString&&
    MyString& operator=(MyString&& other) noexcept {
        cout << "  [移动赋值] 参数类型: MyString&&" << endl;
        
        if (this != &other) {
            delete[] data;
            
            // ✅ 可以修改 other
            data = other.data;
            length = other.length;
            
            other.data = nullptr;
            other.length = 0;
            
            cout << "     操作：转移资源 + 清空原对象" << endl;
        }
        return *this;
    }
    
    ~MyString() {
        if (data) {
            cout << "  [析构] \"" << data << "\"" << endl;
            delete[] data;
        } else {
            cout << "  [析构] 空对象" << endl;
        }
    }
    
    const char* c_str() const { return data ? data : ""; }
    size_t size() const { return length; }
};

void demonstrate_constructors() {
    cout << "\n=== 2. 拷贝构造 vs 移动构造实际演示 ===" << endl;
    
    cout << "\n场景1: 拷贝构造（使用 const MyString&）" << endl;
    MyString s1("Hello");
    MyString s2 = s1;  // 调用拷贝构造：MyString(const MyString&)
    cout << "  s1 内容: \"" << s1.c_str() << "\" (保留)" << endl;
    cout << "  s2 内容: \"" << s2.c_str() << "\" (新拷贝)" << endl;
    
    cout << "\n场景2: 移动构造（使用 MyString&&）" << endl;
    MyString s3("World");
    MyString s4 = std::move(s3);  // 调用移动构造：MyString(MyString&&)
    cout << "  s3 内容: \"" << s3.c_str() << "\" (被掏空)" << endl;
    cout << "  s3 长度: " << s3.size() << endl;
    cout << "  s4 内容: \"" << s4.c_str() << "\" (接管资源)" << endl;
}

// ============================================================================
// 三、参数绑定规则
// ============================================================================

void demonstrate_binding() {
    cout << "\n=== 3. 参数绑定规则 ===" << endl;
    
    cout << "\n右值引用 MyString&& 的绑定规则：" << endl;
    MyString s1("Test1");
    
    // MyString s2(s1);  // ❌ 编译错误！s1 是左值，不能绑定到 MyString&&
    cout << "  ❌ MyString s2(s1);  // 编译错误" << endl;
    cout << "     原因：s1 是左值，不能绑定到右值引用" << endl;
    
    MyString s3(std::move(s1));  // ✅ OK，std::move 将 s1 转为右值
    cout << "  ✅ MyString s3(std::move(s1));  // 成功" << endl;
    cout << "     原因：std::move 将左值转为右值引用" << endl;
    
    MyString s4(MyString("Temp"));  // ✅ OK，MyString("Temp") 是临时对象（右值）
    cout << "  ✅ MyString s4(MyString(\"Temp\"));  // 成功" << endl;
    cout << "     原因：临时对象是右值" << endl;
    
    cout << "\nconst 左值引用 const MyString& 的绑定规则：" << endl;
    MyString s5("Test2");
    
    MyString s6(s5);  // ✅ OK，s5 是左值
    cout << "  ✅ MyString s6(s5);  // 成功" << endl;
    cout << "     原因：const 引用可以绑定左值" << endl;
    
    // const 引用可以绑定右值（延长临时对象生命周期）
    const MyString& ref = MyString("Temp2");
    cout << "  ✅ const MyString& ref = MyString(\"Temp2\");  // 成功" << endl;
    cout << "     原因：const 引用可以绑定右值" << endl;
    cout << "     特性：延长临时对象的生命周期" << endl;
}

// ============================================================================
// 四、可修改性对比
// ============================================================================

class Demo {
private:
    int* ptr;

public:
    Demo(int val) : ptr(new int(val)) {}
    
    // 拷贝构造：不能修改 other
    Demo(const Demo& other) {
        cout << "\n  拷贝构造：const Demo& other" << endl;
        
        // ❌ 以下操作都会编译错误：
        // other.ptr = nullptr;        // 错误：other 是 const
        // *other.ptr = 999;           // 错误：不能修改 const 对象的内容
        // delete other.ptr;           // 错误：不能删除 const 对象的资源
        
        cout << "    ❌ 不能修改 other" << endl;
        cout << "    ❌ other.ptr = nullptr;  // 编译错误" << endl;
        cout << "    ✅ 只能读取：*other.ptr = " << *other.ptr << endl;
        
        // ✅ 只能深拷贝
        ptr = new int(*other.ptr);
        cout << "    ✅ 必须深拷贝：分配新内存" << endl;
    }
    
    // 移动构造：可以修改 other
    Demo(Demo&& other) noexcept {
        cout << "\n  移动构造：Demo&& other" << endl;
        
        // ✅ 以下操作都可以：
        ptr = other.ptr;           // ✅ 拿走指针
        other.ptr = nullptr;       // ✅ 清空 other
        
        cout << "    ✅ 可以修改 other" << endl;
        cout << "    ✅ ptr = other.ptr;       // 拿走指针" << endl;
        cout << "    ✅ other.ptr = nullptr;   // 清空 other" << endl;
        cout << "    ✅ 转移资源，零拷贝" << endl;
    }
    
    ~Demo() { delete ptr; }
    
    int getValue() const { return ptr ? *ptr : 0; }
};

void demonstrate_modifiability() {
    cout << "\n=== 4. 可修改性对比 ===" << endl;
    
    cout << "\n测试1: 拷贝构造（参数不可修改）" << endl;
    Demo d1(42);
    Demo d2(d1);  // const Demo& 参数
    cout << "  d1 的值: " << d1.getValue() << " (保留)" << endl;
    cout << "  d2 的值: " << d2.getValue() << " (新拷贝)" << endl;
    
    cout << "\n测试2: 移动构造（参数可修改）" << endl;
    Demo d3(100);
    Demo d4(std::move(d3));  // Demo&& 参数
    cout << "  d3 的值: " << d3.getValue() << " (被清空)" << endl;
    cout << "  d4 的值: " << d4.getValue() << " (接管)" << endl;
}

// ============================================================================
// 五、重载决议（Overload Resolution）
// ============================================================================

class TestOverload {
public:
    TestOverload(const char* s) { cout << "  [构造] " << s << endl; }
    
    // 拷贝构造
    TestOverload(const TestOverload& other) {
        cout << "  [调用] 拷贝构造: const TestOverload&" << endl;
    }
    
    // 移动构造
    TestOverload(TestOverload&& other) noexcept {
        cout << "  [调用] 移动构造: TestOverload&&" << endl;
    }
};

void demonstrate_overload_resolution() {
    cout << "\n=== 5. 重载决议：编译器如何选择？===" << endl;
    
    cout << "\n规则：编译器根据参数类型选择最匹配的函数" << endl;
    
    cout << "\n情况1: 左值参数" << endl;
    TestOverload t1("Original");
    TestOverload t2(t1);  // t1 是左值 → 调用拷贝构造
    cout << "  左值 → 拷贝构造（const TestOverload&）" << endl;
    
    cout << "\n情况2: 右值参数（std::move）" << endl;
    TestOverload t3("Move Me");
    TestOverload t4(std::move(t3));  // std::move(t3) 是右值 → 调用移动构造
    cout << "  std::move(左值) → 移动构造（TestOverload&&）" << endl;
    
    cout << "\n情况3: 临时对象（右值）" << endl;
    TestOverload t5(TestOverload("Temporary"));  // 临时对象是右值 → 调用移动构造
    cout << "  临时对象 → 移动构造（TestOverload&&）" << endl;
    
    cout << "\n如果没有移动构造函数会怎样？" << endl;
    cout << "  - 编译器会退化使用拷贝构造函数" << endl;
    cout << "  - const 左值引用可以绑定右值" << endl;
    cout << "  - 但会失去移动优化，性能下降" << endl;
}

// ============================================================================
// 六、const 右值引用（很少用）
// ============================================================================

void demonstrate_const_rvalue_ref() {
    cout << "\n=== 6. const 右值引用（const MyString&&）===" << endl;
    
    cout << "\n语法：const MyString&& other" << endl;
    cout << "  特点：绑定到右值，但不能修改" << endl;
    cout << "  用途：几乎没有！非常罕见" << endl;
    
    cout << "\n为什么很少用？" << endl;
    cout << "  - 右值引用的目的就是为了修改（移动资源）" << endl;
    cout << "  - const 禁止修改，失去了右值引用的意义" << endl;
    cout << "  - 等同于 const 左值引用，但更受限" << endl;
    
    cout << "\n四种引用类型对比：" << endl;
    cout << "  ┌───────────────────┬──────────┬──────────┬────────────┐" << endl;
    cout << "  │ 类型              │ 绑定左值 │ 绑定右值 │ 可修改性   │" << endl;
    cout << "  ├───────────────────┼──────────┼──────────┼────────────┤" << endl;
    cout << "  │ MyString&         │ ✅       │ ❌       │ ✅         │" << endl;
    cout << "  │ const MyString&   │ ✅       │ ✅       │ ❌         │" << endl;
    cout << "  │ MyString&&        │ ❌       │ ✅       │ ✅ (核心)  │" << endl;
    cout << "  │ const MyString&&  │ ❌       │ ✅       │ ❌ (无用)  │" << endl;
    cout << "  └───────────────────┴──────────┴──────────┴────────────┘" << endl;
}

// ============================================================================
// 七、性能对比
// ============================================================================

class BigObject {
private:
    int* data;
    static const size_t SIZE = 1000000;

public:
    BigObject() {
        data = new int[SIZE];
        for (size_t i = 0; i < SIZE; i++) {
            data[i] = i;
        }
    }
    
    // 拷贝构造：const BigObject&
    BigObject(const BigObject& other) {
        data = new int[SIZE];
        memcpy(data, other.data, SIZE * sizeof(int));
    }
    
    // 移动构造：BigObject&&
    BigObject(BigObject&& other) noexcept {
        data = other.data;
        other.data = nullptr;
    }
    
    ~BigObject() { delete[] data; }
};

void performance_comparison() {
    cout << "\n=== 7. 性能对比 ===" << endl;
    
    cout << "\n拷贝 vs 移动的性能差异：" << endl;
    cout << "  拷贝（const MyString&）：" << endl;
    cout << "    - 分配新内存" << endl;
    cout << "    - 拷贝所有数据" << endl;
    cout << "    - 时间复杂度：O(n)" << endl;
    cout << "    - 原对象保持不变" << endl;
    
    cout << "\n  移动（MyString&&）：" << endl;
    cout << "    - 不分配内存" << endl;
    cout << "    - 只转移指针" << endl;
    cout << "    - 时间复杂度：O(1)" << endl;
    cout << "    - 原对象被掏空" << endl;
    
    cout << "\n实际场景：vector 插入大对象" << endl;
    cout << "  vector<BigObject> vec;" << endl;
    cout << "  BigObject obj;" << endl;
    cout << "  " << endl;
    cout << "  vec.push_back(obj);         // 拷贝，慢" << endl;
    cout << "  vec.push_back(move(obj));   // 移动，快" << endl;
}

// ============================================================================
// 八、实际应用场景
// ============================================================================

void practical_examples() {
    cout << "\n=== 8. 实际应用场景 ===" << endl;
    
    cout << "\n场景1: 函数返回值" << endl;
    cout << "  MyString createString() {" << endl;
    cout << "      MyString local(\"data\");" << endl;
    cout << "      return local;  // 自动使用移动构造（MyString&&）" << endl;
    cout << "  }" << endl;
    cout << "  " << endl;
    cout << "  MyString s = createString();  // 调用移动构造，不是拷贝" << endl;
    
    cout << "\n场景2: 容器操作" << endl;
    cout << "  vector<MyString> vec;" << endl;
    cout << "  MyString s(\"data\");" << endl;
    cout << "  " << endl;
    cout << "  vec.push_back(s);         // 使用拷贝构造（const MyString&）" << endl;
    cout << "  vec.push_back(move(s));   // 使用移动构造（MyString&&）" << endl;
    
    cout << "\n场景3: 赋值操作" << endl;
    cout << "  MyString s1(\"Hello\");" << endl;
    cout << "  MyString s2(\"World\");" << endl;
    cout << "  " << endl;
    cout << "  s1 = s2;         // 拷贝赋值（const MyString&）" << endl;
    cout << "  s1 = move(s2);   // 移动赋值（MyString&&）" << endl;
}

// ============================================================================
// 九、常见误区
// ============================================================================

void common_mistakes() {
    cout << "\n=== 9. 常见误区 ===" << endl;
    
    cout << "\n误区1: 认为 const MyString& 只能绑定左值" << endl;
    cout << "  ❌ 错误理解" << endl;
    cout << "  ✅ 正确：const 左值引用可以绑定左值和右值" << endl;
    cout << "  " << endl;
    cout << "  const MyString& ref1 = s1;            // 绑定左值" << endl;
    cout << "  const MyString& ref2 = MyString(\"T\"); // 绑定右值" << endl;
    
    cout << "\n误区2: 认为右值引用会自动\"移动\"" << endl;
    cout << "  ❌ 错误理解" << endl;
    cout << "  ✅ 正确：MyString&& 只是类型，需要在函数体内手动转移资源" << endl;
    cout << "  " << endl;
    cout << "  MyString(MyString&& other) {" << endl;
    cout << "      data = other.data;      // 手动转移" << endl;
    cout << "      other.data = nullptr;   // 手动清空" << endl;
    cout << "  }" << endl;
    
    cout << "\n误区3: 移动后仍然使用原对象" << endl;
    cout << "  MyString s1(\"data\");" << endl;
    cout << "  MyString s2 = move(s1);" << endl;
    cout << "  cout << s1.c_str();  // ❌ 危险！s1 已被掏空" << endl;
    
    cout << "\n误区4: 对 const 对象使用 std::move" << endl;
    cout << "  const MyString s1(\"data\");" << endl;
    cout << "  MyString s2 = move(s1);  // ❌ 退化为拷贝，不是移动！" << endl;
    cout << "  原因：const 对象不能修改，无法\"偷\"走资源" << endl;
}

// ============================================================================
// 十、核心总结
// ============================================================================

void summary() {
    cout << "\n========================================" << endl;
    cout << "           核心总结" << endl;
    cout << "========================================" << endl;
    
    cout << "\n问题: MyString&& other 和 const MyString& other 区别在哪？" << endl;
    
    cout << "\n核心区别：" << endl;
    cout << "  MyString&& other（右值引用）：" << endl;
    cout << "    ✅ 可以修改 other" << endl;
    cout << "    ✅ 可以\"偷\"走 other 的资源" << endl;
    cout << "    ✅ 实现移动语义（转移所有权）" << endl;
    cout << "    ❌ 只能绑定右值" << endl;
    cout << "    🎯 用于：移动构造函数、移动赋值运算符" << endl;
    
    cout << "\n  const MyString& other（const 左值引用）：" << endl;
    cout << "    ❌ 不能修改 other" << endl;
    cout << "    ❌ 不能\"偷\"走资源，必须拷贝" << endl;
    cout << "    ✅ 实现拷贝语义（深拷贝）" << endl;
    cout << "    ✅ 可以绑定左值和右值" << endl;
    cout << "    🎯 用于：拷贝构造函数、拷贝赋值运算符" << endl;
    
    cout << "\n完整对比表：" << endl;
    cout << "  ┌──────────────┬─────────────────┬─────────────────┐" << endl;
    cout << "  │ 特性         │ MyString&&      │ const MyString& │" << endl;
    cout << "  ├──────────────┼─────────────────┼─────────────────┤" << endl;
    cout << "  │ 参数类型     │ 右值引用        │ const左值引用   │" << endl;
    cout << "  │ 绑定左值     │ ❌              │ ✅              │" << endl;
    cout << "  │ 绑定右值     │ ✅              │ ✅              │" << endl;
    cout << "  │ 可修改性     │ ✅ 可修改       │ ❌ 只读         │" << endl;
    cout << "  │ 资源操作     │ 转移（偷）      │ 拷贝（复制）    │" << endl;
    cout << "  │ 原对象状态   │ 被掏空          │ 保持不变        │" << endl;
    cout << "  │ 内存分配     │ 不需要          │ 需要            │" << endl;
    cout << "  │ 数据拷贝     │ 不需要          │ 需要            │" << endl;
    cout << "  │ 时间复杂度   │ O(1)            │ O(n)            │" << endl;
    cout << "  │ 用途         │ 移动构造/赋值   │ 拷贝构造/赋值   │" << endl;
    cout << "  └──────────────┴─────────────────┴─────────────────┘" << endl;
    
    cout << "\n典型实现对比：" << endl;
    cout << "\n拷贝构造（const MyString& other）：" << endl;
    cout << "  MyString(const MyString& other) {" << endl;
    cout << "      // ❌ other.data = nullptr;  // 编译错误！" << endl;
    cout << "      length = other.length;       // ✅ 读取" << endl;
    cout << "      data = new char[length+1];   // ✅ 分配新内存" << endl;
    cout << "      strcpy(data, other.data);    // ✅ 拷贝数据" << endl;
    cout << "  }" << endl;
    
    cout << "\n移动构造（MyString&& other）：" << endl;
    cout << "  MyString(MyString&& other) noexcept {" << endl;
    cout << "      data = other.data;           // ✅ 偷走指针" << endl;
    cout << "      length = other.length;" << endl;
    cout << "      other.data = nullptr;        // ✅ 清空 other" << endl;
    cout << "      other.length = 0;" << endl;
    cout << "  }" << endl;
    
    cout << "\n选择指南：" << endl;
    cout << "  需要拷贝，保留原对象 → const MyString&" << endl;
    cout << "  需要移动，可以掏空原对象 → MyString&&" << endl;
    cout << "  性能优先，临时对象 → MyString&&" << endl;
    cout << "  安全优先，共享数据 → const MyString&" << endl;
    
    cout << "\n记忆口诀：" << endl;
    cout << "  const 引用不可改，只能拷贝不能偷" << endl;
    cout << "  右值引用可以改，转移资源性能优" << endl;
    cout << "  拷贝语义用 const&，移动语义用 &&" << endl;
    cout << "  左值右值分得清，性能优化不用愁" << endl;
    
    cout << "\n========================================" << endl;
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "\n╔═══════════════════════════════════════════════════╗" << endl;
    cout << "║   右值引用 vs const 左值引用对比                  ║" << endl;
    cout << "║   MyString&& 和 const MyString& 区别在哪？        ║" << endl;
    cout << "╚═══════════════════════════════════════════════════╝" << endl;
    
    // 1. 核心区别
    core_difference();
    
    // 2. 实际演示
    demonstrate_constructors();
    
    // 3. 绑定规则
    demonstrate_binding();
    
    // 4. 可修改性
    demonstrate_modifiability();
    
    // 5. 重载决议
    demonstrate_overload_resolution();
    
    // 6. const 右值引用
    demonstrate_const_rvalue_ref();
    
    // 7. 性能对比
    performance_comparison();
    
    // 8. 实际应用
    practical_examples();
    
    // 9. 常见误区
    common_mistakes();
    
    // 10. 总结
    summary();
    
    return 0;
}
