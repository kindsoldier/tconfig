/*
 * Copyright 2023 Oleg Borodin  <borodin@unix7.org>
 */


#include <stdio.h>

#include <tclexer.h>
#include <tccomp.h>
#include <bstream.h>

typedef struct {
    bstream_t   stream;
    tclexer_t   lexer;
    tccomp_t    comp;
    vmapper_t   mapper;
} tconfig_t;

#define TCONF_STR    MAPPER_STR
#define TCONF_INT    MAPPER_INT
#define TCONF_BOOL   MAPPER_BOOL

void tconfig_init(tconfig_t* tconfig);
int tconfig_bind(tconfig_t* tconfig, int type, char* name, void* ptr);
ssize_t tconfig_read(tconfig_t* tconfig, char* filename);
int tconfig_parse(tconfig_t* tconfig);
void tconfig_destroy(tconfig_t* tconfig);
