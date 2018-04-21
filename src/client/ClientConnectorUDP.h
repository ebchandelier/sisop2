#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#pragma once
#include "../shared/datagram.h"
#include <string>

class ClientConnectorUDP
{
public:
	void connect(std::string server_name, int port);

	void send_package(datagram package);

	datagram receive_package();

	void close();

private:
	int sockfd;

	struct sockaddr_in serv_addr, cli_addr;
};