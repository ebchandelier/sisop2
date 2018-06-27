#include "CheckFileChangesDaemonThread.h"

void CheckFileChangesDaemonThread::run(std::string path, DeviceFilesInfo& files_info)
{
    checkFileChange(path, files_info);
}

void CheckFileChangesDaemonThread::checkFileChange(std::string path, DeviceFilesInfo& files_info)
{
    char buffer[BUF_LEN];
    int fd = inotify_init();
    std::vector<std::string> wdVector = add_inotify_watch_recursive(fd, path);
 
    while(true) 
    {
        int i = 0;
        int length = read(fd, buffer, BUF_LEN);
        while (i < length) 
        {
            struct inotify_event *event = (struct inotify_event *) &buffer[i];
            if (event->len)
            {
                if (event->mask & IN_CREATE)
                {
                    if (event->mask & IN_ISDIR) 
                    {
                        //std::cout << "The directory " << event->name << " was Created\n";
                    } 
                    else
                    {
                        std::cout << "The file " << event->name << " was Created\n";
                        file_info file;
                        file.name = event->name;
                        file.version = 1;
                        files_info.set(file);
                    }
                }
                if (event->mask & IN_CLOSE_WRITE)
                {
                    if (event->mask & IN_ISDIR)
                    {
                        //std::cout << "The directory " << event->name << " was modified\n";      
                    }
                    else
                    {
                        std::cout << "The file " << event->name << " was modified\n";
                        if (files_info.has(event->name))
                        {
                            auto file = files_info.get(event->name);
                            file.version++;
                            files_info.set(file); 
                        }
                        else
                        {
                            file_info file;
                            file.name = event->name;
                            file.version = 1;
                            files_info.set(file);
                        }
                        
                    }
                }
                if (event->mask & IN_DELETE || event->mask & IN_MOVE)
                {
                    // event->mask == IN_MOVED_TO due to a gnome bug, see https://github.com/cooltronics/NFC_device/issues/2
                    if (event->mask & IN_ISDIR)
                    {
                        //std::cout << "The directory " << event->name << " was deleted\n";      
                    } 
                    else 
                    {
                        // BAD TRIGGER
                        //std::cout << "The file " << event->name << " was deleted\n";
                        //files_info.remove(event->name);
                    }
                } 
                i += EVENT_SIZE + event->len;
            }
        }
    }
    close(fd);
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
