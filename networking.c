/*----------------------*/
/*---  networking.c  ---*/
/*----------------------*/

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <pthread.h>

#include "networking.h"


static inline unsigned short cksum(const unsigned short *addr, int len);


void do_ping(const char *target)
{
    unsigned char   *packet;
    unsigned char   *buf;
    struct icmphdr  *icmphdr;
    struct addrinfo *servinfo;
    struct addrinfo *p;
    struct addrinfo hints;
    struct icmp     icmp;
    struct timeval  tv;
    int             ret;
    int             sock;
    int             sent;
    int             count = 0;

    packet = calloc(sizeof (unsigned char), PACKETSIZE);
    if (!packet)
        die('f', "malloc failed");

    buf = calloc(sizeof (unsigned char), PACKETSIZE);
    if (!packet)
        die('f', "malloc failed");

    icmp.icmp_type  = ICMP_ECHO; // ICMP type: Echo Request
    icmp.icmp_code  = 0;         // Code 0: Echo Request
    icmp.icmp_id    = pid;       // PID of our process
    icmp.icmp_seq   = 666;       // Sequence number
    icmp.icmp_cksum = 0;
    icmp.icmp_cksum = cksum((unsigned short *)&icmp, sizeof icmp);

    memcpy(packet, &icmp, sizeof icmp);

    memset(&hints, 0, sizeof hints);
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;
    hints.ai_flags    = AI_PASSIVE;

    ret = getaddrinfo(target, NULL, &hints, &servinfo);
    if (ret != 0) {
        /*die('f', "getaddrinfo: %s", gai_strerror(ret));*/
        printf("%-14s unknown\n", target);
        free(packet);
        free(buf);
        return;
    }

    for (p = servinfo; p; p = p->ai_next) {
        sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sock == -1)
            continue;
        break;
    }

    if (!p)
        die('f', "failed to bind socket");

    tv.tv_sec  = 3;
    tv.tv_usec = 0;

    ret = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof tv);
    if (ret == -1)
        die('p', "setsockopt failed");

    for (int i = 0; i < 3; ++i) {
        sent = sendto(sock, packet, PACKETSIZE, 0, p->ai_addr, p->ai_addrlen);
        if (sent == -1)
            die('p', "sendto failed");
    }

    for (int i = 0; i < 3; ++i) {
        ret = recv(sock, packet, PACKETSIZE, 0);
        if (ret == -1)
            continue;

        icmphdr = (struct icmphdr *)(sizeof (struct iphdr) + packet);

        if (icmphdr->un.echo.id != pid) {
            ++i;
            continue;
        }

        switch (icmphdr->type) {
            case ICMP_ECHOREPLY:
                ++count;
                break;
            case ICMP_DEST_UNREACH:
                printf("%-14s unreachable\n", target);
                pthread_exit(NULL);
            default:
                printf("%-14s unknown\n", target);
                pthread_exit(NULL);
        }
    }
    printf("%-14s %d\n", target, count);
    close(sock);
    freeaddrinfo(servinfo);
    free(packet);
    free(buf);
}

static inline unsigned short cksum(const unsigned short *addr, int len)
{
    long sum = 0;

    while (len > 1) {
        sum += *(addr++);
        len -= 2;
    }
    if (len > 0)
        sum += *addr;
    while (sum >> 16)
        sum = ((sum & 0xFFFF) + (sum >> 16));
    sum = ~sum;

    return (unsigned short)sum;
}

