#include "dropboxClient.h"

int main(int argc, char **argv)
{

  ui_welcome_client(argv[1]);

  if (argc!=4)
    ui_error(ERR_NOFARGS, emptymsg, emptymsg, emptymsg);
  else
  {
    set_username(argv[1]);
    set_endereco(argv[2]);
    set_porta(argv[3]);
    ui_success(SUCC_ARGS, username, endereco, porta);
  }

  login_server(endereco, atoi(porta));

  command = ui_cmdline(UI_START);
  command_solver(command);

  return 0;
}

int login_server(char* host, int port)
{
  int result = manager.login_server(host, port);
  if (result == 0)
  {
    ui_success(SUCC_LOGIN, emptymsg, emptymsg, emptymsg);
    return LOGIN_TRUE;
  } else {
    ui_error(ERR_LOGIN_CONN, emptymsg, emptymsg, emptymsg);
    return LOGIN_FALSE;
  }
}

void sync_client()
{
}

void send_file(char* file)
{
}

void get_file(char* file)
{
}

void delete_file(char* file)
{
}

void close_session()
{
  ui_success(SUCC_TERMINATE, emptymsg, emptymsg, emptymsg);
  return;
}

void command_solver(int command)
{

  switch (command)
  {

  case CODE_UPLOAD:
    //send_file();
    break;

  case CODE_DOWNLOAD:
    //get_file();
    break;

  case CODE_LISTSERV:
    //close_session();
    break;

  case CODE_LISTCLI:
    //close_session();
    break;

  case CODE_SYNCDIR:
    //sync_client();
    break;

  case CODE_EXIT:
    close_session();
    break;

  default:
    ui_cmdline_input();
    break;

  }

  return;

}

void set_username(char* arg)
{
  if (sizeof(arg) <= MAXNAME)
    username = arg;
  return;
}

void set_endereco(char* arg)
{
  endereco = arg;
  return;
}

void set_porta(char* arg)
{
  porta = arg;
  return;
}
