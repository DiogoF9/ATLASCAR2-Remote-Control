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


int main(int argc, char **argv)
{
        ros::init(argc, argv, "canReceiveAndUpdateStatus");
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

     ros::Publisher pub = n.advertise<atlascar2::NominalData>("NominalData", 1000);
     ros::Publisher pub2 = n.advertise<can_msgs::Frame>("RawFrames", 1000);
     ros::Rate loop_rate(1000);

     // the message to be published
     atlascar2::NominalData nominaldata;
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
                // velocity
                if (frame.can_id == 0x412)
                {
                        nominaldata.velocity = frame.data[1];
                        nominaldata.iter = count++;
                }
                // orientation
                if (frame.can_id == 0x236)
                {
                        nominaldata.orientation = (frame.data[0]*256+frame.data[1]-4096)/2;
                }
                // odometer
                if (frame.can_id == 0x412)
                {
                        nominaldata.odometer = (frame.data[2]*256+frame.data[3])*256+frame.data[4];
                }
                // breaker
                if (frame.can_id == 0x231 && frame.data[4] == 0x00)
                {
                        nominaldata.breaker = false;
                }
                if (frame.can_id == 0x231 && frame.data[4] == 0x02)
                {
                        nominaldata.breaker = true;
                }
                // doors
                if ((frame.can_id == 0x424 && frame.data[2] == 0x0F) || (frame.can_id == 0x424 && frame.data[2] == 0x0D) )
                {
                        nominaldata.door = false;
                }
                if (frame.can_id == 0x424 && frame.data[2] == 0x0C)
                {
                        nominaldata.door = true;
                }
                // Left, right and both blinkers
                if (frame.can_id == 0x424 && frame.data[1] == 0x02)
                {
                        nominaldata.left_blinker = true;
                }
                if (frame.can_id == 0x424 && frame.data[1] == 0x01)
                {
                        nominaldata.right_blinker = true;
                }
                if (frame.can_id == 0x424 && frame.data[1] == 0x03)
                {
                        nominaldata.left_blinker = true;
                        nominaldata.right_blinker = true;
                }
                if (frame.can_id == 0x424 && frame.data[1] != 0x01 && frame.data[1] != 0x02 && frame.data[1] != 0x03)
                {
                        nominaldata.left_blinker = false;
                        nominaldata.right_blinker = false;
                }
                // med lights
                if (frame.can_id == 0x424 && frame.data[0] == 0x47) // && frame.data[1] == 0x60) // on
                {
                        nominaldata.med_lights = true;
                }
                // max lights
                if (frame.can_id == 0x424 && (frame.data[1] == 0x24 || frame.data[1] == 0x64)) // on with and without med lights
                {
                        nominaldata.max_lights = true;
                }
                if (frame.can_id == 0x424 && (frame.data[0] == 0x43 || frame.data[0] == 0x03)) //  && frame.data[1] == 0x60) // on
                {
                        nominaldata.med_lights = false;
                }
                if (frame.can_id == 0x424 && frame.data[1] == 0x00) // && frame.data[1] == 0x60) // on
                {
                        nominaldata.max_lights = false;
                }
                // seat belt
                if (frame.can_id == 0x424 && frame.data[0] == 0x03) // on
                {
                        nominaldata.seat_belt = true;
                }
                if (frame.can_id == 0x424 && (frame.data[0] == 0x43)) // on
                {
                        nominaldata.seat_belt = false;
                }

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

                pub.publish(nominaldata);
                pub2.publish(msg);
                ros::spinOnce();

                loop_rate.sleep();
                ++count;
        }
        return 0;
}
