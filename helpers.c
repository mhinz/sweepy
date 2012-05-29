/*-------------------*/
/*---  helpers.c  ---*/
/*-------------------*/

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


void die(const char mode, const char *fmt, ...)
{
    va_list ap;
    char    *msg;

    va_start(ap, fmt);
    vasprintf(&msg, fmt, ap);

    if (mode == 'p')
        perror(msg);
    else
        fprintf(stderr, "%s\n", msg);

    va_end(ap);
    free(msg);

    exit(EXIT_FAILURE);
}
