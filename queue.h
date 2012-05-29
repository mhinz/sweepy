/*-----------------*/
/*---  queue.h  ---*/
/*-----------------*/

typedef struct Q {
    char     *host;
    struct Q *next;
} QUEUE;

// helpers.c
extern void die(const char, const char *, ...);

// core.c
extern QUEUE *queue;
