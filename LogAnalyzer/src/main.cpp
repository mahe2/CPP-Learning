/*
 * main.cpp
 * LogAnalyzer 主程序入口
 */

#include "LogEntry.h"
#include "LogParser.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iomanip>

using namespace LogAnalyzer;

/**
 * 显示使用帮助信息
 */
void showHelp(const std::string& programName) {
    std::cout << "LogAnalyzer - C++ 日志分析工具\n"
              << "用法: " << programName << " [选项] <日志文件...>\n\n"
              << "选项:\n"
              << "  -h, --help          显示此帮助信息\n"
              << "  -s, --stats         显示统计信息\n"
              << "  -l, --level <级别>  过滤指定级别的日志 (DEBUG|INFO|WARN|ERROR|FATAL)\n"
              << "  -f, --format        检测日志文件格式\n"
              << "  -c, --count         统计各级别日志数量\n"
              << "  -r, --recent <N>    显示最近的 N 条日志\n"
              << "  -p, --pattern <正则> 添加自定义解析模式\n\n"
              << "示例:\n"
              << "  " << programName << " app.log\n"
              << "  " << programName << " --stats --count app.log\n"
              << "  " << programName << " --level ERROR error.log\n"
              << std::endl;
}

/**
 * 统计各级别日志数量
 */
void showLevelStatistics(const std::vector<LogEntry>& entries) {
    std::map<LogLevel, size_t> levelCounts;
    
    for (const auto& entry : entries) {
        levelCounts[entry.getLevel()]++;
    }
    
    std::cout << "\n=== 日志级别统计 ===\n";
    for (const auto& pair : levelCounts) {
        std::cout << std::left << std::setw(8) << logLevelToString(pair.first) 
                  << ": " << pair.second << " 条\n";
    }
    std::cout << "总计: " << entries.size() << " 条\n";
}

/**
 * 根据级别过滤日志
 */
std::vector<LogEntry> filterByLevel(const std::vector<LogEntry>& entries, LogLevel level) {
    std::vector<LogEntry> filtered;
    std::copy_if(entries.begin(), entries.end(), std::back_inserter(filtered),
                 [level](const LogEntry& entry) {
                     return entry.getLevel() == level;
                 });
    return filtered;
}

/**
 * 显示最近的 N 条日志
 */
void showRecentLogs(const std::vector<LogEntry>& entries, size_t count) {
    size_t startIndex = entries.size() > count ? entries.size() - count : 0;
    
    std::cout << "\n=== 最近 " << (entries.size() - startIndex) << " 条日志 ===\n";
    for (size_t i = startIndex; i < entries.size(); ++i) {
        std::cout << entries[i] << "\n";
    }
}

/**
 * 显示所有日志条目
 */
void showAllLogs(const std::vector<LogEntry>& entries) {
    std::cout << "\n=== 所有日志条目 ===\n";
    for (const auto& entry : entries) {
        std::cout << entry << "\n";
    }
}

/**
 * 检测并显示文件格式信息
 */
void showFormatInfo(const std::vector<std::string>& filenames) {
    std::cout << "\n=== 日志文件格式检测 ===\n";
    for (const auto& filename : filenames) {
        std::cout << "文件: " << filename << "\n";
        std::cout << "格式: " << detectLogFormat(filename) << "\n";
        std::cout << "可读: " << (isLogFileReadable(filename) ? "是" : "否") << "\n\n";
    }
}

/**
 * 主函数
 */
int main(int argc, char* argv[]) {
    if (argc < 2) {
        showHelp(argv[0]);
        return 1;
    }
    
    // 解析命令行参数
    std::vector<std::string> filenames;
    bool showStats = false;
    bool showCount = false;
    bool showFormat = false;
    LogLevel filterLevel = LogLevel::INFO;
    bool hasLevelFilter = false;
    size_t recentCount = 0;
    std::vector<std::string> customPatterns;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            showHelp(argv[0]);
            return 0;
        } else if (arg == "-s" || arg == "--stats") {
            showStats = true;
        } else if (arg == "-c" || arg == "--count") {
            showCount = true;
        } else if (arg == "-f" || arg == "--format") {
            showFormat = true;
        } else if (arg == "-l" || arg == "--level") {
            if (i + 1 < argc) {
                std::string levelStr = argv[++i];
                std::transform(levelStr.begin(), levelStr.end(), levelStr.begin(), ::toupper);
                filterLevel = stringToLogLevel(levelStr);
                hasLevelFilter = true;
            } else {
                std::cerr << "错误: --level 需要一个参数\n";
                return 1;
            }
        } else if (arg == "-r" || arg == "--recent") {
            if (i + 1 < argc) {
                try {
                    recentCount = std::stoul(argv[++i]);
                } catch (const std::exception&) {
                    std::cerr << "错误: --recent 需要一个有效的数字参数\n";
                    return 1;
                }
            } else {
                std::cerr << "错误: --recent 需要一个参数\n";
                return 1;
            }
        } else if (arg == "-p" || arg == "--pattern") {
            if (i + 1 < argc) {
                customPatterns.push_back(argv[++i]);
            } else {
                std::cerr << "错误: --pattern 需要一个参数\n";
                return 1;
            }
        } else if (arg[0] != '-') {
            filenames.push_back(arg);
        } else {
            std::cerr << "错误: 未知选项 " << arg << "\n";
            showHelp(argv[0]);
            return 1;
        }
    }
    
    if (filenames.empty()) {
        std::cerr << "错误: 请提供至少一个日志文件\n";
        showHelp(argv[0]);
        return 1;
    }
    
    // 如果只需要格式检测，直接执行并返回
    if (showFormat) {
        showFormatInfo(filenames);
        return 0;
    }
    
    try {
        // 创建解析器并添加自定义模式
        LogParser parser;
        for (const auto& pattern : customPatterns) {
            if (!parser.addCustomPattern(pattern)) {
                std::cerr << "警告: 添加自定义模式失败: " << pattern << "\n";
            }
        }
        
        std::cout << "正在解析日志文件...\n";
        
        // 解析所有文件
        auto entries = parser.parseFiles(filenames);
        
        if (entries.empty()) {
            std::cout << "未找到有效的日志条目\n";
            if (showStats) {
                std::cout << "\n" << parser.getStatsReport() << "\n";
            }
            return 0;
        }
        
        std::cout << "成功解析 " << entries.size() << " 条日志条目\n";
        
        // 应用级别过滤
        if (hasLevelFilter) {
            entries = filterByLevel(entries, filterLevel);
            std::cout << "级别过滤后剩余 " << entries.size() << " 条日志条目\n";
        }
        
        // 显示统计信息
        if (showStats) {
            std::cout << "\n" << parser.getStatsReport() << "\n";
        }
        
        // 显示级别统计
        if (showCount) {
            showLevelStatistics(entries);
        }
        
        // 显示日志条目
        if (recentCount > 0) {
            showRecentLogs(entries, recentCount);
        } else if (!showStats && !showCount) {
            // 如果没有指定其他显示选项，显示所有日志
            showAllLogs(entries);
        }
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}