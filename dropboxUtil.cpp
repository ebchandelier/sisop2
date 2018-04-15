#include "dropboxUtil.h"

void ui_welcome_client(char* client_name)
{
  printf("\n");
  printf("__________________________________________\n");
  printf("\\                                        /\n");
  printf("|            " DROPBOX_COLOR "DROPBOX" STANDARD_COLOR " [CLIENT]            |\n");
  printf("|                Versao 1                |\n");
  printf("|                                        |\n");
  printf("|               Bem vindo!               |\n");
  printf("|                                        |\n");
  printf("/________________________________________\\ \n");
}

void ui_error(int err_code)
{
  printf("\n");
  printf("["DROPBOX_COLOR"DboxClient"STANDARD_COLOR"]["ERROR_COLOR"ERROR"STANDARD_COLOR"]: ");

  switch (err_code)
  {

  case ERR_NOFARGS:
    printf("Wrong number of arguments, please use: ./dropboxClient user endereco porta;");
    ui_error(ERR_TERMINATE);
    exit(ERR_NOFARGS);
    break;

  case ERR_TERMINATE:
    printf("Dropbox will now terminate.\n\n");
    break;

  }

  return;
}
