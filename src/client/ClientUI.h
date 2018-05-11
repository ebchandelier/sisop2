#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

#define SUCC_ARGS       100
#define SUCC_ARG1       101
#define SUCC_ARG2       102
#define SUCC_ARG3       103
#define SUCC_LOGIN      104
#define SUCC_TERMINATE  199

#define WAR_LOGIN_SESS  -204

#define ERR_NOFARGS     -1
#define ERR_LOGIN_CONN  -104
#define ERR_LOGIN_RESP  -105
#define ERR_TERMINATE   -199

#define UI_START        200
#define UI_S_HELP       202
#define UI_S_UPLOAD	    203
#define UI_S_DOWNLOAD   204
#define UI_S_LISTSERV   205
#define UI_S_LISTCLI    206
#define UI_S_SYNCDIR    207
#define UI_S_EXIT       208
#define UI_CMD_UNKNOWN  221
#define UI_CMD_HELP	    222
#define UI_CMD_UPLOAD   223
#define UI_CMD_DOWNLOAD	224
#define UI_CMD_LISTSERV	225
#define UI_CMD_LISTCLI  226
#define UI_CMD_SYNCDIR  227
#define UI_CMD_EXIT	    228
#define UI_MISSING_PAR  230

#define CMD_HELP     "help"
#define CMD_UPLOAD   "upload"
#define CMD_DOWNLOAD "download"
#define CMD_LISTSERV "list_server"
#define CMD_LISTCLI  "list_client"
#define CMD_SYNCDIR  "get_sync_dir"
#define CMD_EXIT     "exit"

#define CODE_HELP     302
#define CODE_UPLOAD	  303
#define CODE_DOWNLOAD 304
#define CODE_LISTSERV 305
#define CODE_LISTCLI  306
#define CODE_SYNCDIR  307
#define CODE_EXIT     308

#define ERROR_COLOR	    "\x1b[31m"
#define DROPBOX_COLOR   "\x1b[34m"
#define CMDLINE_COLOR   "\x1b[35m"
#define INPUT_COLOR     "\x1b[36m"
#define SUCCESS_COLOR   "\x1b[32m"
#define WARNING_COLOR   "\x1b[33m"
#define STANDARD_COLOR  "\x1b[0m"

class ClientUI
{
public:
    //Mensagem de boas vindas ao cliente
    void welcome_client(char* client_name);

    //Mensagens do laco de linha de comando
    int cmdline(int ui_code);

    //Mensagens do laco de linha de comando
    void cmdline_input();

    //Command input selector
    void cmd_selector(std::string command);

    //Mensagem de sucesso em geral
    void success(std::string message);
    void success(int succ_code, std::string message1, std::string message2, std::string message3);

    //Mensagens de aviso em geral
    void warning(std::string message);
    void warning(int warn_code, std::string message1, std::string message2, std::string message3);

    //Mensagem de erros em geral
    void error(std::string message);
    void error(int err_code, std::string message1, std::string message2, std::string message3);

    //Acesso parametro do comando
    std::string get_parameter();

private:

    std::string new_command;
    int command_to_main;
    std::string input_parameter;
    bool skip_new_line;
};


