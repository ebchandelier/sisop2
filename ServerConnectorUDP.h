#pragma once

class ServerConnectorUDP
{
public:
	void init();

	void receive_next_package();

	void close();

private:
	int sockfd;
};