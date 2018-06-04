#pragma once
#include <list>
#include <string>
#include <cstring>
#include "DeviceFilesInfo.h"

class FileInfoVectorSerializer
{
public:
    void serialize(char* out, DeviceFilesInfo files);

    DeviceFilesInfo deserialize(char* buffer);
};