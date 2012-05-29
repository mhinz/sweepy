/*----------------*/
/*---  core.h  ---*/
/*----------------*/

#define NUM_THREADS 4

// queue.c
extern void q_add(QUEUE **, const char *);
extern void q_destroy(QUEUE *);
extern void q_loop(QUEUE *);

// networking.c
extern void do_ping(char *);

// helpers.c
extern void die(const char, const char *, ...);
