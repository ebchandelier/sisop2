#include "ServerConnectorUDP.h"


void ServerConnectorUDP::init(int port)
{
	// Open socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
	{
		printf("ERROR opening socket");
	}

	// Fill struct
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);    
	 
	// Bind
	auto bind_result = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr));
	if (bind_result < 0)
	{
		printf("ERROR on binding");
	}

	// Set socket to be non-blocking
	auto fcntl_result = fcntl(sockfd, F_SETFL, O_NONBLOCK);
	if (fcntl_result == -1)
	{
		printf("ERROR on fcntl");
	}
	
	clilen = sizeof(struct sockaddr_in);
}

void ServerConnectorUDP::send_package(datagram package, sockaddr_in addr)
{
	n = sendto(sockfd, &package, DATAGRAM_SIZE, 0,(struct sockaddr *) &addr, sizeof(struct sockaddr));
	if (n < 0)
	{
		printf("ERROR on sendto");
	}
}

bool ServerConnectorUDP::has_new_package()
{
	int n = recvfrom(sockfd, &package, DATAGRAM_SIZE, 0, (struct sockaddr *) &cli_addr, &clilen);
	if (n < 0)
	{
		new_package = false;
		//printf("ERROR on recvfrom");
	}
	else
	{
		// TODO: Check read length
		// if (n == DATAGRAM_SIZE) { ... }
		new_package = true;
	}
	return new_package;
}

std::pair<datagram, sockaddr_in> ServerConnectorUDP::receive_next_package_and_addr()
{
	if (new_package)
	{
		new_package = false;
		return std::make_pair(package, cli_addr);
	}
	else
	{
		throw("No package available");
	}
}

void ServerConnectorUDP::close()
{

}