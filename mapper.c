/*
 * Copyright 2023 Oleg Borodin  <borodin@unix7.org>
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <mapper.h>

static char* copystr(char* src) {
    size_t srcsize = strlen(src) + 1;
    char* dest = malloc(srcsize);
    memset(dest, '\0', srcsize);
    strcpy(dest, src);
    return dest;
}

#define MAPPER_STRING   1
#define MAPPER_INTEGER  2
#define MAPPER_BOOL     3

static mlink_t* new_mlink_string(char* name, char** val) {
    mlink_t* mlink = malloc(sizeof(mlink_t));
    mlink->name = name;
    mlink->vptr = (void*)val;
    mlink->type = MAPPER_STRING;
    return mlink;
}

static mlink_t* new_mlink_integer(char* name, int* val) {
    mlink_t* mlink = malloc(sizeof(mlink_t));
    mlink->name = name;
    mlink->vptr = (void*)val;
    mlink->type = MAPPER_INTEGER;
    return mlink;
}

static mlink_t* new_mlink_bool(char* name, bool* val) {
    mlink_t* mlink = malloc(sizeof(mlink_t));
    mlink->name = name;
    mlink->vptr = (void*)val;
    mlink->type = MAPPER_BOOL;
    return mlink;
}


#define MAPPER_INITCAPA 64


mapper_t* new_mapper() {
    mapper_t* mapper = malloc(sizeof(mapper_t));
    if (mapper == NULL) return NULL;
    mapper->mlinks = malloc(sizeof(mlink_t) * MAPPER_INITCAPA);
    mapper->capa = MAPPER_INITCAPA;
    mapper->size = 0;
    mapper->err = false;
    mapper->errstr = NULL;
    return mapper;
}


void mapper_init(mapper_t* mapper) {
    mapper->mlinks = malloc(sizeof(mlink_t) * MAPPER_INITCAPA);
    mapper->capa = MAPPER_INITCAPA;
    mapper->size = 0;
    mapper->err = false;
    mapper->errstr = NULL;
    return;
}

#define RES_BIND_OK   ((int)0)
#define RES_BIND_ERR  ((int)-1)

int mapper_check_capa(mapper_t* mapper) {
    if (mapper->size == mapper->capa) {
        size_t newcapa = mapper->capa * 2;
        mlink_t** newmlinks = realloc(mapper->mlinks, newcapa);
        if (newmlinks == NULL) return RES_BIND_ERR;
        mapper->mlinks = newmlinks;
        mapper->capa = newcapa;
    }
    return RES_BIND_OK;
}

int mapper_bind_string(mapper_t* mapper, char* name, char** val) {
    int res = 0;
    if ((res = mapper_check_capa(mapper)) != RES_BIND_OK) {
        return res;
    }
    mlink_t* mlink = new_mlink_string(name, val);
    mapper->mlinks[mapper->size] = mlink;
    mapper->size++;
    return RES_BIND_OK;
}

int mapper_bind_integer(mapper_t* mapper, char* name, int* val) {
    int res = 0;
    if ((res = mapper_check_capa(mapper)) != RES_BIND_OK) {
        return res;
    }
    mlink_t* mlink = new_mlink_integer(name, val);
    mapper->mlinks[mapper->size] = mlink;
    mapper->size++;
    return RES_BIND_OK;
}


int mapper_bind_bool(mapper_t* mapper, char* name, bool* val) {
    int res = 0;
    if ((res = mapper_check_capa(mapper)) != RES_BIND_OK) {
        return res;
    }
    mlink_t* mlink = new_mlink_bool(name, val);
    mapper->mlinks[mapper->size] = mlink;
    mapper->size++;
    return RES_BIND_OK;
}

int mapper_set_int(mapper_t* mapper, char* key, char* val) {
    for (size_t i = 0; i < mapper->size; i++) {
        mlink_t* mlink = mapper->mlinks[i];
        if (mlink->type == MAPPER_INTEGER && strcmp(mlink->name, key) == 0) {
            char* eptr = NULL;
            *(int*)(mlink->vptr) = (int)strtol(val, &eptr, 10);
        }
    }
    return 0;
}

int mapper_set_string(mapper_t* mapper, char* key, char* val) {
    for (size_t i = 0; i < mapper->size; i++) {
        mlink_t* mlink = mapper->mlinks[i];
        if (mlink->type == MAPPER_STRING && strcmp(mlink->name, key) == 0) {
            *(char**)(mlink->vptr) = copystr(val);
        }
    }
    return 0;
}

int mapper_set_bool(mapper_t* mapper, char* key, char* val) {
    for (size_t i = 0; i < mapper->size; i++) {
        mlink_t* mlink = mapper->mlinks[i];
        if (mlink->type == MAPPER_BOOL && strcmp(mlink->name, key) == 0) {
            if (strcmp(val, "true") == 0) {
                *(bool*)(mlink->vptr) = true;
            }
            if (strcmp(val, "false") == 0) {
                *(bool*)(mlink->vptr) = false;
            }
        }
    }
    return 0;
}

int mapper_set(mapper_t* mapper, char* key, char* val) {
    mapper_set_int(mapper, key, val);
    mapper_set_string(mapper, key, val);
    mapper_set_bool(mapper, key, val);
    return 0;
}

void mapper_destroy(mapper_t* mapper) {
    if (mapper != NULL) {
        for (size_t i = 0; i < mapper->size; i++) {
            free(mapper->mlinks[i]);
        }
        free(mapper->mlinks);
    }
    return;
}

void mapper_free(mapper_t* mapper) {
    if (mapper != NULL) {
        for (size_t i = 0; i < mapper->size; i++) {
            free(mapper->mlinks[i]);
        }
        free(mapper->mlinks);
    }
    free(mapper);
    return;
}
