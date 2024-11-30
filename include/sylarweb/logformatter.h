#pragma once
#ifndef __LOGFORMATTER_H__
#define __LOGFORMATTER_H__

#include <string_view>
#include <vector>
#include <memory>

namespace Cot {
class LogEvent;

class FormatItemBase;

/**
 * @brief 日志格式器，用于格式化一个日志事件，将其转化成一串字符串
 */
class LogFormatter {
public:
    using Sptr = std::shared_ptr<LogFormatter>;

    /**
     * @brief 构造函数
     * @param[in] pattern 格式模板，参考sylar与log4cpp
     * @details 模板参数说明：
     * - %%m 消息
     * - %%p 日志级别
     * - %%c 日志器名称
     * - %%d 日期时间，后面可跟一对括号指定时间格式，比如%%d{%%Y-%%m-%%d %%H:%%M:%%S}，这里的格式字符与C语言strftime一致
     * - %%r 该日志器创建后的累计运行毫秒数
     * - %%f 文件名
     * - %%l 行号
     * - %%t 线程id
     * - %%F 协程id
     * - %%N 线程名称
     * - %%% 百分号
     * - %%T 制表符
     * - %%n 换行
     * 
     * 默认格式：%%d{%%Y-%%m-%%d %%H:%%M:%%S}%%T%%t%%T%%N%%T%%F%%T[%%p]%%T[%%c]%%T%%f:%%l%%T%%m%%n
     * 
     * 默认格式描述：年-月-日 时:分:秒 [累计运行毫秒数] \\t 线程id \\t 线程名称 \\t 协程id \\t [日志级别] \\t [日志器名称] \\t 文件名:行号 \\t 日志消息 换行符
     */

    LogFormatter(std::string_view pattern = "%d{%Y-%m-%d %H:%M:%S} [%rms]%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n");

    /**
     * @brief 返回格式化日志文本
     * @param[in] logger 日志器
     * @param[in] level 日志级别
     * @param[in] event 日志事件
     */
    auto format(std::shared_ptr<LogEvent> event)
        -> std::string;

    auto format(std::ostream& ofs,
                std::shared_ptr<LogEvent> event)
        -> std::ostream&;

    /**
     * @brief 初始化，即开始解析日志模板
     */
    auto init() -> void;

    bool isError() const { return m_error; }

    auto getPattern() const -> std::string_view { return m_pattern; }

private:
    std::string m_pattern;                                //格式模板
    std::vector<std::shared_ptr<FormatItemBase>> m_items; //日志格式模板解析后的格式
    bool m_error = false;                                 //解析是否出错
};

} //namespace Cot

#endif