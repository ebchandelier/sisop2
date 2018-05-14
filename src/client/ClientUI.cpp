#include "./ClientUI.h"

void ClientUI::welcome_client(char* client_name)
{
  std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
  std::cout << "__________________________________________\n";
  std::cout << "\\                                        /\n";
  std::cout << "|            " DROPBOX_COLOR "DROPBOX" STANDARD_COLOR " [CLIENT]            |\n";
  std::cout << "|                Versao 1                |\n";
  std::cout << "|                                        |\n";
  std::cout << "|               Bem vindo!               |\n";
  std::cout << "|                                        |\n";
  std::cout << "/________________________________________\\ \n";
  input_parameter = "";
  skip_new_line = false;
}

int ClientUI::cmdline(int ui_code)
{
  if (skip_new_line)
    skip_new_line = false;
  else
    std::cout << "\n";

  std::cout << "[" DROPBOX_COLOR "DboxClient" STANDARD_COLOR "][" CMDLINE_COLOR "CMDLINE" STANDARD_COLOR "]: ";

  switch (ui_code)
  {

  case UI_START:
    std::cout << "Welcome to Dropbox command line interface!" ;
    cmdline(UI_S_HELP);
    cmdline(UI_S_UPLOAD);
    cmdline(UI_S_DOWNLOAD);
    cmdline(UI_S_LISTSERV);
    cmdline(UI_S_LISTCLI);
    cmdline(UI_S_SYNCDIR);
    cmdline(UI_S_EXIT);
    cmdline_input();
    break;

  case UI_S_HELP:
    std::cout << "To call for help use \"help\";" ;
    break;

  case UI_S_UPLOAD:
    std::cout << "To upload a file use \"upload <path\\filename>\";" ;
    break;

  case UI_S_DOWNLOAD:
    std::cout << "To download a file use \"download <filename.ext>\";" ;
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

  case UI_CMD_UNKNOWN:
    std::cout << "Unrecognized command, if you need help type \"help\";" ;
    cmdline_input();
    break;

  case UI_CMD_HELP:
    std::cout << "Here is the list of available commands" ;
    cmdline(UI_S_HELP);
    cmdline(UI_S_UPLOAD);
    cmdline(UI_S_DOWNLOAD);
    cmdline(UI_S_LISTSERV);
    cmdline(UI_S_LISTCLI);
    cmdline(UI_S_SYNCDIR);
    cmdline(UI_S_EXIT);
    cmdline_input();
    break;

  case UI_CMD_UPLOAD:
    command_to_main = CODE_UPLOAD;
    break;

  case UI_CMD_DOWNLOAD:
    command_to_main = CODE_DOWNLOAD;
    break;

  case UI_CMD_LISTSERV:
    command_to_main = CODE_LISTSERV;
    break;

  case UI_CMD_LISTCLI:
    command_to_main = CODE_LISTCLI;
    break;

  case UI_CMD_SYNCDIR:
    command_to_main = CODE_SYNCDIR;
    break;

  case UI_CMD_EXIT:
    std::cout << "User called exit, will now terminate connection;" ;
    command_to_main = CODE_EXIT;
    break;

  case UI_MISSING_PAR:
    std::cout << "No filename provided. Please, when using upload and download input filename after the command;" ;
    cmdline_input();
    break;

  case UI_DEFAULT:
    cmdline_input();
    break;

  default:
    std::cout << "BAD CALL;" ;
    break;
  }

  return command_to_main;
}

void ClientUI::cmdline_input()
{
  std::cout << "\n[" DROPBOX_COLOR "DboxClient" STANDARD_COLOR "][" INPUT_COLOR "INPUT" STANDARD_COLOR "]: ";
  std::getline(std::cin, new_command);
  skip_new_line = true;
  cmd_selector(new_command);
}

void ClientUI::cmd_selector(std::string command)
{

  if (!command.compare(CMD_HELP))
  {
    cmdline(UI_CMD_HELP);
    return;
  }

  if (!command.compare(0, 6, CMD_UPLOAD))
  {
    if (command.size() <= 8)
    {
      cmdline(UI_MISSING_PAR);
      return;
    }
    input_parameter = command.substr(7);
    cmdline(UI_CMD_UPLOAD);
    return;
  }

  if (!command.compare(0, 8, CMD_DOWNLOAD))
  {
    if (command.size() <= 10)
    {
      cmdline(UI_MISSING_PAR);
      return;
    }
    input_parameter = command.substr(9);
    cmdline(UI_CMD_DOWNLOAD);
    return;
  }

  if (!command.compare(CMD_LISTSERV))
  {
    cmdline(UI_CMD_LISTSERV);      
    return;
  }

  if (!command.compare(CMD_LISTCLI))
  {
    cmdline(UI_CMD_LISTCLI);
    return;
  }

  if (!command.compare(CMD_SYNCDIR))
  {
    cmdline(UI_CMD_SYNCDIR);
    return;
  }

  if (!command.compare(CMD_EXIT))
  {
    cmdline(UI_CMD_EXIT);
    return;
  }		

  cmdline(UI_CMD_UNKNOWN);
  return;

}

void ClientUI::success(std::string message)
{
  std::cout << "\n";
  std::cout << "[" DROPBOX_COLOR "DboxClient" STANDARD_COLOR "][" SUCCESS_COLOR "SUCCESS" STANDARD_COLOR "]: ";
  std::cout << message;
}

void ClientUI::success(int succ_code, std::string message1, std::string message2, std::string message3)
{
  std::cout << "\n";
  std::cout << "[" DROPBOX_COLOR "DboxClient" STANDARD_COLOR "][" SUCCESS_COLOR "SUCCESS" STANDARD_COLOR "]: ";

  switch (succ_code)
  {

  case SUCC_ARGS:
    std::cout << "Received succesfully the following inputs" ;
    success(SUCC_ARG1, message1, message2, message3);
    success(SUCC_ARG2, message1, message2, message3);
    success(SUCC_ARG3, message1, message2, message3);
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

  case SUCC_LOGIN:
    std::cout << "You connected successfully to the server.";
    break;

  case SUCC_TERMINATE:
    std::cout << "Thank you. Dropbox will now terminate.\n\n";
    break;

  default:
    std::cout << "BAD CALL;" ;
    break;

  }

  return;
}


void ClientUI::warning(std::string message)
{
  std::cout << "\n";
  std::cout << "[" DROPBOX_COLOR "DboxClient" STANDARD_COLOR "][" WARNING_COLOR "WARNING" STANDARD_COLOR "]: " ;
  std::cout << message;
}

void ClientUI::warning(int warn_code, std::string message1, std::string message2, std::string message3)
{
  switch (warn_code)
  {
  case WAR_LOGIN_SESS:
    warning("There is already the maximum of two sessions of this client connected to the server.");
    break;

  default:
    warning("BAD CALL;");
    break;
  }
}

void ClientUI::error(std::string message)
{
  std::cout << "\n";
  std::cout << "[" DROPBOX_COLOR "DboxClient" STANDARD_COLOR "][" ERROR_COLOR "ERROR" STANDARD_COLOR "]: " ;
  std::cout << message;
}

void ClientUI::error(int err_code, std::string message1, std::string message2, std::string message3)
{
  std::cout << "\n";
  std::cout << "[" DROPBOX_COLOR "DboxClient" STANDARD_COLOR "][" ERROR_COLOR "ERROR" STANDARD_COLOR "]: " ;

  switch (err_code)
  {

  case ERR_NOFARGS:
    std::cout << "Wrong number of arguments, please use: ./dropboxClient user endereco porta pasta;" ;
    error(ERR_TERMINATE, message1, message2, message3);
    exit(ERR_NOFARGS);
    break;

  case ERR_LOGIN_CONN:
    std::cout << "Could not establish connection to the server." ;
    break;

  case ERR_TERMINATE:
    std::cout << "Dropbox will now terminate.\n\n" ;
    break;

  default:
    std::cout << "BAD CALL;" ;
    break;

  }

  return;
}

std::string ClientUI::get_parameter()
{
  return input_parameter;
}

