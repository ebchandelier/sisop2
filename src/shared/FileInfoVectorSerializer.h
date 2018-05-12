#pragma once
#include <list>
#include <string>
#include <cstring>
#include "./fileInfo.h"

class FileInfoVectorSerializer
{
public:
    void serialize(char* out, std::list<file_info> files);

    std::list<file_info> deserialize(char* buffer);
};