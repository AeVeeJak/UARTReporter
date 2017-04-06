/* Write to UART */
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<termios.h>
#include<stdio.h>

main()
{
    struct termios tio;
    struct termios stdio;
    struct termios old_stdio;
    int tty_fd;
    int i;
    unsigned char c = (unsigned char)0xA1;
    unsigned char d = (unsigned char)0xA2;
    FILE *fp;
    char returnData[64];

    tcgetattr(STDOUT_FILENO, &old_stdio);
    memset(&stdio, 0, sizeof(stdio));
    tcgetattr(STDOUT_FILENO, &stdio);			// I added
#if 0    
    stdio.c_iflag = 0;
    stdio.c_oflag = 0;
    stdio.c_cflag = 0;
    stdio.c_lflag = 0;
    stdio.c_cc[VMIN] = 1;
    stdio.c_cc[VTIME] = 0;
#endif
    tcsetattr(STDOUT_FILENO, TCSANOW, &stdio);
    tcsetattr(STDOUT_FILENO, TCSAFLUSH, &stdio);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);		// non-blocking read

    // this is one I car about
    memset(&tio, 0, sizeof(tio));
    tio.c_iflag = 0;
    tio.c_oflag = 0;
    tio.c_cflag = CS8|CREAD|CLOCAL;
    tio.c_lflag = 0;
    tio.c_cc[VMIN] = 1;
    tio.c_cc[VTIME] = 5;
    tty_fd = open("/dev/ttyO1", O_RDWR | O_NONBLOCK);
    cfsetospeed(&tio, B115200);
    cfsetispeed(&tio, B115200);
    tcsetattr(tty_fd, TCSANOW, &tio);

    write(tty_fd, &c, 1);
    printf("All Good");
    //sleep(2);

//    for(i = 0; i < 1; i++)
    while(0)
    {
        sleep(2);
        fp = popen("/sbin/ifconfig wlan0", "r");
        //if(fp == NULL)
        if(fgets(returnData, 64, fp) == NULL)
            write(tty_fd, &d, 1);
    }
    close(tty_fd);
}
