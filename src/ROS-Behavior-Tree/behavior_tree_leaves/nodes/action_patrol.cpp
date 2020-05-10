/* Copyright (C) 2015-2017 Michele Colledanchise - All Rights Reserved
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
*   to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
*   and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
*   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
*   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <behavior_tree_core/BTAction.h>

#include <string>

#include <tf/transform_listener.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Spawn.h>


enum Status {RUNNING, SUCCESS, FAILURE};  // BT return status


class BTAction
{
protected:
    ros::NodeHandle nh_;
    // NodeHandle instance must be created before this line. Otherwise strange error may occur.
    actionlib::SimpleActionServer<behavior_tree_core::BTAction> as_;
    std::string action_name_;
    // create messages that are used to published feedback/result
    behavior_tree_core::BTFeedback feedback_;  // action feedback (SUCCESS, FAILURE)
    behavior_tree_core::BTResult result_;  // action feedback  (same as feedback for us)
    
    // 定义turtle2的速度控制发布器
    ros::Publisher turtle_vel =
    nh_.advertise<geometry_msgs::Twist>("turtle2/cmd_vel", 10);



public:
    explicit BTAction(std::string name) :
        as_(nh_, name, boost::bind(&BTAction::execute_callback, this, _1), false),
        action_name_(name)
    {
        // Starts the action server
        as_.start();
    }

    ~BTAction(void)
    {}

    void execute_callback(const behavior_tree_core::BTGoalConstPtr &goal)
    {
        // tf监听器
        tf::TransformListener listener;
        tf::StampedTransform transform_a, transform_b, transform;
        
        // 查找坐标变换
        listener.waitForTransform("/turtle2", "/point_a", ros::Time(0), ros::Duration(3.0));
        listener.lookupTransform("/turtle2", "/point_a", ros::Time(0), transform_a);
        listener.waitForTransform("/turtle2", "/point_b", ros::Time(0), ros::Duration(3.0));
        listener.lookupTransform("/turtle2", "/point_b", ros::Time(0), transform_b);
        
        double distance_a, distance_b;
        distance_a = sqrt(pow(transform_a.getOrigin().x(), 2) + pow(transform_a.getOrigin().y(), 2));
        distance_b = sqrt(pow(transform_b.getOrigin().x(), 2) + pow(transform_b.getOrigin().y(), 2));
        
        if (nh_.hasParam("/goal_point")){
            if(distance_a < 0.5) {
                nh_.setParam("/goal_point", "b");
                ROS_INFO("Change direction to b");
            }
            else if(distance_b < 0.5){
                nh_.setParam("/goal_point", "a");
                ROS_INFO("Change direction to b");
            }
        }
        else{
            nh_.setParam("/goal_point", "a");
        }
        
        std::string direction;
        nh_.getParam("/goal_point", direction);
        
        if(direction == "a"){
            ROS_INFO("Nav to a");
            transform = transform_a;
        }
        else if(direction == "b"){
            ROS_INFO("Nav to b");
            transform = transform_b;
        }
           
        // 根据turtle1和turtle2之间的坐标变换，计算turtle2需要运动的线速度和角速度
        // 并发布速度控制指令，使turtle2向turtle1移动
        geometry_msgs::Twist vel_msg;
        vel_msg.angular.z = 1.0 * atan2(transform.getOrigin().y(),
                                        transform.getOrigin().x());
//      vel_msg.linear.x = 0.5 * sqrt(pow(transform.getOrigin().x(), 2) +
//                                          pow(transform.getOrigin().y(), 2));
        vel_msg.linear.x = 0.8;
                                                                            
        turtle_vel.publish(vel_msg);

        set_status(SUCCESS);
        ROS_INFO("Action nav_b is successful!");
    }


    // returns the status to the client (Behavior Tree)
    void set_status(int status)
    {
        // Set The feedback and result of BT.action
        feedback_.status = status;
        result_.status = feedback_.status;
        // publish the feedback
        as_.publishFeedback(feedback_);
        // setSucceeded means that it has finished the action (it has returned SUCCESS or FAILURE).
        as_.setSucceeded(result_);

        switch (status)  // Print for convenience
        {
        case SUCCESS:
            ROS_INFO("Action %s Succeeded", ros::this_node::getName().c_str() );
            break;
        case FAILURE:
            ROS_INFO("Action %s Failed", ros::this_node::getName().c_str() );
            break;
        default:
            break;
        }
    }
};


int main(int argc, char** argv)
{
    ros::init(argc, argv, "action");
    ROS_INFO(" Enum: %d", RUNNING);
    ROS_INFO(" Action Ready for Ticks");
    BTAction bt_action(ros::this_node::getName());
    ros::spin();

    return 0;
}
