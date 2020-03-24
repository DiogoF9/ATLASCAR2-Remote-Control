#include "ros/ros.h"
#include "atlascar2/NominalData.h"
#include <iostream>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <vector>
#include <math.h>

#define PI 3.14159265

void dataCallback(const atlascar2::NominalData::ConstPtr& msg)
{
  //ROS_INFO("travao: %d", msg->breaker);
  ROS_INFO("velocidade: %d", msg->velocity);
  ROS_INFO("orientacao: %d", msg->orientation);

  static tf::TransformBroadcaster br;
  static tf::TransformListener listener;


/*
tf::Transform T0;
            float sx = 0;
            float sy = 0;

            T0.setOrigin( tf::Vector3(sx, sy, 0.0) );
            tf::Quaternion q;
            q.setRPY(0, 0, 0);
            T0.setRotation(q);

            br.sendTransform(tf::StampedTransform(T0, ros::Time::now(), "world", "atlascar2"));

/*
            tf::StampedTransform T0;
            try{
                listener.lookupTransform("/world", "atlascar2", ros::Time(0), T0);
            }
            catch (tf::TransformException ex){
                ros::Duration(0.1).sleep();
            }

            tf::Transform T1;
            T1.setOrigin( tf::Vector3(msg->velocity, 0.0, 0.0) );
            tf::Quaternion q;
            q.setRPY(0, 0,  msg->orientation*3.14/180);
            T1.setRotation(q);

            tf::Transform Tglobal = T0*T1;
            br.sendTransform(tf::StampedTransform(Tglobal, ros::Time::now(), "world", "atlascar2"));
*/

float x;
float y;

// posicao inicial
if (msg->count==1)
{
    x = 0;
    y = 0;
}

float delta_x = (float)msg->velocity/3600*cos(msg->orientation*PI/180);
float delta_y = (float)msg->velocity/3600*sin(msg->orientation*PI/180);


if (msg->gear == 2)
{
delta_x = -delta_x;
delta_y = -delta_y;
}

x = x+delta_x;
y = y+delta_y;

ROS_INFO("contagem: %d", msg->count);
ROS_INFO("posicao x: %f", delta_x);
ROS_INFO("posicao y: %f", delta_y);


/* faz orientacao a partir da origem */
  tf::Transform T1;
  T1.setOrigin( tf::Vector3(x,y, 0.0) );
  tf::Quaternion q1;
  q1.setRPY(0, 0,  msg->orientation*PI/180);
  T1.setRotation(q1);



  tf::Transform Tglobal = T1;
  br.sendTransform(tf::StampedTransform(Tglobal, ros::Time::now(), "world", "atlascar2"));
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "odometria");
  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("/NominalData", 1000, dataCallback);

  ROS_INFO("QUALQUER COISA");


  ros::spin();

  return 0;
}
