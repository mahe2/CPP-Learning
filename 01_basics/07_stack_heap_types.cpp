/**
 * "对象在栈，数据在堆" 的数据类型详解
 * 
 * 回答问题：是不是只有指针和集合？
 * 答案：不是！还有很多其他类型也是这样的结构
 */

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <set>
#include <list>
#include <deque>
#include <unordered_map>
#include <functional>

using namespace std;

class MyClass {
public:
    int value;
    MyClass(int v) : value(v) { 
        cout << "MyClass 构造: " << value << endl;
    }
    ~MyClass() {
        cout << "MyClass 析构: " << value << endl;
    }
};

// ============================================
// 所有"对象在栈，数据在堆"的类型
// ============================================
void all_types_with_heap_data() {
    cout << "\n" << string(60, '=') << endl;
    cout << "所有「对象在栈，数据在堆」的类型" << endl;
    cout << string(60, '=') << endl;
    
    // ========== 1. 指针类型 ==========
    cout << "\n1. 指针类型" << endl;
    cout << "---------------------------------------" << endl;
    
    int* rawPtr = new int(100);
    cout << "int* rawPtr = new int(100);" << endl;
    cout << "  指针本身: " << &rawPtr << " (栈)" << endl;
    cout << "  指向数据: " << rawPtr << " (堆)" << endl;
    delete rawPtr;
    
    // ========== 2. 智能指针 ==========
    cout << "\n2. 智能指针" << endl;
    cout << "---------------------------------------" << endl;
    
    unique_ptr<int> uptr = make_unique<int>(200);
    cout << "unique_ptr<int> uptr;" << endl;
    cout << "  unique_ptr 对象: " << &uptr << " (栈)" << endl;
    cout << "  管理的数据: " << uptr.get() << " (堆)" << endl;
    
    shared_ptr<int> sptr = make_shared<int>(300);
    cout << "\nshared_ptr<int> sptr;" << endl;
    cout << "  shared_ptr 对象: " << &sptr << " (栈)" << endl;
    cout << "  管理的数据: " << sptr.get() << " (堆)" << endl;
    
    // ========== 3. 容器类型 ==========
    cout << "\n3. STL 容器（所有容器都是这样！）" << endl;
    cout << "---------------------------------------" << endl;
    
    vector<int> vec = {1, 2, 3};
    cout << "vector<int> vec;" << endl;
    cout << "  对象: " << &vec << " (栈)" << endl;
    cout << "  数据: " << vec.data() << " (堆)" << endl;
    
    list<int> lst = {1, 2, 3};
    cout << "\nlist<int> lst;" << endl;
    cout << "  对象: " << &lst << " (栈)" << endl;
    cout << "  节点: 在堆上（每个节点单独分配）" << endl;
    
    deque<int> dq = {1, 2, 3};
    cout << "\ndeque<int> dq;" << endl;
    cout << "  对象: " << &dq << " (栈)" << endl;
    cout << "  数据块: 在堆上（多个块）" << endl;
    
    map<int, string> mp = {{1, "one"}, {2, "two"}};
    cout << "\nmap<int, string> mp;" << endl;
    cout << "  对象: " << &mp << " (栈)" << endl;
    cout << "  树节点: 在堆上" << endl;
    
    set<int> st = {1, 2, 3};
    cout << "\nset<int> st;" << endl;
    cout << "  对象: " << &st << " (栈)" << endl;
    cout << "  树节点: 在堆上" << endl;
    
    unordered_map<int, string> ump = {{1, "one"}};
    cout << "\nunordered_map<int, string> ump;" << endl;
    cout << "  对象: " << &ump << " (栈)" << endl;
    cout << "  哈希表数据: 在堆上" << endl;
    
    // ========== 4. string 类型 ==========
    cout << "\n4. string（非常重要！）" << endl;
    cout << "---------------------------------------" << endl;
    
    string str = "Hello, World!";
    cout << "string str = \"Hello, World!\";" << endl;
    cout << "  string 对象: " << &str << " (栈, ~32字节)" << endl;
    cout << "  字符数据: " << (void*)str.data() << " (堆)" << endl;
    cout << "  注意: 短字符串可能使用 SSO（小字符串优化）" << endl;
    
    string longStr(100, 'A');
    cout << "\nstring longStr(100, 'A');" << endl;
    cout << "  string 对象: " << &longStr << " (栈)" << endl;
    cout << "  字符数据: " << (void*)longStr.data() << " (堆)" << endl;
    
    // ========== 5. 函数对象（有捕获的 lambda）==========
    cout << "\n5. Lambda（有捕获时）" << endl;
    cout << "---------------------------------------" << endl;
    
    vector<int> data = {1, 2, 3};
    auto lambda = [data]() {  // 按值捕获 vector
        return data.size();
    };
    cout << "auto lambda = [data]() { ... };" << endl;
    cout << "  lambda 对象: " << &lambda << " (栈)" << endl;
    cout << "  捕获的 vector: 在 lambda 对象内（栈）" << endl;
    cout << "  但 vector 的数据: 在堆上！" << endl;
    
    // ========== 6. 自定义类（包含动态成员）==========
    cout << "\n6. 自定义类（包含动态成员）" << endl;
    cout << "---------------------------------------" << endl;
    
    class MyContainer {
    public:
        int* data;
        size_t size;
        
        MyContainer(size_t n) : size(n) {
            data = new int[n];
            cout << "  分配 " << n << " 个 int 在堆上" << endl;
        }
        ~MyContainer() {
            delete[] data;
        }
    };
    
    MyContainer container(10);
    cout << "MyContainer container(10);" << endl;
    cout << "  对象: " << &container << " (栈)" << endl;
    cout << "  data 指针: " << &container.data << " (栈，指针本身)" << endl;
    cout << "  data 指向: " << container.data << " (堆)" << endl;
}

// ============================================
// 完整分类
// ============================================
void complete_classification() {
    cout << "\n" << string(60, '=') << endl;
    cout << "完整分类" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n一、完全在栈上的类型：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ 基本类型: int, double, char, bool" << endl;
    cout << "✅ C 数组: int arr[10]" << endl;
    cout << "✅ std::array: array<int, 10>" << endl;
    cout << "✅ 简单结构体: struct { int x; double y; }" << endl;
    cout << "✅ 无动态成员的类" << endl;
    
    cout << "\n二、对象在栈，数据在堆：" << endl;
    cout << "---------------------------------------" << endl;
    
    cout << "\n1️⃣  指针类型：" << endl;
    cout << "  - int* ptr" << endl;
    cout << "  - char* str" << endl;
    cout << "  - MyClass* obj" << endl;
    
    cout << "\n2️⃣  智能指针：" << endl;
    cout << "  - unique_ptr<T>" << endl;
    cout << "  - shared_ptr<T>" << endl;
    cout << "  - weak_ptr<T>" << endl;
    
    cout << "\n3️⃣  所有 STL 容器：" << endl;
    cout << "  - vector<T>" << endl;
    cout << "  - list<T>" << endl;
    cout << "  - deque<T>" << endl;
    cout << "  - map<K, V>" << endl;
    cout << "  - set<T>" << endl;
    cout << "  - unordered_map<K, V>" << endl;
    cout << "  - unordered_set<T>" << endl;
    cout << "  - queue<T>" << endl;
    cout << "  - stack<T>" << endl;
    cout << "  - priority_queue<T>" << endl;
    
    cout << "\n4️⃣  string 类型：" << endl;
    cout << "  - string (标准字符串)" << endl;
    cout << "  - wstring (宽字符串)" << endl;
    
    cout << "\n5️⃣  function 对象：" << endl;
    cout << "  - function<void()>" << endl;
    cout << "  - 有捕获的 lambda" << endl;
    
    cout << "\n6️⃣  自定义类（包含动态成员）：" << endl;
    cout << "  - 包含 vector/string 成员的类" << endl;
    cout << "  - 包含指针成员的类" << endl;
    cout << "  - 使用 new/malloc 的类" << endl;
    
    cout << "\n7️⃣  其他标准库类型：" << endl;
    cout << "  - stringstream (字符串流)" << endl;
    cout << "  - ifstream/ofstream (文件流)" << endl;
    cout << "  - thread (线程对象)" << endl;
    cout << "  - future/promise" << endl;
}

// ============================================
// 内存结构详解
// ============================================
void memory_structure_details() {
    cout << "\n" << string(60, '=') << endl;
    cout << "内存结构详解" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n典型结构（以 vector 为例）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << R"(
栈上的 vector 对象（约 24 字节）：
┌────────────────────────────┐
│  指针: 指向堆上的数据块      │  8 字节
├────────────────────────────┤
│  大小: 当前元素个数          │  8 字节
├────────────────────────────┤
│  容量: 分配的空间大小        │  8 字节
└────────────────────────────┘
        |
        ↓
堆上的数据块（动态大小）：
┌────┬────┬────┬────┬────┐
│ 10 │ 20 │ 30 │ 40 │ 50 │ ...
└────┴────┴────┴────┴────┘
)" << endl;
    
    cout << "\n典型结构（以 string 为例）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << R"(
栈上的 string 对象（约 32 字节）：
┌────────────────────────────┐
│  指针: 指向字符数据          │  8 字节
├────────────────────────────┤
│  长度: 字符串长度            │  8 字节
├────────────────────────────┤
│  容量: 分配的空间            │  8 字节
├────────────────────────────┤
│  小缓冲区 (SSO)              │  8 字节
└────────────────────────────┘
        |
        ↓ (如果字符串较长)
堆上的字符数据：
┌───┬───┬───┬───┬───┬───┐
│ H │ e │ l │ l │ o │ \0│ ...
└───┴───┴───┴───┴───┴───┘
)" << endl;
}

// ============================================
// 为什么要这样设计？
// ============================================
void why_this_design() {
    cout << "\n" << string(60, '=') << endl;
    cout << "为什么要这样设计？" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n1. 栈的限制" << endl;
    cout << "---------------------------------------" << endl;
    cout << "栈空间有限（通常 8MB）" << endl;
    cout << "如果把所有数据都放栈上：" << endl;
    cout << "  vector<int> v(1000000);  // 4MB" << endl;
    cout << "  ❌ 很快就会栈溢出！" << endl;
    
    cout << "\n2. 动态大小需求" << endl;
    cout << "---------------------------------------" << endl;
    cout << "栈上的大小必须在编译时确定" << endl;
    cout << "但很多时候大小是运行时才知道的：" << endl;
    cout << "  vector<int> v;" << endl;
    cout << "  v.push_back(10);  // 动态增长" << endl;
    cout << "  ✅ 只能在堆上实现！" << endl;
    
    cout << "\n3. 生命周期管理" << endl;
    cout << "---------------------------------------" << endl;
    cout << "栈变量离开作用域就销毁" << endl;
    cout << "但数据可能需要更长的生命周期：" << endl;
    cout << "  shared_ptr<T> ptr = make_shared<T>();" << endl;
    cout << "  // 可以被多个地方共享" << endl;
    cout << "  ✅ 堆上的数据可以独立管理！" << endl;
    
    cout << "\n4. 性能优化" << endl;
    cout << "---------------------------------------" << endl;
    cout << "对象本身很小（几十字节）→ 栈上（快）" << endl;
    cout << "数据可能很大（MB/GB）→ 堆上（灵活）" << endl;
    cout << "这样既快速又灵活！" << endl;
}

// ============================================
// 实际示例对比
// ============================================
void practical_examples() {
    cout << "\n" << string(60, '=') << endl;
    cout << "实际示例对比" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n示例1: 包含 vector 的类" << endl;
    cout << "---------------------------------------" << endl;
    
    class Student {
    public:
        string name;           // 对象栈，数据堆
        vector<int> scores;    // 对象栈，数据堆
        int age;               // 完全在栈
        
        Student(string n, int a) : name(n), age(a) {}
    };
    
    Student s("张三", 20);
    cout << "Student s(\"张三\", 20);" << endl;
    cout << "\n内存布局：" << endl;
    cout << "栈上的 Student 对象: " << &s << endl;
    cout << "  ├─ name (string 对象): " << &s.name << " (栈)" << endl;
    cout << "  │   └─ 字符数据: " << (void*)s.name.data() << " (堆)" << endl;
    cout << "  ├─ scores (vector 对象): " << &s.scores << " (栈)" << endl;
    cout << "  │   └─ 数组数据: " << s.scores.data() << " (堆)" << endl;
    cout << "  └─ age (int): " << &s.age << " (栈)" << endl;
    
    cout << "\n示例2: 嵌套容器" << endl;
    cout << "---------------------------------------" << endl;
    
    vector<vector<int>> matrix = {{1,2}, {3,4}};
    cout << "vector<vector<int>> matrix;" << endl;
    cout << "\n内存布局：" << endl;
    cout << "栈: [外层 vector 对象]" << endl;
    cout << "      |" << endl;
    cout << "      ↓" << endl;
    cout << "堆: [内层 vector1 对象][内层 vector2 对象]" << endl;
    cout << "         |                    |" << endl;
    cout << "         ↓                    ↓" << endl;
    cout << "堆: [1,2]              [3,4]" << endl;
    cout << "\n三层结构！" << endl;
}

// ============================================
// 大小对比
// ============================================
void size_comparison() {
    cout << "\n" << string(60, '=') << endl;
    cout << "对象大小对比" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n完全在栈上的类型：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "sizeof(int) = " << sizeof(int) << " 字节" << endl;
    cout << "sizeof(int[100]) = " << sizeof(int[100]) << " 字节" << endl;
    cout << "sizeof(array<int, 100>) = " << sizeof(array<int, 100>) << " 字节" << endl;
    
    cout << "\n对象在栈，数据在堆：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "sizeof(int*) = " << sizeof(int*) << " 字节 (只是指针)" << endl;
    cout << "sizeof(unique_ptr<int>) = " << sizeof(unique_ptr<int>) << " 字节" << endl;
    cout << "sizeof(shared_ptr<int>) = " << sizeof(shared_ptr<int>) << " 字节" << endl;
    cout << "sizeof(vector<int>) = " << sizeof(vector<int>) << " 字节 (固定)" << endl;
    cout << "sizeof(string) = " << sizeof(string) << " 字节 (固定)" << endl;
    cout << "sizeof(map<int,int>) = " << sizeof(map<int,int>) << " 字节 (固定)" << endl;
    
    cout << "\n注意：无论容器有多少元素，sizeof 都是固定的！" << endl;
    vector<int> v1;
    vector<int> v2(1000000);
    cout << "vector<int> v1; sizeof = " << sizeof(v1) << endl;
    cout << "vector<int> v2(1000000); sizeof = " << sizeof(v2) << endl;
    cout << "→ 因为只是对象本身的大小，数据在堆上！" << endl;
}

int main() {
    cout << "===========================================" << endl;
    cout << "  「对象在栈，数据在堆」类型详解" << endl;
    cout << "===========================================" << endl;
    
    all_types_with_heap_data();
    complete_classification();
    memory_structure_details();
    why_this_design();
    practical_examples();
    size_comparison();
    
    cout << "\n===========================================" << endl;
    cout << "总结" << endl;
    cout << "===========================================" << endl;
    cout << "\n回答你的问题：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "❌ 不只是指针和集合！" << endl;
    cout << "\n「对象在栈，数据在堆」的类型包括：" << endl;
    cout << "1️⃣  指针类型（原始指针、智能指针）" << endl;
    cout << "2️⃣  所有 STL 容器（vector、map、list...）" << endl;
    cout << "3️⃣  string 类型" << endl;
    cout << "4️⃣  function 对象" << endl;
    cout << "5️⃣  文件流、线程等" << endl;
    cout << "6️⃣  任何包含上述成员的自定义类" << endl;
    cout << "\n本质规律：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "只要类型内部需要动态分配内存，" << endl;
    cout << "就是「对象在栈，数据在堆」的结构！" << endl;
    cout << "\n这是 C++ 管理动态内存的核心模式！" << endl;
    cout << "===========================================" << endl;
    
    return 0;
}

/**
 * 快速参考：
 * 
 * ┌─────────────────────────────────────────────┐
 * │  「对象在栈，数据在堆」的类型                │
 * ├─────────────────────────────────────────────┤
 * │  ✅ 指针: int*, char*                       │
 * │  ✅ 智能指针: unique_ptr, shared_ptr        │
 * │  ✅ 容器: vector, map, list, set...         │
 * │  ✅ string                                  │
 * │  ✅ function                                │
 * │  ✅ 流: ifstream, stringstream              │
 * │  ✅ 线程: thread                            │
 * │  ✅ 包含以上成员的自定义类                   │
 * └─────────────────────────────────────────────┘
 * 
 * 判断规则：
 * - 内部使用 new/malloc → 数据在堆
 * - 大小可以动态变化 → 数据在堆
 * - sizeof 固定但能存储任意量数据 → 数据在堆
 */
