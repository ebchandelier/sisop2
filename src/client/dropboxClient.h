#include "ClientUI.h"
#include "ClientConnectionManager.h"
// #include "CheckFileChangesDaemonThread.cpp"
#include <string>

#define MAXNAME		50

#define LOGIN_TRUE	1
#define LOGIN_FALSE	0


//ClientConnectionManager manager;

// CheckFileChangesDaemonThread checkFileChangesDaemonThread;
// ClientUI ui;
// std::string username;
// char* endereco;
// char* porta;

// std::string emptymsg;

// int command;

//Estabelece uma sessão entre o cliente com o servidor.
int login_server(char* username, char* host, int port);

//Sincroniza o diretório “sync_dir_<nomeusuário>” com o servidor.
void sync_client();

//Envia um arquivo file para o servidor. Deverá ser executada quando for realizar upload de um arquivo.
void send_file(char* file);

//Obtém um arquivo file do servidor. Deverá ser executada quando for realizar download de um arquivo. file – filename.ext
void get_file(char* file);

//Exclui um arquivo file de “sync_dir_<nomeusuário>”. file –filename.ext
void delete_file(char* file);	

//Fecha a sessão com o servidor.
void close_session();

//Recebe codigo e executa comando
void command_solver(int command);

//Valida tamanho do usuario
void set_username(char* name);

//Valida endereco
void set_endereco(char* name);

//Valida porta
void set_porta(char* name);

void set_pasta(char* name);