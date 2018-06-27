#include "CheckFileChangesDaemonThread.h"

void CheckFileChangesDaemonThread::run(std::string path, DeviceFilesInfo& files_info)
{
    checkFileChange(path, files_info);
}

void CheckFileChangesDaemonThread::print_debug(struct inotify_event* event)
{
    printf("%s: ", event->name);
    if (event->mask & IN_CREATE) {
        printf("IN_CREATE, ");
    }
    if (event->mask & IN_CLOSE_WRITE) {
        printf("IN_CLOSE_WRITE");
    }
    if (event->mask & IN_MOVED_FROM) {
        printf("MOVED_FROM, ");
    }
    if (event->mask & IN_MOVED_TO) {
        printf("MOVED_TO, ");
    }
    if (event->mask & IN_DELETE) {
        printf("IN_DELETED, ");
    }
    if (event->mask & IN_MODIFY) {
        printf("IN_MODIFY, ");
    }
    printf("\n");
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
                print_debug(event);
                if (event->mask & IN_CLOSE_WRITE) 
                {
                    // O arquivo pode ter sido criado ou editado
                    printf("%s changed\n");
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
                if (event->mask & IN_MOVED_FROM)
                {
                    // O arquivo foi 'deletado', removido da pasta para a lixeira
                    printf("%s was removed\n");
                    if (files_info.has(event->name))
                    {
                        files_info.remove(event->name);
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
