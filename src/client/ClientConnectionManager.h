#pragma once
#include <cstring>
#include <mutex>
#include <list>
#include <string>
#include <vector>
#include "ClientConnectorUDP.h"
#include "../shared/datagram.h"
#include "../shared/PersistenceFileManager.cpp"
#include "../shared/DatagramStringifier.h"
#include "../shared/DeviceFilesInfo.h"
#include "../shared/FileInfoVectorSerializer.h"

class ClientConnectionManager
{
public:

    ClientConnectionManager(DeviceFilesInfo& device_files) : device_files(device_files) { }; 

    // Tries to connect to the server. Returns -1 on error and 0 on success
    int login_server(char* username, char* host, int port);
    void sync_client();
    void send_file(char* file);
    void get_file(char* file);
    void delete_file(char* file);
    int logout();
    DeviceFilesInfo sendListFilesRequest();

    void enqueue_file_update(char* file);

    std::string work_dir;

private:
    ClientConnectorUDP connector;

    // Internal functions do not acquire the lock, so they can be called from other functions
    // inside the class
    void internal_send_file(const char* file);
    void internal_get_file(char* file);
    void internal_delete_file(char* file);
    void internal_delete_device_file(char* file);
    DeviceFilesInfo internal_sendListFilesRequest();

    void resolve_diff(DeviceFilesInfo client, DeviceFilesInfo server);

    // TODO: Pass by reference
    DeviceFilesInfo& device_files;

    std::vector<std::string> queue;

    // This class behaves like a monitor, only allowing one method to be executing at a time
    std::mutex mutex;
};
