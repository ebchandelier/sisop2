#include "ServerConnectorUDP.h"
#include <iostream>

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
	// Set socket to be non-blocking
	auto bind_result = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr));
	if (bind_result < 0)
	{
		printf("ERROR on binding");
	}

	auto fcntl_result = fcntl(sockfd, F_SETFL, O_NONBLOCK);
	if (fcntl_result == -1)
	{
		printf("ERROR on fcntl");
	}
	
	clilen = sizeof(struct sockaddr_in);
}

void ServerConnectorUDP::send_package(datagram package, sockaddr_in addr, bool with_ack)
{
	if(package.type == datagram_type::control) {

		package.control.package_id = ++lastSentPackageId;
	}

	if(package.type == datagram_type::data) {

		package.data.package_id = ++lastSentPackageId;
	}
	
	while(true) {

		if(0 <= sendto(sockfd, &package, DATAGRAM_SIZE, 0,(struct sockaddr *) &addr, sizeof(struct sockaddr))) {
			if (!with_ack) {
				return;
			}
			datagram possibleAck;
			if(0 <= recvfrom(sockfd, &possibleAck, DATAGRAM_SIZE, 0, (struct sockaddr *) &cli_addr, &clilen)) {

				if(possibleAck.type == datagram_type::ack) {

					// std::cout << "ESPERANDO ACK " << lastSentPackageId << ", RECEBIDO => " << possibleAck.ack_last_package_id_received << "\n";

					if(possibleAck.ack_last_package_id_received == lastSentPackageId) {

						return;
					}
				} 
			} 
		}
	}
}

bool ServerConnectorUDP::has_new_package(bool with_ack)
{
	new_package = false;
	
	if(0 <= recvfrom(sockfd, &package, DATAGRAM_SIZE, 0, (struct sockaddr *) &cli_addr, &clilen)){

		if (!with_ack) {
			new_package = true;
			return new_package;
		}
		if(package.type == datagram_type::control) {

			if(package.control.package_id != getLastPackageReceivedOfClient(cli_addr)) {
		
				new_package = true;
				setLastPackageReceivedOfClient(package.control.package_id, cli_addr);
			}
		}

		if(package.type == datagram_type::data) {

			if(package.data.package_id != getLastPackageReceivedOfClient(cli_addr)) {
				
				new_package = true;
				setLastPackageReceivedOfClient(package.data.package_id, cli_addr);
			}
		}

		// Send ack  
		datagram ack;  
		ack.type = datagram_type::ack;
		ack.ack_last_package_id_received = getLastPackageReceivedOfClient(cli_addr);
		// std::cout << "MANDANDO ACK " <<  getLastPackageReceivedOfClient(cli_addr) << "\n";
		sendto(sockfd, &ack, DATAGRAM_SIZE, 0,(struct sockaddr *) &cli_addr, sizeof(struct sockaddr));
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

bool isEqual(sockaddr_in anAddr, sockaddr_in fraeAddress) {

	return (anAddr.sin_addr.s_addr == fraeAddress.sin_addr.s_addr) && (anAddr.sin_port == fraeAddress.sin_port);
}

void ServerConnectorUDP::setLastPackageReceivedOfClient(int lastPackageReveived, sockaddr_in client) {

	bool found = false;
	for(int i=0; i<lastReceivedPackageIdOfClient.size(); i++) {

		std::pair<int, sockaddr_in> lastReceivedPackageId = lastReceivedPackageIdOfClient.at(i);

		if(isEqual(lastReceivedPackageId.second, client)) {

			found = true;
			lastReceivedPackageIdOfClient.at(i) = std::make_pair(lastPackageReveived, client);
		}
	}

	if(!found) {
		
		std::cout << "NEW INSTANCE OF DROPBOXCLIENT CONNECTED!\n";
		lastReceivedPackageIdOfClient.push_back(std::make_pair(lastPackageReveived, client));
	}
}

int ServerConnectorUDP::getLastPackageReceivedOfClient(sockaddr_in client) {

	for(std::pair<int, sockaddr_in> lastReceivedPackageId : lastReceivedPackageIdOfClient) {

		if(isEqual(lastReceivedPackageId.second, client)) {

			return lastReceivedPackageId.first;
		}
	}

	setLastPackageReceivedOfClient(0, client);

	return getLastPackageReceivedOfClient(client);
}
