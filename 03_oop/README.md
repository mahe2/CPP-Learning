# 03 - 面向对象编程

## 📚 本章内容

学习 C++ 的面向对象特性：类、继承、多态。

### 示例文件

1. **01_classes_objects.cpp** - 类和对象、构造/析构函数
2. **02_inheritance.cpp** - 继承、多继承
3. **03_polymorphism.cpp** - 多态、虚函数、抽象类

---

## 🚀 如何编译运行

```bash
# 编译单个文件
g++ -std=c++17 01_classes_objects.cpp -o classes
./classes

# 编译多态示例
g++ -std=c++17 03_polymorphism.cpp -o polymorphism
./polymorphism
```

---

## 🔑 核心概念

### 1. 类和对象

```cpp
class Person {
private:
    string name;  // 私有成员
    int age;

public:
    // 构造函数
    Person(string n, int a) : name(n), age(a) {}
    
    // 析构函数（C++ 特有）
    ~Person() {
        cout << "对象销毁: " << name << endl;
    }
    
    // 成员方法
    void introduce() {
        cout << "我是 " << name << endl;
    }
};
```

**对比 Java:**
- C++ 有析构函数，Java 有 finalize（不推荐）
- C++ 对象可以在栈或堆上创建
- C++ 使用成员初始化列表（更高效）

### 2. 继承

```cpp
// 基类
class Animal {
protected:
    string name;
public:
    virtual void makeSound() {
        cout << "动物叫声" << endl;
    }
};

// 派生类
class Dog : public Animal {
public:
    void makeSound() override {
        cout << "汪汪！" << endl;
    }
};
```

**继承方式：**
- `public` 继承：最常用，类似 Java
- `protected` 继承：较少用
- `private` 继承：最少用

**C++ vs Java:**
| 特性 | C++ | Java |
|------|-----|------|
| 多继承 | ✅ 支持 | ❌ 不支持（接口除外） |
| 继承关键字 | `: public` | `extends` |
| 构造顺序 | 基类→派生类 | 父类→子类 |
| 析构顺序 | 派生类→基类 | 无（GC） |

### 3. 多态

```cpp
// 抽象基类
class Shape {
public:
    // 纯虚函数（抽象方法）
    virtual double area() const = 0;
    
    // 虚析构函数（重要！）
    virtual ~Shape() {}
};

// 实现类
class Circle : public Shape {
    double radius;
public:
    double area() const override {
        return 3.14159 * radius * radius;
    }
};

// 多态使用
Shape* shape = new Circle(5.0);
cout << shape->area();  // 调用 Circle::area()
delete shape;
```

**多态三要素：**
1. 继承关系
2. 虚函数（`virtual`）
3. 基类指针/引用指向派生类对象

---

## 💡 重要知识点

### 析构函数（Destructor）

**C++ 独有特性，非常重要！**

```cpp
class Resource {
    int* data;
public:
    Resource(int size) {
        data = new int[size];
        cout << "资源分配" << endl;
    }
    
    ~Resource() {
        delete[] data;
        cout << "资源释放" << endl;
    }
};
```

**何时调用：**
- 栈对象：作用域结束时自动调用
- 堆对象：`delete` 时调用

**虚析构函数：**
```cpp
class Base {
public:
    virtual ~Base() {}  // 必须是虚函数！
};

class Derived : public Base {
    int* data;
public:
    ~Derived() {
        delete[] data;  // 确保被调用
    }
};

Base* obj = new Derived();
delete obj;  // 正确调用 Derived::~Derived()
```

**规则：如果类有虚函数，必须有虚析构函数！**

---

### 访问控制

| 修饰符 | 类内部 | 派生类 | 外部 | 说明 |
|--------|--------|--------|------|------|
| `private` | ✅ | ❌ | ❌ | 只能类内部访问 |
| `protected` | ✅ | ✅ | ❌ | 派生类可以访问 |
| `public` | ✅ | ✅ | ✅ | 所有地方都能访问 |

**与 Java 对比：**
- C++ 默认 `private`，Java 类默认 `package-private`
- C++ 有 `friend` 友元机制
- C++ 没有 `package` 访问级别

---

### 虚函数表（VTable）

**多态的实现原理：**

```cpp
class Base {
public:
    virtual void func1() {}
    virtual void func2() {}
};

class Derived : public Base {
public:
    void func1() override {}  // 重写
};
```

**内存布局：**
```
Base 对象:
+------------------+
| vptr (指向VTable) |  <- 虚函数表指针
+------------------+

VTable:
+------------------+
| Base::func1      |
| Base::func2      |
+------------------+

Derived 对象:
+------------------+
| vptr (指向VTable) |
+------------------+

VTable:
+------------------+
| Derived::func1   |  <- 重写
| Base::func2      |     <- 继承
+------------------+
```

---

## 🎯 最佳实践

### 1. 使用成员初始化列表
```cpp
// ✅ 好：使用初始化列表
class Person {
    string name;
    int age;
public:
    Person(string n, int a) : name(n), age(a) {}
};

// ❌ 差：构造函数体内赋值
class Person {
    string name;
    int age;
public:
    Person(string n, int a) {
        name = n;  // 先默认构造，再赋值（效率低）
        age = a;
    }
};
```

### 2. 虚析构函数
```cpp
// ✅ 好：有虚函数就要有虚析构函数
class Base {
public:
    virtual void func() {}
    virtual ~Base() {}  // 虚析构函数
};

// ❌ 差：可能导致内存泄漏
class Base {
public:
    virtual void func() {}
    ~Base() {}  // 非虚析构函数
};
```

### 3. override 关键字
```cpp
// ✅ 好：明确标记重写
class Derived : public Base {
public:
    void func() override {}  // 编译器检查
};

// ❌ 差：可能拼写错误
class Derived : public Base {
public:
    void func() {}  // 如果基类没有 func，不会报错
};
```

---

## 🐛 常见错误

### 1. 忘记虚析构函数
```cpp
Base* obj = new Derived();
delete obj;  // 如果 Base 没有虚析构函数，只调用 Base::~Base()
```

### 2. 对象切片
```cpp
Derived d;
Base b = d;  // ❌ 对象切片，丢失 Derived 部分
b.func();    // 调用 Base::func()，不是多态

Base& b = d; // ✅ 使用引用，保持多态
```

### 3. 纯虚函数未实现
```cpp
class Base {
public:
    virtual void func() = 0;  // 纯虚函数
};

Base obj;  // ❌ 错误！抽象类不能实例化
```

---

## 📖 练习建议

### 基础练习
1. 创建一个 `BankAccount` 类，包含存款、取款方法
2. 实现 `Student` 继承 `Person`
3. 创建形状类层次结构（Shape → Circle/Rectangle）

### 进阶练习
1. 实现一个动物园管理系统（多态）
2. 创建一个图形绘制系统
3. 实现简单的员工管理系统（继承层次）

---

## 🔥 面试重点

1. **析构函数和虚析构函数的区别？**
2. **什么是多态？如何实现？**
3. **纯虚函数和虚函数的区别？**
4. **C++ 如何支持多继承？有什么问题？**
5. **什么是对象切片？如何避免？**
6. **虚函数表是什么？**

---

**下一章：现代 C++ 特性 ➡️**
