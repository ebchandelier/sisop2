#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <string>

class ClientConnectorUDP
{
	void connect(std::string server, int port);

	void send_package();

	void receive_package();
};