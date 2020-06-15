#include "ros/ros.h"

#include <vector>

#include <sstream>

#include "stdio.h"
#include "ctype.h"
#include <unistd.h>   //para o usleep()
#include <sys/time.h>  //header file para gettimeofday e struct timeval
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
// includes for socketcan communication
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>



int main(int argc, char **argv)
{

    int s;
    struct sockaddr_can addr;
    struct can_frame frame;
    struct ifreq ifr;

    const char *ifname = "vcan0";

	if((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("Error while opening socket");
		return -1;
	}

	strcpy(ifr.ifr_name, ifname);
	ioctl(s, SIOCGIFINDEX, &ifr);

	addr.can_family  = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if(bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Error in socket bind");
		return -2;
	}

    int dirD = 15;

    frame.can_id  = 0x500;
	frame.can_dlc = 8;  // confirm
	// define data in order to control steering
	frame.data[0] = dirD; // pretended car direction in degrees
    frame.data[1] = 0x00; // 01 - right , 00 - left
    frame.data[2] = 0x00;
    frame.data[3] = 0x00;
    frame.data[4] = 0x00;
    frame.data[5] = 0x00;
    frame.data[6] = 0x00;
    frame.data[7] = 0x00;



    while(1)
    {
        write(s, &frame, sizeof(struct can_frame));
        usleep(10000);
    }
}