#pragma once

#include <iostream>
#include <unistd.h>
#include <thread>
#include <sys/inotify.h>
#include <string.h>
#include "./../shared/datagram.h"
#include <vector>
#include <dirent.h>
#include <stdio.h>

#define MAX_EVENTS 1024 
#define EVENT_SIZE (sizeof(struct inotify_event)) 
#define BUF_LEN (MAX_EVENTS*(EVENT_SIZE+MAXIMUM_FILE_NAME)) 

#define FILTERS IN_CREATE|IN_DELETE|IN_MODIFY|IN_MOVE

class CheckFileChangesDaemonThread
{
private:
    std::thread thread;

public:
    // TODO: This should be implicit in the contructor/destructor
    void create(std::string userName);

    void kill();

private:

    static void checkFileChange(std::string userName);

    static std::vector<std::string> add_inotify_watch_recursive(int fd, std::string folder);
};