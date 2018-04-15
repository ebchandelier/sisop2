#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define SUCC_ARGS	100
#define SUCC_ARG1	101
#define SUCC_ARG2	102
#define SUCC_ARG3	103
#define SUCC_TERMINATE	199

#define ERR_NOFARGS	-1
#define ERR_TERMINATE	-199

#define ERROR_COLOR	"\x1b[31m"
#define DROPBOX_COLOR	"\x1b[34m"
#define SUCCESS_COLOR	"\x1b[32m"
#define WARNING_COLOR	"\x1b[33m"
#define STANDARD_COLOR	"\x1b[0m"

//Mensagem de boas vindas ao cliente
void ui_welcome_client(char* client_name);

void ui_next_action();

//Mensagem de sucesso em geral
void ui_success(int succ_code, std::string message1, std::string message2, std::string message3);

//Mensagens de aviso em geral
void ui_warning(int warn_code, std::string message1, std::string message2, std::string message3);

//Mensagem de erros em geral
void ui_error(int err_code, std::string message1, std::string message2, std::string message3);
