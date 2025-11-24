/*
 * Lambda 表达式 - 匿名函数
 * 
 * C++11 引入 Lambda，类似 Java Lambda 和 Kotlin Lambda
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

int main() {
    // ========== 基本语法 ==========
    cout << "=== Lambda 基本语法 ===" << endl;
    
    // 最简单的 Lambda
    auto hello = []() {
        cout << "Hello, Lambda!" << endl;
    };
    hello();
    
    // 带参数的 Lambda
    auto add = [](int a, int b) {
        return a + b;
    };
    cout << "5 + 3 = " << add(5, 3) << endl;
    
    // 指定返回类型
    auto divide = [](double a, double b) -> double {
        return a / b;
    };
    cout << "10 / 3 = " << divide(10, 3) << endl;
    
    // ========== 捕获外部变量 ==========
    cout << "\n=== 捕获外部变量 ===" << endl;
    
    int x = 10;
    int y = 20;
    
    // 按值捕获
    auto byValue = [x, y]() {
        cout << "按值捕获: x=" << x << ", y=" << y << endl;
        // x = 100;  // ❌ 错误！按值捕获是只读的
    };
    byValue();
    
    // 按引用捕获
    auto byReference = [&x, &y]() {
        x = 100;  // ✅ 可以修改
        y = 200;
        cout << "按引用捕获: x=" << x << ", y=" << y << endl;
    };
    byReference();
    cout << "修改后: x=" << x << ", y=" << y << endl;
    
    // 捕获所有外部变量
    int a = 1, b = 2, c = 3;
    
    auto captureAll = [=]() {  // = 按值捕获所有
        cout << "捕获所有(值): a=" << a << ", b=" << b << ", c=" << c << endl;
    };
    captureAll();
    
    auto captureAllRef = [&]() {  // & 按引用捕获所有
        a = 10;
        b = 20;
        c = 30;
        cout << "捕获所有(引用): a=" << a << ", b=" << b << ", c=" << c << endl;
    };
    captureAllRef();
    
    // 混合捕获
    int m = 100, n = 200;
    auto mixed = [=, &n]() {  // m 按值，n 按引用
        cout << "m=" << m << ", n=" << n << endl;
        n = 999;
    };
    mixed();
    cout << "n 修改为: " << n << endl;
    
    // ========== 在算法中使用 Lambda ==========
    cout << "\n=== Lambda 与 STL 算法 ===" << endl;
    
    vector<int> numbers = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    
    // 1. 排序（升序）
    sort(numbers.begin(), numbers.end(), [](int a, int b) {
        return a < b;
    });
    
    cout << "升序排序: ";
    for (int n : numbers) cout << n << " ";
    cout << endl;
    
    // 2. 排序（降序）
    sort(numbers.begin(), numbers.end(), [](int a, int b) {
        return a > b;
    });
    
    cout << "降序排序: ";
    for (int n : numbers) cout << n << " ";
    cout << endl;
    
    // 3. 过滤（找出大于5的数）
    vector<int> filtered;
    copy_if(numbers.begin(), numbers.end(), back_inserter(filtered),
        [](int n) { return n > 5; }
    );
    
    cout << "大于5的数: ";
    for (int n : filtered) cout << n << " ";
    cout << endl;
    
    // 4. 计数
    int count = count_if(numbers.begin(), numbers.end(),
        [](int n) { return n % 2 == 0; }
    );
    cout << "偶数个数: " << count << endl;
    
    // 5. 遍历
    cout << "遍历输出: ";
    for_each(numbers.begin(), numbers.end(),
        [](int n) { cout << n * 2 << " "; }
    );
    cout << endl;
    
    // ========== Lambda 作为函数参数 ==========
    cout << "\n=== Lambda 作为参数 ===" << endl;
    
    auto applyOperation = [](int x, int y, function<int(int, int)> op) {
        return op(x, y);
    };
    
    int result1 = applyOperation(10, 5, [](int a, int b) { return a + b; });
    int result2 = applyOperation(10, 5, [](int a, int b) { return a - b; });
    int result3 = applyOperation(10, 5, [](int a, int b) { return a * b; });
    
    cout << "10 + 5 = " << result1 << endl;
    cout << "10 - 5 = " << result2 << endl;
    cout << "10 × 5 = " << result3 << endl;
    
    // ========== 返回 Lambda ==========
    cout << "\n=== 返回 Lambda ===" << endl;
    
    auto makeMultiplier = [](int factor) {
        return [factor](int n) { return n * factor; };
    };
    
    auto times2 = makeMultiplier(2);
    auto times10 = makeMultiplier(10);
    
    cout << "5 × 2 = " << times2(5) << endl;
    cout << "5 × 10 = " << times10(5) << endl;
    
    // ========== mutable Lambda ==========
    cout << "\n=== mutable Lambda ===" << endl;
    
    int counter = 0;
    auto increment = [counter]() mutable {  // mutable 允许修改按值捕获的变量
        counter++;
        return counter;
    };
    
    cout << "调用1: " << increment() << endl;
    cout << "调用2: " << increment() << endl;
    cout << "调用3: " << increment() << endl;
    cout << "原始 counter: " << counter << endl;  // 不变
    
    return 0;
}

/*
 * Lambda 语法详解：
 * 
 * [捕获列表](参数列表) mutable -> 返回类型 { 函数体 }
 * 
 * 捕获列表：
 * []          - 不捕获任何变量
 * [=]         - 按值捕获所有外部变量
 * [&]         - 按引用捕获所有外部变量
 * [x]         - 按值捕获 x
 * [&x]        - 按引用捕获 x
 * [x, &y]     - x 按值，y 按引用
 * [=, &x]     - 所有按值，x 按引用
 * [&, x]      - 所有按引用，x 按值
 * [this]      - 捕获当前对象指针
 * 
 * 
 * 对比 Java:
 * 
 * // Java 8 Lambda
 * Runnable r = () -> System.out.println("Hello");
 * 
 * BiFunction<Integer, Integer, Integer> add = (a, b) -> a + b;
 * 
 * list.stream()
 *     .filter(n -> n > 5)
 *     .forEach(n -> System.out.println(n));
 * 
 * 
 * 对比 Kotlin:
 * 
 * // Kotlin Lambda
 * val hello = { println("Hello") }
 * 
 * val add = { a: Int, b: Int -> a + b }
 * 
 * list.filter { it > 5 }
 *     .forEach { println(it) }
 * 
 * 
 * C++ Lambda 特点：
 * 1. 可以捕获外部变量（Java/Kotlin 也可以）
 * 2. 可以选择按值或按引用捕获
 * 3. mutable 关键字允许修改按值捕获的变量
 * 4. 可以指定返回类型
 * 5. 编译时生成函数对象（零开销抽象）
 * 
 * 
 * 使用场景：
 * 1. STL 算法（sort, find, count_if 等）
 * 2. 回调函数
 * 3. 自定义排序规则
 * 4. 事件处理
 * 5. 线程函数
 */
