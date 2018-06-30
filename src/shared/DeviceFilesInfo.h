#pragma once
#include <vector>
#include <algorithm>
#include <string>
#include <mutex>
#include "./fileInfo.h"

class DeviceFilesInfo
{
    friend class ListFiles;
    friend class FileInfoVectorSerializer;
public:

    bool has(std::string file_name);

    file_info get(std::string file_name);

    void set(file_info file);

    void remove(std::string file_name);

    static std::vector<std::string> common_files(
        DeviceFilesInfo& f1, 
        DeviceFilesInfo& f2);

    //std::mutex mutex;

private:

    std::vector<file_info> files;
};