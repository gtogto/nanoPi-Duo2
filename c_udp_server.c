#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#define PORT 8011

#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>

/*create By. gto. 2020.07*/

void uart_ch(char ch);
void uart_str(char *str);

int main(void)
{ 
	/*============== UDP struct ==============*/
    int sock;
    struct sockaddr_in addr, client_addr; 
    char recv_buffer[1024]; 
    int recv_len; 
    int addr_len; 
   	char *exit = "bye";
	/*========================================*/

	/*============== UART struct ==============*/
	char str[100] = {0,};
	char str1[100] = {0,};
	/*=========================================*/

	int case_num;

    if((sock = socket(AF_INET, SOCK_DGRAM, 0)) <0)
    { 
        perror("socket "); return 1;
    }
    
    memset(&addr, 0x00, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);
    if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind ");
        return 1;
    } 

	if(wiringPiSetup () == -1)
	{
		fprintf(stdout, "Unable to start wirintPi: %s\n", strerror (errno));
		return 1;
	}
	/*
	while(1)
	{
		scanf("%s", str1);
		getchar();
		sprintf(str, "send message : %s\n\r", str1);
		uart_str(str);
	}*/

	    
    printf("waiting for messages, if you want program close, send to bye\n");
	
	while(1){
		/*printf("choice menu\n");
		scanf ("%d", &case_num);
		switch(case_num)
		{
			case 1: 
				printf("menu 1\n");
				break;
			case 2:
				printf("menu 2\n");
				break;
			default:
				break;
		}*/

		addr_len = sizeof(client_addr);
		if ((recv_len = recvfrom(sock, recv_buffer, 1024, 0, 
			(struct sockaddr *)&client_addr, &addr_len)) < 0)
		{ 
			perror("recvfrom "); 
			return 1;
		} 
    
	    recv_buffer[recv_len] = '\0';
    
    	printf("ip : %s\n", inet_ntoa(client_addr.sin_addr)); 
	
		int ret = strcmp(recv_buffer, exit);
		int end = 0;

		if(ret==0){			
			printf("Goodbye User! \n");
			end = 1;
			if (end==1) {
				break;			
			}			
		}
		else {
			printf("received data >>>>>>>   %s \n", recv_buffer); 
			sendto(sock, recv_buffer, strlen(recv_buffer), 0, 
			(struct sockaddr *)&client_addr, sizeof(client_addr));
			
			scanf("%s", str1);
			getchar();
			sprintf(str, "send data : %s\n\r", str1);
			uart_str(str);

		}
		/*
		scanf("%s", str1);
		getchar();
		sprintf(str, "send Data : %s\n\r", str1);
		uart_str(str);*/

	}	
	close(sock);
    return 0; 
}

void uart_ch(char ch)
{
	int fd;

	if((fd = serialOpen ("/dev/ttyS3", 115200)) < 0)
	{
		fprintf(stderr, "Unable to start wiringPi: %s\n", strerror (errno));
		return;
	}

	serialPutchar(fd, ch);
}

void uart_str(char *str)
{
	while(*str) uart_ch(*str++);
}
