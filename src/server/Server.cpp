#include "Server.h"



Server::Server(int port) : outgoing_packages(MAXIMUM_PACKAGES_QUEUE_SIZE)
{
    this->port = port;
}

void Server::run()
{
	connector.init(port);
	while(true)
	{
		process_incoming_messages();
		process_outgoing_messages();
	}
}

void Server::process_incoming_messages()
{
	// TODO: Check if it has a package pending
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
		std::thread([&]() {
			handlers.at(client_id).run();
		}).detach();
	}
	// Dispatch the package to the correct queue
	incoming_queues.at(client_id)->produce(package);
}
void Server::process_outgoing_messages()
{
	// TODO: Uncomment once the incoming_messages checks if it has any data before blocking
	//if (!outgoing_packages.is_empty())
	{
		auto outgoing_package_info = outgoing_packages.consume();
		printf("Sending package to %d:\n%s\n", outgoing_package_info.first.sin_addr.s_addr, stringifier.stringify(outgoing_package_info.second).c_str());		
		connector.send_package(outgoing_package_info.second, outgoing_package_info.first);
	}
}
