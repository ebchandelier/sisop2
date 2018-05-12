#include "Server.h"

Server::Server(int port, std::string base_path) : outgoing_packages(MAXIMUM_PACKAGES_QUEUE_SIZE)
{
    this->port = port;
	this->work_path = base_path + "/sync_dir";
}

void Server::run()
{
	create_root_folder_if_needed();
	connector.init(port);
	while(true)
	{
		// We process, at most, one incoming message and one outgoing message at a time,
		// so if both buffers have data, the download/upload ratio will be 50/50.
		try
		{
			process_incoming_messages();
			process_outgoing_messages();
		} 
		catch (std::exception e) 
		{
			std::cout << "Error: " << e.what();
		}
	}
}

void Server::create_root_folder_if_needed()
{
	std::string command = "mkdir -p " + this->work_path;
    system(command.c_str());
}

void Server::process_incoming_messages()
{
	// If the connector has a new package available
	if (connector.has_new_package()) 
	{
		// Fetch it
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
			handlers.emplace(client_id, ClientHandler(this->work_path, incoming_queues.at(client_id), new OutgoingPackages(addr, &outgoing_packages)));
			// And run the handler in a new thread
			std::thread([&]() {
				handlers.at(client_id).run();
			}).detach();
		}
		// Dispatch the package to the correct queue
		incoming_queues.at(client_id)->produce(package);
	}
}
void Server::process_outgoing_messages()
{
	// If one of the clientHandler has produced a package to be sent
	if (!outgoing_packages.is_empty())
	{
		// Fetch it
		auto outgoing_package_info = outgoing_packages.consume();
		printf("Sending package to %d:\n%s\n", outgoing_package_info.first.sin_addr.s_addr, stringifier.stringify(outgoing_package_info.second).c_str());		
		// And send
		connector.send_package(outgoing_package_info.second, outgoing_package_info.first);
	}
}
