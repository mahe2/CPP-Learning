/*
 * 多态 - 运行时动态绑定
 * 
 * 通过虚函数实现多态
 */

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ========== 抽象基类（类似 Java 抽象类）==========
class Shape {
protected:
    string name;

public:
    Shape(string n) : name(n) {}
    
    // 纯虚函数（= 0），类似 Java 抽象方法
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    
    // 虚函数（可以有默认实现）
    virtual void display() const {
        cout << "形状: " << name << endl;
    }
    
    // 虚析构函数（重要！）
    virtual ~Shape() {
        cout << "Shape 析构: " << name << endl;
    }
    
    string getName() const { return name; }
};

// ========== 派生类：Circle ==========
class Circle : public Shape {
private:
    double radius;

public:
    Circle(double r) : Shape("圆形"), radius(r) {}
    
    ~Circle() {
        cout << "Circle 析构" << endl;
    }
    
    // 实现纯虚函数
    double area() const override {
        return 3.14159 * radius * radius;
    }
    
    double perimeter() const override {
        return 2 * 3.14159 * radius;
    }
    
    void display() const override {
        Shape::display();  // 调用基类方法
        cout << "  半径: " << radius << endl;
        cout << "  面积: " << area() << endl;
        cout << "  周长: " << perimeter() << endl;
    }
};

// ========== 派生类：Rectangle ==========
class Rectangle : public Shape {
private:
    double width;
    double height;

public:
    Rectangle(double w, double h) : Shape("矩形"), width(w), height(h) {}
    
    ~Rectangle() {
        cout << "Rectangle 析构" << endl;
    }
    
    double area() const override {
        return width * height;
    }
    
    double perimeter() const override {
        return 2 * (width + height);
    }
    
    void display() const override {
        Shape::display();
        cout << "  宽: " << width << ", 高: " << height << endl;
        cout << "  面积: " << area() << endl;
        cout << "  周长: " << perimeter() << endl;
    }
};

// ========== 派生类：Triangle ==========
class Triangle : public Shape {
private:
    double a, b, c;  // 三边长

public:
    Triangle(double side1, double side2, double side3) 
        : Shape("三角形"), a(side1), b(side2), c(side3) {}
    
    ~Triangle() {
        cout << "Triangle 析构" << endl;
    }
    
    double area() const override {
        // 海伦公式
        double s = perimeter() / 2;
        return sqrt(s * (s - a) * (s - b) * (s - c));
    }
    
    double perimeter() const override {
        return a + b + c;
    }
    
    void display() const override {
        Shape::display();
        cout << "  三边: " << a << ", " << b << ", " << c << endl;
        cout << "  面积: " << area() << endl;
        cout << "  周长: " << perimeter() << endl;
    }
};

// ========== 计算总面积（多态应用）==========
double calculateTotalArea(const vector<Shape*>& shapes) {
    double total = 0;
    for (const Shape* shape : shapes) {
        total += shape->area();  // 多态调用
    }
    return total;
}

int main() {
    cout << "=== 多态示例 ===" << endl;
    
    // 创建不同的形状对象
    Circle circle(5.0);
    Rectangle rect(4.0, 6.0);
    Triangle tri(3.0, 4.0, 5.0);
    
    // 基类指针指向派生类对象
    Shape* shape1 = &circle;
    Shape* shape2 = &rect;
    Shape* shape3 = &tri;
    
    // 多态调用（运行时确定调用哪个方法）
    cout << "\n=== 通过基类指针调用 ===" << endl;
    shape1->display();
    cout << endl;
    shape2->display();
    cout << endl;
    shape3->display();
    
    // ========== 使用容器存储多态对象 ==========
    cout << "\n=== 容器中的多态 ===" << endl;
    
    vector<Shape*> shapes;
    shapes.push_back(new Circle(3.0));
    shapes.push_back(new Rectangle(5.0, 8.0));
    shapes.push_back(new Triangle(6.0, 8.0, 10.0));
    shapes.push_back(new Circle(7.0));
    
    // 遍历容器，多态调用
    cout << "所有形状信息：\n" << endl;
    for (const Shape* s : shapes) {
        s->display();
        cout << endl;
    }
    
    // 计算总面积
    double totalArea = calculateTotalArea(shapes);
    cout << "总面积: " << totalArea << endl;
    
    // 释放内存（调用虚析构函数）
    cout << "\n=== 释放内存 ===" << endl;
    for (Shape* s : shapes) {
        delete s;  // 正确调用派生类的析构函数
    }
    
    cout << "\n=== 程序结束 ===" << endl;
    return 0;
}

/*
 * 对比 Java:
 * 
 * public abstract class Shape {
 *     protected String name;
 *     
 *     public abstract double area();
 *     public abstract double perimeter();
 *     
 *     public void display() {
 *         System.out.println("形状: " + name);
 *     }
 * }
 * 
 * public class Circle extends Shape {
 *     private double radius;
 *     
 *     @Override
 *     public double area() {
 *         return Math.PI * radius * radius;
 *     }
 *     
 *     @Override
 *     public double perimeter() {
 *         return 2 * Math.PI * radius;
 *     }
 * }
 * 
 * // 使用多态
 * Shape shape = new Circle(5.0);
 * shape.area();  // 调用 Circle 的 area()
 * 
 * 
 * 对比 Kotlin:
 * 
 * abstract class Shape(protected val name: String) {
 *     abstract fun area(): Double
 *     abstract fun perimeter(): Double
 *     
 *     open fun display() {
 *         println("形状: $name")
 *     }
 * }
 * 
 * class Circle(private val radius: Double) : Shape("圆形") {
 *     override fun area() = Math.PI * radius * radius
 *     override fun perimeter() = 2 * Math.PI * radius
 * }
 * 
 * 
 * C++ 多态要点：
 * 1. virtual 关键字启用多态
 * 2. 纯虚函数 (= 0) 定义抽象类
 * 3. 虚析构函数确保正确释放内存
 * 4. override 关键字明确重写（C++11）
 * 5. 基类指针/引用指向派生类对象
 * 6. 运行时动态绑定（晚绑定）
 * 
 * 
 * 为什么需要虚析构函数：
 * 
 * Shape* shape = new Circle(5.0);
 * delete shape;  // 如果 Shape 没有虚析构函数，只会调用 Shape 的析构函数
 *                // 导致 Circle 的资源无法释放（内存泄漏）
 * 
 * 规则：如果类有虚函数，就应该有虚析构函数！
 */
