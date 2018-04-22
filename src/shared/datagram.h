#pragma once
#include <stdint.h>
#define DATA_BUFFER_SIZE 1400
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
    char filename[MAXIMUM_FILE_NAME];
} file_info;

typedef struct
{
    control_actions action;
    union {
        control_login_data login_response_data;
        file_info file;
    };

} control_datagram;

typedef struct
{
    // How many bytes are actual data on the buffer
    uint32_t data_lenght;
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



