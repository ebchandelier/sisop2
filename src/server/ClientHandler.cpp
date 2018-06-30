#include "ClientHandler.h"

ClientHandler::ClientHandler(
	std::string base_path, 
	DeviceFilesInfo* device_files,
	ThreadSafeQueue<datagram>* incoming_packages, 
	OutgoingPackages* outgoing_packages)
{
    this->incoming_packages = incoming_packages;
    this->outgoing_packages = outgoing_packages;
	this->base_path = base_path;
	this->device_files = device_files;
}

void ClientHandler::run()
{
    while(true)
    {
		try
		{
			// This lock could be more granular
			//device_files->mutex.lock();
			internal_handle();
			//device_files->mutex.unlock();
		}
		catch (std::exception e)
		{
			std::cout << "ClientHandler error: " << e.what() << std::endl;
			//device_files->mutex.unlock();
		}
	}
}


void ClientHandler::internal_handle()
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
			// Read file info
			upload_fileinfo.name = package.control.upload_request_data.filename;
			upload_fileinfo.version = package.control.upload_request_data.version;
			printf("Receiving filresponsee %s version %d\n", upload_fileinfo.name.c_str(), upload_fileinfo.version);

			// Only accept if version is file is new or version is higher
			if (device_files->has(upload_fileinfo.name) && (device_files->get(upload_fileinfo.name).version >= upload_fileinfo.version))
			{
				// Deny
				datagram response;
				response.type = datagram_type::control;
				response.control.action = control_actions::deny_upload;
				outgoing_packages->produce(response);
			}
			else
			{
				// Prepare state
				buffer.clear();
				state = ClientHandlerState::receiving_file;

				// Accept upload
				datagram response;
				response.type = datagram_type::control;
				response.control.action = control_actions::accept_upload;
				outgoing_packages->produce(response);
			}
		}
		if (package.control.action == control_actions::request_download)
		{
			// Check if the file is valid
			if (!device_files->has(package.control.download_request_data.filename))
			{
				// Deny if not
				datagram deny_request;
				deny_request.type = datagram_type::control;
				deny_request.control.action = control_actions::deny_download;
				strcpy(deny_request.control.download_deny_data.message, "File does not exits");
				outgoing_packages->produce(deny_request);
				return;
			}

			// Read file from disk
			auto file = device_files->get(package.control.download_request_data.filename);
			auto packages = PersistenceFileManager().read(this->user_path + "/" + file.name);

			// Accept download request
			datagram accept_request;
			accept_request.type = datagram_type::control;
			accept_request.control.action = control_actions::accept_download;
			strcpy(accept_request.control.download_accept_data.filename, file.name.c_str());
			accept_request.control.download_accept_data.version = file.version;
			outgoing_packages->produce(accept_request);

			// Send file
			for (auto package : packages)
			{
				outgoing_packages->produce(package);
			}
		}
		if (package.control.action == control_actions::request_list_files)
		{
			datagram response;
			response.type = datagram_type::control;
			response.control.action = control_actions::accept_list_files;
			FileInfoVectorSerializer().serialize(response.control.list_files_response.data, *device_files);
			outgoing_packages->produce(response);
		}
		if (package.control.action == control_actions::request_exclude)
		{
			// Delete file
			auto file_path = this->user_path + "/" + std::string(package.control.exclude_request_data.filename);
			auto success = remove(file_path.c_str());
			if (success != 0)
			{
				printf("Failed to delete file\n");
				datagram response;
				response.type = datagram_type::control;
				response.control.action = control_actions::deny_exclude;
				outgoing_packages->produce(response);
			}
			else
			{
				// Remove from device files
				device_files->remove(package.control.exclude_request_data.filename);
				
				// Send response
				datagram response;
				response.type = datagram_type::control;
				response.control.action = control_actions::accept_exclude;
				outgoing_packages->produce(response);
			}
		}
	}
	else if (package.type == datagram_type::data)
	{
		buffer.push_back(package);
		if (package.data.is_last)
		{
			// Write file to disk
			auto full_file_path = this->user_path + "/" + upload_fileinfo.name;
			PersistenceFileManager().write(full_file_path, buffer);

			// Add file to the set
			device_files->set(upload_fileinfo);

			// Clear state
			buffer.clear();
			state = ClientHandlerState::other;
		}
	}
}

