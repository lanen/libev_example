/* Copyright 2017 evan */
#include <stdcfg.hpp>
#include <stdio.h>
#include <fcntl.h>

int
main(int argc, char** argv) {
    // open file
    int fd = open("./tf.txt", O_WRONLY|O_CREAT);
    if (fd > 0) {
        // write(fd,"S",1);

        printf("%d", fd);
    }
    // write hello
    return 0;
}
