/**
 * 指针变量的内存分配详解
 * 
 * 核心问题：int* heapVar = new int(20);
 * heapVar 变量存储在栈上吗？
 * 
 * 答案：是的！heapVar 本身在栈上，它指向的数据在堆上
 */

#include <iostream>
#include <iomanip>

using namespace std;

void pointer_memory_analysis() {
    cout << "\n" << string(60, '=') << endl;
    cout << "指针变量的内存分配详解" << endl;
    cout << string(60, '=') << endl;
    
    // 关键示例：int* heapVar = new int(20);
    int* heapVar = new int(20);
    
    cout << "\n代码：int* heapVar = new int(20);\n" << endl;
    
    cout << "内存布局：" << endl;
    cout << "┌─────────────────────────────────────────────┐" << endl;
    cout << "│  栈（Stack）                                 │" << endl;
    cout << "│  ┌─────────────────────────────────┐        │" << endl;
    cout << "│  │ heapVar（指针变量）              │        │" << endl;
    cout << "│  │ 类型: int*                      │        │" << endl;
    cout << "│  │ 大小: 8 字节（64位系统）         │        │" << endl;
    cout << "│  │ 存储内容: 0x" << heapVar << " │ ← 堆地址 │" << endl;
    cout << "│  └─────────────────────────────────┘        │" << endl;
    cout << "│                    │                         │" << endl;
    cout << "│                    │ 指向                    │" << endl;
    cout << "│                    ↓                         │" << endl;
    cout << "└─────────────────────────────────────────────┘" << endl;
    cout << "                     │" << endl;
    cout << "                     ↓" << endl;
    cout << "┌─────────────────────────────────────────────┐" << endl;
    cout << "│  堆（Heap）                                  │" << endl;
    cout << "│  ┌─────────────────────────────────┐        │" << endl;
    cout << "│  │ int 对象                        │        │" << endl;
    cout << "│  │ 大小: 4 字节                    │        │" << endl;
    cout << "│  │ 值: 20                          │        │" << endl;
    cout << "│  │ 地址: 0x" << heapVar << "      │        │" << endl;
    cout << "│  └─────────────────────────────────┘        │" << endl;
    cout << "└─────────────────────────────────────────────┘" << endl;
    
    cout << "\n详细分析：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. heapVar 变量本身：" << endl;
    cout << "   - 位置：栈（Stack）" << endl;
    cout << "   - 类型：int* (指针)" << endl;
    cout << "   - 大小：" << sizeof(heapVar) << " 字节" << endl;
    cout << "   - 地址：" << &heapVar << " ← 栈地址" << endl;
    cout << "   - 内容：" << heapVar << " ← 这是堆地址" << endl;
    
    cout << "\n2. heapVar 指向的数据：" << endl;
    cout << "   - 位置：堆（Heap）" << endl;
    cout << "   - 类型：int" << endl;
    cout << "   - 大小：" << sizeof(*heapVar) << " 字节" << endl;
    cout << "   - 地址：" << heapVar << " ← 堆地址" << endl;
    cout << "   - 值：" << *heapVar << endl;
    
    cout << "\n关键理解：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ heapVar 是一个局部变量，存储在栈上" << endl;
    cout << "✅ heapVar 的值是一个堆地址" << endl;
    cout << "✅ new int(20) 创建的数据在堆上" << endl;
    cout << "✅ 离开作用域时，heapVar 自动销毁（栈）" << endl;
    cout << "⚠️  但堆上的数据不会自动销毁！" << endl;
    cout << "⚠️  必须手动 delete heapVar;" << endl;
    
    delete heapVar;  // 释放堆内存
}

void detailed_comparison() {
    cout << "\n" << string(60, '=') << endl;
    cout << "不同变量的内存分配对比" << endl;
    cout << string(60, '=') << endl;
    
    // 1. 完全在栈上
    int stackVar = 10;
    
    // 2. 指针在栈，数据在堆
    int* heapPtr = new int(20);
    
    // 3. 对象在栈，内部数据可能在堆
    string str = "Hello";
    
    cout << "\n1. 完全在栈上的变量：" << endl;
    cout << "   int stackVar = 10;" << endl;
    cout << "   - stackVar 在栈：" << &stackVar << endl;
    cout << "   - 值：" << stackVar << endl;
    cout << "   - 大小：" << sizeof(stackVar) << " 字节" << endl;
    
    cout << "\n2. 指针在栈，数据在堆：" << endl;
    cout << "   int* heapPtr = new int(20);" << endl;
    cout << "   - heapPtr 变量在栈：" << &heapPtr << endl;
    cout << "   - heapPtr 的值（堆地址）：" << heapPtr << endl;
    cout << "   - 堆上的数据：" << *heapPtr << endl;
    cout << "   - heapPtr 大小：" << sizeof(heapPtr) << " 字节（指针大小）" << endl;
    cout << "   - 数据大小：" << sizeof(*heapPtr) << " 字节" << endl;
    
    cout << "\n3. 对象在栈，内部数据可能在堆：" << endl;
    cout << "   string str = \"Hello\";" << endl;
    cout << "   - str 对象在栈：" << &str << endl;
    cout << "   - str 的内容（可能在堆）：" << str << endl;
    cout << "   - str 对象大小：" << sizeof(str) << " 字节" << endl;
    
    cout << "\n地址对比（观察栈 vs 堆）：" << endl;
    cout << "---------------------------------------" << endl;
    
    // 计算地址差异
    long long stackAddr = (long long)&stackVar;
    long long ptrAddr = (long long)&heapPtr;
    long long heapAddr = (long long)heapPtr;
    
    cout << "栈地址：0x" << hex << stackAddr << dec << endl;
    cout << "栈地址：0x" << hex << ptrAddr << dec << endl;
    cout << "堆地址：0x" << hex << heapAddr << dec << endl;
    
    cout << "\n观察：" << endl;
    cout << "- 栈地址通常较大（高地址）" << endl;
    cout << "- 堆地址通常较小（低地址）" << endl;
    cout << "- 栈变量地址相近" << endl;
    
    delete heapPtr;
}

void lifecycle_demo() {
    cout << "\n" << string(60, '=') << endl;
    cout << "生命周期演示" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n场景：作用域结束时会发生什么？\n" << endl;
    
    {
        cout << "进入作用域 {" << endl;
        
        int* ptr = new int(100);
        cout << "  int* ptr = new int(100);" << endl;
        cout << "  - ptr 变量在栈：" << &ptr << endl;
        cout << "  - 堆数据地址：" << ptr << endl;
        cout << "  - 堆数据值：" << *ptr << endl;
        
        cout << "\n离开作用域 }" << endl;
    }  // ptr 变量被销毁（栈自动清理）
       // 但堆上的数据还在！（内存泄漏！）
    
    cout << "\n结果：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ ptr 变量已销毁（栈自动清理）" << endl;
    cout << "❌ 堆上的数据仍然存在，但地址丢失了！" << endl;
    cout << "❌ 这就是内存泄漏！" << endl;
    
    cout << "\n正确做法：" << endl;
    cout << "---------------------------------------" << endl;
    {
        int* ptr = new int(100);
        cout << "  int* ptr = new int(100);" << endl;
        cout << "  使用 ptr..." << endl;
        delete ptr;  // 手动释放堆内存
        cout << "  delete ptr;  ← 释放堆内存" << endl;
    }  // ptr 变量销毁（栈）
    
    cout << "\n✅ 堆内存已释放" << endl;
    cout << "✅ 栈变量已销毁" << endl;
    cout << "✅ 无内存泄漏" << endl;
}

void size_analysis() {
    cout << "\n" << string(60, '=') << endl;
    cout << "内存大小分析" << endl;
    cout << string(60, '=') << endl;
    
    // 各种指针
    int* intPtr = new int(42);
    double* doublePtr = new double(3.14);
    char* charPtr = new char('A');
    string* stringPtr = new string("Hello");
    
    // 数组指针
    int* arrPtr = new int[100];
    
    cout << "\n指针变量的大小（在栈上）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "int* intPtr:      " << sizeof(intPtr) << " 字节" << endl;
    cout << "double* doublePtr:" << sizeof(doublePtr) << " 字节" << endl;
    cout << "char* charPtr:    " << sizeof(charPtr) << " 字节" << endl;
    cout << "string* stringPtr:" << sizeof(stringPtr) << " 字节" << endl;
    cout << "int* arrPtr:      " << sizeof(arrPtr) << " 字节" << endl;
    
    cout << "\n关键发现：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ 所有指针变量大小相同：" << sizeof(void*) << " 字节" << endl;
    cout << "✅ 这是因为指针只是存储地址" << endl;
    cout << "✅ 在 64 位系统上，地址都是 8 字节" << endl;
    cout << "✅ 与指向的数据类型无关" << endl;
    
    cout << "\n指向的数据大小（在堆上）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "*intPtr:      " << sizeof(*intPtr) << " 字节" << endl;
    cout << "*doublePtr:   " << sizeof(*doublePtr) << " 字节" << endl;
    cout << "*charPtr:     " << sizeof(*charPtr) << " 字节" << endl;
    cout << "*stringPtr:   " << sizeof(*stringPtr) << " 字节" << endl;
    // cout << "*arrPtr:      " << sizeof(*arrPtr) << " 字节（只是第一个元素）" << endl;
    
    cout << "\n实际堆内存分配：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "int[100] 数组：100 × 4 = 400 字节" << endl;
    cout << "但 arrPtr 变量本身只占 8 字节（在栈上）" << endl;
    
    // 清理
    delete intPtr;
    delete doublePtr;
    delete charPtr;
    delete stringPtr;
    delete[] arrPtr;
}

void address_visualization() {
    cout << "\n" << string(60, '=') << endl;
    cout << "地址可视化" << endl;
    cout << string(60, '=') << endl;
    
    int stackVar1 = 1;
    int stackVar2 = 2;
    int* heapVar1 = new int(10);
    int* heapVar2 = new int(20);
    
    cout << "\n栈变量（指针本身）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "&stackVar1: " << &stackVar1 << endl;
    cout << "&stackVar2: " << &stackVar2 << endl;
    cout << "&heapVar1:  " << &heapVar1 << " ← 指针变量在栈" << endl;
    cout << "&heapVar2:  " << &heapVar2 << " ← 指针变量在栈" << endl;
    
    cout << "\n堆数据（指针指向的内容）：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "heapVar1:   " << heapVar1 << " ← 堆地址" << endl;
    cout << "heapVar2:   " << heapVar2 << " ← 堆地址" << endl;
    
    cout << "\n地址差异分析：" << endl;
    cout << "---------------------------------------" << endl;
    
    long long stack_diff = (long long)&stackVar2 - (long long)&stackVar1;
    long long ptr_diff = (long long)&heapVar2 - (long long)&heapVar1;
    long long heap_diff = (long long)heapVar2 - (long long)heapVar1;
    
    cout << "栈变量地址差: " << abs(stack_diff) << " 字节" << endl;
    cout << "指针变量地址差: " << abs(ptr_diff) << " 字节" << endl;
    cout << "堆数据地址差: " << abs(heap_diff) << " 字节" << endl;
    
    cout << "\n观察：" << endl;
    cout << "- 栈变量地址连续，差值为变量大小" << endl;
    cout << "- 指针变量也在栈上，地址也连续" << endl;
    cout << "- 堆数据地址不一定连续" << endl;
    
    delete heapVar1;
    delete heapVar2;
}

void summary_table() {
    cout << "\n" << string(60, '=') << endl;
    cout << "总结对比表" << endl;
    cout << string(60, '=') << endl;
    
    cout << R"(
┌──────────────────┬─────────────────┬─────────────────┐
│  代码             │  变量位置       │  数据位置       │
├──────────────────┼─────────────────┼─────────────────┤
│ int x = 10;      │ 栈              │ 栈              │
├──────────────────┼─────────────────┼─────────────────┤
│ int* p =         │ p 在栈          │ 数据在堆        │
│   new int(20);   │ (指针变量)      │ (new 分配)      │
├──────────────────┼─────────────────┼─────────────────┤
│ int arr[100];    │ 栈              │ 栈（连续）      │
├──────────────────┼─────────────────┼─────────────────┤
│ int* arr =       │ arr 在栈        │ 数组在堆        │
│   new int[100];  │ (指针变量)      │ (new 分配)      │
├──────────────────┼─────────────────┼─────────────────┤
│ string s ="hi";  │ s 对象在栈      │ 字符数据可能在堆│
│                  │ (对象本身)      │ (取决于SSO)     │
├──────────────────┼─────────────────┼─────────────────┤
│ vector<int> v;   │ v 对象在栈      │ 元素数据在堆    │
│                  │ (对象本身)      │ (动态分配)      │
└──────────────────┴─────────────────┴─────────────────┘

关键理解：
┌────────────────────────────────────────────────┐
│  int* heapVar = new int(20);                   │
│                                                 │
│  heapVar：                                      │
│    ✅ 是一个指针变量                            │
│    ✅ 存储在栈上                                │
│    ✅ 占用 8 字节（64位系统）                   │
│    ✅ 存储的内容是一个堆地址                    │
│    ✅ 作用域结束时自动销毁                      │
│                                                 │
│  new int(20)：                                  │
│    ✅ 在堆上分配内存                            │
│    ✅ 占用 4 字节                               │
│    ✅ 存储值 20                                 │
│    ⚠️  不会自动销毁                             │
│    ⚠️  必须手动 delete                          │
└────────────────────────────────────────────────┘
)" << endl;
}

int main() {
    cout << "===========================================" << endl;
    cout << "  指针变量的内存分配完全解析" << endl;
    cout << "===========================================" << endl;
    
    pointer_memory_analysis();
    detailed_comparison();
    lifecycle_demo();
    size_analysis();
    address_visualization();
    summary_table();
    
    cout << "\n===========================================" << endl;
    cout << "核心答案" << endl;
    cout << "===========================================" << endl;
    
    cout << "\n问题：int* heapVar = new int(20);" << endl;
    cout << "     heapVar 变量存储在栈上吗？" << endl;
    
    cout << "\n答案：是的！heapVar 本身在栈上！" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ heapVar 是一个局部变量（指针）" << endl;
    cout << "✅ 局部变量存储在栈上" << endl;
    cout << "✅ heapVar 占用 8 字节（64位系统）" << endl;
    cout << "✅ heapVar 的值是一个堆地址" << endl;
    cout << "✅ 作用域结束时，heapVar 自动销毁" << endl;
    
    cout << "\n但是！" << endl;
    cout << "---------------------------------------" << endl;
    cout << "⚠️  new int(20) 创建的数据在堆上" << endl;
    cout << "⚠️  堆数据占用 4 字节，存储值 20" << endl;
    cout << "⚠️  堆数据不会自动销毁" << endl;
    cout << "⚠️  必须手动 delete heapVar;" << endl;
    
    cout << "\n记忆口诀：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "指针变量在栈上，占用固定八字节" << endl;
    cout << "指向数据在堆上，大小由类型定" << endl;
    cout << "栈上变量自动销，堆上数据要手删" << endl;
    cout << "混淆两者易出错，理解清楚最关键" << endl;
    
    cout << "\n类比理解：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "指针就像一个门牌号（在栈上）" << endl;
    cout << "门牌号指向一栋房子（在堆上）" << endl;
    cout << "门牌号可以随时丢弃（栈自动销毁）" << endl;
    cout << "但房子不会自动消失（需要手动 delete）" << endl;
    cout << "===========================================" << endl;
    
    return 0;
}
