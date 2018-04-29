#include "Server.h"

#define MAXIMUM_PACKAGES_QUEUE_SIZE 100


Server::Server(int port)
{
    this->port = port;
}

void Server::run()
{

	std::map<uint32_t, ClientHandler> handlers;
	std::map<uint32_t, ThreadSafeQueue<datagram>*> incoming_queues;
	ThreadSafeQueue<std::pair<sockaddr_in, datagram>> outgoing_packages(MAXIMUM_PACKAGES_QUEUE_SIZE);

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

		auto outgoing_package_info = outgoing_packages.consume();
		printf("Sending package to %d:\n%s\n", outgoing_package_info.first.sin_addr.s_addr, stringifier.stringify(outgoing_package_info.second).c_str());		
		connector.send_package(outgoing_package_info.second, outgoing_package_info.first);
	}
}