#include "dropboxClient.h"
#include "CheckFileChangesDaemonThread.h"


ClientConnectionManager manager;
ClientUI ui;
std::string username;
char* endereco;
char* porta;
char* pasta;
std::string emptymsg;

int command;

bool running;


int main(int argc, char **argv)
{

  ui.welcome_client(argv[1]);

  if (argc!=5)
  {
    ui.error(ERR_NOFARGS, emptymsg, emptymsg, emptymsg);
  }
  else
  {
    set_username(argv[1]);
    set_endereco(argv[2]);
    set_porta(argv[3]);
    set_pasta(argv[4]);
    ui.success(SUCC_ARGS, username, endereco, porta);
  }

  login_server((char *)username.c_str(), endereco, atoi(porta));

  running = true;
  while(running)
  {
    command = ui.cmdline(UI_START);
    command_solver(command);
  }

  return 0;
}

int login_server(char* username, char* host, int port)
{
  int result = manager.login_server(username, host, port);
  if (result == 0)
  {
    ui.success(SUCC_LOGIN, emptymsg, emptymsg, emptymsg);

    // Create folder if needed
    std::string path = std::string(pasta) + "/sync_dir_" + std::string(username);
    std::string command = "mkdir -p " + path;
    system(command.c_str());

    // Start Daemon
    CheckFileChangesDaemonThread checkFileChangesDaemonThread = CheckFileChangesDaemonThread(manager);
    std::thread([&checkFileChangesDaemonThread, path](){
      checkFileChangesDaemonThread.run(path);
    }).detach();
    return LOGIN_TRUE;
  } 
  else 
  {
    ui.error(ERR_LOGIN_CONN, emptymsg, emptymsg, emptymsg);
    return LOGIN_FALSE;
  }
}

void sync_client()
{
  manager.sync_client();
}

void send_file(char* file)
{
  manager.send_file(file);
}

void get_file(char* file)
{
  manager.get_file(file);
}

void delete_file(char* file)
{
  manager.delete_file(file);
}

void close_session()
{
  auto response = manager.logout();
  if (response == 0)
  {
    ui.success(SUCC_TERMINATE, emptymsg, emptymsg, emptymsg);
  }
  else
  {
    ui.warning("Server did not acknowledged the logout");
  }
  //checkFileChangesDaemonThread.kill();
  running = false;
}

void command_solver(int command)
{

  switch (command)
  {

  case CODE_UPLOAD:
    send_file((char *)ui.get_parameter().c_str());
    break;

  case CODE_DOWNLOAD:
    get_file((char *)ui.get_parameter().c_str());
    break;

  case CODE_LISTSERV:
    //close_session();
    break;

  case CODE_LISTCLI:
    //close_session();
    break;

  case CODE_SYNCDIR:
    sync_client();
    break;

  case CODE_EXIT:
    close_session();
    break;

  default:
    break;

  }
}

void set_username(char* arg)
{
  if (sizeof(arg) <= MAXNAME)
  {
    username = arg;
  }
}

void set_endereco(char* arg)
{
  endereco = arg;
}

void set_porta(char* arg)
{
  porta = arg;
}

void set_pasta(char* arg)
{
  pasta = arg;
}
