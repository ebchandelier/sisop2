#include "ClientHandler.h"

ClientHandler::ClientHandler(std::string base_path, ThreadSafeQueue<datagram>* incoming_packages, OutgoingPackages* outgoing_packages)
{
    this->incoming_packages = incoming_packages;
    this->outgoing_packages = outgoing_packages;
	this->base_path = base_path;
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
  				auto files = ListFiles::listFilesAt(this->user_path);				
				datagram response;
				response.type = datagram_type::control;
				response.control.action = control_actions::accept_list_files;
				FileInfoVectorSerializer().serialize(response.control.list_files_response.data, files);
				outgoing_packages->produce(response);
			}
			if (package.control.action == control_actions::request_exclude)
			{
				// Delete file
				auto file_path = this->user_path + "/" + std::string(package.control.file.filename);
				auto success = remove(file_path.c_str());
				if (success != 0)
				{
					printf("Failed to delete file\n");
					// TODO: handle
				}
				// Accept
				datagram response;
				response.type = datagram_type::control;
				response.control.action = control_actions::accept_exclude;
				outgoing_packages->produce(response);
			}
			if (package.control.action == control_actions::request_sync_dir)
			{
				// Confirm Sync
				auto files = ListFiles::listFilesAt(this->user_path);
				datagram response;
				response.type = datagram_type::control;
				response.control.action = control_actions::accept_sync_dir;
				response.control.sync_dir_response.files_count = files.size();
				outgoing_packages->produce(response);
				for (auto file : files)
				{
					// Request upload for file
					datagram upload_request;
					upload_request.type = datagram_type::control;
					upload_request.control.action = control_actions::request_upload;
					strcpy(upload_request.control.file.filename, file.name.c_str());
					outgoing_packages->produce(upload_request);

					auto upload_request_response = incoming_packages->consume();
					// TODO: Check response

					auto packages = PersistenceFileManager().read(this->user_path + "/" + file.name);
					for (auto package : packages)
					{
						outgoing_packages->produce(package);
					}
				}
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
				file.name = working_file_name;
				files.push_back(file);
			}
		}
    }
}