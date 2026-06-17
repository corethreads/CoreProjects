#include "AsciiColors.h"
#include "printBytes.h"
#include <bits/sockaddr.h>
#include <netinet/in.h>
// #include <stdio.h>
#include <dirent.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

void print_my_fds() {
  DIR *d = opendir("/proc/self/fd/");
  if (!d)
    return;

  // Get the internal FD number used by opendir itself
  int opendir_fd = dirfd(d);

  struct dirent *dir;
  int count = 0;

  printf("\n--- Active File Descriptors ---\n");
  while ((dir = readdir(d)) != NULL) {
    if (dir->d_name[0] != '.') {
      int current_fd = atoi(dir->d_name);

      // Skip printing the temporary opendir file descriptor
      if (current_fd == opendir_fd) {
        continue;
      }

      printf("FD Number: %d\n", current_fd);
      count++;
    }
  }
  printf("Total Open FDs (Excluding opendir): %d\n", count);
  closedir(d);
}

void ErrorLog(char *message) {

  printf("%s [ERROR] %s %s\n ", AC_RED, message, AC_WHITE);
}
void SuccessLog(char *message) {
  printf("%s [SUCCESS] %s %s\n", AC_GREEN, message, AC_WHITE);
}
void EchoServer() {
  int socketS, socketC;

  socketS = socket(AF_INET, SOCK_STREAM, 0);
  print_bytes(&socketS, sizeof(socketS));

  struct sockaddr_in Structure_t;
  Structure_t.sin_family = AF_INET;
  Structure_t.sin_port = htons(8080);
  Structure_t.sin_addr.s_addr = INADDR_ANY; // to take any ip 0.0.0.0
  socklen_t Structure_tLen = sizeof(Structure_t);

  if (bind(socketS, (struct sockaddr *)&Structure_t, Structure_tLen) < 0) {

    ErrorLog("Socket Not Bound");

  } else {
    SuccessLog("Socket Bound SuccessFully...");
  }

  if (listen(socketS, 10) < 0) {

    ErrorLog("[corethreads] Error Occurred");

  } else {
    SuccessLog("[corethreads] Socket turned into a passive server");
  }

  struct sockaddr_in Client_Structure;
  socklen_t clientLen = sizeof(Client_Structure);

  while (1) {
    socketC = accept(socketS, (struct sockaddr *)&Client_Structure, &clientLen);
    print_bytes(&socketC, sizeof(socketC));
    if (socketC < 0) {
      ErrorLog("[corethreads] Error Occurred");
    } else {

      SuccessLog("[corethreads] Connection Established[+]..... ");
    }

    char MessageStore[1024] = {0};
    ssize_t ReadFileDescriptor;
    ReadFileDescriptor = read(socketC, MessageStore, 1023);

    if (ReadFileDescriptor < 0) {
      ErrorLog("Nothing Read in File Descriptor");
    } else {
      SuccessLog("File Descriptor read SuccessFully");
    }
  }
}

int main() {
  EchoServer();
  print_my_fds();
}
