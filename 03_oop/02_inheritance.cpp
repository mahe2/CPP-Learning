/*
 * 继承 - 代码复用和扩展
 * 
 * C++ 支持单继承和多继承
 */

#include <iostream>
#include <string>

using namespace std;

// ========== 基类 ==========
class Animal {
protected:  // protected: 子类可以访问
    string name;
    int age;

public:
    Animal(string n, int a) : name(n), age(a) {
        cout << "Animal 构造: " << name << endl;
    }
    
    ~Animal() {
        cout << "Animal 析构: " << name << endl;
    }
    
    void eat() {
        cout << name << " 正在吃东西" << endl;
    }
    
    void sleep() {
        cout << name << " 正在睡觉" << endl;
    }
    
    // 虚函数（可被子类重写）
    virtual void makeSound() {
        cout << name << " 发出声音" << endl;
    }
    
    // 显示信息
    void display() {
        cout << "名字: " << name << ", 年龄: " << age << endl;
    }
};

// ========== 派生类：Dog ==========
class Dog : public Animal {  // public 继承
private:
    string breed;

public:
    // 调用基类构造函数
    Dog(string n, int a, string b) : Animal(n, a), breed(b) {
        cout << "Dog 构造: " << name << endl;
    }
    
    ~Dog() {
        cout << "Dog 析构: " << name << endl;
    }
    
    // 重写基类方法
    void makeSound() override {  // override 关键字（C++11）
        cout << name << " 汪汪叫！" << endl;
    }
    
    // 子类特有方法
    void wagTail() {
        cout << name << " 摇尾巴" << endl;
    }
    
    void displayBreed() {
        cout << name << " 是 " << breed << endl;
    }
};

// ========== 派生类：Cat ==========
class Cat : public Animal {
private:
    bool isIndoor;

public:
    Cat(string n, int a, bool indoor) : Animal(n, a), isIndoor(indoor) {
        cout << "Cat 构造: " << name << endl;
    }
    
    ~Cat() {
        cout << "Cat 析构: " << name << endl;
    }
    
    void makeSound() override {
        cout << name << " 喵喵叫！" << endl;
    }
    
    void climb() {
        cout << name << " 正在爬树" << endl;
    }
};

// ========== 多继承示例 ==========
class Flyer {
public:
    void fly() {
        cout << "正在飞行" << endl;
    }
};

class Swimmer {
public:
    void swim() {
        cout << "正在游泳" << endl;
    }
};

// 鸭子既能飞也能游泳
class Duck : public Animal, public Flyer, public Swimmer {
public:
    Duck(string n, int a) : Animal(n, a) {
        cout << "Duck 构造: " << name << endl;
    }
    
    ~Duck() {
        cout << "Duck 析构: " << name << endl;
    }
    
    void makeSound() override {
        cout << name << " 嘎嘎叫！" << endl;
    }
};

int main() {
    cout << "=== 创建 Dog 对象 ===" << endl;
    Dog dog("旺财", 3, "金毛");
    dog.display();       // 继承自 Animal
    dog.eat();           // 继承自 Animal
    dog.makeSound();     // 重写的方法
    dog.wagTail();       // 自己的方法
    dog.displayBreed();
    
    cout << "\n=== 创建 Cat 对象 ===" << endl;
    Cat cat("咪咪", 2, true);
    cat.display();
    cat.sleep();
    cat.makeSound();
    cat.climb();
    
    cout << "\n=== 多继承：Duck ===" << endl;
    Duck duck("唐老鸭", 1);
    duck.display();      // 来自 Animal
    duck.makeSound();    // 重写的方法
    duck.fly();          // 来自 Flyer
    duck.swim();         // 来自 Swimmer
    
    cout << "\n=== 基类指针指向派生类对象 ===" << endl;
    Animal* animals[3];
    animals[0] = new Dog("小黑", 2, "拉布拉多");
    animals[1] = new Cat("小白", 1, false);
    animals[2] = new Duck("小鸭", 1);
    
    // 多态：通过基类指针调用重写的方法
    for (int i = 0; i < 3; i++) {
        animals[i]->makeSound();  // 调用各自重写的方法
    }
    
    // 释放内存
    for (int i = 0; i < 3; i++) {
        delete animals[i];
    }
    
    cout << "\n=== 程序结束 ===" << endl;
    return 0;
}

/*
 * 对比 Java:
 * 
 * public class Animal {
 *     protected String name;
 *     
 *     public Animal(String name) {
 *         this.name = name;
 *     }
 *     
 *     public void makeSound() {
 *         System.out.println("发出声音");
 *     }
 * }
 * 
 * public class Dog extends Animal {
 *     public Dog(String name) {
 *         super(name);  // 调用父类构造函数
 *     }
 *     
 *     @Override
 *     public void makeSound() {
 *         System.out.println("汪汪叫");
 *     }
 * }
 * 
 * // Java 不支持多继承，只能实现多个接口
 * public interface Flyer {
 *     void fly();
 * }
 * 
 * public class Duck extends Animal implements Flyer {
 *     // ...
 * }
 * 
 * 
 * 对比 Kotlin:
 * 
 * open class Animal(protected val name: String) {
 *     open fun makeSound() {
 *         println("发出声音")
 *     }
 * }
 * 
 * class Dog(name: String) : Animal(name) {
 *     override fun makeSound() {
 *         println("汪汪叫")
 *     }
 * }
 * 
 * 
 * C++ 继承特点：
 * 1. 支持多继承（Java/Kotlin 不支持）
 * 2. 继承方式：public, protected, private
 * 3. virtual 关键字支持多态
 * 4. 构造顺序：基类 → 派生类
 * 5. 析构顺序：派生类 → 基类
 * 6. override 关键字明确重写（C++11）
 */
