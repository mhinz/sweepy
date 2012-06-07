/*----------------*/
/*---  core.c  ---*/
/*----------------*/

#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#include "queue.h"
#include "core.h"

pid_t pid;
QUEUE *queue;

static QUEUE *foo;
static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;


static void get_hostlist()
{
    FILE *hostlist;
    char host[50];
    int  ret;

    hostlist = fopen("list", "r");
    if (!hostlist)
        die('p', "fopen");

    while (fgets(host, sizeof host, hostlist)) {
        host[strlen(host)-1] = '\0';
        q_add(&queue, host);
    }

    ret = fclose(hostlist);
    if (ret != 0)
        die('p', "fclose");
}

__attribute__((hot))
static void *worker()
{
    QUEUE *tmp;

    while (1) {
        pthread_mutex_lock(&m);
        if (!foo->next) {
            pthread_mutex_unlock(&m);
            break;
        }
        tmp = foo;
        foo = foo->next;
        pthread_mutex_unlock(&m);

        sleep(1);
        do_ping(tmp->host);
    }

    pthread_exit(NULL);
}

static void start_manager(void)
{
    pthread_t t[NUM_THREADS];
    int       ret;

    for (int i = 0; i < NUM_THREADS; ++i) {
        ret = pthread_create(&t[i], NULL, worker, NULL);
        if (ret != 0)
            die('f', "pthread_create failed");
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        ret = pthread_join(t[i], NULL);
        if (ret != 0)
            die('f', "pthread_join failed");
    }
}

int main(void)
{
    pid = getpid();
    queue = malloc(sizeof (QUEUE));
    if (!queue)
        die('f', "malloc failed");
    QUEUE *qptr = queue;
    foo = queue;

    get_hostlist();
    start_manager();
    q_destroy(qptr);

    pthread_exit(NULL);
}
