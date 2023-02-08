/*
 * Copyright 2023 Oleg Borodin  <borodin@unix7.org>
 */


#include <stdio.h>

#include <clexer.h>
#include <cyacc.h>
#include <bstream.h>
#include <mapper.h>
#include <tconfig.h>

void tconfig_init(tconfig_t* tconfig) {
    tconfig->stream = new_bstream();
    tconfig->mapper = new_mapper();
    tconfig->lexer = new_clexer(tconfig->stream);
    tconfig->yacc = new_cyacc(tconfig->lexer, tconfig->mapper);
}

int tconfig_bind(tconfig_t* tconfig, int type, char* name, void* ptr) {
    mapper_t* mapper = tconfig->mapper;
    return mapper_bind(mapper, type, name, ptr);
}

ssize_t tconfig_read(tconfig_t* tconfig, char* filename) {
    bstream_t* stream = tconfig->stream;
    return bstream_fread(stream, filename);
}

int tconfig_parse(tconfig_t* tconfig) {

    return cyacc_parse(tconfig->yacc);
}

void tconfig_destroy(tconfig_t* tconfig) {
    cyacc_free(tconfig->yacc);
    clexer_free(tconfig->lexer);
    bstream_free(tconfig->stream);
    mapper_free(tconfig->mapper);
}
