#include "ros/ros.h"
#include "std_msgs/String.h"
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

struct rawdata{
            int velocity;
            int direction;
}

struct nominalvalues{
            int velocity;
            int direction;
}

struct Status{
            struct can_frame frame;
            int nominal_value;
            }

int main(int argc, char **argv)
{
        ros::init(argc, argv, "ReceiveMessage");
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

        ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
        ros::Rate loop_rate(50000);
  
        int count = 0;
        int velocidade, angulo;
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
                // velocidade
                if (frame.can_id == 0x412)
                {
                        //printf("velocidade: %d    ",frame.data[1]);
                        velocidade = frame.data[1];
                }
                // direcao
                if (frame.can_id == 0x236)
                {
                        // printf("angulo: %d\n",(frame.data[0]*256+frame.data[1]-4096)/2);
                        angulo = (frame.data[0]*256+frame.data[1]-4096)/2;
                }

                 std_msgs::String msg;

                std::stringstream ss;
                ss << "velocidade: " << velocidade;
                ss << "         angulo: " << angulo;
                msg.data = ss.str();

                ROS_INFO("%s", msg.data.c_str());

                chatter_pub.publish(msg);

                ros::spinOnce();
                
                loop_rate.sleep();
                ++count;

        }
        return 0;
}
