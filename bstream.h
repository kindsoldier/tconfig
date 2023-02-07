/*
 *
 * Copyright 2023 Oleg Borodin  <borodin@unix7.org>
 *
 */

#ifndef BSTREAM_H_QWERTY
#define BSTREAM_H_QWERTY

#include <stdint.h>
#include <unistd.h>

typedef struct {
    size_t rsize;
    size_t wsize;
    size_t capa;
    uint8_t* data;
} bstream_t;


int bstream_init(bstream_t * stream);
ssize_t bstream_dump(bstream_t * stream);
ssize_t bstream_write(bstream_t * stream, void* buf, ssize_t size);
ssize_t bstream_read(bstream_t * stream, void* buf, ssize_t size);
char bstream_getc(bstream_t * stream);
size_t bstream_rrewind(bstream_t * stream, ssize_t size);
size_t bstream_wrewind(bstream_t * stream, ssize_t size);
void bstream_destroy(bstream_t * stream);

size_t bstream_wsize(bstream_t * stream);
size_t bstream_rsize(bstream_t * stream);


#endif
