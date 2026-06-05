#include "AsciiColors.h"
#include "printBytes.h"
#include <bits/sockaddr.h>
#include <errno.h>
#include <netinet/in.h>
// #include <stdio.h>
#include <stdio.h>
#include <sys/socket.h>

void EchoServer() {
  int socketS, socketC;

  socketS = socket(AF_INET, SOCK_STREAM, 0);
  print_bytes(&socketS, sizeof(socketS));

  struct sockaddr_in Structure_t;
  Structure_t.sin_family = AF_INET;
  Structure_t.sin_port = 8080;
  Structure_t.sin_addr.s_addr = INADDR_ANY; // to take any ip 0.0.0.0

  if (bind(socketS, (struct sockaddr *)&Structure_t, sizeof(Structure_t)) !=
          -1 ||
      errno) {
    printf("%s Socket bind created successfully \n", AC_GREEN);

  } else {
    printf("%s Error Occurred \n", AC_RED);
  }

  if (listen(socketS, 10) != -1 || errno) {
    printf("%s Socket turned into a passive server\n", AC_GREEN);
  } else {
    printf("%s Error Occurred >-<\n", AC_RED);
  }
}

int main() { EchoServer(); }
