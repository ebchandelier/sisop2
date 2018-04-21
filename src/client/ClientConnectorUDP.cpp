#include "ClientConnectorUDP.h"

void ClientConnectorUDP::connect(std::string server_name, int port)
{
	socklen_t clilen;
	struct hostent *server;
	char buf[256];

	// Get server
	server = gethostbyname(server_name.c_str());
	if (server == NULL) 
	{
		printf("ERROR, no such host\n");
		return;
	}

	// Open socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1)
	{
		printf("ERROR opening socket");
		return;
	}

	// Fill struct
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8); 
}

void ClientConnectorUDP::send_package(datagram package)
{
	int n = sendto(sockfd, &package, DATAGRAM_SIZE, 0, (const struct sockaddr *) &serv_addr, sizeof(struct sockaddr_in));
	if (n < 0)
	{
		printf("ERROR sendto");
	}
}

datagram ClientConnectorUDP::receive_package()
{
	datagram package;
	unsigned int length;
	struct sockaddr_in from;
	int n = recvfrom(sockfd, &package, DATAGRAM_SIZE, 0, (struct sockaddr *) &from, &length);
	if (n < 0)
	{
		printf("ERROR recvfrom");
	}
}

void ClientConnectorUDP::close()
{
}