/*
 * Copyright 2023 Oleg Borodin  <borodin@unix7.org>
 */


#include <stdio.h>

#include <tclexer.h>
#include <tccomp.h>
#include <bstream.h>
#include <vmapper.h>
#include <tconfig.h>

void tconfig_init(tconfig_t* tconfig) {
    bstream_init(&(tconfig->stream));
    vmapper_init(&(tconfig->mapper));
    tclexer_init(&(tconfig->lexer), &(tconfig->stream));
    tccomp_init(&(tconfig->comp), &(tconfig->lexer), &(tconfig->mapper));
}

int tconfig_bind(tconfig_t* tconfig, int type, char* name, void* ptr) {
    vmapper_t* vmapper = &(tconfig->mapper);
    return vmapper_bind(vmapper, type, name, ptr);
}

ssize_t tconfig_read(tconfig_t* tconfig, char* filename) {
    bstream_t* stream = &(tconfig->stream);
    return bstream_fread(stream, filename);
}

int tconfig_parse(tconfig_t* tconfig) {

    return tccomp_parse(&(tconfig->comp));
}

void tconfig_destroy(tconfig_t* tconfig) {
    tccomp_destroy(&(tconfig->comp));
    tclexer_destroy(&(tconfig->lexer));
    bstream_destroy(&(tconfig->stream));
    vmapper_destroy(&(tconfig->mapper));
}
