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
            case control_actions::request_list_files:
                out += "request_list_files";
                break;
            case control_actions::accept_list_files:
                out += "accept_list_files";
                break;
            default:
                out += "corrupted (" + std::to_string((int)action) + ")";
        }
        out += "\"\n";

        if (package.control.action == control_actions::request_login)
        {
            out += "username: \"";
            out.append(package.control.login_request_data.username);
            out += "\"\n";
        }
        if (package.control.action == control_actions::accept_list_files)
        {
            char data[DATA_BUFFER_SIZE];
            strncpy(data, package.control.list_files_response.data, DATA_BUFFER_SIZE);
            out += "files: \"";
            out += data;
            out += "\"\n";
        }
        if (package.control.action == control_actions::request_download ||
            package.control.action == control_actions::request_upload ||
            package.control.action == control_actions::request_exclude)
        {
            out += "file: \"";
            out.append(package.control.file.filename);
            out += "\"\n";
        }
    }
    if (package.type == datagram_type::data)
    {
        out += "Content: \"" + std::string(package.data.buffer) + "\"\n";
        out += "Id: " + std::to_string(package.data.package_id) + "\n";
        out += "Is Last:" + std::to_string(package.data.is_last) + "\n";
    }


    return out;
}