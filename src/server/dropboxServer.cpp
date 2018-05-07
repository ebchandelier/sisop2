#include "Server.h"

int main(int argc, char **argv)
{
	printf("Initializing server...\n");
	int port;

	if(argc < 3) 
	{
		printf("How to use:\n");
		printf("dropboxServer -p <port>\n");
		exit(1);
	}
    for(int i = 1; i < argc; i++) 
	{
		if(argv[i][0]=='-') 
		{
			switch(argv[i][1]) 
			{
				case 'p':
					i++;
					port = atoi(argv[i]);
					if(port < 1024) 
					{
						printf("Invalid port value\n");
						exit(1);
					}
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

	Server(port).run();
}