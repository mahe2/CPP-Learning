/*
 * LogParser.cpp
 * 日志文件解析器类实现
 */

#include "LogParser.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace LogAnalyzer {

    // 静态成员初始化：预定义的日志格式模式
    const std::vector<std::regex> LogParser::LOG_PATTERNS = {
        // Apache 通用日志格式
        std::regex(R"((\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2})\s+\[(\w+)\]\s+\[([^\]]+)\]\s+(.+))"),
        
        // Syslog 格式  
        std::regex(R"((\w{3} \d{1,2} \d{2}:\d{2}:\d{2})\s+(\w+)\s+([^:]+):\s+(.+))"),
        
        // Java 应用日志格式
        std::regex(R"((\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2},\d{3})\s+(\w+)\s+\[([^\]]+)\]\s+(.+))"),
        
        // 简单格式：时间戳 + 级别 + 消息
        std::regex(R"((\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2})\s+(\w+):\s+(.+))"),
        
        // 带毫秒的格式
        std::regex(R"((\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}\.\d{3}Z?)\s+\[(\w+)\]\s+\[([^\]]+)\]\s+(.+))")
    };

    // 构造函数
    LogParser::LogParser() 
        : totalLines_(0), parsedLines_(0), errorLines_(0) {
    }

    // 添加自定义模式
    bool LogParser::addCustomPattern(const std::string& pattern) {
        try {
            std::regex customRegex(pattern);
            customPatterns_.push_back(std::move(customRegex));
            return true;
        } catch (const std::regex_error& e) {
            std::cerr << "错误：无效的正则表达式模式: " << e.what() << std::endl;
            return false;
        }
    }

    // 尝试用给定模式解析日志行
    std::unique_ptr<LogEntry> LogParser::tryParseWithPattern(const std::string& line, 
                                                           const std::regex& pattern) const {
        std::smatch matches;
        if (std::regex_match(line, matches, pattern)) {
            try {
                // 根据匹配组数量判断日志格式
                if (matches.size() >= 4) {
                    std::string timestampStr = matches[1].str();
                    std::string levelStr = matches[2].str();
                    std::string sourceStr = matches.size() >= 5 ? matches[3].str() : "unknown";
                    std::string messageStr = matches.size() >= 5 ? matches[4].str() : matches[3].str();
                    
                    auto timestamp = parseTimestamp(timestampStr);
                    auto level = stringToLogLevel(levelStr);
                    
                    return std::make_unique<LogEntry>(timestamp, level, sourceStr, messageStr);
                }
            } catch (const std::exception& e) {
                std::cerr << "解析日志条目时发生错误: " << e.what() << std::endl;
            }
        }
        return nullptr;
    }

    // 解析时间戳
    std::chrono::system_clock::time_point LogParser::parseTimestamp(const std::string& timestampStr) const {
        std::istringstream ss(timestampStr);
        std::tm tm = {};
        
        // 尝试不同的时间格式
        std::vector<std::string> formats = {
            "%Y-%m-%d %H:%M:%S",           // 2024-01-15 14:30:45
            "%Y-%m-%dT%H:%M:%S",           // 2024-01-15T14:30:45
            "%Y-%m-%d %H:%M:%S,%f",        // 2024-01-15 14:30:45,123
            "%Y-%m-%dT%H:%M:%S.%f",        // 2024-01-15T14:30:45.123
            "%b %d %H:%M:%S"               // Jan 15 14:30:45
        };
        
        for (const auto& format : formats) {
            ss.clear();
            ss.str(timestampStr);
            ss >> std::get_time(&tm, format.c_str());
            if (!ss.fail()) {
                break;
            }
        }
        
        // 如果解析失败，使用当前时间
        if (ss.fail()) {
            return std::chrono::system_clock::now();
        }
        
        auto time_t_value = std::mktime(&tm);
        return std::chrono::system_clock::from_time_t(time_t_value);
    }

    // 解析单行日志
    std::unique_ptr<LogEntry> LogParser::parseLine(const std::string& line) {
        totalLines_++;
        
        // 跳过空行
        if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) {
            return nullptr;
        }
        
        // 首先尝试自定义模式
        for (const auto& pattern : customPatterns_) {
            auto entry = tryParseWithPattern(line, pattern);
            if (entry) {
                parsedLines_++;
                return entry;
            }
        }
        
        // 然后尝试预定义模式
        for (const auto& pattern : LOG_PATTERNS) {
            auto entry = tryParseWithPattern(line, pattern);
            if (entry) {
                parsedLines_++;
                return entry;
            }
        }
        
        errorLines_++;
        return nullptr;
    }

    // 解析文件
    std::vector<LogEntry> LogParser::parseFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("无法打开文件: " + filename);
        }
        
        return parseStream(file);
    }

    // 解析多个文件
    std::vector<LogEntry> LogParser::parseFiles(const std::vector<std::string>& filenames) {
        std::vector<LogEntry> allEntries;
        
        for (const auto& filename : filenames) {
            try {
                auto fileEntries = parseFile(filename);
                allEntries.insert(allEntries.end(), 
                                std::make_move_iterator(fileEntries.begin()),
                                std::make_move_iterator(fileEntries.end()));
            } catch (const std::exception& e) {
                std::cerr << "解析文件 " << filename << " 时发生错误: " << e.what() << std::endl;
            }
        }
        
        // 按时间戳排序
        std::sort(allEntries.begin(), allEntries.end());
        
        return allEntries;
    }

    // 解析输入流
    std::vector<LogEntry> LogParser::parseStream(std::istream& input) {
        std::vector<LogEntry> entries;
        std::string line;
        
        while (std::getline(input, line)) {
            auto entry = parseLine(line);
            if (entry) {
                entries.emplace_back(std::move(*entry));
            }
        }
        
        return entries;
    }

    // 获取解析成功率
    double LogParser::getParseSuccessRate() const {
        if (totalLines_ == 0) return 0.0;
        return static_cast<double>(parsedLines_) / totalLines_ * 100.0;
    }

    // 重置统计信息
    void LogParser::resetStats() {
        totalLines_ = 0;
        parsedLines_ = 0;
        errorLines_ = 0;
    }

    // 获取统计报告
    std::string LogParser::getStatsReport() const {
        std::ostringstream oss;
        oss << "解析统计信息:\n"
            << "  总行数: " << totalLines_ << "\n"
            << "  成功解析: " << parsedLines_ << "\n"
            << "  解析失败: " << errorLines_ << "\n"
            << "  成功率: " << std::fixed << std::setprecision(2) 
            << getParseSuccessRate() << "%";
        return oss.str();
    }

    // 工具函数实现
    std::string detectLogFormat(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return "文件无法读取";
        }
        
        std::string line;
        std::vector<std::string> sampleLines;
        
        // 读取前几行作为样本
        for (int i = 0; i < 10 && std::getline(file, line); ++i) {
            if (!line.empty()) {
                sampleLines.push_back(line);
            }
        }
        
        if (sampleLines.empty()) {
            return "文件为空或无有效内容";
        }
        
        // 简单的格式检测逻辑
        const std::string& firstLine = sampleLines[0];
        
        if (firstLine.find("GMT") != std::string::npos || 
            firstLine.find("UTC") != std::string::npos) {
            return "Web服务器日志格式";
        } else if (firstLine.find("[") != std::string::npos && 
                  firstLine.find("]") != std::string::npos) {
            return "应用程序日志格式";
        } else if (std::regex_search(firstLine, std::regex(R"(\d{4}-\d{2}-\d{2})"))) {
            return "标准时间戳格式";
        } else {
            return "未知格式";
        }
    }

    bool isLogFileReadable(const std::string& filename) {
        std::ifstream file(filename);
        return file.is_open() && file.good();
    }

} // namespace LogAnalyzer