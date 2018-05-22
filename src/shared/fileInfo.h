#pragma once
#include <string>
#define MAXFILENAME 10
typedef struct {
	std::string name;
	char version;
	//char last_modified[MAXFILENAME];	
	//int	size;	
} file_info;