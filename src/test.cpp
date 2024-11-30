#include "sylarweb/log.h"
#include "sylarweb/logevent.h"
#include "sylarweb/loglevel.h"
#include "sylarweb/util.h"
#include "sylarweb/logappender.h"
#include <time.h>

using namespace Cot;

auto main()
    -> int
{
    auto logger = std::make_shared<Cot::Logger>(new Cot::Logger());
    // auto fmter  = std::make_shared<LogFormatter>("%d{%Y-%m-%d %H:%M:%S} [%rms]%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n");
    // logger->addAppender(std::make_shared<LogAppenderBase>(new LogAppenderImpl<StdoutLogAppender>(fmter)));
    // auto level = Cot::LogLevel::DEBUG;
    // COT_LOG_LEVEL(logger, level);

    return 0;
};