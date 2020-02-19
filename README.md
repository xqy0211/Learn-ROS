# Learn-ROS
Indigo中的ROS学习
src下3个功能包：均是用c++学习的。</br>
learning_topic包含了对Publisher、Subscriber的学习以及对Person.msg的自定义消息。</br>
learning_service包含了对Client、Server的实现。</br>
learning_parameter包含了对参数的使用及编程方法。</br>

## learning_topic
pos_subscriber和velocity_publisher主要与海龟仿真器节点(turtlesim)一起运行。</br>
pos_subscriber：订阅turtlesim发布的消息/turtle1/pose， 之后在新终端打印海龟位置信息。</br>
velocity_publisher：通过发布者的文件形式给turtlesim的/turtle1/cmd_vel发布速度指令，消息类型为geometry_msgs::Twist，线速度0.5m/s，角速度0.2rad/s。</br>
定义新的消息名/person_info，新的消息类型Person.msg，主要包括姓名、性别、年龄。</br>
person_publisher：本节点将/person_info以learning_topic::Person数据类型发布。</br>
person_subscriber：本节点接受/person_info并在终端打印/person_info信息。

## learning_service
主要与海龟仿真器节点(turtlesim)一起运行，turtlesim仿真器定义了/spawn服务，用途在于产生新的海龟。</br>
turtle_spawn：通过文件形式向turtlesim server请求service，在x = 2.0和y = 2.0处产生新的小海龟，并命名为turtle2。</br>
turtle_command_server：定义了一个海龟运动开关的服务/turtle_command，功能在于当/turtle_command收到请求时，通过一个标志位颠倒海龟的运动状态，
开关速度指令的发布。</br>
在学习topic与message时会遇到这样的问题：如果不希望消息一直发布，而是需要的时候发布一次，这时候就需要service。定义了新的srv:Person.srv，里面包含了与Person.msg中类似的信息，不同之处在于srv有反馈。</br>
person_client:本节点是用户端，等待服务器/show_person响应，向服务器发送learning_servie::Person类型的服务请求，希望打印Person信息，成功后打印response信息。</br>
person_server:作为/show_person的服务器端，在有用户提出服务请求时进入回调函数，打印出Person信息，并反馈结果（response）给用户端。

## learning_parameter
简单学习了全局参数的设置、获取，以及在命令行中设置获取参数以及参数文件的保存和加载。</br>
parameter_config.cpp：与海龟仿真器一起使用，主要实现了对背景颜色的获取和修改，修改完毕后通过Client调用服务/clear实现背景参数写入海龟仿真器。
