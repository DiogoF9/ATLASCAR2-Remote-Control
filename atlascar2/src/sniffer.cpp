#include "ros/ros.h"
#include "can_msgs/Frame.h"

void dataCallback(const can_msgs::Frame::ConstPtr& msg)
{
  ROS_INFO("%03X  %d  %02X %02X %02X %02X %02X %02X %02X %02X", msg->id, msg->dlc, msg->data[0], msg->data[1], msg->data[2], msg->data[3], msg->data[4], msg->data[5], msg->data[6], msg->data[7]);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "sniffer");
  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("/RawFrames", 1000, dataCallback);

  ros::spin();

  return 0;
}