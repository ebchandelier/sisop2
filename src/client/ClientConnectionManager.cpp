#include "./ClientConnectionManager.h"

int	ClientConnectionManager::login_server(char* host, int port)
{
    printf("Connection to server...\n");
    connector.connect(host, port);

    // Build login request package
    datagram login_request;
    login_request.type = datagram_type::control;
    login_request.control.action = control_actions::request_login;

    connector.send_package(login_request);
    datagram response = connector.receive_package();

    printf("Got login response:\n%s\n", DatagramStringifier().stringify(response).c_str());
    if (response.type == datagram_type::control)
    {
        if (response.control.action == control_actions::accept_login)
        {
            printf("Login accepted");
            return 0;
        }
        else if (response.control.action == control_actions::deny_login)
        {
            printf("Login denied");
            return -1;
        }
    }
    printf("Unknown response");
    return -1;
}
void ClientConnectionManager::sync_client()
{

}
void ClientConnectionManager::send_file(char* file)
{

}
void ClientConnectionManager::get_file(char* file)
{

}
void ClientConnectionManager::delete_file(char* file)
{

}
void ClientConnectionManager::close_session()
{

}
