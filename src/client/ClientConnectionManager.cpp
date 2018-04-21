#include "./ClientConnectionManager.h"

int	ClientConnectionManager::login_server(char* host, int port)
{
    printf("Connection to server...\n");
    connector.connect(host, port);
    // TODO: Send login request package
    connector.send_package();
    return 0;
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