#pragma once
#include <list>
#include <string>
#include <stdio.h>
#include "ThreadSafeQueue.h"
#include "OutgoingPackages.h"
#include "../shared/datagram.h"
#include "../shared/DatagramStringifier.h"
#include "../shared/PersistenceFileManager.cpp"
#include "../shared/DeviceFilesInfo.h"
#include "../shared/FileInfoVectorSerializer.h"
#include "../shared/ListFiles.h"

enum class ClientHandlerState
{
    receiving_file,
    other
};

class ClientHandler
{
public:

    ClientHandler(std::string base_path, ThreadSafeQueue<datagram>* incoming_packages, OutgoingPackages* outgoing_packages);
    void run();

private:

    DeviceFilesInfo device_files;

    file_info upload_fileinfo;

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