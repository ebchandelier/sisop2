#include "DistributedServer.h"

#define BUFFERSIZE 1024

DistributedServer::DistributedServer(int port, std::vector<PROCESS_PATH> *ipPortConnectedList, std::vector<PROCESS_PATH> *shouldWarn, int *threadCount) {

    std::unique_lock<std::mutex> mlock(mutex_constructor);

    this->basePort = port;

    this->threadCounter = threadCount;

    *this->threadCounter = *this->threadCounter + 1;

    this->port = port + *this->threadCounter;
    
    this->ipPortConnectListPointer = ipPortConnectedList;

    this->shouldWarn = shouldWarn;
}

PROCESS_PATH getNullPath() {

    PROCESS_PATH pp;
    pp.pid = -1;

    return pp;
}

bool isNullPath(PROCESS_PATH pp) {

    if(pp.pid == -1) return true;

    return false;
}

PROCESS_PATH buildProcessPath(std::string ip, int port, int pid) {

    PROCESS_PATH pp;

    memcpy(pp.ip, (ip+"            ").c_str(), 15);
    pp.port = port;
    pp.pid = pid;

    return pp;
}

void DistributedServer::warnEveryProcessAboutMyConnectedProcess(std::string ip, int basePort, int pid) {

    std::unique_lock<std::mutex> mlock(mutex_warn);

    for(int i=0; i < this->ipPortConnectListPointer->size(); i++) {

        this->shouldWarn->at(i) = getNullPath();
    }

    for(int i=0; i < this->ipPortConnectListPointer->size()-1; i++) {

        this->shouldWarn->at(i) = buildProcessPath(ip, basePort + i + 1, pid);
    }
}

bool DistributedServer::contains(int pid) {

    for(auto process : *this->ipPortConnectListPointer) {

        if(process.pid == pid) return true;
    }

    return false;
}

void DistributedServer::communicate(int socket, int indexAdded) {

    // std::cout << "starting communication on socket " << socket << " at index " << indexAdded << "\n";

    while(true) {

        usleep(1000000);

        // std::cout << socket << " " << indexAdded << "\n";

        if(!isNullPath(this->shouldWarn->at(indexAdded))) {
        
            PROCESS_PATH ipPort = this->shouldWarn->at(indexAdded);
            this->shouldWarn->at(indexAdded) = getNullPath();

            TYPE type;
            type.type = control_type::action_add_ip_port;
            type.ip_port = ipPort;

            // std::cout << "MANDANDO MSG PRA INDEX: " << indexAdded << ", ip:" << type.ip_port.ip << " port:" << type.ip_port.port << "\n";
            write(socket, &type, sizeof(type));
        }

        TYPE type_response;
        memset(&type_response, -1, sizeof(TYPE));
        read(socket, &type_response, sizeof(TYPE));

        if(type_response.type == control_type::action_add_ip_port) {

            // std::cout << "OKAY RECEBEU MSG PARA ADICIONAR, " << type_response.ip_port.ip << " " << type_response.ip_port.port << " " << type_response.ip_port.pid << "\n";

            if(!this->contains(type_response.ip_port.pid)) {

                std::unique_lock<std::mutex> mlock(mutex_add_thread);
                
                int currentSize = this->ipPortConnectListPointer->size();

                std::thread([&, type_response]() {
		            DistributedServer(basePort, ipPortConnectListPointer, shouldWarn, threadCounter).connectWith(std::string(type_response.ip_port.ip), type_response.ip_port.port);
                }).detach();
                
                while(currentSize == this->ipPortConnectListPointer->size());
            } 
        
        } else if(type_response.type == control_type::ip_port_added) {

            // std::cout << "RECEIVED OKAY, ADDED.\n";
        } 
    }
}

std::string cleanUpIp(std::string ip) {

    std::string result = "";

    for(int i=0; i<ip.size(); i++) {

        if(ip[i] != ' ') {

            result += ip[i];
        }
    }

    return result;
}

int DistributedServer::addCommunication(std::string ip, int port, int pid) {

    std::unique_lock<std::mutex> mlock(mutex_add_communication);

    ip = cleanUpIp(ip);

    PROCESS_PATH processPath = buildProcessPath(ip, port, pid);

    this->ipPortConnectListPointer->push_back(processPath);
    this->shouldWarn->push_back(getNullPath());

    std::cout << "\n\nAdded comunication, now talking with:\n";
    for(auto processPath : *this->ipPortConnectListPointer) {

        std::cout << "ip: " << processPath.ip << ", port: " << processPath.port << " pid:" << processPath.pid << "\n";
    }

    return this->ipPortConnectListPointer->size()-1; //index in which it was added 
}

void DistributedServer::waitNewConnection() {

    int sockfd, newsockfd, n;
	socklen_t clilen;

	struct sockaddr_in serv_addr, cli_addr;
	
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
        printf("ERROR opening socket");
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(this->port);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);     
    
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
		printf("ERROR on binding");
	
	listen(sockfd, 5);

    std::cout << "Listening on port: " << this->port << "\n";
	
	clilen = sizeof(struct sockaddr_in);
	if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) == -1) 
		printf("ERROR on accept");


    PROCESS_PATH anotherProcessPath;
    read(newsockfd, &anotherProcessPath, sizeof(PROCESS_PATH));

    PROCESS_PATH thisProcessPath = buildProcessPath("YOU KNOW", 666, getpid()); // send just the pid
    write(newsockfd, &thisProcessPath, sizeof(PROCESS_PATH));

    fcntl(sockfd, F_SETFL, O_NONBLOCK); /* Change the socket into non-blocking state	*/
    fcntl(newsockfd, F_SETFL, O_NONBLOCK); /* Change the socket into non-blocking state	*/
    
    std::thread([&]() {
        DistributedServer(basePort, ipPortConnectListPointer, shouldWarn, threadCounter).waitNewConnection();
    }).detach();

    int indexAdded = this->addCommunication(std::string(anotherProcessPath.ip), anotherProcessPath.port, anotherProcessPath.pid);

    this->warnEveryProcessAboutMyConnectedProcess(std::string(anotherProcessPath.ip), anotherProcessPath.port, anotherProcessPath.pid);

    this->communicate(newsockfd, indexAdded);
}

void DistributedServer::connectWith(std::string ip, int port) {

    while(true) { // Keep trying till it get on communicate loop

        std::cout << "Trying to connect with " << ip << " " << port << "\n";

        usleep(200000); // trying not to kill my leptop

        ip = cleanUpIp(ip);

        int sockfd, n;
        struct sockaddr_in serv_addr;
        struct hostent *server;
        
        server = gethostbyname(ip.c_str());
        if (server == NULL) {
            fprintf(stderr,"ERROR, no such host\n");
            continue;
        }
        
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {

            printf("ERROR opening socket\n");
            continue;
        }
        
        serv_addr.sin_family = AF_INET;     
        serv_addr.sin_port = htons(port);    
        serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
        bzero(&(serv_addr.sin_zero), 8);     
        
        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {

            printf("ERROR connecting\n");
            continue;
        }

        // std::cout << "Connected with " << ip << " " << port << "\n";
        
        PROCESS_PATH thisProcessPath = buildProcessPath(ip, this->port, getpid()); // provavel treta no ip....
        write(sockfd, &thisProcessPath, sizeof(PROCESS_PATH));

        PROCESS_PATH anotherProcessPath;
        read(sockfd, &anotherProcessPath, sizeof(PROCESS_PATH));

        fcntl(sockfd, F_SETFL, O_NONBLOCK); /* Change the socket into non-blocking state	*/

        int indexAdded = this->addCommunication(ip, port, anotherProcessPath.pid);

        this->communicate(sockfd, indexAdded);
    }
}