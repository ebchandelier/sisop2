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

  command = ui_cmdline(UI_START);
  command_solver(command);

  return 0;
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
