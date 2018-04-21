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


class ServerConnectorUDP
{
public:
	void init(int port);

	datagram receive_next_package();

	void send_package();

	void close();

private:
	int sockfd, n;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	char buf[256];
};