#include "Client.h"


int Client::main(std::string user, std::string dir, std::vector<rm> servers)
{

  ui.welcome_client((char*) user.c_str());

  set_username((char*) user.c_str());
  set_endereco((char*)servers[0].ip.c_str());
  set_porta((char *)std::to_string(servers[0].port).c_str());
  set_pasta((char*) dir.c_str());
  ui.success(SUCC_ARGS, username, endereco, porta);
  

  login_server((char *)username.c_str(), endereco, atoi(porta));

  running = 2;
  while(running)
  {
    if (running == 2)
    {
      command = ui.cmdline(UI_START);
      running = 1;
    } else
      command = ui.cmdline(UI_DEFAULT);
    command_solver(command);
  }

  return 0;
}


int Client::login_server(char* username, char* host, int port)
{
  int result = manager.login_server(username, host, port);
  if (result == 0)
  {
    ui.success(SUCC_LOGIN, emptymsg, emptymsg, emptymsg);

    // Create folder if needed
    std::string path = std::string(pasta) + "/sync_dir_" + std::string(username);
    std::string command = "mkdir -p " + path;
    system(command.c_str());
    manager.work_dir = path;

    // Do initial sync
    // TODO: This should be another method, with another resolution algorithm
    //manager.sync_client();
    //device_files = ListFiles::listFilesAt(pasta);
    

    // Start synchronizer daemon
    std::thread([&]() {
      ClientFilesSynchronizer().run(manager);
    }).detach();
    
    // Start folder watcher daemon
    std::thread([&, path](){
      auto daemon = CheckFileChangesDaemonThread();
      daemon.manager = &manager;
      daemon.run(path, device_files);
    }).detach();
    
    return LOGIN_TRUE;
  } 
  else 
  {
    ui.error(ERR_LOGIN_CONN, emptymsg, emptymsg, emptymsg);
    return LOGIN_FALSE;
  }
}

void Client::sync_client()
{
  manager.sync_client();
}

void Client::send_file(char* file)
{
  manager.send_file(file);
}

void Client::get_file(char* file)
{
  manager.get_file(file);
}

void Client::delete_file(char* file)
{
  manager.delete_file(file);
}

void Client::close_session()
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

void Client::command_solver(int command)
{
  DeviceFilesInfo files;
  std::string absolutePath = std::string(realpath(pasta, NULL)) + "/sync_dir_" + std::string(username);

  switch (command)
  {

  case CODE_UPLOAD:
    send_file((char *)ui.get_parameter().c_str());
    break;

  case CODE_DOWNLOAD:
    get_file((char *)ui.get_parameter().c_str());
    break;

  case CODE_LISTSERV:
  {
    DeviceFilesInfo s = manager.sendListFilesRequest();
    ListFiles::print(s);
    break;
  }

  case CODE_LISTCLI:
    ListFiles::print(device_files);
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

void Client::set_username(char* arg)
{
  if (sizeof(arg) <= MAXNAME)
  {
    username = arg;
  }
}

void Client::set_endereco(char* arg)
{
  endereco = arg;
}

void Client::set_porta(char* arg)
{
  porta = arg;
}

void Client::set_pasta(char* arg)
{
  pasta = arg;
}
