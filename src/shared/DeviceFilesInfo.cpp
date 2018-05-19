#include "DeviceFilesInfo.h"

bool DeviceFilesInfo::has(std::string file_name)
{
    return std::any_of(files.begin(), files.end(), [file_name](const file_info& file) {
        return file.name == file_name;
    });
}

file_info DeviceFilesInfo::get(std::string file_name)
{
    auto result = std::find_if(files.begin(), files.end(), [file_name](const file_info& file) {
        return file.name == file_name;
    });
    return *result;
}

void DeviceFilesInfo::set(file_info file)
{
    if (this->has(file.name))
    {
        this->remove(file.name);
    }
    files.push_back(file);
}

void DeviceFilesInfo::remove(std::string file_name)
{
    files.erase(
        std::remove_if(files.begin(), files.end(), [file_name](const file_info& file) {
            return file.name == file_name;
        })
    );
}

std::vector<std::string> DeviceFilesInfo::common_files(
    DeviceFilesInfo& f1, 
    DeviceFilesInfo& f2)
{
    std::vector<std::string> result;
    for (auto file : f1.files)
    {
        result.push_back(file.name);
    }
    for (auto file : f2.files)
    {
        if (!f1.has(file.name))
        {
            result.push_back(file.name);
        }
    }
    return result;
}
