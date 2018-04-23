#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class ThreadSafeQueue
{
    int maximum_size;
    
public:
    ThreadSafeQueue(int maximum_size)
    { 
        this->maximum_size = maximum_size;
    }
    // TODO:
    void produce (T element)
    {

    }

    T consume()
    {

    }

    
};
