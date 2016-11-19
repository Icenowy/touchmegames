
/*	Copyright (C) 2005, 2006 by Andy Grebe a.k.a. tecknophreak(tecknophreak@gmail.com)
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



#include <tmgwave.h>
#include <glib.h>
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <iostream>

#ifndef WIN32


#ifndef LLONG_MAX
#define LLONG_MAX    9223372036854775807LL
#endif

#define DEFAULT_FORMAT		SND_PCM_FORMAT_U8
#define DEFAULT_SPEED 		8000

#define FORMAT_DEFAULT		-1
#define FORMAT_RAW		0
#define FORMAT_VOC		1
#define FORMAT_WAVE		2
#define FORMAT_AU		3

/* Definitions for Microsoft WAVE format */

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define COMPOSE_ID(a,b,c,d)	((a) | ((b)<<8) | ((c)<<16) | ((d)<<24))
#define LE_SHORT(v)		(v)
#define LE_INT(v)		(v)
#define BE_SHORT(v)		bswap_16(v)
#define BE_INT(v)		bswap_32(v)
#elif __BYTE_ORDER == __BIG_ENDIAN
#define COMPOSE_ID(a,b,c,d)	((d) | ((c)<<8) | ((b)<<16) | ((a)<<24))
#define LE_SHORT(v)		bswap_16(v)
#define LE_INT(v)		bswap_32(v)
#define BE_SHORT(v)		(v)
#define BE_INT(v)		(v)
#else
#error "Wrong endian"
#endif

#define WAV_RIFF		COMPOSE_ID('R','I','F','F')
#define WAV_WAVE		COMPOSE_ID('W','A','V','E')
#define WAV_FMT			COMPOSE_ID('f','m','t',' ')
#define WAV_DATA		COMPOSE_ID('d','a','t','a')
#define WAV_PCM_CODE		1


/* Definitions for Sparc .au header */

#define AU_MAGIC		COMPOSE_ID('.','s','n','d')

#define AU_FMT_ULAW		1
#define AU_FMT_LIN8		2
#define AU_FMT_LIN16		3





#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 95)
#define error(...) do {\
	fprintf(stderr, "%s: %s:%d: ", command, __FUNCTION__, __LINE__); \
	fprintf(stderr, __VA_ARGS__); \
	putc('\n', stderr); \
} while (0)
#else
#define error(args...) do {\
	fprintf(stderr, "%s: %s:%d: ", command, __FUNCTION__, __LINE__); \
	fprintf(stderr, ##args); \
	putc('\n', stderr); \
} while (0)
#endif	
#endif // WIN32


TmgWave::TmgWave() {
#ifndef WIN32
    timelimit = 0;
    quiet_mode = 0;
    file_type = FORMAT_DEFAULT;
    sleep_min = 0;
    open_mode = 0;
    stream = SND_PCM_STREAM_PLAYBACK;
    mmap_flag = 0;
    interleaved = 1;
    nonblock = 0;
    audiobuf = NULL;
    chunk_size = 0;
    period_time = 0;
    buffer_time = 0;
    period_frames = 0;
    buffer_frames = 0;
    avail_min = -1;
    start_delay = 0;
    stop_delay = 0;
    verbose = 0;
    buffer_pos = 0;
    pbrec_count = LLONG_MAX;
    
    initialized = false;
    fd = -1;
#endif
}

void TmgWave::init() {
#ifndef WIN32
    g_thread_init(NULL);
    
    if (g_thread_supported() == false) {
        initialized = false;
        std::cerr << "gthread is not supported, no sound will be played" << std::endl;
        return;
    }
    
    player = g_thread_create(wavePlayer, this, TRUE, NULL);
    

    if (player == NULL) {
        std::cout << "Thread creation error" << std::endl;
        exit(-1);
    }
    
    msid = msgget((key_t)TMG_WAVE_KEY_T, 0666 | IPC_CREAT);
    if (msid < 0) {
        std::cerr << "Message creation error: " << strerror(errno) << std::endl;
        exit(-1);
    }
    
    initialized = true;
#endif
}

void TmgWave::play(char *name) {
#ifndef WIN32
    int res;

    if (!initialized) {
        init();
    }
    message.msgtype = 1;
    strcpy(message.command, name);
    
    // send it to the thread
    res = msgsnd(msid, &message, 1000, 0);
    if (res < 0) {
        std::cout << "play Res: " << res << ' ' << strerror(errno) << std::endl;
    }
#endif    
}

void TmgWave::cleanUp() {
#ifndef WIN32
    void *thread_result;
    int res;
    if (!initialized) {
        return;
    }

    strcpy(message.command, "exit");

    // send it to the thread
    res = msgsnd(msid, &message, 1000, 0);
    if (res < 0) {
        std::cout << "Res: " << res << ' ' << strerror(errno) << std::endl;
    }
    thread_result = g_thread_join(player);
    if (msgctl(msid, IPC_RMID, 0) == -1) {
        std::cerr << "Error closing tmgwave message queue." << std::endl;
    }
#endif
}
#ifndef WIN32
void TmgWave::signal_handler(int sig) {
    
    fprintf(stderr, "Aborted by signal %s...\n", strsignal(sig));
    
    exit(EXIT_FAILURE);
}

int TmgWave::playFile(char *name) {
    char *pcm_name = "default";
    int err;
    snd_pcm_info_t *info;

    snd_pcm_info_alloca(&info);

    err = snd_output_stdio_attach(&log, stderr, 0);
    assert(err >= 0);

    
    file_type = FORMAT_DEFAULT;

    stream = SND_PCM_STREAM_PLAYBACK;
    command = "aplay";

    chunk_size = (snd_pcm_uframes_t)-1;
    rhwparams.format = DEFAULT_FORMAT;
    rhwparams.rate = (snd_pcm_uframes_t)DEFAULT_SPEED;
    rhwparams.channels = 1;


    err = snd_pcm_open(&handle, pcm_name, stream, open_mode);
    if (err < 0) {
        error("audio open error: %s", snd_strerror(err));
        return 1;
    }

    if ((err = snd_pcm_info(handle, info)) < 0) {
        error("info error: %s", snd_strerror(err));
        return 1;
    }

    if (nonblock) {
        err = snd_pcm_nonblock(handle, 1);
        if (err < 0) {
            error("nonblock setting error: %s", snd_strerror(err));
            return 1;
        }
    }

    chunk_size = 1024;
    hwparams = rhwparams;

    audiobuf = (char *)malloc(1024);
    if (audiobuf == NULL) {
        error("not enough memory");
        return 1;
    }

     if (mmap_flag) {
         
        writei_func = snd_pcm_mmap_writei;
        
        
     }
     else {
         
         writei_func = snd_pcm_writei;
     }


    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGABRT, signal_handler);
    
    playback(name);
    
    snd_pcm_close(handle);
    free(audiobuf);
    snd_output_close(log);
    snd_config_update_free_global();
    return EXIT_SUCCESS;
}

/*
 * Safe read (for pipes)
 */
 
ssize_t TmgWave::safe_read(int fd, void *buf, size_t count) {
    ssize_t result = 0, res;

    while (count > 0) {
        if ((res = read(fd, buf, count)) == 0)
            break;
        if (res < 0)
            return result > 0 ? result : res;
        count -= res;
        result += res;
        buf = (char *)buf + res;
    }
    return result;
}


/*
 * helper for test_wavefile
 */

size_t TmgWave::test_wavefile_read(int fd, char *buffer, size_t *size, size_t reqsize, int line) {
    if (*size >= reqsize)
        return *size;
    if ((size_t)safe_read(fd, buffer + *size, reqsize - *size) != reqsize - *size) {
        error("read error (called from line %i)", line);
        exit(EXIT_FAILURE);
    }
    return *size = reqsize;
}

#define check_wavefile_space(buffer, len, blimit) \
	if (len > blimit) { \
		blimit = len; \
		if ((buffer = (char *)realloc(buffer, blimit)) == NULL) { \
			error("not enough memory"); \
			exit(EXIT_FAILURE); \
} \
}

/*
 * test, if it's a .WAV file, > 0 if ok (and set the speed, stereo etc.)
 *                            == 0 if not
 * Value returned is bytes to be discarded.
 */
ssize_t TmgWave::test_wavefile(int fd, char *_buffer, size_t size) {
    WaveHeader *h = (WaveHeader *)_buffer;
    char *buffer = NULL;
    size_t blimit = 0;
    WaveFmtBody *f;
    WaveChunkHeader *c;
    u_int type, len;

    if (size < sizeof(WaveHeader))
        return -1;
    if (h->magic != WAV_RIFF || h->type != WAV_WAVE)
        return -1;
    if (size > sizeof(WaveHeader)) {
        check_wavefile_space(buffer, size - sizeof(WaveHeader), blimit);
        memcpy(buffer, _buffer + sizeof(WaveHeader), size - sizeof(WaveHeader));
    }
    size -= sizeof(WaveHeader);
    while (1) {
        check_wavefile_space(buffer, sizeof(WaveChunkHeader), blimit);
        test_wavefile_read(fd, buffer, &size, sizeof(WaveChunkHeader), __LINE__);
        c = (WaveChunkHeader*)buffer;
        type = c->type;
        len = LE_INT(c->length);
        len += len % 2;
        if (size > sizeof(WaveChunkHeader))
            memmove(buffer, buffer + sizeof(WaveChunkHeader), size - sizeof(WaveChunkHeader));
        size -= sizeof(WaveChunkHeader);
        if (type == WAV_FMT)
            break;
        check_wavefile_space(buffer, len, blimit);
        test_wavefile_read(fd, buffer, &size, len, __LINE__);
        if (size > len)
            memmove(buffer, buffer + len, size - len);
        size -= len;
    }

    if (len < sizeof(WaveFmtBody)) {
        error("unknown length of 'fmt ' chunk (read %u, should be %u at least)", len, (u_int)sizeof(WaveFmtBody));
        exit(EXIT_FAILURE);
    }
    check_wavefile_space(buffer, len, blimit);
    test_wavefile_read(fd, buffer, &size, len, __LINE__);
    f = (WaveFmtBody*) buffer;
    if (LE_SHORT(f->format) != WAV_PCM_CODE) {
        error("can't play not PCM-coded WAVE-files");
        exit(EXIT_FAILURE);
    }
    if (LE_SHORT(f->modus) < 1) {
        error("can't play WAVE-files with %d tracks", LE_SHORT(f->modus));
        exit(EXIT_FAILURE);
    }
    hwparams.channels = LE_SHORT(f->modus);
    switch (LE_SHORT(f->bit_p_spl)) {
        case 8:
            if (hwparams.format != DEFAULT_FORMAT &&
                hwparams.format != SND_PCM_FORMAT_U8)
                
            hwparams.format = SND_PCM_FORMAT_U8;
            break;
        case 16:
            if (hwparams.format != DEFAULT_FORMAT &&
                hwparams.format != SND_PCM_FORMAT_S16_LE)
                
            hwparams.format = SND_PCM_FORMAT_S16_LE;
            break;
        case 24:
            switch (LE_SHORT(f->byte_p_spl) / hwparams.channels) {
                case 3:
                    if (hwparams.format != DEFAULT_FORMAT &&
                        hwparams.format != SND_PCM_FORMAT_S24_3LE)
                        
                    hwparams.format = SND_PCM_FORMAT_S24_3LE;
                    break;
                case 4:
                    if (hwparams.format != DEFAULT_FORMAT &&
                        hwparams.format != SND_PCM_FORMAT_S24_LE)
                        
                    hwparams.format = SND_PCM_FORMAT_S24_LE;
                    break;
                default:
                    error(" can't play WAVE-files with sample %d bits in %d bytes wide (%d channels)", LE_SHORT(f->bit_p_spl), LE_SHORT(f->byte_p_spl), hwparams.channels);
                    exit(EXIT_FAILURE);
            }
            break;
        case 32:
            hwparams.format = SND_PCM_FORMAT_S32_LE;
            break;
        default:
            error(" can't play WAVE-files with sample %d bits wide", LE_SHORT(f->bit_p_spl));
            exit(EXIT_FAILURE);
    }
    hwparams.rate = LE_INT(f->sample_fq);
	
    if (size > len)
        memmove(buffer, buffer + len, size - len);
    size -= len;
	
    while (1) {
        u_int type, len;

        check_wavefile_space(buffer, sizeof(WaveChunkHeader), blimit);
        test_wavefile_read(fd, buffer, &size, sizeof(WaveChunkHeader), __LINE__);
        c = (WaveChunkHeader*)buffer;
        type = c->type;
        len = LE_INT(c->length);
        if (size > sizeof(WaveChunkHeader))
            memmove(buffer, buffer + sizeof(WaveChunkHeader), size - sizeof(WaveChunkHeader));
        size -= sizeof(WaveChunkHeader);
        if (type == WAV_DATA) {
            if (len < pbrec_count && len < 0x7ffffffe)
                pbrec_count = len;
            if (size > 0)
                memcpy(_buffer, buffer, size);
            free(buffer);
            return size;
        }
        len += len % 2;
        check_wavefile_space(buffer, len, blimit);
        test_wavefile_read(fd, buffer, &size, len, __LINE__);
        if (size > len)
            memmove(buffer, buffer + len, size - len);
        size -= len;
    }

    /* shouldn't be reached */
    return -1;
}



void TmgWave::set_params(void) {
    snd_pcm_hw_params_t *params;
    snd_pcm_sw_params_t *swparams;
    snd_pcm_uframes_t buffer_size;
    int err;
    size_t n;
    snd_pcm_uframes_t xfer_align;
    unsigned int rate;
    snd_pcm_uframes_t start_threshold, stop_threshold;

    snd_pcm_hw_params_alloca(&params);
    snd_pcm_sw_params_alloca(&swparams);
    err = snd_pcm_hw_params_any(handle, params);
    if (err < 0) {
        error("Broken configuration for this PCM: no configurations available");
        exit(EXIT_FAILURE);
    }
    if (mmap_flag) {
        snd_pcm_access_mask_t *mask = (snd_pcm_access_mask_t *)alloca(snd_pcm_access_mask_sizeof());
        snd_pcm_access_mask_none(mask);
        snd_pcm_access_mask_set(mask, SND_PCM_ACCESS_MMAP_INTERLEAVED);
        snd_pcm_access_mask_set(mask, SND_PCM_ACCESS_MMAP_NONINTERLEAVED);
        snd_pcm_access_mask_set(mask, SND_PCM_ACCESS_MMAP_COMPLEX);
        err = snd_pcm_hw_params_set_access_mask(handle, params, mask);
    } else if (interleaved)
        err = snd_pcm_hw_params_set_access(handle, params,
                                           SND_PCM_ACCESS_RW_INTERLEAVED);
        else
            err = snd_pcm_hw_params_set_access(handle, params,
                                               SND_PCM_ACCESS_RW_NONINTERLEAVED);
        if (err < 0) {
            error("Access type not available");
            exit(EXIT_FAILURE);
        }
        err = snd_pcm_hw_params_set_format(handle, params, hwparams.format);
        if (err < 0) {
            error("Sample format non available");
            exit(EXIT_FAILURE);
        }
        err = snd_pcm_hw_params_set_channels(handle, params, hwparams.channels);
        if (err < 0) {
            error("Channels count non available");
            exit(EXIT_FAILURE);
        }

#if 0
	err = snd_pcm_hw_params_set_periods_min(handle, params, 2);
	assert(err >= 0);
#endif
	rate = hwparams.rate;
	err = snd_pcm_hw_params_set_rate_near(handle, params, &hwparams.rate, 0);
 assert(err >= 0);
 if ((float)rate * 1.05 < hwparams.rate || (float)rate * 0.95 > hwparams.rate) {
     if (!quiet_mode) {
         fprintf(stderr, "Warning: rate is not accurate (requested = %iHz, got = %iHz)\n", rate, hwparams.rate);
         fprintf(stderr, "         please, try the plug plugin (-Dplug:%s)\n", snd_pcm_name(handle));
     }
 }
 rate = hwparams.rate;
 if (buffer_time == 0 && buffer_frames == 0) {
     err = snd_pcm_hw_params_get_buffer_time_max(params,
             &buffer_time, 0);
     assert(err >= 0);
     if (buffer_time > 500000)
         buffer_time = 500000;
 }
 if (period_time == 0 && period_frames == 0) {
     if (buffer_time > 0)
         period_time = buffer_time / 4;
     else
         period_frames = buffer_frames / 4;
 }
 if (period_time > 0)
     err = snd_pcm_hw_params_set_period_time_near(handle, params,
         &period_time, 0);
 else
     err = snd_pcm_hw_params_set_period_size_near(handle, params,
         &period_frames, 0);
 assert(err >= 0);
 if (buffer_time > 0) {
     err = snd_pcm_hw_params_set_buffer_time_near(handle, params,
             &buffer_time, 0);
 } else {
     err = snd_pcm_hw_params_set_buffer_size_near(handle, params,
             &buffer_frames);
 }
 assert(err >= 0);
 err = snd_pcm_hw_params(handle, params);
 if (err < 0) {
     error("Unable to install hw params:");
     snd_pcm_hw_params_dump(params, log);
     exit(EXIT_FAILURE);
 }
 snd_pcm_hw_params_get_period_size(params, &chunk_size, 0);
 snd_pcm_hw_params_get_buffer_size(params, &buffer_size);
 if (chunk_size == buffer_size) {
     error("Can't use period equal to buffer size (%lu == %lu)", chunk_size, buffer_size);
     exit(EXIT_FAILURE);
 }
 snd_pcm_sw_params_current(handle, swparams);
 err = snd_pcm_sw_params_get_xfer_align(swparams, &xfer_align);
 if (err < 0) {
     error("Unable to obtain xfer align\n");
     exit(EXIT_FAILURE);
 }
 if (sleep_min)
     xfer_align = 1;
 err = snd_pcm_sw_params_set_sleep_min(handle, swparams,
                                       sleep_min);
 assert(err >= 0);
 if (avail_min < 0) {
     n = chunk_size;
 }
 else {
     n = (size_t)((double)rate * (double)avail_min / 1000000.0);
 }
 err = snd_pcm_sw_params_set_avail_min(handle, swparams, n);

 /* round up to closest transfer boundary */
 n = (size_t)((buffer_size / xfer_align) * xfer_align);
 if (start_delay <= 0) {
     start_threshold = (snd_pcm_uframes_t)(n + (double) rate * start_delay / 1000000);
 } else
     start_threshold = (snd_pcm_uframes_t)((double) rate * start_delay / 1000000);
     if (start_threshold < 1)
         start_threshold = 1;
     if (start_threshold > n)
         start_threshold = n;
     err = snd_pcm_sw_params_set_start_threshold(handle, swparams, start_threshold);
     assert(err >= 0);
     if (stop_delay <= 0) 
         stop_threshold = (snd_pcm_uframes_t)(buffer_size + (double) rate * stop_delay / 1000000);
     else
         stop_threshold = (snd_pcm_uframes_t)((double) rate * stop_delay / 1000000);
     err = snd_pcm_sw_params_set_stop_threshold(handle, swparams, stop_threshold);
     assert(err >= 0);

     err = snd_pcm_sw_params_set_xfer_align(handle, swparams, xfer_align);
     assert(err >= 0);

     if (snd_pcm_sw_params(handle, swparams) < 0) {
         error("unable to install sw params:");
         snd_pcm_sw_params_dump(swparams, log);
         exit(EXIT_FAILURE);
     }

     if (verbose)
         snd_pcm_dump(handle, log);

     bits_per_sample = snd_pcm_format_physical_width(hwparams.format);
     bits_per_frame = bits_per_sample * hwparams.channels;
     chunk_bytes = chunk_size * bits_per_frame / 8;
     audiobuf = (char *)realloc(audiobuf, chunk_bytes);
     if (audiobuf == NULL) {
         error("not enough memory");
         exit(EXIT_FAILURE);
     }
	
}

#ifndef timersub
#define	timersub(a, b, result) \
do { \
	(result)->tv_sec = (a)->tv_sec - (b)->tv_sec; \
	(result)->tv_usec = (a)->tv_usec - (b)->tv_usec; \
	if ((result)->tv_usec < 0) { \
		--(result)->tv_sec; \
		(result)->tv_usec += 1000000; \
} \
} while (0)
#endif

/* I/O error handler */
void TmgWave::xrun(void) {
    std::cerr << "Error: Xrun: I/O error" << std::endl;
    exit(EXIT_FAILURE);
}

/* I/O suspend handler */
void TmgWave::suspend(void) {
    printf("Suspend error\n");
    exit(EXIT_FAILURE);
    
}

/* peak handler */
void TmgWave::compute_max_peak(u_char *data, size_t count) {
    std::cerr << "Error: unhandled error: compute_max_peak" << std::endl;
    exit(-1);
}

/*
 *  write function
 */

ssize_t TmgWave::pcm_write(char *data, size_t count) {
    ssize_t r;
    ssize_t result = 0;

    if (sleep_min == 0 &&
        count < chunk_size) {
        
        snd_pcm_format_set_silence(hwparams.format, data + count * bits_per_frame / 8, (chunk_size - count) * hwparams.channels);
        count = chunk_size;
        }
        while (count > 0) {
            
            r = writei_func(handle, data, count);
            
            if (r == -EAGAIN || (r >= 0 && (size_t)r < count)) {
                snd_pcm_wait(handle, 1000);
            } else if (r == -EPIPE) {
                xrun();
            } else if (r == -ESTRPIPE) {
                suspend();
            } else if (r < 0) {
                error("write error: %s", snd_strerror(r));
                exit(EXIT_FAILURE);
            }
            if (r > 0) {
                if (verbose > 1)
                    compute_max_peak((u_char *)data, r * hwparams.channels);
                result += r;
                count -= r;
                data += r * bits_per_frame / 8;
            }
        }
        return result;
}





/* setting the globals for playing raw data */
void TmgWave::init_raw_data(void) {
    hwparams = rhwparams;
}

/* calculate the data count to read from/to dsp */
off64_t TmgWave::calc_count(void) {
    off64_t count;

    if (timelimit == 0) {
        count = pbrec_count;
    } else {
        count = snd_pcm_format_size(hwparams.format, hwparams.rate * hwparams.channels);
        count *= (off64_t)timelimit;
    }
    return count < pbrec_count ? count : pbrec_count;
}


void TmgWave::header(int rtype, char *name) {
    quiet_mode = 1;
    if (!quiet_mode) {
        fprintf(stderr, "%s, ", snd_pcm_format_description(hwparams.format));
        fprintf(stderr, "Rate %d Hz, ", hwparams.rate);
        if (hwparams.channels == 1)
            fprintf(stderr, "Mono");
        else if (hwparams.channels == 2)
            fprintf(stderr, "Stereo");
        else
            fprintf(stderr, "Channels %i", hwparams.channels);
        fprintf(stderr, "\n");
    }
}

/* playing raw data */

void TmgWave::playback_go(int fd, size_t loaded, off64_t count, int rtype, char *name) {
    int l, r;
    off64_t written = 0;
    off64_t c;

    header(rtype, name);
    set_params();
    

    while (loaded > chunk_bytes && written < count) {
        if (pcm_write(audiobuf + written, chunk_size) <= 0)
            return;
        written += chunk_bytes;
        loaded -= chunk_bytes;
    }
    if (written > 0 && loaded > 0)
        memmove(audiobuf, audiobuf + written, loaded);

    l = loaded;
    while (written < count) {
        do {
            c = count - written;
            if (c > chunk_bytes)
                c = chunk_bytes;
            c -= l;

            if (c == 0)
                break;
            r = safe_read(fd, audiobuf + l, c);
            if (r < 0) {
                perror(name);
                exit(EXIT_FAILURE);
            }
            fdcount += r;
            if (r == 0)
                break;
            l += r;
        } while (sleep_min == 0 && (size_t)l < chunk_bytes);
        l = l * 8 / bits_per_frame;
        r = pcm_write(audiobuf, l);
        if (r != l)
            break;
        r = r * bits_per_frame / 8;
        written += r;
        l = 0;
    }
    snd_pcm_drain(handle);
}




/*
 *  let's play or capture it (capture_type says VOC/WAVE/raw)
 */

void TmgWave::playback(char *name) {

    size_t dta;
    ssize_t dtawave;

	
    pbrec_count = LLONG_MAX;
    fdcount = 0;
    if (!name || !strcmp(name, "-")) {
        fd = fileno(stdin);
        name = "stdin";
    } else {
        if ((fd = open64(name, O_RDONLY, 0)) == -1) {
            perror(name);
            exit(EXIT_FAILURE);
        }
    }
    /* read the file header */
    dta = sizeof(AuHeader);
    if ((size_t)safe_read(fd, audiobuf, dta) != dta) {
        error("read error");
        exit(EXIT_FAILURE);
    }
    dta = sizeof(VocHeader);
    if ((size_t)safe_read(fd, audiobuf + sizeof(AuHeader),
         dta - sizeof(AuHeader)) != dta - sizeof(AuHeader)) {
             error("read error");
             exit(EXIT_FAILURE);
         }

         /* read bytes for WAVE-header */
         if ((dtawave = test_wavefile(fd, audiobuf, dta)) >= 0) {
             pbrec_count = calc_count();
             playback_go(fd, dtawave, pbrec_count, FORMAT_WAVE, name);
         } else {
             /* should be raw data */
             init_raw_data();
             pbrec_count = calc_count();
             playback_go(fd, dta, pbrec_count, FORMAT_RAW, name);
         }
      
    if (fd != 0) {
        close(fd);
    }
}


// here's where all the real action happens
void *TmgWave::wavePlayer(void *arg) {
    TmgWave *tm((TmgWave *)arg);
    int msid;
    bool done(false);
    Message msg;
    std::queue<std::string> playlist;
    std::string sysstr;
    int res;
    bool playing(false);
    msid = msgget((key_t)TMG_WAVE_KEY_T, 0666 | IPC_CREAT);
    if (msid < 0) {
        std::cerr << "Message creation error: " << strerror(errno) << std::endl;
        exit(-1);
    }

    while (!done) {
        res = msgrcv(msid, &msg, 1000, 0, 0);
        
        if (res >= 0) {
            if (!strcmp(msg.command, "exit")) {
                done = true;
            }
            else if (playing == false && tm->initialized) {
                tm->playFile(msg.command);
            }
        }
    }
    g_thread_exit(NULL);
    return NULL;
}
#endif // WIN32
