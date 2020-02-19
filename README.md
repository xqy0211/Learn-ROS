# Learn-ROS
Indigo中的ROS学习
src下两个功能包：均是用c++学习的。</br>
learning_topic包含了对Publisher、Subscriber的学习以及对Person.msg的自定义消息。</br>
learning_service包含了对Client、Server的实现。</br>

## learning_topic
pos_subscriber和velocity_publisher主要与海龟仿真器节点(turtlesim)一起运行。</br>
pos_subscriber：订阅turtlesim发布的消息/turtle1/pose， 之后在新终端打印海龟位置信息。</br>
velocity_publisher：通过发布者的文件形式给turtlesim的/turtle1/cmd_vel发布速度指令，消息类型为geometry_msgs::Twist，线速度0.5m/s，角速度0.2rad/s。</br>
person_publisher：定义新的消息名/person_info，新的消息类型Person.msg，通过本节点将/person_info发布。</br>
person_subscriber：本节点接受/person_info并在终端打印/person_info信息。

## learning_service
主要与海龟仿真器节点(turtlesim)一起运行，turtlesim仿真器定义了/spawn服务，用途在于产生新的海龟。</br>
turtle_spawn：通过文件形式向turtlesim server请求service，在x = 2.0和y = 2.0处产生新的小海龟，并命名为turtle2。</br>
turtle_command_server：定义了一个海龟运动开关的服务/turtle_command，功能在于当/turtle_command收到请求时，通过一个标志位颠倒海龟的运动状态，
开关速度指令的发布。</br>
