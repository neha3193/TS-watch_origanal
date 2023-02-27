#include "header.h"
struct in_addr print_ip(char *device)
{
        int sock_des,ret_length,i;
	struct in_addr ret,*addr;
	struct ifconf list;
	struct ifreq  req[50];
        char ip[24];
        sock_des=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
        if(sock_des<0)
        {
            perror("socket");
            exit(3);
        }
	//list.ifc_ifcu.ifcu_req=&req;//not included
	//list.ifc_req=&req;//alternate way
	list.ifc_ifcu.ifcu_buf= req;
        list.ifc_buf=req;//could be for simplicity 
        list.ifc_len=sizeof req;
        if(ioctl(sock_des,SIOCGIFCONF,&list)<0)
        {
            perror("ioctl");
            memset(&ret,0,sizeof ret);
            return ret;
        }
        ret_length=list.ifc_len/(sizeof req[0]);
        for(i=0;i<ret_length;i++)
        {
            if(!strcmp(req[i].ifr_ifrn.ifrn_name,device))
            {
                //&(((struct sockaddr_in*)&(req[i].ifr_ifru.ifru_addr))->sin_addr);
                memcpy(&ret, &(((struct sockaddr_in*)&(req[i].ifr_ifru.ifru_addr))->sin_addr),sizeof ret);
                return ret;
                //struct sockaddr_in me;
                //memcpy(&me,req+i,sizeof(me));
            }
        }
        printf(" Selected device is not found or inaccurate word\n");
        
}
/*
void main(int argc , char *argv[])
{
//ruct in_addr ret = print_ip("eth0");
int p;
char j;
struct in_addr ret;
j=p;
//struct in_addr ret;



}
*/
