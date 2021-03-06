#include <ros/ros.h>
#include <string>
#include <std_srvs/Empty.h>

int main(int argc, char **argv)
{
	int red, green, blue;
	ros::init(argc, argv, "parameter_config");
	ros::NodeHandle node;
	
	ros::param::get("/background_r", red);
	ros::param::get("/background_g", green);
	ros::param::get("/background_b", blue);
	ROS_INFO("Get background color: [red: %d, green: %d, blue: %d]",red, green, blue);
	
	ros::param::set("/background_r", 255);
	ros::param::set("/background_g", 255);
	ros::param::set("/background_b", 255);
	ROS_INFO("Background color set to [255, 255, 255]");
	
	ros::param::get("/background_r", red);
	ros::param::get("/background_g", green);
	ros::param::get("/background_b", blue);
	ROS_INFO("Re-get background color: [red: %d, green: %d, blue: %d]",red, green, blue);
	
	ros::service::waitForService("/clear");
	ros::ServiceClient clear_background = node.serviceClient<std_srvs::Empty>("/clear");
	
	std_srvs::Empty srv;
	clear_background.call(srv);
	sleep(1);
	return 0 ;
}
