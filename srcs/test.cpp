#include "sylarweb/log.h"
#include "sylarweb/logevent.h"
#include "sylarweb/loglevel.h"
#include "sylarweb/util.h"
#include "sylarweb/logappender.h"
#include <time.h>
#include "sylarweb/lexicalcast.hpp"
#include <iostream>
#include <sstream>

using namespace Cot;

auto main()
    -> int
{
    auto logger = std::make_shared<Cot::Logger>();
    // auto fmter  = std::make_shared<LogFormatter>("%d{%Y-%m-%d %H:%M:%S} [%rms]%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n");
    logger->addAppender(std::make_shared<LogAppenderImpl<StdoutLogAppender>>());
    auto level = Cot::LogLevel::DEBUG;
    COT_LOG_LEVEL(logger, level);

    auto str1   = LexicalCast<int, std::string>()(123);      // ok, str1等于"123"
    auto int1   = LexicalCast<std::string, int>()("123");    // ok, int1等于123
    auto str2   = LexicalCast<float, std::string>()(3.14);   // ok，str2等于"3.14"
    auto float2 = LexicalCast<std::string, float>()("3.14"); // ok，float2等于3.14

    std::cout << str1 << std::endl;
    std::cout << int1 << std::endl;
    std::cout << str2 << std::endl;
    std::cout << float2 << std::endl;

    // auto ss       = std::stringstream();
    auto emiter   = YAML::Emitter();
    auto node     = YAML::LoadFile("/home/tan/sylar-web/srcs/test.yaml");
    auto node_str = std::string((emiter << node).c_str());
    std::cout << node_str << std::endl;
    auto fstr = std::string("[1, 2, 3]");
    auto vec  = LexicalCast<std::string, std::vector<int>>()(fstr);
    for (auto i = size_t {0}; i < vec.size(); i++)
    {
        std::cout << vec[i] << " ";
    }
    printf("\n");
    auto str = LexicalCast<std::vector<int>, std::string>()(vec);

    std::cout << Sylar::typeToName<int>() << std::endl;

    std::cout << Sylar::typeToName<std::vector<int>>() << std::endl;

    std::cout << Sylar::typeToName<std::vector<std::vector<int>>>() << std::endl;

    // auto v = LexicalCast<std::string, std::vector<int>>()(...); // 错误，LexicalCast目前还不支持实例化T类型为vector<int>的模板参数
    // auto s = LexicalCast<std::vector<int>, std::string>()(...); // 错误，同上
    return 0;
};