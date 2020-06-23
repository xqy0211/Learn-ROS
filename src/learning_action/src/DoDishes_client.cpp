#include <learning_action/DoDishesAction.h> 
#include <actionlib/client/simple_action_client.h>

typedef actionlib::SimpleActionClient<learning_action::DoDishesAction> Client;


// 当action完成后会调用次回调函数一次
void doneCb(const actionlib::SimpleClientGoalState& state,
        const learning_action::DoDishesResultConstPtr& result)
{
    ROS_INFO("Yay! The dishes are now clean");
    ros::shutdown();
}

// 当action激活后会调用次回调函数一次
void activeCb()
{
    ROS_INFO("Goal just went active");
}

// 收到feedback后调用的回调函数
void feedbackCb(const learning_action::DoDishesFeedbackConstPtr& feedback)
{
    ROS_INFO(" percent_complete : %f ", feedback->percent_complete);
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "do_dishes_client");
	
	//定义客户端
	Client client("do_dishes", true);// true -> don't need ros::spin()
	
    // 等待服务器端
    ROS_INFO("Waiting for action server to start.");
    client.waitForServer();
    ROS_INFO("Action server started, sending goal.");

    // 创建一个action的goal
    learning_action::DoDishesGoal goal;
    goal.dishwasher_id = 1;

    // 发送action的goal给服务器端，并且设置回调函数
    client.sendGoal(goal,  &doneCb, &activeCb, &feedbackCb);

    ros::spin();

    return 0;
}