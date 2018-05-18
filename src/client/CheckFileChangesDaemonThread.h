#pragma once

#include <iostream>
#include <unistd.h>
#include <thread>
#include <sys/inotify.h>
#include <string.h>
#include <vector>
#include <dirent.h>
#include <stdio.h>

#include "../shared/DeviceFilesInfo.h"
#include "ClientConnectionManager.h"

#define MAX_EVENTS 1024 
#define EVENT_SIZE (sizeof(struct inotify_event)) 
#define BUF_LEN (MAX_EVENTS*(EVENT_SIZE+MAXIMUM_FILE_NAME)) 

#define FILTERS IN_CREATE|IN_DELETE|IN_MODIFY|IN_MOVE

class CheckFileChangesDaemonThread
{
public:

    void run(std::string path, DeviceFilesInfo& files_info);

private:

    void checkFileChange(std::string path, DeviceFilesInfo& files_info);

    static std::vector<std::string> add_inotify_watch_recursive(int fd, std::string folder);

};