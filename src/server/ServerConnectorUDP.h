#pragma once

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <utility>
#include <fcntl.h>

#include "../shared/datagram.h"


class ServerConnectorUDP
{
public:

	// Initializes a UDP connection on this port
	void init(int port);

	bool has_new_package();

	// Blocking get the next package and return it with the sender information
	std::pair<datagram, sockaddr_in> receive_next_package_and_addr();

	// Send package to client at 'addr'
	void send_package(datagram package, sockaddr_in addr);

	// Closes this UDP connection
	void close();

private:
	int sockfd, n;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;

	int lastReceivedPackageId;
	int lastSentPackageId = 0;

	bool new_package = false;
	datagram package;
};