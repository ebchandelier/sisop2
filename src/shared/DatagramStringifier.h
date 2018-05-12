#pragma once

#include <string>
#include <cstring>
#include "datagram.h"

class DatagramStringifier
{
public:
    std::string stringify(datagram package);
};