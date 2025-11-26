/**
 * C++ 存储类（Storage Class）详解
 * 
 * 核心问题：
 * 1. C++ 存储类是否是为多文件配合设计的？
 *    答：部分是！尤其是 extern 和 static
 * 
 * 2. Java 是否有类似概念？
 *    答：有，但方式完全不同！
 */

#include <iostream>
#include <string>

using namespace std;

// ============================================
// 1. C++ 的存储类概念
// ============================================

void explain_storage_classes() {
    cout << "\n" << string(70, '=') << endl;
    cout << "1. C++ 的存储类（Storage Class）" << endl;
    cout << string(70, '=') << endl;
    
    cout << R"(
C++ 有 4 种存储类说明符：

1. auto（C++11 前）
   - 局部变量的默认存储类
   - C++11 后变成类型推导关键字
   - 现在几乎不用于存储类

2. register
   - 建议编译器将变量存储在寄存器
   - 现代编译器自动优化，已过时
   - C++17 弃用

3. static ★★★★★（重要！用于多文件）
   - 局部静态：函数内保持值
   - 全局静态：限制在当前文件
   - 类静态成员：所有对象共享

4. extern ★★★★★（重要！用于多文件）
   - 声明变量/函数在其他文件定义
   - 多文件共享全局变量的关键
   - C++ 多文件协作的核心机制


关键理解：
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
✅ static 和 extern 确实是为多文件设计的！
✅ 它们控制变量/函数的可见性和链接性
✅ 这是 C++ 没有包（Package）机制的补偿
✅ Java 用访问修饰符达到类似目的

)" << endl;
}

// ============================================
// 2. static - 文件内私有
// ============================================

// 全局静态变量：只在当前文件可见
static int filePrivateCounter = 0;
static const string FILE_NAME = "storage_class_demo.cpp";

// 全局静态函数：只在当前文件可见
static void filePrivateFunction() {
    cout << "这个函数只能在当前文件内调用" << endl;
}

void demonstrate_static() {
    cout << "\n" << string(70, '=') << endl;
    cout << "2. static - 文件内私有（多文件隔离）" << endl;
    cout << string(70, '=') << endl;
    
    cout << "\n全局静态变量：" << endl;
    cout << "  static int filePrivateCounter = 0;" << endl;
    cout << "  特点：只在当前文件可见，其他文件无法访问" << endl;
    cout << "  当前值: " << filePrivateCounter << endl;
    
    filePrivateFunction();
    
    cout << "\n作用：" << endl;
    cout << "  ✅ 避免命名冲突（不同文件可以有同名静态变量）" << endl;
    cout << "  ✅ 隐藏实现细节（类似 private）" << endl;
    cout << "  ✅ 控制链接性（internal linkage）" << endl;
    
    cout << "\n多文件场景：" << endl;
    cout << "  // file1.cpp" << endl;
    cout << "  static int counter = 0;  // 只在 file1.cpp 可见" << endl;
    cout << "" << endl;
    cout << "  // file2.cpp" << endl;
    cout << "  static int counter = 0;  // 不冲突！这是不同的变量" << endl;
    
    // 局部静态变量
    cout << "\n局部静态变量示例：" << endl;
    auto countCalls = []() {
        static int callCount = 0;  // 只初始化一次
        callCount++;
        cout << "  函数被调用 " << callCount << " 次" << endl;
    };
    
    countCalls();
    countCalls();
    countCalls();
}

// ============================================
// 3. extern - 多文件共享
// ============================================

// 声明：变量在其他文件定义
extern int sharedCounter;  // 声明（不分配内存）
extern void externalFunction();  // 声明

void demonstrate_extern() {
    cout << "\n" << string(70, '=') << endl;
    cout << "3. extern - 多文件共享" << endl;
    cout << string(70, '=') << endl;
    
    cout << "\nextern 的作用：" << endl;
    cout << "  ✅ 声明变量/函数在其他文件定义" << endl;
    cout << "  ✅ 让多个文件共享同一个全局变量" << endl;
    cout << "  ✅ 控制链接性（external linkage）" << endl;
    
    cout << "\n典型用法：" << endl;
    cout << "  // globals.h（头文件）" << endl;
    cout << "  extern int globalCounter;  // 声明" << endl;
    cout << "  extern void initGlobals(); // 声明" << endl;
    cout << "" << endl;
    cout << "  // globals.cpp（源文件）" << endl;
    cout << "  int globalCounter = 0;     // 定义（分配内存）" << endl;
    cout << "  void initGlobals() { ... } // 定义" << endl;
    cout << "" << endl;
    cout << "  // main.cpp" << endl;
    cout << "  #include \"globals.h\"" << endl;
    cout << "  int main() {" << endl;
    cout << "      globalCounter++;       // 使用" << endl;
    cout << "      initGlobals();         // 使用" << endl;
    cout << "  }" << endl;
    
    cout << "\n关键理解：" << endl;
    cout << "  声明（Declaration）：告诉编译器变量存在" << endl;
    cout << "  定义（Definition）：  分配内存，只能有一次" << endl;
    cout << "  extern：            只声明，不定义" << endl;
}

// ============================================
// 4. static vs extern 对比
// ============================================

void compare_static_extern() {
    cout << "\n" << string(70, '=') << endl;
    cout << "4. static vs extern 对比" << endl;
    cout << string(70, '=') << endl;
    
    cout << "\n特性对比：" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "特性           static              extern" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "可见性         当前文件            所有文件" << endl;
    cout << "链接性         内部链接            外部链接" << endl;
    cout << "用途           隐藏实现            共享数据" << endl;
    cout << "命名冲突       不会冲突            可能冲突" << endl;
    cout << "类比           private             public" << endl;
    cout << "---------------------------------------------------------------" << endl;
    
    cout << "\n使用场景：" << endl;
    cout << "\nstatic（文件私有）：" << endl;
    cout << "  ✅ 工具函数（不想被其他文件调用）" << endl;
    cout << "  ✅ 文件内部常量" << endl;
    cout << "  ✅ 避免命名冲突" << endl;
    cout << "  ✅ 隐藏实现细节" << endl;
    
    cout << "\nextern（文件共享）：" << endl;
    cout << "  ✅ 全局配置变量" << endl;
    cout << "  ✅ 跨文件共享数据" << endl;
    cout << "  ✅ 公共 API 函数" << endl;
    cout << "  ✅ 需要在多处访问的资源" << endl;
}

// ============================================
// 5. 多文件协作示例
// ============================================

void demonstrate_multi_file_collaboration() {
    cout << "\n" << string(70, '=') << endl;
    cout << "5. 多文件协作示例" << endl;
    cout << string(70, '=') << endl;
    
    cout << "\n【示例：日志系统】" << endl;
    cout << "\n// logger.h（头文件）" << endl;
    cout << "-----------------------------" << endl;
    cout << "#ifndef LOGGER_H" << endl;
    cout << "#define LOGGER_H" << endl;
    cout << "" << endl;
    cout << "// 共享变量（声明）" << endl;
    cout << "extern int logLevel;        // 其他文件可访问" << endl;
    cout << "extern const char* logFile;" << endl;
    cout << "" << endl;
    cout << "// 公共函数（声明）" << endl;
    cout << "extern void initLogger();" << endl;
    cout << "extern void log(const char* msg);" << endl;
    cout << "" << endl;
    cout << "#endif" << endl;
    
    cout << "\n// logger.cpp（源文件）" << endl;
    cout << "-----------------------------" << endl;
    cout << "#include \"logger.h\"" << endl;
    cout << "#include <iostream>" << endl;
    cout << "" << endl;
    cout << "// 共享变量（定义）" << endl;
    cout << "int logLevel = 1;           // 分配内存" << endl;
    cout << "const char* logFile = \"app.log\";" << endl;
    cout << "" << endl;
    cout << "// 私有变量（只在本文件可见）" << endl;
    cout << "static int logCount = 0;    // static = 文件私有" << endl;
    cout << "" << endl;
    cout << "// 私有函数（只在本文件可见）" << endl;
    cout << "static void writeToFile(const char* msg) {" << endl;
    cout << "    // 内部实现，不暴露给其他文件" << endl;
    cout << "}" << endl;
    cout << "" << endl;
    cout << "// 公共函数（定义）" << endl;
    cout << "void initLogger() {" << endl;
    cout << "    logCount = 0;" << endl;
    cout << "}" << endl;
    cout << "" << endl;
    cout << "void log(const char* msg) {" << endl;
    cout << "    logCount++;" << endl;
    cout << "    writeToFile(msg);  // 调用私有函数" << endl;
    cout << "}" << endl;
    
    cout << "\n// main.cpp（使用）" << endl;
    cout << "-----------------------------" << endl;
    cout << "#include \"logger.h\"" << endl;
    cout << "" << endl;
    cout << "int main() {" << endl;
    cout << "    logLevel = 2;        // 访问共享变量" << endl;
    cout << "    initLogger();        // 调用公共函数" << endl;
    cout << "    log(\"Hello\");        // 调用公共函数" << endl;
    cout << "    " << endl;
    cout << "    // logCount++;       // ❌ 错误！静态变量不可见" << endl;
    cout << "    // writeToFile(...); // ❌ 错误！静态函数不可见" << endl;
    cout << "    return 0;" << endl;
    cout << "}" << endl;
    
    cout << "\n关键点：" << endl;
    cout << "  ✅ extern 变量/函数：跨文件共享（public）" << endl;
    cout << "  ✅ static 变量/函数：文件内私有（private）" << endl;
    cout << "  ✅ 通过这种方式实现封装和模块化" << endl;
}

// ============================================
// 6. 与 Java 的对比
// ============================================

void compare_with_java() {
    cout << "\n" << string(70, '=') << endl;
    cout << "6. C++ 存储类 vs Java 访问修饰符" << endl;
    cout << string(70, '=') << endl;
    
    cout << "\n【Java 的方式】" << endl;
    cout << "-----------------------------" << endl;
    cout << "Java 用访问修饰符控制可见性：" << endl;
    cout << "" << endl;
    cout << "// Logger.java" << endl;
    cout << "package com.example;" << endl;
    cout << "" << endl;
    cout << "public class Logger {" << endl;
    cout << "    // public：所有类可访问（类似 extern）" << endl;
    cout << "    public static int logLevel = 1;" << endl;
    cout << "    " << endl;
    cout << "    // private：只在本类可访问（类似 static）" << endl;
    cout << "    private static int logCount = 0;" << endl;
    cout << "    " << endl;
    cout << "    // public 方法：其他类可调用" << endl;
    cout << "    public static void log(String msg) {" << endl;
    cout << "        logCount++;" << endl;
    cout << "        writeToFile(msg);" << endl;
    cout << "    }" << endl;
    cout << "    " << endl;
    cout << "    // private 方法：只在本类可调用" << endl;
    cout << "    private static void writeToFile(String msg) {" << endl;
    cout << "        // ..." << endl;
    cout << "    }" << endl;
    cout << "}" << endl;
    cout << "" << endl;
    cout << "// Main.java" << endl;
    cout << "import com.example.Logger;" << endl;
    cout << "" << endl;
    cout << "public class Main {" << endl;
    cout << "    public static void main(String[] args) {" << endl;
    cout << "        Logger.logLevel = 2;  // 访问 public 变量" << endl;
    cout << "        Logger.log(\"Hello\");   // 调用 public 方法" << endl;
    cout << "        " << endl;
    cout << "        // Logger.logCount++;     // ❌ private 不可访问" << endl;
    cout << "        // Logger.writeToFile(); // ❌ private 不可访问" << endl;
    cout << "    }" << endl;
    cout << "}" << endl;
    
    cout << "\n【对比总结】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "概念           C++                    Java" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "组织单位       文件                   类" << endl;
    cout << "公共访问       extern + 头文件        public" << endl;
    cout << "私有访问       static                 private" << endl;
    cout << "包级访问       匿名命名空间           (default)" << endl;
    cout << "模块机制       多文件 + 链接器        包(Package)" << endl;
    cout << "---------------------------------------------------------------" << endl;
    
    cout << "\n相似之处：" << endl;
    cout << "  ✅ 都控制可见性和访问权限" << endl;
    cout << "  ✅ 都支持公共和私有概念" << endl;
    cout << "  ✅ 都实现封装和模块化" << endl;
    
    cout << "\n差异之处：" << endl;
    cout << "  C++：基于文件的访问控制" << endl;
    cout << "  Java：基于类的访问控制" << endl;
    cout << "" << endl;
    cout << "  C++：static = 文件私有" << endl;
    cout << "  Java：static = 类级别（不是文件级）" << endl;
}

// ============================================
// 7. 现代 C++ 的改进
// ============================================

void modern_cpp_improvements() {
    cout << "\n" << string(70, '=') << endl;
    cout << "7. 现代 C++ 的改进" << endl;
    cout << string(70, '=') << endl;
    
    cout << "\n传统 C++ 的问题：" << endl;
    cout << "  ❌ static 会导致每个文件有自己的副本" << endl;
    cout << "  ❌ 头文件保护繁琐（#ifndef）" << endl;
    cout << "  ❌ extern 容易出错（声明和定义不一致）" << endl;
    
    cout << "\n现代 C++ 的解决方案：" << endl;
    cout << "\n1. inline 变量（C++17）" << endl;
    cout << "   // header.h" << endl;
    cout << "   inline int globalVar = 0;  // 可以在头文件定义！" << endl;
    cout << "   inline const string name = \"app\";" << endl;
    cout << "   " << endl;
    cout << "   特点：" << endl;
    cout << "   ✅ 可以在头文件定义变量" << endl;
    cout << "   ✅ 只有一个实例（不是每个文件一份）" << endl;
    cout << "   ✅ 替代 extern 的现代方式" << endl;
    
    cout << "\n2. 匿名命名空间（替代 static）" << endl;
    cout << "   // file.cpp" << endl;
    cout << "   namespace {  // 匿名命名空间" << endl;
    cout << "       int filePrivate = 0;  // 只在本文件可见" << endl;
    cout << "       void helper() { }     // 只在本文件可见" << endl;
    cout << "   }" << endl;
    cout << "   " << endl;
    cout << "   特点：" << endl;
    cout << "   ✅ 现代 C++ 推荐（替代 static）" << endl;
    cout << "   ✅ 更清晰表达意图" << endl;
    cout << "   ✅ 支持类型定义" << endl;
    
    cout << "\n3. constexpr（编译时常量）" << endl;
    cout << "   constexpr int MAX_SIZE = 100;  // 编译时确定" << endl;
    cout << "   constexpr double PI = 3.14159;" << endl;
    cout << "   " << endl;
    cout << "   特点：" << endl;
    cout << "   ✅ 编译时求值" << endl;
    cout << "   ✅ 性能更好" << endl;
    cout << "   ✅ 类型安全" << endl;
    
    cout << "\n4. 模块（C++20）" << endl;
    cout << "   // logger.cppm（模块文件）" << endl;
    cout << "   export module logger;  // 定义模块" << endl;
    cout << "   " << endl;
    cout << "   export int logLevel = 1;  // 导出" << endl;
    cout << "   int internal = 0;         // 不导出（私有）" << endl;
    cout << "   " << endl;
    cout << "   export void log(const char* msg) { }" << endl;
    cout << "   " << endl;
    cout << "   // main.cpp" << endl;
    cout << "   import logger;  // 导入模块" << endl;
    cout << "   " << endl;
    cout << "   int main() {" << endl;
    cout << "       logLevel = 2;  // 使用导出的变量" << endl;
    cout << "       log(\"Hello\");" << endl;
    cout << "   }" << endl;
    cout << "   " << endl;
    cout << "   特点：" << endl;
    cout << "   ✅ 替代头文件的现代方式" << endl;
    cout << "   ✅ 更快的编译速度" << endl;
    cout << "   ✅ 更好的封装性" << endl;
    cout << "   ✅ 类似 Java 的 import 机制" << endl;
}

// ============================================
// Main
// ============================================

int main() {
    cout << "======================================================================" << endl;
    cout << "  C++ 存储类与多文件协作" << endl;
    cout << "======================================================================" << endl;
    
    explain_storage_classes();
    demonstrate_static();
    demonstrate_extern();
    compare_static_extern();
    demonstrate_multi_file_collaboration();
    compare_with_java();
    modern_cpp_improvements();
    
    cout << "\n======================================================================" << endl;
    cout << "核心总结" << endl;
    cout << "======================================================================" << endl;
    
    cout << "\n【问题1：C++ 存储类是否是为多文件配合设计的？】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "答：部分是！尤其是 static 和 extern" << endl;
    cout << "\nstatic：文件内私有（internal linkage）" << endl;
    cout << "  - 限制变量/函数在当前文件可见" << endl;
    cout << "  - 避免命名冲突" << endl;
    cout << "  - 隐藏实现细节" << endl;
    cout << "  - 类似 Java 的 private（但作用于文件级别）" << endl;
    
    cout << "\nextern：多文件共享（external linkage）" << endl;
    cout << "  - 声明变量/函数在其他文件定义" << endl;
    cout << "  - 让多个文件共享数据" << endl;
    cout << "  - C++ 多文件协作的核心" << endl;
    cout << "  - 类似 Java 的 public（但作用于文件级别）" << endl;
    
    cout << "\n【问题2：Java 是否有类似概念？】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "答：有，但方式不同！" << endl;
    cout << "\nJava 用访问修饰符：" << endl;
    cout << "  - public：   所有类可访问（类似 extern）" << endl;
    cout << "  - private：  只在本类可访问（类似 static）" << endl;
    cout << "  - protected：子类可访问" << endl;
    cout << "  - (default)：包内可访问" << endl;
    cout << "\n但组织单位不同：" << endl;
    cout << "  C++：基于文件" << endl;
    cout << "  Java：基于类和包" << endl;
    
    cout << "\n【关键对比】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "C++ 多文件协作：" << endl;
    cout << "  1. 头文件声明（.h）：extern 变量/函数" << endl;
    cout << "  2. 源文件定义（.cpp）：实际实现" << endl;
    cout << "  3. static：文件内私有" << endl;
    cout << "  4. 链接器：将多个文件链接成可执行文件" << endl;
    
    cout << "\nJava 多类协作：" << endl;
    cout << "  1. 包（Package）：组织类" << endl;
    cout << "  2. import：引入其他类" << endl;
    cout << "  3. 访问修饰符：控制可见性" << endl;
    cout << "  4. JVM：类加载器动态加载" << endl;
    
    cout << "\n【现代 C++ 改进】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "  ✅ inline 变量（C++17）：替代 extern" << endl;
    cout << "  ✅ 匿名命名空间：替代 static" << endl;
    cout << "  ✅ constexpr：编译时常量" << endl;
    cout << "  ✅ 模块（C++20）：类似 Java import" << endl;
    
    cout << "\n【记忆要点】" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "static 文件私有，extern 多文件共享" << endl;
    cout << "C++ 基于文件，Java 基于类包" << endl;
    cout << "存储类确实为多文件设计" << endl;
    cout << "Java 访问符达到类似目的" << endl;
    cout << "现代 C++ 有更好替代方案" << endl;
    cout << "======================================================================" << endl;
    
    return 0;
}

/**
 * 补充说明：
 * 
 * 1. 链接性（Linkage）：
 *    - Internal linkage（内部链接）：static，只在当前文件
 *    - External linkage（外部链接）：extern，跨文件可见
 *    - No linkage（无链接）：局部变量
 * 
 * 2. 最佳实践：
 *    - 现代 C++：用匿名命名空间替代 static
 *    - 现代 C++：用 inline 变量替代 extern
 *    - 未来：用模块（C++20）替代头文件
 * 
 * 3. 常见错误：
 *    - 在头文件定义非 inline 变量（导致重复定义）
 *    - extern 声明和定义类型不一致
 *    - 忘记在某个文件定义 extern 变量
 * 
 * 4. 与 Java 的根本差异：
 *    - C++：编译时链接，基于文件
 *    - Java：运行时加载，基于类
 *    - C++：需要头文件
 *    - Java：不需要声明文件
 */
