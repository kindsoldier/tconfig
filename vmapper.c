/*
 * Copyright 2023 Oleg Borodin  <borodin@unix7.org>
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <vmapper.h>

static char* copystr(char* src) {
    size_t srcsize = strlen(src) + 1;
    char* dest = malloc(srcsize);
    memset(dest, '\0', srcsize);
    strcpy(dest, src);
    return dest;
}



static mlink_t* new_mlink_string(char* name, char** val) {
    mlink_t* mlink = malloc(sizeof(mlink_t));
    mlink->name = name;
    mlink->vptr = (void*)val;
    mlink->type = MAPPER_STR;
    return mlink;
}

static mlink_t* new_mlink_integer(char* name, int* val) {
    mlink_t* mlink = malloc(sizeof(mlink_t));
    mlink->name = name;
    mlink->vptr = (void*)val;
    mlink->type = MAPPER_INT;
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


vmapper_t* new_vmapper(void) {
    vmapper_t* vmapper = malloc(sizeof(vmapper_t));
    if (vmapper == NULL) return NULL;
    vmapper->mlinks = malloc(sizeof(mlink_t) * MAPPER_INITCAPA);
    vmapper->capa = MAPPER_INITCAPA;
    vmapper->size = 0;
    vmapper->err = false;
    vmapper->errstr = NULL;
    return vmapper;
}


void vmapper_init(vmapper_t* vmapper) {
    vmapper->mlinks = malloc(sizeof(mlink_t) * MAPPER_INITCAPA);
    vmapper->capa = MAPPER_INITCAPA;
    vmapper->size = 0;
    vmapper->err = false;
    vmapper->errstr = NULL;
    return;
}

#define RES_BIND_OK   ((int)0)
#define RES_BIND_ERR  ((int)-1)

int vmapper_check_capa(vmapper_t* vmapper) {
    if (vmapper->size == vmapper->capa) {
        size_t newcapa = vmapper->capa * 2;
        mlink_t** newmlinks = realloc(vmapper->mlinks, newcapa);
        if (newmlinks == NULL) return RES_BIND_ERR;
        vmapper->mlinks = newmlinks;
        vmapper->capa = newcapa;
    }
    return RES_BIND_OK;
}

int vmapper_bind_string(vmapper_t* vmapper, char* name, char** val) {
    int res = 0;
    if ((res = vmapper_check_capa(vmapper)) != RES_BIND_OK) {
        return res;
    }
    mlink_t* mlink = new_mlink_string(name, val);
    vmapper->mlinks[vmapper->size] = mlink;
    vmapper->size++;
    return RES_BIND_OK;
}

int vmapper_bind_int(vmapper_t* vmapper, char* name, int* val) {
    int res = 0;
    if ((res = vmapper_check_capa(vmapper)) != RES_BIND_OK) {
        return res;
    }
    mlink_t* mlink = new_mlink_integer(name, val);
    vmapper->mlinks[vmapper->size] = mlink;
    vmapper->size++;
    return RES_BIND_OK;
}


int vmapper_bind_bool(vmapper_t* vmapper, char* name, bool* val) {
    int res = 0;
    if ((res = vmapper_check_capa(vmapper)) != RES_BIND_OK) {
        return res;
    }
    mlink_t* mlink = new_mlink_bool(name, val);
    vmapper->mlinks[vmapper->size] = mlink;
    vmapper->size++;
    return RES_BIND_OK;
}

int vmapper_set_int(vmapper_t* vmapper, char* key, char* val) {
    for (size_t i = 0; i < vmapper->size; i++) {
        mlink_t* mlink = vmapper->mlinks[i];
        if (mlink->type == MAPPER_INT && strcmp(mlink->name, key) == 0) {
            char* eptr = NULL;
            *(int*)(mlink->vptr) = (int)strtol(val, &eptr, 10);
        }
    }
    return 0;
}

int vmapper_set_string(vmapper_t* vmapper, char* key, char* val) {
    for (size_t i = 0; i < vmapper->size; i++) {
        mlink_t* mlink = vmapper->mlinks[i];
        if (mlink->type == MAPPER_STR && strcmp(mlink->name, key) == 0) {
            *(char**)(mlink->vptr) = copystr(val);
        }
    }
    return 0;
}

int vmapper_set_bool(vmapper_t* vmapper, char* key, char* val) {
    for (size_t i = 0; i < vmapper->size; i++) {
        mlink_t* mlink = vmapper->mlinks[i];
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

int vmapper_bind(vmapper_t* vmapper, int type, char* name, void* val) {
    switch (type) {
        case MAPPER_STR: {
            return vmapper_bind_string(vmapper, name, (char**)val);
        }
        case MAPPER_INT: {
            return vmapper_bind_int(vmapper, name, (int*)val);
        }
        case MAPPER_BOOL: {
            return vmapper_bind_bool(vmapper, name, (bool*)val);
        }
    }
    return 0;
}



int vmapper_set(vmapper_t* vmapper, char* key, char* val) {
    vmapper_set_int(vmapper, key, val);
    vmapper_set_string(vmapper, key, val);
    vmapper_set_bool(vmapper, key, val);
    return 0;
}

void vmapper_destroy(vmapper_t* vmapper) {
    if (vmapper != NULL) {
        for (size_t i = 0; i < vmapper->size; i++) {
            free(vmapper->mlinks[i]);
        }
        free(vmapper->mlinks);
    }
    return;
}

void vmapper_free(vmapper_t* vmapper) {
    if (vmapper != NULL) {
        for (size_t i = 0; i < vmapper->size; i++) {
            free(vmapper->mlinks[i]);
        }
        free(vmapper->mlinks);
    }
    free(vmapper);
}
