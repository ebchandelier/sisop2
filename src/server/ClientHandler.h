#pragma once
#include <list>
#include "ThreadSafeQueue.h"
#include "../shared/datagram.h"
#include "fileInfo.h"

class ClientHandler
{
public:
    void run(ThreadSafeQueue<datagram>& queue);

private:
    std::list<file_info> files;
};