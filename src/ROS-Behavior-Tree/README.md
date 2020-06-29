参见https://github.com/miccol/ROS-Behavior-Tree
-----------

以下对代码学习和分析
-------
与直接clone下的behavior-tree的文件差别（需要自己修改或者添加的部分）：  
**1.** behavior_tree_core文件夹中的src下增加文件夹tree，tree中新增文件**guard_robot_tree.cpp**  
**2.** behavior_tree_leaves文件夹中的launch下增加1个文件：**guard_robot_behavior_tree.launch**   </br>
**3.** behavior_tree_leaves文件夹中的nodes下增加3个文件：**action_nav_enemy.cpp、action_patrol.cpp、condition_have_enemy.cpp**</br>
**4.** behavior_tree_leaves文件夹中的src下增加两个文件：**turtle_tf_broadcaster.cpp**和**turtle_tf_broadcaster_2.cpp**</br></br>

### guard_robot_tree.cpp
主要看try中代码，  
new BT::ROSCondition新建条件节点，  
new BT::ROSAction新建动作节点，  
new BT::FallbackNode新建选择节点，  
new BT::SequenceNode新建序列节点。  
新建好这些节点以后通过->AddChild的方式设计节点间的关系。  
设计好之后 Execute(guard, TickPeriod_milliseconds)生成树。</br>

### guard_robot_behavior_tree.launch
两个关于海龟仿真器的节点启动：turtlesim包下的turtlesim_node和turtle_teleop_key；  
两个启动tf，与learning_tf中几乎一样；  
三个启动行为树节点，一个启动行为树。  </br>

### action_patrol.cpp
很多bahavior_tree中格式化的东西，重点在于void execute_callback()函数，其中监听了turtle2和point_a及point_b之间的transform， 通过计算turtle2至两个点的距离来设置运动方向，根据transform来改变Twist，发布速度消息，从而实现巡逻操作。  

### action_nav_enemy.cpp
与action_patrol.cpp类似，execute_callback()中监听turtle1和turtle2间的transform，以此改变turtle2的速度，实现跟随turtle1。

### condition_have_enemy.cpp
execute_callback()中判断是否找到turtle1，通过监听turtle1和turtle2间的transform，计算两者之间的距离，从而判断结果。

**turtle_tf_broadcaster.cpp**：与learning_tf中几乎一样，发布  </br>
**turtle_tf_broadcaster_2.cpp**：与learning_tf中几乎一样，不同之处在于还发布了两个固定点的坐标

