#include <alsa/asoundlib.h>
#include <stdio.h>
#include "wave.h"

#define PCM_DEVICE "default"

int main(int argc, char* argv[]){
    snd_pcm_t *pcm_handle;
    snd_pcm_hw_params_t *params;
    snd_pcm_uframes_t frames;
    int dir, pcmrc, err, read;

    char *filename = argv[1];
    unsigned char buffer4[4];
    unsigned char buffer2[2];

    struct HEADER header;
    FILE* file;
    file = fopen(filename, "rb");

    read = fread(header.riff, sizeof(header.riff), 1, file);
    printf("(1-4): %s \n", header.riff); 

    read = fread(buffer4, sizeof(buffer4), 1, file);
    printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
    header.overall_size = buffer4[0] | (buffer4[1]<<8) | (buffer4[2]<<16) | (buffer4[3]<<24);
    printf("(5-8) Overall size: bytes:%u, Kb:%u \n", header.overall_size, header.overall_size/1024);

    read = fread(header.wave, sizeof(header.wave), 1, file);
    printf("(9-12) Wave marker: %s\n", header.wave);

    read = fread(header.fmt_chunk_marker, sizeof(header.fmt_chunk_marker), 1, file);
    printf("(13-16) Fmt marker: %s\n", header.fmt_chunk_marker);

    read = fread(buffer4, sizeof(buffer4), 1, file);
    printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
    header.length_of_fmt = buffer4[0] |(buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);
    printf("(17-20) Length of Fmt header: %u \n", header.length_of_fmt);

    read = fread(buffer2, sizeof(buffer2), 1, file); printf("%u %u \n", buffer2[0], buffer2[1]);
    header.format_type = buffer2[0] | (buffer2[1] << 8);   
    char format_name[10] = "";   
    if (header.format_type == 1)   
        strcpy(format_name,"PCM");   
    else if (header.format_type == 6)   
        strcpy(format_name, "A-law");   
    else if (header.format_type == 7)   
        strcpy(format_name, "Mu-law");    
    printf("(21-22) Format type: %u %s \n", header.format_type, format_name);

    read = fread(buffer2, sizeof(buffer2), 1, file);   
    printf("%u %u \n", buffer2[0], buffer2[1]);
    header.channels = buffer2[0] | (buffer2[1] << 8);
    printf("(23-24) Channels: %u \n", header.channels);
   
    read = fread(buffer4, sizeof(buffer4), 1, file);
    printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
    header.sample_rate = buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);
    printf("(25-28) Sample rate: %u\n", header.sample_rate);
   
    read = fread(buffer4, sizeof(buffer4), 1, file);
    printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
    header.byterate  = buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);   
    printf("(29-32) Byte Rate: %u , Bit Rate:%u\n", header.byterate, header.byterate*8);
   
    read = fread(buffer2, sizeof(buffer2), 1, file);
    printf("%u %u \n", buffer2[0], buffer2[1]);
    header.block_align = buffer2[0] | (buffer2[1] << 8);
    printf("(33-34) Block Alignment: %u \n", header.block_align);

    read = fread(buffer2, sizeof(buffer2), 1, file);
    printf("%u %u \n", buffer2[0], buffer2[1]);
    header.bits_per_sample = buffer2[0] | (buffer2[1] << 8);   
    printf("(35-36) Bits per sample: %u \n", header.bits_per_sample);
   
    read = fread(header.data_chunk_header, sizeof(header.data_chunk_header), 1, file);   
    printf("(37-40) Data Marker: %s \n", header.data_chunk_header);
   
    read = fread(buffer4, sizeof(buffer4), 1, file);
    printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);   
    //header.data_size = buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);   
    header.data_size = header.overall_size - sizeof(struct HEADER);
    printf("(41-44) Size of data chunk: %u \n", header.data_size);
   
    // calculate no.of samples   
    long num_samples = (8 * header.data_size) / (header.channels * header.bits_per_sample);
    printf("Number of samples:%lu \n", num_samples);

    long size_of_each_sample = (header.channels * header.bits_per_sample) / 8;
    printf("Size of each sample:%ld bytes\n", size_of_each_sample);

    err = snd_pcm_open(&pcm_handle, PCM_DEVICE, SND_PCM_STREAM_PLAYBACK, 0);

    snd_pcm_hw_params_alloca(&params);
    err = snd_pcm_hw_params_any(pcm_handle, params);

    err = snd_pcm_hw_params_set_access(pcm_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    err = snd_pcm_hw_params_set_format(pcm_handle, params, SND_PCM_FORMAT_S16_LE);
    err = snd_pcm_hw_params_set_channels(pcm_handle, params, header.channels);
    err = snd_pcm_hw_params_set_rate(pcm_handle, params, header.sample_rate, 0);

    err = snd_pcm_hw_params(pcm_handle, params);

    err = snd_pcm_hw_params_get_period_size(params, &frames, &dir);
    fprintf(stderr, "# frames in a period: %d\n", frames);

    fprintf(stderr, "Starting read/write loop\n");
    int total = 0;
    char data_buffer[size_of_each_sample];
    for (long i = 1; i <= num_samples; i++){
        read = fread(data_buffer, sizeof(data_buffer), 1, file);
        total += read;
        pcmrc = snd_pcm_writei(pcm_handle, data_buffer, read);
        if (pcmrc == -EPIPE){
            fprintf(stderr, "Underrun!\n");
            snd_pcm_prepare(pcm_handle);
        } else if (pcmrc < 0) {
            fprintf(stderr, "Error writting to PCM device: %s\n", snd_strerror(pcmrc));
        } else if (pcmrc != read) {
            fprintf(stderr, "PCM write differs from PCM read.\n");
        }
    }

    fprintf(stderr, "End read/write loop\n");
    fprintf(stderr, "Total read: %d\n", total);

    err = snd_pcm_drain(pcm_handle);
    err = snd_pcm_close(pcm_handle);
    fclose(file);

    return 0;
}