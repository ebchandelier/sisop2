#pragma once
#include "fileInfo.h"
#define MAXNAME 10
#define MAXFILES 100

typedef struct {	
	int	devices[2];	
	char userid[MAXNAME];	
	file_info files[MAXFILES];	
    int	logged_in;	
} client_info;