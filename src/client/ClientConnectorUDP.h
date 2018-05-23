#pragma once

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include "../shared/datagram.h"
#include <string>

class ClientConnectorUDP
{
public:

	// Initializes an UDP connection with the server
	void connect(std::string server_name, int port);

	// Blocking send package to the server
	void send_package(datagram package);

	// Blocking get package from the server
	datagram receive_package();

	// Closes this UDP connection
	void close();

private:
    int sockfd, n;
	int lastReceivedPackageId;
	int lastSentPackageId = 0;
	unsigned int length;
	struct sockaddr_in serv_addr, from;
	struct hostent *server;
};