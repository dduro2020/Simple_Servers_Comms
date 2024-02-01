#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <err.h>

#define BUF_SIZE        500  

 
/* This clients connects, sends a text and disconnects */
int main(int argc, char *argv[]) { 

    if (argc != 4) {
        err(EXIT_FAILURE, "%s usage: [client_id] [ip] [port]\n", argv[0]);
    }

    char buf_tx[BUF_SIZE] = "Hello server! From client: ";     
    char buf_rx[BUF_SIZE];                     /* receive buffer */

    int sockfd, port = 0; 
    struct sockaddr_in servaddr;

    setbuf(stdout, NULL);
    memset(&buf_rx, '\0', sizeof(buf_rx));
    strcat(buf_tx, argv[1]);
    strcat(buf_tx, "\n");

    /* Socket creation */
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        err(EXIT_FAILURE, "error: socket creation failed\n");
    } 
    
    printf("Socket successfully created...\n"); 
    
    memset(&servaddr, 0, sizeof(servaddr));

    /* assign IP, PORT */
    port = strtol(argv[3], NULL, 10);
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr(argv[2]); 
    servaddr.sin_port = htons(port); 
  
    /* try to connect the client socket to server socket */
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) { 
        err(EXIT_FAILURE, "error: connection with server failed\n");
    } 
    
    printf("connected to the server...\n"); 

    
    /* send test sequences*/

    if (send(sockfd, buf_tx, sizeof(buf_tx), 0) == -1) {
        err(EXIT_FAILURE, "error: failure sending message\n");
    }

    if(recv(sockfd, buf_rx, sizeof(buf_rx), 0) == -1) {
        err(EXIT_FAILURE, "error: failure receiving message\n");
    }

    printf("+++ %s", buf_rx);
   
    
    /* close the socket */
    close(sockfd); 
}