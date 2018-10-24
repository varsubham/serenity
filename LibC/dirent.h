#pragma once

#include "types.h"

extern "C" {

struct dirent {
    ino_t d_ino;
    off_t d_off;
    unsigned short d_reclen;
    unsigned char d_type;
    char d_name[256];
};

struct DIR {
    int fd;
    dirent cur_ent;
    char* buffer;
    size_t buffer_size;
    char* nextptr;
};

DIR* opendir(const char* name);
dirent* readdir(DIR* dirp);

}

