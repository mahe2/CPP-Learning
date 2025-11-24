/*
 * 类作用域详解
 * 
 * 类作用域 ≠ 静态变量
 * 类作用域包含：普通成员变量、静态成员变量、成员函数、静态成员函数等
 */

#include <iostream>
#include <string>

using namespace std;

class Student {
// ========== 类作用域中的成员 ==========
private:
    // 1. 普通成员变量（非静态）- 每个对象都有独立的副本
    string name;
    int age;
    
    // 2. 静态成员变量 - 所有对象共享，属于类本身
    static int studentCount;  // 声明
    static string schoolName;
    
public:
    // 3. 构造函数
    Student(string n, int a) : name(n), age(a) {
        studentCount++;  // 每创建一个对象，计数加1
        cout << "学生创建: " << name << " (总数: " << studentCount << ")" << endl;
    }
    
    // 4. 析构函数
    ~Student() {
        studentCount--;
        cout << "学生销毁: " << name << " (剩余: " << studentCount << ")" << endl;
    }
    
    // 5. 普通成员函数（非静态）- 需要对象才能调用
    void introduce() const {
        cout << "我叫 " << name << ", " << age << " 岁" << endl;
        cout << "学校: " << schoolName << endl;  // 可以访问静态成员
    }
    
    // 6. 静态成员函数 - 不需要对象，通过类名调用
    static int getStudentCount() {
        // 注意：静态函数只能访问静态成员！
        return studentCount;
        // return age;  // ❌ 错误！不能访问非静态成员
    }
    
    static void setSchoolName(const string& school) {
        schoolName = school;
    }
    
    static void showSchoolInfo() {
        cout << "学校名称: " << schoolName << endl;
        cout << "学生总数: " << studentCount << endl;
        // introduce();  // ❌ 错误！不能调用非静态函数
    }
    
    // 7. Getter 方法
    string getName() const { return name; }
    int getAge() const { return age; }
};

// ========== 静态成员变量的定义（必须在类外）==========
int Student::studentCount = 0;     // 初始化为0
string Student::schoolName = "未设置";

// ========== 演示函数 ==========
void demonstrateClassScope() {
    cout << "\n=== 类作用域演示 ===" << endl;
    
    // 访问静态成员（通过类名，不需要对象）
    cout << "初始学生数: " << Student::getStudentCount() << endl;
    Student::setSchoolName("北京大学");
    Student::showSchoolInfo();
    
    cout << "\n创建对象：" << endl;
    
    // 创建对象
    Student s1("张三", 20);
    Student s2("李四", 21);
    
    // 访问普通成员（需要对象）
    cout << "\n对象成员访问：" << endl;
    s1.introduce();
    s2.introduce();
    
    // 静态成员被所有对象共享
    cout << "\n通过对象访问静态成员：" << endl;
    cout << "学生总数（通过类名）: " << Student::getStudentCount() << endl;
    cout << "学生总数（通过对象）: " << s1.getStudentCount() << endl;  // 也可以，但不推荐
    
    {
        cout << "\n进入内部作用域：" << endl;
        Student s3("王五", 22);
        cout << "当前学生总数: " << Student::getStudentCount() << endl;
    }  // s3 离开作用域，被销毁
    
    cout << "\n离开内部作用域后：" << endl;
    cout << "当前学生总数: " << Student::getStudentCount() << endl;
}

// ========== 更多示例：不同类型的作用域 ==========
class Example {
public:
    // 1. 普通成员变量 - 对象作用域
    int instanceVar = 10;
    
    // 2. 静态成员变量 - 类作用域（共享）
    static int staticVar;
    
    // 3. 常量成员
    const int constVar = 100;
    
    // 4. 静态常量成员（编译时常量）
    static constexpr int BUFFER_SIZE = 1024;
    
    void showMembers() {
        cout << "\n成员变量值：" << endl;
        cout << "  普通成员: " << instanceVar << endl;
        cout << "  静态成员: " << staticVar << endl;
        cout << "  常量成员: " << constVar << endl;
        cout << "  静态常量: " << BUFFER_SIZE << endl;
    }
    
    static void modifyStatic(int val) {
        staticVar = val;  // ✅ 可以修改静态成员
        // instanceVar = val;  // ❌ 错误！不能访问非静态成员
    }
};

int Example::staticVar = 0;  // 静态成员定义

int main() {
    cout << "=== C++ 类作用域详解 ===" << endl;
    
    // ========== 1. 类作用域演示 ==========
    demonstrateClassScope();
    
    // ========== 2. 对比普通成员和静态成员 ==========
    cout << "\n\n=== 普通成员 vs 静态成员 ===" << endl;
    
    Example obj1, obj2;
    
    // 修改普通成员 - 只影响当前对象
    obj1.instanceVar = 20;
    obj2.instanceVar = 30;
    
    cout << "obj1 普通成员: " << obj1.instanceVar << endl;  // 20
    cout << "obj2 普通成员: " << obj2.instanceVar << endl;  // 30
    
    // 修改静态成员 - 影响所有对象
    Example::modifyStatic(999);
    
    cout << "obj1 静态成员: " << obj1.staticVar << endl;  // 999
    cout << "obj2 静态成员: " << obj2.staticVar << endl;  // 999
    cout << "类   静态成员: " << Example::staticVar << endl;  // 999
    
    // ========== 3. 作用域总结 ==========
    cout << "\n\n=== 作用域对比 ===" << endl;
    
    obj1.showMembers();
    obj2.showMembers();
    
    return 0;
}

/*
 * ========== 总结 ==========
 * 
 * 1. 类作用域（Class Scope）：
 *    - 指在类内部定义的所有成员
 *    - 包括：普通成员变量、静态成员变量、成员函数、静态函数等
 *    - 作用域范围：整个类
 * 
 * 2. 普通成员变量（非静态）：
 *    - 每个对象都有独立的副本
 *    - 必须通过对象访问
 *    - 对象销毁时一起销毁
 * 
 * 3. 静态成员变量：
 *    - 所有对象共享同一份
 *    - 可以通过类名直接访问（推荐）
 *    - 也可以通过对象访问（不推荐）
 *    - 程序结束时才销毁
 *    - 必须在类外定义和初始化
 * 
 * 4. 访问规则：
 *    - 普通成员函数：可以访问所有成员（静态+非静态）
 *    - 静态成员函数：只能访问静态成员
 * 
 * 
 * ========== 对比表 ==========
 * 
 * ┌─────────────────┬──────────────┬──────────────┐
 * │   特性          │   普通成员   │   静态成员   │
 * ├─────────────────┼──────────────┼──────────────┤
 * │ 属于            │ 对象         │ 类           │
 * │ 内存位置        │ 对象内存     │ 全局区       │
 * │ 生命周期        │ 对象生命周期 │ 程序生命周期 │
 * │ 访问方式        │ 对象.成员    │ 类::成员     │
 * │ 初始化          │ 构造函数     │ 类外定义     │
 * │ 每个对象独立    │ ✅           │ ❌（共享）   │
 * └─────────────────┴──────────────┴──────────────┘
 * 
 * 
 * ========== Java/Kotlin 对比 ==========
 * 
 * Java:
 * class Student {
 *     private String name;        // 普通成员（实例变量）
 *     private static int count;   // 静态成员（类变量）
 *     
 *     public void introduce() {   // 普通方法
 *         System.out.println(name);
 *     }
 *     
 *     public static int getCount() {  // 静态方法
 *         return count;
 *     }
 * }
 * 
 * Kotlin:
 * class Student(private val name: String) {
 *     fun introduce() {           // 普通方法
 *         println(name)
 *     }
 *     
 *     companion object {          // 伴生对象（类似静态）
 *         var count = 0
 *         fun getCount() = count
 *     }
 * }
 * 
 * 
 * ========== 使用建议 ==========
 * 
 * 1. 何时使用静态成员：
 *    - 需要在所有对象间共享的数据（如计数器）
 *    - 与对象无关的工具函数
 *    - 常量定义（配合 const 或 constexpr）
 * 
 * 2. 何时使用普通成员：
 *    - 对象特有的属性（如姓名、年龄）
 *    - 需要操作对象状态的方法
 * 
 * 3. 注意事项：
 *    - 静态成员必须在类外定义（constexpr 除外）
 *    - 静态函数不能访问非静态成员
 *    - 通过类名访问静态成员更清晰
 */
