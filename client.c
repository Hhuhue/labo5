// Client side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include "wave.h"

#define PORT	 8080 
#define MAXLINE 1024 

// Driver code 
int main(int argc, char* argv[]) { 
	int sockfd; 
	char buffer[MAXLINE]; 
	char *hello = "Hello from client"; 
	struct sockaddr_in servaddr; 

    char *filename = argv[1];

    FILE* file;
    file = fopen(filename, "rb");
    struct HEADER* header = init_file(file); 

	// Creating socket file descriptor 
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = inet_addr(argv[2]); 
	
	int n, len; 
	
	sendto(sockfd, &header->channels, sizeof(unsigned int), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
	sendto(sockfd, &header->sample_rate, sizeof(unsigned int), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
	sendto(sockfd, &header->size_of_each_sample, sizeof(long), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
	sendto(sockfd, &header->num_samples, sizeof(long), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
	printf("Header infos sent.\n"); 

    fprintf(stderr, "Starting read/write loop\n");
    int total, read = 0;
    char data_buffer[header->size_of_each_sample];
    for (long i = 1; i <= header->num_samples; i++){
        read = fread(data_buffer, sizeof(data_buffer), 1, file);
	    sendto(sockfd, data_buffer, read, MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
    }    
    fprintf(stderr, "End read/write loop\n");

	close(sockfd); 
    free(header);
    fclose(file);
	return 0; 
} 
