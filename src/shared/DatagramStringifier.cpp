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
            case control_actions::deny_list_files:
                out += "deny_list_files";
                break;
            case control_actions::request_exclude:
                out += "request_exclude";
                break;
            case control_actions::accept_exclude:
                out += "accept_exclude";
                break;
            case control_actions::deny_exclude:
                out += "deny_exclude";
                break;
            default:
                out += "corrupted (" + std::to_string((int)action) + ")";
        }
        out += "\"\n";

        switch (package.control.action)
        {
            case control_actions::request_login:
                out += add_username(package.control.login_request_data.username);
                break;
            case control_actions::request_upload:
                out += add_file(package.control.upload_request_data.filename);
                out += add_version(package.control.upload_request_data.version);
                break;
            case control_actions::accept_download:
                out += add_file(package.control.download_accept_data.filename);
                out += add_version(package.control.download_accept_data.version);
                break;
            case control_actions::request_download:
                out += add_file(package.control.download_request_data.filename);
                break;
            case control_actions::request_exclude:
                out += add_file(package.control.exclude_request_data.filename);
                break;
            case control_actions::accept_list_files:
                out += add_files(package.control.list_files_response.data);
                break;
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

std::string DatagramStringifier::add_version(int version)
{
    std::string out;
    out += "Version: \"";
    out += std::to_string(version);
    out += "\"\n";
    return out;
}

std::string DatagramStringifier::add_username(char* username)
{
    std::string out;
    out += "username: \"";
    out += username;
    out += "\"\n";
    return out;
}

std::string DatagramStringifier::add_file(char* buffer)
{
    std::string out;
    out += "file: \"";
    out.append(buffer);
    out += "\"\n";
    return out;
}

std::string DatagramStringifier::add_files(char* buffer)
{
    std::string out;
    char data[DATA_BUFFER_SIZE];
    strncpy(data, buffer, DATA_BUFFER_SIZE);
    out += "files: \"";
    out += data;
    out += "\"\n";
    return out;
}