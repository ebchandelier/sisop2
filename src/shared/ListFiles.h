#pragma once

#include <iostream>
#include "../shared/fileInfo.h"
#include <list>
#include <dirent.h>
#include <string.h>


class ListFiles
{
public:

    static std::list<file_info> listFilesAt(std::string path);

    static void print(std::list<file_info> files);
};