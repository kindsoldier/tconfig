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
    size_t rpos;
    size_t wpos;
    size_t capa;
    uint8_t* data;
} bstream_t;


bstream_t* new_bstream();
bstream_t* bstream_init(bstream_t * stream);
size_t bstream_dump(bstream_t * stream);
ssize_t bstream_write(bstream_t * stream, void* buf, size_t size);
ssize_t bstream_read(bstream_t * stream, void* buf, size_t size);

ssize_t bstream_fread(bstream_t * stream, char* filename);
char bstream_getc(bstream_t * stream);

size_t bstream_wpos(bstream_t * stream);
size_t bstream_rpos(bstream_t * stream);

void bstream_destroy(bstream_t * stream);
void bstream_free(bstream_t * stream);

#endif
