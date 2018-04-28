#pragma once

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
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