#include <ros/ros.h>
#include <turtlesim/Pose.h>
#include <tf/transform_broadcaster.h>

std::string turtle_name;

void PoseCallback(const turtlesim::PoseConstPtr& msg)
{
	static tf::TransformBroadcaster br;
	
	tf::Transform transform;
	transform.setOrigin(tf::Vector3(msg->x, msg->y, 0));
	tf::Quaternion q;
	q.setRPY(0, 0, msg->theta);
	transform.setRotation(q);
	
	br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", turtle_name));
	ROS_INFO("tf between world and %s broadcasted!", turtle_name.c_str());
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "turtle_tf_broadcaster");
	ros::NodeHandle node;
	
	if(argc != 2)
	{
		ROS_ERROR("need turtle name as argument");
		return -1;
	}
	
	turtle_name = argv[1];
	
	ros::Subscriber sub = node.subscribe(turtle_name+"/pose", 10, &PoseCallback);
	ros::spin();
	return 0;
}
