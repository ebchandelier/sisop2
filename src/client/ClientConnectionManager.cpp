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
}
void ClientConnectionManager::send_file(char* file)
{
    std::unique_lock<std::mutex> mlock(mutex);
    auto packages = PersistenceFileManager().read(file);
    for (auto package : packages)
    {
        std::cout << DatagramStringifier().stringify(package);
        connector.send_package(package);
        //auto response = connector.receive_package();
        // TODO: Send ack
    }
}
void ClientConnectionManager::get_file(char* file)
{
    std::unique_lock<std::mutex> mlock(mutex);
}
void ClientConnectionManager::delete_file(char* file)
{
    std::unique_lock<std::mutex> mlock(mutex);
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

