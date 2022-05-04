#include "ft_malcolm.h"

int getHost(char *hostname, char *ip)
{
    struct hostent *H;
    struct in_addr **AddrList;

    if ((H = gethostbyname(hostname)) == NULL)
        return 1;
    AddrList = (struct in_addr **)H->h_addr_list;
    for (int i = 0; AddrList[i]; i++)
    {
        strcpy(ip, inet_ntoa(*AddrList[i]));
        return 0;
    }
    return 1;
}

int isIPv4(char *ip)
{
    int point;
    int i;

    point = 0;
    i = -1;
    while (ip[++i])
    {
        if (ip[i] >= '0' && ip[i] <= '9')
            continue;
        else if (ip[i] == '.' && (i > 0 && ip[i - 1] != '.'))
            point++;
        else
            return 0;
    }
    if (point != 3)
        return 0;
    return 1;
}

int		ft_tolower(int c)
{
	if ((c >= 'A' && c <= 'Z'))
		return (c += 32);
	return (c);
}
int ishex(int c)
{
    if ((c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') || (c >= '0' && c <= '9'))
        return 1;
    return 0;
}

int isMac(char *mac)
{
    int counter;
    int separator;
    int i;

    counter = 0;
    separator = 0;
    i = -1;
    if (strlen(mac) != 17)
        return 0;
    while (mac[++i])
    {
        if (i != 0 && (i + 1) % 3 == 0 && mac[i] == ':' && counter == 2)
        {
           separator++;
           counter = 0;
        }
        else if (ishex(mac[i]) && counter < 2)
        {
            mac[i] = ft_tolower(mac[i]);
            counter++;
        }
        else
            return 0;
    }
    if (counter != 2 || separator != 5)
        return 0;
    return 1;
}

int check_param(int ac, char **av, t_lustructur *param)
{
    if (ac < 5 && ac > 7)
        return 1;
    for(int i = 5; i < ac; i++)
    {
        for(int j = 1; av[i][j]; j++)
        {
            if (j == 0 && av[i][j] != '-')
                return 1;
            else if (av[i][j] == 'v')
                param->verbose = 1;
            else if (av[i][j] == 'u')
                param->unique = 1;
            else
                return 1;
        }
    }
    return 0;
}

int check_error(int ac, char **av, t_lustructur *param)
{
    if (getuid() || check_param(ac, av, param))
    {
        printf("Usage: sudo ./ft_malcolm <source_ip> <source_mac> <target_ip> <target_mac> [-v] [-u]\n");
        return 1;
    }
    else if (getHost(av[1], param->ip_host) && !isIPv4(av[1]))
    {
        printf("ft_malcolm: <source_ip> not valid (%s)\n", av[1]);
        return 1;
    }
    else if (!isMac(av[2]))
    {
        printf("ft_malcolm: <source_mac> not valid (%s)\n", av[2]);
        return 1;
    }
    else if (getHost(av[3], param->ip_target) && !isIPv4(av[3]))
    {
        printf("ft_malcolm: <target_ip> not valid (%s)\n", av[3]);
        return 1;
    }
    else if (!isMac(av[4]))
    {
        printf("ft_malcolm: <target_mac> not valid (%s)\n", av[3]);
        return 1;
    }
    return 0;
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t					i;
	unsigned char			*dst;
	unsigned const char		*source;

	if (!dest && !src && n)
		return (NULL);
	dst = (unsigned char *)dest;
	source = (unsigned const char *)src;
	i = -1;
	while (++i < n)
		dst[i] = source[i];
	return (dest);
}