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

#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <fstream>
using namespace std;
#pragma once

int main(int argc, char **argv)
{
        ros::init(argc, argv, "canReceiveAndUpdateStatus");
        ros::NodeHandle n;


        int angulo = 0;
        int velocidade = 0;
        int contador=0;
        int s;
        int nbytes_rcv;
        struct sockaddr_can addr;
        struct can_frame frame;
        struct ifreq ifr;

        /*name of the can device - vcan0 in case of virtual CAN bus*/
        const char *ifname = "can0";

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
     ros::Rate loop_rate(1000);

     // the message to be published
     atlascar2::NominalData nominaldata;

    ofstream inFile;
	inFile.open("40.txt");

    //check for error
    if (inFile.fail()) {
    cerr << "error opening file" << endl;
    exit(1);
    }
    double signal1;
    double signal2;
    double tempo;


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


                count = count + 1;
                nominaldata.count = count;

                // ao mandar mensagem segundo a segundo não preciso de calcular tempo

                //time_t timer;
                //struct tm y2k = {0};
                //double seconds;

                //y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
                //y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

                //time(&timer);  /* get current time; same as: timer = time(NULL)  */

                //seconds = difftime(timer,mktime(&y2k));


                // velocity
                if (frame.can_id == 0x412)
                {
                        nominaldata.velocity = frame.data[1];
                        velocidade = frame.data[1];
                        nominaldata.iter = count++;
                        // publish the current time x
                }
                // orientation
                if (frame.can_id == 0x236)
                {
                        nominaldata.orientation = (frame.data[0]*256+frame.data[1]-4096)/2;
                        angulo = (frame.data[0]*256+frame.data[1]-4096)/2;
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
                if ((frame.can_id == 0x424 && frame.data[2] == 0x0F) || (frame.can_id == 0x424 && frame.data[2] == 0x0D))
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
                // mudança
                if (frame.can_id == 0x418)
                {
                    if (frame.data[0] == 0x50) //P
                    {
                            nominaldata.gear = 1;
                    }
                    else if (frame.data[0] == 0x52) //R
                    {
                            nominaldata.gear = 2;
                    }
                    else if (frame.data[0] == 0x4E) //P
                    {
                            nominaldata.gear = 3;
                    }
                    else if (frame.data[0] == 0x44) //D
                    {
                            nominaldata.gear = 4;
                    }
                    else
                    {
                            nominaldata.gear = 0; //erro
                    }
                }
                // direcao desejada
                if (frame.can_id == 0x500)
                {
                contador += 1;
                printf("estou aqui\n");

                signal1 = (float)(frame.data[0]*100+frame.data[1])*5/1023;
                signal2 = (float)(frame.data[2]*100+frame.data[3])*5/1023;
                tempo = (float)(frame.data[4]);

                inFile << contador;
                inFile << " ";
                inFile << signal1;
                inFile << " ";
                inFile << signal2;
                inFile << " ";
                inFile << angulo;
                inFile << " ";
                inFile << velocidade;
                inFile << " ";
                inFile << tempo << endl;

                }
                pub.publish(nominaldata);
                ros::spinOnce();

                loop_rate.sleep();
        }
        return 0;
}
