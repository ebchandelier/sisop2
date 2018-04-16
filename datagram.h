#include <stdint.h>
#define DATA_BUFFER_SIZE 1400
#define MAXIMUM_FILE_NAME 100

typedef enum
{
    control,
    data
} datagram_type;

typedef enum
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

    ack
} control_actions;

typedef enum
{
    new_user,
    old_user
} user_type;

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



