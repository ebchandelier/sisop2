#pragma once
#include <list>
#include "ThreadSafeQueue.h"
#include "../shared/datagram.h"
#include "../shared/DatagramStringifier.h"
#include "../shared/PersistenceFileManager.cpp"
#include "OutgoingPackages.h"
#include "fileInfo.h"

enum class ClientHandlerState
{
    receiving_file
};

class ClientHandler
{
public:

    ClientHandler(ThreadSafeQueue<datagram>* incoming_packages, OutgoingPackages* outgoing_packages);
    void run();

private:
    std::list<file_info> files;

    ThreadSafeQueue<datagram> *incoming_packages;
    OutgoingPackages *outgoing_packages;

    DatagramStringifier stringifier;

    ClientHandlerState state;

    std::vector<datagram> buffer;
};