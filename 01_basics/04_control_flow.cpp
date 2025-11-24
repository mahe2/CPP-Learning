/*
 * 控制流程：条件语句和循环
 * 
 * 与 Java/Kotlin 非常相似
 */

#include <iostream>
#include <string>

using namespace std;

int main() {
    // ========== if-else 语句 ==========
    int score = 85;
    
    cout << "=== if-else 示例 ===" << endl;
    if (score >= 90) {
        cout << "成绩: 优秀" << endl;
    } else if (score >= 80) {
        cout << "成绩: 良好" << endl;
    } else if (score >= 60) {
        cout << "成绩: 及格" << endl;
    } else {
        cout << "成绩: 不及格" << endl;
    }
    
    // ========== 三元运算符 ==========
    string result = (score >= 60) ? "通过" : "未通过";
    cout << "考试结果: " << result << endl;
    
    // ========== switch 语句 ==========
    cout << "\n=== switch 示例 ===" << endl;
    int day = 3;
    
    switch (day) {
        case 1:
            cout << "星期一" << endl;
            break;  // break 很重要！否则会继续执行下一个 case
        case 2:
            cout << "星期二" << endl;
            break;
        case 3:
            cout << "星期三" << endl;
            break;
        case 4:
            cout << "星期四" << endl;
            break;
        case 5:
            cout << "星期五" << endl;
            break;
        case 6:
        case 7:
            cout << "周末" << endl;
            break;
        default:
            cout << "无效的日期" << endl;
    }
    
    // ========== for 循环 ==========
    cout << "\n=== for 循环 ===" << endl;
    for (int i = 1; i <= 5; i++) {
        cout << "循环第 " << i << " 次" << endl;
    }
    
    // 基于范围的 for 循环（C++11，类似 Java 的增强 for 循环）
    int numbers[] = {1, 2, 3, 4, 5};
    cout << "\n数组元素: ";
    for (int num : numbers) {  // 类似 Java: for (int num : numbers)
        cout << num << " ";
    }
    cout << endl;
    
    // ========== while 循环 ==========
    cout << "\n=== while 循环 ===" << endl;
    int count = 1;
    while (count <= 3) {
        cout << "Count: " << count << endl;
        count++;
    }
    
    // ========== do-while 循环 ==========
    cout << "\n=== do-while 循环 ===" << endl;
    int num = 1;
    do {
        cout << "Number: " << num << endl;
        num++;
    } while (num <= 3);
    
    // ========== break 和 continue ==========
    cout << "\n=== break 和 continue ===" << endl;
    for (int i = 1; i <= 10; i++) {
        if (i == 3) {
            continue;  // 跳过本次循环
        }
        if (i == 7) {
            break;  // 退出循环
        }
        cout << i << " ";
    }
    cout << endl;
    
    return 0;
}

/*
 * 对比 Java/Kotlin:
 * 
 * === Java ===
 * if (score >= 90) {
 *     System.out.println("优秀");
 * }
 * 
 * for (int i = 0; i < 5; i++) {
 *     System.out.println(i);
 * }
 * 
 * for (int num : numbers) {
 *     System.out.println(num);
 * }
 * 
 * === Kotlin ===
 * if (score >= 90) {
 *     println("优秀")
 * }
 * 
 * for (i in 1..5) {
 *     println(i)
 * }
 * 
 * for (num in numbers) {
 *     println(num)
 * }
 * 
 * when (day) {  // Kotlin 的 switch
 *     1 -> println("星期一")
 *     2 -> println("星期二")
 *     else -> println("其他")
 * }
 */
