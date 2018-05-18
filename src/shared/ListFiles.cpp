#include "ListFiles.h"

DeviceFilesInfo ListFiles::listFilesAt(std::string path) 
{
    DeviceFilesInfo files;
    struct dirent *dp;
    DIR *dirp = opendir(path.c_str());
        
    while ((dp = readdir(dirp)) != NULL) 
    {
        if(strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) 
        {
            file_info file;
            file.name = dp->d_name;
            file.version = 1;
            files.set(file);
        }
    }
        
    closedir(dirp);

    return files;
}

void ListFiles::print(DeviceFilesInfo& device_files) 
{
    std::cout << "\nListing Files:\n";
    for(auto file : device_files.files) 
    {
        std::cout << file.name << "\tVersion: " << std::to_string(file.version) << "\n";
    }
}
