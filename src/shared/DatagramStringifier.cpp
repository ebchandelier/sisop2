#include "DatagramStringifier.h"

std::string DatagramStringifier::stringify(datagram package)
{
    std::string out;

    out += "Type: \"";
    switch(package.type)
    {
        case datagram_type::control:
            out += "control";
            break;
        case datagram_type::data:
            out += "data";
            break;
        default:
            out += "corrupted (" + std::to_string((int)package.type) + ")";
    }
    out += "\"\n";

    if (package.type == datagram_type::control)
    {
        out += "Action: \"";
        auto action = package.control.action;
        switch (action)
        {
            case control_actions::request_login:
                out += "request_login";
                break;
            case control_actions::deny_login:
                out += "deny_login";
                break;
            case control_actions::accept_login:
                out += "accept_login";
                break;
            // TODO: Add the others
            default:
                out += "corrupted (" + std::to_string((int)action) + ")";
        }
        out += "\"\n";
    }

    return out;
}