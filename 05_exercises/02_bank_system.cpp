/*
 * 练习2：简单银行账户系统
 * 
 * 要求：
 * 1. 创建 Account 基类
 * 2. 派生 SavingsAccount（储蓄账户）和 CheckingAccount（支票账户）
 * 3. 实现存款、取款、转账功能
 * 4. 使用多态管理不同类型账户
 */

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <iomanip>

using namespace std;

// 基类：账户
class Account {
protected:
    string accountNumber;
    string ownerName;
    double balance;
    
public:
    Account(string accNum, string owner, double initialBalance)
        : accountNumber(accNum), ownerName(owner), balance(initialBalance) {
        cout << "账户创建: " << accountNumber << endl;
    }
    
    virtual ~Account() {
        cout << "账户销毁: " << accountNumber << endl;
    }
    
    // 存款
    virtual void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "存款成功！当前余额: " << balance << endl;
        } else {
            cout << "存款金额必须大于0" << endl;
        }
    }
    
    // 取款（纯虚函数，子类必须实现）
    virtual bool withdraw(double amount) = 0;
    
    // 查询余额
    double getBalance() const { return balance; }
    
    // 获取账号
    string getAccountNumber() const { return accountNumber; }
    
    // 获取户主
    string getOwnerName() const { return ownerName; }
    
    // 显示账户信息（虚函数，可被重写）
    virtual void displayInfo() const {
        cout << "账号: " << accountNumber 
             << ", 户主: " << ownerName 
             << ", 余额: " << fixed << setprecision(2) << balance << endl;
    }
    
    // 获取账户类型
    virtual string getAccountType() const = 0;
};

// 储蓄账户
class SavingsAccount : public Account {
private:
    double interestRate;  // 利率

public:
    SavingsAccount(string accNum, string owner, double initialBalance, double rate)
        : Account(accNum, owner, initialBalance), interestRate(rate) {}
    
    ~SavingsAccount() {
        cout << "储蓄账户销毁: " << accountNumber << endl;
    }
    
    bool withdraw(double amount) override {
        if (amount <= 0) {
            cout << "取款金额必须大于0" << endl;
            return false;
        }
        
        // 储蓄账户必须保留最低余额100
        if (balance - amount < 100) {
            cout << "余额不足！储蓄账户最低余额为100" << endl;
            return false;
        }
        
        balance -= amount;
        cout << "取款成功！当前余额: " << balance << endl;
        return true;
    }
    
    // 计算利息
    void addInterest() {
        double interest = balance * interestRate;
        balance += interest;
        cout << "利息: " << interest << ", 新余额: " << balance << endl;
    }
    
    void displayInfo() const override {
        Account::displayInfo();
        cout << "  类型: 储蓄账户, 利率: " << (interestRate * 100) << "%" << endl;
    }
    
    string getAccountType() const override {
        return "储蓄账户";
    }
};

// 支票账户
class CheckingAccount : public Account {
private:
    double overdraftLimit;  // 透支限额

public:
    CheckingAccount(string accNum, string owner, double initialBalance, double limit)
        : Account(accNum, owner, initialBalance), overdraftLimit(limit) {}
    
    ~CheckingAccount() {
        cout << "支票账户销毁: " << accountNumber << endl;
    }
    
    bool withdraw(double amount) override {
        if (amount <= 0) {
            cout << "取款金额必须大于0" << endl;
            return false;
        }
        
        // 支票账户可以透支
        if (balance - amount < -overdraftLimit) {
            cout << "超出透支限额！当前透支限额: " << overdraftLimit << endl;
            return false;
        }
        
        balance -= amount;
        cout << "取款成功！当前余额: " << balance << endl;
        return true;
    }
    
    void displayInfo() const override {
        Account::displayInfo();
        cout << "  类型: 支票账户, 透支限额: " << overdraftLimit << endl;
    }
    
    string getAccountType() const override {
        return "支票账户";
    }
};

// 银行系统
class BankSystem {
private:
    vector<shared_ptr<Account>> accounts;

public:
    // 创建储蓄账户
    void createSavingsAccount(string accNum, string owner, double balance, double rate) {
        auto account = make_shared<SavingsAccount>(accNum, owner, balance, rate);
        accounts.push_back(account);
        cout << "储蓄账户创建成功！" << endl;
    }
    
    // 创建支票账户
    void createCheckingAccount(string accNum, string owner, double balance, double limit) {
        auto account = make_shared<CheckingAccount>(accNum, owner, balance, limit);
        accounts.push_back(account);
        cout << "支票账户创建成功！" << endl;
    }
    
    // 查找账户
    shared_ptr<Account> findAccount(const string& accNum) {
        for (auto& account : accounts) {
            if (account->getAccountNumber() == accNum) {
                return account;
            }
        }
        return nullptr;
    }
    
    // 转账
    void transfer(const string& fromAcc, const string& toAcc, double amount) {
        auto from = findAccount(fromAcc);
        auto to = findAccount(toAcc);
        
        if (!from) {
            cout << "转出账户不存在" << endl;
            return;
        }
        if (!to) {
            cout << "转入账户不存在" << endl;
            return;
        }
        
        cout << "\n转账: " << fromAcc << " -> " << toAcc << ", 金额: " << amount << endl;
        
        if (from->withdraw(amount)) {
            to->deposit(amount);
            cout << "转账成功！" << endl;
        } else {
            cout << "转账失败！" << endl;
        }
    }
    
    // 显示所有账户
    void displayAllAccounts() const {
        cout << "\n=== 所有账户 ===" << endl;
        for (const auto& account : accounts) {
            account->displayInfo();
            cout << endl;
        }
        cout << "总账户数: " << accounts.size() << endl;
    }
    
    // 计算总资产
    double calculateTotalAssets() const {
        double total = 0;
        for (const auto& account : accounts) {
            total += account->getBalance();
        }
        return total;
    }
};

int main() {
    BankSystem bank;
    
    cout << "=== 银行账户系统示例 ===" << endl;
    
    // 创建账户
    bank.createSavingsAccount("SA001", "张三", 5000, 0.03);
    bank.createSavingsAccount("SA002", "李四", 10000, 0.03);
    bank.createCheckingAccount("CA001", "王五", 2000, 1000);
    bank.createCheckingAccount("CA002", "赵六", 3000, 2000);
    
    // 显示所有账户
    bank.displayAllAccounts();
    
    // 存款
    cout << "\n=== 存款操作 ===" << endl;
    auto account1 = bank.findAccount("SA001");
    if (account1) {
        account1->deposit(1000);
    }
    
    // 取款
    cout << "\n=== 取款操作 ===" << endl;
    auto account2 = bank.findAccount("CA001");
    if (account2) {
        account2->withdraw(500);
    }
    
    // 转账
    cout << "\n=== 转账操作 ===" << endl;
    bank.transfer("SA001", "CA001", 2000);
    
    // 显示更新后的账户
    bank.displayAllAccounts();
    
    // 总资产
    cout << "\n总资产: " << fixed << setprecision(2) 
         << bank.calculateTotalAssets() << endl;
    
    return 0;
}

/*
 * 练习要点：
 * 
 * 1. 继承和多态的使用
 * 2. 抽象类和纯虚函数
 * 3. 虚析构函数的重要性
 * 4. shared_ptr 管理多态对象
 * 5. override 关键字
 * 6. 业务逻辑实现
 * 
 * 扩展练习：
 * 1. 添加交易历史记录
 * 2. 实现定期存款账户
 * 3. 添加手续费计算
 * 4. 实现账户冻结功能
 * 5. 添加密码验证
 * 6. 实现文件持久化
 */
