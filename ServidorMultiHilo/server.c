#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>  
#include <err.h>
#include <pthread.h>
#include <unistd.h>

/* server parameters */
#define BUF_SIZE        500               /* Buffer rx, tx max size  */
#define BACKLOG         100                /* Max. client pending connections  */
#define MAXTHREAD       100

void * handle_connection(void *p_client_socket) {
    int *client_socket = p_client_socket;

    char buff_tx[BUF_SIZE] = "Hello client!\n";
    char buff_rx[BUF_SIZE];   /* buffers for reception  */
    
    memset(&buff_rx, '\0', sizeof(buff_rx)); 
    
    /* read client message, copy it into buffer */ 
    if (recv(*client_socket, buff_rx, sizeof(buff_rx), 0) == -1) {
        err(EXIT_FAILURE, "error: failure receiving message\n");
    }
    /*print message*/
    printf("> %s", buff_rx); 
    /*send message*/
    if (send(*client_socket, buff_tx, sizeof(buff_tx), 0) == -1) {
        err(EXIT_FAILURE, "error: failure sendind message\n");
    }

}

int main(int argc, char *argv[]) { 

    if (argc != 2) {
        err(EXIT_FAILURE, "%s usage: [port]\n", argv[0]);
    }

    int sockfd;
    int connfd[MAXTHREAD];  /* listening socket and connection socket file descriptors */
    unsigned int len;     /* length of client address */
    struct sockaddr_in servaddr, client;
    pthread_t thread[MAXTHREAD]; /*max threads at the same time*/
    int port, i, n= 0;
    const int enable = 1;
    
    setbuf(stdout, NULL);

    /* socket creation */
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        err(EXIT_FAILURE, "error: socket creation failed\n");
    } 

    printf("Socket successfully created...\n"); 

    /* clear structure */
    memset(&servaddr, 0, sizeof(servaddr));
  
    /* assign IP, SERV_PORT, IPV4 */
    port = strtol(argv[1], NULL, 10);
    servaddr.sin_family      = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port        = htons(port); 
    
    
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        err(EXIT_FAILURE, "error: setsockopt(SO_REUSEADDR) failed\n");
    }
    
    /* Bind socket */
    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) { 
        err(EXIT_FAILURE, "error: socket bind failed\n");
    } 

    printf("Socket successfully binded...\n");
  
    /* Listen */
    if ((listen(sockfd, BACKLOG)) != 0) { 
        err(EXIT_FAILURE, "error: socket listen failed\n");
    }
        
    printf("Server listening...\n"); 
    
  
    while(1) {  /* read data from a client socket till it is closed *               
    
        /* Accept the data from incoming socket*/
        len = sizeof(client);
        connfd[n] = accept(sockfd, (struct sockaddr *)&client, &len); 

        if (connfd[n] < 0) { 
            err(EXIT_FAILURE, "error: connection not accepted\n");
        }

        /*threads creation*/
        if (pthread_create(&thread[n], NULL, handle_connection, &connfd[n]) != 0) {
            err(EXIT_FAILURE, "error: cannot create thread\n");
        }

        n = n + 1;

        if (n == 100) {
            /*closing sockets*/
            for (i = 0; i < MAXTHREAD; i++) {
                if (pthread_join(thread[i], NULL) != 0) {
                    err(EXIT_FAILURE, "error: cannot join thread\n");
                }
                close(connfd[i]); 
            }
            n = 0;
        }
    }                      
} 