#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

#define SUCC_ARGS	100
#define SUCC_ARG1	101
#define SUCC_ARG2	102
#define SUCC_ARG3	103
#define SUCC_TERMINATE	199

#define ERR_NOFARGS	-1
#define ERR_TERMINATE	-199

#define UI_START	200
#define UI_S_HELP	202
#define UI_S_UPLOAD	203
#define UI_S_DOWNLOAD	204
#define UI_S_LISTSERV	205
#define UI_S_LISTCLI	206
#define UI_S_SYNCDIR	207
#define UI_S_EXIT	208
#define UI_CMD_UNKNOWN	221
#define UI_CMD_HELP	222
#define UI_CMD_UPLOAD	223
#define UI_CMD_DOWNLOAD	224
#define UI_CMD_LISTSERV	225
#define UI_CMD_LISTCLI	226
#define UI_CMD_SYNCDIR	227
#define UI_CMD_EXIT	228

#define CMD_HELP	"help"
#define CMD_EXIT	"exit"

#define CODE_EXIT	501

#define ERROR_COLOR	"\x1b[31m"
#define DROPBOX_COLOR	"\x1b[34m"
#define CMDLINE_COLOR	"\x1b[35m"
#define INPUT_COLOR	"\x1b[36m"
#define SUCCESS_COLOR	"\x1b[32m"
#define WARNING_COLOR	"\x1b[33m"
#define STANDARD_COLOR	"\x1b[0m"

//Mensagem de boas vindas ao cliente
void ui_welcome_client(char* client_name);

//Mensagens do laco de linha de comando
int ui_cmdline(int ui_code);

//Mensagens do laco de linha de comando
void ui_cmdline_input();

//Command input selector
void ui_cmd_selector(std::string command);

//Mensagem de sucesso em geral
void ui_success(int succ_code, std::string message1, std::string message2, std::string message3);

//Mensagens de aviso em geral
void ui_warning(int warn_code, std::string message1, std::string message2, std::string message3);

//Mensagem de erros em geral
void ui_error(int err_code, std::string message1, std::string message2, std::string message3);