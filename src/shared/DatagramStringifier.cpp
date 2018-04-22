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
            case control_actions::request_rename:
                out += "request_rename";
                    break;
            case control_actions::accept_rename:
                out += "accept_rename";
                    break;
            case control_actions::deny_rename:
                out += "deny_rename";
                    break;
            case control_actions::request_download:
                out += "request_download";
                    break;
            case control_actions::accept_download:
                out += "accept_download";
                    break;
            case control_actions::deny_download:
                out += "deny_download";
                    break;
            case control_actions::request_upload:
                out += "request_upload";
                    break;
            case control_actions::accept_upload:
                out += "accept_upload";
                    break;
            case control_actions::deny_upload:
                out += "deny_upload";
                    break;
            case control_actions::request_login:
                out += "request_login";
                    break;
            case control_actions::accept_login:
                out += "accept_login";
                    break;
            case control_actions::deny_login:
                out += "deny_login";
                    break;
            case control_actions::request_logout:
                out += "request_logout";
                    break;
            case control_actions::accept_logout:
                out += "accept_logout";
                    break;
            default:
                out += "corrupted (" + std::to_string((int)action) + ")";
        }
        out += "\"\n";
    }

    return out;
}