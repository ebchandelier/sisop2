#pragma once
#include "ClientConnectorUDP.h"
#include "../shared/datagram.h"
#include "../shared/DatagramStringifier.h"
#include "dropboxUtil.h"

class ClientConnectionManager
{
public:
    // Tries to connect to the server. Returns -1 on error and 0 on success
    int login_server(char* host, int port);
    void sync_client();
    void send_file(char* file);
    void get_file(char* file);
    void delete_file(char* file);
    int close_session();

private:

    ClientConnectorUDP connector;
};
