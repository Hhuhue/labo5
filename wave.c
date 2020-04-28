#include "wave.h"

struct HEADER* init_file(FILE* file){
    unsigned char buffer4[4];
    unsigned char buffer2[2];
    int read;

    struct HEADER* header = (struct HEADER*)malloc(sizeof(struct HEADER));

    read = fread(header->riff, sizeof(header->riff), 1, file);
    printf("(1-4): %s \n", header->riff); 

    read = fread(buffer4, sizeof(buffer4), 1, file);
    printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
    header->overall_size = buffer4[0] | (buffer4[1]<<8) | (buffer4[2]<<16) | (buffer4[3]<<24);
    printf("(5-8) Overall size: bytes:%u, Kb:%u \n", header->overall_size, header->overall_size/1024);

    read = fread(header->wave, sizeof(header->wave), 1, file);
    printf("(9-12) Wave marker: %s\n", header->wave);

    read = fread(header->fmt_chunk_marker, sizeof(header->fmt_chunk_marker), 1, file);
    printf("(13-16) Fmt marker: %s\n", header->fmt_chunk_marker);

    read = fread(buffer4, sizeof(buffer4), 1, file);
    printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
    header->length_of_fmt = buffer4[0] |(buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);
    printf("(17-20) Length of Fmt header: %u \n", header->length_of_fmt);

    read = fread(buffer2, sizeof(buffer2), 1, file); printf("%u %u \n", buffer2[0], buffer2[1]);
    header->format_type = buffer2[0] | (buffer2[1] << 8);   
    char format_name[10] = "";   
    if (header->format_type == 1)   
        strcpy(format_name,"PCM");   
    else if (header->format_type == 6)   
        strcpy(format_name, "A-law");   
    else if (header->format_type == 7)   
        strcpy(format_name, "Mu-law");    
    printf("(21-22) Format type: %u %s \n", header->format_type, format_name);

    read = fread(buffer2, sizeof(buffer2), 1, file);   
    printf("%u %u \n", buffer2[0], buffer2[1]);
    header->channels = buffer2[0] | (buffer2[1] << 8);
    printf("(23-24) Channels: %u \n", header->channels);
   
    read = fread(buffer4, sizeof(buffer4), 1, file);
    printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
    header->sample_rate = buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);
    printf("(25-28) Sample rate: %u\n", header->sample_rate);
   
    read = fread(buffer4, sizeof(buffer4), 1, file);
    printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
    header->byterate  = buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);   
    printf("(29-32) Byte Rate: %u , Bit Rate:%u\n", header->byterate, header->byterate*8);
   
    read = fread(buffer2, sizeof(buffer2), 1, file);
    printf("%u %u \n", buffer2[0], buffer2[1]);
    header->block_align = buffer2[0] | (buffer2[1] << 8);
    printf("(33-34) Block Alignment: %u \n", header->block_align);

    read = fread(buffer2, sizeof(buffer2), 1, file);
    printf("%u %u \n", buffer2[0], buffer2[1]);
    header->bits_per_sample = buffer2[0] | (buffer2[1] << 8);   
    printf("(35-36) Bits per sample: %u \n", header->bits_per_sample);
   
    read = fread(header->data_chunk_header, sizeof(header->data_chunk_header), 1, file);   
    printf("(37-40) Data Marker: %s \n", header->data_chunk_header);
   
    read = fread(buffer4, sizeof(buffer4), 1, file);
    printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);   
    //header->data_size = buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);   
    header->data_size = header->overall_size - sizeof(struct HEADER);
    printf("(41-44) Size of data chunk: %u \n", header->data_size);
   
    // calculate no.of samples   
    header->num_samples = (8 * header->data_size) / (header->channels * header->bits_per_sample);
    printf("Number of samples:%lu \n", header->num_samples);

    header->size_of_each_sample = (header->channels * header->bits_per_sample) / 8;
    printf("Size of each sample:%ld bytes\n", header->size_of_each_sample);

    return header;
}