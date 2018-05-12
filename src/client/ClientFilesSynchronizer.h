#pragma once
#include <unistd.h>
#include "./ClientConnectionManager.h"

class ClientFilesSynchronizer
{
public:
    void run(ClientConnectionManager& manager);

    //ClientConnectionManager* manager;
};