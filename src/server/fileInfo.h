#pragma once
#define MAXNAME 10
typedef struct {
	char name[MAXNAME];	
	char extension[MAXNAME];	
	char last_modified[MAXNAME];	
	int	size;	
} file_info;