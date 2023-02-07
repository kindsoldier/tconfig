/*
 *
 * Copyright 2023 Oleg Borodin  <borodin@unix7.org>
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <bstream.h>


#define STREAM_INITCAPA 128

bstream_t* bstream_init(bstream_t* stream) {
    stream->data = malloc(STREAM_INITCAPA);
    stream->wpos = 0;
    stream->rpos = 0;
    stream->capa = STREAM_INITCAPA;
    return stream;
}

bstream_t* new_bstream() {
    bstream_t* stream = malloc(sizeof(bstream_t));
    if (stream == NULL) return NULL;
    stream->data = malloc(STREAM_INITCAPA);
    stream->wpos = 0;
    stream->rpos = 0;
    stream->capa = STREAM_INITCAPA;
    return stream;
}

ssize_t bstream_dump(bstream_t * stream) {
    for (size_t i = 0; i < stream->wpos; i++) {
        printf("%c", stream->data[i]);
    }
    return stream->wpos;
}


ssize_t bstream_write(bstream_t * stream, void* buf, ssize_t size) {
    if ((stream->wpos + size) > stream->capa) {
        size_t newcapa = stream->capa * 2;
        stream->data = realloc(stream->data, newcapa);
        stream->capa = newcapa;
    }
    if (buf != NULL) {
        memcpy(&(stream->data[stream->wpos]), buf, size);
    }
    stream->wpos += size;
    return size;
}

ssize_t bstream_read(bstream_t * stream, void* buf, ssize_t size) {
    size_t unread = stream->wpos - stream->rpos;

    if (size > unread) {
        size = unread;
    }
    if (buf != NULL) {
        memcpy(buf, &(stream->data[stream->rpos]), size);
    }
    stream->rpos += size;
    return size;
}

char bstream_getc(bstream_t * stream) {
    size_t unread = stream->wpos - stream->rpos;

    if (unread == 0)
        return EOF;
    return stream->data[stream->rpos++];
}


size_t bstream_wpos(bstream_t * stream) {
    return stream->wpos;
}

size_t bstream_rpos(bstream_t * stream) {
    return stream->rpos;
}


void bstream_destroy(bstream_t * stream) {
    if (stream != NULL)
        free(stream->data);
}

void bstream_free(bstream_t * stream) {
    if (stream != NULL)
        free(stream->data);
    free(stream);
}
