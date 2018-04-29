#pragma once

#include <map>
#include <thread>
#include "ServerConnectorUDP.h"
#include "../shared/DatagramStringifier.h"
#include "ClientHandler.h"
#include "ThreadSafeQueue.h"

class Server
{
public:
    Server(int port);
    void run();

private:
    int port;
};