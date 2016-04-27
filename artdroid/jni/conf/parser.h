//
// Created by vaioco on 08/03/16.
//
#ifndef ARTDROID_C_PARSER_H
#define ARTDROID_C_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/errno.h>
#include "json.h"

#define VERSION "0.1.2"

#define MYOPTDIR  "/data/local/tmp/dex/opt"

typedef struct config_t {
    int version;
    int debug;
    char *fname;
    char *optdir;
    char *dexfile;
    int osversion;
    int zygote;

    int (*parser)(struct config_t *);
} config_t;


typedef config_t *configT_ptr;

int config_init_working_dir(struct config_t *c);

void printJson(json_value *node, int indent, char *name);

static void process_value(json_value *value, int depth);

void *config_init(char *);


#ifdef __cplusplus
}
#endif

#endif //ARTDROID_C_PARSER_H