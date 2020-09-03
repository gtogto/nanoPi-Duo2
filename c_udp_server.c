#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#define PORT 8011

#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>

/*create By. gto. 2020.07*/

void uart_ch3(char ch);
void uart_str3(char *str3);

void uart_ch1(char ch);
void uart_str1(char *str1);

char device1[] = "/dev/ttyS1";
char device2[] = "/dev/ttyS3";

unsigned long baud = 115200;

int fd;

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
	char str1[100] = {0,};
	char str1_1[100] = {0,};

	char str3[100] = {0,};
	char str3_3[100] = {0,};

	int data1, data3;
	/*=========================================*/
	int ret;
	char gpio_num;

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
	    
    printf("waiting for messages, if you want program close, send to bye\n");
	
	while(1){
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
			printf("UDP data receive >>>>>>>   %s \n", recv_buffer); 
			sendto(sock, recv_buffer, strlen(recv_buffer), 0, 
			(struct sockaddr *)&client_addr, sizeof(client_addr));
			
			printf("UART3 data : ");
			scanf("%s", str3_3);
			getchar();
			sprintf(str3, "send data3 : %s\n\r", str3_3);
			uart_str3(str3);
			
			data3 = serialGetchar(fd);
			printf("\nUART3 -> nano = %c\n", (char)data3);
			serialPutchar(fd, data3);
			serialPuts(fd, "\n");
			fflush(stdout);
			

			printf("UART1 data : ");
			scanf("%s", str1_1);
			getchar();
			sprintf(str1, "send data1 : %s\n\r", str1_1);
			uart_str1(str1);

			data1 = serialGetchar(fd);
			printf("\nUART1 -> nano = %c\n", (char)data1);
			serialPuts(fd, "\n");
			fflush(stdout);
			char compare_num = '1';
			if (data1 == compare_num) {
				ret = system("./gpio_on_shell");
			}
			else ret = system("./gpio_off_shell");
			
			//data = serialGetchar(fd);

		}

	}	
	close(sock);
    return 0; 
}

void uart_ch1(char ch)
{
	//int fd;
	if((fd = serialOpen (device1, baud)) < 0)
	{
		fprintf(stderr, "Unable to start wiringPi: %s\n", strerror (errno));
		return;
	}
	
	serialPutchar(fd, ch);
}

void uart_str1(char *str1)
{
	while(*str1) uart_ch1(*str1++);
}

void uart_ch3(char ch)
{
	//int fd;
	if((fd = serialOpen (device2, baud)) < 0)
	{
		fprintf(stderr, "Unable to start wiringPi: %s\n", strerror (errno));
		return;
	}

	serialPutchar(fd, ch);
}

void uart_str3(char *str3)
{
	while(*str3) uart_ch3(*str3++);
}
