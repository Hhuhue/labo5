
// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include "wave.h"
  
#define PORT     8080 
#define MAXLINE 1024 
  
// Driver code 
int main() { 
    int sockfd; 
    char buffer[MAXLINE]; 
    char *hello = "Hello from server"; 
    struct sockaddr_in servaddr, cliaddr; 
      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT); 
      
    // Bind the socket with the server address 
    if (bind(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    int len, n; 
  
    len = sizeof(cliaddr);  //len is value/resuslt 
  
    n = recvfrom(sockfd, (long*)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr*) &cliaddr, &len); 
    long* sample_size = (long*) (buffer); 
    printf("Client header: size sample is %ld \n", *sample_size); 

    n = recvfrom(sockfd, (long*)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr*) &cliaddr, &len); 
    long* num_samples = (long*) (buffer); 
    printf("Client header: num sample is %ld \n", *num_samples); 

    //printf("Client header: data size %ld\n", header->data_size); 

    return 0; 
} 
