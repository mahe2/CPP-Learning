/*
 * 练习3：图书馆管理系统
 * 
 * 要求：
 * 1. 创建 Book 类和 Member 类
 * 2. 实现图书借阅、归还功能
 * 3. 使用 map 管理图书和会员
 * 4. 综合运用 STL 容器和算法
 */

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <algorithm>
#include <ctime>

using namespace std;

// 图书类
class Book {
private:
    string isbn;
    string title;
    string author;
    bool isAvailable;
    
public:
    Book(string i, string t, string a) 
        : isbn(i), title(t), author(a), isAvailable(true) {}
    
    // Getters
    string getISBN() const { return isbn; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool available() const { return isAvailable; }
    
    // 借出
    void borrowBook() { isAvailable = false; }
    
    // 归还
    void returnBook() { isAvailable = true; }
    
    void display() const {
        cout << "ISBN: " << isbn 
             << ", 书名: " << title 
             << ", 作者: " << author 
             << ", 状态: " << (isAvailable ? "可借" : "已借出") << endl;
    }
};

// 借阅记录
struct BorrowRecord {
    string isbn;
    string bookTitle;
    time_t borrowDate;
    
    BorrowRecord(string i, string t) : isbn(i), bookTitle(t) {
        borrowDate = time(nullptr);
    }
};

// 会员类
class Member {
private:
    string memberId;
    string name;
    vector<BorrowRecord> borrowedBooks;
    static const int MAX_BORROW = 5;  // 最多借5本书
    
public:
    Member(string id, string n) : memberId(id), name(n) {}
    
    string getMemberId() const { return memberId; }
    string getName() const { return name; }
    int getBorrowedCount() const { return borrowedBooks.size(); }
    
    // 借书
    bool borrowBook(const string& isbn, const string& title) {
        if (borrowedBooks.size() >= MAX_BORROW) {
            cout << "已达到借书上限（" << MAX_BORROW << "本）" << endl;
            return false;
        }
        
        // 检查是否已借过这本书
        auto it = find_if(borrowedBooks.begin(), borrowedBooks.end(),
            [&isbn](const BorrowRecord& record) {
                return record.isbn == isbn;
            });
        
        if (it != borrowedBooks.end()) {
            cout << "您已借阅此书" << endl;
            return false;
        }
        
        borrowedBooks.emplace_back(isbn, title);
        return true;
    }
    
    // 还书
    bool returnBook(const string& isbn) {
        auto it = find_if(borrowedBooks.begin(), borrowedBooks.end(),
            [&isbn](const BorrowRecord& record) {
                return record.isbn == isbn;
            });
        
        if (it == borrowedBooks.end()) {
            cout << "您未借阅此书" << endl;
            return false;
        }
        
        borrowedBooks.erase(it);
        return true;
    }
    
    // 显示借阅记录
    void displayBorrowedBooks() const {
        if (borrowedBooks.empty()) {
            cout << "无借阅记录" << endl;
            return;
        }
        
        cout << "已借图书：" << endl;
        for (const auto& record : borrowedBooks) {
            cout << "  - " << record.bookTitle << " (ISBN: " << record.isbn << ")" << endl;
        }
    }
    
    void display() const {
        cout << "会员号: " << memberId 
             << ", 姓名: " << name 
             << ", 已借: " << borrowedBooks.size() << " 本" << endl;
    }
};

// 图书馆系统
class Library {
private:
    map<string, shared_ptr<Book>> books;        // ISBN -> Book
    map<string, shared_ptr<Member>> members;    // MemberId -> Member
    
public:
    // 添加图书
    void addBook(const string& isbn, const string& title, const string& author) {
        if (books.find(isbn) != books.end()) {
            cout << "图书已存在" << endl;
            return;
        }
        
        books[isbn] = make_shared<Book>(isbn, title, author);
        cout << "图书添加成功：" << title << endl;
    }
    
    // 注册会员
    void registerMember(const string& memberId, const string& name) {
        if (members.find(memberId) != members.end()) {
            cout << "会员已存在" << endl;
            return;
        }
        
        members[memberId] = make_shared<Member>(memberId, name);
        cout << "会员注册成功：" << name << endl;
    }
    
    // 借书
    void borrowBook(const string& memberId, const string& isbn) {
        // 检查会员
        auto memberIt = members.find(memberId);
        if (memberIt == members.end()) {
            cout << "会员不存在" << endl;
            return;
        }
        
        // 检查图书
        auto bookIt = books.find(isbn);
        if (bookIt == books.end()) {
            cout << "图书不存在" << endl;
            return;
        }
        
        auto member = memberIt->second;
        auto book = bookIt->second;
        
        // 检查图书是否可借
        if (!book->available()) {
            cout << "图书已被借出" << endl;
            return;
        }
        
        // 借书
        if (member->borrowBook(isbn, book->getTitle())) {
            book->borrowBook();
            cout << "借书成功！" << endl;
        }
    }
    
    // 还书
    void returnBook(const string& memberId, const string& isbn) {
        auto memberIt = members.find(memberId);
        if (memberIt == members.end()) {
            cout << "会员不存在" << endl;
            return;
        }
        
        auto bookIt = books.find(isbn);
        if (bookIt == books.end()) {
            cout << "图书不存在" << endl;
            return;
        }
        
        auto member = memberIt->second;
        auto book = bookIt->second;
        
        if (member->returnBook(isbn)) {
            book->returnBook();
            cout << "还书成功！" << endl;
        }
    }
    
    // 搜索图书（按书名）
    void searchByTitle(const string& keyword) const {
        cout << "\n搜索结果：" << endl;
        bool found = false;
        
        for (const auto& [isbn, book] : books) {
            if (book->getTitle().find(keyword) != string::npos) {
                book->display();
                found = true;
            }
        }
        
        if (!found) {
            cout << "未找到相关图书" << endl;
        }
    }
    
    // 显示所有图书
    void displayAllBooks() const {
        cout << "\n=== 所有图书 ===" << endl;
        for (const auto& [isbn, book] : books) {
            book->display();
        }
        cout << "总计: " << books.size() << " 本" << endl;
    }
    
    // 显示所有会员
    void displayAllMembers() const {
        cout << "\n=== 所有会员 ===" << endl;
        for (const auto& [id, member] : members) {
            member->display();
        }
        cout << "总计: " << members.size() << " 人" << endl;
    }
    
    // 显示会员借阅信息
    void displayMemberBorrows(const string& memberId) const {
        auto it = members.find(memberId);
        if (it == members.end()) {
            cout << "会员不存在" << endl;
            return;
        }
        
        cout << "\n会员信息：" << endl;
        it->second->display();
        it->second->displayBorrowedBooks();
    }
    
    // 统计信息
    void displayStatistics() const {
        int totalBooks = books.size();
        int availableBooks = count_if(books.begin(), books.end(),
            [](const auto& pair) {
                return pair.second->available();
            });
        
        cout << "\n=== 统计信息 ===" << endl;
        cout << "图书总数: " << totalBooks << endl;
        cout << "可借图书: " << availableBooks << endl;
        cout << "已借图书: " << (totalBooks - availableBooks) << endl;
        cout << "会员总数: " << members.size() << endl;
    }
};

int main() {
    Library library;
    
    cout << "=== 图书馆管理系统 ===" << endl;
    
    // 添加图书
    library.addBook("978-1", "C++ Primer", "Stanley Lippman");
    library.addBook("978-2", "Effective C++", "Scott Meyers");
    library.addBook("978-3", "The C++ Programming Language", "Bjarne Stroustrup");
    library.addBook("978-4", "Modern C++ Design", "Andrei Alexandrescu");
    library.addBook("978-5", "C++ Concurrency in Action", "Anthony Williams");
    
    // 注册会员
    library.registerMember("M001", "张三");
    library.registerMember("M002", "李四");
    library.registerMember("M003", "王五");
    
    // 显示所有图书和会员
    library.displayAllBooks();
    library.displayAllMembers();
    
    // 借书
    cout << "\n=== 借书操作 ===" << endl;
    library.borrowBook("M001", "978-1");
    library.borrowBook("M001", "978-2");
    library.borrowBook("M002", "978-3");
    
    // 显示借阅信息
    library.displayMemberBorrows("M001");
    library.displayMemberBorrows("M002");
    
    // 尝试借已借出的书
    cout << "\n=== 尝试借已借出的书 ===" << endl;
    library.borrowBook("M003", "978-1");
    
    // 还书
    cout << "\n=== 还书操作 ===" << endl;
    library.returnBook("M001", "978-1");
    
    // 搜索图书
    library.searchByTitle("C++");
    
    // 统计信息
    library.displayStatistics();
    
    return 0;
}

/*
 * 练习要点：
 * 
 * 1. 类的设计和关系
 * 2. map 容器的使用
 * 3. vector 和算法（find_if, count_if）
 * 4. 智能指针管理对象
 * 5. const 正确性
 * 6. 业务逻辑实现
 * 
 * 扩展练习：
 * 1. 添加图书分类
 * 2. 实现预约功能
 * 3. 添加逾期罚款计算
 * 4. 实现图书推荐
 * 5. 支持文件保存/加载
 * 6. 添加借阅历史统计
 */
