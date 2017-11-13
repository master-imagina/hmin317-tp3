#ifndef LOG_H
#define LOG_H

#include <cstdlib>

#define C_ASSERT(cond, message) \
    if (!(cond)) { \
        std::cerr << "assert failed: " << message << std::endl; \
        std::abort(); \
    }

#endif // LOG_H
