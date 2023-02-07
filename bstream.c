/*
 *
 * Copyright 2023 Oleg Borodin  <borodin@unix7.org>
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <bstream.h>

#define RES_OK   0
#define RES_ERR -1

#define STREAM_INITCAPA 64

int bstream_init(bstream_t * stream) {
    stream->data = malloc(STREAM_INITCAPA);
    stream->wsize = 0;
    stream->rsize = 0;
    stream->capa = STREAM_INITCAPA;
    return RES_OK;
}


ssize_t bstream_dump(bstream_t * stream) {
    for (size_t i = 0; i < stream->wsize; i++) {
        printf("%c", stream->data[i]);
    }
    return stream->wsize;
}


ssize_t bstream_write(bstream_t * stream, void* buf, ssize_t size) {
    if ((stream->wsize + size) > stream->capa) {
        size_t newcapa = stream->capa * 2;

        stream->data = realloc(stream->data, newcapa);
        stream->capa = newcapa;
    }
    if (buf != NULL) {
        memcpy(&(stream->data[stream->wsize]), buf, size);
    }
    stream->wsize += size;
    return size;
}

ssize_t bstream_read(bstream_t * stream, void* buf, ssize_t size) {
    size_t unread = stream->wsize - stream->rsize;

    if (size > unread) {
        size = unread;
    }
    if (buf != NULL) {
        memcpy(buf, &(stream->data[stream->rsize]), size);
    }
    stream->rsize += size;
    return size;
}

char bstream_getc(bstream_t * stream) {
    size_t unread = stream->wsize - stream->rsize;

    if (unread == 0)
        return EOF;
    return stream->data[stream->rsize++];
}


size_t bstream_rrewind(bstream_t * stream, ssize_t size) {
    return stream->rsize - size;
}

size_t bstream_wrewind(bstream_t * stream, ssize_t size) {
    return stream->wsize - size;
}

size_t bstream_wsize(bstream_t * stream) {
    return stream->wsize;
}

size_t bstream_rsize(bstream_t * stream) {
    return stream->rsize;
}


void bstream_destroy(bstream_t * stream) {
    if (stream != NULL)
        free(stream->data);
}

