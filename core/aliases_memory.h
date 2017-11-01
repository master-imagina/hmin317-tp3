#ifndef ALIASES_MEMORY_H
#define ALIASES_MEMORY_H

#include <memory>
#include <vector>


template <class T>
using uptr = std::unique_ptr<T>;

template <class T>
using uptr_vector = std::vector<std::unique_ptr<T>>;

#endif // ALIASES_MEMORY_H
