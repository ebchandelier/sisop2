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

void ui_cmdline(int ui_code)
{
  std::cout << "\n";
  std::cout << "["DROPBOX_COLOR"DboxClient"STANDARD_COLOR"]["CMDLINE_COLOR"CMDLINE"STANDARD_COLOR"]: ";

  switch (ui_code)
  {

  case UI_START:
    std::cout << "Welcome to Dropbox command line interface!" ;
    ui_cmdline(UI_S_HELP);
    ui_cmdline(UI_S_UPLOAD);
    ui_cmdline(UI_S_DOWNLOAD);
    ui_cmdline(UI_S_LISTSERV);
    ui_cmdline(UI_S_LISTCLI);
    ui_cmdline(UI_S_SYNCDIR);
    ui_cmdline(UI_S_EXIT);
    break;

  case UI_S_HELP:
    std::cout << "To call for help simply type: \"help\";" ;
    break;

  case UI_S_UPLOAD:
    std::cout << "To upload a file use \"upload<path\\filename>\";" ;
    break;

  case UI_S_DOWNLOAD:
    std::cout << "To download a file use \"download<filename.ext>\";" ;
    break;

  case UI_S_LISTSERV:
    std::cout << "To list the serverside saved files use \"list_server\";" ;
    break;

  case UI_S_LISTCLI:
    std::cout << "To list the clientside saved files use \"list_client\";" ;
    break;

  case UI_S_SYNCDIR:
    std::cout << "To create the sync_dir on your home use \"get_sync_dir\";" ;
    break;

  case UI_S_EXIT:
    std::cout << "To terminate a session use \"exit\";" ;
    break;

  default:
    std::cout << "BAD CALL;" ;

  }

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
