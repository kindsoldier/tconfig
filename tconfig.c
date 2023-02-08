/*
 * Copyright 2023 Oleg Borodin  <borodin@unix7.org>
 */


#include <stdio.h>

#include <clexer.h>
#include <cyacc.h>
#include <bstream.h>
#include <vmapper.h>
#include <tconfig.h>

void tconfig_init(tconfig_t* tconfig) {
    tconfig->stream = new_bstream();
    tconfig->vmapper = new_vmapper();
    tconfig->lexer = new_clexer(tconfig->stream);
    tconfig->yacc = new_cyacc(tconfig->lexer, tconfig->vmapper);
}

int tconfig_bind(tconfig_t* tconfig, int type, char* name, void* ptr) {
    vmapper_t* vmapper = tconfig->vmapper;
    return vmapper_bind(vmapper, type, name, ptr);
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
    vmapper_free(tconfig->vmapper);
}
