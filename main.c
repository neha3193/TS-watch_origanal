
/* 
 * Author: feanisd.sw
 * Multi cast Enabled 
 * with complete packet info AND bit rates of 
 * PAT, PMT, AUDIO, VIDEO 
 * Created on November 22, 2013, 12:14 PM
 * http://forum.digital-digest.com/f4/help-extract-i-frames-mpeg2-ts-89736.html
 */
#define man 
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>
#include <sys/signal.h>
#include <fcntl.h>
#include <time.h>
#include <arpa/inet.h>

#undef  DEBUG
#define DEBUG   1 
#define MAX_SIZ 12
/*
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
 */
#define TOTAL_COMMA 12
struct sockaddr_in bind_add={0},sorc_add ={0};
typedef int filedescriptor;
unsigned char report=0;
filedescriptor fdd;
 volatile unsigned char thread_exit=0;
 char t_prog=0;
 char print_opt=0;
 unsigned int ii,hh,dd=1;
 pthread_t thread_id,print_thread_id,req_print,colounm_print_thread_id;
 pthread_mutex_t mutex_var;
 unsigned short no_prog;
 unsigned short pid;
 unsigned long dcont_pat;
 unsigned long dcont_pmt;
 unsigned long dcont_audio;
 unsigned long dcont_video;
 unsigned int ts_count=0;
 struct ip_mreq ip_class={0};
 unsigned char three_bit_info;
 unsigned char tsafcc;
 unsigned char afc;
 unsigned char cc;
 unsigned char pntr_field;
 unsigned char pmt_pntr_field;
 unsigned char table_id;
 unsigned char table_idd;
 double pat_byte;
 double pmt_byte;
 double audio_byte;
 double video_byte;
 double total_byte;
 unsigned short int sil;
 unsigned short int sl;//section length
 unsigned short int pmt_sl;//pmt_sl section length 
 unsigned short int tsi;
 unsigned char vncni;
 unsigned char vn;//Version number
 unsigned char cni;
 unsigned char sn;//section number
 unsigned char lsn;
 unsigned long crc;
 unsigned long pmt_crc;
 unsigned short pcr_pid;
 unsigned short pil;
 unsigned char st;
 unsigned char sst;
 unsigned short epid;
 unsigned short epid_nxt;
 unsigned short es_info_len;
 unsigned short es_info_total;
 unsigned short es_info_len_nxt;
 unsigned short av_pid_siz;
 unsigned char *SDT_buf=0;
 unsigned short running_free_length;
 unsigned char EIT_schedule_present;
 unsigned short descriptor_loop_length;
 unsigned short org_service_id;
 unsigned char descriptor_tag;	
 unsigned char descriptor_len;
 unsigned char service_type;
 unsigned char service_provider_name_length;
 unsigned char service_name_length;
 unsigned char buf[1316];
 unsigned int counter_start_flag;
 //char pid_buf[8192];//NL
 uint32_t time_avrg=0;
 char thread_break=0;
 extern filedescriptor fd;
/*****PES PARAMETERS******/ 
 unsigned char  adapt_field_length=0;
 unsigned long  pes_start_cprefix;
 unsigned char  pes_stream_id;
 unsigned short pes_packet_leng;
 unsigned char  pes_byte_info;
 unsigned char  pes_scramble_control;
 unsigned char  pes_priority;
 unsigned char  data_alignment_indicator;
 unsigned char  pes_copyright;
 unsigned char  pes_original_or_copy;
 unsigned char  pes_seven_flags;
 unsigned char  pes_pts_dts_flags;
 unsigned char  escr_flag ;
 unsigned char  es_rate_flag;
 unsigned char  dsm_trick_mode_flag;
 unsigned char  additional_copy_info_flag;
 unsigned char  pes_crc_flag;
 unsigned char	pes_ext_flag;
 unsigned char  pes_header_data_length;
 unsigned long long pts;
 unsigned long long dts;
 unsigned int loop_index;
 unsigned char A_V;
 unsigned char print_flag;
 double T_Brate = 0;
 unsigned char hid_flag=0;
 unsigned char SDT_section_no=0;
/*********END*********/
unsigned char start_check_flag=0;
unsigned int b_sz=0;
char dirbuf[100] = {0};
char file_term_flag=0;
//#define USE_IT 5

typedef 
enum comm_lst   {
    PAT,
    PMT,
    SDT,
    RST,//Reset
    STR,//Store
    RET//Return
                }COMMAND_LIST;
struct parameter 
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
typedef struct pmt_aud_head
{
    unsigned short int aud_pid;
    unsigned char type;
    struct pmt_aud_head *next_aud_pid;
    
}PMT_AUD;
typedef struct pmt_data_head
{
    unsigned short int  data_pid;
    struct pmt_data_head *next_data_pid;
    
}PMT_DATA;
typedef struct ts_info
{
    unsigned short pmt_pid;
    unsigned short prog_no;
    unsigned char *src_provider;
    unsigned char *srv_name;
    unsigned short video_pid;
    unsigned char video_type;
    PMT_AUD  * aud_head;
    PMT_DATA * data_head;
    struct ts_info *ts_info_next;
}TS_INFO;
unsigned char details_flag = 0,var_argc;
 void auto_subfile (char *dir, char *sub_dir, char *file, char *string);
 int PES_VideoDecoder(void);
 int PES_AudioDecoder(void);
 void disp_audio_mode(char mod); 
 void Print_ts_InFO();
 void *difformat_thread(void *arg);
  void command_input(int argc, char *argv[]);
  int mainprocess(void);
  struct in_addr print_ip(char *device);
 TS_INFO *TS_INFO_HEAD=0,*TS_INFO_TAIL=0;
 static PMT_AUD  * aud_head=0;
 static PMT_AUD  * aud_tail=0;
 static PMT_DATA  * data_head=0;
 static PMT_DATA  * data_tail=0;
 void print_video_type(unsigned char stream_type,char file_flag);
 typedef struct pidd_buf
 {
	char CC;
	double  total_bit_rate;
	unsigned int dis_cont;
 }PIDD_BUF; 
 
 typedef struct pid_list
 {
	unsigned int pid;
        unsigned long long bit_rate;
	struct pid_list *next;
 }PID_LIST;
 PID_LIST *PID_LIST_HEAD=0, *PID_LIST_TAIL=0;
 PIDD_BUF pidd_buf[8192]={0};
 struct in_addr my_ip;
 char ADD_ts_infO(COMMAND_LIST command,unsigned short prog_no,unsigned short pmt_pid,unsigned char *srv_name, 
                  unsigned char *src_provider,unsigned short video_pid,PMT_AUD * aud_head,PMT_DATA * data_head,unsigned char v_typ)
 {
     TS_INFO *temp;
     
     //printf("Function's service Provider:%s %s\n",srv_name,src_provider);
     switch(command)
     {
        case PAT:
        {
            /**/
            TS_INFO *new_pmt=malloc(sizeof(TS_INFO));
            new_pmt->ts_info_next       = 0;
            new_pmt->aud_head           = 0;
            new_pmt->data_head          = 0;
            new_pmt->src_provider       = 0;
            new_pmt->srv_name           = 0;
            new_pmt->video_pid          = 0;
            new_pmt->aud_head           = 0;
            new_pmt->data_head          = 0;
            new_pmt->pmt_pid            = pmt_pid;
            new_pmt->prog_no            = prog_no;
            if((TS_INFO_TAIL||TS_INFO_HEAD)==0)
            {
                TS_INFO_TAIL = new_pmt;
		TS_INFO_HEAD = new_pmt;
                //printf("Prog_no:%hu PMT_Pid:%u\n",new_pmt->prog_no,new_pmt->pmt_pid);
            }
	    else
	    {
		TS_INFO_TAIL = TS_INFO_TAIL->ts_info_next=new_pmt;
                TS_INFO_TAIL->ts_info_next = 0;
                //printf("Prog_no:%hu PMT_Pid:%u\n",new_pmt->prog_no,new_pmt->pmt_pid);
                
                /*
		while(temp)
		{
			if(temp->pmt_pid == pmt_pid)
			{
				//exit(0);
				return 0;
			}
			temp = temp->ts_info_next;
		} 
		TS_INFO_TAIL->ts_info_next = new_pmt;
		TS_INFO_TAIL = new_pmt;
                 */
	    }
        }
        break;
        case PMT:
        {   temp = TS_INFO_HEAD;
            while(temp)
            {
                if(temp->prog_no == prog_no)
		{
                    temp->video_pid = video_pid;
                    temp->video_type= v_typ;
                    temp->aud_head  = aud_head;
                    temp->data_head = data_head;
                    return 1;
		}
		temp = temp->ts_info_next;
	    } 
        }
        break;
        case SDT:
        {   temp = TS_INFO_HEAD;
            while(temp)
            {
                if(temp->prog_no == prog_no)
		{
                    temp->src_provider  = src_provider;
                    temp->srv_name      = srv_name;
		}
		temp = temp->ts_info_next;
	    } 
        }
        break;
     }
 }
  PMT_AUD *Store_audio_inFO(unsigned short aud_pid,COMMAND_LIST command,unsigned char type)
 {
      switch(command)
      {
        case STR:
        {
                if(!aud_head||!aud_tail)
                {
                    aud_head=aud_tail   = (PMT_AUD*)malloc(sizeof(PMT_AUD)); 
                    aud_head->aud_pid   = aud_pid;
                    aud_head->type      = type;
                    aud_tail->next_aud_pid=0;
                }
                else
                {
                    aud_tail=aud_tail->next_aud_pid=(PMT_AUD*)malloc(sizeof(PMT_AUD));
                    aud_tail->aud_pid=aud_pid;
                    aud_tail->type =type;
                    aud_tail->next_aud_pid=0;
                }
        }
        break;
        case RST:
        {
            aud_head=0;
            aud_tail=0;
        }
        break;
        case RET:
            return aud_head;
        break;
      }
      return aud_head;
 
 }
  unsigned long long Show_bitrate(unsigned short pid)
 {
     PID_LIST  *list_head = PID_LIST_HEAD;
     while(list_head)
     {
         if(list_head->pid==pid)
         {
             //printf("%llu",list_head->bit_rate);
             return list_head->bit_rate;
         }
         list_head=list_head->next;
     }
     return -1;
 }
 PMT_DATA *Store_data_inFO(unsigned short data_pid,COMMAND_LIST command)
 {
      switch(command)
      {
        case STR:
        {
                if(!data_head||!data_tail)
                {
                    data_head=data_tail= malloc(sizeof(PMT_DATA)); 
                    data_head->data_pid =data_pid;
                    data_tail->next_data_pid=0;
                }
                else
                {
                    data_tail=data_tail->next_data_pid=malloc(sizeof(PMT_DATA));
                    data_tail->data_pid=data_pid;
                    data_tail->next_data_pid=0;
                }
        }
        break;
        case RST:
        {
            data_head=0;
            data_tail=0;
        }
        break;
        case RET:
            return data_head;
        break;
      }
      return data_head;
 
 }
 void *Print_Thread_ts_InFO(void *arg)
 {
     TS_INFO *temp=TS_INFO_HEAD;
     PMT_AUD  *temp_aud;
     PMT_DATA *temp_dat;
     unsigned char comma_counter=0;
     unsigned long long brate=0;
     char i;
     char buf[400]={0};
     char xbuf[100]={0};
     //while(1)
     sleep(time_avrg*2);
     if(thread_break)
     {
         printf("thread breaked\n");
         pthread_exit(0);
     }
     {
     temp=TS_INFO_HEAD;
     //while(getchar()!='\n');
     {
     
     //printf(MAG);
     sprintf(buf,"SRV_NO  ,PROVIDER_NAME     ,SERVICE_NAME            ,PMT_PID     ,VEDIO_PID      ,V_TYPE     ,AUD_PID_0   ,A_TYP   ,AUD_PID_1   ,A_TYP   ,AUD_PID_2   ,A_TYP\n");
     write(fdd,buf,strlen(buf));
     printf(buf);
     while(temp)
     {
        comma_counter=0;
        if(Show_bitrate(temp->pmt_pid)==-1)
        sprintf(buf,"%-8hu,%-18s,%-24s,%-5hu[%-3s]",temp->prog_no,temp->src_provider,temp->srv_name,temp->pmt_pid,"NAV");
        else
        sprintf(buf,"%-8hu,%-18s,%-24s,%-5hu[%-3llu]",temp->prog_no,temp->src_provider,temp->srv_name,temp->pmt_pid,Show_bitrate(temp->pmt_pid));
        comma_counter+=4;
        write(fdd,buf,strlen(buf));
        printf(buf);
        if((temp->pmt_pid >= 0)&&(temp->pmt_pid <= 18))
        {
                sprintf(buf,"\n",1);
                write(fdd,buf,strlen(buf));
                printf(buf);
                temp=temp->ts_info_next;
                continue;
        }
        sprintf(buf,"  ,%-5hu[%-5llu]   ,",temp->video_pid,Show_bitrate(temp->video_pid));
        comma_counter++;
        write(fdd,buf,strlen(buf));
        printf(buf);
        print_video_type(temp->video_type,0);
        print_video_type(temp->video_type,1);
        comma_counter++;
        temp_aud=temp->aud_head;
        if(!temp_aud)
        {
                sprintf(buf,"%hu ,",temp_aud);
                write(fdd,buf,strlen(buf));
                comma_counter++;
                printf(buf);
        }
        while(temp_aud)
        {
            sprintf(buf,"%-4hu[%-3llu]   ,",temp_aud->aud_pid,Show_bitrate(temp_aud->aud_pid));
            write(fdd,buf,strlen(buf));
            printf(buf);
            comma_counter++;
            print_video_type(temp_aud->type,0);
            print_video_type(temp_aud->type,1);
            comma_counter++;
            temp_aud=temp_aud->next_aud_pid;
        }
        for(i=TOTAL_COMMA-comma_counter;i>0;i--)
        {
           write(fdd," ,",2);
           printf(" ,");
        }
         sprintf(buf,"\n",1);
         write(fdd,buf,strlen(buf));
         printf(buf);
         temp=temp->ts_info_next;
     }
//     printf(MAG);
     }
     }
     start_check_flag=1;
     //intf("pthread exited\n");
     if(file_term_flag)
     exit(10);
     pthread_exit(0);
 }
 

 void Print_ts_InFO_type()
 {
     TS_INFO *temp=TS_INFO_HEAD;
     PMT_AUD  *temp_aud;
     PMT_DATA *temp_dat;
     if(report)
     {
     printf("SRV_NO  PROVIDER_NAME     SERVICE_NAME            [PMT_PID ]  [VEDIO_PID ]   V_TYPE     AUD_PID_0   AUD_PID_1   AUD_PID_2  AUD_PID_3  AUD_PID_4\n");
     while(temp)
     {
     printf("%-8hu%-18s%-24s%-5hu[%-3llu]  %-5hu[%-5llu]   ",temp->prog_no,temp->src_provider,temp->srv_name,temp->pmt_pid,Show_bitrate(temp->pmt_pid),temp->video_pid,Show_bitrate(temp->video_pid));
    
         print_video_type(temp->video_type,1);
         temp_aud=temp->aud_head;
         if(!temp_aud)
         printf("%hu ",temp_aud);
         while(temp_aud)
         {
             printf("%hu[%-3llu]   ",temp_aud->aud_pid,Show_bitrate(temp_aud->aud_pid));
             temp_aud=temp_aud->next_aud_pid;
         }
         temp=temp->ts_info_next;
         printf("\n");
     }
     }
     else
     {
         printf("SRV_NO  PROVIDER_NAME       SERVICE_NAME             PMT_PID    VEDIO_PID   V_TYPE     AUDIO_PID\n");
         while(temp)
         {
         printf("%-8hu%-20s%-25s%-11hu%-12hu",temp->prog_no,temp->src_provider,temp->srv_name,temp->pmt_pid,temp->video_pid);
   /*  
         temp_dat=temp->data_head;
         if(!temp_dat)
         printf("%-9s ","__");
         while(temp_dat)
         {
             printf("%-9hu ",temp_dat->data_pid);
             temp_dat = temp_dat -> next_data_pid;
         }
    */   print_video_type(temp->video_type,1);
         temp_aud=temp->aud_head;
         if(!temp_aud)
         printf("%hu ",temp_aud);
         while(temp_aud)
         {
             printf("%hu ",temp_aud->aud_pid);
             temp_aud=temp_aud->next_aud_pid;
         }
         temp=temp->ts_info_next;
         printf("\n");
     }
     }
     
 }
 void print_video_type(unsigned char stream_type, char file_flag)
 {
     char buf[20];
     switch(stream_type)
     {
         case 2://Video Sequence Header
         if(file_flag)
         printf("%-11s,","mpeg2vid");
         else
         {
                sprintf(buf,"%-11s,","mpeg2vid");
                write(fdd,buf,strlen(buf));
         }
         break;
         case 3:
         case 4://
         if(file_flag)
         printf("%-8s,","mp2");
         else
         {
                sprintf(buf,"%-8s,","mp2");
                write(fdd,buf,strlen(buf));
         }
         break;
         case 15://
         if(file_flag)
         printf("%-8s,","aac");
         else
         {
                sprintf(buf,"%-8s,","aac");
                write(fdd,buf,strlen(buf));
         }
         break;
         case 129://
         if(file_flag)
         printf("%-8s,","dolby");
         else
         {
                sprintf(buf,"%-8s,","aac");
                write(fdd,buf,strlen(buf));
         }
         break;
         case 27://H.264 header
         if(file_flag)
         printf("%-11s,","h264");
         else
         {
                sprintf(buf,"%-11s,","h264");
                write(fdd,buf,strlen(buf));
         }
         break;
 
         default:
         if(file_flag)
         printf("%-11s,","unknown");
         else
         {
         sprintf(buf,"%-11s,","unknown");
         write(fdd,buf,strlen(buf));
         }
     };
     //printf(BLU);
 }
 void STore_Ts_Info(void)
 {
     TS_INFO *temp=TS_INFO_HEAD;
     PMT_AUD  *temp_aud;
     PMT_DATA *temp_dat;
     unsigned char comma_counter=0;
     char i;
     char buf[400]={0};
     sprintf(buf,"SRV_NO  ,PROVIDER_NAME     ,SERVICE_NAME            ,PMT_PID     ,VEDIO_PID      ,V_TYPE     ,AUD_PID_0   ,A_TYP   ,AUD_PID_1   ,A_TYP   ,AUD_PID_2   ,A_TYP\n");
     write(fdd,buf,strlen(buf));
     printf(buf);
     while(temp)
     {
        comma_counter=0;
        sprintf(buf,"%-8hu,%-18s,%-24s,%-5hu[%-3llu]",temp->prog_no,temp->src_provider,temp->srv_name,temp->pmt_pid,Show_bitrate(temp->pmt_pid));
        comma_counter+=4;
        write(fdd,buf,strlen(buf));
        printf(buf);
        if((temp->pmt_pid >= 0)&&(temp->pmt_pid <= 18))
        {
                sprintf(buf,"\n",1);
                write(fdd,buf,strlen(buf));
                printf(buf);
                temp=temp->ts_info_next;
                continue;
        }
        sprintf(buf,"  ,%-5hu[%-5llu]   ,",temp->video_pid,Show_bitrate(temp->video_pid));
        comma_counter++;
        write(fdd,buf,strlen(buf));
        printf(buf);
        print_video_type(temp->video_type,0);
        print_video_type(temp->video_type,1);
        comma_counter++;
        temp_aud=temp->aud_head;
        if(!temp_aud)
        {
                sprintf(buf,"%hu ,",temp_aud);
                write(fdd,buf,strlen(buf));
                comma_counter++;
                printf(buf);
        }
        while(temp_aud)
        {
            sprintf(buf,"%-4hu[%-3llu]   ,",temp_aud->aud_pid,Show_bitrate(temp_aud->aud_pid));
            write(fdd,buf,strlen(buf));
            printf(buf);
            comma_counter++;
            print_video_type(temp_aud->type,0);
            print_video_type(temp_aud->type,1);
            comma_counter++;
            temp_aud=temp_aud->next_aud_pid;
        }
        for(i=TOTAL_COMMA-comma_counter;i>0;i--)
        {
           write(fdd," ,",2);
           printf(" ,");
        }
         sprintf(buf,"\n",1);
         write(fdd,buf,strlen(buf));
         printf(buf);
         temp=temp->ts_info_next;
     }
	if(file_term_flag)
	exit(9);
     //close(fdd);
     
 }
  void Print_ts_InFO()
 {
     TS_INFO *temp=TS_INFO_HEAD;
     PMT_AUD  *temp_aud;
     PMT_DATA *temp_dat;
     printf("SRV_NO  PROVIDER_NAME       SERVICE_NAME             PMT_PID    VEDIO_PID   DATA_PID  AUDIO_PID\n");
     while(temp)
     {
     printf("%-8hu%-20s%-25s%-11hu%-12hu",temp->prog_no,temp->src_provider,temp->srv_name,temp->pmt_pid,temp->video_pid);
         temp_dat=temp->data_head;
         if(!temp_dat)
         printf("%-9s ","__");
         while(temp_dat)
         {
             printf("%-9hu ",temp_dat->data_pid);
             temp_dat = temp_dat -> next_data_pid;
         }
         temp_aud=temp->aud_head;
         if(!temp_aud)
         printf("%hu ",temp_aud);
         while(temp_aud)
         {
             printf("%hu ",temp_aud->aud_pid);
             temp_aud=temp_aud->next_aud_pid;
         }
         temp=temp->ts_info_next;
         printf("\n");
     }
     
 }
 
 
void EIT_Event_status(unsigned char table_id)
{
 if((table_id >= 0x50) && (table_id <= 0x5F))
 {
	table_id=0;
 }
 if((table_id >= 0x60) && (table_id <= 0x6F))
 {
	table_id=1;
 }
 switch(table_id)
 {
 case 0:
	printf("actual TS, event schedule information = table_id = \"0x50\" to \"0x5F\"\n");
 break;
 case 1:
	printf("other TS, event schedule information = table_id = \"0x60\" to \"0x6F\"\n");
 break;
 case 0x4E:
 	printf("actual TS, present/following event information\n");
 break;
 case 0x4F:
	printf("other TS, present/following event information\n");
 break;
 }
}

void Align_type(unsigned char Alignment_type ,char A_V)
{
 switch(Alignment_type)
 {	
    case 0:
    printf("reserved\n");
    break;
    case 1:
    printf("slice, picture, GOP, or sequence\n");
    break;
    case 2:
    printf("picture, GOP, or sequence\n");
    break;
    case 3:
    printf("GOP or sequence\n");
    break;
    case 4:
    printf("sequence\n");
    break;
    default:
    printf("reserved\n");
 }
}

 char TS_descriptor_decoder(unsigned char descriptor_tag)
 {
     unsigned int ii;
     unsigned char *service_provider_pntr=0;
     unsigned char *service_name_pntr=0;
   switch(descriptor_tag)
   {
	case 0x00:
#ifdef DEBUG
        if(details_flag)
#endif
	printf("Reserved\n");
	break;
	case 0x01:
#ifdef DEBUG
        if(details_flag)
#endif
	printf("Reserved\n");
	break;
	case 0x02:
#ifdef DEBUG
        if(details_flag)
#endif
	printf("Video Stream Descriptor\n");
	break;
	case 0x03:
#ifdef DEBUG
        if(details_flag)
#endif
	printf("Audio Stream Descriptor\n");
	break;
	case 0x04:
#ifdef DEBUG
        if(details_flag)
#endif
	printf("Hierarchy Descriptor\n");
	break;
	case 0x05:
#ifdef DEBUG
        if(details_flag)
#endif
	printf("Registration Descriptor\n");
	break;
	case 0x06:
#ifdef DEBUG
        if(details_flag)
#endif
        {
	printf("Data Stream Alignment Descriptor\n");
	printf("Descriptor Len:%02x\n",*(buf+ts_count + loop_index+ pil + pntr_field + es_info_total+1));
	printf("Alignment_type:%02x\n",*(buf+ts_count + loop_index+ pil + pntr_field + es_info_total+2));
	Align_type(*(buf+ts_count + loop_index+ pil + pntr_field + es_info_total+2),A_V);
        }
	break;
	case 0x07:
#ifdef DEBUG
        if(details_flag)
#endif
	printf("Target Background Grid Descriptor\n");
	break;
	case 0x08:
#ifdef DEBUG
        if(details_flag)
#endif
	printf("Video Window Descriptor\n");
	break;
	case 0x09:
#ifdef DEBUG
        if(details_flag)
#endif
	printf("CA Descriptor\n");
	break;
	case 0x0A:
#ifdef DEBUG
        if(details_flag)
#endif
        {
	printf("ISO Language Descriptor\n");
	printf("Descriptor Len:%02x\n",*(buf+ts_count + loop_index+ pil + pntr_field + es_info_total+1));
	printf("LANGUAGE:");
	printf("%s\n",buf+ts_count + loop_index+ pil + pntr_field + es_info_total+2);
        }
	break;
	case 0x0B:
#ifdef DEBUG
        if(details_flag)
#endif
	printf("System Clock Descriptor\n");
	break;
	case 0x0C:
#ifdef DEBUG
        if(details_flag)
#endif
	printf("Multiplex Buffer Utilization Descriptor\n");
	break;
	case 0x0D:
#ifdef DEBUG
        if(details_flag)
#endif
	printf("Copyright Descriptor\n");
	break;
	case 0x0E:
#ifdef DEBUG
        if(details_flag)
#endif
	printf("Maximum Bitrate Descriptor\n");	
	break;
	case 0x0F:
#ifdef DEBUG
        if(details_flag)
#endif
	printf("Private Data Indicator Descriptor\n");
	break;
	case 0x10:
#ifdef DEBUG            
        if(details_flag)
#endif
	printf("Smoothing Buffer Descriptor\n");
	break;
	case 0x11:
#ifdef DEBUG
        if(details_flag)
#endif           
	printf("STD_descriptor\n");
	break;
	case 0x12:
#ifdef DEBUG   
        if(details_flag)
#endif          
	printf("IBP_descriptor\n");
	break;
//Other Descriptors
	case 0x13:
	printf("");
	break;
	case 0x14:
	printf("");
	break;
	case 0x40:
#ifdef DEBUG            
        if(details_flag)
#endif       
	printf("network_name_descriptor\n");
	break;
	case 0x41:
#ifdef DEBUG
        if(details_flag)
#endif            
	printf("service_list_descriptor\n");
	break;
        case 0x42:
#ifdef DEBUG            
        if(details_flag)
#endif            
	printf("stuffing_descriptor\n");
	break;
	case 0x43:
#ifdef DEBUG            
        if(details_flag)
#endif            
	printf("satellite_delivery_system_descriptor\n");
	break;
	case 0x44: 
#ifdef DEBUG            
        if(details_flag)
#endif            
	printf("cable_delivery_system_descriptor\n");
	break;
	case 0x45: 
#ifdef DEBUG            
        if(details_flag)
#endif            
	printf("VBI_data_descriptor\n");
	break;
	case 0x46: 
#ifdef DEBUG            
        if(details_flag)
#endif            
	printf("VBI_teletext_descriptor");
	break;
	case 0x47:
#ifdef DEBUG            
        if(details_flag)
#endif            
	printf("bouquet_name_descriptor");
	break;
	case 0x48: 
	{
       
           descriptor_loop_length = (running_free_length & 0x0FFF);
	   //printf(CYN"%hu     "BLU,org_service_id);
	   //printf(CYN"0x%02X   "BLU, descriptor_tag);
           *((unsigned char *)(SDT_buf+ts_count+  (b_sz +=1)));
	   //printf("descriptor_len:%02x\n",*((unsigned char *)(SDT_buf+ts_count+  (b_sz +=1))));
           *((unsigned char *)(SDT_buf+ts_count+  (b_sz +=1)));
	   //printf("service_typ:"CYN"%u\n"BLU,*((unsigned char *)(SDT_buf+ts_count+  (b_sz +=1))));	
	   service_provider_name_length= *((unsigned char *)(SDT_buf+(b_sz+=1)));
	   //printf("service_provider_name_length:%u\n",service_provider_name_length=*((unsigned char *)(SDT_buf+(b_sz+=1))));
	   if(service_provider_name_length)
	   {
                service_provider_pntr=malloc(service_provider_name_length+1);
		for(ii=0;ii<service_provider_name_length;ii++)
		{
                         service_provider_pntr[ii]=*((unsigned char *)(SDT_buf+(b_sz+=1)));
			//printf(CYN"%c"BLU,*((unsigned char *)(SDT_buf+(b_sz+=1))));
		}
                service_provider_pntr[service_provider_name_length]='\0';
                 //printf("   ");
	   }	
	   service_name_length=*((unsigned char *)(SDT_buf+(b_sz+=1)));
	   //printf("service_name_length:%u\n",service_name_length=*((unsigned char *)(SDT_buf+(b_sz+=1))));
	   if(service_name_length)
	   {
                service_name_pntr=malloc(service_name_length+1);
		for(ii=0;ii<service_name_length;ii++)
		{
			//printf(CYN"%c"BLU,*((unsigned char *)(SDT_buf+(b_sz+=1))));
                        service_name_pntr[ii]=*((unsigned char *)(SDT_buf+(b_sz+=1)));
		}
                service_name_pntr[service_name_length]='\0';
                //printf("SERVICE_NAME:%s\n",service_name_pntr);
                //printf("SERVICE_PROV:%s\n",service_provider_pntr);
	   }	
	   b_sz+=1;
           //Print_ts_InFO();
           ADD_ts_infO(SDT,org_service_id,0,service_name_pntr,
                  service_provider_pntr,0,0,0,0);
           
           /*char ADD_ts_infO(COMMAND_LIST command//1,unsigned short prog_no//2,unsigned short pmt_pid//3,unsigned char *srv_name//4, 
                  unsigned char *src_provider//5,unsigned short video_pid//6,PMT_AUD_HEAD * aud_head//7,PMT_DATA_HEAD * data_head//8)*/
   	   //printf("\n");
           /*
	   switch(table_id)
	   {
		case 0x42:
		printf("service_description_section -"CYN"actual_transport_stream\n"BLU);	
		break;
	   }
	   printf("\n");
            */
	}
	break;
	case 0x49:
#ifdef DEBUG        
        if(details_flag)
#endif
	printf("country_availability_descriptor\n"); 
	break;
	case 0x4A:
#ifdef DEBUG
        if(details_flag)
#endif
	printf("linkage_descriptor\n"); 
	break;
	case 0x4B: 
#ifdef DEBUG        
        if(details_flag)
#endif
	printf("NVOD_reference_descriptor\n"); 
	break;
	case 0x4C:
#ifdef DEBUG            
        if(details_flag)
#endif
	printf("time_shifted_service_descriptor\n"); 
	break;
	case 0x4D: 
#ifdef DEBUG            
        if(details_flag)
#endif            
	printf("short_event_descriptor\n"); 
	break;
	case 0x4E: 
#ifdef DEBUG            
        if(details_flag)
#endif           
	printf("extended_event_descriptor\n"); 
	break;
	case 0x4F: 
#ifdef DEBUG            
        if(details_flag)
#endif            
	printf("time_shifted_event_descriptor\n"); 
	break;
	case 0x50:
#ifdef DEBUG            
        if(details_flag)
#endif
	printf("component_descriptor\n"); 
	break;
	case 0x51: 
#ifdef DEBUG            
        if(details_flag)
#endif            
	printf("mosaic_descriptor\n"); 
	break;
	case 0x52: 
#ifdef DEBUG        
        if(details_flag)
        {
	printf("stream_identifier_descriptor\n"); 
	printf("Descriptor Len:%02x\n",*(buf+ts_count + loop_index+ pil + pntr_field + es_info_total+1));
	printf("Component Tag :%02x\n",*(buf+ts_count + loop_index+ pil + pntr_field + es_info_total+2));
        }
#endif       
	break;
	case 0x53:
#ifdef DEBUG
        if(details_flag)
#endif
	printf("CA_identifier_descriptor\n"); 
	break;
	case 0x54: 
#ifdef DEBUG
        if(details_flag)
#endif           
	printf("content_descriptor\n"); 
	break;
	case 0x55:
#ifdef DEBUG
        if(details_flag)
#endif          
	printf("parental_rating_descriptor\n"); 
	break;
	case 0x56:
#ifdef DEBUG            
        if(details_flag)
#endif
	printf("teletext_descriptor\n"); 
	break;
	case 0x57: 
#ifdef DEBUG           
        if(details_flag)
#endif            
	printf("telephone_descriptor\n"); 
	break;
	case 0x58:
#ifdef DEBUG
        if(details_flag)
#endif
	printf("local_time_offset_descriptor\n"); 
	break;
	case 0x59:
#ifdef DEBUG
        if(details_flag)
#endif
	printf("subtitling_descriptor\n"); 
	break;
	case 0x5A:
#ifdef DEBUG
        if(details_flag)
#endif        
	printf("terrestrial_delivery_system_descriptor\n"); 
	break;
	case 0x5B:
#ifdef DEBUG
        if(details_flag)
#endif
	printf("multilingual_network_name_descriptor\n"); 
	break;
	case 0x5C:
#ifdef DEBUG
        if(details_flag)
#endif
	printf("multilingual_bouquet_name_descriptor\n"); 
	break;
	case 0x5D: 
#ifdef DEBUG
        if(details_flag)
#endif
	printf("multilingual_service_name_descriptor \n"); 
	break;
/*
   multilingual_component_descriptor
   0x5E
   private_data_specifier_descriptor
   0x5F
   service_move_descriptor
   0x60
   short_smoothing_buffer_descriptor
   0x61
   frequency_list_descriptor
   0x62
   partial_transport_stream_descriptor
   0x63
   data_broadcast_descriptor
   0x64
   scrambling_descriptor
   0x65
   data_broadcast_id_descriptor
   0x66
   transport_stream_descriptor
   0x67
   DSNG_descriptor 
   0x68
   PDC_descriptor
   0x69
   AC-3_descriptor (see annex D)
   0x6A
   ancillary_data_descriptor
   0x6B
   cell_list_descriptor
   0x6C
   cell_frequency_link_descriptor
   0x6D
   announcement_support_descriptor
   0x6E
   application_signalling_descriptor
   0x6F
   adaptation_field_data_descriptor
   0x70
   service_identifier_descriptor
   0x71
   service_availability_descriptor
   0x72
   default_authority_descriptor
   0x73
   (TS 102 323 [36])
   related_content_descriptor
   0x74
   (TS 102 323 [36])
   TVA_id_descriptor (TS 102 323 [36])
   0x75
   content_identifier_descriptor
   0x76
   (TS 102 323 [36])
   time_slice_fec_identifier_descriptor
   0x77
   (EN 301 192 [37]) (see note 3)
   ECM_repetition_rate_descriptor
   0x78
   (EN 301 192 [37])
   reserved for future use
   0x79 to 0x7F
   user defined
   0x80 to 0xFE
   forbidden
   0xFF
*/
   }		
 }


 unsigned long long  pts_dts_decoder(unsigned long long input)
 {
 unsigned long long output_pts_or_dts=0;
 unsigned char *ptr=(unsigned char*)&input;
 unsigned char *o_ptr=(unsigned char*)&output_pts_or_dts;
 o_ptr[0]= (ptr[4]>>1) | ((ptr[3]&0x01)<<7);
 o_ptr[1]=(ptr[3]>>1) | ((ptr[2] & 0x02)<<6);
 o_ptr[2]=(ptr[2]>>2) | ((ptr[1] & 0x03)<<6);
 o_ptr[3]=(ptr[1]>>2) | ((ptr[0] & 0x06)<<5);
 o_ptr[4]=(((0x08 & ptr[0]))>>3) & 01;
 o_ptr[5]=0x00;
 o_ptr[6]=0x00;
 o_ptr[7]=0x00; 
 return output_pts_or_dts;
 }

 typedef struct pid_queue
 {     
     char cur_cc;
     char pre_cc;
 }PID_QUEUE;
 typedef struct pat_payload
 {
    unsigned short prog_no;
    unsigned short pid_no;
    unsigned short bit;
 }PAT_PAYLOAD;
 typedef struct data_pid
 {
	unsigned short pid;
        char entry_bit;
	struct data_pid *next;
 }DATA_PID;
 typedef struct vedio_pid
 {
	unsigned short pid;
        char entry_bit;
	struct vedio_pid *next;
 }VIDEO_PID;
 typedef struct audio_pid
 {
	unsigned short pid;
        char entry_bit;
	struct audio_pid *next;
 }AUDIO_PID;
AUDIO_PID *A_HEAD=0;
AUDIO_PID *A_TAIL=0;
VIDEO_PID *V_HEAD=0;
VIDEO_PID *V_TAIL=0;
DATA_PID *D_HEAD=0;
DATA_PID *D_TAIL=0;
 int data_pid(unsigned short pid)
 {
	DATA_PID *temp = 0, *new = malloc(sizeof(A_HEAD));
	new->pid = pid;
	new->next= 0; 
        new->entry_bit=0;
	if((D_TAIL||D_HEAD)==0)
	{
		D_TAIL = new;
		D_HEAD = new;
	}
	else
	{
		temp = D_HEAD;
		while(temp)
		{
			if(temp->pid == pid)
			{
				//exit(0);
				return 0;
			}
			temp = temp->next;
		} 
		D_TAIL -> next = new;
		D_TAIL = new;
	}
 }
 int audio_pid(unsigned short pid)
 {
	AUDIO_PID *temp=0,*new = malloc(sizeof(A_HEAD));
	new->pid = pid;
	new->next= 0;
        new->entry_bit=0;
	if((A_TAIL||A_HEAD)==0)
	{
		A_TAIL = new;
		A_HEAD = new;
	}
	else
	{
		temp = A_HEAD;
		while(temp)
		{
			if(temp->pid == pid)
			{
				//exit(0);
				return 0;
			}
			temp = temp->next;
		} 
		A_TAIL -> next = new;
		A_TAIL = new;
	}
 }
 int video_pid(unsigned short pid)
 {
	VIDEO_PID *temp = 0, *new = malloc(sizeof(A_HEAD));
	new->pid = pid;
	new->next= 0; 
        new->entry_bit=0;
	if((V_TAIL||V_HEAD)==0)
	{
		V_TAIL = new;
		V_HEAD = new;
	}
	else
	{
		temp = V_HEAD;
		while(temp)
		{
			if(temp->pid == pid)
			{
				//exit(0);
				return 0;
			}
			temp = temp->next;
		} 
		V_TAIL -> next = new;
		V_TAIL = new;
	}
 }

 int add_pid(unsigned short pid)
 {
	PID_LIST *temp=0,*new = malloc(sizeof(PID_LIST));
	new->pid  = pid;
	new->next = 0;
	if((PID_LIST_TAIL==0)||(PID_LIST_HEAD==0))
	{
		PID_LIST_TAIL = new;
		PID_LIST_HEAD = new;
	}
	else
	{
		PID_LIST_TAIL -> next = new;
		PID_LIST_TAIL = new;
	}
 }
 int print_pid_des(void)
 {
	PID_LIST *temp=PID_LIST_HEAD;
        char buf[300];
       // printf("entered to print_pid_des temp=%d\n",temp);
        //printf("%-10s%s","PID","BIT_RATE\n");
        sprintf(buf,"%-10s%s","PID","BIT_RATE\n");
        write(fdd,buf,strlen(buf));
        if(report)
        printf("%s",buf);
        sprintf(buf,"%-10s%lu\n","total",(unsigned long long)((T_Brate*8*1316)/(time_avrg*1000)));
        write(fdd,buf,strlen(buf));
        if(report)
        printf("%s",buf);
        while(temp)
        {
             sprintf(buf,"%-10hu%llu\n",temp->pid,temp->bit_rate=(unsigned long long)((8*188*pidd_buf[temp->pid].total_bit_rate)/(time_avrg*1000)));
             write(fdd,buf,strlen(buf));
             if(report)
             printf("%s",buf);
             temp=temp->next;
        }
        sprintf(buf,"\n");
        write(fdd,buf,strlen(buf));
        if(report)
        printf("%s",buf);
 }
  int Store_pid_des(void)
 {
	PID_LIST *temp=PID_LIST_HEAD;
        char buf[300]={0};
       // printf("entered to print_pid_des temp=%d\n",temp);
        sprintf(buf,"%s %s","PID","BIT_RATE\n");
        write(fdd,buf,strlen(buf));
        sprintf(buf,"%s %llu\n","total",(unsigned long long)((T_Brate*8*1316)/(time_avrg*1000)));
        write(fdd,buf,strlen(buf));
        while(temp)
        {
             sprintf(buf,"%hu %llu\n",temp->pid,temp->bit_rate=(unsigned long long)((8*188*pidd_buf[temp->pid].total_bit_rate)/(time_avrg*1000)));
             write(fdd,buf,strlen(buf));
             temp=temp->next;
        }
        //printf("\n");
 }
 
 int refresh_all(void)
 {
        PID_LIST *temp = PID_LIST_HEAD;
        while(temp)
        {
                pidd_buf[temp->pid].total_bit_rate = 0;
                pidd_buf[temp->pid].dis_cont = 0;
                temp=temp->next;
        }
 }
 ////////////////
 void *coloum_print_thread(void *arg)
 { 
        sleep(time_avrg);
        pthread_mutex_lock(&mutex_var);
	print_pid_des();
        //Store_pid_des();
        thread_exit=1;
        pthread_mutex_unlock(&mutex_var);
        
 }
 
 void *thread(void *arg)
 { 
        PID_LIST *temp;
        struct tm current_time;
        time_t t;
        char dirbuf[100]       = {0};
        char sudirbuf[50]      = {0};
        char filebuf[50]       = {0};
        char buf[400]          = {0};
        unsigned int sec_count=0,min_count=0,hr_count=0,day_count=0,month_count=0;
	char sec,prev_sec,min,prev_min,hr;
	sec=prev_sec=min=prev_min=hr=0;
	//unsigned int time_avrg= *((unsigned int *)arg);
	unsigned int timer=1;
        time(&t);
        localtime_r(&t,&current_time);
        strftime(dirbuf, MAX_SIZ, "%m", &current_time);
        month_count  = atoi(dirbuf);
        strftime(dirbuf, MAX_SIZ, "%d", &current_time);
        day_count  = atoi(dirbuf);
        strftime(dirbuf, MAX_SIZ, "%H", &current_time);
        hr_count  = atoi(dirbuf);
        strftime(dirbuf, MAX_SIZ, "%M", &current_time);
        min_count = atoi(dirbuf);
        strftime(dirbuf, MAX_SIZ, "%S", &current_time);
        sec_count = atoi(dirbuf);
        strftime(dirbuf, MAX_SIZ, "./log_%m_%y", &current_time);
        strftime(filebuf, MAX_SIZ, "/log_%H:%M", &current_time);
        strftime(sudirbuf, MAX_SIZ, "/log_%d_%m_%y", &current_time);
        //auto_subfile(dirbuf,sudirbuf,filebuf,0);
	printf("TIMER_THREAD_ENTERED\n");	
	while(1)
	{
           sleep(1);
           if(counter_start_flag)
           {
		timer++;
                sec_count++;
		if(sec_count == 60)
                {
                    sec_count = 0;
                    min_count ++;
                    if(min_count == 60)
                    {
                        min_count = 0;
                        hr_count ++;
                        time(&t);
                        localtime_r(&t,&current_time);
                        /*FILE CREATION ON COMPLETION OF ONE HOUR*/
                        strftime(filebuf, MAX_SIZ, "/log_%H:%M", &current_time);
                        if(hr_count==24)
                        {
                                /*DIRECTORY CREATION ON COMPLETION OF 24 HOUR*/
                                strftime(sudirbuf, MAX_SIZ, "/log_%d_%m_%y", &current_time);
                        }
                    }                   
                }
		if(timer>time_avrg)
		{       strftime(dirbuf, MAX_SIZ, "./log_%m_%y", &current_time);
                        temp=PID_LIST_HEAD;
                        //pthread_mutex_lock(&mutex_var);
			timer = 1;
                        if((!print_flag)&&(!t_prog))
                        {
                            print_flag=1;
                            printf("T_brate ",
                            (unsigned long long)((T_Brate*8*1316)/(time_avrg*1000)));
                            while(temp)
                            {
                                 printf("|DCC ,%5u",temp->pid);
                                 temp=temp->next;
                            }
                                printf("\n");
                                T_Brate =0;
                                //pthread_mutex_unlock(&mutex_var);
                            
                        }
                        temp=PID_LIST_HEAD;
                        if((!print_opt)&&(!t_prog))
                        {
                                printf("%8llu",
                                (unsigned long long)((T_Brate*8*1316)/(time_avrg*1000)));
                                while(temp)
                                {
                                        printf("|%4u,%5llu",pidd_buf[temp->pid].dis_cont,
                                        (unsigned long long)(8*188*pidd_buf[temp->pid].total_bit_rate)/(time_avrg*1000));
                                        temp=temp->next;
                                }
                                printf("\n");
                                refresh_all();
                                T_Brate =0;
                                //pthread_mutex_unlock(&mutex_var);
                        }
                        else if(!t_prog)
                        {
                                sprintf(buf,"T_brate:%llu Kbps::",(unsigned long long)((T_Brate*8*1316)/(time_avrg*1000)));
                                while(temp)
                                {
                                        sprintf(buf+strlen(buf), "<PID:%u,DCC:%u,BRATE:%lluKbps>",temp->pid,pidd_buf[temp->pid].dis_cont,
                                        (unsigned long long)(8*188*pidd_buf[temp->pid].total_bit_rate)/(time_avrg*1000));
                                        temp=temp->next;
                                }
                                strcat(buf,"\n");
                                refresh_all();
                                T_Brate =0;
                                //pthread_mutex_unlock(&mutex_var);
                                //auto_subfile(dirbuf,sudirbuf,filebuf,buf);
                        }
		}
           }          
	}		
      		
 }
 typedef struct psi_pid_list
 {
     unsigned short psi_pid;
     struct psi_pid_list *next; 
 }PSI_PID_LIST;
 PSI_PID_LIST *PSI_LST_HD=0;
 PSI_PID_LIST *PSI_LST_TL=0;
 int add_psi_pid(unsigned short pid)
 {
	PSI_PID_LIST *temp=0,*new = malloc(sizeof(PSI_PID_LIST));
	new->psi_pid  = pid;
	new->next = 0;
	if((PSI_LST_TL==0)||(PSI_LST_HD==0))
	{
		PSI_LST_TL = new;
		PSI_LST_HD = new;
	}
	else
	{
		PSI_LST_TL -> next = new;
		PSI_LST_TL = new;
	}
 }
 
 void *difformat_thread(void *arg)
 { 
        //PID_LIST *temp;
        TS_INFO *temp=TS_INFO_HEAD;
        PMT_DATA *temp_dat=0;
        PMT_AUD *temp_aud=0;
        PSI_PID_LIST *temp_psi = PSI_LST_HD;
        struct tm current_time;
        time_t t;
        char sudirbuf[50]      = {0};
        char filebuf[50]       = {0};
        char buf[400]          = {0};
        unsigned int sec_count=0,min_count=0,hr_count=0,day_count=0,month_count=0;
	char sec,prev_sec,min,prev_min,hr;
	sec=prev_sec=min=prev_min=hr=0;
	//unsigned int time_avrg= *((unsigned int *)arg);
	unsigned int timer=1;
        time(&t);
        localtime_r(&t,&current_time);
        strftime(dirbuf, MAX_SIZ, "%m", &current_time);
        month_count  = atoi(dirbuf);
        strftime(dirbuf, MAX_SIZ, "%d", &current_time);
        day_count  = atoi(dirbuf);
        strftime(dirbuf, MAX_SIZ, "%H", &current_time);
        hr_count  = atoi(dirbuf);
        strftime(dirbuf, MAX_SIZ, "%M", &current_time);
        min_count = atoi(dirbuf);
        strftime(dirbuf, MAX_SIZ, "%S", &current_time);
        sec_count = atoi(dirbuf);
        strftime(dirbuf, MAX_SIZ, "./log_%m_%y", &current_time);
        strftime(filebuf, MAX_SIZ, "/log_%H:%M", &current_time);
        strftime(sudirbuf, MAX_SIZ, "/log_%d_%m_%y", &current_time);
        auto_subfile(dirbuf,sudirbuf,filebuf,0);
	printf("TIMER_THREAD_ENTERED\n");	
	while(1)
	{
           sleep(1);
           if(counter_start_flag)
           {
		timer++;// my timer counter which incremented on completion of a second
                sec_count++;// normal second counter
		if(sec_count == 60)//when seconds counter completes its 60 seconds
                {
                    sec_count = 0;// reset the seconds counter
                    min_count ++;//incremented the minute counter on completion of each 60 seconds by secon
                    if(min_count == 60)
                    {
                        min_count = 0;
                        hr_count ++;
                        time(&t);
                        localtime_r(&t,&current_time);
                        /*FILE CREATION ON COMPLETION OF ONE HOUR*/
                        strftime(filebuf, MAX_SIZ, "/log_%H:%M", &current_time);
                        if(hr_count==24)
                        {
                                /*DIRECTORY CREATION ON COMPLETION OF 24 HOUR*/
                                strftime(sudirbuf, MAX_SIZ, "/log_%d_%m_%y", &current_time);
                        }
                    }                   
                }
		if(timer>time_avrg)
		{       strftime(dirbuf, MAX_SIZ, "./log_%m_%y", &current_time);
                        temp=TS_INFO_HEAD;
                        temp_psi = PSI_LST_HD;
                        pthread_mutex_lock(&mutex_var);
			timer = 1;
                        if((!print_flag))
                        {
                            print_flag=1;
                            printf("T_brate ");
                            while(temp_psi)
                            {
                                 printf("|DCC ,%5u",temp_psi->psi_pid);
                                 temp_psi=temp_psi->next;
                            }
                            while(temp)
                            {
                                 printf("|DCC ,%5u",temp->pmt_pid);
                                 printf("|DCC ,%5u",temp->video_pid);
                                 temp_dat=temp->data_head;
                                 temp_aud=temp->aud_head;
                                 while(temp_dat)
                                 {
                                      printf("|DCC ,%5u",temp_dat->data_pid);
                                      temp_dat=temp_dat->next_data_pid;
                                 }  
                                 while(temp_aud)
                                 {
                                      printf("|DCC ,%5u",temp_aud->aud_pid);
                                      temp_aud=temp_aud->next_aud_pid;
                                 }
                                 temp=temp->ts_info_next;
                            }
                            
                                printf("\n");
                                //T_Brate =0;
                                //pthread_mutex_unlock(&mutex_var);
                            
                        }
                        temp=TS_INFO_HEAD;
                        temp_psi = PSI_LST_HD;
                        {
                                printf("%8llu",
                                (unsigned long long)((T_Brate*8*1316)/(time_avrg*1000)));
                                while(temp_psi)
                                {
                                        printf("|%4u,%5llu",pidd_buf[temp_psi->psi_pid].dis_cont,
                                        (unsigned long long)(8*188*pidd_buf[temp_psi->psi_pid].total_bit_rate)/(time_avrg*1000));
                                           temp_psi=temp_psi->next;
                                }
                                while(temp)
                                {
                                        printf("|%4u,%5llu",pidd_buf[temp->pmt_pid].dis_cont,
                                        (unsigned long long)(8*188*pidd_buf[temp->pmt_pid].total_bit_rate)/(time_avrg*1000));
                                        printf("|%4u,%5llu",pidd_buf[temp->video_pid].dis_cont,
                                        (unsigned long long)(8*188*pidd_buf[temp->video_pid].total_bit_rate)/(time_avrg*1000));
                                        temp_dat=temp->data_head;
                                        temp_aud=temp->aud_head;
                                        while(temp_dat)
                                        {
                                           printf("|%4u,%5llu",pidd_buf[temp_dat->data_pid].dis_cont,
                                        (unsigned long long)(8*188*pidd_buf[temp_dat->data_pid].total_bit_rate)/(time_avrg*1000));
                                           temp_dat=temp_dat->next_data_pid;
                                        }  
                                        while(temp_aud)
                                        {
                                           printf("|%4u,%5llu",pidd_buf[temp_aud->aud_pid].dis_cont,
                                        (unsigned long long)(8*188*pidd_buf[temp_aud->aud_pid].total_bit_rate)/(time_avrg*1000));
                                           temp_aud=temp_aud->next_aud_pid;
                                        }
                                        
                                        temp=temp->ts_info_next;
                                }
                                printf("\n");
                                refresh_all();
                                T_Brate =0;
                                //
                        }
                        pthread_mutex_unlock(&mutex_var);
                        /*
                        else if(!t_prog)
                        {
                                sprintf(buf,"T_brate:%llu Kbps::",(unsigned long long)((T_Brate*8*1316)/(time_avrg*1000)));
                                while(temp)
                                {
                                        sprintf(buf+strlen(buf), "<PID:%u,DCC:%u,BRATE:%lluKbps>",temp->pid,pidd_buf[temp->pid].dis_cont,
                                        (unsigned long long)(8*188*pidd_buf[temp->pid].total_bit_rate)/(time_avrg*1000));
                                        temp=temp->next;
                                }
                                strcat(buf,"\n");
                                refresh_all();
                                T_Brate =0;
                                //pthread_mutex_unlock(&mutex_var);
                                auto_subfile(dirbuf,sudirbuf,filebuf,buf);
                        }
                         */
                        
		}
           }          
	}		
      		
 }
 
 
 
 
 int discont_check(unsigned int pid,unsigned char cc)
 {
 	unsigned char pid_cur_cc;
	if(( pid_cur_cc=pidd_buf[pid].CC ) == 0x10)
	{
		pidd_buf[pid].CC = cc;
		printf("new entry of PID:%u\n",pid);
        }
        else
        {
         	pid_cur_cc = ((pid_cur_cc) + 1)% 0X10;
                if(pid_cur_cc == cc )
                 {
                        pidd_buf[pid].CC=pid_cur_cc;
			return 0;
                 }
                 else
                 {
                        pidd_buf[pid].CC=((tsafcc) & 0X0F);
			return 1;
                 }
        }
 }
 Display_Video_Type(unsigned char stream_typ)
{
switch(stream_typ)
{
case 0X00:
if(details_flag)
printf("reserved [ITU-T | ISO/IEC]\n");//0
break;
case 0X01:
if(details_flag)
printf("MPEG-1 [ISO/IEC 11172-2 Video]\n");//1
break;
case 0X02:
if(details_flag)
printf("MPEG-2 [ITU-T H.262|ISO/IEC13818-2 Video/ISO/IEC 11172-2 constrained parameter video stream]\n");//2
break;
case 0X03:
if(details_flag)
printf("MPEG-1 audio[ISO/IEC 11172-3 Audio]\n");//3
break;
case 0X04:
if(details_flag)
printf("MPEG-2 Part 3 [ISO/IEC 13818-3 Audio]\n");//4
break;
case 0X05:
if(details_flag)
printf("MPEG-2 [ITU-T H.222.0 | ISO/IEC 13818-1 private_sections]\n");//5
break;
case 0X06:
if(details_flag)
printf("MPEG-2 PES packets containing private data\n");//6
break;
case 0X07:
if(details_flag)
printf("ISO/IEC 13522 MHEG\n");//7
break;
case 0X08:
if(details_flag)
printf("MPEG-2 DSM CC\n");//8
break;
case 0X09:
if(details_flag)
printf("ITU-T H.222.1\n");//9
break;
case 0X0A:
if(details_flag)
printf("MPEG-2 DSM-CC Multi-protocol Encapsulation[ISO/IEC 13818-6 type A]\n");//10
break;
case 0X0B:
if(details_flag)
printf("MPEG-2 DSM-CC U-N Messages[ISO/IEC 13818-6 type B]\n");//11
break;
case 0X0C:
if(details_flag)
printf("MPEG-2 DSM-CC Stream Descriptors[ISO/IEC 13818-6 type C]\n");//12
break;
case 0X0D:
if(details_flag)
printf("MPEG-2 DSM-CC Sections[ISO/IEC 13818-6 type D]\n");//13
break;
case 0X0E:
if(details_flag)
printf("MPEG-2 ISO/IEC 13818-1 auxiliary[1]\n");//14
break;
case 0X0F:
if(details_flag)
printf("MPEG-2 Part 7 audio[ISO/IEC 13818-7 Audio with ADTS transport syntax]\n");//15
break;
case 0X10:
if(details_flag)
printf("MPEG-4 Part 2 visual[ISO/IEC 14496-2 Video]\n");//16
break;
case 0X11:
if(details_flag)
printf("MPEG-4 audio[ISO/IEC 14496-3 Audio]\n");//17
break;
case 0X12:
if(details_flag)
printf("MPEG-4 SL-packetized stream or FlexMux stream carried in PES packets[ISO/IEC 14496 SL-packetized stream/FlexMux stream carried in PES]\n");//18
break;
case 0X13:
if(details_flag)
printf("MPEG-4 SL-packetized stream or FlexMux stream carried in MPEG-4 sections[ISO/IEC 14496 SL-packetized stream or FlexMux stream carried in ISO/IEC 13818-1 sections]\n");//19
break;
case 0X14:
if(details_flag)
printf("MPEG-2 Synchronized Download Protocol[ISO/IEC 13818-6 Synchronized Download Protocol]\n");//20
break;
case 0X15:
if(details_flag)
printf("Metadata carried in PES packets\n");//21
break;
case 0X16:
if(details_flag)
printf("Metadata carried in metadata_sections\n");//22
break;
case 0X17:
if(details_flag)
printf("Metadata carried in MPEG-2 Data Carousel\n");//23
break;
case 0X18:
if(details_flag)
printf("Metadata carried in MPEG-2 Object Carousel\n");//24
break;
case 0X19:
if(details_flag)
printf("Metadata carried in MPEG-2 Synchronized Download Protocol\n");//25
break;
case 0X1A:
if(details_flag)
printf("IPMP stream\n");//26
break;
case 0X1B:
if(details_flag)
printf("MPEG-4 Part 10 video\n");//27
break;
/**************User Private Details*****************/
case 0X81:
if(details_flag)
printf("Dolby Digital\n");
break;
case 0X82:
if(details_flag)
printf("OpenCableTM subtitle\n");
break;
case 0X83:
if(details_flag)
printf("OpenCableTM isochronous data\n");
break;
case 0X85:
if(details_flag)
printf("ATSC program identifier\n");
break;
case 0X87:
if(details_flag)
printf("Dolby Digital Plus\n");
break;
case 0X88:
 if(details_flag)
printf("SMPE VC-9 video\n");
break;
case 0X91:
if(details_flag)
printf("DTS audio\n");
break;
case 0X95:
 if(details_flag)
printf("ATSC Data Ser vice Table and Network Resources Table\n");
break;
case 0XC2:
if(details_flag)
printf("ATSC synchronous data stream\n");
break;
case 0XC3:
if(details_flag)
printf("OpenCableTM asynchronous data\n");
break;
};

 }                                
                     
 ES_Header_Decod(unsigned int header,char *start)
 {
     
     printf("VIDEO FORMAT:");
     switch(header)
     {
         case 0x000001B3://Video Sequence Header
         printf("mpeg2video\n");
         break;
         case 0x00000100://Picture Header
         printf("mpeg2video\n");
         break;
         case 0x00000001://H.264 header
         printf("h264\n");
         break;
         case 0x000001B5://extension header
         printf("mpeg2video\n");
         break;
         case 0x000001B8://Group of Picture(GOP) Extension 
         printf("mpeg2video\n");
         break;
     };
 }
 void *Print_Thread_ts_InFooO(void *arg)
 {
     TS_INFO *temp=TS_INFO_HEAD;
     PMT_AUD  *temp_aud;
     PMT_DATA *temp_dat;
     while(1)
     {
     temp=TS_INFO_HEAD;
     while(getchar()!='P');
     {
     printf("SRV_NO  PROVIDER_NAME       SERVICE_NAME             PMT_PID    VEDIO_PID   DATA_PID  AUDIO_PID\n");
     while(temp)
     {
     printf("%-8hu%-20s%-25s%-11hu%-12hu",temp->prog_no,temp->src_provider,temp->srv_name,temp->pmt_pid,temp->video_pid);
         temp_dat=temp->data_head;
         if(!temp_dat)
         printf("%-9s ","__");
         while(temp_dat)
         {
             printf("%-9hu ",temp_dat->data_pid);
             temp_dat = temp_dat -> next_data_pid;
         }
         temp_aud=temp->aud_head;
         if(!temp_aud)
         printf("%hu ",temp_aud);
         while(temp_aud)
         {
             printf("%hu ",temp_aud->aud_pid);
             temp_aud=temp_aud->next_aud_pid;
         }
         temp=temp->ts_info_next;
         printf("\n");
     }
     }
     }
     
 }
 extern int fifo_fd;
 int sd,leng;
 main(int argc, char *argv[])
 {
  bzero(&ip_class, sizeof(ip_class));   
  command_input(argc, argv);
  sd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
  leng=sizeof(struct sockaddr_in);
  int reuse = 1;
 if(!SELECT.unicast)
if(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0)
{
    perror("Setting SO_REUSEADDR error");
    close(sd);
    exit(1);
}

 if(bind(sd,(struct sockaddr*)&bind_add,leng)<0)
 {
        perror("bind");
        return 1;
 }
 else
 {
        printf("SUCCESSFULLY BINDED AT ADDRESS:%s\nPORT:%u\n",inet_ntoa(bind_add.sin_addr),ntohs(bind_add.sin_port));
 }
 if(!SELECT.unicast)
 if(setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&ip_class, sizeof(ip_class)) < 0)
 {
        perror("setsockopt");
        exit(0);
 }
 else
 {
        printf("MILTICAST IP:%s\n",inet_ntoa(ip_class.imr_multiaddr));
 }
  if(SELECT.daemon)
  {
  	if(!fork())
  	{
      		close(1);
      		close(0);
      		close(2);
      		setsid();
		if(!file_term_flag)
      		dup2(fifo_fd,1);
      		mainprocess();
  	}
  }
  else
  mainprocess();
     
 }
 
 
int mainprocess(void)
 {
     //printf("Making it Daemon  %d\n",getpid());
     //daemon(0,0);
     //printf("After Daemon\n");


     //my_ip=print_ip("eth0");
     //printf("my_ip:%s\n",inet_ntoa(my_ip));
 unsigned char pmt_comp=1;
 unsigned char NIT_status_flag=0;
 unsigned char SDT_bit=1;
 unsigned short sdt_sl=1;
 unsigned char SDT_flag=0;
 unsigned char SDT_permit_flag=0;
 unsigned char sdt_flag=0;
 unsigned short sdt_index=0;
 unsigned short sdt_bytes=184;
 unsigned char EIT_permit_flag=1; 
 unsigned char EIT_schedule_flag;
 unsigned char EIT_present_following_flag;
 unsigned char running_status;
 unsigned char free_CA_mode;
 unsigned short descriptors_loop_length;	
 unsigned char  reserved; 
 unsigned short org_network_id;
 unsigned char dir_mcast_select = 0;
 unsigned int ip_count = 0;
 unsigned int total_ip = 0; 
 unsigned int arg_cnt  = 0;
 unsigned int w=0;
 AUDIO_PID *a_temp=0; 
 VIDEO_PID *v_temp=0;
 DATA_PID  *d_temp=0;
 unsigned char null = 0;
 unsigned char pid_cur_cc=0;
 struct sockaddr_in *dest_add=0;//pointer to address space
 unsigned char pat_bit=0;
 unsigned char pmt_bit=0;
 unsigned char found_bit=0;
 unsigned int counter=0;
 unsigned short int prog_no;
 unsigned short int pmt_count=0;
 unsigned short pid_no;
 unsigned char vncni=0;
 unsigned short prog_count=0;
 unsigned char pntr_record;
 unsigned int pid_count;
 unsigned int V_PID;
// unsigned int indx;
 unsigned int temp_fd;
 unsigned char Print_Flag=1;
 unsigned char video_type;
 b_sz=0;
/*EIT variables*/
 unsigned short int tab_id;
 unsigned short int ts_id;
 unsigned short int serv_id;
 unsigned short int org_net_id;
 unsigned char 	    seg_lsn;
 unsigned char 	    last_tab_id;
 unsigned int i;
 char once_flag = 1;
  for(i=0;i<8192;i++)
 {
	pidd_buf[i].CC=0X10;	
 }
////////////////////////////
 PAT_PAYLOAD pmt_ptr[50]={0};
 memset(pmt_ptr,0,50*sizeof(PAT_PAYLOAD));
 unsigned int pat_payload_struct_leng = sizeof(PAT_PAYLOAD);
 int ret;
 pthread_mutex_init(&mutex_var, NULL);
 
// memset(pid_buf,0X10,8192);//NL
 //auto_subfile(dirbuf,sudirbuf,filebuf,"feanisd.sw\n");   
 //pthread_create(&thread_id,0,thread,0);
 //pthread_create(&thread_id,0,difformat_thread,0);
//thread creation
//pthread_create(&thread_id,0,Print_Thread_ts_Ifno,0);
 unsigned int pack_count=0;
 fdd=open("ts_report.txt",O_CREAT|O_APPEND|O_RDWR|O_TRUNC,00666);
/////////////   
   {
   printf("Waiting For Data...\n");
   ret=recvfrom(sd,buf,1316,0,(struct sockaddr*)&sorc_add,&leng);
   //if(report==1)
   if(pthread_create(&req_print,0,Print_Thread_ts_InFO,0))
   {
       printf("Print_Thread_ts_InFO thread_creation_failed\n");
       exit(90);
   }
   if(pthread_create(&colounm_print_thread_id,0,coloum_print_thread,0))
   {
       printf("coloum_print_thread thread_creation_failed\n");
       exit(46);
   }
   T_Brate++;
   if(ret>0)
   {
    for(ts_count=0;ts_count<1316;ts_count=ts_count+188)
    {
    	if(buf[ts_count]==0X47)
    	{
   	   pid = *((unsigned short*)(buf+(ts_count + 1)));
     	   three_bit_info = (char)(pid);
     	   tsafcc=buf[(ts_count + 3)];
	   afc=(((tsafcc) & 0X30)>>4);
	   cc=((tsafcc) & 0X0F);
     	   pid = ntohs(pid);
     	   pid = 0X1FFF & pid;           
               pthread_mutex_lock(&mutex_var);
               //if(counter_start_flag)
               pidd_buf[pid].total_bit_rate++;
               if((pid_cur_cc = pidd_buf[pid].CC) == 0x10)
               {       
                   pidd_buf[pid].CC = ((tsafcc) & 0X0F);
	           add_pid(pid);
#ifdef DEBUG              
                     if(details_flag)
#endif                  
                       
                     printf("new entry of PID:%u\n",pid);
                     if((pid<=18)&&(pid>=0))
                     {
                     // printf(MAG"new entry of PID:%u\n"BLU,pid);
                     add_psi_pid(pid);
                     }
               }       
               else
               {       
		   //printf("pid:%u pid_cur_cc:%u cc:%u\n",pid,pid_cur_cc,((tsafcc) & 0X0F));
                   if(pid_cur_cc == 15)
                   {
                        pid_cur_cc = 0;
                   }
                   else
                   {
                        pid_cur_cc += 1 ;
                   } 
                   if(pid_cur_cc == ((tsafcc) & 0X0F))
                   {
                        pidd_buf[pid].CC=pid_cur_cc;
                   }
                   else
                   {
                        pidd_buf[pid].CC=((tsafcc) & 0X0F);
                        pidd_buf[pid].dis_cont++;
                        //printf("Discontinuity of PID:%u at %u\n",pid,pid_cur_cc);
                   }
                }
                pthread_mutex_unlock(&mutex_var);
           }
        }
    }
 }
   printf("Data reception intiated\n");
 //if(report==1)
 {
 while(1)
 {
   if(thread_exit)
   {
       break;
   }
   ret=recvfrom(sd,buf,1316,0,(struct sockaddr*)&sorc_add,&leng);
   T_Brate++;
   if(ret>0)
   {
       
    for(ts_count=0;ts_count<1316;ts_count=ts_count+188)
    {
    	if(buf[ts_count]==0X47)
    	{
   	   pid = *((unsigned short*)(buf+(ts_count + 1)));
     	   three_bit_info = (char)(pid);
     	   tsafcc=buf[(ts_count + 3)];
	   afc=(((tsafcc) & 0X30)>>4);
	   cc=((tsafcc) & 0X0F);
     	   pid = ntohs(pid);
     	   pid = 0X1FFF & pid;           
               pthread_mutex_lock(&mutex_var);
               //if(counter_start_flag)
               pidd_buf[pid].total_bit_rate++;
               if((pid_cur_cc = pidd_buf[pid].CC) == 0x10)
               {       
                   pidd_buf[pid].CC = ((tsafcc) & 0X0F);
	           add_pid(pid);
#ifdef DEBUG              
                     if(details_flag)
#endif                  
                       
                     printf("new entry of PID:%u\n",pid);
                     if((pid<=18)&&(pid>=0))
                     {
                     // printf(MAG"new entry of PID:%u\n"BLU,pid);
                     add_psi_pid(pid);
                     }
               }       
               else
               {       
		   //printf("pid:%u pid_cur_cc:%u cc:%u\n",pid,pid_cur_cc,((tsafcc) & 0X0F));
                   if(pid_cur_cc == 15)
                   {
                        pid_cur_cc = 0;
                   }
                   else
                   {
                        pid_cur_cc += 1 ;
                   } 
                   if(pid_cur_cc == ((tsafcc) & 0X0F))
                   {
                        pidd_buf[pid].CC=pid_cur_cc;
                   }
                   else
                   {
                        pidd_buf[pid].CC=((tsafcc) & 0X0F);
                        pidd_buf[pid].dis_cont++;
                        //printf("Discontinuity of PID:%u at %u\n",pid,pid_cur_cc);
                   }
                }
                pthread_mutex_unlock(&mutex_var);
           }
   
   
        }
    }
 }
 
 pthread_join(colounm_print_thread_id,0);
 refresh_all();
 T_Brate=0;
 }

 while(1)
 {
   ret=recvfrom(sd,buf,1316,0,(struct sockaddr*)&sorc_add,&leng);
   if(!Print_Flag)
   T_Brate++;
   if(ret>0)
   {
    for(ts_count=0;ts_count<1316;ts_count=ts_count+188)
    {
    	if(buf[ts_count]==0X47)
    	{
   	   pid = *((unsigned short*)(buf+(ts_count + 1)));
     	   three_bit_info = (char)(pid);
     	   tsafcc=buf[(ts_count + 3)];
	   afc=(((tsafcc) & 0X30)>>4);
	   cc=((tsafcc) & 0X0F);
     	   pid = ntohs(pid);
     	   pid = 0X1FFF & pid;
           /*
	   if(pid==16)
	   {
      		//printf("<PAT_PID:%u> <TEI:%u> <PUSI:%u> <TP:%u> <TSC:%u> <AFC:%u> <CC:%u>\n",pid,
		//((three_bit_info & 0X80)?1:0),((three_bit_info & 0X40)?1:0),((three_bit_info & 0X20)?1:0),(((tsafcc) & 0XC0)>>6),afc,((tsafcc) & 0X0F));
                continue;
           }
            */
           
           if(!Print_Flag)
           {
               pthread_mutex_lock(&mutex_var);
               //if(counter_start_flag)
               pidd_buf[pid].total_bit_rate++;
               if((pid_cur_cc = pidd_buf[pid].CC) == 0x10)
               {       
                   pidd_buf[pid].CC = ((tsafcc) & 0X0F);
	           add_pid(pid);
#ifdef DEBUG              
                   if(details_flag)
#endif                  
                       
                   printf("new entry of PID:%u\n",pid);
                   if((pid<=18)&&(pid>=0))
                   {
                     add_psi_pid(pid);
                   }
               }       
               else
               {       
		   //printf("pid:%u pid_cur_cc:%u cc:%u\n",pid,pid_cur_cc,((tsafcc) & 0X0F));
                   if(pid_cur_cc == 15)
                   {
                        pid_cur_cc = 0;
                   }
                   else
                   {
                        pid_cur_cc += 1 ;
                   } 
                   if(pid_cur_cc == ((tsafcc) & 0X0F))
                   {
                        pidd_buf[pid].CC=pid_cur_cc;
                   }
                   else
                   {
                        pidd_buf[pid].CC=((tsafcc) & 0X0F);
                        pidd_buf[pid].dis_cont++;
                        //printf("Discontinuity of PID:%u at %u\n",pid,pid_cur_cc);
                   }
                }
                pthread_mutex_unlock(&mutex_var);
           }
           
	{    
	if(pid == 0)
     	{
	    SDT_permit_flag=1; 
	    if(!pat_bit)
	    {
		//SDT_permit_flag=1;
#ifdef DEBUG                
                if(details_flag)
#endif                    
      		printf("<PAT_PID:%u> <TEI:%u> <PUSI:%u> <TP:%u> <TSC:%u> <AFC:%u> <CC:%u>\n",pid,
		((three_bit_info & 0X80)?1:0),((three_bit_info & 0X40)?1:0),((three_bit_info & 0X20)?1:0),
		(((tsafcc) & 0XC0)>>6),afc,((tsafcc) & 0X0F));

      		if(((((tsafcc) & 0X30)>>4)==3)||((((tsafcc) & 0X30)>>4)==2))
      		{
       		   printf("ADAPTION FIELD EXIST\n");
       		   exit(0);
      	  	}
      		else
      		{
       		   if(((three_bit_info & 0X40)?1:0) == 1)
       		   {        
        		pntr_field 	= buf[(ts_count + 4)];
        		table_id 	= buf[(ts_count + 4 + pntr_field + 1)];
        		sil 		= *((unsigned short int *)(buf+(ts_count + 5 + pntr_field + 1)));
        		sil 		= ntohs(sil);
        		sl 		= ((sil) & 0X0FFF);
        		tsi 		= *((unsigned short int *)(buf+(ts_count + 7 + pntr_field + 1)));
        		tsi 		= ntohs(tsi);
        		vncni 		= buf[(ts_count + 9 + pntr_field + 1)];
        		vn 		= (vncni & 0X3E);
        		cni 		= (vncni & 0X01);
        		sn 		= buf[(ts_count +10 + pntr_field + 1)];
        		lsn 		= buf[(ts_count +11 + pntr_field + 1)];
        		crc 		= ntohl(*((unsigned long*)(buf+(((ts_count + 5 + pntr_field + 1)) + sl - 3))));
        		no_prog=(sl-9)/4;
#ifdef DEBUG                        
                        if(details_flag)
#endif                            
                        {
                                printf("<PAT_TABLE_ID:%u> <PNTR_FIELD:%u> <SSI:%u> <SL:%u> <TSI:%u>" 
                                "<VN:%u> <CNI:%u> <SN:%u> <LSN:%u> <CRC:%lx>\n",
                                table_id,pntr_field,(((sil) & 0X1000)?1:0),sl,tsi,vn,cni,sn,lsn,crc);
                                printf("NUMB OF PROGRAMS:%u:-\n",no_prog);
                        }
        		pntr_record = pntr_field;
        		for(prog_count=0;prog_count < no_prog;prog_count++)
        		{
         		   pmt_ptr[prog_count].prog_no = ntohs(*((unsigned short int*)(buf+(ts_count+12+pntr_record+1))));
         		   pmt_ptr[prog_count].pid_no =0x1FFF & ntohs(*((unsigned short int*)(buf+(ts_count +12 + pntr_record + 1)+2)));
                           //char ADD_ts_infO(COMMAND_LIST command,unsigned short prog_no,unsigned short pmt_pid,unsigned char *srv_name, 
                  //unsigned char *src_provider,unsigned short video_pid,PMT_AUD * aud_head,PMT_DATA * data_head)
			   ADD_ts_infO(PAT,pmt_ptr[prog_count].prog_no,pmt_ptr[prog_count].pid_no,0,0,0,0,0,0);
#ifdef DEBUG                           
                           if(details_flag)
#endif                               
         		   printf("<PROG_NO:%hu> <PID_NO:%u>\n",pmt_ptr[prog_count].prog_no,pmt_ptr[prog_count].pid_no);
         		   pntr_record += 4;
                           
        		}
       		   }
      		}
		pat_bit = 1;
	   }
	}
     	else
	if((pat_bit)&&(pmt_comp))
     	{
            //if(pmt_count==12)
              //  exit(0);
            
           //printf("RUNNING no of prog:%u pmt count %u\n",no_prog,pmt_count);
     	   for(pid_count=0;pid_count < no_prog;pid_count++)
     	   {
                 //printf("pid_count:%u %u\n",pid_count,pmt_ptr[pid_count].pid_no);
      		if(pid==pmt_ptr[pid_count].pid_no)
      		{
		   if(!pmt_ptr[pid_count].bit)
		   {
                 #ifdef DEBUG  
                 if(details_flag)                 
                 #endif 
                   printf("RUNNING no of prog:%u pmt count %u\n",no_prog,pmt_count);
                        pmt_ptr[pid_count].bit=1;
			pmt_count++;
                        
                        if(pmt_count==no_prog)
                        {       //exit(0);
                                pmt_comp=0;
                        }
#ifdef DEBUG                        
                        if(details_flag)
#endif                            
       			printf("<PMT_PID:%u> <TEI:%u> <PUSI:%u> <TP:%u> <TSC:%u> <AFC:%u> <CC:%u>\n",
			pid,((three_bit_info & 0X80)?1:0),((three_bit_info & 0X40)?1:0),
			((three_bit_info & 0X20)?1:0),(((tsafcc) & 0XC0)>>6),
			(((tsafcc) & 0X30)>>4),((tsafcc) & 0X0F));
                          if(pid==16)
                          {   
                              pmt_ptr[pid_count].bit=1;
                              break;
                          }
       			if(((three_bit_info & 0X40)?1:0) == 1)
                        {
                           pntr_field = buf[(ts_count + 4)];
                           table_id   = buf[(ts_count + 5 + pntr_field)];
                           pmt_sl     = *((unsigned short int *)(buf+(ts_count + 6 + pntr_field)));
                           pmt_sl     = ntohs(pmt_sl);
                           pmt_sl     = ((pmt_sl) & 0X0FFF);
                           tsi        = ntohs(*((unsigned short int *)(buf+(ts_count + 8 + pntr_field))));
                           vncni      = buf[(ts_count + 10 + pntr_field)];
                           vn 	      = (vncni & 0X3E);
                           cni 	      = (vncni & 0X01);
                           sn 	      = buf[(ts_count +11 + pntr_field)];
                           lsn 	      = buf[(ts_count +12 + pntr_field)];
                           pcr_pid    = 0X1FFF & ntohs(*((unsigned short int *)(buf+(ts_count +13 + pntr_field))));
                           pil 	      = 0X0FFF & ntohs(*((unsigned short int *)(buf+(ts_count +15 + pntr_field))));
			   loop_index = 17;
			   es_info_len	= 0;
			   es_info_total= 0;
#ifdef DEBUG                           
                           if(details_flag)
#endif                               
                           printf("PROGRAM NO:%u\n",tsi);
			   while(1)
			   {
        			st 	= buf[ts_count + loop_index + pil + pntr_field + es_info_total];
                                epid 	= 0X1FFF & ntohs(*((unsigned short int *)(buf+(ts_count + (loop_index += 1) 
					+ pil+ pntr_field + es_info_total))));
        			es_info_len=(0X0FFF & ntohs(*((unsigned short int *)(buf+(ts_count + (loop_index += 2)+ pil 
					+ pntr_field + es_info_total)))));
				loop_index    += 2;
				if((st==1)||(st==2)||(st==27)||(st==16))
				{
#ifdef DEBUG                                    
                                   if(details_flag)
#endif                                       
				   printf("E_PID:%u\nSTREAM_TYPE:VIDEO\nDESCRIPTOR:%u\n",epid,st);
                                   Display_Video_Type(video_type=st);
				   video_pid(V_PID=epid);
				   A_V=0;
                                   t_prog++;
			        }
				else
				if((st==3)||(st==4)||(st==15)||(st==17)||(st==129))
				{	
#ifdef DEBUG                                    
                                   if(details_flag)
#endif                                       
				   printf("E_PID:%u\nSTREAM_TYPE:AUDIO\nDESCRIPTOR:%u\n",epid,st);
                                   Display_Video_Type(st);
                                   //Store_audio_inFO(unsigned short aud_pid,COMMAND_LIST command)
                                   Store_audio_inFO(epid,STR,st);
				   audio_pid(epid);
				   A_V=1;
                                   t_prog++;
				}
				else
				{	
#ifdef DEBUG                                    
                                    if(details_flag)
#endif                                        
				    printf("E_PID:%u\nSTREAM_TYPE:DATA\nDESCRIPTOR:%u\n",epid,st);
                                    Display_Video_Type(st);
                                    Store_data_inFO(epid,STR);
				    data_pid(epid);
				    A_V=2;
                                    t_prog++;
				}
				//Descriptor
				descriptor_tag=*(buf+ts_count + loop_index+ pil + pntr_field + es_info_total); 		
                                if(es_info_len)
				{
#ifdef DEBUG                                    
                                    if(details_flag)
#endif                                        
                                     printf("Descriptor Tag:%02x\n",*(buf+ts_count + loop_index+ pil + pntr_field + es_info_total));
                                     TS_descriptor_decoder(descriptor_tag);	
				}
				//printf("\n"BLU);
        			es_info_total 	+= es_info_len;
                                if((loop_index + pil +pntr_field + es_info_total) == (4 + pntr_field + pmt_sl))
				{
				   break;
				}
			   }
//char ADD_ts_infO(COMMAND_LIST command,unsigned short prog_no,unsigned short pmt_pid,unsigned char *srv_name, 
//unsigned char *src_provider,unsigned short video_pid,PMT_AUD * aud_head,PMT_DATA * data_head)
                           ADD_ts_infO(PMT,tsi,pid,0,0,V_PID,Store_audio_inFO(0,RET,0),Store_data_inFO(0,RET),video_type);
                           Store_audio_inFO(0,RST,0);
/*1*/                      Store_data_inFO(0,RST);
       		   }
                   break;
		}
      	   }
     	}
    }
#ifdef DEBUG        
   if(details_flag)
#endif       
   if((pid == 0x12)&& EIT_permit_flag)
   {
	    	EIT_permit_flag = 0;
      		printf("<EIT_PID:%u> <TEI:%u> <PUSI:%u> <TP:%u> <TSC:%u> <AFC:%u> <CC:%u>\n",pid,
		((three_bit_info & 0X80)?1:0),((three_bit_info & 0X40)?1:0),((three_bit_info & 0X20)?1:0),
		(((tsafcc) & 0XC0)>>6),afc,((tsafcc) & 0X0F));
      		if(((((tsafcc) & 0X30)>>4)==3)||((((tsafcc) & 0X30)>>4)==2))
      		{
       		   printf("ADAPTION FIELD EXIST\n");
       		   exit(0);
      	  	}
      		else
      		{
       		   if(((three_bit_info & 0X40)?1:0) == 1)
       		   { 
        		pntr_field 	= buf[(ts_count + 4)];
        		tab_id 		= buf[(ts_count + 5 + pntr_field )];
        		sil 		= *((unsigned short int *)(buf+(ts_count + 6 + pntr_field )));
        		sil 		= ntohs(sil);
        		sl 		= ((sil) & 0X0FFF);
        		serv_id		= ntohs(*((unsigned short int *)(buf+(ts_count + 8 + pntr_field ))));
        		vncni 		= buf[(ts_count + 9 + pntr_field + 1)];
        		vn 		= (vncni & 0X3E);
        		cni 		= (vncni & 0X01);
        		sn 		= buf[(ts_count +10 + pntr_field + 1)];
        		lsn 		= buf[(ts_count +11 + pntr_field + 1)];
			ts_id		=ntohs(*((unsigned short int *)(buf+(ts_count + 12  + pntr_field + 1))));
			org_net_id	=ntohs(*((unsigned short int *)(buf+(ts_count + 14  + pntr_field + 1))));
			seg_lsn		=buf[(ts_count +16 + pntr_field + 1)];	
			last_tab_id	=buf[(ts_count +17 + pntr_field + 1)];
        		crc 		= ntohl(*((unsigned long*)(buf+(((ts_count + 5 + pntr_field + 1)) + sl - 3))));
        		printf("<EIT_TABLE_ID:%02X> <PNTR_FIELD:%u> <SSI:%u> <SL:%u> <SERV_ID:%hu> <TSI:%u>" 
			"<VN:%u> <CNI:%u> <SN:%u> <LSN:%u> <TS_ID:%04X> <ORG_NET_ID:%04X> <SEG_LSN:%02X> <LAST_TAB_ID:%02X> <CRC:%lx>\n",
			tab_id,pntr_field,(((sil) & 0X1000)?1:0),sl,serv_id,tsi,vn,cni,sn,lsn,ts_id,org_net_id,seg_lsn,last_tab_id,crc);
			EIT_Event_status(tab_id);
		   }
		}
   }
 
    if((pid == 0X11)&&(SDT_permit_flag))
    {
        //printf(CYN"<SDT_PID:%u> <TEI:%u> <PUSI:%u> <TP:%u> <TSC:%u> <AFC:%u> <CC:%u>\n"BLU,pid,
        //((three_bit_info & 0X80)?1:0),((three_bit_info & 0X40)?1:0),((three_bit_info & 0X20)?1:0),
	//(((tsafcc) & 0XC0)>>6),afc,((tsafcc) & 0X0F));
        if((three_bit_info & 0X40)?1:0)//Indicating payload Start Indicator
        {
            pntr_field =0;
            if(((three_bit_info & 0X40)?1:0) == 1)
            {
                pntr_field 	= buf[(ts_count + 4)];
                //printf("pointer field exist\n");
                //it(0);
            }
            table_id            = buf[(ts_count + 4 + pntr_field + 1)];     
            //printf("table_id:0x%02x\n",table_id);
            if(table_id!=0X42)
            continue;
            sil 		= *((unsigned short int *)(buf+(ts_count + 5 + pntr_field + 1)));
            sil 		= ntohs(sil);
            //sdt_sl= sl 	= ((sil) & 0X0FFF);
            tsi 		= *((unsigned short int *)(buf+(ts_count + 7 + pntr_field + 1)));
            tsi 		= ntohs(tsi);
            vncni 		= buf[(ts_count + 9 + pntr_field + 1)];
            vn 		= (vncni & 0X3E);
            cni 		= (vncni & 0X01);
            sn 		= buf[(ts_count +10 + pntr_field + 1)];
            lsn 		= buf[(ts_count +11 + pntr_field + 1)];
            if(SDT_section_no > lsn)
            SDT_permit_flag=0;
            if(SDT_section_no  == sn)//first iteration SDT_section_no default initialization to Zero 
            {   
                //printf(RED"SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSECTION NO:%u\n"BLU,sn);
                SDT_bit =1;
                SDT_flag = 0;
                SDT_section_no++;
                sdt_flag=0;
                sdt_sl=1;
                hh=0;
                b_sz=0;
                //sdt_sl= sl 	= ((sil) & 0X0FFF);
            }
            else
            {
                continue;
            }
        }
    if((SDT_bit))
    {    
#ifdef DEBUG        
                //if(details_flag)
#endif                    
      		//printf(CYN"<SDT_PID:%u> <TEI:%u> <PUSI:%u> <TP:%u> <TSC:%u> <AFC:%u> <CC:%u>\n"BLU,pid,
		//((three_bit_info & 0X80)?1:0),((three_bit_info & 0X40)?1:0),((three_bit_info & 0X20)?1:0),
		//(((tsafcc) & 0XC0)>>6),afc,((tsafcc) & 0X0F));
      		if(((((tsafcc) & 0X30)>>4)==3)||((((tsafcc) & 0X30)>>4)==2))
      		{
       		     printf("ADAPTION FIELD EXIST\n");
       		     exit(0);
      	  	}
      		else if(!SDT_flag)
      		{
                    //printf("HAAAAAAA:%u, %u  %u\n",sdt_flag,sdt_sl,(three_bit_info & 0X40));
		   if((!sdt_flag) && sdt_sl && (three_bit_info & 0X40))
		   {
                        pntr_field =0;
                        if(((three_bit_info & 0X40)?1:0) == 1)
		   	{
        		    pntr_field 	= buf[(ts_count + 4)];
                            //printf("pointer field exist\n");
                                //it(0);
                   	}
        		sdt_sl=sl 	= ((sil) & 0X0FFF);
			org_network_id	= ntohs(*((unsigned short int *)(buf+(ts_count + 12 + pntr_field + 1))));
			reserved 	= (*((unsigned char *)(buf+(ts_count + 14 + pntr_field + 1))));
#ifdef DEBUG                        
                        if(details_flag)
#endif                            
                        {
        		    printf("<SDT_TABLE_ID:%02x> <PNTR_FIELD:%u> <SSI:%u> <SL:%u> <TSI:%u>" 
			    "<VN:%u> <CNI:%u> <SN:%u> <LSN:%u> <CRC:%lx>\n",
			    table_id,pntr_field,(((sil) & 0X1000)?1:0),sl,tsi,vn,cni,sn,lsn,crc);
			    printf("ORIGINAL N/W ID:%04x\n",org_network_id);
                        }
                        
			//printf("no of byte : %hu\n",sl);
			/*<<< >>>*/
			
		   	SDT_buf=malloc((sdt_sl/184)*184 + ((sdt_sl%184)?184:0));//creating a buffer of size of multiple of 184 bytes
                        //printf(GRN"allocated for sdt_sl=%u\n"BLU,sdt_sl);
//////////////////////////////
			if(sdt_sl>=172)
			{
			   sdt_sl=sdt_sl-180;
			   memcpy((char*)SDT_buf,buf+ts_count+16+pntr_field,172);
			   sdt_bytes = 184;
			   sdt_index = 172;
			}
			else
			{
			   sdt_bytes = sdt_sl;	
			   memcpy(SDT_buf,buf+16+ts_count+pntr_field,sdt_bytes);
			   sdt_sl=0;
			   SDT_flag=1;	
			}
			sdt_flag=1;
		   }
		   else
		   if((sdt_sl>0)&&(sdt_flag))
		   {
			memcpy(SDT_buf+sdt_index,buf+4+ts_count,sdt_bytes);
			if(sdt_sl>184)
			{
			   sdt_index    += 184;
			   sdt_sl       =  ((sdt_sl=sdt_sl-184)<0)?(sdt_bytes=sdt_sl+184):sdt_sl;
			}
			else
			{
		   //sdt_index += sl;
			   //sdt_bytes = sdt_sl;
			   sdt_sl=0;
			   SDT_flag=1;
			}
		   }
		}	
       		if(SDT_flag)
       		{
		/*loop starts here*/
                   while(hh<no_prog)
		   {
                        
		        org_service_id  = ntohs(*((unsigned short int *)(SDT_buf+b_sz)));
                        //printf("service id:%u\n",org_service_id);
			EIT_schedule_present = *((unsigned char *)(SDT_buf+  (b_sz +=2)));
			EIT_schedule_flag = (EIT_schedule_present&0x02)>>1;
			EIT_present_following_flag = (EIT_schedule_present&0x01);
			running_free_length = ntohs(*((unsigned short int *)(SDT_buf+(b_sz+=1))));
			running_status 	= (running_free_length & 0xE000)>>13;
			free_CA_mode	= (running_free_length & 0x1000)>>12;
			descriptor_loop_length = (running_free_length & 0x0FFF);
                        
                        /**/
			TS_descriptor_decoder(*((unsigned char *)(SDT_buf+(b_sz +=2))));
                        if(sl==(b_sz+12))
                        {
                            hh=0;
                            b_sz=0;
                            break;
                        }
                        //printf("b_sz=%u\n",b_sz+4+8);
			hh++;
		   }
                   
                        //Print_ts_InFO();
			SDT_bit=0;
                        
			free(SDT_buf);
		}
    }
    
    }
    a_temp=A_HEAD;//stating Audio head	
    v_temp=V_HEAD;//stating Video head
    d_temp=D_HEAD;
    if(t_prog)
    while(v_temp)
    {
	if(v_temp->pid == pid)
	{
           if(!v_temp->entry_bit)
           {
                
		if(((three_bit_info & 0X40)?1:0)==1)//CHECKING FOR PUSI BIT STATUS
		{
                   t_prog--;  
                   v_temp->entry_bit=1;
#ifdef DEBUG
                   if(details_flag)
#endif
                   {
                   printf("<VEDIO_PID:%u> <TEI:%u> "
                   "<PUSI:%u> <TP:%u> "
                   "<TSC:%u> <AFC:%u> "
                   "<CC:%u>\n",
                   pid,((three_bit_info & 0X80)?1:0),
                   ((three_bit_info & 0X40)?1:0),
                   ((three_bit_info & 0X20)?1:0),
                   (((tsafcc) & 0XC0)>>6),(((tsafcc) & 0X30)>>4),
                   ((tsafcc) & 0X0F));
		   printf("PES packet Exist\n");
                   
 		   PES_VideoDecoder();
                   }
         	}
           }
	   break;
	}
	v_temp = v_temp->next;
    }
    
    if(t_prog)
    while(a_temp)
    {
	if(a_temp->pid == pid)
	{  
           if(!a_temp->entry_bit)
           {
                if(((three_bit_info & 0X40)?1:0)==1)//CHECKING FOR PUSI BIT STATUS
		{
                   t_prog--;
                   a_temp->entry_bit=1;
#ifdef DEBUG                   
                   if(details_flag)
#endif                       
                   {
                   printf("<AUDIO_PID:%u> <TEI:%u> "
                   "<PUSI:%u> <TP:%u> "
                   "<TSC:%u> <AFC:%u> "
                   "<CC:%u>\n",
                   pid,((three_bit_info & 0X80)?1:0),
                   ((three_bit_info & 0X40)?1:0),
                   ((three_bit_info & 0X20)?1:0),
                   (((tsafcc) & 0XC0)>>6),(((tsafcc) & 0X30)>>4),
                   ((tsafcc) & 0X0F));
		   printf("PES packet Exist\n");
 		   PES_AudioDecoder();
                   }
   
		}
           }
	   break;
	}
	a_temp = a_temp->next;
    }
    if(t_prog)
    while(d_temp)
    {
	if(d_temp->pid == pid)
	{
           if(!d_temp->entry_bit)
           {
		if(((three_bit_info & 0X40)?1:0)==1)//CHECKING FOR PUSI BIT STATUS
		{
                   t_prog--;  
                   d_temp->entry_bit=1;
#ifdef DEBUG                   
                   if(details_flag)
#endif                       
                   {
                   printf("<DATA_PID:%u> <TEI:%u> "
                   "<PUSI:%u> <TP:%u> "
                   "<TSC:%u> <AFC:%u> "
                   "<CC:%u>\n",
                   pid,((three_bit_info & 0X80)?1:0),
                   ((three_bit_info & 0X40)?1:0),
                   ((three_bit_info & 0X20)?1:0),
                   (((tsafcc) & 0XC0)>>6),(((tsafcc) & 0X30)>>4),
                   ((tsafcc) & 0X0F));
		   printf("PES packet Exist\n");
 		   //PES_DataDecoder();
                   }
            
		}
           }
	   break;
	}
	d_temp = d_temp->next;
    }
    //printf("SDT_bit:%u\n",SDT_bit);
    //printf("pmt_comp:%u\n",pmt_comp);
    //printf("start_check_flag:%u once_flag:%u\n",start_check_flag,once_flag);
      //if(!start_check_flag)
      if(((!pmt_comp)&&(!SDT_bit)&&Print_Flag&&(!t_prog))&&(!start_check_flag))
      {
           //printf("first_option choosen\n");
           Print_Flag = 0;
           counter_start_flag=1;
           STore_Ts_Info();
           if(report)
           exit(0);
           thread_break=1;
           //Print_ts_InFO_type();
           pthread_create(&thread_id,0,difformat_thread,0);
      }
      else
      if((start_check_flag)&&(once_flag))
      {
          //printf("second_option choosen\n");
           once_flag=0;
           counter_start_flag=1;
           Print_Flag = 0;
           if(report)
           exit(0);
           if(pthread_create(&thread_id,0,difformat_thread,0))
           {   perror("thread");
               printf("thread creation failed\n");
           }
      }
   
  }
 }
}
}
 }
 }
 PES_AudioDecoder(void)
 {
     unsigned short audio_sync_word,sync_word;
     unsigned char  audio_id,audio_layer,no_protect;
     unsigned char  audio_byte_info,bitrate_index,smpl_freq,padding,private;
     unsigned char  mode_copy_info,audio_mode,aud_mode_ext,copyright,org_copy,emph;
unsigned short bitrate_index_table[16][3]=
                            {
                                {0,0,0},
                                {32,32,32},/*01*/
                                {64,48,40},/*02*/
                                {96,56,48},/*03*/
                                {128,64,56},/*04*/
                                {160,80,64},/*05*/
                                {192,96,80},/*06*/
                                {224,112,96},/*07*/
                                {256,128,112},/*08*/
                                {288,160,128},/*09*/
                                {320,192,160},/*10*/
                                {352,224,192},/*11*/
                                {384,256,224},/*12*/
                                {416,320,256},/*13*/
                                {448,384,320},/*14*/
                                {0,0,0} /*15*/
                            };

    if(((((tsafcc) & 0X30)>>4)==3)||((((tsafcc) & 0X30)>>4)==2))//ADAPTION FIELD EXIST	
    {		
	printf("ADAPTION FIELD EXIST\n");
	adapt_field_length=buf[(ts_count + 4)]+1;
    }
    else
    {
	printf("ADAPTION FIELD DOES NOT EXIST\n");
	adapt_field_length=0;		
    }
    pes_start_cprefix=ntohl(*((unsigned long*)(buf+ts_count+4+adapt_field_length)))>>8;
    pes_stream_id = *((unsigned char*)(buf+ts_count+4+3+adapt_field_length));
    pes_packet_leng = ntohs(*((unsigned short*)(buf+ts_count+4+4+adapt_field_length)));
    pes_byte_info = *((unsigned char*)(buf+ts_count+4+6+adapt_field_length));
/**/
    pes_scramble_control=((pes_byte_info & 0X30)>>4);
    pes_priority=((pes_byte_info & 0X08)>>3);
    data_alignment_indicator = ((pes_byte_info & 0X04)>>2);
    pes_copyright =((pes_byte_info & 0X02)>>1);
    pes_original_or_copy =((pes_byte_info & 0X01));
/**/
    pes_seven_flags =*((unsigned char*)(buf+ts_count+4+7+adapt_field_length));
/**/
    pes_pts_dts_flags = (0xC0 & pes_seven_flags)>>6;
    escr_flag = (0x20 & pes_seven_flags)>>5;	
    es_rate_flag =(0x10 & pes_seven_flags)>>4;
    dsm_trick_mode_flag =(0x08 & pes_seven_flags)>>3;
    additional_copy_info_flag =(0x04 & pes_seven_flags)>>2;
    pes_crc_flag =(0x02 & pes_seven_flags)>>1;
    pes_ext_flag= (0x01 & pes_seven_flags);
/**/
    pes_header_data_length =*((unsigned char*)(buf+ts_count+4+8+adapt_field_length));
    sync_word = ntohs(*((unsigned short*)(buf+ts_count+4+8+adapt_field_length+1+pes_header_data_length)));
    audio_sync_word     = (0xFFF0&sync_word)>>4;
    audio_id            = (0x0008&sync_word)>>3;
    //printf("AAAAA:%01X\n",((0x0006&sync_word)>>1));
    audio_layer         = (((0x0006&sync_word)>>1)==3)?1:(((0x0006&sync_word)>>1)==2)?2:(((0x0006&sync_word)>>1)==1)?3:0;
    no_protect          = (0x0001&sync_word);
    audio_byte_info=*((unsigned char*)(buf+ts_count+4+8+adapt_field_length+1+pes_header_data_length+2));
    bitrate_index  =(audio_byte_info & 0xF0) >> 4;
    smpl_freq      =(audio_byte_info & 0x0C) >> 2;
    padding        =(audio_byte_info & 0x02) >> 1;
    private        =(audio_byte_info & 0x01);
    mode_copy_info=*((unsigned char*)(buf+ts_count+4+8+adapt_field_length+1+pes_header_data_length+3));
    audio_mode     =(mode_copy_info & 0XC0)>>6;
    aud_mode_ext   =(mode_copy_info & 0X30)>>4;
    copyright      =(mode_copy_info & 0X08)>>3;
    org_copy       =(mode_copy_info & 0X04)>>2;
    emph           =(mode_copy_info & 0X03);
    
    
    
    if(no_protect==0)
    {
        printf("Protected by CRC (16bit CRC follows header)\n");
    }
    else
    {
        printf("Not Protected\n");
    }
      
/**/		
    if((pes_pts_dts_flags==2)||(pes_pts_dts_flags==3))
    {
    	pts=0;
	//printf("sizeof:%u\n",sizeof(long long));
	memcpy(&pts,buf+ts_count+4+8+adapt_field_length+1,5);
	printf("//PTS0x%llX\n",pts);
	pts=pts_dts_decoder(pts);
	if(pes_pts_dts_flags==3)
	{
	    dts=0;
	    memcpy(&dts,buf+ts_count+4+8+adapt_field_length+1+5,5);
	    printf("//DTS0%llX\n",dts);
	    dts=pts_dts_decoder(dts);
	}
    }	
    
    printf("pointer field:%u\n",pntr_field);
    printf("pkt_start_cprefix:0x%06x\n",pes_start_cprefix);
    printf("pes_stream_id:%02x\n",pes_stream_id);
    printf("pes_packet_leng:%u\n",pes_packet_leng);
    printf("pes_byte_info:%02x\n",	pes_byte_info);
    //printf("pes_priority:%u\n",	pes_priority);
    printf("PES packet scramble status:");
    switch(pes_scramble_control)
    {	
	case 0:
	printf("not scrambled\n");
	break;
	case 1:
        printf("reserved\n");
        break;
	case 2:
	printf("scrambled with even key\n");
	break;
	case 3:
	printf("scrambled with old key\n");
	break;
    }
    switch(pes_priority)
    {
	case 0:
	printf("PES_PRIORITY:low\n");	
	break;
	case 1:
	printf("PES_PRIORITY:high\n");
	break;
    }
    printf("data_alignment_indicator:%u\n",data_alignment_indicator);
    //printf("pes_copyright:"RED"%x\n"BLACK,pes_copyright);
   switch(pes_copyright)
   {
	case 0:
	printf("PES_COPYRIGHT:copy\n");	
	break;
	case 1:
	printf("PES_COPYRIGHT:orig\n");
	break;
   }
   
   printf("pes_original_or_copy:%X\n",pes_original_or_copy);
   printf("pes_seven_flags:%02X\n",pes_seven_flags);
   printf("pts_dts_status:\n");
   switch(pes_pts_dts_flags)
   {       
	case 0:
        printf("PTS:not available\n");
        printf("DTS:not abailable\n");
        break;
        case 1:
        printf("forbidden stat\n");
        break;
        case 2:
	printf("PTS:0x%llX[%llu]\n",pts,pts);
        printf("DTS:not available\n");
        break;
        case 3:
	printf("PTS:0x%llX[%llu]\n",pts,pts);
	printf("DTS:0X%llX[%llu]\n",dts,dts);
        break;
   }
   //printf(BLU"escr_flags:"RED"%u\n"BLACK,escr_flag);
   //printf(BLU"es_rate_flag:"RED"%u\n"BLACK,es_rate_flag);
   //printf(BLU"dsm_trick_mode_flag:"RED"%u\n"BLACK,dsm_trick_mode_flag);
   //printf(BLU"additional_copy_info_flag:"RED"%u\n"BLACK,additional_copy_info_flag);
   //printf(BLU"pes_crc_flag:"RED"%u\n"BLACK,pes_crc_flag);
   //printf(BLU"pes_ext_flag:"RED"%u\n"BLACK,pes_ext_flag);
   //printf(BLU"pes_header_data_length:"RED"%u\n"BLACK,pes_header_data_length);
   //printf(BLU"SEQ HEADERE:%04X\n",ntohs(*((unsigned short*)(buf+ts_count+4+8+adapt_field_length+1+pes_header_data_length))));
   printf("audio_sync_word:%03X\n",audio_sync_word);
   //printf(BLU"audio_id:"RED"%01X\n",audio_id);
   printf("audio_layer:%01X\n",audio_layer);
   printf("no_protect:%01X\n",no_protect);
   //printf(BLU"audio_byte_info:"RED"%01X\n",audio_byte_info);
   //printf(BLU"bitrate_index:"RED"%01X\n",bitrate_index);
   //printf(BLU"smpl_freq:"RED"%01X\n",smpl_freq);
   //printf(BLU"padding:"RED"%01X\n",padding);
   //printf(BLU"private:"RED"%01X\n",private);
   //printf(BLU"mode_copy_info:"RED"%01X\n",mode_copy_info);
   printf("AUDIO_MODE");
   disp_audio_mode(audio_mode);
   printf("audio_mode:%01X\n",audio_mode);
   //printf(BLU"aud_mode_ext:"RED"%01X\n",aud_mode_ext);
   //printf(BLU"copyright:"RED"%01X\n",copyright);
   //printf(BLU"org_copy:"RED"%01X\n",org_copy);
   //printf(BLU"emph:"RED"%01X\n",emph);
   printf("bit_rate:%hu kbps\n",bitrate_index_table[bitrate_index][audio_layer-1]);
  
/*   for(ii=0;ii<188;ii++)
   {
	//printf("%02X ",(*(buf+ ts_count + ii)));
       
	if((*(buf+ ts_count + ii)==0X00)&&(*(buf+ ts_count + ii+1)==0X00)&&(*(buf+ ts_count + ii+1)==0X01)&&(*(buf+ ts_count + ii+1)==0XB3))
	{
		for(ii=0;ii<188;ii++)
   		{
			printf("%02X ",(*(buf+ ts_count + ii)));
		}
   		exit(0);  
	}
   }      


   
                
                for(ii=0;ii<188;ii++)
   		{
			printf("%02X ",(*(buf+ ts_count + ii)));
		}
   
                
   *//*
   if(pes_header_data_length>10)
   {
       exit(0);
   }
   printf("\n");
                 */
   /*
   for(ii=0;ii<188;ii++)
   {
	printf("%02X ",(*(buf+ ts_count + ii)));
   }
   printf("\n");
    */
 }
  
 PES_VideoDecoder(void)
 {
     unsigned char uu=0;
     unsigned long video_Header;
    if(((((tsafcc) & 0X30)>>4)==3)||((((tsafcc) & 0X30)>>4)==2))//ADAPTION FIELD EXIST	
    {		
	printf("ADAPTION FIELD EXIST\n");
	adapt_field_length=buf[(ts_count + 4)]+1;
    }
    else
    {
	printf("ADAPTION FIELD DOES NOT EXIST\n");
	adapt_field_length=0;		
    }
    pes_start_cprefix=ntohl(*((unsigned long*)(buf+ts_count+4+adapt_field_length)))>>8;
    pes_stream_id = *((unsigned char*)(buf+ts_count+4+3+adapt_field_length));
    pes_packet_leng = ntohs(*((unsigned short*)(buf+ts_count+4+4+adapt_field_length)));
    pes_byte_info = *((unsigned char*)(buf+ts_count+4+6+adapt_field_length));
/**/
    pes_scramble_control=((pes_byte_info & 0X30)>>4);
    pes_priority=((pes_byte_info & 0X08)>>3);
    data_alignment_indicator = ((pes_byte_info & 0X04)>>2);
    pes_copyright =((pes_byte_info & 0X02)>>1);
    pes_original_or_copy =((pes_byte_info & 0X01));
/**/
    pes_seven_flags =*((unsigned char*)(buf+ts_count+4+7+adapt_field_length));
/**/
    pes_pts_dts_flags = (0xC0 & pes_seven_flags)>>6;
    escr_flag = (0x20 & pes_seven_flags)>>5;	
    es_rate_flag =(0x10 & pes_seven_flags)>>4;
    dsm_trick_mode_flag =(0x08 & pes_seven_flags)>>3;
    additional_copy_info_flag =(0x04 & pes_seven_flags)>>2;
    pes_crc_flag =(0x02 & pes_seven_flags)>>1;
    pes_ext_flag= (0x01 & pes_seven_flags);
/**/
    pes_header_data_length =*((unsigned char*)(buf+ts_count+4+8+adapt_field_length));
/**/		
    if((pes_pts_dts_flags==2)||(pes_pts_dts_flags==3))
    {
    	pts=0;
	//printf("sizeof:%u\n",sizeof(long long));
	memcpy(&pts,buf+ts_count+4+8+adapt_field_length+1,5);
	printf("//PTS0x%llX\n",pts);
	pts=pts_dts_decoder(pts);
	if(pes_pts_dts_flags==3)
	{
	    dts=0;
	    memcpy(&dts,buf+ts_count+4+8+adapt_field_length+1+5,5);
	    printf("//DTS0%llX\n",dts);
	    dts=pts_dts_decoder(dts);
	}
    }	
    
    //printf(BLU"pointer field:"RED"%u\n"BLACK,pntr_field);
    printf("pkt_start_cprefix:0x%06x\n",pes_start_cprefix);
    printf("pes_stream_id:%02x\n",pes_stream_id);
    //printf(BLU"pes_packet_leng:"RED"%u\n"BLACK,	pes_packet_leng);
   // printf(BLU"pes_byte_info:"RED"%02x\n"BLACK,	pes_byte_info);
    //printf("pes_priority:%u\n",	pes_priority);
    printf("PES packet scramble status:");
    switch(pes_scramble_control)
    {	
	case 0:
	printf("not scrambled\n");
	break;
	case 1:
        printf("reserved\n");
        break;
	case 2:
	printf("scrambled with even key\n");
	break;
	case 3:
	printf("scrambled with old key\n");
	break;
    }
    switch(pes_priority)
    {
	case 0:
	printf("PES_PRIORITY:low\n");	
	break;
	case 1:
	printf("PES_PRIORITY:high\n");
	break;
    }
    printf("data_alignment_indicator:%u\n",data_alignment_indicator);
    //printf("pes_copyright:"RED"%x\n"BLACK,pes_copyright);
   switch(pes_copyright)
   {
	case 0:
	printf("PES_COPYRIGHT:copy\n");	
	break;
	case 1:
	printf("PES_COPYRIGHT:orig\n");
	break;
   }
   printf("pes_original_or_copy:%X\n",pes_original_or_copy);
   printf("pes_seven_flags:%02X\n",pes_seven_flags);
   printf("pts_dts_status:\n");
   switch(pes_pts_dts_flags)
   {       
	case 0:
        printf("PTS:not available\n");
        printf("DTS:not abailable\n");
        break;
        case 1:
        printf("forbidden stat\n");
        break;
        case 2:
	printf("PTS:0x%llX[%llu]\n",pts,pts);
        printf("DTS:not available\n");
        break;
        case 3:
	printf("PTS:0x%llX[%llu]\n",pts,pts);
	printf("DTS:0X%llX[%llu]\n",dts,dts);
        break;
   }
  printf("escr_flags:%u\n",escr_flag);
   printf("es_rate_flag:%u\n",es_rate_flag);
  printf("dsm_trick_mode_flag:%u\n",dsm_trick_mode_flag);
 printf("additional_copy_info_flag:%u\n",additional_copy_info_flag);
  printf("pes_crc_flag:%u\n",pes_crc_flag);
  printf("pes_ext_flag:%u\n",pes_ext_flag);
   printf("pes_header_data_length:%u\n",pes_header_data_length);
   printf("SEQ HEADERE:%08X\n",video_Header=ntohl(*((unsigned long*)(buf+ts_count+4+8+adapt_field_length+1+pes_header_data_length))));
    ES_Header_Decod(video_Header,0);
   /*
   for(ii=0;ii<188;ii++)
   {
	//printf("%02X ",(*(buf+ ts_count + ii)));
       
	if((*(buf+ ts_count + ii)==0X00)&&(*(buf+ ts_count + ii+1)==0X00)&&(*(buf+ ts_count + ii+1)==0X01)&&(*(buf+ ts_count + ii+1)==0XB3))
	{
		for(ii=0;ii<188;ii++)
   		{
			printf("%02X ",(*(buf+ ts_count + ii)));
		}
   		exit(0);  
	}
   }      


   
                
                for(ii=0;ii<188;ii++)
   		{
			printf("%02X ",(*(buf+ ts_count + ii)));
		}
   
                
   */
   /*
   if(pes_header_data_length>10)
   {
       exit(0);
   }
   printf("\n");
                 */
   
   for(uu=0;uu<188;uu++)
   {
	printf("%02X ",(*(buf+ ts_count + uu)));
   }
   printf("\n");
 }
 void disp_audio_mode(char mod)
 {
     switch(mod)
     {
        case 0:
        printf("Stereo\n"); 
        break;
        case 1:
        printf("joint stereo\n");
        break;
        case 2:
        printf("dual channel\n");
        break;
        case 3:
        printf("single channel\n");
        break;
     }
 }
 
 
 
  
			   //indx		= 0;
			   //printf("PIL:%u\n",pil);
                     		//sst = buf[ts_count + 22 + pil + pntr_field + es_info_len];
     				//epid_nxt = 0X1FFF & ntohs(*((unsigned short int *)(buf+ts_count + 23 + pil + pntr_field+es_info_len)));
        			//es_info_len_nxt = 0X0FFF & ntohs(*((unsigned short int *)(buf+ts_count + 25 + pil + pntr_field + es_info_len)));
        			//av_pid_siz = pmt_sl-13-pil-es_info_len;
        			//pmt_crc = ntohl(*((unsigned long*)(buf+ts_count + 4 + pntr_field + pmt_sl)));
/*2*/
                        	//printf("<TABLE_ID:%u> <PNTR_FIELD:%u> <SSI:%u> <PMT_SL:%u>" 
				//"<TSI:%u> <VN:%u> <CNI:%u> <SN:%u> <LSN:%u> <PCR_PID:%u>" 
				//"<PIL:%u> <PMT_CRC:%lx> <ST:%u> <EPID:%u> <ESINFO:%u>" 
				//"<SST:%u> <EPID_NXT:%u> <ESINFO_LEN:%u>\n",
				//table_id,pntr_field,(((sil) & 0X1000)?1:0),
				//pmt_sl,tsi,vn,cni,sn,lsn,pcr_pid,pil,pmt_crc,
				//st,epid,es_info_len,sst,epid_nxt,es_info_len_nxt);
 
/*
 void command_input(int argc, char *argv[])
 {
     int comm_counter;
     bind_add.sin_family=AF_INET;
     uint16_t port;
     for(comm_counter=1;comm_counter<argc;comm_counter++)
     {                
         if(!strcmp(argv[comm_counter],"-u"))
         {
             SELECT.unicast=1;
                        
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
         if(!strcmp(argv[comm_counter],"-D"))
         {
             SELECT.details=1;
             details_flag=1;
         }
         else
         if(!strcmp(argv[comm_counter],"-t"))
         {
             time_avrg = atoi(argv[comm_counter+1]);
         }
         else
         {
             time_avrg=10;
         }
         
       
     }
     for(comm_counter=1;comm_counter<argc;comm_counter++)
     {
         if(!strcmp(argv[comm_counter],"-e"))
         {
             my_ip=print_ip(argv[comm_counter+1]);
             bind_add.sin_addr=my_ip;
             break;
         }
     }
     if(comm_counter==argc)
     {
         printf("CL_Error:please select an ethernet interface:-e [device name]\n");
         Print_direction();
         exit(98);
     }
     for(comm_counter=1;comm_counter<argc;comm_counter++)
     {
         if(!strcmp(argv[comm_counter],"-p"))
         {   
             SELECT.port=1;
             port=atoi(argv[comm_counter+1]);
             if((port<1024)||(port<0)||(port>0XFFFF))
             {
                printf("CL_Error:Invalid Port:%s\n",argv[comm_counter+1]);
                Print_direction();
                exit(78);
             }
             bind_add.sin_port=htons(atoi(argv[comm_counter+1]));
             break;
         }
      }
        
      if(!SELECT.port)
      {
         printf("CL_Error:port number required\n");
         Print_direction();
         exit(79);
      }
      if(SELECT.unicast !=1 )
      {
         for(comm_counter=1;comm_counter<argc;comm_counter++)
         {                
             if(!strcmp(argv[comm_counter],"-m"))
             {
                SELECT.multicast=1;
                ip_class.imr_interface=my_ip;
                if((comm_counter+1)!=argc)
                ip_class.imr_multiaddr.s_addr=inet_addr(argv[comm_counter+1]);
                else
                {
                        printf("multi-cast IP is not given following -m\n");
                        Print_direction();
                }
             }     
                    
         }
         if(SELECT.multicast!=1)
         {
             printf("CL_error:add multicast ip following -m\n");
             Print_direction();
             exit(45);
         }
      }
 }
 */
