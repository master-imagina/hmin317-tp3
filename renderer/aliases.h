#ifndef MEMORYTOOLS_H
#define MEMORYTOOLS_H

#include <memory>
#include <vector>


template <class T>
using uptr = std::unique_ptr<T>;

template <class T>
using uptr_vector = std::vector<std::unique_ptr<T>>;

#endif // MEMORYTOOLS_H
