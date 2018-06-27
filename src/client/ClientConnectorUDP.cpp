#include "ClientConnectorUDP.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>

void ClientConnectorUDP::connect(std::string server_name, int port)
{
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
		printf("ERROR opening socket\n");
		return;
	}

	// Fill struct
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = inet_addr(server_name.c_str());
	bzero(&(serv_addr.sin_zero), 8); 
}

void ClientConnectorUDP::send_package(datagram package)
{
	if(package.type == datagram_type::control) {

		package.control.package_id = ++lastSentPackageId;
	}

	if(package.type == datagram_type::data) {

		package.data.package_id = ++lastSentPackageId;
	}
	
	while(true) {

		if(0 <= sendto(sockfd, &package, DATAGRAM_SIZE, 0, (const struct sockaddr *) &serv_addr, sizeof(struct sockaddr_in))) {
			
			datagram possibleAck;
			length = sizeof(struct sockaddr_in);
			recvfrom(sockfd, &possibleAck, DATAGRAM_SIZE, 0, (struct sockaddr *) &from, &length); // adicionar timeout
			
			if(possibleAck.type == datagram_type::ack) {
			
				//std::cout << "ESPERANDO ACK " << lastSentPackageId << ", RECEBIDO => " << possibleAck.ack_last_package_id_received << "\n";

				if(possibleAck.ack_last_package_id_received == lastSentPackageId) {

					return;
				} 
			}
		}
	}
}

datagram ClientConnectorUDP::receive_package()
{
	while(true) {

		datagram package;
		length = sizeof(struct sockaddr_in);
		n = recvfrom(sockfd, &package, DATAGRAM_SIZE, 0, (struct sockaddr *) &from, &length);
		if (n < 0)
		{
			printf("ERROR recvfrom\n");
		
		} else {

			bool shouldSendNewPackage = false;

			if(package.type == datagram_type::control) {

				if(package.control.package_id != lastReceivedPackageId) {

					shouldSendNewPackage = true;
					lastReceivedPackageId = package.control.package_id;
				}
			}

			if(package.type == datagram_type::data) {

				if(package.data.package_id != lastReceivedPackageId) {

					shouldSendNewPackage = true;
					lastReceivedPackageId = package.data.package_id;
				}
			}

			// Send ack  
			datagram ack;  
			ack.type = datagram_type::ack;
			ack.ack_last_package_id_received = lastReceivedPackageId;
			//std::cout << "MANDANDO ACK " << lastReceivedPackageId << "\n";
			sendto(sockfd, &ack, DATAGRAM_SIZE, 0, (const struct sockaddr *) &from, sizeof(struct sockaddr_in));
			
			if(shouldSendNewPackage) {

				return package;
			}
		}
	}
}

void ClientConnectorUDP::close()
{
}