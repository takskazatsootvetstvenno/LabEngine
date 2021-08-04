#ifndef LOG_HPP
#define LOG_HPP
#include <ctime>
#include <iostream>
#include <string>
namespace LogLEFuncs
{
    inline std::string logGetTimeString() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        return std::to_string(ltm->tm_hour) + ":" + std::to_string(ltm->tm_min) + ":" +
            std::to_string(ltm->tm_sec);
    }
}
namespace LabEngine {
#ifdef NDEBUG
#    define LOG_INFO(...)   // std::cout<<"LOG:  "<< __VA_ARGS__;
#    define LOG_WARN(...)   // std::cout<<"WARN:  "<< __VA_ARGS__;
#    define LOG_ERROR(...)  // std::cout<<"ERROR:  "<< __VA_ARGS__;
#    define LOG_CRIT(...)   // std::cout<<"CRIT:  "<< __VA_ARGS__;
#else
#    define LOG_INFO(...) std::cout << LogLEFuncs::logGetTimeString() << "||LOG:  " << __VA_ARGS__ << std::endl;
#    define LOG_WARN(...) std::cout << LogLEFuncs::logGetTimeString() << "||WARN:  " << __VA_ARGS__ << std::endl;
#    define LOG_ERROR(...) std::cout << LogLEFuncs::logGetTimeString() << "||ERROR:  " << __VA_ARGS__ << std::endl;
#    define LOG_CRIT(...) std::cout << LogLEFuncs::logGetTimeString() << "||CRIT:  " << __VA_ARGS__ << std::endl;
#endif
}  // namespace LabEngine
#endif
