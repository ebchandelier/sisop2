#include "DatagramStringifier.h"

std::string DatagramStringifier::stringify(datagram package)
{
    std::string out;

    out += "Type: \"";
    if (package.type == datagram_type::control)
    {
        out += "control";
    }
    else if (package.type == datagram_type::data) 
    {
        out += "data";
    }
    else
    {
        out += "corrupted";
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
            // TODO: Add the others
            default:
                out += "corrupted";
        }
        out += "\"\n";
    }

    return out;
}