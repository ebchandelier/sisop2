#include "Server.h"

Server::Server(std::string local_ip, int port, std::string base_path) : outgoing_packages(MAXIMUM_PACKAGES_QUEUE_SIZE)
{
    this->port = port;
	this->work_path = base_path + "/sync_dir";

	this->is_leader = true;

	std::thread([&, port]() {
		DistributedServer(local_ip, port, &ipPortConnectedList, &shouldWarn, &threadCount, &elected, &fightingForElection, &is_leader).waitNewConnection();
    }).detach();
}

Server::Server(std::string local_ip, int port, std::string base_path, std::string ip, int portToConnect) : outgoing_packages(MAXIMUM_PACKAGES_QUEUE_SIZE)
{
	this->local_ip = local_ip;
	this->port = port;
	this->work_path = base_path + "/sync_dir";
	
	int currentSizeConnected = this->ipPortConnectedList.size();

	std::thread([&, port, ip, portToConnect]() {
		DistributedServer(local_ip, port, &ipPortConnectedList, &shouldWarn, &threadCount, &elected, &fightingForElection, &is_leader).connectWith(ip, portToConnect);
    }).detach();

	while(this->ipPortConnectedList.size()==currentSizeConnected);

	std::thread([&, port]() {
		DistributedServer(local_ip, port, &ipPortConnectedList, &shouldWarn, &threadCount, &elected, &fightingForElection, &is_leader).waitNewConnection();
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
			printf("Server is leader, forwarding packages to replcias:\n");
			// Send message to every replica

			for (auto replica: this->ipPortConnectedList)
			{
				printf("Master forwarding package to RM %s : %d\n", replica.ip, replica.portUDP);
				// Build forwarded package
				datagram forwarded_package = package;
				forwarded_package.is_from_master = true;
				forwarded_package.original_sender = addr;
				// Build replica sockaddr_in
				sockaddr_in replica_addr;
				replica_addr.sin_family = AF_INET;
				replica_addr.sin_port = htons(replica.portUDP);
				replica_addr.sin_addr.s_addr = inet_addr(replica.ip);
				// Send package
				connector.send_package(forwarded_package, replica_addr);
			}
		}
		else
		{
			// Use the addr information from the original sender, not the master
			printf("Server is not leader, using original sender information\n");
			addr = package.original_sender;
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
