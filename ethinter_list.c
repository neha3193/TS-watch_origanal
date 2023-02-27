
#include <stdio.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/netdevice.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
 struct in_addr print_addresses(char *eth_dev)
{
  int s;
  struct ifconf ifconf;
  struct ifreq ifr[50];
  int ifs;
  int i;
  int domain[] = { AF_INET, AF_INET6 };
  for (i = 0; i < sizeof(domain) / sizeof(domain[0]); i++)
  {
	struct sockaddr_in d;
        s = socket(domain[i], SOCK_STREAM, 0);
        if (s < 0) 
        {
                perror("socket");
                //turn d.;
        }
        ifconf.ifc_buf = (char *) ifr;
        ifconf.ifc_len = sizeof ifr;
        //getting Interface list
        if (ioctl(s, SIOCGIFCONF, &ifconf) == -1) 
        {
                perror("ioctl");
                //return 20;
        }
        ifs = ifconf.ifc_len / sizeof(ifr[0]);
        printf("interfaces = %d:\n", ifs);
        for (i = 0; i < ifs; i++) 
        {
                //ar ip[INET_ADDRSTRLEN];
                //char ip[24];
                /*
                struct sockaddr_in *s_in = (struct sockaddr_in *) &ifr[i].ifr_addr;
                if(!inet_ntop(domain, &s_in->sin_addr, ip, sizeof(ip))) 
                {
                        perror("inet_ntop");
                        return 0;
                }
                */
                if(!strcmp(ifr[i].ifr_name,eth_dev))
                {
                close(s);
                //return (((struct sockaddr_in)(ifr[i].ifr_addr)).sin_addr);
                return (d.sin_addr);
                }
      //printf("%s - %s\n", ifr[i].ifr_name, ip);
        }
  }
        close(s);
  
}
/*
int main(int argc, char *argv[])
{
  int domains[] = { AF_INET, AF_INET6 };
  int i;

  for (i = 0; i < sizeof(domains) / sizeof(domains[0]); i++)
  {
    if (!print_addresses(domains[i]))
      return 1;
  }

  return 0;

}*/

