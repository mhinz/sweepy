/*----------------------*/
/*---  networking.h  ---*/
/*----------------------*/

#define PACKETSIZE 64

// helpers.c
extern void die(const char, const char *, ...);

// core.c
extern pid_t           pid;
extern pthread_attr_t  attr;
extern pthread_mutex_t m;

