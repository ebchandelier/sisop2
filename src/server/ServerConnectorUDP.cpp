#include "ServerConnectorUDP.h"


void ServerConnectorUDP::init(int port)
{
	
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
		printf("ERROR opening socket");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);    
	 
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr)) < 0) 
		printf("ERROR on binding");
	
	clilen = sizeof(struct sockaddr_in);
	
}

void ServerConnectorUDP::send_package()
{
	n = sendto(sockfd, "Got your message\n", 17, 0,(struct sockaddr *) &cli_addr, sizeof(struct sockaddr));
	if (n  < 0)
	{
		printf("ERROR on sendto");
	}
}

void ServerConnectorUDP::receive_next_package()
{
	int n = recvfrom(sockfd, buf, 256, 0, (struct sockaddr *) &cli_addr, &clilen);
	if (n < 0)
	{
		printf("ERROR on recvfrom");
	}
	printf("Received a datagram: %s\n", buf);

}

void ServerConnectorUDP::close()
{

}