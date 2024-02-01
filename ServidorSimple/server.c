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

/* server parameters */
#define SERV_PORT       8080              /* port */
#define BUF_SIZE        500               /* Buffer rx, tx max size  */
#define BACKLOG         1                /* Max. client pending connections  */

int main(int argc, char* argv[]) {          /* input arguments are not used */

    int sockfd, connfd ;  /* listening socket and connection socket file descriptors */
    unsigned int len;     /* length of client address */
    struct sockaddr_in servaddr, client; 
    
    int  len_rx, len_tx, nr = 0;                     /* received and sent length, in bytes */
    char buff_tx[BUF_SIZE];
    char buff_rx[BUF_SIZE];   /* buffers for reception  */
    
    memset(&buff_tx, '\0', sizeof(buff_tx));
    memset(&buff_rx, '\0', sizeof(buff_rx));
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
    servaddr.sin_family      = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port        = htons(SERV_PORT); 
    
    
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
    
    len = sizeof(client); 
  
      /* Accept the data from incoming socket*/
    
    connfd = accept(sockfd, (struct sockaddr *)&client, &len); 
    if (connfd < 0) { 
        err(EXIT_FAILURE, "error: connection not accepted\n");
    } 
    else {              
        while(1) {  /* read data from a client socket till it is closed */ 
        
            /* read client message, copy it into buffer */ 
            if (recv(connfd, buff_rx, sizeof(buff_rx), 0) == -1) {
                err(EXIT_FAILURE, "error: failure receiving message\n");
            }
            
            printf("+++ %s", buff_rx); 

            printf("> ");
            fgets(buff_tx, sizeof(buff_tx), stdin);

            if (send(connfd, buff_tx, sizeof(buff_tx), 0) == -1) {
                err(EXIT_FAILURE, "error: failure sendind message\n");
            }
                        
        }  
    }                      
        
} 
