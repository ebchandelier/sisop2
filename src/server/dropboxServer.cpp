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
		auto package_and_addr = connector.receive_next_package_and_addr();
		auto package = package_and_addr.first;
		auto addr = package_and_addr.second;
		printf("Received package from %d:\n%s\n", addr.sin_addr.s_addr, stringifier.stringify(package).c_str());

		if (package.type == datagram_type::control)
		{
			if (package.control.action == control_actions::request_login)
			{
				datagram login_response;
				login_response.type = datagram_type::control;
				login_response.control.action = control_actions::accept_login;
				printf("Sending package:\n%s\n", stringifier.stringify(login_response).c_str());
				connector.send_package(login_response, addr);
			}
			if (package.control.action == control_actions::request_logout)
			{
				datagram logout_response;
				logout_response.type = datagram_type::control;
				logout_response.control.action = control_actions::accept_logout;
				printf("Sending package:\n%s\n", stringifier.stringify(logout_response).c_str());
				connector.send_package(logout_response, addr);
			}
		}
	}
}