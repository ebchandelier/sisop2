#pragma once
#include <string>
#define MAXNAME 10
typedef struct {
	std::string name;
//	char name[MAXNAME];	
//	char extension[MAXNAME];	
	char last_modified[MAXNAME];	
	int	size;	
} file_info;