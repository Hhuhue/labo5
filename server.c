
// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <alsa/asoundlib.h>
#include "wave.h"
  
#define PORT     8080 
#define MAXLINE 1024 
#define PCM_DEVICE "default"
  
// Driver code 
int main() { 
    int sockfd, new_socket; 
    char buffer[MAXLINE]; 
    char *hello = "Hello from server"; 
    struct sockaddr_in servaddr, cliaddr;
    int opt = 1; 
    int addrlen = sizeof(servaddr);

    FILE* file;
    file = fopen("Crab8.wav", "rb");
    struct HEADER* header = init_file(file);    
      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt");
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
      
    if (listen(sockfd, 3) < 0 ) 
    { 
        perror("listen failed"); 
        exit(EXIT_FAILURE); 
    } 

    if ((new_socket = accept(sockfd, (struct sockaddr*)&servaddr, (socklen_t*)&addrlen))<0) 
    { 
        perror("accept failed"); 
        exit(EXIT_FAILURE); 
    } 

    int len, n; 
  
    len = sizeof(cliaddr);  //len is value/resuslt 
  
    n = read(new_socket, (unsigned int*)buffer, MAXLINE); 
    unsigned int* channels = (unsigned int*) (buffer); 
    printf("Client header: chanels is %lu \n", *channels); 
    
    n = read(new_socket, (unsigned int*)buffer, MAXLINE); 
    unsigned int* sample_rate = (unsigned int*) (buffer); 
    printf("Client header: sample_rate is %ld \n", *sample_rate); 
    
    n = read(new_socket, (long*)buffer, MAXLINE); 
    long* sample_size = (long*) (buffer); 
    printf("Client header: size sample is %ld \n", *sample_size); 

    n = read(new_socket, (unsigned long*)buffer, MAXLINE); 
    long* num_samples = (long*) (buffer); 
    printf("Client header: num sample is %ld \n", *num_samples); 

    //Alsa Initialization
    snd_pcm_t *pcm_handle;
    snd_pcm_hw_params_t *params;
    snd_pcm_uframes_t frames;
    int dir, pcmrc, err;
    err = snd_pcm_open(&pcm_handle, PCM_DEVICE, SND_PCM_STREAM_PLAYBACK, 0);
    snd_pcm_hw_params_alloca(&params);
    err = snd_pcm_hw_params_any(pcm_handle, params);
    err = snd_pcm_hw_params_set_access(pcm_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    err = snd_pcm_hw_params_set_format(pcm_handle, params, SND_PCM_FORMAT_S16_LE);
    err = snd_pcm_hw_params_set_channels(pcm_handle, params, header->channels);
    err = snd_pcm_hw_params_set_rate(pcm_handle, params, header->sample_rate, 0);
    err = snd_pcm_hw_params(pcm_handle, params);
    err = snd_pcm_hw_params_get_period_size(params, &frames, &dir);
    fprintf(stderr, "# frames in a period: %d\n", frames);
    //End Initialization

    fprintf(stderr, "Starting read/write loop\n");
    int total = 0;
    char data_buffer[header->size_of_each_sample];
    for (long i = 1; i <= header->num_samples; i++){
        n = read(new_socket, (char*)data_buffer, sizeof(data_buffer)); 
        total += 1;
        pcmrc = snd_pcm_writei(pcm_handle, data_buffer, 1);
		if(i<20){
			printf("n is %i, data : %x%x%x%x\n", n, data_buffer[0],data_buffer[1],data_buffer[2],data_buffer[3] );
		}

        if (pcmrc == -EPIPE){
            fprintf(stderr, "Underrun!\n");
            snd_pcm_prepare(pcm_handle);
        } else if (pcmrc < 0) {
            fprintf(stderr, "Error writting to PCM device: %s\n", snd_strerror(pcmrc));
        } else if (pcmrc != 1) {
            fprintf(stderr, "PCM write differs from PCM read.\n");
        }
    }

    fprintf(stderr, "End read/write loop\n");
    fprintf(stderr, "Total read: %d\n", total);

    err = snd_pcm_drain(pcm_handle);
    err = snd_pcm_close(pcm_handle);

    return 0; 
} 
