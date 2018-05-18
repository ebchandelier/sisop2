#pragma once

#include <string>
#include <cstring>
#include "datagram.h"

class DatagramStringifier
{
public:
    std::string stringify(datagram package);

private:

    std::string add_version(int version);

    std::string add_username(char* username);

    std::string add_file(char* buffer);

    std::string add_files(char* buffer);
};