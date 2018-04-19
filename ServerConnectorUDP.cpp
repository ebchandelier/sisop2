#include "ServerConnectorUDP.h"


void ServerConnectorUDP::init()
{
	
	struct sockaddr_in serv_addr, cli_addr;
	char buf[256];

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		printf("ERROR opening socket");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr)) < 0)
		printf("ERROR on binding");

	return 0;
}

void ServerConnectorUDP::receive_next_package()
{
	struct sockaddr_in cli_addr;
	socklen_t clilen = sizeof(struct sockaddr_in);
	/* receive from socket */
	int n = recvfrom(sockfd, buf, 256, 0, (struct sockaddr *) &cli_addr, &clilen);
	if (n < 0)
		printf("ERROR on recvfrom");
	printf("Received a datagram: %s\n", buf);

	/* send to socket */
	n = sendto(sockfd, "Got your message\n", 17, 0, (struct sockaddr *) &cli_addr, sizeof(struct sockaddr));
	if (n  < 0)
		printf("ERROR on sendto");

}

void ServerConnectorUDP::close()
{
	close(sockfd);
}