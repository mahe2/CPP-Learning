/*
 * LogParser.h
 * 日志文件解析器类定义
 */

#pragma once

#include "LogEntry.h"
#include <vector>
#include <string>
#include <regex>
#include <fstream>
#include <memory>

namespace LogAnalyzer {

    /**
     * 日志解析器类
     * 负责解析各种格式的日志文件
     */
    class LogParser {
    private:
        // 预定义的日志格式正则表达式
        static const std::vector<std::regex> LOG_PATTERNS;
        
        // 自定义的日志格式模式
        std::vector<std::regex> customPatterns_;
        
        // 解析统计信息
        size_t totalLines_;
        size_t parsedLines_;
        size_t errorLines_;
        
        /**
         * 尝试用给定的正则表达式解析日志行
         * @param line 日志行内容
         * @param pattern 正则表达式模式
         * @return 解析成功的 LogEntry 智能指针，失败返回 nullptr
         */
        std::unique_ptr<LogEntry> tryParseWithPattern(const std::string& line, 
                                                     const std::regex& pattern) const;
        
        /**
         * 解析时间戳字符串
         * @param timestampStr 时间戳字符串
         * @return 转换后的时间点
         */
        std::chrono::system_clock::time_point parseTimestamp(const std::string& timestampStr) const;

    public:
        /**
         * 默认构造函数
         */
        LogParser();
        
        /**
         * 禁用拷贝构造函数和赋值操作符
         */
        LogParser(const LogParser&) = delete;
        LogParser& operator=(const LogParser&) = delete;
        
        /**
         * 移动构造函数和移动赋值操作符
         */
        LogParser(LogParser&&) = default;
        LogParser& operator=(LogParser&&) = default;
        
        /**
         * 析构函数
         */
        ~LogParser() = default;

        /**
         * 添加自定义日志格式模式
         * @param pattern 正则表达式模式字符串
         * @return 添加是否成功
         */
        bool addCustomPattern(const std::string& pattern);
        
        /**
         * 解析单行日志
         * @param line 日志行内容
         * @return 解析成功的 LogEntry 智能指针，失败返回 nullptr
         */
        std::unique_ptr<LogEntry> parseLine(const std::string& line);
        
        /**
         * 解析整个日志文件
         * @param filename 日志文件名
         * @return 包含所有解析成功的日志条目的向量
         * @throws std::runtime_error 如果文件无法打开
         */
        std::vector<LogEntry> parseFile(const std::string& filename);
        
        /**
         * 解析多个日志文件
         * @param filenames 日志文件名列表
         * @return 包含所有解析成功的日志条目的向量
         */
        std::vector<LogEntry> parseFiles(const std::vector<std::string>& filenames);
        
        /**
         * 解析输入流中的日志内容
         * @param input 输入流引用
         * @return 包含所有解析成功的日志条目的向量
         */
        std::vector<LogEntry> parseStream(std::istream& input);
        
        // 统计信息访问器
        size_t getTotalLines() const { return totalLines_; }
        size_t getParsedLines() const { return parsedLines_; }
        size_t getErrorLines() const { return errorLines_; }
        double getParseSuccessRate() const;
        
        /**
         * 重置解析统计信息
         */
        void resetStats();
        
        /**
         * 获取解析统计信息摘要
         * @return 统计信息字符串
         */
        std::string getStatsReport() const;
    };

    /**
     * 工具函数：检测日志文件的可能格式
     * @param filename 日志文件名
     * @return 可能的格式描述
     */
    std::string detectLogFormat(const std::string& filename);
    
    /**
     * 工具函数：验证日志文件是否可读
     * @param filename 日志文件名
     * @return 文件是否可读
     */
    bool isLogFileReadable(const std::string& filename);

} // namespace LogAnalyzer