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

  ui_cmdline(UI_START);
  ui_success(SUCC_TERMINATE, emptymsg, emptymsg, emptymsg);

  return 0;
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
