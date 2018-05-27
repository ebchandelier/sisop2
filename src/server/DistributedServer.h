#include<iostream>
#include<vector>

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h> 

#include<thread>

class DistributedServer
{
public:

    DistributedServer(int port, std::vector<std::pair<std::string, int>> *ipPortConnectedList, std::vector<std::pair<std::string, int>> *shouldWarn);
    void waitNewConnection();
    void connectWith(std::string ip, int port);
    // std::string receive(int socket, int bufferSize);
    void addCommunication(std::string ip, int port);
    void warnEveryProcessAboutMyConnectedProcess(std::string ip, int basePort);
    void communicate(int socket);

private:

    int port;
    std::vector<std::pair<std::string, int>> serversConnected;
    std::vector<std::pair<std::string, int>> *ipPortConnectListPointer;
    std::vector<std::pair<std::string, int>> *shouldWarn;

};

