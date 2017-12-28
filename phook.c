/* phook -- runs commands around a parent process
 *
 * Copyright (C) 2017 Ivan Drinchev
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/event.h>
#include <errno.h>
#include <limits.h>

#define PROGRAM_NAME "phook"
#define PROGRAM_VERSION "0.07"

const char *after = NULL;

static struct option const longopts[] = {{"help",    no_argument,       NULL, 'h'},
                                         {"version", no_argument,       NULL, 1},
                                         {"after",   required_argument, NULL, 'a'},
                                         {"execute", required_argument, NULL, 'e'},
                                         {"process", required_argument, NULL, 'p'},
                                         {NULL, 0,                      NULL, 0}};

void copyright() {
    printf(
            "Copyright (C) 2017 Ivan Drinchev\n\
This software may be modified and distributed\n\
under the terms of the MIT license.\n\
");
}

void usage(int status) {
    if (status != EXIT_SUCCESS)
        fprintf(stderr, "Try '%s --help' for more information.\n", PROGRAM_NAME);
    else {
        printf("Usage: %s [OPTION]...\n", PROGRAM_NAME);
        printf("Runs a command after a parent process has finished.");
        printf("\n\nWith no OPTION provided, will do nothing and exit\n\n");
        printf("\
Mandatory arguments to long options are mandatory for short options too\n\
  -a, --after=COMMAND        executes command after the parent process has ended\n\
  -e, --execute=COMMAND      executes command on start\n\
  -p, --process=PID          waits for PID to exit instead of parent process\n\
  -h, --help                 display this help and exit\n\
      --version              output version information and exit\n\n\
");
        copyright();
    }
    exit(status);
}

void version() {
    printf("%s %s\n", PROGRAM_NAME, PROGRAM_VERSION);
    copyright();
    exit(EXIT_SUCCESS);
}

void sigint_handler() {
    system(after);
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {

    const char *execute = NULL;
    int acode = 0;
    int ecode = 0;
    int optc;
    pid_t ppid, fpid;
    struct kevent kev;
    int kq;
    int kret;
    struct timespec timeout;

    signal(SIGINT, sigint_handler);
    signal(SIGPIPE, sigint_handler);

    while ((optc = getopt_long(argc, argv, "ha:e:p:", longopts, NULL)) != -1) {
        switch (optc) {
            case 'a':
                after = optarg;
                break;
            case 'e':
                execute = optarg;
                break;
            case 'p': {
                char *end;
                long p = strtol(optarg, &end, 10);
                if (errno != 0) {
                    perror("process");
                } else if (end == optarg || *end != '\0') {
                    fprintf(stderr, "process: Invalid argument\n");
                } else if (p < 0) {
                    fprintf(stderr, "process: Cannot be negative\n");
                } else if (p > INT_MAX) {
                    fprintf(stderr, "process: Result too large\n");
                } else if (p == 0) {
                    fprintf(stderr, "process: Cannot watch pid 0\n");
                } else {
                    ppid = (pid_t) p;
                    break;
                }
                usage(EXIT_FAILURE);
                break;
            }
            case 'h':
                usage(EXIT_SUCCESS);
                break;
            case 1:
                version();
                break;
            default:
                usage(EXIT_FAILURE);
        }
    }

    if (execute)
        ecode = system(execute);

    if (ecode > 0)
        exit(EXIT_FAILURE);

    if (!after)
        exit(EXIT_SUCCESS);

    if (ppid == 0)
        ppid = getppid();

    fpid = fork();

    if (fpid != 0)
        exit(EXIT_SUCCESS);

    EV_SET(&kev, ppid, EVFILT_PROC, EV_ADD, NOTE_EXIT, 0, 0);

    kq = kqueue();

    if (kq == -1) {
        perror("kqueue");
        exit(EXIT_FAILURE);
    }

    kret = kevent(kq, &kev, 1, NULL, 0, NULL);

    if (kret == -1) {
        perror("kevent");
        exit(EXIT_FAILURE);
    }

    timeout.tv_sec = (8 * 60 * 60);
    timeout.tv_nsec = 0;

    kret = kevent(kq, NULL, 0, &kev, 1, &timeout);

    if (kret == -1) {
        perror("kevent");
        exit(EXIT_FAILURE);
    }

    if (kret > 0)
        acode = system(after);

    if (acode > 0)
        exit(EXIT_FAILURE);

    exit(EXIT_SUCCESS);

}
