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

    void create(std::string userName) {

        this->thread = std::thread(checkFileChange, userName);
    }

    void kill() {

        this->thread.~thread();
    }

private:

    static void checkFileChange(std::string userName) {
        
        char buffer[BUF_LEN];
        int fd = inotify_init();
        std::string userPath = "/home/sync_dir_" + userName;

        std::vector<std::string> wdVector = add_inotify_watch_recursive(fd, userPath);
 
        while(true) {

            int i = 0;
            int length = read( fd, buffer, BUF_LEN ); 
    
            while ( i < length ) {
                
                struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
                
                if ( event->len ) {
                    
                    if ( event->mask & IN_CREATE) {

                        if (event->mask & IN_ISDIR) {

                            std::cout << "The directory " << event->name << " was Created\n";
                            std::string newPath = userPath + "/" + std::string(event->name);
                            int anotherWd = inotify_add_watch(fd, newPath.c_str(), FILTERS);
                            wdVector.push_back(newPath);

                        
                        } else {

                            std::cout << "The file " << event->name << " was Created\n";      
                        }
                    }
            
                    if ( event->mask & IN_MODIFY) {

                        if (event->mask & IN_ISDIR) {

                            std::cout << "The directory " << event->name << " was modified\n";      
                        
                        } else {

                            std::cout << "The file " << event->name << " was modified\n";      
                        }
                    }
                    if ( event->mask & IN_DELETE || event->mask & IN_MOVE) { 
                        // event->mask == IN_MOVED_TO due to a gnome bug, see https://github.com/cooltronics/NFC_device/issues/2

                        if (event->mask & IN_ISDIR) {

                            std::cout << "The directory " << event->name << " was deleted\n";      
                        
                        } else {

                            std::cout << "The file " << event->name << " was deleted\n";      
                        }
                    } 

                    i += EVENT_SIZE + event->len;
                }
            }
        }

        close( fd );
    }

    static std::vector<std::string> add_inotify_watch_recursive(int fd, std::string folder) {

        std::vector<std::string> wdVector;
        int wd = inotify_add_watch(fd, folder.c_str(), FILTERS);
        wdVector.push_back(folder.c_str());

        DIR *dir = opendir(folder.c_str());

        struct dirent *entry = readdir(dir);

        while (entry != NULL) {

            if (entry->d_type == DT_DIR) {
                
                if(strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..") ) {

                    std::vector<std::string> rec = add_inotify_watch_recursive(fd, folder + "/" + std::string(entry->d_name));
                    wdVector.insert(wdVector.end(), rec.begin(), rec.end());
                } 
            }

            entry = readdir(dir);
        }

        closedir(dir);

        return wdVector;
    }
};