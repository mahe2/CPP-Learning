/*
 * LogEntry.h
 * 表示单条日志记录的类定义
 */

#pragma once

#include <string>
#include <chrono>
#include <iostream>

namespace LogAnalyzer {

    // 日志级别枚举
    enum class LogLevel {
        DEBUG = 0,
        INFO = 1,
        WARN = 2,
        ERROR = 3,
        FATAL = 4
    };

    // 日志条目类
    class LogEntry {
    private:
        std::chrono::system_clock::time_point timestamp_;
        LogLevel level_;
        std::string source_;
        std::string message_;

    public:
        // 构造函数
        LogEntry() = default;
        LogEntry(const std::chrono::system_clock::time_point& timestamp,
                LogLevel level, 
                const std::string& source, 
                const std::string& message);

        // 访问器
        const std::chrono::system_clock::time_point& getTimestamp() const;
        LogLevel getLevel() const;
        const std::string& getSource() const;
        const std::string& getMessage() const;

        // 修改器
        void setTimestamp(const std::chrono::system_clock::time_point& timestamp);
        void setLevel(LogLevel level);
        void setSource(const std::string& source);
        void setMessage(const std::string& message);

        // 工具方法
        std::string getLevelString() const;
        std::string getFormattedTimestamp() const;
        std::string toString() const;

        // 比较运算符
        bool operator<(const LogEntry& other) const;
        bool operator==(const LogEntry& other) const;
    };

    // 日志级别转换函数
    std::string logLevelToString(LogLevel level);
    LogLevel stringToLogLevel(const std::string& levelStr);

    // 输出流操作符
    std::ostream& operator<<(std::ostream& os, const LogEntry& entry);
    std::ostream& operator<<(std::ostream& os, LogLevel level);

} // namespace LogAnalyzer