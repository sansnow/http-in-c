#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

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

  int yes = 1;
  setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

  if (bind(server, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind");
    exit(1);
  };

  if (listen(server, 10) < 0) {
    perror("listen");
    exit(1);
  };

  printf("listening on port %d \n", PORT);

  while (1) {
    int client = accept(server, NULL, NULL);
    if (client < 0) {
      perror("accept");
      exit(1);
    }

    printf("client fd %d \n", client);

    char request[4096];
    ssize_t n = read(client, request, sizeof(request) - 1);

    if (n <= 0) {
      close(client);
      continue;
    }

    request[n] = '\0';

    char method[8], path[256];
    if (sscanf(request, "%7s %255s", method, path) != 2) {
      close(client);
      continue;
    }

    printf("%s %s\n", method, path);

    char *response = "HTTP/1.0 200 OK\r\n"
                     "Content-Type: text/html\r\n"
                     "\r\n"
                     "<h1>hi supp gng</h1>"
                     "<h1>how's it been?</h1>";

    write(client, response, strlen(response));
    close(client);
  }

  return 0;
};
