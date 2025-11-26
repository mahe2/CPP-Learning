/*
 * 数组 - 固定大小的数据集合
 * 
 * C++ 数组与 Java 数组类似，但有一些重要区别
 */

#include <iostream>
#include <array>   // C++11 标准数组
#include <vector>  // 动态数组（推荐使用）

using namespace std;

int main() {
    // ========== 传统 C 风格数组 ==========
    cout << "=== 传统数组 ===" << endl;
    
    // 声明并初始化
    int numbers[5] = {10, 20, 30, 40, 50};
    
    // 访问元素（下标从0开始，与Java相同）
    cout << "第一个元素: " << numbers[0] << endl;
    cout << "第三个元素: " << numbers[2] << endl;
    
    // 修改元素
    numbers[1] = 25;
    cout << "修改后第二个元素: " << numbers[1] << endl;
    
    // 遍历数组
    cout << "所有元素: ";
    for (int i = 0; i < 5; i++) {
        cout << numbers[i] << " ";
    }
    cout << endl;
    
    // C++11 范围for循环
    cout << "使用范围for: ";
    for (int num : numbers) {
        cout << num << " ";
    }
    cout << endl;
    
    // ========== 多维数组 ==========
    cout << "\n=== 二维数组 ===" << endl;
    
    int matrix[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    
    cout << "矩阵:\n";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
    
    // ========== C++11 标准数组 std::array ==========
    cout << "\n=== std::array (推荐) ===" << endl;
    
    array<int, 5> scores = {85, 90, 78, 92, 88};
    
    cout << "数组大小: " << scores.size() << endl;
    cout << "第一个分数: " << scores.front() << endl;
    cout << "最后一个分数: " << scores.back() << endl;
    
    // at() 方法会进行边界检查（更安全）
    cout << "第三个分数: " << scores.at(2) << endl;
    
    // ========== std::vector 动态数组 ==========
    cout << "\n=== std::vector (最推荐) ===" << endl;
    
    // 类似 Java 的 ArrayList 或 Kotlin 的 MutableList
    vector<int> dynamicArray;
    // 添加元素
    dynamicArray.push_back(10);  // 类似 Java list.add()
    dynamicArray.push_back(20);
    dynamicArray.push_back(30);
    
    cout << "Vector 大小: " << dynamicArray.size() << endl;
    
    // 访问元素
    cout << "第一个元素: " << dynamicArray[0] << endl;
    cout << "最后一个元素: " << dynamicArray.back() << endl;
    
    // 遍历 vector
    cout << "所有元素: ";
    for (int val : dynamicArray) {
        cout << val << " ";
    }
    cout << endl;
    
    // 插入元素
    dynamicArray.insert(dynamicArray.begin() + 1, 15);  // 在位置1插入15
    
    // 删除元素
    dynamicArray.pop_back();  // 删除最后一个元素
    
    cout << "修改后: ";
    for (int val : dynamicArray) {
        cout << val << " ";
    }
    cout << endl;
    
    // 清空 vector
    dynamicArray.clear();
    cout << "清空后大小: " << dynamicArray.size() << endl;
    
    // ========== 初始化 vector ==========
    vector<string> names = {"Alice", "Bob", "Charlie"};
    cout << "\n姓名列表: ";
    for (const string& name : names) {  // const& 避免复制，提高性能
        cout << name << " ";
    }
    cout << endl;
    
    return 0;
}

/*
 * 对比 Java/Kotlin:
 * 
 * === Java ===
 * // 数组
 * int[] numbers = {10, 20, 30, 40, 50};
 * 
 * // ArrayList（动态数组）
 * ArrayList<Integer> list = new ArrayList<>();
 * list.add(10);
 * list.add(20);
 * int first = list.get(0);
 * 
 * === Kotlin ===
 * // 数组
 * val numbers = arrayOf(10, 20, 30, 40, 50)
 * 
 * // MutableList（动态列表）
 * val list = mutableListOf<Int>()
 * list.add(10)
 * list.add(20)
 * val first = list[0]
 * 
 * === C++ ===
 * // 传统数组
 * int numbers[5] = {10, 20, 30, 40, 50};
 * 
 * // std::vector（推荐）
 * vector<int> list;
 * list.push_back(10);
 * list.push_back(20);
 * int first = list[0];
 */
