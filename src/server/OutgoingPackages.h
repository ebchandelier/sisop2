#pragma once

#include <netdb.h>
#include "../shared/datagram.h"
#include "ThreadSafeQueue.h"

class OutgoingPackages
{
public:

    OutgoingPackages(sockaddr_in receiver, ThreadSafeQueue<std::pair<sockaddr_in, datagram>>* queue)
    {
        this->receiver = receiver;
        this->queue = queue;
    }

    void produce(datagram package)
    {
        queue->produce({ receiver, package });
    }

    ThreadSafeQueue<std::pair<sockaddr_in, datagram>>* queue;
    sockaddr_in receiver;
};