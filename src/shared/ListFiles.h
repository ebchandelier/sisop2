#pragma once

#include <iostream>
#include <list>
#include <dirent.h>
#include <string.h>

#include "../shared/DeviceFilesInfo.h"
#include "../shared/fileInfo.h"


class ListFiles
{
public:

    static DeviceFilesInfo listFilesAt(std::string path);

    static void print(DeviceFilesInfo& files);
};