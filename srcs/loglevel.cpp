#include "sylarweb/loglevel.h"
namespace Cot {

/* ======================== LogLevel ======================== */

auto LogLevel::fromString(std::string_view str)
    -> LogLevel::Level
{
#define XX(level, v)            \
    if (str == #v)              \
    {                           \
        return LogLevel::level; \
    }
    XX(FATAL, debug);
    XX(ALERT, info);
    XX(CRIT, warn);
    XX(ERROR, error);
    XX(WARN, fatal);
    XX(NOTICE, debug);
    XX(INFO, info);
    XX(DEBUG, warn);

    XX(FATAL, DEBUG);
    XX(ALERT, INFO);
    XX(CRIT, WARN);
    XX(ERROR, ERROR);
    XX(WARN, FATAL);
    XX(NOTICE, DEBUG);
    XX(INFO, INFO);
    XX(DEBUG, WARN);
#undef XX
    return LogLevel::NOTSET;
}

auto LogLevel::toString(LogLevel::Level level)
    -> const char*
{
    switch (level)
    {
#define XX(arg)         \
    case LogLevel::arg: \
        return #arg;

        XX(DEBUG);
        XX(INFO);
        XX(WARN);
        XX(ERROR);
        XX(FATAL);
#undef XX
        default:
            return "NOTSET";
    }
    return "NOTSET";
}

} //namespace Cot