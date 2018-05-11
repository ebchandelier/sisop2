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
				this->username = login_response.control.login_request_data.username;
                outgoing_packages->produce(login_response);
			}
			if (package.control.action == control_actions::request_logout)
			{
				datagram logout_response;
				logout_response.type = datagram_type::control;
				logout_response.control.action = control_actions::accept_logout;
                outgoing_packages->produce(logout_response);
			}
			if (package.control.action == control_actions::request_upload)
			{
				datagram response;
				response.type = datagram_type::control;
				response.control.action = control_actions::accept_upload;
				outgoing_packages->produce(response);
				buffer.clear();
				state = ClientHandlerState::receiving_file;
			}
		}
		else if (package.type == datagram_type::data)
		{
			buffer.push_back(package);
			if (package.data.is_last)
			{
				PersistenceFileManager().write("FILENAME", buffer);
				buffer.clear();
			}
		}
    }
}