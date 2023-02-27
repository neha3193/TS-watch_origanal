/* 
 * File:   header.h
 * Author: root
 *
 * Created on February 7, 2014, 10:18 AM
 */
#ifndef HEADER_H
#define	HEADER_H
#ifdef man
#undef var
#else
#define var extern
#endif

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/signal.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/netdevice.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>       
#include <sys/types.h>
#include <sys/stat.h>
#include <error.h>
#include <errno.h>


#undef  DEBUG
#define DEBUG   1 
#define MAX_SIZ 12
#define BLACK   "\033[22;30m"
//#define RED     "\033[22;31m"
#define GREEN   "\033[22;32m"
#define NRM  "\x1B[0m"
#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YEL  "\x1B[33m"
#define BLU  "\x1B[34m"
#define MAG  "\x1B[35m"
#define CYN  "\x1B[36m"
#define WHT  "\x1B[37m"
#define ANSI_UNDERLINED_PRE  "\033[4m"
#define ANSI_UNDERLINED_POST "\033[0m"
#define TOTAL_COMMA 12
#define LOWER_CASE ((argv[path_position][i]>='a')&&(argv[path_position][i]<='z'))
#define UPER_CASE  ((argv[path_position][i]>='A')&&(argv[path_position][i]<='Z'))
#define UNDERSCORE (argv[path_position][i]=='_')
#define DOT	   (argv[path_position][i]=='.')
var struct sockaddr_in bind_add;
var struct parameter 
{
    uint8_t multicast:1;
    uint8_t unicast:1;
    uint8_t report:1;
    uint8_t all:1;
    uint8_t daemon:1;
    uint8_t details:1;
    uint8_t port:1;
    uint8_t tm_avr:1;
    uint8_t eth:1;
    uint8_t fl:1;
    uint8_t ff:1;
}SELECT;
var unsigned char report;
var unsigned char details_flag;
var uint32_t time_avrg;
var struct in_addr my_ip;
var struct sockaddr_in bind_add;
var struct ip_mreq ip_class;
var char file_term_flag;
/*function prototypes*/
var struct in_addr print_ip(char *device);
var  void Print_direction(void);

#ifdef	__cplusplus
}
#endif

#endif	/* HEADER_H */

