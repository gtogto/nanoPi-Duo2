#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>

void uart_ch(char ch);
void uart_str(char *str);

int main(void)
{
    char str[100] = {0,};
    char str1[100] = {0,};

    if(wiringPiSetup () == -1)
    {
        fprintf(stdout, "Unable to start wiringPi: %s\n", strerror (errno));
        return 1;
    }

    while(1)
    {
        scanf("%s", str1);
        getchar();		
        sprintf(str,"send data : %s\n\r", str1);
        uart_str(str);
    }

    return 0;
}

void uart_ch(char ch)
{
    int fd;

    if((fd = serialOpen ("/dev/ttyS3", 115200)) <0)
    {
        fprintf(stderr, "Unable to start wiringPi: %s\n", strerror (errno));
        return;
    }

    serialPutchar(fd, ch);
    //close(fd);
}

void uart_str(char *str)
{
    while(*str) uart_ch(*str++);
}


