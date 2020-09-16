#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <string.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>

#define TRUE 1
#define FALSE 0
#define PORT 8011

#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>

/*create By. gto. 2020.07*/

void uart_ch3(char ch);
void uart_str3(char *str3);

void uart_ch1(char ch);
void uart_str1(char *str1);

void append(char *dst, char c);
void insert(char *dst, char c);

char device1[] = "/dev/ttyS1";
char device2[] = "/dev/ttyS3";

unsigned long baud = 115200;

int fd;

int main(void)
{ 
	
	/*============== UDP struct ==============*/
    int sock;
    struct sockaddr_in addr, client_addr; 
    char recv_buffer[20]; 
	//char *recv_buffer = malloc(sizeof(char) * 50);
	char udp_buffer[2024];
    int recv_len; 
    int addr_len; 
   	char *exit = "bye";

	char sp[2] = "[";

	char sum_1[1024];
	char sum_2[2014];

	/*========================================*/

	/*============== UART struct ==============*/
	char str1[1024] = {0,};
	char str1_1[1024] = {0,};

	char str3[1024] = {0,};
	char str3_3[1024] = {0,};

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
			printf("Message received from UDP Client : %s\n", recv_buffer); 
			//sendto(sock, recv_buffer, strlen(recv_buffer), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
			
			//sprintf(str1, "%s", recv_buffer);

			for (int i = 0; i<sizeof(recv_buffer); i++) {
				printf("%c = 0x%02X\n", recv_buffer[i], recv_buffer[i]);
				sprintf(str1, "%02X", recv_buffer[i]);
			}

			//printf("size : %d\n", sizeof(recv_buffer) / sizeof(char));
			//scanf("%s", str1_1);
			//getchar();
			//sprintf(str1, "UDP ---------------------> UART1 : %s\n\r", recv_buffer);
			//append('[', recv_buffer);
			//insert(recv_buffer, '[');
			//strcat(sp, recv_buffer);
			//append(recv_buffer, ']');
			//append(sp, recv_buffer);
			//printf("Combined data : %c\n", recv_buffer);

			/*
			char temp[100+1];
			int i=0;

			memset(temp, 0x00, sizeof(temp));
			memcpy(temp, recv_buffer, strlen(recv_buffer));
			StrToHex(temp, strlen(temp));*/

			uart_str1(str1);			
			printf("UART1 ---------------------> UDP : ");

			/*scanf("%s", str1_1);
			printf("\n");
			sendto(sock, str1_1, strlen(str1_1), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));*/
			
			
			scanf("%s", udp_buffer);
			//getchar();
			printf("\n\r");
			sendto(sock, udp_buffer, strlen(udp_buffer), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));

			//printf("uart send length : %d\n", sizeof(str1_1));
			
			/*
			
			if (data3 == 1) {
				ret = system("./gpio_on_shell");
			}
			else ret = system("./gpio_off_shell");
			*/
			//data = serialGetchar(fd);

		}
		//free(recv_buffer);
	}	
	//free(recv_buffer);
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

void append(char *dst, char c) 
{
	char *p = dst;
	while(*p != '\0') p++;
	*p = c;
	*(p+1) = '\0';
}
void insert(char *dst, char c)
{
	char *p = dst;
	while(*p != '\0') p--;
	*p = c;
	*(p+1) = '\0';
}

/*
int StrToHex(char* pBuf, int nBufSize)
{
	/* int   nBufSize = atoi(nBufSize); */
	//if( pBuf==NULL || nBufSize<1 ){
	/*if(pBuf!=NULL) {
		printf(" HEX 로 변환할 문자열이 없습니다 \n");
		return 0;
	}*/
/*	
	printf("\n");
	printf("        00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
	printf("  =======================================================\n");
	
	const unsigned char* pData = (const unsigned char*)pBuf;
	char szDataPart[100];  // left  /
	char szViewPart[100];  // right /
	memset(szDataPart, 0, sizeof(szDataPart));
	memset(szViewPart, 0, sizeof(szViewPart));											
	unsigned char byteHigh = 0;
	for (int nPos = 0; nPos < nBufSize; nPos++) {
		unsigned char b = pData[nPos];
		char szHex[10];
		sprintf(szHex, " %02X", b);
		strcat(szDataPart, szHex);
		if (byteHigh) {
			if (strlen(szViewPart) == 0) {
				if (!(nPos % 16) == 0) {
					strcpy(szViewPart, " ");
				}
			}
			szViewPart[strlen(szViewPart)] = byteHigh;
			szViewPart[strlen(szViewPart)] = b;
			byteHigh = 0;
		} else {
			if (strlen(szViewPart) == 0) {
				szViewPart[strlen(szViewPart)] = ' ';
			}
			if (b < 0x20) {
				szViewPart[strlen(szViewPart)] = '.';
			} else if (b < 0x7F) {
				szViewPart[strlen(szViewPart)] = b;
			} else if (b < 0xA0) {
				szViewPart[strlen(szViewPart)] = '.';
			} else {
				byteHigh = b;
			}
		}
		
		if (((nPos + 1) % 16) == 0) {
			char szLine[200];
			sprintf(szLine, "  %04X:%s : %s\n", (nPos / 16) * 16, szDataPart, szViewPart);
			printf(szLine);
			memset(szDataPart, 0, sizeof(szDataPart));
			memset(szViewPart, 0, sizeof(szViewPart));
		}
	}
	
	if (0 != (nBufSize % 16)) {
		char szBlank[100];
		memset(szBlank, ' ', sizeof(szBlank));
		szBlank[3 * (16 - (nBufSize % 16))] = '\0';
		strcat(szDataPart, szBlank);
		char szLine[200];
		sprintf(szLine, "  %04X:%s : %s\n", (nBufSize / 16) * 16, szDataPart, szViewPart);
		printf(szLine);
		memset(szDataPart, 0, sizeof(szDataPart));
		memset(szViewPart, 0, sizeof(szViewPart));
	}
	
	printf("  =======================================================\n");
	printf("        00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
	printf("\n");
	return 0;
}
*/
