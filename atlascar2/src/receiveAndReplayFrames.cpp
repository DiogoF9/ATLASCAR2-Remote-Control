#include "ros/ros.h"
#include <atlascar2/NominalData.h>
#include <can_msgs/Frame.h>
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
#include <time.h>


int main(int argc, char **argv)
{
        ros::init(argc, argv, "receiveAndReplayFrames");
        ros::NodeHandle n;

        int s;
        int nbytes_rcv;
        struct sockaddr_can addr;
        struct can_frame frame;
        struct ifreq ifr;

        /*name of the can device - vcan0 in case of virtual CAN bus*/
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

     ros::Publisher pub2 = n.advertise<can_msgs::Frame>("RawFrames", 1000);
     ros::Rate loop_rate(1000);

     // the message to be published
     can_msgs::Frame msg;


     int count = 0;
     while (ros::ok())
	{
	        nbytes_rcv = recv(s, &frame, sizeof(struct can_frame),0);

                if (nbytes_rcv < 0) {
                        perror("can raw socket read");
                        return 1;
                }
                if (nbytes_rcv < sizeof(struct can_frame)) {
                        fprintf(stderr, "read: incomplete CAN frame\n");
                        return 1;
                }
                //printf("message received: %03X  %d  %02X %02X %02X %02X %02X %02X %02X %02X\n",frame.can_id, frame.can_dlc, frame.data[0], frame.data[1], frame.data[2], frame.data[3], frame.data[4], frame.data[5], frame.data[6], frame.data[7]);    }

                msg.id = frame.can_id;
                msg.dlc = frame.can_dlc;
                msg.data[0] = frame.data[0];
                msg.data[1] = frame.data[1];
                msg.data[2] = frame.data[2];
                msg.data[3] = frame.data[3];
                msg.data[4] = frame.data[4];
                msg.data[5] = frame.data[5];
                msg.data[6] = frame.data[6];
                msg.data[7] = frame.data[7];

                pub2.publish(msg);
                ros::spinOnce();

                loop_rate.sleep();
        }
        return 0;
}
