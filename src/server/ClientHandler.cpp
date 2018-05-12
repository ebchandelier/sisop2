#include "ClientHandler.h"
#include "../shared/ListFiles.cpp"

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
				this->user_path = this->base_path + "/" + this->username;
				std::string command = std::string("mkdir -p ") + this->user_path;
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

				working_file_name = package.control.file.filename;
				outgoing_packages->produce(response);
				buffer.clear();
				state = ClientHandlerState::receiving_file;
			}
			if (package.control.action == control_actions::request_download)
			{
				working_file_name = package.control.file.filename;
				auto packages = PersistenceFileManager().read(this->user_path + "/" + working_file_name);
				for (auto package : packages)
				{
					outgoing_packages->produce(package);
				}
			}
			if (package.control.action == control_actions::request_list_files)
			{
  				std::vector<file_info> files = ListFiles::listFilesAt(this->user_path);
				
				// SERIALIZATION?
				datagram packages;//
				
				outgoing_packages->produce(package);
			}
		}
		else if (package.type == datagram_type::data)
		{
			buffer.push_back(package);
			if (package.data.is_last)
			{
				// Write file to disk
				auto full_file_path = this->user_path + "/" + working_file_name;
				PersistenceFileManager().write(full_file_path, buffer);
				buffer.clear();

				// Add file to list
				file_info file;
				strcpy(file.name, working_file_name.c_str());
				files.push_back(file);
			}
		}
    }
}