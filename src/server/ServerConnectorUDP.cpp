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
	
	clilen = sizeof(struct sockaddr_in);
}

void ServerConnectorUDP::send_package(datagram package)
{
	n = sendto(sockfd, &package, DATAGRAM_SIZE, 0,(struct sockaddr *) &cli_addr, sizeof(struct sockaddr));
	if (n < 0)
	{
		printf("ERROR on sendto");
	}
}

datagram ServerConnectorUDP::receive_next_package()
{
	datagram package;
	int n = recvfrom(sockfd, &package, DATAGRAM_SIZE, 0, (struct sockaddr *) &cli_addr, &clilen);
	if (n < 0)
	{
		printf("ERROR on recvfrom");
	}
	return package;
}

void ServerConnectorUDP::close()
{

}