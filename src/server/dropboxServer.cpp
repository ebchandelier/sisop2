#include "Server.h"

int main(int argc, char **argv)
{
	printf("Initializing server...\n");
	int port;
	std::string local_ip;
	std::string dir;
	std::string serverIpOfOneOfTheGroups = "NOPE";
	int serverPort = -1;

	if(argc < 7) 
	{
		printf("How to use:\n");
		printf("dropboxServer -i <this machine's local ip> -p <port> -d <working dir> -s <ip of another server> <port of that server>\n");
		exit(1);
	}
    for(int i = 1; i < argc; i++) 
	{
		if(argv[i][0]=='-') 
		{
			switch(argv[i][1]) 
			{
				case 'i':
					i++;
					local_ip = argv[i];
				break;
				case 'p':
					i++;
					port = atoi(argv[i]);
					if(port < 1024) 
					{
						printf("Invalid port value\n");
						exit(1);
					}
				break;

				case 'd':
					i++;
					dir = argv[i];
				break;

				case 's':
					i++;
					serverIpOfOneOfTheGroups = argv[i];
					i++;
					serverPort = atoi(argv[i]);
				break;

				default:
					printf("Invalid parameter %d: %s\n", i, argv[i]);
					exit(1);
			}
		} else 
		{
			printf("Invalid parameter %d: %s\n", i, argv[i]); 
			exit(1);
		}
	}
	
	if(serverIpOfOneOfTheGroups == "NOPE" || serverPort == -1) {
		
		Server(local_ip, port, dir).run();

	} else {

		Server(local_ip, port, dir, serverIpOfOneOfTheGroups, serverPort).run();
	}
}