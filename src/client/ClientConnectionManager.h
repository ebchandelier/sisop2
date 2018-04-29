#pragma once
#include <cstring>
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

private:

    ClientConnectorUDP connector;
};
