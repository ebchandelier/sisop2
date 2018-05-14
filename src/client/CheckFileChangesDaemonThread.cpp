#include "CheckFileChangesDaemonThread.h"

void CheckFileChangesDaemonThread::run(std::string path, ClientConnectionManager& clientConnectionManager)
{
    checkFileChange(path, clientConnectionManager);
}

void CheckFileChangesDaemonThread::checkFileChange(std::string path, ClientConnectionManager& clientConnectionManager) 
{
    char buffer[BUF_LEN];
    int fd = inotify_init();
    std::vector<std::string> wdVector = add_inotify_watch_recursive(fd, path);
 
    while(true) 
    {
        int i = 0;
        int length = read( fd, buffer, BUF_LEN ); 
        while ( i < length ) 
        {
            struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
            if ( event->len )
            {
                if ( event->mask & IN_CREATE) 
                {
                    if (event->mask & IN_ISDIR) 
                    {
                        std::cout << "The directory " << event->name << " was Created\n";
                        std::string newPath = path + "/" + std::string(event->name);
                        int anotherWd = inotify_add_watch(fd, newPath.c_str(), FILTERS);
                        wdVector.push_back(newPath);
                    } 
                    else 
                    {
                        std::cout << "The file " << event->name << " was Created\n";
                        std::string fileName(event->name);
                        std::string filePath = path + "/" + fileName;
                        clientConnectionManager.send_file((char *)filePath.c_str());   
                    }
                }
                if ( event->mask & IN_MODIFY) 
                {
                    if (event->mask & IN_ISDIR) 
                    {
                        std::cout << "The directory " << event->name << " was modified\n";      
                    } else 
                    {
                        std::cout << "The file " << event->name << " was modified\n";      
                    }
                }
                if ( event->mask & IN_DELETE || event->mask & IN_MOVE) 
                {
                    // event->mask == IN_MOVED_TO due to a gnome bug, see https://github.com/cooltronics/NFC_device/issues/2
                    if (event->mask & IN_ISDIR) 
                    {
                        std::cout << "The directory " << event->name << " was deleted\n";      
                    } else 
                    {
                        std::cout << "The file " << event->name << " was deleted\n";      
                    }
                } 
                i += EVENT_SIZE + event->len;
            }
        }
    }
    close( fd );
}

std::vector<std::string> CheckFileChangesDaemonThread::add_inotify_watch_recursive(int fd, std::string folder) 
{
    std::vector<std::string> wdVector;
    int wd = inotify_add_watch(fd, folder.c_str(), FILTERS);
    wdVector.push_back(folder.c_str());

    DIR *dir = opendir(folder.c_str());

    struct dirent *entry = readdir(dir);

    while (entry != NULL) 
    {
        if (entry->d_type == DT_DIR) 
        {
            if(strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..") ) 
            {
                std::vector<std::string> rec = add_inotify_watch_recursive(fd, folder + "/" + std::string(entry->d_name));
                wdVector.insert(wdVector.end(), rec.begin(), rec.end());
            } 
        }
        entry = readdir(dir);
    }
    closedir(dir);
    return wdVector;
}
