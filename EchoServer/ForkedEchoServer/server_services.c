#include "server_utils.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "ASCIIcolors.h"
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>

void error_log(char *message){
  printf(" %s [error] %s %s %s\n",AC_RED,message, strerror(errno), AC_NORMAL);

}

void success_log(char *message){
  printf("%s [sucess] %s %s\n", AC_GREEN, message, AC_NORMAL);
}


int create_server_socket(void){
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  
  if(server_socket < 0){
    error_log("Failed to create socket");
    return -1;
  }
  
  success_log("Server socket created");
  int option_value = 1;
  socklen_t option_valuesize = sizeof(option_value);

  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option_value, option_valuesize) < 0 ){
    error_log("Descriptor not Modified");
  };

  return server_socket;
}

int bind_server_socket(int server_socket){

  struct sockaddr_in server_address;
  socklen_t address_length = sizeof(server_address);

  server_address.sin_addr.s_addr =  INADDR_ANY;
  server_address.sin_port = htons(SOCKET_PORT);
  server_address.sin_family = AF_INET;

  int bindSocket = bind(server_socket, (struct sockaddr *)&server_address, address_length);

  if(bindSocket < 0){
    error_log("Failed to bind Socket");
    return -1;
  }

  success_log("Socket Bound...");
  return 0;

}

int start_listening(int server_socket){
  if(listen(server_socket, CONNECTION_LENGTH) < 0){
    error_log("Error Occured");
    return -1;
  }

  success_log("Server Socket turned Passive");
  return 0;
  
}

int accept_client_connections(int server_socket) {
  struct sockaddr_in client_address;
  socklen_t clientaddr_len = sizeof(client_address);

  int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &clientaddr_len);
  if(client_socket < 0){
    error_log("Error Occured");
    return -1;
  }

  success_log("[+] Connection Established...");
  return client_socket;
}

ssize_t recieve_client_message(int client_socket, char *messageBuffer, size_t buffer_size){
  ssize_t bytes_recieved = read(client_socket, messageBuffer, buffer_size);
  if(bytes_recieved <0){
   error_log("Failed to read client message");
    return -1;
  }
  success_log("Message recieved from client");
  printf("Message: %s\n ", messageBuffer);
  return bytes_recieved;

}

ssize_t write_to_client(int client_socket, char *message,
 size_t message_len){
  ssize_t bytes_written = write(client_socket,message ,message_len); 
  if(bytes_written < 0){
    error_log("Failed writing to client");
    return -1;
  }

  success_log("Message written to Client");
  printf("Message: %s\n", message);
  return bytes_written; 
}

void handle_client_communication(int client_socket){
  char messageBuffer[MESSAGE_BUFFER] = {0};

  ssize_t bytesRecieved = recieve_client_message(client_socket,messageBuffer, BYTES_TO_READ);

  if(bytesRecieved > 0){
    write_to_client(client_socket, messageBuffer, bytesRecieved);
  
  }

  close(client_socket);
  success_log("Client Socket Closed");
}

int run_echo_server(int server_socket){
  printf("Forked Echo Server\n");

  while (1) {
    int client_socket = accept_client_connections(server_socket);

    pid_t childForks = fork();
    
    if(childForks < 0){
      error_log("Error Occured in Forks..");
      exit(1);
    }
    if(childForks == 0){
    handle_client_communication(client_socket);
    printf("Child's PID: %d , PPID %d\n",getpid(), getppid() );

    }
  }
  
  return 0;
 
}

void server_cleanup(int server_socket){
  close(server_socket);
  success_log("Server Closed SuccessFully");
}

