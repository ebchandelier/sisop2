#include "DistributedServer.h"

#define BUFFERSIZE 1024

DistributedServer::DistributedServer(int port, std::vector<PROCESS_PATH> *ipPortConnectedList, std::vector<TYPE> *communicationVector, int *threadCount, int *elected, bool *fightingForElection) {

    std::unique_lock<std::mutex> mlock(mutex_constructor);

    this->basePort = port;

    this->threadCounter = threadCount;

    *this->threadCounter = *this->threadCounter + 1;

    this->port = port + *this->threadCounter;
    
    this->ipPortConnectListPointer = ipPortConnectedList;

    this->communicationVector = communicationVector;

    this->elected = elected;

    this->fightingForElection = fightingForElection;
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

TYPE buildCommandType(control_type type_to_set) {

    TYPE type;
    type.type = type_to_set;

    return type;
}

void pauseThread() { // master gambi

    while(true) {

        usleep(100000000);
    }
}

TYPE getNullCommandType() {

    TYPE type;
    type.type = control_type::nothing;

    return type;
}

bool isNullCommandTYPE(TYPE type) {

    return type.type == control_type::nothing;
}

void DistributedServer::setElected(int elected) {

    if(elected != *this->elected)
    {
        *this->elected = elected;
        std::cout << "Process " << getpid() << " electing process of pid: " << *this->elected << " to be the lider\n"; 
        if (is_leader != nullptr && getpid() == *this->elected)
        {
            // Warn server that the elected was changed            
            *is_leader = *this->elected;
        }
    }
}

void DistributedServer::warnEveryProcessAboutMyConnectedProcess(std::string ip, int basePort, int pid) {

    std::unique_lock<std::mutex> mlock(mutex_warn);

    for(int i=0; i < this->ipPortConnectListPointer->size(); i++) {

        this->communicationVector->at(i) = getNullCommandType();
    }

    for(int i=0; i < this->ipPortConnectListPointer->size()-1; i++) {

        TYPE type = buildCommandType(control_type::action_add_ip_port);
        type.ip_port = buildProcessPath(ip, basePort + i + 1, pid);

        this->communicationVector->at(i) = type;
    }
}

bool DistributedServer::contains(int pid) {

    for(auto process : *this->ipPortConnectListPointer) {

        if(process.pid == pid) return true;
    }

    return false;
}

void DistributedServer::startElection() {

    std::unique_lock<std::mutex> mlock(mutex_startElection);

    for(int i=0; i<this->communicationVector->size(); i++) {

        this->communicationVector->at(i) = buildCommandType(control_type::start_election);
    }
}

bool DistributedServer::writeWithError(int socket, TYPE *type) {

    int error_code;
    int error_code_size = sizeof(error_code);
    getsockopt(socket, SOL_SOCKET, SO_ERROR, &error_code, (socklen_t*)&error_code_size);

    if(error_code == 0) {

        write(socket, type, sizeof(*type));

        return true;
    }

    return false;
}

void DistributedServer::communicate(int socket, int indexAdded) {

    int timeout = 0;

    usleep(100000); // why do we need this?

    // When some process enters, it restarts the election, even when the lider is ok (bully)
    this->startElection();

    while(true) {

        std::unique_lock<std::mutex> mlock(mutex_communicate);

        usleep(100000);

        TYPE type_forTestingConnection = buildCommandType(control_type::nothing);
        
        if(!this->writeWithError(socket, &type_forTestingConnection)) {

            this->startElection();

            return;
        }

        if(!isNullCommandTYPE(this->communicationVector->at(indexAdded))) {
            
            TYPE type = this->communicationVector->at(indexAdded);
            this->communicationVector->at(indexAdded) = getNullCommandType();

            if(type.type == control_type::action_add_ip_port) {

                if(!this->writeWithError(socket, &type)) {

                    this->startElection();

                    return;
                }

            } else if(type.type == control_type::start_election) {

                // Sendo election to all the process with the pid greater than mine

                std::cout << "OKAY LETS SEND MSG TO START AN ALECTION\n";
                
                *this->fightingForElection = true;

                if(getpid() < this->ipPortConnectListPointer->at(indexAdded).pid) {

                    TYPE newType = buildCommandType(control_type::election);
                    
                    if(!this->writeWithError(socket, &newType)) {

                        this->startElection();

                        return;
                    }
                    
                }
                
                timeout = 100; // it should be something like 2 * (time to trans) +  1 * (time to process)
            }
        }

        if(timeout--== 0 && *this->fightingForElection) {

            TYPE coordinator = buildCommandType(control_type::coordinator);
            this->writeWithError(socket, &coordinator);

            std::cout << "timeout....\n";
            this->setElected(getpid());
        }
        
        TYPE type_response = getNullCommandType();
        read(socket, &type_response, sizeof(TYPE));
        
        if(type_response.type == control_type::action_add_ip_port) {

            if(!this->contains(type_response.ip_port.pid)) {

                std::unique_lock<std::mutex> mlock(mutex_add_thread);
                
                int currentSize = this->ipPortConnectListPointer->size();

                std::thread([&, type_response]() {
		            DistributedServer(basePort, ipPortConnectListPointer, communicationVector, threadCounter, elected, fightingForElection).connectWith(std::string(type_response.ip_port.ip), type_response.ip_port.port);
                }).detach();
                
                while(currentSize == this->ipPortConnectListPointer->size());
            } 
        
        } else if(type_response.type == control_type::election) {

            TYPE answer = buildCommandType(control_type::answer);
            this->writeWithError(socket, &answer); 

            if(!this->writeWithError(socket, &answer)) {

                this->startElection();

                return;
            }

        } else if(type_response.type == control_type::answer) {

            // std::cout << "RECEIVED ANSWER\n";

            *this->fightingForElection = false;

        } else if(type_response.type == control_type::coordinator) {

            this->setElected(this->ipPortConnectListPointer->at(indexAdded).pid);
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
    this->communicationVector->push_back(getNullCommandType());

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
        DistributedServer(basePort, ipPortConnectListPointer, communicationVector, threadCounter, elected, fightingForElection).waitNewConnection();
    }).detach();

    int indexAdded = this->addCommunication(std::string(anotherProcessPath.ip), anotherProcessPath.port, anotherProcessPath.pid);

    this->warnEveryProcessAboutMyConnectedProcess(std::string(anotherProcessPath.ip), anotherProcessPath.port, anotherProcessPath.pid);

    this->communicate(newsockfd, indexAdded);

    std::cout << "Terminate thread that started with waitNewConnection\n";

    pauseThread();
}

void DistributedServer::connectWith(std::string ip, int port) {

    while(true) { // Keep trying till it connects

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

        PROCESS_PATH thisProcessPath = buildProcessPath(ip, this->port, getpid()); // provavel treta no ip....
        write(sockfd, &thisProcessPath, sizeof(PROCESS_PATH));

        PROCESS_PATH anotherProcessPath;
        read(sockfd, &anotherProcessPath, sizeof(PROCESS_PATH));

        fcntl(sockfd, F_SETFL, O_NONBLOCK); /* Change the socket into non-blocking state	*/

        int indexAdded = this->addCommunication(anotherProcessPath.ip, anotherProcessPath.port, anotherProcessPath.pid);

        this->communicate(sockfd, indexAdded);

        std::cout << "Terminate thread that started with connectWith\n";

        pauseThread();
    }

}