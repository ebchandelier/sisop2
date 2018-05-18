#include "./ClientFilesSynchronizer.h"


void ClientFilesSynchronizer::run(ClientConnectionManager& manager)
{
    while(true)
    {
        sleep(5);
        //manager.sync_client();
    }
}
