#include <functional>

#include "sylarweb/log.h"
#include "sylarweb/logformatitem.hpp"
#include "sylarweb/logformatter.h"
#include "sylarweb/loglevel.h"
#include "sylarweb/logevent.h"

#include <unordered_map>
#include <iostream>

namespace {
enum class ParseState {
    NORMAL,
    PATTERN
};
}

namespace Cot {

/* ======================== FormatterItem ======================== */
/**
 * @brief 消息format
 */
class MessageFormatItem {
public:
    static void format(std::ostream& os,
                       LogEvent::Sptr event)
    {
        os << event->getContent();
    }
};

//日志级别format
class LevelFormatItem {
public:
    static void format(std::ostream& os,
                       LogEvent::Sptr event)
    {
        os << LogLevel::toString(event->getLevel());
    }
};

/**
 * @brief 执行时间format
 */
class ElapseFormatItem {
public:
    static void format(std::ostream& os,
                       LogEvent::Sptr event)
    {
        os << event->getElapse();
    }

private:
};

/**
 * @brief 日志器名称format
 */
class NameFormatItem {
public:
    static void format(std::ostream& os,
                       LogEvent::Sptr event)
    {
        os << event->getLoggerName();
    }

private:
};

/**
 * @brief 线程ID format
 */
class ThreadIdFormatItem {
public:
    static void format(std::ostream& os,
                       LogEvent::Sptr event)
    {
        os << event->getThreadId();
    }
};

/**
 * @brief 协程Id format
 */
class FiberIdFormatItem {
public:
    static void format(std::ostream& os,
                       LogEvent::Sptr event)

    {
        os << event->getFiberId();
    }
};

/**
 * @brief 线程名称format
 */
class ThreadNameFormatItem {
public:
    static void format(std::ostream& os,
                       LogEvent::Sptr event)
    {
        os << event->getThreadName();
    }

private:
};

/**
 * @brief 时间format
 */
class DateTimeFormatItem {
public:
    DateTimeFormatItem(std::string_view dateformat = "%Y-%m-%d %H:%M:%S")
        : m_format(dateformat)
    {
        if (m_format.empty())
        {
            m_format = "%Y-%m-%d %H:%M:%S";
        }
    }

    void format(std::ostream& os,
                LogEvent::Sptr event)
    {
        struct tm tm;
        time_t time = event->getTime();
        localtime_r(&time, &tm);
        char buf[64];
        strftime(buf, sizeof(buf), m_format.data(), &tm);
        os << buf;
    }

private:
    std::string m_format;
};

class NewLineFormatItem {
public:
    static void format(std::ostream& os,
                       LogEvent::Sptr event)
    {
        // os << event->getFiberId();
        os << "\n";
    }
};

/**
 * @brief 文件名format
 */
class FilenameFormatItem {
public:
    static void format(std::ostream& os,
                       LogEvent::Sptr event)
    {
        os << event->getFile();
    }
};

/**
 * @brief 行号format
 */
class LineFormatItem {
public:
    static void format(std::ostream& os,
                       LogEvent::Sptr event)
    {
        os << event->getLine();
    }

private:
};

/**
 * @brief tab format
 */
class TabFormatItem {
public:
    static void format(std::ostream& os, LogEvent::Sptr event)
    {
        os << "\t";
    }
};

class StringFormatItem {
public:
    StringFormatItem(std::string_view str)
        : m_string(str)
    {
    }

    void format(std::ostream& os,
                LogEvent::Sptr event)
    {
        os << m_string;
    }

private:
    std::string m_string;
};

class PercentSignFormatItem {
public:
    static void format(std::ostream& os, LogEvent::Sptr event)
    {
        os << "%";
    }
};

/* ======================== LogFormatter ======================== */

LogFormatter::LogFormatter(std::string_view pattern)
    : m_pattern(pattern)
{
    init();
}

auto LogFormatter::format(LogEvent::Sptr event)
    -> std::string
{
    auto ss = std::stringstream();
    for (auto& i : m_items)
    {
        i->format(ss, event);
    }
    return ss.str();
}

auto LogFormatter::format(std::ostream& ofs,
                          LogEvent::Sptr event)
    -> std::ostream&
{
    for (auto& i : m_items)
    {
        i->format(ofs, event);
    }
    return ofs;
}

// //%xxx %xxx{xxx} %%
// /**
//  * 简单的状态机判断，提取pattern中的常规字符和模式字符
//  *
//  * 解析的过程就是从头到尾遍历，根据状态标志决定当前字符是常规字符还是模式字符
//  *
//  * 一共有两种状态，即正在解析常规字符和正在解析模板转义字符
//  *
//  * 比较麻烦的是%%d，后面可以接一对大括号指定时间格式，比如%%d{%%Y-%%m-%%d %%H:%%M:%%S}，这个状态需要特殊处理
//  *
//  * 一旦状态出错就停止解析，并设置错误标志，未识别的pattern转义字符也算出错
//  *
//  * @see LogFormatter::LogFormatter
//  */

void LogFormatter::init()
{
    auto patterns   = std::vector<std::pair<bool, std::string>>(); // true is pattern, false is normal str;
    auto normal_str = std::string();                               // to store the normal str

    auto dateformat = std::string(); // the c after d{

    auto state = ParseState::NORMAL;
    for (auto i = size_t {0}; i < m_pattern.size(); i++)
    {
        auto c = std::string(1, m_pattern[i]);
        switch (state)
        {
            case ParseState::NORMAL: {
                if (c == "%")
                {
                    if (not normal_str.empty())
                    {
                        patterns.push_back(std::make_pair(0, std::move(normal_str)));
                    }
                    /* 对于只有%的情况 */
                    if (i + 1 >= m_pattern.size())
                    {
                        // m_error = true;
                        return;
                    }
                    normal_str.clear();
                    state = ParseState::PATTERN;
                }
                else
                {
                    normal_str.push_back(c[0]);
                    state = ParseState::NORMAL;
                }
                break;
            }
            case ParseState::PATTERN: {
                if (c == "%")
                {
                    normal_str.push_back('%');
                    patterns.push_back(std::make_pair(0, std::move(normal_str)));
                    state = ParseState::NORMAL;
                }
                else if (c == "d")
                {
                    patterns.push_back(std::pair(1, c));
                    i++;
                    if (i >= m_pattern.size())
                        break;

                    if (m_pattern[i] != '{')
                    {
                        state = ParseState::NORMAL;
                        i--;
                        break;
                    }
                    i++;
                    while (i < m_pattern.size() and m_pattern[i] != '}')
                    {
                        dateformat.push_back(m_pattern[i]);
                        i++;
                    }
                    if (i >= m_pattern.size())
                    {
                        // m_error = true;
                        return;
                    }
                    state = ParseState::NORMAL;
                }
                else
                {
                    patterns.push_back(std::make_pair(1, c));
                    state = ParseState::NORMAL;
                }
                break;
            }
        }
    }
    // for debug
    // for (auto& [type, str] : patterns) { std::cout << type << ":" << str << std::endl;
    // }
    // std::cout << dateformat << std::endl;
    if (not normal_str.empty())
    {
        patterns.push_back(std::make_pair(0, std::move(normal_str)));
    }

    using GetItemFuncs = std::function<FormatItemBase::Sptr()>;

    static auto sItemFuncs = std::unordered_map<std::string, GetItemFuncs> {
#define XX(p, item)                                                                                               \
    {                                                                                                             \
#p, []() -> FormatItemBase::Sptr { return std::make_shared<FormatItemBase>(new FormatItemImpl<item>()); } \
    }
#include "itemtype.inl"
#undef XX
    };

    for (auto& p : patterns)
    {
        if (p.first == 0)
        {
            m_items.push_back(std::make_shared<FormatItemBase>(new FormatItemImpl<StringFormatItem>(p.second)));
        }
        else if (p.second == "d")
        {
            m_items.push_back(std::make_shared<FormatItemBase>(new FormatItemImpl<DateTimeFormatItem>(dateformat)));
        }
        else
        {
            auto it = sItemFuncs.find(p.second);
            if (it == sItemFuncs.end())
            {
                std::cout << "[ERROR] LogFormatter::init() "
                          << "pattern: [" << m_pattern << "] "
                          << "unknown format item: " << p.second << std::endl;
                return;
            }
            m_items.push_back(it->second());
        }
    }
}
} //namespace Cot
