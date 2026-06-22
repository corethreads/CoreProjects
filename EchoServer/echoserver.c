#include "AsciiColors.h"
#include "printBytes.h"
#include <bits/posix1_lim.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// CONSTANTS
#define SOCKET_PORT 4040
#define CONNECTION_LENGTH 10
#define MESSAGE_BUFFER 1024
#define BYTES_TO_READ (MESSAGE_BUFFER - 1)

// LOGGING PROGRAMS
void error_log(char *message) {
  printf("%s [ERROR] %s %s %s\n", AC_RED, message, strerror(errno), AC_WHITE);
};

void success_log(char *message) {
  printf("%s [SUCCESS] %s %s\n", AC_GREEN, message, AC_WHITE);
};

static int create_server_socket(void) {
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);

  if(server_socket < 0){
    error_log("Failed to create socket");
    return -1;
  }

  success_log("Server Socket created SuccessFully");
  print_bytes(&server_socket, sizeof(server_socket));

  return server_socket;
};

static int bind_server_socket(int server_socket){
  

  struct sockaddr_in server_address;
  socklen_t address_length = sizeof(server_address);

  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(SOCKET_PORT);
  
  if(bind(server_socket, (struct sockaddr *)&server_address, address_length) < 0){
    error_log("Failed to bind to Socket");
    return -1;
  }

  success_log("Socket Bound SuccessFully");
  return 0; 
};

static int start_listening(int server_socket){
  
  
  if(listen(server_socket, CONNECTION_LENGTH) < 0 ){
    error_log("Error Occured");
    return -1;
  }

  success_log("Server Socket turned Passive SuccessFully");
  return 0;
}

static int accept_client_connections(int server_socket){
  
  
  struct sockaddr_in client_address;
  socklen_t client_length = sizeof(client_address);

  int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_length);
  if (client_socket < 0) {
    error_log("Error Occured");
    return -1;  
  }

  success_log("[+] Connection Established...");
  return client_socket; 
}

static ssize_t recieve_client_message(int client_socket, char *messageBuffer, size_t buffer_size) {
  ssize_t bytes_recieved = read(client_socket, messageBuffer, buffer_size);
  if (bytes_recieved < 0) {
    error_log("Failed to read client Message");
    return -1;
  }

  success_log("Messages recieved from client");
  return bytes_recieved;
}

static ssize_t write_to_client(int client_socket, char *message, size_t message_length) {
  ssize_t bytes_written = write(client_socket, message, message_length);
  if (bytes_written < 0) {
    error_log("Failed writing to client");
    return -1;
  }

  success_log("Message Written to Client");
  return bytes_written;
}

static void handle_client_connection(int client_socket){
  char message_buffer[MESSAGE_BUFFER] = {0};

  ssize_t bytes_recieved = recieve_client_message(client_socket, message_buffer, BYTES_TO_READ);

  if (bytes_recieved > 0) {
    write_to_client(client_socket, message_buffer, bytes_recieved);
  }

  close(client_socket);
  success_log("Client Socket Closed");
}

static void run_echo_server(int server_socket){
  success_log("Echo Server Running");

  while (1) {
    int client_socket = accept_client_connections(server_socket);
    
    
    handle_client_connection(client_socket);
    
  }
};

static void cleanup_server(int server_socket) {
  close(server_socket);
  success_log("Server Socket Closed SuccessFully");  
};

int main(void) {
  int server_socket = create_server_socket();

  if(server_socket < 0){
    return 1;
  }

  if(bind_server_socket(server_socket) < 0){
    close(server_socket);
    return 1;
  }

  if(start_listening(server_socket) < 0) {
    close(server_socket);
    return 1;
  }

  run_echo_server(server_socket);
  cleanup_server(server_socket);
  return 0;
};
