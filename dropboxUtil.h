#include <stdio.h>
#include <stdlib.h>

#define ERR_NOFARGS	-1
#define ERR_TERMINATE   -101

#define ERROR_COLOR	"\x1b[31m"
#define DROPBOX_COLOR	"\x1b[34m"
#define SUCCESS_COLOR	"\x1b[32m"
#define WARNING_COLOR	"\x1b[33m"
#define STANDARD_COLOR	"\x1b[0m"

//Mensagem de boas vindas ao cliente
void ui_welcome_client(char* client_name);

//Mensagem de erro para inpute de argumentos errado
void ui_error(int err_code);
