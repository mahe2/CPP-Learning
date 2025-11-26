/*
 * 练习1：学生成绩管理系统
 * 
 * 要求：
 * 1. 创建 Student 类，包含姓名、学号、成绩
 * 2. 使用 vector 存储多个学生
 * 3. 实现添加、删除、查询、排序功能
 * 4. 使用智能指针管理学生对象
 * 5. 使用 Lambda 实现自定义排序
 */

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>

using namespace std;

class Student {
    private:
        string name;
        int id;
        double score;
    
    public:
        Student(string name, int id, double score): name(name), id(id), score(score) {
            cout << "创建学生: "<< name << endl;
        }
        ~Student(){
            cout << "销毁学生: "<< name << endl; 
        }

        string getName() const {
            return name;
        }
        int getId() const {
            return id;
        }
        double getScore() const {
            return score;
        }

        void setScore(double s) {
            score = s;
        }

        void display() const {
            cout << "学号: "<< id
                << ", 姓名: "<<name
                <<", 成绩: "<<score<<endl;
        }
};
class StudentManager {
    private:
        vector<shared_ptr<Student>> students;
    
    public:
        void addStudent(const string& name, int id, double score){
            auto student = make_shared<Student>(name, id, score);
            students.push_back(student);
            cout << "添加成功" << endl;
        }

        void removeStudent(int id){
            auto it = find_if(students.begin(), students.end(),
                [id](const shared_ptr<Student>& s){
                    return s->getId() == id;
                }
            );

            if (it != students.end()){
                cout << "删除学生: "<< (*it)->getName() << endl;
                students.erase(it);
            } else {
                cout << "未找到学号为: " << id << "的学生" << endl;
            }
            
        }

        void findStudent(int id) {
            auto it = find_if(students.begin(), students.end(),
                [id](const shared_ptr<Student>& s){
                    return s->getId() == id;
                }
            );
            if (it != students.end()){
                (*it)->display();
            }else{
                cout << "未找到学号为 " << id << " 的学生" << endl;
            }
        }

        void displayAll() const {
            cout << "\n=== 所有学生 ===" << endl;
            for(const auto& student: students){
                student -> display();
            }
            cout << "总人数" << students.size() << endl;
        }

        void sortByScore(bool ascending = true){
            sort(students.begin(), students.end(),
                [ascending](const shared_ptr<Student>& a, const shared_ptr<Student>& b){
                    return ascending ? a->getScore() < b->getScore() : a->getScore() > b->getScore();
                }
            );
            cout << "排序完成！" <<endl;
        }

        void sortByName(){
            sort(students.begin(), students.end(),
                [](const shared_ptr<Student>& a, const shared_ptr<Student>& b){
                    return a->getName() < b->getName();
                }
            );
        }

        double calculateAverage() const {
            if(students.empty()) return 0.0;

            double sum = 0;
            for(const auto& student: students){
                sum += student->getScore();
            }
            return sum / students.size();
        }

        void findTopStudent() const {
            if (students.empty()){
                cout << "没有学生记录" << endl;
                return;
            }

            auto top = max_element(students.begin(), students.end(),
                [](const shared_ptr<Student>& a, const shared_ptr<Student>& b){
                    return a->getScore() < b->getScore();
                }
            );

            cout << "最高学生分: ";
            (*top) -> display();
        }
};

void showMenu() {
    cout << "\n====== 学生成绩管理系统 ======" << endl;
    cout << "1. 添加学生" << endl;
    cout << "2. 删除学生" << endl;
    cout << "3. 查询学生" << endl;
    cout << "4. 显示所有学生" << endl;
    cout << "5. 按成绩排序（升序）" << endl;
    cout << "6. 按成绩排序（降序）" << endl;
    cout << "7. 按姓名排序" << endl;
    cout << "8. 计算平均分" << endl;
    cout << "9. 查找最高分学生" << endl;
    cout << "0. 退出" << endl;
    cout << "=============================" << endl;
    cout << "请选择: ";
}

int main() {
    StudentManager manager;
    
    // 添加一些初始数据
    manager.addStudent("张三", 1001, 85.5);
    manager.addStudent("李四", 1002, 92.0);
    manager.addStudent("王五", 1003, 78.5);
    manager.addStudent("赵六", 1004, 88.0);
    
    int choice;
    while (true) {
        showMenu();
        cin >> choice;
        
        switch (choice) {
            case 1: {
                string name;
                int id;
                double score;
                cout << "输入姓名: ";
                cin >> name;
                cout << "输入学号: ";
                cin >> id;
                cout << "输入成绩: ";
                cin >> score;
                manager.addStudent(name, id, score);
                break;
            }
            case 2: {
                int id;
                cout << "输入要删除的学号: ";
                cin >> id;
                manager.removeStudent(id);
                break;
            }
            case 3: {
                int id;
                cout << "输入要查询的学号: ";
                cin >> id;
                manager.findStudent(id);
                break;
            }
            case 4:
                manager.displayAll();
                break;
            case 5:
                manager.sortByScore(true);
                manager.displayAll();
                break;
            case 6:
                manager.sortByScore(false);
                manager.displayAll();
                break;
            case 7:
                manager.sortByName();
                manager.displayAll();
                break;
            case 8:
                cout << "平均分: " << manager.calculateAverage() << endl;
                break;
            case 9:
                manager.findTopStudent();
                break;
            case 0:
                cout << "退出程序" << endl;
                return 0;
            default:
                cout << "无效选择，请重新输入" << endl;
        }
    }
    
    return 0;
}

/*
 * 练习要点：
 * 
 * 1. 类的设计和封装
 * 2. shared_ptr 管理对象生命周期
 * 3. vector 存储和操作
 * 4. Lambda 表达式在算法中的应用
 * 5. STL 算法：find_if, sort, max_element
 * 6. const 正确性
 * 
 * 扩展练习：
 * 1. 添加文件保存/加载功能
 * 2. 支持多科目成绩
 * 3. 实现成绩统计（及格率、优秀率等）
 * 4. 添加输入验证
 * 5. 使用异常处理
 */
