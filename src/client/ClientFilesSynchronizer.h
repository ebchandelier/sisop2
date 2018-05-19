#pragma once
#include <unistd.h>
#include "../shared/DeviceFilesInfo.h"
#include "./ClientConnectionManager.h"

class ClientFilesSynchronizer
{
public:
    void run(ClientConnectionManager& manager);

private:
    void resolve_diff(DeviceFilesInfo client, DeviceFilesInfo server, ClientConnectionManager& manager);
    //ClientConnectionManager* manager;
};