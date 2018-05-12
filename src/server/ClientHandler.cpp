#include "ClientHandler.h"

ClientHandler::ClientHandler(std::string base_path, ThreadSafeQueue<datagram>* incoming_packages, OutgoingPackages* outgoing_packages)
{
    this->incoming_packages = incoming_packages;
    this->outgoing_packages = outgoing_packages;
	this->base_path = base_path;
	printf("ClientHandler path is %s\n", base_path.c_str());
	
}

void ClientHandler::run()
{
    while(true)
    {
        datagram package = incoming_packages->consume();
		if (package.type == datagram_type::control)
		{
			if (package.control.action == control_actions::request_login)
			{
				datagram login_response;
				login_response.type = datagram_type::control;
				login_response.control.action = control_actions::accept_login;

				// Create folder for this user
				this->username = std::string(package.control.login_request_data.username);
				std::string command = std::string("mkdir -p ") + this->base_path + "/" + this->username;
				//std::cout << "command: " << command;
    			system(command.c_str());

				// Send confirmation
                outgoing_packages->produce(login_response);
			}
			if (package.control.action == control_actions::request_logout)
			{
				datagram logout_response;
				logout_response.type = datagram_type::control;
				logout_response.control.action = control_actions::accept_logout;
                outgoing_packages->produce(logout_response);
			}
			if (package.control.action == control_actions::request_upload)
			{
				datagram response;
				response.type = datagram_type::control;
				response.control.action = control_actions::accept_upload;
				outgoing_packages->produce(response);
				buffer.clear();
				state = ClientHandlerState::receiving_file;
			}
		}
		else if (package.type == datagram_type::data)
		{
			buffer.push_back(package);
			if (package.data.is_last)
			{
				PersistenceFileManager().write("FILENAME", buffer);
				buffer.clear();
			}
		}
    }
}