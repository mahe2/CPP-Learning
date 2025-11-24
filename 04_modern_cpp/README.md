# 04 - 现代 C++ 特性

## 📚 本章内容

学习 C++11/14/17 的现代特性，让 C++ 更接近 Java/Kotlin 的便利性。

### 示例文件

1. **01_smart_pointers.cpp** - 智能指针（unique_ptr, shared_ptr, weak_ptr）
2. **02_lambda.cpp** - Lambda 表达式
3. **03_stl_containers.cpp** - STL 容器和算法

---

## 🚀 如何编译运行

```bash
# 使用 C++17 标准编译
g++ -std=c++17 01_smart_pointers.cpp -o smart_ptrs
./smart_ptrs

# Lambda 示例
g++ -std=c++17 02_lambda.cpp -o lambda
./lambda

# STL 容器
g++ -std=c++17 03_stl_containers.cpp -o stl
./stl
```

---

## 🌟 核心特性

### 1. 智能指针 - 告别手动 new/delete

```cpp
#include <memory>

// unique_ptr - 独占所有权
unique_ptr<int> ptr1 = make_unique<int>(42);

// shared_ptr - 共享所有权
shared_ptr<int> ptr2 = make_shared<int>(100);
shared_ptr<int> ptr3 = ptr2;  // 引用计数+1

// weak_ptr - 弱引用
weak_ptr<int> weak = ptr2;
```

**对比传统方式：**
```cpp
// ❌ 传统方式
int* ptr = new int(42);
delete ptr;  // 容易忘记

// ✅ 智能指针
auto ptr = make_unique<int>(42);
// 自动释放，不需要 delete
```

**何时使用：**
- `unique_ptr`：默认选择，独占资源
- `shared_ptr`：需要多个所有者时
- `weak_ptr`：观察者模式，避免循环引用

---

### 2. Lambda 表达式

```cpp
// 基本语法
auto add = [](int a, int b) { return a + b; };
cout << add(3, 5);  // 8

// 捕获外部变量
int x = 10;
auto addX = [x](int n) { return n + x; };

// 在 STL 算法中使用
vector<int> nums = {5, 2, 8, 1, 9};
sort(nums.begin(), nums.end(), [](int a, int b) {
    return a > b;  // 降序
});
```

**捕获方式：**
- `[]` - 不捕获
- `[=]` - 按值捕获所有
- `[&]` - 按引用捕获所有
- `[x]` - 按值捕获 x
- `[&x]` - 按引用捕获 x
- `[=, &x]` - 混合捕获

**对比 Java/Kotlin：**
```java
// Java
list.stream().filter(n -> n > 5).forEach(System.out::println);

// Kotlin
list.filter { it > 5 }.forEach { println(it) }

// C++
auto filtered = vector<int>();
copy_if(list.begin(), list.end(), back_inserter(filtered),
    [](int n) { return n > 5; });
```

---

### 3. STL 容器

| C++ 容器 | Java/Kotlin 对应 | 说明 |
|----------|------------------|------|
| `vector<T>` | `ArrayList` / `MutableList` | 动态数组 |
| `list<T>` | `LinkedList` | 双向链表 |
| `set<T>` | `TreeSet` | 有序集合 |
| `map<K,V>` | `TreeMap` | 有序映射 |
| `unordered_set<T>` | `HashSet` | 哈希集合 |
| `unordered_map<K,V>` | `HashMap` | 哈希映射 |
| `stack<T>` | `Stack` | 栈 |
| `queue<T>` | `Queue` | 队列 |

**常用操作：**
```cpp
// vector
vector<int> vec = {1, 2, 3};
vec.push_back(4);
vec.pop_back();
cout << vec.size();

// map
map<string, int> ages;
ages["Alice"] = 25;
ages["Bob"] = 30;

for (const auto& [name, age] : ages) {
    cout << name << ": " << age << endl;
}

// set
set<int> nums = {3, 1, 4, 1, 5};  // 自动去重和排序
nums.insert(2);
```

---

## 💡 其他现代特性

### auto 关键字
```cpp
auto x = 42;              // int
auto y = 3.14;            // double
auto s = "Hello";         // const char*
auto vec = vector<int>(); // vector<int>
```

### 范围 for 循环
```cpp
vector<int> nums = {1, 2, 3, 4, 5};

// C++11
for (int n : nums) {
    cout << n << " ";
}

// 引用（可修改）
for (int& n : nums) {
    n *= 2;
}

// 常量引用（高效，只读）
for (const auto& n : nums) {
    cout << n << " ";
}
```

### 初始化列表
```cpp
// 统一初始化语法
int x{42};
vector<int> vec{1, 2, 3, 4, 5};
map<string, int> m{{"Alice", 25}, {"Bob", 30}};

// 类成员初始化
class Person {
    string name = "Unknown";  // C++11
    int age{0};               // C++11
};
```

### nullptr
```cpp
// ❌ 旧方式
int* ptr = NULL;  // NULL 是宏，值为 0

// ✅ 新方式
int* ptr = nullptr;  // 类型安全的空指针
```

### 右值引用和移动语义
```cpp
// 避免不必要的复制
vector<int> createVector() {
    vector<int> v{1, 2, 3};
    return v;  // 自动移动，不复制
}

auto vec = createVector();  // 高效！

// 显式移动
unique_ptr<int> p1 = make_unique<int>(42);
unique_ptr<int> p2 = move(p1);  // 转移所有权
```

### 结构化绑定（C++17）
```cpp
// 解构 pair
pair<int, string> p{1, "Alice"};
auto [id, name] = p;  // C++17
cout << id << ": " << name << endl;

// 解构 map
map<string, int> ages{{"Alice", 25}, {"Bob", 30}};
for (const auto& [name, age] : ages) {
    cout << name << ": " << age << endl;
}
```

---

## 🎯 最佳实践

### 1. 优先使用智能指针
```cpp
// ❌ 避免
int* ptr = new int(42);
// ...容易忘记 delete

// ✅ 推荐
auto ptr = make_unique<int>(42);
// 自动管理
```

### 2. 使用 auto 减少冗余
```cpp
// ❌ 冗长
vector<string>::iterator it = vec.begin();

// ✅ 简洁
auto it = vec.begin();
```

### 3. const 引用传递大对象
```cpp
// ❌ 低效（复制）
void process(vector<int> vec);

// ✅ 高效（引用）
void process(const vector<int>& vec);
```

### 4. Lambda 替代函数对象
```cpp
// ❌ 繁琐
struct Greater {
    bool operator()(int a, int b) const {
        return a > b;
    }
};
sort(vec.begin(), vec.end(), Greater());

// ✅ 简洁
sort(vec.begin(), vec.end(), [](int a, int b) {
    return a > b;
});
```

---

## 📊 性能对比

| 特性 | 传统方式 | 现代方式 | 性能 |
|------|----------|----------|------|
| 内存管理 | new/delete | 智能指针 | 相同 |
| 函数对象 | 结构体 | Lambda | 相同/更好 |
| 容器 | 数组 | vector | 更好 |
| 类型推导 | 显式 | auto | 编译时 |
| 移动语义 | 复制 | move | 更好 |

**结论：现代 C++ 更安全、更简洁，性能不降反升！**

---

## 🐛 常见错误

### 1. 智能指针混用
```cpp
// ❌ 错误
auto ptr = make_unique<int>(42);
int* rawPtr = ptr.get();
delete rawPtr;  // 危险！double delete

// ✅ 正确
auto ptr = make_unique<int>(42);
// 只让智能指针管理
```

### 2. Lambda 捕获悬空引用
```cpp
// ❌ 危险
auto makeLambda() {
    int x = 10;
    return [&x]() { return x; };  // x 已销毁
}

// ✅ 安全
auto makeLambda() {
    int x = 10;
    return [x]() { return x; };  // 按值捕获
}
```

### 3. 忘记 const 引用
```cpp
// ❌ 低效
for (auto item : largeVector) {  // 每次复制
    process(item);
}

// ✅ 高效
for (const auto& item : largeVector) {  // 不复制
    process(item);
}
```

---

## 📖 学习建议

1. **智能指针**：优先学习，解决最大痛点
2. **Lambda**：掌握捕获方式，结合 STL 使用
3. **STL 容器**：对比 Java Collections 理解
4. **auto**：适度使用，保持代码可读性
5. **移动语义**：理解原理，提升性能

---

## 🔥 面试高频问题

1. **智能指针的区别？**
2. **Lambda 如何捕获变量？**
3. **vector 和 list 的区别？**
4. **什么是移动语义？**
5. **map 和 unordered_map 的区别？**

---

**下一章：综合练习 ➡️**
