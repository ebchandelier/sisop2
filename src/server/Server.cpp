#include "Server.h"

Server::Server(int port, std::string base_path) : outgoing_packages(MAXIMUM_PACKAGES_QUEUE_SIZE)
{
    this->port = port;
	this->work_path = base_path + "/sync_dir";

	std::thread([&, port]() {
		auto distributed_server = DistributedServer(port, &ipPortConnectedList, &shouldWarn, &threadCount, &elected, &fightingForElection);
		distributed_server.is_leader = &is_leader;
		distributed_server.waitNewConnection();
    }).detach();
}

Server::Server(int port, std::string base_path, std::string ip, int portToConnect) : outgoing_packages(MAXIMUM_PACKAGES_QUEUE_SIZE)
{
	this->port = port;
	this->work_path = base_path + "/sync_dir";
	
	int currentSizeConnected = this->ipPortConnectedList.size();

	std::thread([&, port, ip, portToConnect]() {
		DistributedServer(port, &ipPortConnectedList, &shouldWarn, &threadCount, &elected, &fightingForElection).connectWith(ip, portToConnect);
    }).detach();

	while(this->ipPortConnectedList.size()==currentSizeConnected);

	std::thread([&, port]() {
		DistributedServer(port, &ipPortConnectedList, &shouldWarn, &threadCount, &elected, &fightingForElection).waitNewConnection();
    }).detach();
}

void Server::run()
{
	create_root_folder_if_needed();
	connector.init(port);
	while(true)
	{
		try
		{
			process_incoming_message();
			process_outgoing_message();
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

void Server::process_incoming_message()
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
		
		if (is_leader)
		{
			// Send message to every replica
			for (auto& replica: ipPortConnectedList)
			{
				printf("TODO: Sending package to replica %s : %d\n", replica.ip, replica.port);
			}
		}
		
		// If this is a package from a new device
		if (handlers.count(client_id) == 0)
		{
			std::string user = std::string(package.control.login_request_data.username);
			DeviceFilesInfo* device_files;
			if (users_files.count(user) == 0)
			{
				// This is the first device from this user
				device_files = new DeviceFilesInfo();
				users_files[user] = device_files;			
			}
			else
			{
				// Not the first device, fetch files info data
				device_files = users_files.at(user);
			}
			// Create a queue for future incoming messages
			incoming_queues.emplace(client_id, new ThreadSafeQueue<datagram>(MAXIMUM_PACKAGES_QUEUE_SIZE));
			// Create a ClientHandler
			handlers.emplace(client_id, ClientHandler(this->work_path, device_files, incoming_queues.at(client_id), new OutgoingPackages(addr, &outgoing_packages)));
			// And run the handler in a new thread
			std::thread([&]() {
				handlers.at(client_id).run();
			}).detach();
		}
		// Dispatch the package to the correct queue
		incoming_queues.at(client_id)->produce(package);
	}

}
void Server::process_outgoing_message()
{
	// If one of the clientHandler has produced a package to be sent
	if (!outgoing_packages.is_empty())
	{
		// Fetch it
		auto outgoing_package_info = outgoing_packages.consume();
		printf("Sending package to %d:\n%s\n", outgoing_package_info.first.sin_addr.s_addr, stringifier.stringify(outgoing_package_info.second).c_str());		
		// And send responsel only if its the leader
		if (is_leader)
		{
			connector.send_package(outgoing_package_info.second, outgoing_package_info.first);
		}
	}
}
