#include <map>
#include <thread>
#include "ServerConnectorUDP.h"
#include "../shared/DatagramStringifier.h"
#include "ClientHandler.h"
#include "ThreadSafeQueue.h"

#define MAXIMUM_PACKAGES_QUEUE_SIZE 100

int main(int argc, char **argv)
{
	printf("Initializing server...\n");
	int port;

	if(argc < 3) {
		  printf("How to use:\n");
		  printf("dropboxServer -p <port>\n");
		  exit(1);
	 }
    for(int i=1; i<argc; i++) {
		if(argv[i][0]=='-') {
			switch(argv[i][1]) {
				case 'p':
					i++;
					port = atoi(argv[i]);
					if(port < 1024) {
						printf("Invalid port value\n");
						exit(1);
					}
					break;
						  
				default:
					printf("Invalid parameter %d: %s\n", i, argv[i]);
					exit(1);
			}
		} else {
			printf("Invalid parameter %d: %s\n", i, argv[i]); 
			exit(1);
		}
	}



	std::map<uint32_t, ClientHandler> handlers;
	std::map<uint32_t, ThreadSafeQueue<datagram>*> incoming_queues;
	std::map<uint64_t, sockaddr_in> senders;
	ThreadSafeQueue<std::pair<sockaddr_in, datagram>> outgoing_packages(100);

	printf("Initializing UDP stack...\n");
    ServerConnectorUDP connector = ServerConnectorUDP();
	connector.init(port);
	auto stringifier = DatagramStringifier();
	printf("Done. Waiting for packages:\n");
	while(true)
	{
		auto package_and_addr = connector.receive_next_package_and_addr();
		auto package = package_and_addr.first;
		auto addr = package_and_addr.second;
		auto client_id = addr.sin_addr.s_addr;
		printf("Received package from %d:\n%s\n", client_id, stringifier.stringify(package).c_str());
		// If this is a package from a new client
		if (handlers.count(client_id) == 0)
		{
			// Create a queue for future incoming messages
			incoming_queues.emplace(client_id, new ThreadSafeQueue<datagram>(MAXIMUM_PACKAGES_QUEUE_SIZE));
			// Create a ClientHandler
			handlers.emplace(client_id, ClientHandler(incoming_queues.at(client_id), new OutgoingPackages(addr, &outgoing_packages)));
			// And run the handler in a new thread
			std::thread([&handlers, client_id]() {
				handlers.at(client_id).run();
			}).detach();
		}
		// Dispatch the package to the correct queue
		incoming_queues.at(client_id)->produce(package);


		auto outgoing_package = outgoing_packages.consume();
		connector.send_package(outgoing_package.second, outgoing_package.first);
	}
}