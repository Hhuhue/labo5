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
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	if(inet_pton(AF_INET, argv[2], &servaddr.sin_addr)<=0){
		printf("Invalid address \n");
		return -1;
	}
	
	if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
		printf("Connection Failed \n");
		return -1;
	}

	int n, len;
    unsigned int a = 4;
    unsigned int b = 1537857;
    unsigned int params[4];
    params[0] = header->channels;
    params[1] = header->sample_rate;
    params[2] = header->size_of_each_sample;
    params[3] = header->num_samples;
	
	send(sockfd, params, sizeof(params), 0); 
	printf("Header infos sent.\n"); 

    fprintf(stderr, "Starting read/write loop\n");
    int total, read = 0;
    char data_buffer[header->size_of_each_sample];
	
    for (long i = 1; i <= header->num_samples; i++){
        read = fread(data_buffer, sizeof(data_buffer), 1, file);
		if(i<20){
			printf("read is %i, data : %x%x%x%x\n", read, data_buffer[0],data_buffer[1],data_buffer[2],data_buffer[3] );
		}
		send(sockfd, (char*)data_buffer, sizeof(data_buffer), 0); 
    }    
    fprintf(stderr, "End read/write loop\n");

	close(sockfd); 
    free(header);
    fclose(file);
	return 0; 
} 
