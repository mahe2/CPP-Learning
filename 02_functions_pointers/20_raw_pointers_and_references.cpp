/**
 * C++ 裸指针（Raw Pointer）和裸引用（Raw Reference）深度解析
 * 
 * 核心问题：
 * 1. 什么是裸指针和裸引用？
 * 2. 为什么叫"裸"（Raw）？
 * 3. 与智能指针的对比
 * 4. 使用场景和最佳实践
 */

#include <iostream>
#include <string>
#include <memory>
#include <vector>

using namespace std;

// ============================================
// 1. 裸指针（Raw Pointer）
// ============================================

void raw_pointer_basics() {
    cout << "\n" << string(60, '=') << endl;
    cout << "1. 裸指针（Raw Pointer）基础" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n什么是裸指针？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "裸指针 = C++ 原生的、未封装的指针" << endl;
    cout << "  int* ptr;  // 这就是裸指针" << endl;
    cout << "\n特点：" << endl;
    cout << "  ❌ 没有自动内存管理" << endl;
    cout << "  ❌ 没有所有权语义" << endl;
    cout << "  ❌ 需要手动 new/delete" << endl;
    cout << "  ⚠️  容易造成内存泄漏" << endl;
    cout << "  ⚠️  容易造成悬空指针" << endl;
    
    cout << "\n为什么叫'裸'（Raw）？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. 没有任何封装和保护" << endl;
    cout << "2. 直接暴露内存地址" << endl;
    cout << "3. 完全依赖程序员手动管理" << endl;
    cout << "4. 就像'裸奔'一样，没有安全保障" << endl;
    
    cout << "\n示例：" << endl;
    cout << "---------------------------------------" << endl;
    
    // 裸指针的使用
    int* rawPtr = new int(42);  // ⚠️ 手动分配内存
    cout << "int* rawPtr = new int(42);" << endl;
    cout << "  地址: " << rawPtr << endl;
    cout << "  值: " << *rawPtr << endl;
    
    *rawPtr = 100;
    cout << "\n修改后: " << *rawPtr << endl;
    
    delete rawPtr;  // ⚠️ 必须手动释放！
    cout << "\ndelete rawPtr;  // 必须手动释放" << endl;
    
    // 忘记 delete 会导致内存泄漏！
    rawPtr = nullptr;  // 避免悬空指针
}

// ============================================
// 2. 裸引用（Raw Reference）
// ============================================

void raw_reference_basics() {
    cout << "\n" << string(60, '=') << endl;
    cout << "2. 裸引用（Raw Reference）基础" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n什么是裸引用？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "裸引用 = C++ 原生的、未封装的引用" << endl;
    cout << "  int& ref = x;  // 这就是裸引用" << endl;
    cout << "\n特点：" << endl;
    cout << "  ✅ 不涉及内存分配" << endl;
    cout << "  ✅ 必须初始化" << endl;
    cout << "  ✅ 不能为 null" << endl;
    cout << "  ✅ 不能重新绑定" << endl;
    cout << "  ⚠️  可能变成悬空引用" << endl;
    
    cout << "\n为什么叫'裸'（Raw）？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. 没有封装保护" << endl;
    cout << "2. 直接绑定到对象" << endl;
    cout << "3. 没有生命周期检查" << endl;
    cout << "4. 可能引用已销毁的对象" << endl;
    
    cout << "\n示例：" << endl;
    cout << "---------------------------------------" << endl;
    
    int value = 42;
    int& rawRef = value;  // 裸引用
    
    cout << "int value = 42;" << endl;
    cout << "int& rawRef = value;" << endl;
    cout << "  原始值: " << value << endl;
    cout << "  引用值: " << rawRef << endl;
    
    rawRef = 100;  // 修改引用 = 修改原始对象
    cout << "\nrawRef = 100;" << endl;
    cout << "  原始值: " << value << " (已改变)" << endl;
    cout << "  引用值: " << rawRef << endl;
}

// ============================================
// 3. 裸指针 vs 智能指针
// ============================================

class Resource {
public:
    string name;
    
    Resource(const string& n) : name(n) {
        cout << "  [构造] Resource: " << name << endl;
    }
    
    ~Resource() {
        cout << "  [析构] Resource: " << name << endl;
    }
    
    void use() {
        cout << "  [使用] Resource: " << name << endl;
    }
};

void raw_vs_smart_pointers() {
    cout << "\n" << string(60, '=') << endl;
    cout << "3. 裸指针 vs 智能指针" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n场景1: 裸指针（需要手动管理）" << endl;
    cout << "---------------------------------------" << endl;
    {
        cout << "Resource* rawPtr = new Resource(\"Raw\");" << endl;
        Resource* rawPtr = new Resource("Raw");
        rawPtr->use();
        
        cout << "\ndelete rawPtr;  // ⚠️ 必须手动调用！" << endl;
        delete rawPtr;  // 忘记这行 = 内存泄漏！
        cout << "(如果忘记 delete，资源永远不会释放)" << endl;
    }
    
    cout << "\n场景2: unique_ptr（自动管理）" << endl;
    cout << "---------------------------------------" << endl;
    {
        cout << "auto smartPtr = make_unique<Resource>(\"Smart\");" << endl;
        auto smartPtr = make_unique<Resource>("Smart");
        smartPtr->use();
        
        cout << "\n// 离开作用域，自动释放" << endl;
    }  // ✅ 自动调用析构函数
    cout << "(无需手动 delete，自动清理)" << endl;
    
    cout << "\n场景3: shared_ptr（共享所有权）" << endl;
    cout << "---------------------------------------" << endl;
    {
        cout << "auto ptr1 = make_shared<Resource>(\"Shared\");" << endl;
        auto ptr1 = make_shared<Resource>("Shared");
        cout << "  引用计数: " << ptr1.use_count() << endl;
        
        {
            cout << "\nauto ptr2 = ptr1;  // 共享所有权" << endl;
            auto ptr2 = ptr1;
            cout << "  引用计数: " << ptr1.use_count() << endl;
        }
        
        cout << "\nptr2 离开作用域" << endl;
        cout << "  引用计数: " << ptr1.use_count() << endl;
    }  // 最后一个引用消失，自动释放
    cout << "(引用计数归零，自动清理)" << endl;
}

// ============================================
// 4. 裸指针的危险
// ============================================

void raw_pointer_dangers() {
    cout << "\n" << string(60, '=') << endl;
    cout << "4. 裸指针的危险" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n危险1: 内存泄漏" << endl;
    cout << "---------------------------------------" << endl;
    cout << "void memoryLeak() {" << endl;
    cout << "    int* ptr = new int(42);" << endl;
    cout << "    // 忘记 delete ptr;" << endl;
    cout << "    return;  // ❌ 内存泄漏！" << endl;
    cout << "}" << endl;
    
    cout << "\n危险2: 悬空指针（Dangling Pointer）" << endl;
    cout << "---------------------------------------" << endl;
    cout << "int* danglingPtr = nullptr;" << endl;
    cout << "{" << endl;
    cout << "    int x = 42;" << endl;
    cout << "    danglingPtr = &x;  // 指向局部变量" << endl;
    cout << "}  // x 被销毁" << endl;
    cout << "// 现在 danglingPtr 指向无效内存！" << endl;
    
    cout << "\n危险3: 重复释放（Double Delete）" << endl;
    cout << "---------------------------------------" << endl;
    cout << "int* ptr = new int(42);" << endl;
    cout << "delete ptr;" << endl;
    cout << "delete ptr;  // ❌ 重复释放，崩溃！" << endl;
    
    cout << "\n危险4: 使用已释放的内存（Use After Free）" << endl;
    cout << "---------------------------------------" << endl;
    cout << "int* ptr = new int(42);" << endl;
    cout << "delete ptr;" << endl;
    cout << "cout << *ptr;  // ❌ 访问已释放的内存！" << endl;
    
    cout << "\n危险5: 数组越界" << endl;
    cout << "---------------------------------------" << endl;
    cout << "int* arr = new int[5];" << endl;
    cout << "arr[10] = 100;  // ❌ 越界访问，未定义行为！" << endl;
    cout << "delete[] arr;  // 注意：数组要用 delete[]" << endl;
}

// ============================================
// 5. 裸引用的危险
// ============================================

// 返回局部变量的引用（危险！）
// int& dangerousFunction() {
//     int local = 42;
//     return local;  // ❌ 返回局部变量的引用！
// }  // local 被销毁，返回悬空引用

void raw_reference_dangers() {
    cout << "\n" << string(60, '=') << endl;
    cout << "5. 裸引用的危险" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n危险1: 悬空引用（返回局部变量的引用）" << endl;
    cout << "---------------------------------------" << endl;
    cout << "int& dangerousFunction() {" << endl;
    cout << "    int local = 42;" << endl;
    cout << "    return local;  // ❌ 局部变量的引用" << endl;
    cout << "}  // local 被销毁" << endl;
    cout << "int& ref = dangerousFunction();" << endl;
    cout << "// ref 现在是悬空引用！" << endl;
    
    cout << "\n危险2: 引用已删除的对象" << endl;
    cout << "---------------------------------------" << endl;
    cout << "int* ptr = new int(42);" << endl;
    cout << "int& ref = *ptr;  // 引用指针指向的对象" << endl;
    cout << "delete ptr;       // 删除对象" << endl;
    cout << "// ref 现在是悬空引用！" << endl;
    
    cout << "\n危险3: 引用生命周期短的对象" << endl;
    cout << "---------------------------------------" << endl;
    cout << "int& ref;" << endl;
    cout << "{" << endl;
    cout << "    int x = 42;" << endl;
    cout << "    ref = x;  // 引用局部变量" << endl;
    cout << "}  // x 被销毁" << endl;
    cout << "// ref 现在是悬空引用！" << endl;
}

// ============================================
// 6. 什么时候使用裸指针/裸引用？
// ============================================

void when_to_use_raw() {
    cout << "\n" << string(60, '=') << endl;
    cout << "6. 什么时候使用裸指针/裸引用？" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n裸指针的合理使用场景：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ 1. 观察者指针（不拥有对象）" << endl;
    cout << "   - 只是观察，不负责释放" << endl;
    cout << "   - 所有权在其他地方" << endl;
    cout << "   void observe(const Widget* ptr);" << endl;
    
    cout << "\n✅ 2. 遗留 C 库接口" << endl;
    cout << "   - 与 C 代码交互" << endl;
    cout << "   - FILE* fp = fopen(...);" << endl;
    
    cout << "\n✅ 3. 性能关键代码" << endl;
    cout << "   - 避免智能指针开销" << endl;
    cout << "   - 但要确保安全" << endl;
    
    cout << "\n✅ 4. 可选参数（可以为 nullptr）" << endl;
    cout << "   void func(int* optional = nullptr);" << endl;
    
    cout << "\n裸引用的合理使用场景：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ 1. 函数参数（对象必定存在）" << endl;
    cout << "   void process(const string& str);" << endl;
    
    cout << "\n✅ 2. 别名（为已存在的对象起别名）" << endl;
    cout << "   int x = 42;" << endl;
    cout << "   int& ref = x;  // ref 是 x 的别名" << endl;
    
    cout << "\n✅ 3. 返回类成员的引用" << endl;
    cout << "   string& getName() { return name; }" << endl;
    
    cout << "\n✅ 4. 迭代器解引用" << endl;
    cout << "   for (auto& item : vec) { ... }" << endl;
}

// ============================================
// 7. 最佳实践
// ============================================

void best_practices() {
    cout << "\n" << string(60, '=') << endl;
    cout << "7. 最佳实践" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n原则1: 优先使用智能指针" << endl;
    cout << "---------------------------------------" << endl;
    cout << "❌ int* ptr = new int(42);" << endl;
    cout << "   delete ptr;" << endl;
    cout << "\n✅ auto ptr = make_unique<int>(42);" << endl;
    cout << "   // 自动管理，不会泄漏" << endl;
    
    cout << "\n原则2: 如果必须用裸指针，遵循规则" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ 立即初始化为 nullptr" << endl;
    cout << "   int* ptr = nullptr;" << endl;
    cout << "\n✅ new 之后立即检查" << endl;
    cout << "   ptr = new int(42);" << endl;
    cout << "   if (!ptr) { /* 错误处理 */ }" << endl;
    cout << "\n✅ delete 之后立即置空" << endl;
    cout << "   delete ptr;" << endl;
    cout << "   ptr = nullptr;" << endl;
    cout << "\n✅ 使用 RAII（资源获取即初始化）" << endl;
    cout << "   把裸指针封装在类中" << endl;
    
    cout << "\n原则3: 引用的安全使用" << endl;
    cout << "---------------------------------------" << endl;
    cout << "✅ 只引用生命周期明确的对象" << endl;
    cout << "✅ 不要返回局部变量的引用" << endl;
    cout << "✅ 函数参数优先用 const 引用" << endl;
    cout << "   void func(const string& str);" << endl;
    
    cout << "\n原则4: 所有权明确" << endl;
    cout << "---------------------------------------" << endl;
    cout << "unique_ptr: 独占所有权" << endl;
    cout << "shared_ptr: 共享所有权" << endl;
    cout << "weak_ptr:   不拥有，观察" << endl;
    cout << "裸指针:     只观察，不拥有" << endl;
    cout << "引用:       只观察，不拥有" << endl;
}

// ============================================
// 8. 对比表格
// ============================================

void comparison_table() {
    cout << "\n" << string(60, '=') << endl;
    cout << "8. 完整对比表" << endl;
    cout << string(60, '=') << endl;
    
    cout << R"(
┌────────────────┬─────────────┬─────────────┬─────────────┐
│  特性          │  裸指针     │  智能指针   │  裸引用     │
├────────────────┼─────────────┼─────────────┼─────────────┤
│ 自动释放       │ ❌ 手动     │ ✅ 自动     │ ✅ 不需要   │
├────────────────┼─────────────┼─────────────┼─────────────┤
│ 可为 null      │ ✅ 是       │ ✅ 是       │ ❌ 否       │
├────────────────┼─────────────┼─────────────┼─────────────┤
│ 可重新赋值     │ ✅ 是       │ ✅ 是       │ ❌ 否       │
├────────────────┼─────────────┼─────────────┼─────────────┤
│ 所有权语义     │ ❌ 无       │ ✅ 有       │ ❌ 无       │
├────────────────┼─────────────┼─────────────┼─────────────┤
│ 内存泄漏风险   │ ⚠️  高      │ ✅ 低       │ ✅ 无       │
├────────────────┼─────────────┼─────────────┼─────────────┤
│ 悬空风险       │ ⚠️  高      │ ✅ 低       │ ⚠️  中      │
├────────────────┼─────────────┼─────────────┼─────────────┤
│ 性能开销       │ ✅ 无       │ ⚠️  小      │ ✅ 无       │
├────────────────┼─────────────┼─────────────┼─────────────┤
│ 使用复杂度     │ ⚠️  高      │ ✅ 低       │ ✅ 低       │
├────────────────┼─────────────┼─────────────┼─────────────┤
│ 推荐使用       │ ⚠️  少用    │ ✅ 推荐     │ ✅ 推荐     │
│                │ (只观察)    │ (拥有所有权)│ (不拥有)    │
└────────────────┴─────────────┴─────────────┴─────────────┘

裸指针/引用的"裸"含义：
┌────────────────────────────────────────────────┐
│  1. 没有封装保护（Bare/Naked）                 │
│  2. 直接暴露底层实现（Raw/Direct）             │
│  3. 没有自动管理（Unmanaged）                  │
│  4. 完全依赖程序员（Manual）                   │
│  5. 就像"裸奔"，危险而不安全                   │
└────────────────────────────────────────────────┘
)" << endl;
}

// ============================================
// 9. 与 Java/Kotlin 对比
// ============================================

void language_comparison() {
    cout << "\n" << string(60, '=') << endl;
    cout << "9. 与 Java/Kotlin 对比" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\nJava：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "Integer obj = new Integer(42);" << endl;
    cout << "// 不需要 delete，垃圾回收自动处理" << endl;
    cout << "obj = null;  // 可以为 null" << endl;
    cout << "\n特点：" << endl;
    cout << "✅ 所有对象引用都是'托管'的" << endl;
    cout << "✅ 垃圾回收器自动释放内存" << endl;
    cout << "✅ 不会内存泄漏（但可能内存溢出）" << endl;
    cout << "⚠️  没有真正的'裸指针'概念" << endl;
    cout << "⚠️  GC 有性能开销" << endl;
    
    cout << "\nKotlin：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "var obj: MyClass? = MyClass(42)" << endl;
    cout << "// 不需要手动释放" << endl;
    cout << "obj = null  // 可空类型" << endl;
    cout << "\n特点：" << endl;
    cout << "✅ 类型系统区分可空/非空" << endl;
    cout << "✅ 编译时防止空指针异常" << endl;
    cout << "✅ 自动内存管理" << endl;
    cout << "⚠️  没有'裸指针/引用'概念" << endl;
    
    cout << "\nC++：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "// 裸指针（手动管理）" << endl;
    cout << "int* rawPtr = new int(42);" << endl;
    cout << "delete rawPtr;  // 必须手动！" << endl;
    cout << "\n// 智能指针（自动管理）" << endl;
    cout << "auto smartPtr = make_unique<int>(42);" << endl;
    cout << "// 自动释放" << endl;
    cout << "\n// 裸引用" << endl;
    cout << "int& ref = *smartPtr;" << endl;
    cout << "\n特点：" << endl;
    cout << "✅ 灵活：可以选择手动或自动管理" << endl;
    cout << "✅ 高性能：没有 GC 开销" << endl;
    cout << "✅ 精确控制：清楚对象生命周期" << endl;
    cout << "⚠️  责任大：需要程序员小心管理" << endl;
    cout << "⚠️  易出错：内存泄漏、悬空指针等" << endl;
}

// ============================================
// 10. 实际示例
// ============================================

// 示例：使用裸指针作为观察者（不拥有）
class Observable {
private:
    vector<class Observer*> observers;  // 裸指针：只观察，不拥有
    
public:
    void addObserver(Observer* obs) {
        if (obs) {
            observers.push_back(obs);
        }
    }
    
    void notifyAll();
};

// 示例：使用智能指针管理资源
class Document {
private:
    unique_ptr<Resource> resource;  // 独占所有权
    
public:
    Document(const string& name) 
        : resource(make_unique<Resource>(name)) {}
    
    // 提供裸指针观察（不转移所有权）
    Resource* getResource() const {
        return resource.get();
    }
    
    // 提供引用访问
    Resource& getResourceRef() const {
        return *resource;
    }
};

void practical_examples() {
    cout << "\n" << string(60, '=') << endl;
    cout << "10. 实际示例" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n场景1: 智能指针拥有，裸指针观察" << endl;
    cout << "---------------------------------------" << endl;
    {
        auto doc = make_unique<Document>("MyDoc");
        
        // 获取裸指针观察（不拥有）
        Resource* observerPtr = doc->getResource();
        cout << "裸指针观察: ";
        observerPtr->use();
        
        // 获取引用访问
        Resource& ref = doc->getResourceRef();
        cout << "引用访问: ";
        ref.use();
    }  // doc 销毁，resource 自动释放
    cout << "(智能指针自动清理)" << endl;
    
    cout << "\n场景2: 函数参数使用引用" << endl;
    cout << "---------------------------------------" << endl;
    auto processResource = [](const Resource& res) {
        cout << "处理资源: " << res.name << endl;
    };
    
    auto res = make_unique<Resource>("Param");
    processResource(*res);  // 传递引用
}

// ============================================
// Main
// ============================================

int main() {
    cout << "===========================================" << endl;
    cout << "  裸指针和裸引用深度解析" << endl;
    cout << "===========================================" << endl;
    
    raw_pointer_basics();
    raw_reference_basics();
    raw_vs_smart_pointers();
    raw_pointer_dangers();
    raw_reference_dangers();
    when_to_use_raw();
    best_practices();
    comparison_table();
    language_comparison();
    practical_examples();
    
    cout << "\n===========================================" << endl;
    cout << "核心总结" << endl;
    cout << "===========================================" << endl;
    
    cout << "\n什么是裸指针/裸引用？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "裸（Raw）= 原始的、未封装的、没有保护的" << endl;
    cout << "\n裸指针: int* ptr" << endl;
    cout << "  - C++ 原生指针，没有自动内存管理" << endl;
    cout << "  - 需要手动 new/delete" << endl;
    cout << "  - 容易内存泄漏和悬空指针" << endl;
    cout << "\n裸引用: int& ref" << endl;
    cout << "  - C++ 原生引用，直接绑定对象" << endl;
    cout << "  - 不涉及内存分配" << endl;
    cout << "  - 可能变成悬空引用" << endl;
    
    cout << "\n为什么叫'裸'？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. 没有封装保护（像裸奔一样）" << endl;
    cout << "2. 直接暴露内存地址" << endl;
    cout << "3. 没有自动管理机制" << endl;
    cout << "4. 完全依赖程序员手动控制" << endl;
    
    cout << "\n什么时候用？" << endl;
    cout << "---------------------------------------" << endl;
    cout << "裸指针：" << endl;
    cout << "  ✅ 观察者模式（不拥有所有权）" << endl;
    cout << "  ✅ C 库接口" << endl;
    cout << "  ✅ 性能关键代码" << endl;
    cout << "  ✅ 可选参数（可为 nullptr）" << endl;
    cout << "\n裸引用：" << endl;
    cout << "  ✅ 函数参数（对象必定存在）" << endl;
    cout << "  ✅ 返回类成员" << endl;
    cout << "  ✅ 别名" << endl;
    
    cout << "\n最佳实践：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "1. 优先使用智能指针（unique_ptr, shared_ptr）" << endl;
    cout << "2. 函数参数优先用 const 引用" << endl;
    cout << "3. 只在观察时使用裸指针（不拥有所有权）" << endl;
    cout << "4. 避免返回局部变量的引用" << endl;
    cout << "5. 如果必须用裸指针，遵循 RAII 原则" << endl;
    
    cout << "\n记忆口诀：" << endl;
    cout << "---------------------------------------" << endl;
    cout << "裸指针引用没保护，就像裸奔需小心" << endl;
    cout << "智能指针自动管，引用参数最常见" << endl;
    cout << "裸指针观察不拥有，所有权交给智能针" << endl;
    cout << "手动管理易出错，现代C++要记牢" << endl;
    cout << "===========================================" << endl;
    
    return 0;
}

/**
 * 术语解释：
 * 
 * Raw Pointer（裸指针）：
 *   - 原始的、未封装的 C++ 指针
 *   - int* ptr = new int(42);
 *   - 需要手动 delete
 * 
 * Raw Reference（裸引用）：
 *   - 原始的、未封装的 C++ 引用
 *   - int& ref = x;
 *   - 不需要 delete，但可能悬空
 * 
 * Smart Pointer（智能指针）：
 *   - 封装的、自动管理的指针
 *   - unique_ptr, shared_ptr, weak_ptr
 *   - 自动释放内存
 * 
 * "裸"（Raw）的含义：
 *   - Bare/Naked（赤裸的）
 *   - Unmanaged（未托管的）
 *   - Direct（直接的）
 *   - Manual（手动的）
 */
