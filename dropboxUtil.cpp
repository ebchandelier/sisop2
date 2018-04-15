#include "dropboxUtil.h"

void ui_welcome_client(char* client_name)
{
  std::cout << "\n";
  std::cout << "__________________________________________\n";
  std::cout << "\\                                        /\n";
  std::cout << "|            " DROPBOX_COLOR "DROPBOX" STANDARD_COLOR " [CLIENT]            |\n";
  std::cout << "|                Versao 1                |\n";
  std::cout << "|                                        |\n";
  std::cout << "|               Bem vindo!               |\n";
  std::cout << "|                                        |\n";
  std::cout << "/________________________________________\\ \n";
}

void ui_success(int succ_code, std::string message1, std::string message2, std::string message3)
{
  std::cout << "\n";
  std::cout << "["DROPBOX_COLOR"DboxClient"STANDARD_COLOR"]["SUCCESS_COLOR"SUCCESS"STANDARD_COLOR"]: ";

  switch (succ_code)
  {

  case SUCC_ARGS:
    std::cout << "Received succesfully the following inputs" ;
    ui_success(SUCC_ARG1, message1, message2, message3);
    ui_success(SUCC_ARG2, message1, message2, message3);
    ui_success(SUCC_ARG3, message1, message2, message3);
    break;

  case SUCC_ARG1:
    std::cout << "Username:  " << message1 << ";" ;
    break;

  case SUCC_ARG2:
    std::cout << "Address:  " << message2 << ";" ;
    break;

  case SUCC_ARG3:
    std::cout << "Port:  " << message3 << ";" ;
    break;

  case SUCC_TERMINATE:
    std::cout << "Thank you. Dropbox will now terminate.\n\n";
    break;

  default:
    std::cout << "BAD CALL;" ;

  }

  return;
}

void ui_warn(int warn_code, std::string message1, std::string message2, std::string message3)
{
  std::cout << "\n";
  std::cout << "["DROPBOX_COLOR"DboxClient"STANDARD_COLOR"]["WARNING_COLOR"WARNING"STANDARD_COLOR"]: ";

  switch (warn_code)
  {

  default:
    std::cout << "BAD CALL;" ;

  }

  return;
}

void ui_error(int err_code, std::string message1, std::string message2, std::string message3)
{
  std::cout << "\n";
  std::cout << "["DROPBOX_COLOR"DboxClient"STANDARD_COLOR"]["ERROR_COLOR"ERROR"STANDARD_COLOR"]: " ;

  switch (err_code)
  {

  case ERR_NOFARGS:
    std::cout << "Wrong number of arguments, please use: ./dropboxClient user endereco porta;" ;
    ui_error(ERR_TERMINATE, message1, message2, message3);
    exit(ERR_NOFARGS);
    break;

  case ERR_TERMINATE:
    std::cout << "Dropbox will now terminate.\n\n" ;
    break;

  default:
    std::cout << "BAD CALL;" ;

  }

  return;
}
