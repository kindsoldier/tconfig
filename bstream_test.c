/*
 * Copyright 2022 Oleg Borodin  <borodin@unix7.org>
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <bstream.h>
#include <massert.h>



void test_rwrite(void) {
    bstream_t stream;
    bstream_init(&stream);
    char* data = "_123456789";
    size_t dsize = strlen(data);
    size_t wsize = 0;

    int count = 3;
    for (int i = 0; i < count; i++) {
        wsize += bstream_write(&stream, data, dsize);
    }
    printf("wsize = %ld, data = [%s]\n", wsize, stream.data);

    MASSERT(stream.wpos == dsize * count);
    MASSERT(wsize == dsize * count);

    size_t bsize = bstream_wpos(&stream);
    char* buf = malloc(bsize + 1);
    memset(buf, 0, bsize + 1);

    size_t rsize = bstream_read(&stream, buf, bsize);
    printf("rsize = %lu, buf = [%s]\n", rsize, buf);

    MASSERT(wsize == dsize * count);
    MASSERT(rsize == wsize);

    bstream_destroy(&stream);
}


int main(int argc, char **argv) {
    test_rwrite();
    return 0;
}
