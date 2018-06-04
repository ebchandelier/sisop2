#include "./FileInfoVectorSerializer.h"

void FileInfoVectorSerializer::serialize(char* out, DeviceFilesInfo device_files)
{
    char* pt = out;
    for (auto file : device_files.files)
    {
        //auto n = sprintf(out, "%s\n%d\n", file.name.c_str(), file.version);
        //out += n + 1;
        strncpy(pt, file.name.c_str(), file.name.length());
        pt += file.name.length() + 1;
        *pt = '\n';
        pt += 1;
        *pt = file.version;
        pt++;
    }
    *pt = NULL;
}

DeviceFilesInfo FileInfoVectorSerializer::deserialize(char* buffer)
{
    DeviceFilesInfo files;
    while(*buffer != NULL)
    {
        file_info file;
        // Read filename
        while(*buffer != '\n')
        {
            file.name += *buffer;
            buffer++;
        }
        // Read version
        buffer++; // \n
        file.version = *buffer; // char
        buffer++; // \n

        files.set(file);
    }
    return files;
}