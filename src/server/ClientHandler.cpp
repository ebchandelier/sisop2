#include "ClientHandler.h"

ClientHandler::ClientHandler(ThreadSafeQueue<datagram>* incoming_packages, OutgoingPackages* outgoing_packages)
{
    this->incoming_packages = incoming_packages;
    this->outgoing_packages = outgoing_packages;
}

void ClientHandler::run()
{
    while(true)
    {
        datagram package = incoming_packages->consume();
		if (package.type == datagram_type::control)
		{
			if (package.control.action == control_actions::request_login)
			{
				datagram login_response;
				login_response.type = datagram_type::control;
				login_response.control.action = control_actions::accept_login;
				printf("Sending package:\n%s\n", stringifier.stringify(login_response).c_str());
                outgoing_packages->produce(login_response);
				//connector.send_package(login_response, addr);
			}
			if (package.control.action == control_actions::request_logout)
			{
				datagram logout_response;
				logout_response.type = datagram_type::control;
				logout_response.control.action = control_actions::accept_logout;
				printf("Sending package:\n%s\n", stringifier.stringify(logout_response).c_str());
                outgoing_packages->produce(logout_response);
				//connector.send_package(logout_response, addr);
			}
		}
    }
}