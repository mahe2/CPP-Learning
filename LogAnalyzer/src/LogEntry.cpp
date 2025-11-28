/*
 * LogEntry.cpp
 * LogEntry 类的实现
 */

#include "LogEntry.h"
#include <sstream>
#include <iomanip>
#include <map>

namespace LogAnalyzer {

    // 构造函数实现
    LogEntry::LogEntry(const std::chrono::system_clock::time_point& timestamp,
                      LogLevel level, 
                      const std::string& source, 
                      const std::string& message)
        : timestamp_(timestamp), level_(level), source_(source), message_(message) {
    }

    // 访问器实现
    const std::chrono::system_clock::time_point& LogEntry::getTimestamp() const {
        return timestamp_;
    }

    LogLevel LogEntry::getLevel() const {
        return level_;
    }

    const std::string& LogEntry::getSource() const {
        return source_;
    }

    const std::string& LogEntry::getMessage() const {
        return message_;
    }

    // 修改器实现
    void LogEntry::setTimestamp(const std::chrono::system_clock::time_point& timestamp) {
        timestamp_ = timestamp;
    }

    void LogEntry::setLevel(LogLevel level) {
        level_ = level;
    }

    void LogEntry::setSource(const std::string& source) {
        source_ = source;
    }

    void LogEntry::setMessage(const std::string& message) {
        message_ = message;
    }

    // 工具方法实现
    std::string LogEntry::getLevelString() const {
        return logLevelToString(level_);
    }

    std::string LogEntry::getFormattedTimestamp() const {
        auto time_t = std::chrono::system_clock::to_time_t(timestamp_);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    std::string LogEntry::toString() const {
        std::stringstream ss;
        ss << "[" << getFormattedTimestamp() << "] "
           << "[" << getLevelString() << "] "
           << "[" << source_ << "] "
           << message_;
        return ss.str();
    }

    // 比较运算符实现
    bool LogEntry::operator<(const LogEntry& other) const {
        return timestamp_ < other.timestamp_;
    }

    bool LogEntry::operator==(const LogEntry& other) const {
        return timestamp_ == other.timestamp_ &&
               level_ == other.level_ &&
               source_ == other.source_ &&
               message_ == other.message_;
    }

    // 全局函数实现
    std::string logLevelToString(LogLevel level) {
        static const std::map<LogLevel, std::string> levelMap = {
            {LogLevel::DEBUG, "DEBUG"},
            {LogLevel::INFO,  "INFO"},
            {LogLevel::WARN,  "WARN"},
            {LogLevel::ERROR, "ERROR"},
            {LogLevel::FATAL, "FATAL"}
        };
        
        auto it = levelMap.find(level);
        return (it != levelMap.end()) ? it->second : "UNKNOWN";
    }

    LogLevel stringToLogLevel(const std::string& levelStr) {
        static const std::map<std::string, LogLevel> stringMap = {
            {"DEBUG", LogLevel::DEBUG},
            {"INFO",  LogLevel::INFO},
            {"WARN",  LogLevel::WARN},
            {"WARNING", LogLevel::WARN},  // 别名
            {"ERROR", LogLevel::ERROR},
            {"FATAL", LogLevel::FATAL}
        };
        
        auto it = stringMap.find(levelStr);
        return (it != stringMap.end()) ? it->second : LogLevel::INFO;
    }

    // 输出流操作符实现
    std::ostream& operator<<(std::ostream& os, const LogEntry& entry) {
        os << entry.toString();
        return os;
    }

    std::ostream& operator<<(std::ostream& os, LogLevel level) {
        os << logLevelToString(level);
        return os;
    }

} // namespace LogAnalyzer