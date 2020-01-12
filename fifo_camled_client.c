#include "fifo_camled.h"
#include <stdio.h>
#include <ctype.h>
#include <limits.h>

static char client_fifo[CLIENT_FIFO_NAME_LEN];

static void remove_fifo() {
    unlink(client_fifo);
}

int main(int argc, char* argv[]) {
    int server_fd, client_fd;
    struct request req;
    struct response resp;

    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        usageErr("%s [seq-len...]\n", argv[0]);
    }

    umask(0);
    snprintf(client_fifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE, (long) getpid());
    if (mkfifo(client_fifo, S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST) {
        errExit("mkfifo %s", client_fifo);
    }
    if (atexit(remove_fifo) != 0) {
        errExit("atexit");
    }

    server_fd = open(SERVER_FIFO, O_WRONLY | O_NONBLOCK);
    if (server_fd == -1) {
        errExit("open %s", SERVER_FIFO);
    }

    int len, index;
    Boolean bad_pattern;
    for (;;) {
        printf("R: ");
        fflush(stdout);
        if (fgets(req.data, MAX_CMD_SIZE, stdin) == NULL) {
            break;
        }
        len = strlen(req.data);
        if (len <= 1) {
            break;
        }
        if (req.data[len - 1] == '\n') {
            req.data[len - 1] = '\0';
        }

        for (index = 0, bad_pattern = FALSE; index < len && !bad_pattern; index++)
            if (!isalnum((unsigned char) req.data[index]) &&
                strchr("_*?[^-]. ", req.data[index]) == NULL)
                bad_pattern = TRUE;
        if (bad_pattern) {
            errExit("Bad pattern character: %c\n", req.data[index - 1]);
        }

        req.pid = getpid();
        req.dataLen = len;
        if (write(server_fd, &req, sizeof(struct request)) != sizeof(struct request)) {
            fatal("Can't write to server");
        }

        client_fd = open(client_fifo, O_RDONLY);
        if (client_fd == -1) {
            errExit("open %s", client_fifo);
        }

        if (read(client_fd, &resp, sizeof(struct response)) != sizeof(struct response)) {
            fatal("Can't read response from server");
        }
//        free(req.data);
        printf("A: %d\n", resp.seqNum);
    }

    exit(EXIT_SUCCESS);
}
