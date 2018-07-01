#include <iostream>
#include <vector>
#include <string>
#include <mutex>

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
#include <fcntl.h>

#include <thread>

#include "./../shared/datagram.h"

class DistributedServer
{
public:

    DistributedServer(std::string local_ip, int port, std::vector<PROCESS_PATH> *ipPortConnectedList, std::vector<TYPE> *communicationVector, int *threadCount, int *elected, bool *fightingForElection, bool *is_leader);
    void waitNewConnection();
    void connectWith(std::string ip, int port);
    // std::string receive(int socket, int bufferSize);
    int addCommunication(std::string ip, int port, int pid);
    void warnEveryProcessAboutMyConnectedProcess(std::string ip, int basePort, int pid);
    void communicate(int socket, int indexAdded);
    bool contains(int pid);
    void startElection();
    bool writeWithError(int socket, TYPE *type);
    void setElected(int elected);

    bool *is_leader = nullptr;

private:
    std::string local_ip;
    int port;
    int basePort;
    std::vector<PROCESS_PATH> *ipPortConnectListPointer;
    std::vector<TYPE> *communicationVector;
    int *threadCounter;
    int *elected;
    bool *fightingForElection;

    std::mutex mutex_constructor;
    std::mutex mutex_connect;
    std::mutex mutex_warn;
    std::mutex mutex_send;
    std::mutex mutex_add_communication;
    std::mutex mutex_add_thread;
    std::mutex mutex_communicate;
    std::mutex mutex_startElection;
};

