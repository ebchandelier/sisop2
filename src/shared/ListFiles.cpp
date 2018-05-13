#include "ListFiles.h"

std::list<file_info> ListFiles::listFilesAt(std::string path) 
{
    std::list<file_info> files;
    struct dirent *dp;
    DIR *dirp = opendir(path.c_str());
        
    while ((dp = readdir(dirp)) != NULL) 
    {
        if(strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) 
        {
            file_info file;
            file.name = dp->d_name;
            // get type?

            files.push_back(file);
        }
    }
        
    closedir(dirp);


    return files;
}

void ListFiles::print(std::list<file_info> files) {
    std::cout << "\nListing Files:\n";
    for(auto file : files) 
    {
        std::cout << file.name << "\n";
    }
}
