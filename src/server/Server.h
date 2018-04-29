#pragma once

#include <map>
#include <thread>
#include "ServerConnectorUDP.h"
#include "../shared/DatagramStringifier.h"
#include "ClientHandler.h"
#include "ThreadSafeQueue.h"

#define MAXIMUM_PACKAGES_QUEUE_SIZE 100


class Server
{
public:
    Server(int port);
    void run();

private:
    int port;

    void process_incoming_messages();
    void process_outgoing_messages();

    DatagramStringifier stringifier;
    ServerConnectorUDP connector;
    std::map<uint32_t, ClientHandler> handlers;
	std::map<uint32_t, ThreadSafeQueue<datagram>*> incoming_queues;
	ThreadSafeQueue<std::pair<sockaddr_in, datagram>> outgoing_packages;

};