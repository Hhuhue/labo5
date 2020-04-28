#include <alsa/asoundlib.h>
#include <stdio.h>
#include <stdlib.h>
#include "wave.h"

#define PCM_DEVICE "default"

int main(int argc, char* argv[]){
    snd_pcm_t *pcm_handle;
    snd_pcm_hw_params_t *params;
    snd_pcm_uframes_t frames;
    int dir, pcmrc, err, read;

    char *filename = argv[1];

    FILE* file;
    file = fopen(filename, "rb");
    struct HEADER* header = init_file(file);    

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

    fprintf(stderr, "Starting read/write loop\n");
    int total = 0;
    char data_buffer[header->size_of_each_sample];
    for (long i = 1; i <= header->num_samples; i++){
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
    free(header);

    return 0;
}