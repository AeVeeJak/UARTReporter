/* Write to UART */

// INCLUDES
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<termios.h>
#include<stdio.h>
#include<stdbool.h>

// MACROS
#define	CONNECTED	1
#define DISCONNECTED	0
#define WLAN0		"/sbin/ifconfig wlan0"
#define ETH0		"/sbin/ifconfig eth0"

int get_network_status(char * cmd_path, int current_status)
{
    FILE * fp;
    char returnData[100];
    char * oldDataPtr;
    static char old_eth0_data[100] = " ";
    static char old_wlan0_data[100] = " ";

    /////////////////////////////////
    // Setup ptr to correct old data
    ////////////////////////////////
    if(cmd_path == WLAN0)
        oldDataPtr = old_wlan0_data;
    else if(cmd_path == ETH0)
	oldDataPtr = old_eth0_data;
 
    /////////////////////////////////
    // execute cmd on network device
    /////////////////////////////////
    fp = popen(cmd_path, "r");
    if(fp == NULL)
    {
        pclose(fp);
        return current_status;
    }

    ////////////////////////////////
    // parse fifth line of returned
    // FILE stream data.  This line
    // had the number of rcv packets
    ////////////////////////////////
    fgets(returnData, 90, fp);
    fgets(returnData, 90, fp);
    fgets(returnData, 90, fp);
    fgets(returnData, 90, fp);
    if(fgets(returnData, 90, fp) != NULL)
    {
	if(strcmp(returnData, oldDataPtr) == 0)
	{
            pclose(fp);
	    return DISCONNECTED;
	}
        else
	{
	    strcpy(oldDataPtr, returnData);
            pclose(fp);
	    return CONNECTED;
	}
    }
}

main()
{
    struct termios tio;
    int tty_fd;
    int network_status = CONNECTED;
    int wlan0_status = DISCONNECTED;
    int eth0_status = CONNECTED;
    int wlan0_status_at_startup = DISCONNECTED;
    unsigned char c = (unsigned char)0xA1;
    unsigned char d = (unsigned char)0xA2;
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

    //////////////////////////////////////
    // check wireless status at startup
    // if DISCONNECTED, ignore until reset
    //////////////////////////////////////
    wlan0_status_at_startup = get_network_status(WLAN0,wlan0_status_at_startup);
    sleep(10);
    wlan0_status_at_startup = get_network_status(WLAN0,wlan0_status_at_startup);

    /////////////////////////////////////////
    // check on status of network connections
    /////////////////////////////////////////
    while(1)
    {
        ///////////////////////////////////////
        // Get Wired and Wireless network statu
        ///////////////////////////////////////
        if(wlan0_status_at_startup == CONNECTED)
            wlan0_status = get_network_status(WLAN0, wlan0_status);
        eth0_status = get_network_status(ETH0, eth0_status);

	///////////////////////////////////////
        // If network status changed, Uart Tx
        //////////////////////////////////////
        if(network_status == CONNECTED)
	{
	    if(wlan0_status == DISCONNECTED && eth0_status == DISCONNECTED)
	    {
	        network_status = DISCONNECTED;
	        write(tty_fd, &d, 1);
	    }
	    else sleep(8);
	}
	else if(network_status == DISCONNECTED)
	{
	    if(wlan0_status == CONNECTED || eth0_status == CONNECTED)
	    {
		network_status = CONNECTED;
		write(tty_fd, &c, 1);
		sleep(3);
	    }
	}
    }   
    close(tty_fd);
}








