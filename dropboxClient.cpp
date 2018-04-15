#include "dropboxClient.h"

int main(int argc, char **argv)
{

  ui_welcome_client(argv[1]);

  if (argc!=4){
    ui_error(ERR_NOFARGS);
  }

  return 0;
}
