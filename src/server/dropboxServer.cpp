#include "ServerConnectorUDP.h"
#include "../shared/DatagramStringifier.h"

int main(int argc, char **argv)
{
	printf("Initializing server...\n");
	int port;

	if(argc < 3) {
		  printf("How to use:\n");
		  printf("dropboxServer -p <port>\n");
		  exit(1);
	 }
    for(int i=1; i<argc; i++) {
		if(argv[i][0]=='-') {
			switch(argv[i][1]) {
				case 'p':
					i++;
					port = atoi(argv[i]);
					if(port < 1024) {
						printf("Invalid port value\n");
						exit(1);
					}
					break;
						  
				default:
					printf("Invalid parameter %d: %s\n", i, argv[i]);
					exit(1);
			}
		} else {
			printf("Invalid parameter %d: %s\n", i, argv[i]); 
			exit(1);
		}
	}
	
	printf("Initializing UDP stack...\n");
    ServerConnectorUDP connector = ServerConnectorUDP();
	connector.init(port);
	auto stringifier = DatagramStringifier();
	printf("Done. Waiting for packages:\n");
	while(true)
	{
		auto package = connector.receive_next_package();
		printf("Package received:\n%s\n", stringifier.stringify(package).c_str());

		if (package.type == datagram_type::control)
		{
			if (package.control.action == control_actions::request_login)
			{
				datagram login_response;
				login_response.type = datagram_type::control;
				login_response.control.action = control_actions::accept_login;
				connector.send_package(login_response);
			}
		}
	}
}