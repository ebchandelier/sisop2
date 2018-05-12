#pragma once
#include <stdint.h>
#define DATA_BUFFER_SIZE 1400
#define MAXIMUM_USERNAME 10
#define MAXIMUM_FILE_NAME 100

enum class datagram_type
{
    control,
    data
};

enum class control_actions
{
    request_exclude,
    accept_exclude,
    deny_exclude,

    request_rename,
    accept_rename,
    deny_rename,

    request_download,
    accept_download,
    deny_download,

    request_upload,
    accept_upload,
    deny_upload,

    request_login,
    accept_login,
    deny_login,

    request_logout,
    accept_logout,

    request_list_files,

    ack
};

enum class user_type
{
    new_user,
    old_user
};

typedef struct
{
    user_type type;
} control_login_data;

typedef struct
{
    char username[MAXIMUM_USERNAME];
} control_login_request_data;

typedef struct
{
    char filename[MAXIMUM_FILE_NAME];
} control_file_info;

typedef struct {

    char username[MAXIMUM_USERNAME];
} control_list_file_of_user;

typedef struct
{
    control_actions action;
    union {
        control_login_data login_response_data;
        control_login_request_data login_request_data;
        control_file_info file;
        control_list_file_of_user user;
    };

} control_datagram;

typedef struct
{
    // Is this the last package of the file
    bool is_last;    
    // How many bytes are actual data on the buffer
    uint32_t data_length;
    // Buffer holding the data
    char buffer[DATA_BUFFER_SIZE];
    uint32_t package_id;
} data_datagram;

typedef struct
{
    datagram_type type;
    union {
        data_datagram data;
        control_datagram control;
    };
} datagram;


#define DATAGRAM_SIZE sizeof(datagram)



