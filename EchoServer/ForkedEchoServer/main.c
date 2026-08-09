#include "server_utils.h"
#include <signal.h>


int main(void){
  signal(SIGINT | SIGCHLD, process_handler);
  int server_socket = create_server_socket();
  bind_server_socket(server_socket);
  start_listening(server_socket);

  run_echo_server(server_socket);
  server_cleanup(server_socket);

  return 0;
}
