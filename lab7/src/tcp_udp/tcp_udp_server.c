#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <getopt.h>
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

  const size_t kSize = sizeof(struct sockaddr_in);

  int lfd, cfd;
  int nread;
  char buf[BUFSIZE];
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;

  if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }

  memset(&servaddr, 0, kSize);
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);

  if (bind(lfd, (SADDR *)&servaddr, kSize) < 0) {
    perror("bind");
    exit(1);
  }

  if (listen(lfd, 5) < 0) {
    perror("listen");
    exit(1);
  }

//   while (1) {
    unsigned int clilen = kSize;

    if ((cfd = accept(lfd, (SADDR *)&cliaddr, &clilen)) < 0) {
      perror("accept");
      exit(1);
    }
    printf("connection established\n");
    close(cfd);
    // udp starts
    
    int sockfd, n;
    char mesg[BUFSIZE], ipadr[16];

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
      perror("socket problem");
      exit(1);
    }

    memset(&servaddr, 0, SLEN);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    if (bind(sockfd, (SADDR *)&servaddr, SLEN) < 0) {
      perror("bind problem");
      exit(1);
    }
    printf("Accepting udp starts...\n");

    while (1) {
      unsigned int len = SLEN;

      if ((n = recvfrom(sockfd, mesg, BUFSIZE, 0, (SADDR *)&cliaddr, &len)) <
          0) {
        perror("recvfrom");
        exit(1);
      }
      mesg[n] = 0;

      printf("REQUEST %s      FROM %s : %d\n", mesg,
             inet_ntop(AF_INET, (void *)&cliaddr.sin_addr.s_addr, ipadr, 16),
             ntohs(cliaddr.sin_port));

      if (sendto(sockfd, mesg, n, 0, (SADDR *)&cliaddr, len) < 0) {
        perror("sendto");
        exit(1);
      }
    }
}
// ./server --BUFSIZE 1024 --SERV_PORT 10050