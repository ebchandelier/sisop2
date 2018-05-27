#include "DistributedServer.h"

#define BUFFERSIZE 1024

typedef struct {
    char ip[15];
    int port;
} IP_PORT;

DistributedServer::DistributedServer(int port, std::vector<std::pair<std::string, int>> *ipPortConnectedList, std::vector<std::pair<std::string, int>> *shouldWarn) {

    this->port = port;
    
    this->ipPortConnectListPointer = ipPortConnectedList;

    this->shouldWarn = shouldWarn;
}

void DistributedServer::warnEveryProcessAboutMyConnectedProcess(std::string ip, int basePort) {

    for(int i=1; i <= this->ipPortConnectListPointer->size(); i++) {

        this->shouldWarn->push_back(std::make_pair(ip, basePort + i));
    }
}
void DistributedServer::communicate(int socket) {


    std::cout << "starting communication\n";

    while(true) {

        if(this->shouldWarn->size() > 0) {

            std::pair<std::string, int> ipPort = this->shouldWarn->at(0);
            this->shouldWarn->erase(this->shouldWarn->begin(), this->shouldWarn->begin());

            //sendo ip, port, to connect
        }

        // std::string received = receive(socket);
        //do what?
        //send_string // lógica do guloso
    }
}

void DistributedServer::addCommunication(std::string ip, int port) {

    std::pair<std::string, int> pair = std::make_pair(ip, port);
    
    this->ipPortConnectListPointer->push_back(pair);

    std::cout << "\n\nAdded comunication, now talking with:\n";
    for(auto pair : *this->ipPortConnectListPointer) {

        std::cout << "ip: " << pair.first << ", port: " << pair.second << "\n";
    }
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

    std::cout << "Accepted connection\n";
	
    IP_PORT ip_port;
    char buffer[sizeof(ip_port)];
    read(newsockfd, buffer, sizeof(ip_port));
    memcpy(&ip_port, buffer, sizeof(ip_port));

    int port = this->port;
    std::vector<std::pair<std::string, int>> *ipPortConnectListPointer = this->ipPortConnectListPointer;
    std::vector<std::pair<std::string, int>> *shouldWarnPointer = this->shouldWarn;

    std::thread([&, port, ipPortConnectListPointer]() {
        DistributedServer(port+1, ipPortConnectListPointer, shouldWarnPointer).waitNewConnection();
    }).detach();


    this->warnEveryProcessAboutMyConnectedProcess(std::string(ip_port.ip), ip_port.port);

    this->addCommunication(std::string(ip_port.ip), ip_port.port);

    communicate(newsockfd);
}

void DistributedServer::connectWith(std::string ip, int port) {

    std::cout << "Connecting With " << ip << " " << port << "\n";

    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
	
	server = gethostbyname(ip.c_str());
	if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
        printf("ERROR opening socket\n");
    
	serv_addr.sin_family = AF_INET;     
	serv_addr.sin_port = htons(port);    
	serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
	bzero(&(serv_addr.sin_zero), 8);     
    
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        printf("ERROR connecting\n");

    std::cout << "Connected with " << ip << " " << port << "\n";

    IP_PORT ip_port;
    memcpy(ip_port.ip, (ip+"     ").c_str(), 15);//corrigir o ip
    ip_port.port = this->port;


    char buffer[sizeof(ip_port)];
    memccpy(buffer, (void*)&ip_port, 1,  sizeof(ip_port));
    write(sockfd, buffer, sizeof(ip_port));

    this->addCommunication(ip, port);

    communicate(sockfd);
}