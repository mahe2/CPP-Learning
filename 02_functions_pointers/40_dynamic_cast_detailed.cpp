/*
 * dynamic_cast 详解
 * 问题: dynamic_cast 只能用在多态情况下，父类向子类转吗？
 * 答案: 主要用途是父类→子类，但还有侧向转换等其他用途
 * 核心: dynamic_cast 需要多态（虚函数），提供运行时类型检查
 */

#include <iostream>
#include <string>
#include <typeinfo>
using namespace std;

// ============================================================================
// 一、dynamic_cast 的使用前提
// ============================================================================

// ❌ 错误示例：没有虚函数，不能使用 dynamic_cast
class NonPolymorphicBase {
public:
    int x = 0;
    // 没有虚函数！
};

class NonPolymorphicDerived : public NonPolymorphicBase {
public:
    int y = 0;
};

// ✅ 正确示例：有虚函数，可以使用 dynamic_cast
class Base {
public:
    virtual void show() { cout << "Base" << endl; }
    virtual ~Base() {}  // 虚析构函数
};

class Derived : public Base {
public:
    void show() override { cout << "Derived" << endl; }
    void derivedMethod() { cout << "Derived 特有方法" << endl; }
};

void demonstrate_prerequisites() {
    cout << "\n=== 1. dynamic_cast 的使用前提 ===" << endl;
    
    cout << "\n前提条件（缺一不可）：" << endl;
    cout << "  1️⃣ 类必须是多态类（有虚函数）" << endl;
    cout << "  2️⃣ 必须有虚析构函数（推荐）" << endl;
    cout << "  3️⃣ 编译器开启 RTTI（运行时类型信息，默认开启）" << endl;
    
    cout << "\n为什么需要虚函数？" << endl;
    cout << "  - dynamic_cast 依赖虚函数表（vtable）存储的类型信息" << endl;
    cout << "  - 没有虚函数 = 没有 vtable = 无法运行时识别类型" << endl;
    
    // 尝试对非多态类使用 dynamic_cast
    cout << "\n❌ 尝试对非多态类使用 dynamic_cast：" << endl;
    NonPolymorphicBase* nbPtr = new NonPolymorphicDerived();
    // NonPolymorphicDerived* ndPtr = dynamic_cast<NonPolymorphicDerived*>(nbPtr);
    // 编译错误：'NonPolymorphicBase' is not polymorphic
    cout << "  编译错误：'NonPolymorphicBase' is not polymorphic" << endl;
    delete nbPtr;
    
    // 对多态类使用 dynamic_cast
    cout << "\n✅ 对多态类使用 dynamic_cast：" << endl;
    Base* basePtr = new Derived();
    Derived* derivedPtr = dynamic_cast<Derived*>(basePtr);
    if (derivedPtr) {
        cout << "  转换成功！" << endl;
        derivedPtr->show();
    }
    delete basePtr;
}

// ============================================================================
// 二、主要用途：向下转换（父类→子类）
// ============================================================================

class Animal {
public:
    virtual void makeSound() { cout << "Animal sound" << endl; }
    virtual ~Animal() {}
};

class Dog : public Animal {
public:
    void makeSound() override { cout << "Woof!" << endl; }
    void fetch() { cout << "Dog fetching ball" << endl; }
};

class Cat : public Animal {
public:
    void makeSound() override { cout << "Meow!" << endl; }
    void climb() { cout << "Cat climbing tree" << endl; }
};

void demonstrate_downcast() {
    cout << "\n=== 2. 主要用途：向下转换（父类→子类）===" << endl;
    
    cout << "\n什么是向下转换（downcast）？" << endl;
    cout << "  基类指针/引用 → 派生类指针/引用" << endl;
    cout << "  Animal* → Dog* 或 Cat*" << endl;
    
    cout << "\n场景1：转换成功（实际类型匹配）" << endl;
    Animal* animal1 = new Dog();  // 实际指向 Dog
    
    Dog* dog = dynamic_cast<Dog*>(animal1);
    if (dog) {
        cout << "  ✅ 转换成功！animal1 确实是 Dog" << endl;
        dog->makeSound();
        dog->fetch();
    } else {
        cout << "  ❌ 转换失败" << endl;
    }
    
    cout << "\n场景2：转换失败（实际类型不匹配）" << endl;
    Cat* cat = dynamic_cast<Cat*>(animal1);  // animal1 是 Dog，不是 Cat
    if (cat) {
        cout << "  ✅ 转换成功" << endl;
    } else {
        cout << "  ❌ 转换失败！animal1 不是 Cat" << endl;
        cout << "  dynamic_cast 返回 nullptr，避免了野指针" << endl;
    }
    
    delete animal1;
    
    cout << "\n为什么需要向下转换？" << endl;
    cout << "  - 基类指针只能调用基类方法" << endl;
    cout << "  - 需要调用派生类特有方法时，必须转换" << endl;
    cout << "  - dynamic_cast 提供运行时类型检查，保证安全" << endl;
    
    cout << "\n与 static_cast 对比：" << endl;
    Animal* animal2 = new Animal();  // 实际指向 Animal
    
    // static_cast 不检查，直接转换（危险！）
    Dog* dog2 = static_cast<Dog*>(animal2);
    cout << "  static_cast: 不检查类型，总是成功（危险）" << endl;
    // dog2->fetch();  // 未定义行为！animal2 不是 Dog
    
    // dynamic_cast 检查类型，失败返回 nullptr（安全）
    Dog* dog3 = dynamic_cast<Dog*>(animal2);
    if (dog3) {
        cout << "  dynamic_cast: 转换成功" << endl;
    } else {
        cout << "  dynamic_cast: 转换失败，返回 nullptr（安全）" << endl;
    }
    
    delete animal2;
}

// ============================================================================
// 三、引用转换：失败抛异常
// ============================================================================

void demonstrate_reference_cast() {
    cout << "\n=== 3. 引用转换：失败抛异常 ===" << endl;
    
    cout << "\n指针 vs 引用的区别：" << endl;
    cout << "  指针转换失败：返回 nullptr" << endl;
    cout << "  引用转换失败：抛出 bad_cast 异常" << endl;
    
    cout << "\n为什么引用不能返回 nullptr？" << endl;
    cout << "  - 引用必须指向有效对象，不能为空" << endl;
    cout << "  - 所以失败时只能抛异常" << endl;
    
    Dog dog;
    Animal& animalRef = dog;
    
    cout << "\n场景1：引用转换成功" << endl;
    try {
        Dog& dogRef = dynamic_cast<Dog&>(animalRef);
        cout << "  ✅ 引用转换成功" << endl;
        dogRef.makeSound();
    } catch (bad_cast& e) {
        cout << "  ❌ 抛出异常: " << e.what() << endl;
    }
    
    cout << "\n场景2：引用转换失败" << endl;
    Animal animal;
    Animal& animalRef2 = animal;
    
    try {
        Dog& dogRef2 = dynamic_cast<Dog&>(animalRef2);
        cout << "  ✅ 引用转换成功" << endl;
    } catch (bad_cast& e) {
        cout << "  ❌ 抛出异常: " << e.what() << endl;
        cout << "  需要用 try-catch 捕获" << endl;
    }
    
    cout << "\n使用建议：" << endl;
    cout << "  - 指针转换：检查是否为 nullptr" << endl;
    cout << "  - 引用转换：用 try-catch 捕获异常" << endl;
}

// ============================================================================
// 四、侧向转换（crosscast）：多重继承场景
// ============================================================================

class Drawable {
public:
    virtual void draw() { cout << "Drawing..." << endl; }
    virtual ~Drawable() {}
};

class Clickable {
public:
    virtual void onClick() { cout << "Clicked!" << endl; }
    virtual ~Clickable() {}
};

// 多重继承：Button 同时继承 Drawable 和 Clickable
class Button : public Drawable, public Clickable {
public:
    void draw() override { cout << "Drawing button" << endl; }
    void onClick() override { cout << "Button clicked" << endl; }
};

void demonstrate_crosscast() {
    cout << "\n=== 4. 侧向转换（crosscast）：多重继承场景 ===" << endl;
    
    cout << "\n什么是侧向转换？" << endl;
    cout << "  - 多重继承中，同一对象的不同基类指针之间转换" << endl;
    cout << "  - 例如：Drawable* → Clickable*（都指向同一个 Button 对象）" << endl;
    
    cout << "\n继承关系：" << endl;
    cout << "     Drawable    Clickable" << endl;
    cout << "         \\      /" << endl;
    cout << "          Button" << endl;
    
    Button* button = new Button();
    
    // 向上转换（隐式）
    Drawable* drawable = button;
    Clickable* clickable = button;
    
    cout << "\n向上转换（upcast）：" << endl;
    cout << "  Button* → Drawable*: " << drawable << endl;
    cout << "  Button* → Clickable*: " << clickable << endl;
    
    // 侧向转换：Drawable* → Clickable*
    cout << "\n侧向转换（crosscast）：" << endl;
    cout << "  Drawable* → Clickable*" << endl;
    
    Clickable* clickable2 = dynamic_cast<Clickable*>(drawable);
    if (clickable2) {
        cout << "  ✅ 侧向转换成功！" << endl;
        cout << "  原指针: " << drawable << endl;
        cout << "  新指针: " << clickable2 << endl;
        clickable2->onClick();
    } else {
        cout << "  ❌ 侧向转换失败" << endl;
    }
    
    cout << "\n为什么需要 dynamic_cast？" << endl;
    cout << "  - static_cast 不能做侧向转换" << endl;
    cout << "  - 只有 dynamic_cast 能在运行时查找对象的完整类型信息" << endl;
    
    delete button;
}

// ============================================================================
// 五、向上转换：可以但没必要
// ============================================================================

void demonstrate_upcast() {
    cout << "\n=== 5. 向上转换：可以但没必要 ===" << endl;
    
    cout << "\n什么是向上转换（upcast）？" << endl;
    cout << "  派生类指针/引用 → 基类指针/引用" << endl;
    cout << "  Dog* → Animal*" << endl;
    
    Derived* derived = new Derived();
    
    // 方式1：隐式转换（推荐）
    Base* base1 = derived;
    cout << "\n方式1: 隐式转换（推荐）" << endl;
    cout << "  Base* base = derived;  // 自动转换，安全" << endl;
    
    // 方式2：static_cast（显式但无额外好处）
    Base* base2 = static_cast<Base*>(derived);
    cout << "\n方式2: static_cast（显式）" << endl;
    cout << "  Base* base = static_cast<Base*>(derived);" << endl;
    
    // 方式3：dynamic_cast（可以但没必要）
    Base* base3 = dynamic_cast<Base*>(derived);
    cout << "\n方式3: dynamic_cast（没必要）" << endl;
    cout << "  Base* base = dynamic_cast<Base*>(derived);" << endl;
    cout << "  ⚠️ 向上转换总是安全的，不需要运行时检查" << endl;
    cout << "  ⚠️ 使用 dynamic_cast 只会增加运行时开销" << endl;
    
    cout << "\n结论：" << endl;
    cout << "  ✅ 向上转换用隐式转换即可" << endl;
    cout << "  ❌ 不要对向上转换使用 dynamic_cast" << endl;
    
    delete derived;
}

// ============================================================================
// 六、dynamic_cast 的性能开销
// ============================================================================

void demonstrate_performance() {
    cout << "\n=== 6. dynamic_cast 的性能开销 ===" << endl;
    
    cout << "\ndynamic_cast 的开销来源：" << endl;
    cout << "  1️⃣ 需要访问虚函数表（vtable）" << endl;
    cout << "  2️⃣ 需要查找 RTTI（运行时类型信息）" << endl;
    cout << "  3️⃣ 需要遍历继承层次结构" << endl;
    cout << "  4️⃣ 多重继承时开销更大（侧向转换）" << endl;
    
    cout << "\n性能对比：" << endl;
    cout << "  static_cast:  无开销（编译时确定）" << endl;
    cout << "  dynamic_cast: 有开销（运行时检查）" << endl;
    
    cout << "\n什么时候使用 dynamic_cast？" << endl;
    cout << "  ✅ 向下转换（父类→子类）" << endl;
    cout << "  ✅ 不确定对象的实际类型" << endl;
    cout << "  ✅ 需要类型安全检查" << endl;
    cout << "  ✅ 多重继承的侧向转换" << endl;
    
    cout << "\n什么时候用 static_cast？" << endl;
    cout << "  ✅ 向上转换（子类→父类）" << endl;
    cout << "  ✅ 确定对象的实际类型" << endl;
    cout << "  ✅ 性能敏感的代码" << endl;
    
    cout << "\n性能优化建议：" << endl;
    cout << "  1. 设计时尽量避免向下转换" << endl;
    cout << "  2. 用虚函数代替类型判断" << endl;
    cout << "  3. 确定类型时用 static_cast" << endl;
    cout << "  4. 不确定类型才用 dynamic_cast" << endl;
}

// ============================================================================
// 七、实际应用场景
// ============================================================================

class Shape {
public:
    virtual void draw() = 0;  // 纯虚函数
    virtual double area() = 0;
    virtual ~Shape() {}
};

class Circle : public Shape {
public:
    double radius;
    Circle(double r) : radius(r) {}
    
    void draw() override { cout << "Drawing circle" << endl; }
    double area() override { return 3.14159 * radius * radius; }
    void roll() { cout << "Circle rolling" << endl; }
};

class Rectangle : public Shape {
public:
    double width, height;
    Rectangle(double w, double h) : width(w), height(h) {}
    
    void draw() override { cout << "Drawing rectangle" << endl; }
    double area() override { return width * height; }
    void rotate() { cout << "Rectangle rotating" << endl; }
};

void demonstrate_practical_use() {
    cout << "\n=== 7. 实际应用场景 ===" << endl;
    
    cout << "\n场景：图形编辑器，需要处理不同类型的图形" << endl;
    
    // 图形集合（基类指针）
    Shape* shapes[3] = {
        new Circle(5.0),
        new Rectangle(4.0, 6.0),
        new Circle(3.0)
    };
    
    cout << "\n遍历所有图形，调用特定方法：" << endl;
    for (int i = 0; i < 3; i++) {
        cout << "\n图形 " << (i + 1) << ":" << endl;
        shapes[i]->draw();
        cout << "  面积: " << shapes[i]->area() << endl;
        
        // 尝试转换为 Circle
        Circle* circle = dynamic_cast<Circle*>(shapes[i]);
        if (circle) {
            cout << "  这是圆形，可以滚动" << endl;
            circle->roll();
        }
        
        // 尝试转换为 Rectangle
        Rectangle* rect = dynamic_cast<Rectangle*>(shapes[i]);
        if (rect) {
            cout << "  这是矩形，可以旋转" << endl;
            rect->rotate();
        }
    }
    
    cout << "\n更好的设计：用虚函数代替类型判断" << endl;
    cout << "  ❌ 不推荐：dynamic_cast + if-else" << endl;
    cout << "  ✅ 推荐：在基类定义虚函数，派生类重写" << endl;
    
    // 清理
    for (int i = 0; i < 3; i++) {
        delete shapes[i];
    }
}

// ============================================================================
// 八、常见错误和陷阱
// ============================================================================

void demonstrate_common_mistakes() {
    cout << "\n=== 8. 常见错误和陷阱 ===" << endl;
    
    cout << "\n❌ 错误1：对非多态类使用 dynamic_cast" << endl;
    cout << "  class NonPoly { int x; };  // 没有虚函数" << endl;
    cout << "  NonPoly* p = new NonPoly();" << endl;
    cout << "  // dynamic_cast<...>(p);  // 编译错误！" << endl;
    
    cout << "\n❌ 错误2：忘记检查返回值" << endl;
    cout << "  Animal* animal = new Animal();" << endl;
    cout << "  Dog* dog = dynamic_cast<Dog*>(animal);" << endl;
    cout << "  dog->fetch();  // 危险！dog 可能是 nullptr" << endl;
    
    cout << "\n✅ 正确做法：总是检查" << endl;
    cout << "  if (dog) { dog->fetch(); }" << endl;
    
    cout << "\n❌ 错误3：对向上转换使用 dynamic_cast" << endl;
    cout << "  Derived* d = new Derived();" << endl;
    cout << "  Base* b = dynamic_cast<Base*>(d);  // 浪费性能" << endl;
    
    cout << "\n✅ 正确做法：向上转换用隐式转换" << endl;
    cout << "  Base* b = d;  // 安全且无开销" << endl;
    
    cout << "\n❌ 错误4：过度使用 dynamic_cast" << endl;
    cout << "  if (dynamic_cast<Dog*>(animal)) { ... }" << endl;
    cout << "  else if (dynamic_cast<Cat*>(animal)) { ... }" << endl;
    cout << "  ⚠️ 违反开闭原则，添加新类型需要修改代码" << endl;
    
    cout << "\n✅ 正确做法：用虚函数" << endl;
    cout << "  animal->makeSound();  // 多态，无需类型判断" << endl;
}

// ============================================================================
// 九、与 Java 的对比
// ============================================================================

void compare_with_java() {
    cout << "\n=== 9. 与 Java 的对比 ===" << endl;
    
    cout << "\nJava 的类型转换：" << endl;
    cout << "  Animal animal = new Dog();" << endl;
    cout << "  Dog dog = (Dog)animal;  // 向下转换" << endl;
    cout << "  运行时检查，失败抛 ClassCastException" << endl;
    
    cout << "\nJava 的类型检查：" << endl;
    cout << "  if (animal instanceof Dog) {" << endl;
    cout << "      Dog dog = (Dog)animal;" << endl;
    cout << "      dog.fetch();" << endl;
    cout << "  }" << endl;
    
    cout << "\nC++ dynamic_cast 的等价操作：" << endl;
    cout << "  Animal* animal = new Dog();" << endl;
    cout << "  Dog* dog = dynamic_cast<Dog*>(animal);" << endl;
    cout << "  if (dog) { dog->fetch(); }" << endl;
    
    cout << "\n核心区别：" << endl;
    cout << "  Java:" << endl;
    cout << "    - (Type) 转换失败抛异常" << endl;
    cout << "    - instanceof 检查类型" << endl;
    cout << "    - 所有类都支持（不需要虚函数）" << endl;
    
    cout << "\n  C++:" << endl;
    cout << "    - dynamic_cast 失败返回 nullptr（指针）或抛异常（引用）" << endl;
    cout << "    - dynamic_cast 本身就能检查类型" << endl;
    cout << "    - 只支持多态类（需要虚函数）" << endl;
    
    cout << "\n相似之处：" << endl;
    cout << "  - 都是运行时类型检查（RTTI）" << endl;
    cout << "  - 都有性能开销" << endl;
    cout << "  - 都主要用于向下转换" << endl;
}

// ============================================================================
// 十、最佳实践
// ============================================================================

void best_practices() {
    cout << "\n=== 10. 最佳实践 ===" << endl;
    
    cout << "\n使用 dynamic_cast 的黄金法则：" << endl;
    cout << "  1️⃣ 只在必须的时候使用（向下转换、侧向转换）" << endl;
    cout << "  2️⃣ 总是检查返回值（nullptr 或捕获异常）" << endl;
    cout << "  3️⃣ 优先考虑虚函数，而不是类型判断" << endl;
    cout << "  4️⃣ 确定类型时用 static_cast" << endl;
    cout << "  5️⃣ 向上转换用隐式转换" << endl;
    
    cout << "\n设计建议：" << endl;
    cout << "  ✅ 好设计：用多态，让对象自己决定行为" << endl;
    cout << "  ❌ 坏设计：用 dynamic_cast 判断类型，再执行不同逻辑" << endl;
    
    cout << "\n示例对比：" << endl;
    cout << "\n❌ 不好的设计：" << endl;
    cout << "  void process(Animal* animal) {" << endl;
    cout << "      if (Dog* dog = dynamic_cast<Dog*>(animal)) {" << endl;
    cout << "          dog->fetch();" << endl;
    cout << "      } else if (Cat* cat = dynamic_cast<Cat*>(animal)) {" << endl;
    cout << "          cat->climb();" << endl;
    cout << "      }" << endl;
    cout << "  }" << endl;
    
    cout << "\n✅ 好的设计：" << endl;
    cout << "  class Animal {" << endl;
    cout << "      virtual void doSpecialAction() = 0;" << endl;
    cout << "  };" << endl;
    cout << "  class Dog : public Animal {" << endl;
    cout << "      void doSpecialAction() override { fetch(); }" << endl;
    cout << "  };" << endl;
    cout << "  void process(Animal* animal) {" << endl;
    cout << "      animal->doSpecialAction();  // 多态，无需类型判断" << endl;
    cout << "  }" << endl;
    
    cout << "\n何时必须使用 dynamic_cast：" << endl;
    cout << "  - 处理第三方库的类层次结构" << endl;
    cout << "  - 实现访问者模式" << endl;
    cout << "  - 反序列化对象时" << endl;
    cout << "  - 调试和日志记录" << endl;
}

// ============================================================================
// 十一、核心总结
// ============================================================================

void summary() {
    cout << "\n========================================" << endl;
    cout << "           核心总结" << endl;
    cout << "========================================" << endl;
    
    cout << "\n问题: dynamic_cast 只能用在多态情况下，父类向子类转吗？" << endl;
    cout << "\n答案: ✅ 主要用于父类→子类，但不仅限于此" << endl;
    
    cout << "\ndynamic_cast 的三大用途：" << endl;
    cout << "  1️⃣ 向下转换（downcast）- 最常用" << endl;
    cout << "     父类指针/引用 → 子类指针/引用" << endl;
    cout << "     Base* → Derived*" << endl;
    
    cout << "\n  2️⃣ 侧向转换（crosscast）- 多重继承" << endl;
    cout << "     同一对象的不同基类指针之间转换" << endl;
    cout << "     Drawable* → Clickable* (同一个 Button 对象)" << endl;
    
    cout << "\n  3️⃣ 向上转换（upcast）- 可以但没必要" << endl;
    cout << "     子类指针/引用 → 父类指针/引用" << endl;
    cout << "     Derived* → Base* (用隐式转换就行)" << endl;
    
    cout << "\n使用前提（必须满足）：" << endl;
    cout << "  ✅ 类必须是多态类（有虚函数）" << endl;
    cout << "  ✅ 建议有虚析构函数" << endl;
    cout << "  ✅ 编译器开启 RTTI（默认开启）" << endl;
    
    cout << "\n转换失败的处理：" << endl;
    cout << "  指针转换：返回 nullptr" << endl;
    cout << "  引用转换：抛出 bad_cast 异常" << endl;
    
    cout << "\n与 static_cast 对比：" << endl;
    cout << "  ┌──────────────┬──────────────┬──────────────┐" << endl;
    cout << "  │ 转换类型     │ static_cast  │ dynamic_cast │" << endl;
    cout << "  ├──────────────┼──────────────┼──────────────┤" << endl;
    cout << "  │ 向上转换     │ ✅ 推荐      │ ⚠️ 没必要    │" << endl;
    cout << "  │ 向下转换     │ ⚠️ 不安全    │ ✅ 安全      │" << endl;
    cout << "  │ 侧向转换     │ ❌ 不支持    │ ✅ 支持      │" << endl;
    cout << "  │ 运行时检查   │ ❌ 无        │ ✅ 有        │" << endl;
    cout << "  │ 性能开销     │ 无           │ 有           │" << endl;
    cout << "  │ 需要虚函数   │ ❌ 不需要    │ ✅ 必须      │" << endl;
    cout << "  └──────────────┴──────────────┴──────────────┘" << endl;
    
    cout << "\n快速选择指南：" << endl;
    cout << "  向上转换（子→父）→ 隐式转换" << endl;
    cout << "  向下转换（父→子）→ dynamic_cast（不确定类型时）" << endl;
    cout << "  向下转换（父→子）→ static_cast（确定类型时）" << endl;
    cout << "  侧向转换（多重继承）→ dynamic_cast" << endl;
    
    cout << "\n记忆口诀：" << endl;
    cout << "  dynamic 很安全，运行时检查防出错" << endl;
    cout << "  父转子最常用，侧向转换也能做" << endl;
    cout << "  必须有虚函数，没有就编译错" << endl;
    cout << "  返回 nullptr，或者抛异常" << endl;
    cout << "  有性能开销，能避免就避免" << endl;
    cout << "  优先用多态，而不是判类型" << endl;
    
    cout << "\n最佳实践：" << endl;
    cout << "  ✅ 只在必须时使用 dynamic_cast" << endl;
    cout << "  ✅ 总是检查返回值" << endl;
    cout << "  ✅ 优先考虑虚函数" << endl;
    cout << "  ✅ 向上转换用隐式转换" << endl;
    cout << "  ✅ 确定类型用 static_cast" << endl;
    
    cout << "\n========================================" << endl;
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "\n╔═══════════════════════════════════════════════════╗" << endl;
    cout << "║         dynamic_cast 详解                         ║" << endl;
    cout << "║   dynamic_cast 只能用在多态情况下，父类向子类转吗？║" << endl;
    cout << "╚═══════════════════════════════════════════════════╝" << endl;
    
    // 1. 使用前提
    demonstrate_prerequisites();
    
    // 2. 向下转换（主要用途）
    demonstrate_downcast();
    
    // 3. 引用转换
    demonstrate_reference_cast();
    
    // 4. 侧向转换
    demonstrate_crosscast();
    
    // 5. 向上转换
    demonstrate_upcast();
    
    // 6. 性能开销
    demonstrate_performance();
    
    // 7. 实际应用
    demonstrate_practical_use();
    
    // 8. 常见错误
    demonstrate_common_mistakes();
    
    // 9. 与 Java 对比
    compare_with_java();
    
    // 10. 最佳实践
    best_practices();
    
    // 11. 总结
    summary();
    
    return 0;
}
