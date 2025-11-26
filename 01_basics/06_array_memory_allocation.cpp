/**
 * C++ 数组和容器的内存分配详解
 * 
 * 关键问题：变量在栈上还是堆上？
 */

#include <iostream>
#include <array>
#include <vector>
#include <string>

using namespace std;

// 全局变量（存储在全局/静态区）
int globalArray[5] = {1, 2, 3, 4, 5};

// ============================================
// 分析数组测试文件中的每个变量
// ============================================
void analyze_array_file() {
    cout << "\n" << string(60, '=') << endl;
    cout << "05_arrays.cpp 内存分配分析" << endl;
    cout << string(60, '=') << endl;
    
    // ========== 1. 传统 C 风格数组 ==========
    cout << "\n1. int numbers[5] = {10, 20, 30, 40, 50};" << endl;
    cout << "---------------------------------------" << endl;
    int numbers[5] = {10, 20, 30, 40, 50};
    cout << "✅ 整个数组在栈上" << endl;
    cout << "   - 数组名: numbers（栈上的标识符）" << endl;
    cout << "   - 数据: 20 字节（5个int）在栈上" << endl;
    cout << "   - 地址: " << &numbers << " (栈地址)" << endl;
    cout << "   - 数组元素连续存储在栈上" << endl;
    
    // ========== 2. 多维数组 ==========
    cout << "\n2. int matrix[3][3] = {{1,2,3}, {4,5,6}, {7,8,9}};" << endl;
    cout << "---------------------------------------" << endl;
    int matrix[3][3] = {{1,2,3}, {4,5,6}, {7,8,9}};
    cout << "✅ 整个二维数组在栈上" << endl;
    cout << "   - 数据: 36 字节（9个int）在栈上" << endl;
    cout << "   - 地址: " << &matrix << " (栈地址)" << endl;
    cout << "   - 所有元素连续存储" << endl;
    
    // ========== 3. std::array ==========
    cout << "\n3. array<int, 5> scores = {85, 90, 78, 92, 88};" << endl;
    cout << "---------------------------------------" << endl;
    array<int, 5> scores = {85, 90, 78, 92, 88};
    cout << "✅ 整个 array 对象在栈上" << endl;
    cout << "   - array 是栈对象" << endl;
    cout << "   - 内部数据也在栈上（20字节）" << endl;
    cout << "   - 地址: " << &scores << " (栈地址)" << endl;
    cout << "   - 本质上就是 C 数组的封装" << endl;
    
    // ========== 4. std::vector ==========
    cout << "\n4. vector<int> dynamicArray;" << endl;
    cout << "---------------------------------------" << endl;
    vector<int> dynamicArray;
    dynamicArray.push_back(10);
    dynamicArray.push_back(20);
    dynamicArray.push_back(30);
    
    cout << "⚠️  vector 对象本身在栈上，但数据在堆上！" << endl;
    cout << "   - vector 对象（24字节左右）: 栈上" << endl;
    cout << "   - vector 对象地址: " << &dynamicArray << " (栈地址)" << endl;
    cout << "   - 实际数据: 堆上动态分配" << endl;
    cout << "   - 数据指针: " << dynamicArray.data() << " (堆地址)" << endl;
    cout << "\n结构：" << endl;
    cout << "   栈上:  [vector对象: 指针、大小、容量]" << endl;
    cout << "            |" << endl;
    cout << "            ↓" << endl;
    cout << "   堆上:  [10, 20, 30, ...]" << endl;
    
    // ========== 5. vector<string> ==========
    cout << "\n5. vector<string> names = {\"Alice\", \"Bob\", \"Charlie\"};" << endl;
    cout << "---------------------------------------" << endl;
    vector<string> names = {"Alice", "Bob", "Charlie"};
    
    cout << "⚠️  更复杂的两层结构！" << endl;
    cout << "   - vector 对象: 栈上" << endl;
    cout << "   - vector 对象地址: " << &names << " (栈地址)" << endl;
    cout << "   - string 对象数组: 堆上" << endl;
    cout << "   - 数组指针: " << names.data() << " (堆地址)" << endl;
    cout << "   - 每个 string 的数据: 也在堆上" << endl;
    cout << "\n结构：" << endl;
    cout << "   栈上:  [vector对象]" << endl;
    cout << "            |" << endl;
    cout << "            ↓" << endl;
    cout << "   堆上:  [string1, string2, string3]" << endl;
    cout << "            |       |        |" << endl;
    cout << "            ↓       ↓        ↓" << endl;
    cout << "   堆上:  [\"Alice\"] [\"Bob\"] [\"Charlie\"]" << endl;
}

// ============================================
// 完整对比：栈 vs 堆
// ============================================
void stack_vs_heap_comparison() {
    cout << "\n" << string(60, '=') << endl;
    cout << "栈 vs 堆 完整对比" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n在栈上的变量：" << endl;
    cout << "---------------------------------------" << endl;
    int x = 100;
    int arr[5] = {1, 2, 3, 4, 5};
    array<int, 5> stdArr = {1, 2, 3, 4, 5};
    vector<int> vec;  // vec 对象本身
    
    cout << "1. 局部基本类型: int x" << endl;
    cout << "   地址: " << &x << endl;
    
    cout << "\n2. C 数组: int arr[5]" << endl;
    cout << "   地址: " << &arr << endl;
    
    cout << "\n3. std::array: array<int, 5>" << endl;
    cout << "   地址: " << &stdArr << endl;
    
    cout << "\n4. vector 对象: vector<int> vec" << endl;
    cout << "   对象地址: " << &vec << " (栈)" << endl;
    
    cout << "\n在堆上的数据：" << endl;
    cout << "---------------------------------------" << endl;
    vec.push_back(10);
    vec.push_back(20);
    
    int* heapArray = new int[5];  // 动态分配
    for (int i = 0; i < 5; i++) {
        heapArray[i] = i * 10;
    }
    
    cout << "1. vector 的数据: " << vec.data() << " (堆)" << endl;
    
    cout << "\n2. new 分配的数组: int* heapArray = new int[5]" << endl;
    cout << "   指针本身: " << &heapArray << " (栈)" << endl;
    cout << "   指向数据: " << heapArray << " (堆)" << endl;
    
    delete[] heapArray;  // 必须手动释放
}

// ============================================
// 内存布局示意图
// ============================================
void memory_layout_diagram() {
    cout << "\n" << string(60, '=') << endl;
    cout << "内存布局示意图" << endl;
    cout << string(60, '=') << endl;
    
    cout << R"(
┌────────────────────────────────────────────────────────┐
│  内存区域                                               │
├────────────────────────────────────────────────────────┤
│                                                        │
│  代码区 (Code/Text Segment)                            │
│  ├─ 程序机器码                                         │
│  └─ 常量字符串 "Hello"                                 │
│                                                        │
├────────────────────────────────────────────────────────┤
│                                                        │
│  全局/静态区 (Data/BSS Segment)                        │
│  ├─ 全局变量: int globalArray[5]                      │
│  └─ 静态变量: static int count                        │
│                                                        │
├────────────────────────────────────────────────────────┤
│                                                        │
│  堆区 (Heap) ↑ 向上增长                                │
│  ├─ vector<int> 的数据 [10, 20, 30, ...]             │
│  ├─ new int[10]                                       │
│  ├─ string 的字符数据                                  │
│  └─ make_shared<T> 分配的对象                         │
│                                                        │
├────────────────────────────────────────────────────────┤
│                    ↕                                   │
│                  (空闲空间)                             │
│                    ↕                                   │
├────────────────────────────────────────────────────────┤
│                                                        │
│  栈区 (Stack) ↓ 向下增长                               │
│  ├─ int x = 100                                       │
│  ├─ int arr[5] = {1,2,3,4,5}                         │
│  ├─ array<int, 5> stdArr                             │
│  ├─ vector<int> vec (对象本身，24字节)                │
│  ├─ string str (对象本身，32字节)                     │
│  └─ 函数参数、返回地址                                 │
│                                                        │
└────────────────────────────────────────────────────────┘
)" << endl;
}

// ============================================
// 详细分析每种类型
// ============================================
void detailed_analysis() {
    cout << "\n" << string(60, '=') << endl;
    cout << "详细类型分析" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n类型1: C 数组 (完全在栈上)" << endl;
    cout << "---------------------------------------" << endl;
    cout << "int numbers[5] = {10, 20, 30, 40, 50};" << endl;
    cout << "\n内存分配：" << endl;
    cout << "  栈: [10][20][30][40][50]" << endl;
    cout << "  大小: 20 字节（固定）" << endl;
    cout << "  特点: 编译时确定大小，快速，自动释放" << endl;
    
    cout << "\n类型2: std::array (完全在栈上)" << endl;
    cout << "---------------------------------------" << endl;
    cout << "array<int, 5> scores = {85, 90, 78, 92, 88};" << endl;
    cout << "\n内存分配：" << endl;
    cout << "  栈: [array对象: 内部数组[85][90][78][92][88]]" << endl;
    cout << "  大小: 20 字节（固定）" << endl;
    cout << "  特点: C 数组的安全封装，编译时大小固定" << endl;
    
    cout << "\n类型3: std::vector (对象在栈，数据在堆)" << endl;
    cout << "---------------------------------------" << endl;
    cout << "vector<int> dynamicArray;" << endl;
    cout << "dynamicArray.push_back(10);" << endl;
    cout << "\n内存分配：" << endl;
    cout << "  栈: [vector对象: 指针、大小、容量] (~24字节)" << endl;
    cout << "       |" << endl;
    cout << "       ↓" << endl;
    cout << "  堆: [10][20][30][...] (动态大小)" << endl;
    cout << "  特点: 动态增长，自动管理堆内存" << endl;
    
    cout << "\n类型4: 动态数组 new[] (数据在堆)" << endl;
    cout << "---------------------------------------" << endl;
    cout << "int* heapArr = new int[10];" << endl;
    cout << "\n内存分配：" << endl;
    cout << "  栈: [指针 heapArr] (8字节)" << endl;
    cout << "       |" << endl;
    cout << "       ↓" << endl;
    cout << "  堆: [0][0][0]...[0] (40字节)" << endl;
    cout << "  特点: 手动分配，必须 delete[] 释放" << endl;
}

// ============================================
// 大小对比
// ============================================
void size_comparison() {
    cout << "\n" << string(60, '=') << endl;
    cout << "大小对比" << endl;
    cout << string(60, '=') << endl;
    
    int arr[5] = {1, 2, 3, 4, 5};
    array<int, 5> stdArr = {1, 2, 3, 4, 5};
    vector<int> vec = {1, 2, 3, 4, 5};
    
    cout << "\n相同元素的不同类型：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "int arr[5]:" << endl;
    cout << "  sizeof(arr) = " << sizeof(arr) << " 字节 (栈上)" << endl;
    
    cout << "\narray<int, 5> stdArr:" << endl;
    cout << "  sizeof(stdArr) = " << sizeof(stdArr) << " 字节 (栈上)" << endl;
    
    cout << "\nvector<int> vec:" << endl;
    cout << "  sizeof(vec) = " << sizeof(vec) << " 字节 (栈上对象)" << endl;
    cout << "  实际数据: " << vec.size() * sizeof(int) << " 字节 (堆上)" << endl;
    cout << "  capacity: " << vec.capacity() << " (堆上可用空间)" << endl;
}

// ============================================
// 性能对比
// ============================================
void performance_comparison() {
    cout << "\n" << string(60, '=') << endl;
    cout << "性能对比" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n访问速度：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "C 数组/std::array:     最快（直接栈访问）" << endl;
    cout << "vector:                稍慢（通过指针间接访问堆）" << endl;
    cout << "\n分配/释放速度：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "C 数组/std::array:     极快（栈自动管理）" << endl;
    cout << "vector:                稍慢（堆分配/释放）" << endl;
    cout << "\n灵活性：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "C 数组/std::array:     固定大小" << endl;
    cout << "vector:                动态增长 ✅" << endl;
}

// ============================================
// 使用建议
// ============================================
void usage_recommendations() {
    cout << "\n" << string(60, '=') << endl;
    cout << "使用建议" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n何时使用 C 数组？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ 大小固定且已知" << endl;
    cout << "✅ 需要最高性能" << endl;
    cout << "✅ 小数组（几十个元素）" << endl;
    cout << "⚠️  容易越界，不安全" << endl;
    
    cout << "\n何时使用 std::array？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ 大小固定且已知" << endl;
    cout << "✅ 需要高性能" << endl;
    cout << "✅ 需要安全检查（at()）" << endl;
    cout << "✅ 推荐替代 C 数组" << endl;
    
    cout << "\n何时使用 std::vector？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ 大小不确定或会变化" << endl;
    cout << "✅ 需要动态增长" << endl;
    cout << "✅ 元素较多" << endl;
    cout << "✅ 最常用、最推荐 ✅" << endl;
    
    cout << "\n何时使用 new[]？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "⚠️  几乎不推荐！" << endl;
    cout << "除非：需要手动控制生命周期" << endl;
    cout << "建议：用 vector 或 unique_ptr<T[]> 代替" << endl;
}

int main() {
    cout << "===========================================" << endl;
    cout << "  C++ 数组/容器内存分配详解" << endl;
    cout << "===========================================" << endl;
    
    analyze_array_file();
    stack_vs_heap_comparison();
    memory_layout_diagram();
    detailed_analysis();
    size_comparison();
    performance_comparison();
    usage_recommendations();
    
    cout << "\n===========================================" << endl;
    cout << "总结" << endl;
    cout << "===========================================" << endl;
    cout << "\n05_arrays.cpp 中的内存分配：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ int numbers[5]           → 完全在栈上" << endl;
    cout << "✅ int matrix[3][3]         → 完全在栈上" << endl;
    cout << "✅ array<int, 5> scores     → 完全在栈上" << endl;
    cout << "⚠️  vector<int> dynamicArray → 对象在栈，数据在堆" << endl;
    cout << "⚠️  vector<string> names     → 对象在栈，数据和字符串都在堆" << endl;
    cout << "\n大多数变量本身在栈上，但 vector 的数据在堆上！" << endl;
    cout << "===========================================" << endl;
    
    return 0;
}

/**
 * 快速参考：
 * 
 * ┌─────────────────────┬──────────┬──────────┬────────┐
 * │  类型                │  对象    │  数据    │  大小  │
 * ├─────────────────────┼──────────┼──────────┼────────┤
 * │ int arr[5]          │  栈      │  栈      │  固定  │
 * │ array<int, 5>       │  栈      │  栈      │  固定  │
 * │ vector<int>         │  栈      │  堆      │  动态  │
 * │ int* p = new int[5] │  栈(p)   │  堆      │  固定  │
 * └─────────────────────┴──────────┴──────────┴────────┘
 * 
 * 记忆技巧：
 * - 固定大小 + 编译时已知 → 栈
 * - 动态大小 + 运行时分配 → 堆
 * - vector/string 对象在栈，数据在堆
 */
