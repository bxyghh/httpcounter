#ifndef MEMORY_COUNTER_H
#define MEMORY_COUNTER_H

#include "HttpCounter.h"

class MemoryCounter: public Counter {
public:
    MemoryCounter(): _count(0) {}
    void Increment() override { _count++; }
    int Value() const override { return _count; }
private:
    int _count;
};
#endif // #ifndef MEMORY_COUNTER_H