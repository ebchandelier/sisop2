#pragma once
#include <list>
#include <string>
#include "ThreadSafeQueue.h"
#include "../shared/datagram.h"
#include "../shared/DatagramStringifier.h"
#include "../shared/PersistenceFileManager.cpp"
#include "OutgoingPackages.h"
#include "../shared/fileInfo.h"

enum class ClientHandlerState
{
    receiving_file
};

class ClientHandler
{
public:

    ClientHandler(std::string base_path, ThreadSafeQueue<datagram>* incoming_packages, OutgoingPackages* outgoing_packages);
    void run();

private:
    std::list<file_info> files;

    ThreadSafeQueue<datagram> *incoming_packages;
    OutgoingPackages *outgoing_packages;

    DatagramStringifier stringifier;

    ClientHandlerState state;

    std::vector<datagram> buffer;

    std::string username;
    std::string working_file_name;

    std::string base_path;
    std::string user_path;
};