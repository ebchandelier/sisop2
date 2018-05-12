#pragma once
#define MAXFILENAME 10
typedef struct {
	char name[MAXFILENAME];	
	char extension[MAXFILENAME];	
	char last_modified[MAXFILENAME];	
	int	size;	
} file_info;