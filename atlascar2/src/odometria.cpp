#include "ros/ros.h"
#include "atlascar2/NominalData.h"
#include <iostream>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <vector>

void dataCallback(const atlascar2::NominalData::ConstPtr& msg)
{
  ROS_INFO("travao: %d", msg->breaker);
  ROS_INFO("velocidade: %d", msg->velocity);
  ROS_INFO("orientacao: %d", msg->orientation);

  static tf::TransformBroadcaster br;
  static tf::TransformListener listener;

  tf::Transform T0;
  if (msg->iter < 5000)
  {
            float sx = 0;
            float sy = 0;

            T0.setOrigin( tf::Vector3(sx, sy, 0.0) );
            tf::Quaternion q;
            q.setRPY(0, 0, 0);
            T0.setRotation(q);
  }
  else
  {
            tf::StampedTransform T0;
            try{
                listener.lookupTransform("/world", "atlascar2", ros::Time(0), T0);
            }
            catch (tf::TransformException ex){
                ros::Duration(0.1).sleep();
            }
  }

            tf::Transform T1;
            T1.setOrigin( tf::Vector3(msg->velocity, 0.0, 0.0) );
            tf::Quaternion q;
            q.setRPY(0, 0, 3.14/30);
            T1.setRotation(q);

            tf::Transform Tglobal = T0*T1;
            br.sendTransform(tf::StampedTransform(Tglobal, ros::Time::now(), "world", "atlascar2"));


/* faz orientacao a partir da origem
  static tf::TransformBroadcaster br;
  tf::Transform transform;
  transform.setOrigin( tf::Vector3(msg->velocity, 0, 0.0) );
  tf::Quaternion q;
  q.setRPY(0, 0, msg->orientation);
  transform.setRotation(q);
  br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", "atlascar2"));
*/
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "odometria");
  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("/NominalData", 1000, dataCallback);

  ros::spin();

  return 0;
}
