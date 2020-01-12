#ifndef FIFO_CAMLED_H
#define FIFO_CAMLED_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#define SERVER_FIFO "/tmp/camled_sv"
#define CLIENT_FIFO_TEMPLATE "/tmp/camled_%ld"
#define CLIENT_FIFO_NAME_LEN (sizeof(CLIENT_FIFO_TEMPLATE) + 20)
#define MAX_CMD_SIZE 64

struct request {
    pid_t pid;
    int32_t dataLen;
    char data[MAX_CMD_SIZE];
};

struct response {
    int seqNum;
    int dataLen;
};

#endif // FIFO_CAMLED_H
