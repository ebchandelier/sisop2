#include "./ClientConnectionManager.h"

int	ClientConnectionManager::login_server(char* username, char* host, int port)
{
    std::unique_lock<std::mutex> mlock(mutex);
    printf("\nConnection to server...\n");
    connector.connect(host, port);

    // Build login request package
    datagram login_request;
    login_request.type = datagram_type::control;
    login_request.control.action = control_actions::request_login;
    strncpy(login_request.control.login_request_data.username, username, MAXIMUM_USERNAME);
    

    connector.send_package(login_request);
    datagram response = connector.receive_package();

    printf("Got login response:\n%s\n", DatagramStringifier().stringify(response).c_str());
    if (response.type == datagram_type::control)
    {
        if (response.control.action == control_actions::accept_login)
        {
            printf("Login accepted\n");
            return 0;
        }
        else if (response.control.action == control_actions::deny_login)
        {
            printf("Login denied\n");
            return -1;
        }
    }
    printf("Unknown response\n");
    return -1;
}
void ClientConnectionManager::sync_client()
{
    std::unique_lock<std::mutex> mlock(mutex);
    // Try to send modified files since the last sync
    for (auto file : queue)
    {
        internal_send_file(file.c_str());
    }
    queue.clear();
    // And sync the state
    auto server_files = internal_sendListFilesRequest();
    resolve_diff(device_files, server_files);
}

void ClientConnectionManager::enqueue_file_update(char* file)
{
    queue.push_back(file);
}

void ClientConnectionManager::send_file(char* file)
{
    std::unique_lock<std::mutex> mlock(mutex);
    internal_send_file(file);
}

void ClientConnectionManager::internal_send_file(const char* file_name)
{
    printf("sending file %s\n", file_name);
    // Extract filename from path
    std::string path = std::string(work_dir) + "/" + std::string(file_name);
    
    // Read file
    auto packages = PersistenceFileManager().read(path);

    // Setup file_info
    file_info file__info;
    file__info.name = file_name;
    if (!device_files.has(file_name)) 
    {
        file__info.version = 1;
    }
    else
    {
        file__info.version = device_files.get(file_name).version;
    }

    // Send upload request
    datagram request;
    request.type = datagram_type::control;
    request.control.action = control_actions::request_upload;
    request.control.upload_request_data.version = file__info.version;
    strcpy(request.control.upload_request_data.filename, file_name);
    connector.send_package(request);

    auto response = connector.receive_package();
    if (response.type == datagram_type::control && response.control.action == control_actions::accept_upload)
    {
        for (auto package : packages)
        {
            std::cout << DatagramStringifier().stringify(package);
            connector.send_package(package);
        }
        device_files.set(file__info);
    }
}

DeviceFilesInfo ClientConnectionManager::internal_sendListFilesRequest()
{
    // Send upload request
    datagram request;
    request.type = datagram_type::control;
    request.control.action = control_actions::request_list_files;
    connector.send_package(request);

    auto response = connector.receive_package();
    return FileInfoVectorSerializer().deserialize(response.control.list_files_response.data);
}

DeviceFilesInfo ClientConnectionManager::sendListFilesRequest()
{
    std::unique_lock<std::mutex> mlock(mutex);
    return internal_sendListFilesRequest();
}

void ClientConnectionManager::get_file(char* file)
{
    std::unique_lock<std::mutex> mlock(mutex);
    internal_get_file(file);
}

void ClientConnectionManager::internal_get_file(char* file_name)
{
    printf("getting file %s\n", file_name);
    // Send download request
    datagram request;
    request.type = datagram_type::control;
    request.control.action = control_actions::request_download;
    strcpy(request.control.download_request_data.filename, file_name);
    connector.send_package(request);

    // Check response
    auto response = connector.receive_package();
    if (response.type == datagram_type::control)
    {
        // If ok
        if (response.control.action == control_actions::accept_download)
        {
            // Get file
            std::vector<datagram> packages;
            while(true)
            {
                auto package = connector.receive_package();
                packages.push_back(package);
                if (package.data.is_last)
                {
                    break;
                }
            }

            // Save
            PersistenceFileManager().write(work_dir + "/" + std::string(file_name), packages);
            file_info file;
            file.name = response.control.download_accept_data.filename;
            file.version = response.control.download_accept_data.version;
            device_files.set(file);
        }
        else if (response.control.action == control_actions::deny_download)
        {
            printf("Error on downloading file %s: %s\n", file_name, response.control.download_deny_data.message);
        }
    }
}

void ClientConnectionManager::delete_file(char* file)
{
    std::unique_lock<std::mutex> mlock(mutex);
    internal_delete_file(file);
}

void ClientConnectionManager::internal_delete_file(char* file)
{
    // Extract filename from path
    std::string file_name(file);
    auto last_slash = file_name.find_last_of("/");
    if (last_slash != std::string::npos)
    {
        file_name = file_name.substr(last_slash + 1);
    }

    // Send delete request
    datagram request;
    request.type = datagram_type::control;
    request.control.action = control_actions::request_exclude;
    strcpy(request.control.exclude_request_data.filename, file_name.c_str());
    connector.send_package(request);

    // Get response
    connector.receive_package();
}

int ClientConnectionManager::logout()
{
    std::unique_lock<std::mutex> mlock(mutex);
    printf("\nLogging out...\n");
    // Build logout request package
    datagram logout_request;
    logout_request.type = datagram_type::control;
    logout_request.control.action = control_actions::request_logout;

    connector.send_package(logout_request);
    datagram response = connector.receive_package();

    printf("Got logout response:\n%s\n", DatagramStringifier().stringify(response).c_str());
    if (response.type == datagram_type::control)
    {
        if (response.control.action == control_actions::accept_logout)
        {
            printf("Logout successful\n");
            return 0;
        }
    }
    printf("Unknown response\n");
    return -1;
}

void ClientConnectionManager::internal_delete_device_file(char* file)
{
    std::string path = work_dir + std::string("/") + std::string(file);
    int success = remove(path.c_str());
    if (success != 0) {
        printf("Failed to delete file\n");
    }
}

void ClientConnectionManager::resolve_diff(DeviceFilesInfo &client, DeviceFilesInfo &server)
{
    auto files_union = DeviceFilesInfo::common_files(client, server);
    for (const auto file : files_union)
    {
        auto has_client = client.has(file);
        auto has_server = server.has(file);

        if (has_client && !has_server)
        {
            internal_delete_device_file((char *)file.c_str());
        }
        if (!has_client && has_server)
        {
            internal_get_file((char *)file.c_str());
        }
        if (has_client && has_server)
        {
            auto client_file = client.get(file);
            auto server_file = server.get(file);
            printf("File %s is version %d on the client and version %d on the server\n", file.c_str(), client_file.version, server_file.version);
            if (client_file.version > server_file.version)
            {
                internal_send_file((char *)file.c_str());
            }
            if (client_file.version < server_file.version)
            {
                internal_get_file((char *)file.c_str());
                // Decrement version, so when we write it back, 
                // the inotify thread will increase the version again
                auto info = device_files.get(file);
                info.version--;
                device_files.set(info);
            }
            // Otherwise, the version is the same and nothing needs to be done
        }
    }

}
