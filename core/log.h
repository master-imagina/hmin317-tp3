#ifndef LOG_H
#define LOG_H

#include <cstdlib>

#define C_ASSERT(cond, message) \
    if (!(cond)) { \
        std::cerr << "assert failed: " << message << std::endl; \
    }

#endif // LOG_H
