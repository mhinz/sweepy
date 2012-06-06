/*-----------------*/
/*---  queue.c  ---*/
/*-----------------*/

#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"


void q_add(const QUEUE **ref, const char *s)
{
    queue->host = strdup(s);
    if (!queue->host)
        die('f', "strdup failed");

    queue->next = malloc(sizeof (QUEUE));
    if (!queue->next)
        die('f', "malloc failed");

    *ref = queue->next;
}

void q_destroy(QUEUE *q)
{
    void *tmp;

    while (q) {
        tmp = q->next;
        free(q->host);
        free(q);
        q = tmp;
    }
}

void q_loop(const QUEUE *q)
{
    if (!q)
        return;

    for (; q->next; q = q->next)
        printf("%s\n", q->host);
}
