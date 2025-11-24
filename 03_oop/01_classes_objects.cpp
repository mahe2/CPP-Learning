/*
 * 类和对象 - C++ 面向对象基础
 * 
 * C++ 的类与 Java/Kotlin 的类非常相似
 */

#include <iostream>
#include <string>

using namespace std;

// ========== 类定义 ==========
class Person {
// 访问修饰符
private:    // 私有成员，只能在类内部访问
    string name;
    int age;

public:     // 公有成员，可以从外部访问
    // 构造函数（类似 Java 构造器）
    Person() {
        name = "Unknown";
        age = 0;
        cout << "默认构造函数被调用" << endl;
    }
    
    // 带参数的构造函数
    Person(string n, int a) {
        name = n;
        age = a;
        cout << "参数构造函数被调用: " << name << endl;
    }
    
    // 析构函数（C++ 特有，对象销毁时自动调用）
    ~Person() {
        cout << "析构函数被调用: " << name << endl;
    }
    
    // Getter 方法
    string getName() const {  // const 表示不修改对象
        return name;
    }
    
    int getAge() const {
        return age;
    }
    
    // Setter 方法
    void setName(string n) {
        name = n;
    }
    
    void setAge(int a) {
        if (a >= 0) {
            age = a;
        }
    }
    
    // 普通成员方法
    void introduce() const {
        cout << "我叫 " << name << ", 今年 " << age << " 岁。" << endl;
    }
    
    // 静态方法（类似 Java static 方法）
    static void printInfo() {
        cout << "这是 Person 类" << endl;
    }
};

// ========== 成员初始化列表（推荐方式）==========
class Student {
private:
    string name;
    int id;
    double gpa;

public:
    // 使用初始化列表（更高效）
    Student(string n, int i, double g) : name(n), id(i), gpa(g) {
        cout << "Student 构造: " << name << endl;
    }
    
    ~Student() {
        cout << "Student 析构: " << name << endl;
    }
    
    void display() const {
        cout << "学生: " << name 
             << ", ID: " << id 
             << ", GPA: " << gpa << endl;
    }
};

int main() {
    cout << "=== 创建对象 ===" << endl;
    
    // 栈上创建对象（自动管理，类似 Java 局部变量）
    Person p1;  // 调用默认构造函数
    p1.setName("Alice");
    p1.setAge(25);
    p1.introduce();
    
    Person p2("Bob", 30);  // 调用参数构造函数
    p2.introduce();
    
    // ========== 堆上创建对象 ==========
    cout << "\n=== 动态创建对象 ===" << endl;
    
    Person* p3 = new Person("Charlie", 35);
    p3->introduce();  // 使用 -> 访问指针指向对象的成员
    
    // 手动释放
    delete p3;  // 调用析构函数
    
    // ========== 对象数组 ==========
    cout << "\n=== 对象数组 ===" << endl;
    
    Student students[] = {
        Student("Tom", 1001, 3.8),
        Student("Jerry", 1002, 3.5),
        Student("Mike", 1003, 3.9)
    };
    
    for (const Student& s : students) {
        s.display();
    }
    
    // ========== 静态方法调用 ==========
    cout << "\n=== 静态方法 ===" << endl;
    Person::printInfo();  // 通过类名调用
    
    cout << "\n=== 程序结束，对象销毁 ===" << endl;
    // 栈对象会自动调用析构函数
    
    return 0;
}

/*
 * 对比 Java:
 * 
 * public class Person {
 *     private String name;
 *     private int age;
 *     
 *     // 构造函数
 *     public Person() {
 *         name = "Unknown";
 *         age = 0;
 *     }
 *     
 *     public Person(String name, int age) {
 *         this.name = name;
 *         this.age = age;
 *     }
 *     
 *     // 没有析构函数，GC 自动回收
 *     
 *     public String getName() {
 *         return name;
 *     }
 *     
 *     public void setName(String name) {
 *         this.name = name;
 *     }
 * }
 * 
 * // 使用
 * Person p1 = new Person();  // Java 总是在堆上创建
 * Person p2 = new Person("Alice", 25);
 * 
 * 
 * 对比 Kotlin:
 * 
 * class Person(var name: String = "Unknown", var age: Int = 0) {
 *     fun introduce() {
 *         println("我叫 $name, 今年 $age 岁。")
 *     }
 * }
 * 
 * // 使用
 * val p1 = Person()
 * val p2 = Person("Alice", 25)
 * 
 * 
 * C++ 特点：
 * 1. 可以在栈或堆上创建对象
 * 2. 有析构函数，对象销毁时自动调用
 * 3. 需要手动 delete 堆对象
 * 4. 支持成员初始化列表
 * 5. const 成员函数保证不修改对象
 */
