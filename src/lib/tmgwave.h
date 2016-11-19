
/*	Copyright (C) 2005, 2006 by Andy Grebe a.k.a. tecknophreak(tecknophreak@gmail.com)
 *      Based on aplay by Jaroslav Kysela <perex@suse.cz>, which is based on
 *      Based on vplay program by Michael Beck
 *
 *	This file is part of TouchMe Games.
 *
 *	TouchMe Games is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	TouchMe Games is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with TouchMe Games; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifndef TMGWAVE_H
#define TMGWAVE_H


#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#ifndef WIN32
#include <alsa/asoundlib.h>
#include <assert.h>
#include <sys/poll.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/signal.h>
#include <endian.h>
#include <byteswap.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <queue>
#include <glib.h>
#endif
#define TMG_WAVE_KEY_T 2900

class TmgWave {
    private:
#ifndef WIN32
        struct VocHeader {
            u_char magic[20];	// must be MAGIC_STRING
            u_short headerlen;	// Headerlength, should be 0x1A
            u_short version;	// VOC-file version
            u_short coded_ver;	// 0x1233-version
            };
        
        struct WaveHeader {
            u_int magic;		// 'RIFF' 
            u_int length;		// filelen
            u_int type;		// 'WAVE' 
        };
    
        struct WaveFmtBody {
            u_short format;		// should be 1 for PCM-code
            u_short modus;		// 1 Mono, 2 Stereo
            u_int sample_fq;	// frequence of sample
            u_int byte_p_sec;
            u_short byte_p_spl;	// samplesize; 1 or 2 bytes
            u_short bit_p_spl;	// 8, 12 or 16 bit
        };
    
        struct WaveChunkHeader {
            u_int type;		// 'data'
            u_int length;		// samplecount
        };
        
        typedef struct au_header {
            u_int magic;		// '.snd'
            u_int hdr_size;		// size of header (min 24)
            u_int data_size;	// size of data
            u_int encoding;		// see to AU_FMT_XXXX
            u_int sample_rate;	// sample rate
            u_int channels;		// number of channels (voices)
        } AuHeader;
        
        
        // ipc vars
        struct Message {
            long int msgtype;
            char command[1000];
        };
        Message message;
        int msid;
        GThread *player;
        
        // how many of these do we really need?
        bool initialized;
        char *command;
        snd_pcm_t *handle;
        struct {
            snd_pcm_format_t format;
            unsigned int channels;
            unsigned int rate;
        } hwparams, rhwparams;
        int timelimit;
        int quiet_mode;
        int file_type;
        unsigned int sleep_min;
        int open_mode;
        snd_pcm_stream_t stream;
        int mmap_flag;
        int interleaved;
        int nonblock;
        char *audiobuf;
        snd_pcm_uframes_t chunk_size;
        unsigned period_time;
        unsigned buffer_time;
        snd_pcm_uframes_t period_frames;
        snd_pcm_uframes_t buffer_frames;
        int avail_min;
        int start_delay;
        int stop_delay;
        int verbose;
        int buffer_pos;
        size_t bits_per_sample, bits_per_frame;
        size_t chunk_bytes;
        snd_output_t *log;
    
        int fd;
        off64_t pbrec_count, fdcount;
        int vocmajor, vocminor;
    
        /* needed prototypes */
    
        void playback(char *filename);
        snd_pcm_sframes_t (*writei_func)(snd_pcm_t *handle, const void *buffer, snd_pcm_uframes_t size);
    
    
        void begin_wave(int fd, size_t count);
        void end_wave(int fd);
        void end_raw(int fd);
        
        
        static void signal_handler(int sig);
        ssize_t safe_read(int fd, void *buf, size_t count);
        size_t test_wavefile_read(int fd, char *buffer, size_t *size, size_t reqsize, int line);
        ssize_t test_wavefile(int fd, char *_buffer, size_t size);
        void set_params(void);
        static void xrun(void);
        static void suspend(void);
        static void compute_max_peak(u_char *data, size_t count);
        ssize_t pcm_write(char *data, size_t count);
        void init_raw_data(void);
        off64_t calc_count(void);
        void header(int rtype, char *name);
        void playback_go(int fd, size_t loaded, off64_t count, int rtype, char *name);
    
    
        static void *wavePlayer(void *arg);
        int playFile(char *name);
#endif
        
    public:
        TmgWave();
        void init();
        void play(char *name);
        void cleanUp();
};


#endif // TMGWAVE_H
