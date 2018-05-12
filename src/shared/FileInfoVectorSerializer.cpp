#include "./FileInfoVectorSerializer.h"

void FileInfoVectorSerializer::serialize(char* out, std::list<file_info> files)
{
    char* pt = out;
    for (auto file : files)
    {
        strncpy(pt, file.name.c_str(), file.name.length());
        pt += file.name.length() + 1;
        *pt = '\n';
        pt += 1;
    }
    *pt = NULL;
}

std::list<file_info> FileInfoVectorSerializer::deserialize(char* buffer)
{
    std::list<file_info> files;
    while(*buffer != NULL)
    {
        file_info file;
        while(*buffer != '\n')
        {
            file.name += *buffer;
            buffer++;
        }
        files.push_back(file);
        buffer++;
    }
    return files;
}