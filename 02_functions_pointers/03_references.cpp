/*
 * 引用 (Reference)
 * 
 * 引用是 C++ 的特性，类似指针但更安全
 * 可以理解为变量的别名
 */

#include <iostream>
#include <string>

using namespace std;

// ========== 按值传递 ==========
void incrementByValue(int x) {
    x = x + 1;  // 只修改副本，不影响原变量
    cout << "函数内 x = " << x << endl;
}

// ========== 按引用传递 ==========
void incrementByReference(int& x) {  // & 表示引用
    x = x + 1;  // 直接修改原变量
    cout << "函数内 x = " << x << endl;
}

// ========== 按指针传递 ==========
void incrementByPointer(int* x) {
    *x = *x + 1;  // 通过指针修改原变量
    cout << "函数内 *x = " << *x << endl;
}

// ========== 引用作为返回值 ==========
int& getElement(int arr[], int index) {
    return arr[index];  // 返回数组元素的引用
}

// ========== 常量引用（只读）==========
void printName(const string& name) {  // const& 避免复制，且不能修改
    cout << "姓名: " << name << endl;
    // name += "test";  // 错误！不能修改 const 引用
}

int main() {
    // ========== 引用基础 ==========
    cout << "=== 引用基础 ===" << endl;
    int original = 10;
    int& ref = original;  // ref 是 original 的引用（别名）
    
    cout << "original = " << original << endl;
    cout << "ref = " << ref << endl;
    
    ref = 20;  // 通过引用修改值
    cout << "修改 ref 后, original = " << original << endl;  // original 也变为 20
    
    // ========== 按值传递 vs 按引用传递 ==========
    cout << "\n=== 按值传递 ===" << endl;
    int num1 = 5;
    cout << "调用前 num1 = " << num1 << endl;
    incrementByValue(num1);
    cout << "调用后 num1 = " << num1 << endl;  // 不变，仍为 5
    
    cout << "\n=== 按引用传递 ===" << endl;
    int num2 = 5;
    cout << "调用前 num2 = " << num2 << endl;
    incrementByReference(num2);
    cout << "调用后 num2 = " << num2 << endl;  // 变为 6
    
    cout << "\n=== 按指针传递 ===" << endl;
    int num3 = 5;
    cout << "调用前 num3 = " << num3 << endl;
    incrementByPointer(&num3);  // 传递地址
    cout << "调用后 num3 = " << num3 << endl;  // 变为 6
    
    // ========== 引用作为返回值 ==========
    cout << "\n=== 引用作为返回值 ===" << endl;
    int numbers[] = {10, 20, 30, 40, 50};
    
    cout << "修改前: ";
    for (int n : numbers) cout << n << " ";
    cout << endl;
    
    getElement(numbers, 2) = 99;  // 直接修改数组元素
    
    cout << "修改后: ";
    for (int n : numbers) cout << n << " ";
    cout << endl;
    
    // ========== 常量引用 ==========
    cout << "\n=== 常量引用 ===" << endl;
    string myName = "Alice";
    printName(myName);  // 高效传递，避免复制大对象
    
    // ========== 引用 vs 指针 ==========
    cout << "\n=== 引用 vs 指针 ===" << endl;
    int value = 100;
    
    // 引用
    int& valueRef = value;
    valueRef = 200;
    cout << "使用引用: value = " << value << endl;
    
    // 指针
    int* valuePtr = &value;
    *valuePtr = 300;
    cout << "使用指针: value = " << value << endl;
    
    return 0;
}

/*
 * 对比 Java/Kotlin:
 * 
 * === Java ===
 * // Java 对象自动按引用传递
 * void modifyObject(Person person) {
 *     person.setName("New Name");  // 会修改原对象
 * }
 * 
 * // 基本类型按值传递
 * void increment(int x) {
 *     x++;  // 不影响原变量
 * }
 * 
 * === Kotlin ===
 * // Kotlin 没有显式的按引用传递
 * fun increment(x: Int) {
 *     // 不能修改参数
 * }
 * 
 * === C++ ===
 * // 可以选择按值、按引用或按指针传递
 * void byValue(int x);        // 按值传递
 * void byRef(int& x);         // 按引用传递
 * void byPointer(int* x);     // 按指针传递
 * void readOnly(const int& x); // 常量引用（只读）
 * 
 * 
 * 何时使用引用：
 * 1. 需要修改函数外部变量时
 * 2. 传递大对象，避免复制（性能优化）
 * 3. 返回容器元素，允许直接修改
 * 
 * 引用 vs 指针：
 * 1. 引用必须初始化，指针可以为 nullptr
 * 2. 引用不能重新绑定，指针可以指向不同地址
 * 3. 引用更安全，指针更灵活
 * 4. 引用语法更简洁（不需要 * 和 &）
 */
