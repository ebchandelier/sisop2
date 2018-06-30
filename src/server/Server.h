#pragma once

#include <map>
#include <thread>
#include "ServerConnectorUDP.h"
#include "../shared/DatagramStringifier.h"
#include "../shared/DeviceFilesInfo.h"
#include "ClientHandler.h"
#include "ThreadSafeQueue.h"
#include "DistributedServer.h"
#include<iostream>

#define MAXIMUM_PACKAGES_QUEUE_SIZE 100

typedef uint32_t client_id_t;
typedef std::string user_t;

class Server
{
public:
    Server(int port, std::string base_path);
    Server(int port, std::string base_path, std::string ip, int portToConnect);
    void run();

private:
    int port;
    std::string work_path;

    void process_incoming_message();
    void process_outgoing_message();

    void create_root_folder_if_needed();

    DatagramStringifier stringifier;
    ServerConnectorUDP connector;
    std::map<std::string, DeviceFilesInfo*> users_files;
    std::map<client_id_t, ClientHandler> handlers;
    //std::map<user_t, DeviceFilesInfo> device_files;
	std::map<client_id_t, ThreadSafeQueue<datagram>*> incoming_queues;
	ThreadSafeQueue<std::pair<sockaddr_in, datagram>> outgoing_packages;

    std::vector<PROCESS_PATH> ipPortConnectedList;
    std::vector<TYPE> shouldWarn;
    int threadCount = 0;
    int elected = -1; // -1 is the initial value for not entered the election
    bool fightingForElection = false;
};