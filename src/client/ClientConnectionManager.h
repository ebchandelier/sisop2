#pragma once
#include "ClientConnectorUDP.h"
#include "../shared/datagram.h"
#include "../shared/DatagramStringifier.h"
class ClientConnectionManager
{
public:
    int login_server(char* host, int port);
    void sync_client();
    void send_file(char* file);
    void get_file(char* file);
    void delete_file(char* file);
    void close_session();

private:

    ClientConnectorUDP connector;
};