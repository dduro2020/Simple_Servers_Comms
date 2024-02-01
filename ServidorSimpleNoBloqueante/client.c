#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <err.h>
#include <sys/select.h>

#define SERVER_ADDRESS  "127.0.0.1"     /* server IP */
#define PORT            8080 
#define BUF_SIZE        500  
 
/* This clients connects, sends a text and disconnects */
int main() {
     
    char buf_tx[BUF_SIZE];      
    char buf_rx[BUF_SIZE];                     /* receive buffer */
    int sockfd; 
    struct sockaddr_in servaddr; 
    fd_set readmask;
    struct timeval timeout;

    setbuf(stdout, NULL);
    memset(&buf_tx, '\0', sizeof(buf_tx));
    memset(&buf_rx, '\0', sizeof(buf_rx));

    /* Socket creation */
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        err(EXIT_FAILURE, "error: socket creation failed\n");
    } 
    
    printf("Socket successfully created...\n"); 
    
    memset(&servaddr, 0, sizeof(servaddr));

    /* assign IP, PORT */
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr( SERVER_ADDRESS ); 
    servaddr.sin_port = htons(PORT); 
  
    /* try to connect the client socket to server socket */
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) { 
        err(EXIT_FAILURE, "error: connection with server failed\n");
    } 
    
    printf("connected to the server...\n"); 

    while(1) {
        /* send test sequences*/
        printf("> ");
        fgets(buf_tx, sizeof(buf_tx), stdin);

        if (send(sockfd, buf_tx, sizeof(buf_tx), 0) == -1) {
            err(EXIT_FAILURE, "error: failure sending message\n");
        }

        FD_ZERO(&readmask); /*mask reset*/
        FD_SET(sockfd, &readmask); /*new descriptor*/
        timeout.tv_sec = 0; timeout.tv_usec = 500000; /* Timeout de 0.5 seg.*/

        if (select(sockfd+1, &readmask, NULL, NULL, &timeout) == -1) {
            err(EXIT_FAILURE, "error: failure waiting time\n");;
        }

        if (FD_ISSET(sockfd, &readmask)){
            if(recv(sockfd, buf_rx, sizeof(buf_rx), 0) == -1) {
                err(EXIT_FAILURE, "error: failure receiving message\n");
            }

            printf("+++ %s", buf_rx);
        }
   
    }
    /* close the socket */
    close(sockfd); 
}