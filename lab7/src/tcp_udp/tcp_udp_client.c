#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>

#include <getopt.h>

#define SADDR struct sockaddr
#define SIZE sizeof(struct sockaddr_in)

int main(int argc, char *argv[]) {

  int BUFSIZE = -1;

  int SERV_PORT = -1;
  char *ADDR;
  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"BUFSIZE", required_argument, 0, 0},
                                      {"SERV_PORT", required_argument, 0, 0},
                                      {"ADDR", required_argument, 0, 0},
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
        if (BUFSIZE <= 0) {
          printf("BUFSIZE must be a positive number\n");
          return 1;
        }
        break;
      case 1:
        SERV_PORT = atoi(optarg);
        if (SERV_PORT <= 0) {
          printf("SERV_PORT must be a positive number\n");
          return 1;
        }
        break;
      case 2:
        ADDR = optarg;
        break;
      }

    case '?':
      break;

    default:
      printf("getopt returned character code 0%o?\n", c);
    }
  }
  if (BUFSIZE == -1) {
    printf("Usage: %s --bufsize \"buffer_size\" --serv_port \"serv_port\"\n",
           argv[0]);
    return 1;
  }

  int socketFileDescriptor, connectionFileDescriptor;
  char buf[BUFSIZE];
  struct sockaddr_in servaddr;
  if (argc < 3) {
    printf("Too few arguments \n");
    exit(1);
  }

  if ((socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket creating");
    exit(1);
  }

  memset(&servaddr, 0, SIZE);
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERV_PORT); // htons(atoi(argv[2]));

  if (inet_pton(AF_INET, ADDR, &servaddr.sin_addr) <= 0) {
    perror("bad address");
    exit(1);
  }


  if ((connectionFileDescriptor = connect(socketFileDescriptor, (SADDR *)&servaddr, SIZE)) < 0) {
    perror("connection error");
    exit(1);
  }
  printf("connected to server %s:%d\n", ADDR, SERV_PORT);
  // udp starts

  int messageLength;
  char sendline[BUFSIZE], recvline[BUFSIZE + 1];
  struct sockaddr_in cliaddr;

  if ((socketFileDescriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket problem");
    exit(1);
  }

  printf("Enter udp message:\n");
  while ((messageLength = read(0, sendline, BUFSIZE)) > 0) {
    if (sendto(socketFileDescriptor, sendline, messageLength, 0, (SADDR *)&servaddr, SIZE) == -1) {
      perror("sendto problem");
      exit(1);
    }

    if (recvfrom(socketFileDescriptor, recvline, BUFSIZE, 0, NULL, NULL) == -1) {
      perror("recvfrom problem");
      exit(1);
    }

    printf("Response from server= %s\n", recvline);
  }
  close(socketFileDescriptor);
  exit(0);
}
// ./client --BUFSIZE 1024 --SERV_PORT 10050 --ADDR 127.0.0.1