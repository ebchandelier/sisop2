#include <vector>
#include <string>
#include "Client.h"


int main(int argc, char **argv)
{
	std::string user, dir;
	std::vector<rm> servers;
	rm server;
	if(argc < 8) 
	{
		printf("How to use:\n");
		printf("dropboxClient -u <user> -d <base dir> -s <rm ip> <rm port>\n");
		exit(1);
	}
	for(int i = 1; i < argc; i++) 
	{
		if(argv[i][0]=='-') 
		{
			switch(argv[i][1]) 
			{
				case 's':
					i++;
					server.ip = argv[i];
					i++;
					server.port = atoi(argv[i]);
					if(server.port < 1024) 
					{
						printf("Invalid port value\n");
						exit(1);
					}
					servers.push_back(server);
				break;

				case 'd':
					i++;
					dir = argv[i];
				break;

				case 'u':
					i++;
					user = argv[i];
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
	Client().main(user, dir, servers);
	return 0;
}

