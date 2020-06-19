#include <getopt.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

//#define SERV_PORT 10050
//#define BUFSIZE 100
#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)

int main(int argc, char *argv[]) {
  int SERV_PORT = -1;
  int BUFSIZE = -1;

  while (1) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"BUFSIZE", required_argument, 0, 0},
                                      {"SERV_PORT", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0:
      switch (option_index) {
      case 0:
        BUFSIZE = atoi(optarg);
        if (BUFSIZE < 1) {
          BUFSIZE = -1;
          return 1;
        }
        break;
      case 1:
        SERV_PORT = atoi(optarg);
        if (SERV_PORT < 0) {
          SERV_PORT = -1;
          return 1;
        }
        break;
      defalut:
        printf("Index %d is out of options\n", option_index);
      }
      break;
    case '?':
      break;

    default:
      printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (BUFSIZE == -1 || SERV_PORT == -1) {
    printf("Usage: %s --BUFSIZE 100 --SERV_PORT 10050 \n", argv[0]);
    return 1;
  }

  int socketFileDescriptor, connectionFileDescriptor;
  char buf[BUFSIZE];
  unsigned int address_length = (unsigned int)sizeof(struct sockaddr_in);
  struct sockaddr_in serverAddress;
  struct sockaddr_in clientAddress;

  if ((socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }

  memset(&serverAddress, 0, address_length);
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddress.sin_port = htons(SERV_PORT);

  if (bind(socketFileDescriptor, (SADDR *)&serverAddress, address_length) < 0) {
    perror("bind");
    exit(1);
  }

  if (listen(socketFileDescriptor, 5) < 0) {
    perror("listen");
    exit(1);
  }

  if ((connectionFileDescriptor =
           accept(socketFileDescriptor, (SADDR *)&clientAddress,
                  &address_length)) < 0) {
    perror("accept");
    exit(1);
  }
  printf("connection established\n");
  // udp starts
  int messageLength;
  char message[BUFSIZE], ipadr[16];

  if ((socketFileDescriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket problem");
    exit(1);
  }

  if (bind(socketFileDescriptor, (SADDR *)&serverAddress, address_length) < 0) {
    perror("bind problem");
    exit(1);
  }
  printf("Accepting udp starts...\n");
  int endFlag = 0;
  pid_t child_pid = fork();
  if (child_pid < 0) {
    printf("fork failed");
    exit(1);
  }
  if (child_pid == 0) {
    while (1) {
      if ((messageLength = recvfrom(socketFileDescriptor, message, BUFSIZE, 0,
                                    (SADDR *)&clientAddress, &address_length)) <
          0) {
        perror("recvfrom");
        exit(1);
      } else {
        endFlag++;
      }

      printf("Request message: %s\n Accepted %d chars\n", message,
             messageLength);

      if (endFlag > 4) {
        close(socketFileDescriptor);
      }
    }
  }
  close(connectionFileDescriptor);
}
// ./server --BUFSIZE 1024 --SERV_PORT 10050