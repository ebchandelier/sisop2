#pragma once

#include<iostream>
#include "../shared/fileInfo.h"
#include<vector>

#include <dirent.h>
#include<string.h>


class ListFiles
{
public:

    static std::vector<file_info> listFilesAt(std::string path) {

        std::vector<file_info> files;

        struct dirent *dp;
        
        DIR *dirp = opendir(path.c_str());
        
        while ((dp = readdir(dirp)) != NULL) {
            
            if(strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {

                file_info file;

                strcpy(file.name, dp->d_name);
                // get type?

                files.push_back(file);
            }
        }
        
        closedir(dirp);


        return files;
    }

    static void print(std::string path, std::vector<file_info> files) {

        std::cout << "\nListingFiles at " << path << ":\n";

        for(auto file : files) {

            std::cout << file.name << "\n";
        }
    }
};