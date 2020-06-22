# Learn-ROS
Indigo中的ROS学习
src下6个功能包：均是用c++学习的。</br>
learning_topic包含了对Publisher、Subscriber的学习以及对Person.msg的自定义消息。</br>
learning_service包含了对Client、Server的实现。</br>
learning_parameter包含了对参数的使用及编程方法。</br>
learning_tf包含了对ROS中的坐标管理方法，对于多智能体控制理论上有帮助。</br>
learning_launch包含了对于lanunch文件的读写方法，有助于多节点启动的场景。</br>
ROS-Behavior-Tree包含了行为树的使用，目的在于多体控制时定义行为顺序。



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

## learning_tf
学习了ROS中坐标系管理系统tf：可以记录机器人10秒钟之内所有坐标系之间的位置关系，通过广播和监听tf实现。</br>
在查看tf时大致有三种方法：rosrun tf view_frames生成坐标系位置关系pdf、rosrun tf tf_echo [tf1] [tf2]打印出坐标系间关系、rviz工具直观显示。</br>
注意理解的一点是坐标系之间的关系实质上是一个向量（还是2阶矩阵？下学期应该会学到）。</br>
功能包中主要节点有：turtle_tf_listener、turtle_tf_broadcaster，都是与turtlesim海龟仿真器一起使用的。</br>
turtle_tf_broadcaster：用于广播tf。广播相关程序主要在pose的回调函数中，当仿真器turtle1生成（就是仿真器启动时），一个subscriber订阅turtle1的位置信息（Pose），进入回调函数，回调函数中创建了tf广播器，turtle与world之间的tf关系可以由Pose处理后封装，封装的格式为tf::Transform。需要体会一下用一个程序启动两个节点的“重映射”。</br>
turtle_tf_listener：用于接收tf，发布turtle2的速度信息。启动程序时调用/spawn服务生成新的海龟，监听/turtle1和/turtle2的tf，保存于tf::StampedTransform类型的transform（一个连接两个坐标系的向量）中。通过向量的长度得到坐标系间距离，除以时间获得turtle2的cmd_vel的linear.x；通过向量的x、y方向长度反正切确定坐标系间夹角，通过除以时间获得turtle2的cmd_vel的angular.z，进而封装好vel_msg，发布给turtle2。</br>
命令行依次输入：</br>
$ roscore</br>
$ rosrun turtlesim turtlesim_node</br>
$ rosrun learning_tf turtle_tf_broadcaster __name:=turtle1_tf_broadcaster /turtle1</br>
$ rosrun learning_tf turtle_tf_broadcaster __name:=turtle2_tf_broadcaster /turtle2</br>
$ rosrun learning_tf turtle_tf_listener</br>
$ rosrun turtlesim turtle_teleop_key</br>
即可实现海龟跟踪。

## learning_launch
launch文件是ROS通过XML标签实现的，本功能包中学习了launch文件的编写，免去过多终端的繁杂，适合在每个节点调试完毕之后同时启动。</br>
常用的标签有launch、node、param、rosparam、remap、include等。</br>
具体通过四个launch文件学习：simple.launch、turtlesim_parameter_config.launch、start_tf_demo.launch、turtle_remap.launch。</br>
simple.launch：启动了两个节点，分别是learning_topic中的person_subscriber和person_publisher。</br>
turtlesim_parameter_config.launch：学习了参数保存到参数服务器中、通过文件导入参数。</br>
start_tf_demo.launch：将learning_tf中五条指令通过launch文件实现，效果一样。</br>
turtle_remap.launch：学习重映射话题名（重映射计算图资源的命名）。

## ROS-Behavior-Tree
！！！要注意，因为也用到了learning_tf中部分重名的，故使用此包时要把learning_tf包删除！！！ </br>
从公众号：古月居中找到的有用的功能包。</br>
实现功能：敌方白色的小乌龟在地图中沿某条固定线路巡逻，我方用键盘控制红色小乌龟运动。当敌我距离太近时，敌方小乌龟会察觉我方存在，并展开追逐。此时我们需要移动红色小乌龟，甩开敌方一定距离后，敌方才会重新回到巡逻路线上。</br>
链接：https://mp.weixin.qq.com/s?__biz=MzU1NjEwMTY0Mw==&mid=2247486381&idx=1&sn=1609582a0d35774abd54f74a3fb048a1&chksm=fbcb72c9ccbcfbdf62546f30bb7adeddf4d629dd3c374b77f4e857a3d42cfbd851efa9e0da18&mpshare=1&scene=23&srcid=&sharer_sharetime=1589078266917&sharer_shareid=206f850bf7fea469ec89cb52c8bff691#rd
</br>实际上是在tf跟随的基础上增加了单体的决策（通过行为树实现），其中比较重要的文件：</br>
ROS-Behavior-Tree/behavior_tree_core/src/tree/：</br>
 guard_robot_tree.cpp定义了行为树的结构</br>
ROS-Behavior-Tree/behavior_tree_leaves/nodes/：</br>
 action_nav_enemy.cpp动作节点——向敌人移动</br>
 action_patrol.cpp动作节点——巡逻</br>
 condition_have_enemy条件节点——判断敌人</br>
ROS-Behavior-Tree/behavior_tree_leaves/src：</br>
 turtle_tf_broadcaster.cpp广播第一只小乌龟的tf</br>
 turtle_tf_broadcaster_2.cpp广播第二只小乌龟和巡逻点坐标的tf</br>
打开终端编译并运行：</br>
$ cd ~/catkin_ws</br>
$ catkin_make</br>
$ roslaunch behavior_tree_leaves guard_robot_behavior_tree.launch</br>

## learning_action
ActionClient 和ActionServer之间使用action protocol通信，action protocol就是预定义的一组ROS message，包括goal、cancel、status、result、feedback
1.新建action/DoDishes.action文件，action文件由三部分组成，部分之间用---隔开，第一部分goal相当于service中的request，第二部分result相当于server回传给client的结果，第三部分feedback为实时反馈结果。</br>
区别在于result只可能传一次，feedback实时，有可能多次。</br>
实现了.action之后，还需要将这个文件加入编译，在CMakeLists.txt文件中添加如下的编译规则：</br>
find_package(catkin REQUIRED genmsg actionlib_msgs actionlib)</br>
add_action_files(DIRECTORY action FILES DoDishes.action) generate_messages(DEPENDENCIES actionlib_msgs)</br>
在package.xml中添加：</br>
<build_depend>actionlib</build_depend>
<build_depend>actionlib_msgs</build_depend>
<run_depend>actionlib</run_depend> 
<run_depend>actionlib_msgs</run_depend>
catkin_make后在catkin_ws/devel/share/learning_action/msg文件夹下出现7个文件，用于ActionClient 和 ActionServer间的通信，在devel/include/actionlib_test/中也生成了相关的头文件</br>
学习时犯的错误：action文件夹应该放到learning_action下而不是learning_action/src下 否则会出现：add_message_files() directory not found</br>






















