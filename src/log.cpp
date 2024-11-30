// #include "../include/sylarweb/log.h"
// #include "../include/sylarweb/loglevel.h"
// #include "../include/sylarweb/logappender.h"
// #include "../include/sylarweb/logevent.h"

#include "sylarweb/log.h"
#include "sylarweb/loglevel.h"
#include "sylarweb/logappender.h"
#include "sylarweb/logevent.h"

#include <iostream>
#include <tuple>
#include <unordered_map>
#include <functional>

namespace Cot {

/* ======================== Logger ======================== */
Logger::Logger(std::string_view name)
    : m_name(name)
    , m_level(LogLevel::DEBUG)
    , m_createTime(Sylar::getElapseMs())

{
}

void Logger::addAppender(LogAppenderBase::Sptr appender)
{
    auto lk_guard = std::lock_guard<SpinMutex>(m_mutex);
    m_appenders.push_back(appender);
}

void Logger::delAppender(LogAppenderBase::Sptr appender)
{
    auto lk_guard = std::lock_guard<MutexType>(m_mutex);
    for (auto it = std::begin(m_appenders); it != std::end(m_appenders); it++)
    {
        if (*it == appender)
        {
            m_appenders.erase(it);
            break;
        }
    }
}

void Logger::clearAppender()
{
    auto lk_guard = std::lock_guard<MutexType>(m_mutex);
    m_appenders.clear();
}

void Logger::log(LogEvent::Sptr event)
{
    if (event->getLevel() <= m_level)
    {
        for (auto& i : m_appenders)
        {
            i->log(event);
        }
    }
}

} //namespace Cot
