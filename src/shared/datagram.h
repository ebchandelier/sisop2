#pragma once
#include <stdint.h>
#include <netinet/in.h>
#define DATA_BUFFER_SIZE 1400
#define MAXIMUM_USERNAME 10
#define MAXIMUM_FILE_NAME 100
#define MAXIMUM_ERROR_MESSAGE 100

enum class datagram_type
{
    control,
    data,
    ack
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
    deny_logout,

    request_list_files,
    accept_list_files,
    deny_list_files,

    request_primary_server,
    accept_primary_server
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

typedef struct
{
    char filename[MAXIMUM_FILE_NAME];
    char version;
} control_file_with_version_info;

typedef struct 
{
    char data[DATA_BUFFER_SIZE];
} control_list_files_response_data;

typedef struct
{
    int files_count;
} control_sync_dir_response_data;

typedef struct
{
    char message[MAXIMUM_ERROR_MESSAGE];
} error_message;

typedef struct
{
    control_actions action;
    uint32_t package_id;
    union {
        // Login
        control_login_data login_response_data;
        control_login_request_data login_request_data;

        // Upload
        control_file_with_version_info upload_request_data;

        // Download
        control_file_info download_request_data;
        control_file_with_version_info download_accept_data;
        error_message download_deny_data;

        // Exclude
        control_file_info exclude_request_data;

        // List
        control_list_files_response_data list_files_response;

        // Sync dir
        control_sync_dir_response_data sync_dir_response;
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
    bool is_from_master;
    // If the master is fowarding the package to a replica,
    // the original information about the sender must be preserved
    sockaddr_in original_sender;
    union {
        data_datagram data;
        control_datagram control;
        uint32_t ack_last_package_id_received;
    };
} datagram;


//DEFS FOR MULTISERVER
typedef struct {
    char ip[15];
    int portTCP;
    int portUDP;
    int pid;
} PROCESS_PATH;

enum class control_type
{
    nothing,
    action_add_ip_port,
    ip_port_added,
    start_election,
    election,
    answer,
    coordinator
};

typedef struct {
    control_type type;
    union {
        PROCESS_PATH ip_port;
    };
} TYPE;


#define DATAGRAM_SIZE sizeof(datagram)