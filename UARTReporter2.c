/* Write to UART */
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<termios.h>
#include<stdio.h>
#include<stdbool.h>

main()
{
    struct termios tio;
    int tty_fd;
    unsigned char c = (unsigned char)0xA1;
    unsigned char d = (unsigned char)0xA2;
    FILE *fp;
    char returnData[100];
    char oldData[100] = " ";
    char stringFind[] = "packets:";
    bool flag = true;

    ///////////////////////////////////////////////
    // allocate and setup terminal structure params
    ///////////////////////////////////////////////
    memset(&tio, 0, sizeof(tio));
    tio.c_iflag = 0;
    tio.c_oflag = 0;
    tio.c_cflag = CS8|CREAD|CLOCAL;
    tio.c_lflag = 0;
    tio.c_cc[VMIN] = 1;
    tio.c_cc[VTIME] = 5;

    //////////////////////////////
    // setup uart1
    //////////////////////////////
    tty_fd = open("/dev/ttyO1", O_RDWR | O_NONBLOCK);
    cfsetospeed(&tio, B115200);
    cfsetispeed(&tio, B115200);
    tcsetattr(tty_fd, TCSANOW, &tio);

    //////////////////////////////
    // tx uart "network connected
    //////////////////////////////
    write(tty_fd, &c, 1);

    //////////////////////////////
    // terminal display
    /////////////////////////////
    printf("All Good");

    /////////////////////////////////////////
    // check on status of network connections
    /////////////////////////////////////////
    while(flag)
    {
        sleep(5);
        fp = popen("/sbin/ifconfig wlan0", "r");
        fgets(returnData, 90, fp);
        fgets(returnData, 90, fp);
        fgets(returnData, 90, fp);
        fgets(returnData, 90, fp);
        if(fgets(returnData, 90, fp) != NULL)
        {
            // Network is disconnected
            if(strcmp(returnData, oldData) == 0)
            {
                write(tty_fd, &d, 1);
                flag = false;
                printf(returnData);
                printf(oldData);
            }
            // Network is connected
            else
            {
                write(tty_fd, &c, 1);
                strcpy(oldData, returnData);
            }
        }
    }
    while(0)
    {
        sleep(2);
        fp = popen("/sbin/ifconfig wlan0", "r");

        if(fgets(returnData, 64, fp) == NULL)
            write(tty_fd, &d, 1);
    }
    close(tty_fd);
}
