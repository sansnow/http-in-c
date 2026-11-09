#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

int main(void) {
  int server = socket(AF_INET, SOCK_STREAM, 0);
  int PORT = 8080;

  if (server < 0) {
    perror("socket");
    exit(1);
  };

  struct sockaddr_in address;
  memset(&address, 0, sizeof(address));

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(server, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind");
    exit(1);
  }

  return 0;
};
