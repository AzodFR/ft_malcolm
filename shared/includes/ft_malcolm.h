#ifndef FT_MALCOLM
#define FT_MALCOLM

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <ctype.h>
#include <ifaddrs.h>
#include <netinet/if_ether.h>
#include <netpacket/packet.h>
#include <sys/time.h>

typedef struct s_lustructur
{
    int socket;
    int verbose;
    int unique;
    char ip_host[20];
    char ip_target[20];
    int received;
    int sended;
}               t_lustructur;

int      getHost(char *hostname, char *ip);
int      isIPv4(char *ip);
int      isMac(char *mac);
int      check_error(int ac, char **av, t_lustructur *param);
void    *ft_memcpy(void *dest, const void *src, size_t n);


#endif
