/*
 * 智能指针 - 自动内存管理
 * 
 * C++11 引入智能指针，解决手动 new/delete 的问题
 * 类似 Java/Kotlin 的自动内存管理
 */

#include <iostream>
#include <memory>  // 智能指针头文件
#include <string>
#include <vector>

using namespace std;

class Resource {
private:
    string name;
    
public:
    Resource(string n) : name(n) {
        cout << "Resource 构造: " << name << endl;
    }
    
    ~Resource() {
        cout << "Resource 析构: " << name << endl;
    }
    
    void use() {
        cout << "使用资源: " << name << endl;
    }
};

// ========== unique_ptr - 独占所有权 ==========
void uniquePtrDemo() {
    cout << "\n=== unique_ptr 示例 ===" << endl;
    
    // 创建 unique_ptr（推荐使用 make_unique）
    unique_ptr<Resource> ptr1 = make_unique<Resource>("资源1");
    ptr1->use();
    
    // unique_ptr 不能复制，只能移动
    // unique_ptr<Resource> ptr2 = ptr1;  // ❌ 错误！不能复制
    unique_ptr<Resource> ptr2 = move(ptr1);  // ✅ 移动所有权
    
    if (ptr1 == nullptr) {
        cout << "ptr1 已移动，现在为空" << endl;
    }
    
    ptr2->use();
    
    // 作用域结束，自动释放资源（无需 delete）
}

// ========== shared_ptr - 共享所有权 ==========
void sharedPtrDemo() {
    cout << "\n=== shared_ptr 示例 ===" << endl;
    
    // 创建 shared_ptr
    shared_ptr<Resource> ptr1 = make_shared<Resource>("共享资源");
    cout << "引用计数: " << ptr1.use_count() << endl;  // 1
    
    {
        // 多个 shared_ptr 可以指向同一资源
        shared_ptr<Resource> ptr2 = ptr1;  // 复制，引用计数+1
        shared_ptr<Resource> ptr3 = ptr1;
        
        cout << "引用计数: " << ptr1.use_count() << endl;  // 3
        
        ptr2->use();
        ptr3->use();
        
    }  // ptr2 和 ptr3 离开作用域，引用计数-2
    
    cout << "引用计数: " << ptr1.use_count() << endl;  // 1
    
    // ptr1 离开作用域时，引用计数为0，自动释放资源
}

// ========== weak_ptr - 弱引用 ==========
void weakPtrDemo() {
    cout << "\n=== weak_ptr 示例 ===" << endl;
    
    shared_ptr<Resource> sharedPtr = make_shared<Resource>("弱引用资源");
    weak_ptr<Resource> weakPtr = sharedPtr;  // weak_ptr 不增加引用计数
    
    cout << "shared_ptr 引用计数: " << sharedPtr.use_count() << endl;  // 1
    
    // 使用 weak_ptr 时需要先转换为 shared_ptr
    if (shared_ptr<Resource> locked = weakPtr.lock()) {
        locked->use();
        cout << "引用计数: " << locked.use_count() << endl;  // 2
    } else {
        cout << "资源已释放" << endl;
    }
    
    // 释放 shared_ptr
    sharedPtr.reset();
    
    // weak_ptr 不阻止资源释放
    if (weakPtr.expired()) {
        cout << "资源已被释放" << endl;
    }
}

// ========== 智能指针与容器 ==========
void smartPointerInContainer() {
    cout << "\n=== 智能指针与容器 ===" << endl;
    
    // 使用 vector 存储智能指针
    vector<unique_ptr<Resource>> resources;
    
    resources.push_back(make_unique<Resource>("A"));
    resources.push_back(make_unique<Resource>("B"));
    resources.push_back(make_unique<Resource>("C"));
    
    for (const auto& res : resources) {
        res->use();
    }
    
    // 容器销毁时自动释放所有资源
}

// ========== 自定义删除器 ==========
void customDeleterDemo() {
    cout << "\n=== 自定义删除器 ===" << endl;
    
    // 自定义删除器（用于非 new 分配的资源）
    auto deleter = [](Resource* r) {
        cout << "自定义删除器调用" << endl;
        delete r;
    };
    
    unique_ptr<Resource, decltype(deleter)> ptr(
        new Resource("自定义删除"),
        deleter
    );
    
    ptr->use();
}

int main() {
    cout << "=== 智能指针全面示例 ===" << endl;
    
    // 1. unique_ptr 演示
    uniquePtrDemo();
    
    // 2. shared_ptr 演示
    sharedPtrDemo();
    
    // 3. weak_ptr 演示
    weakPtrDemo();
    
    // 4. 智能指针与容器
    smartPointerInContainer();
    
    // 5. 自定义删除器
    customDeleterDemo();
    
    cout << "\n=== 程序结束 ===" << endl;
    return 0;
}

/*
 * 智能指针对比：
 * 
 * | 类型 | 所有权 | 复制 | 引用计数 | 使用场景 |
 * |------|--------|------|----------|----------|
 * | unique_ptr | 独占 | ❌ | 无 | 唯一拥有者 |
 * | shared_ptr | 共享 | ✅ | 有 | 多个拥有者 |
 * | weak_ptr | 弱引用 | ✅ | 不增加 | 观察者，避免循环引用 |
 * 
 * 
 * 对比 Java/Kotlin:
 * 
 * === Java ===
 * // Java 自动管理内存，所有对象引用类似 shared_ptr
 * Resource res = new Resource("资源");
 * // GC 自动回收
 * 
 * === C++ (传统方式) ===
 * Resource* res = new Resource("资源");
 * delete res;  // 手动释放
 * 
 * === C++ (智能指针) ===
 * unique_ptr<Resource> res = make_unique<Resource>("资源");
 * // 自动释放，类似 Java
 * 
 * 
 * 使用建议：
 * 1. 优先使用 unique_ptr（最轻量）
 * 2. 需要共享所有权时用 shared_ptr
 * 3. 避免循环引用时用 weak_ptr
 * 4. 永远不要混用智能指针和裸指针
 * 5. 使用 make_unique/make_shared 创建（更安全）
 * 
 * 
 * 常见问题：
 * 1. unique_ptr 不能复制，只能移动
 * 2. shared_ptr 有性能开销（原子操作）
 * 3. weak_ptr 使用前需要检查资源是否存在
 * 4. 循环引用会导致内存泄漏（用 weak_ptr 解决）
 */
