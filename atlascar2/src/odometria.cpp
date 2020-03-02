#include "ros/ros.h"
#include "atlascar2/NominalData.h"

void dataCallback(const atlascar2::NominalData::ConstPtr& msg)
{
  ROS_INFO("velocidade: %d", msg->velocity);
  ROS_INFO("orientacao: %d", msg->orientation);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "odometria");
  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("/NominalData", 1000, dataCallback);

  ros::spin();

  return 0;
}
