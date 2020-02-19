#include <ros/ros.h>
#include "learning_service/Person.h"

bool personCallback(learning_service::Person::Request &req, learning_service::Person::Response &res)
{
	ROS_INFO("Person name: %s, age: %d, sex: %d", req.name.c_str(), req.age, req.sex);
	
	res.result = "OK";
	
	return true;
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "person_server");
	ros::NodeHandle node;
	
	ros::ServiceServer person_server = node.advertiseService("/show_person", personCallback);
	ROS_INFO("Ready to show person information");
	ros::spin();
	return 0;
}
