#pragma once
#include <cstring>
#include <mutex>
#include "ClientConnectorUDP.h"
#include "../shared/datagram.h"
#include "../shared/PersistenceFileManager.cpp"
#include "../shared/DatagramStringifier.h"

class ClientConnectionManager
{
public:
    // Tries to connect to the server. Returns -1 on error and 0 on success
    int login_server(char* username, char* host, int port);
    void sync_client();
    void send_file(char* file);
    void get_file(char* file);
    void delete_file(char* file);
    int logout();

    std::string work_dir;

private:
    ClientConnectorUDP connector;

    // This class behaves like a monitor, only allowing one method to be executing at a time
    std::mutex mutex;
};
