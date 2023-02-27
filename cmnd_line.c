
#include "header.h"
char path_position;
int fifo_fd;
 void command_input(int argc, char *argv[])
 {
     extern errno;
     int comm_counter,i;
     bind_add.sin_family=AF_INET;
     uint16_t port;
     for(comm_counter=1;comm_counter<argc;comm_counter++)
     {                
         if(!strcmp(argv[comm_counter],"-u"))
         {
             SELECT.unicast=1;
             if(SELECT.multicast)
             {
                 printf("CL_Error:Both options -u and -m are not executable.");
                 Print_direction();
                 exit(9);
             }
         }
         else
         if(!strcmp(argv[comm_counter],"-report"))
         {
             SELECT.report=1;
             report=1;
         }
         else
         if(!strcmp(argv[comm_counter],"-a"))
         {
             SELECT.all=1;
         }
         else
         if(!strcmp(argv[comm_counter],"-D"))//for debug details 
         {
             SELECT.details=1;
             details_flag=1;
         }
         else
	 if(!strcmp(argv[comm_counter],"-term"))
	 {
	     file_term_flag=1;
	 }
	 else
         if(!strcmp(argv[comm_counter],"-t"))
         {
             SELECT.tm_avr=1;
	     if(argc==(comm_counter+1))
	     {
		printf("CL_Error:average time not given following -t\n");
		Print_direction();
		exit(5);
	     }
             time_avrg = atoi(argv[comm_counter+1]);
             if((time_avrg<=0)||(time_avrg>30))
             {
                 printf("CL_Error:average time not given following -t or not an integer\n");
                 Print_direction();
                 exit(12);
             }
         }
         else
         if(!strcmp(argv[comm_counter],"-e"))
         {
             SELECT.eth=1;
             my_ip=print_ip(argv[comm_counter+1]);
             bind_add.sin_addr=my_ip;
         }
         else
         if(!strcmp(argv[comm_counter],"-m"))
         {
             SELECT.multicast=1;
             if(SELECT.unicast)
             {
                 printf("CL_Error:Both options -u and -m are not selectable.");
                 Print_direction();
                 exit(9);
             }
             ip_class.imr_interface=my_ip;
             if((comm_counter+1)!=argc)
             ip_class.imr_multiaddr.s_addr=inet_addr(argv[comm_counter+1]);
             else
             {
                 printf("CL_Error:multi-cast IP is not given following -m\n");
                 Print_direction();
             }
         }
         else
         if(!strcmp(argv[comm_counter],"-p"))
         {   
             SELECT.port=1;
             if((comm_counter+1)==argc)
             {
                 printf("CL_Error:please enter port number -p");
                 Print_direction();
                 exit(2);
             }
             port=atoi(argv[comm_counter+1]);
             if((port<1024)||(port<0)||(port>0XFFFF))
             {
                printf("CL_Error:Invalid Port:%s\n",argv[comm_counter+1]);
                Print_direction();
                exit(78);
             }
             bind_add.sin_port=htons(atoi(argv[comm_counter+1]));
         } 
         else
         if(!strcmp(argv[comm_counter],"-d"))
         {
             SELECT.daemon=1;
         }
         else
         if(!strcmp(argv[comm_counter],"-fl"))
         {
             if(SELECT.ff)
             {
                 printf("CL_Error:please select one -ff or -fl for fifo or file\n");
                  Print_direction();
                 exit(56);
             }
             if(comm_counter+1==argc)
             {
                 printf("CL_Error:please give pathname of director following -fl\n");
                 Print_direction();
                 exit(57);
             }
             SELECT.fl=1;
             path_position=comm_counter + 1;
         }
         else
         if(!strcmp(argv[comm_counter],"-ff"))
         {
             if(SELECT.fl)
             {
                 printf("CL_Error:please select one -ff or -fl for fifo or file redirection\n");
                 Print_direction();
                 exit(56);
             }
             if((comm_counter+1)==argc)
             {
                printf("CL_Error:please give fifo name following -ff\n");
                Print_direction();
                exit(23);
             }  
             path_position=comm_counter + 1;
             SELECT.ff=1;     
         }
     }
     if(!SELECT.eth)
     {
         printf("CL_Error:please select an ethernet interface:-e [device name]\n");
         Print_direction();
         exit(2);
     }
     if(!SELECT.port)
     {
         printf("CL_Error:port number required\n");
         Print_direction();
         exit(3);
     }
     if((report)&&(details_flag))
     {
         printf("CL_Error:-report and -D cannot be selected both");
         Print_direction();
         exit(11);
     }
     if((SELECT.daemon)&&(SELECT.ff))
     {
         for(i=0;argv[path_position][i]!=0;i++)
         {
             if(!(LOWER_CASE||UPER_CASE||UNDERSCORE||DOT))
             {
                printf("CL_Error:fifo name should be combination of alphabets and/or underscore\n");
                Print_direction();
                exit(9);                
             }
         }
         mkfifo(argv[path_position],0666); 
	 if(!file_term_flag)
         if((fifo_fd = open(argv[path_position],O_WRONLY|O_APPEND))<0)
         {
             //perror("opening of FIFO failed:");
             Print_direction();
             exit(7);
         }
     }
     if((SELECT.daemon+SELECT.ff == 1)&&(!SELECT.fl))//if -d/-ff option is selected  
     {
         if(!SELECT.daemon)
         printf("CL_Error:-d option is not selected to make it daemon or either remove -ff [fifo_name] option\n");
         if(!SELECT.ff)
         printf("CL_Error:-ff option is not selected to interprete fifo_name or either remove -d option\n");         
         Print_direction();
         exit(12);
     }
     if(!SELECT.tm_avr)
     {
	time_avrg=5;
     }
     /*
     if((SELECT.daemon)&&((report)||(details_flag)))
     {
         printf("CL_Error:Invalid option\"-report\" or \"-D\" with -d\n");
         Print_direction();
         exit(5);
     }
     */
 }
 void Print_direction(void)
{
    printf("\n./ts_info \n");
    printf("-m  [multicast IP]   for multicast receive IP\n");
    printf("-u  [unicast   IP]   for unicast receivr IP\n");
    printf("-p  [port]           for port");
    printf("-t  [seconds]        for average time for bit rate and SDT, default:10 seconds \n");
    printf("-d                   for making it daemon process\n");
    printf("-ff [fifo file]      for redirection to a fifo file if -d option is chosen\n");
    printf("-fl [regular file])  for redirection to a regular file if -f option is chosen\n");
    printf("-e  [ethernet if]    for ethernet device interface default: eth0\n"); 
}
