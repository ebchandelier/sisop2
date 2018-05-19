#include "./ClientFilesSynchronizer.h"


void ClientFilesSynchronizer::run(ClientConnectionManager& manager)
{
    while(true)
    {
        sleep(8);
        manager.sync_client();
    }
}
