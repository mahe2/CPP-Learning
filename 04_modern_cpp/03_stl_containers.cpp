/*
 * STL 容器 - 标准模板库
 * 
 * 类似 Java Collections 和 Kotlin Collections
 */

#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <queue>
#include <algorithm>

using namespace std;

int main() {
    // ========== vector - 动态数组 ==========
    cout << "=== vector (类似 ArrayList) ===" << endl;
    
    vector<int> vec = {1, 2, 3, 4, 5};
    vec.push_back(6);        // 添加元素
    vec.pop_back();          // 删除最后一个
    
    cout << "大小: " << vec.size() << endl;
    cout << "容量: " << vec.capacity() << endl;
    cout << "第一个: " << vec.front() << ", 最后一个: " << vec.back() << endl;
    
    vec.insert(vec.begin() + 2, 99);  // 在位置2插入
    vec.erase(vec.begin() + 1);       // 删除位置1的元素
    
    cout << "元素: ";
    for (int n : vec) cout << n << " ";
    cout << endl;
    
    // ========== list - 双向链表 ==========
    cout << "\n=== list (类似 LinkedList) ===" << endl;
    
    list<string> names = {"Alice", "Bob", "Charlie"};
    names.push_back("David");      // 尾部添加
    names.push_front("Zero");      // 头部添加
    
    cout << "链表: ";
    for (const string& name : names) cout << name << " ";
    cout << endl;
    
    // ========== deque - 双端队列 ==========
    cout << "\n=== deque (双端队列) ===" << endl;
    
    deque<int> dq = {1, 2, 3};
    dq.push_front(0);   // 头部插入
    dq.push_back(4);    // 尾部插入
    
    cout << "deque: ";
    for (int n : dq) cout << n << " ";
    cout << endl;
    
    // ========== set - 有序集合（不重复）==========
    cout << "\n=== set (类似 TreeSet) ===" << endl;
    
    set<int> numSet = {5, 2, 8, 2, 1, 9};  // 自动去重和排序
    numSet.insert(3);
    numSet.insert(2);  // 重复，不会插入
    
    cout << "set: ";
    for (int n : numSet) cout << n << " ";  // 输出有序
    cout << endl;
    
    // 查找
    if (numSet.find(5) != numSet.end()) {
        cout << "找到 5" << endl;
    }
    
    // ========== map - 有序映射 ==========
    cout << "\n=== map (类似 TreeMap) ===" << endl;
    
    map<string, int> ages;
    ages["Alice"] = 25;
    ages["Bob"] = 30;
    ages["Charlie"] = 28;
    
    cout << "Alice 的年龄: " << ages["Alice"] << endl;
    
    // 遍历
    cout << "所有人员: " << endl;
    for (const auto& [name, age] : ages) {  // C++17 结构化绑定
        cout << "  " << name << ": " << age << " 岁" << endl;
    }
    
    // ========== unordered_map - 哈希表 ==========
    cout << "\n=== unordered_map (类似 HashMap) ===" << endl;
    
    unordered_map<string, string> phoneBook;
    phoneBook["Alice"] = "123-4567";
    phoneBook["Bob"] = "234-5678";
    phoneBook["Charlie"] = "345-6789";
    
    cout << "Bob 的电话: " << phoneBook["Bob"] << endl;
    
    // 检查键是否存在
    if (phoneBook.count("Alice") > 0) {
        cout << "Alice 在电话簿中" << endl;
    }
    
    // ========== unordered_set - 哈希集合 ==========
    cout << "\n=== unordered_set (类似 HashSet) ===" << endl;
    
    unordered_set<string> uniqueNames = {"Tom", "Jerry", "Tom", "Mike"};
    uniqueNames.insert("Lucy");
    
    cout << "唯一名字: ";
    for (const string& name : uniqueNames) cout << name << " ";
    cout << endl;
    
    // ========== stack - 栈 ==========
    cout << "\n=== stack (栈) ===" << endl;
    
    stack<int> stk;
    stk.push(1);
    stk.push(2);
    stk.push(3);
    
    cout << "栈顶: " << stk.top() << endl;
    stk.pop();
    cout << "弹出后栈顶: " << stk.top() << endl;
    
    // ========== queue - 队列 ==========
    cout << "\n=== queue (队列) ===" << endl;
    
    queue<string> q;
    q.push("First");
    q.push("Second");
    q.push("Third");
    
    cout << "队首: " << q.front() << endl;
    q.pop();
    cout << "出队后队首: " << q.front() << endl;
    
    // ========== priority_queue - 优先队列 ==========
    cout << "\n=== priority_queue (优先队列) ===" << endl;
    
    priority_queue<int> pq;  // 默认大顶堆
    pq.push(5);
    pq.push(2);
    pq.push(8);
    pq.push(1);
    
    cout << "优先队列（大顶堆）: ";
    while (!pq.empty()) {
        cout << pq.top() << " ";
        pq.pop();
    }
    cout << endl;
    
    // 小顶堆
    priority_queue<int, vector<int>, greater<int>> minHeap;
    minHeap.push(5);
    minHeap.push(2);
    minHeap.push(8);
    minHeap.push(1);
    
    cout << "小顶堆: ";
    while (!minHeap.empty()) {
        cout << minHeap.top() << " ";
        minHeap.pop();
    }
    cout << endl;
    
    // ========== 常用算法 ==========
    cout << "\n=== STL 算法 ===" << endl;
    
    vector<int> data = {3, 1, 4, 1, 5, 9, 2, 6};
    
    // 排序
    sort(data.begin(), data.end());
    cout << "排序后: ";
    for (int n : data) cout << n << " ";
    cout << endl;
    
    // 反转
    reverse(data.begin(), data.end());
    cout << "反转后: ";
    for (int n : data) cout << n << " ";
    cout << endl;
    
    // 查找
    auto it = find(data.begin(), data.end(), 5);
    if (it != data.end()) {
        cout << "找到 5，位置: " << distance(data.begin(), it) << endl;
    }
    
    // 最大值和最小值
    cout << "最大值: " << *max_element(data.begin(), data.end()) << endl;
    cout << "最小值: " << *min_element(data.begin(), data.end()) << endl;
    
    // 求和
    int sum = accumulate(data.begin(), data.end(), 0);
    cout << "总和: " << sum << endl;
    
    // 去重（需要先排序）
    sort(data.begin(), data.end());
    auto last = unique(data.begin(), data.end());
    data.erase(last, data.end());
    cout << "去重后: ";
    for (int n : data) cout << n << " ";
    cout << endl;
    
    return 0;
}

/*
 * STL 容器对比：
 * 
 * | C++ 容器 | Java 对应 | Kotlin 对应 | 特点 |
 * |----------|-----------|-------------|------|
 * | vector | ArrayList | MutableList | 动态数组 |
 * | list | LinkedList | LinkedList | 双向链表 |
 * | deque | ArrayDeque | ArrayDeque | 双端队列 |
 * | set | TreeSet | TreeSet | 有序集合 |
 * | map | TreeMap | TreeMap | 有序映射 |
 * | unordered_set | HashSet | HashSet | 哈希集合 |
 * | unordered_map | HashMap | HashMap | 哈希映射 |
 * | stack | Stack | Stack | 栈 |
 * | queue | Queue | Queue | 队列 |
 * | priority_queue | PriorityQueue | PriorityQueue | 优先队列 |
 * 
 * 
 * 选择容器的原则：
 * 1. 需要随机访问？用 vector
 * 2. 频繁插入/删除？用 list
 * 3. 需要排序？用 set/map
 * 4. 需要快速查找？用 unordered_set/unordered_map
 * 5. 栈操作？用 stack
 * 6. 队列操作？用 queue
 * 7. 需要优先级？用 priority_queue
 * 
 * 
 * 时间复杂度：
 * 
 * vector:
 *   - 随机访问: O(1)
 *   - 尾部插入/删除: O(1)
 *   - 中间插入/删除: O(n)
 * 
 * list:
 *   - 随机访问: O(n)
 *   - 插入/删除: O(1)
 * 
 * set/map:
 *   - 查找/插入/删除: O(log n)
 * 
 * unordered_set/unordered_map:
 *   - 查找/插入/删除: 平均 O(1)
 * 
 * 
 * 迭代器：
 * begin()  - 指向第一个元素
 * end()    - 指向最后一个元素之后
 * rbegin() - 反向迭代器开始
 * rend()   - 反向迭代器结束
 * 
 * 
 * 常用算法（<algorithm>）：
 * sort, reverse, find, count, max_element, min_element,
 * accumulate, unique, copy, transform, for_each
 */
