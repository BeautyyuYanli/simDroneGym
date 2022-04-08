### 1. 示例 1

`demo/demo1.cpp`:

```cpp
// import nessecary libraries.
// 引入必要的库
#include <iostream>
#include <random>
using std::cout; using std::endl; using std::cin;
// import simDroneGym and use its namespace.
// 引入本项目 simDroneGym, 并使用其命名空间
#include "../simDroneGym/include/simdronegym.hpp"
using namespace simDroneGym;
// define a funtion to genarate a random triple.
// 设计函数生成随机三元组
// ps. `triple` is a class defined in simDroneGym.
// 注: 三元组是定义在 simDroneGym 中的一个类
triple randt(int minint, int maxint){
    auto size = maxint - minint + 1;
    return triple(rand()%size + minint, 
        rand()%size + minint, rand()%size + minint);
}
int main(){
    srand(time(nullptr));
    // Create an environment.
    // 创建一个环境
    environment e;
    // Set the boundary of the environment.
    // 设置环境的边界
    // It's a zone of 27x27x27.
    // 这是一个 27x27x27 的空间
    e.maxCoord = triple(27,27,27);
    e.minCoord = triple(0,0,0);
    // Set actions for drones, for example:
    // 设置无人机的动作, 例如:
    // Drones can add velocity by 1 in any one dimension.
    // 在任一维度上速度加 1
    // Drones can set acceleration as 1 in any one dimension.
    // 在任一维度上加速度设为 1
    // Drones can be transported to (26,26,26) by some magic.
    // 使用魔法将无人机传送至坐标 (26,26,26)
    e.actionsDefine({
        {"add", "velocity", "0 0 0"},     //0
        {"add", "velocity", "1 0 0"},     //1
        {"add", "velocity", "-1 0 0"},    //2
        {"add", "velocity", "0 1 0"},     //3
        {"add", "velocity", "0 -1 0"},    //4
        {"add", "velocity", "0 0 1"},     //5
        {"add", "velocity", "0 0 -1"},    //6
        {"set", "acceleration", "0 0 0"}, //7
        {"set", "acceleration", "1 0 0"}, //8
        {"set", "acceleration", "-1 0 0"},//9
        {"set", "acceleration", "0 1 0"}, //10
        {"set", "acceleration", "0 -1 0"},//11
        {"set", "acceleration", "0 0 1"}, //12
        {"set", "acceleration", "0 0 -1"},//13
        {"set", "coord", "26 26 26"},     //14
    });
    // Create two drones
    // 创建两架无人机
    // By default, the drone's size is 1x1x1, 
    // coordinate/velocity/acceleration are all 0.
    // 默认地, 无人机的尺寸是 1x1x1 的正方体
    // 坐标/速度/加速度均为 (0,0,0)
    auto dro1 = e.droneCreate();
    auto dro2 = e.droneCreate();
    // Set the second drone's coordinate as a random value, 
    // as well as it's initial velocity.
    // 设置第二架无人机的坐标和初速度为随机值
    e.dronesList[dro2].coord = randt(5,25);
    e.dronesList[dro2].velocity = randt(-3,3);
    // Create some obstacles with random coordinate/velocity/shape, 
    // and 0 acceleration by default.
    // 创建一些障碍物, 坐标/速度/尺寸均为随机值, 无加速度
    for (auto i = 0; i < 5; ++i){
        auto k = e.obstacleCreate();
        e.obstaclesList[k].coord = randt(4,21);
        e.obstaclesList[k].velocity = randt(-3,3);
        e.obstaclesList[k].sp = cuboidShape("cuboid", {randt(1,4)});
    }
    // Create a destination zone from (26,26,26) to (26.1,26.1,26.1)
    // 创建一个目的地区域, 从 (26,26,26) 到 (26.1,26.1,26.1)
    auto des = e.destinationCreate();
    e.destinationsList[des].coord = triple(26,26,26);
    e.destinationsList[des].sp = cuboidShape("cuboid", 
        {triple(0.1,0.1,0.1)});
    // Display the environment
    // 展示这个环境
    cout << "\n\ndisplay the environment, ";
    cout << "press any key\n";getchar();
    e.display(cout);
    // Display the sight from 1st drone's radar.
    // 展示第一架无人机的雷达视野
    // By defualt, the drone's radar is oracle, which can monitor 
    // all drones and obstacles in the environment, except itself.
    // 默认地, 无人机的雷达是"神谕"模式,
    // 可以检测除自身以外的所有无人机和障碍物
    cout << "\n\nDisplay the sight from 1st drone's radar, ";
    cout << "press any key\n";getchar();
    e.dronesList[dro1].radarDetect();
    cout << e.dronesList[dro1].radarResultsToString();
    // Set the 2nd drone's radar scope.
    // 设置第二架无人机的雷达范围
    // It's a cube shape centered on the drone's coordiante, 
    // with a radius of 10.
    // 这是一个以无人机坐标为中心的, 尺寸为 20x20x20 的立方体
    e.dronesList[dro2].radarScope = cuboidShape("radar", 
        {triple(20,20,20)});
    // Display the sight from 2nd drone's radar.
    // 展示第二架无人机的雷达视野
    cout << "\n\nDisplay the sight from 2nd drone's radar, ";
    cout << "press any key\n";getchar();
    e.dronesList[dro2].radarDetect();
    cout << e.dronesList[dro2].radarResultsToString();
    // Have a try! Can you fly the 2nd drone to the destination?
    // 来试试! 你能控制第二架无人机飞抵目的地吗?
    // If the drone collides with the another drone/obstacles/the 
    // boundary of the environment, it's workingState will be set to FALSE,
    // and all properities will be set to 0.
    // 如果无人机与其它无人机机/障碍物/边界相撞, 其 workingState 将被设为 FALSE,
    // 所有其它值将被设为0
    // If a moving obstacle collides in the boundary, 
    // it will be stuck there.
    // 如果移动障碍物撞上边界, 其将被卡住
    while(1){
        size_t a;
        cout <<"\n\nTake your action: ";
        cin >> a;
        // Take an action on the 2nd drone. 
        // Then the time flows by 1 second.
        // 让第二架无人机采取动作, 并让时间流过 1s
        e.act(a, dro2).actDone();
        // Display the state of itself.
        // 展示其自身状态
        e.dronesList[dro2].display(cout);
        // If the drone overlaps the destination, 
        // the function returns TRUE.
        // 如果无人机与目的地区域重叠, 该函数为 TRUE
        if (e.onDestination(dro2, des)) break;
        // If the drone is crushed, break the loop too;
        // 如果无人机损毁, 同样退出循环
        if (!e.dronesList[dro2].workingState) break;
        // Display the sight from the radar.
        // 展示当前的雷达视野
        e.dronesList[dro2].radarDetect();
        cout << e.dronesList[dro2].radarResultsToString();
    }
    return 0;
}
```

### 2. 示例 2

示例 2 演示了利用本库进行 Q-Learning 算法设计.

设置一个简单的离散的环境. 高度上限 60, 下限 0, 不限制横向边界. 在 (30,30,30) 处设有目的地. 随机生成 5 个障碍物, 三个方向上尺寸为 [1,5] 间的随机整数, 三个方向上速度为 [-2,2] 间的随机整数.

无人机初始停放在 (0,0,0) 处, 具有"神谕"模式的雷达. 无人机的可采取的动作共有 7 种: 在三个方向中的一个方向上, 加速度设为 -1 或 1, 或不进行变速. 每次动作持续 1s.

为了在这个环境中使用 Q-Learning 算法寻路, 首先考虑设计奖励:

1. 设计势能函数 X = -30*|r|, 其中 |r| 为无人机坐标与目的地坐标的欧几里得距离.

2. 不发生其它情况时, 奖励为 X - X', 其中 X 为下一状态的势能, X' 为当前状态的势能.

3. 若无人机损毁, 奖励为 -500

4. 设计变量 des_reward 为无人机在目的地停下时的奖励, 初值为 0.

5. 当无人机每次到达目的地时, des_reward += max(best_time-j, 0) * 100, 其中 best_time 是无人机在所有成功的尝试中最短的一次耗时, 默认值为 200.

6. 若无人机未能在"规定的时间"内飞抵目的地, 则奖励为 -10000. "规定的时间" 为 best_time+5.

其中, 1. 与 2. 鼓励无人机向目的地的方向飞行; 4. 5. 6. 则鼓励无人机在找到一个成功策略之后, 进一步寻找耗时更短的策略.

同样为了鼓励无人机寻找耗时更短的策略, 在最近 1000 次飞行中若抵达目的地的概率超过 50%, 则增加无人机动作的随机性, 以寻求更多可能性.

`demo/demo2.cpp`:

```cpp
#include <iostream>
#include <random>
#include <unordered_map>
#include <string>
#include <array>
using std::cout; using std::endl; using std::cin;
#include "../simDroneGym/include/simdronegym.hpp"
using namespace simDroneGym;
triple randt(int minint, int maxint){
    auto size = maxint - minint + 1;
    return triple(rand()%size + minint, rand()%size + minint,
        rand()%size + minint);
}
double randd(){
    return double(rand())/RAND_MAX;
}
double X(size_t droneIndex, size_t desIndex, 
    const environment &env){

    double ans = 0;
    auto tmp = env.dronesList[droneIndex].coord 
        - env.destinationsList[desIndex].coord;
    for (auto k = 0; k < 3; ++k)
        ans += std::abs(tmp[k]);
    return -ans*30;
}
std::unordered_map<std::string, std::array<double, 7>> qtable;
int main(){
    srand(time(nullptr));
    environment e;
    // Set max/min height(Z-coordinate)
    // 设置最高/最低高度(Z坐标)
    e.maxCoord[2] = 60;
    e.minCoord[2] = 0;
    e.actionsDefine({
        {"set", "acceleration", "0 0 0"},    //0   
        {"set", "acceleration", "1 0 0"},    //1 
        {"set", "acceleration", "-1 0 0"},   //2  
        {"set", "acceleration", "0 1 0"},    //3 
        {"set", "acceleration", "0 -1 0"},   //4  
        {"set", "acceleration", "0 0 1"},    //5 
        {"set", "acceleration", "0 0 -1"},   //6  
    });
    // Set a default drone
    // 设置默认配置无人机
    auto dro = e.droneCreate();
    auto des = e.destinationCreate();
    // Set a destination
    // 设置目的地
    e.destinationsList[des].coord = triple(30,30,30);
    e.destinationsList[des].sp = cuboidShape("cuboid", 
        {triple(0.1,0.1,0.1)});
    // Set some random obstacles
    // 设置一些随机障碍物
    for (auto i = 0; i < 5; ++i){
        auto k = e.obstacleCreate();
        e.obstaclesList[k].coord = randt(1,55);
        e.obstaclesList[k].velocity = randt(-2,2);
        e.obstaclesList[k].sp = cuboidShape("cuboid", 
            {randt(1,5)});
    }
    // Use Q-Learning algorithm to design the flight policy. 
    // 使用 Q-Learning 算法设计飞行策略
    int 
        max_episodes = 20000,
        max_timelimit = 200;
    double
        exploration_prob = 1,
        exploration_prob_decay_rate = 0.001,
        min_exploration_prob = 0,
        gamma = 0.99,
        learning_rate = 0.1;
    double des_reward = 0;
    // Variables for analyzation
    // 设置用于分析的变量
    double 
        analyze_total_reward = 0,
        analyze_total_time = 0,
        analyze_total_get_des = 0;
    int
        analyze_episodes = 1000;
    // Variables for best practice
    // 设置存储最佳实践的变量
    std::vector<int> best_actions;
    double best_total_reward = -0x7fffffff;
    int best_time = max_timelimit;
    for (auto i = 0; i < max_episodes; ++i){
        std::vector<int> actions;
        double total_reward = 0;
        auto crushed = false;
        auto get_des = false;
        auto env(e);
        auto current_state = env.dronesList[dro].toString();
        if (!qtable.count(current_state)) 
            qtable[current_state].fill(0);
        int j;
        for (j = 0; j < best_time + 5; ++j){
            // Select action
            // 选择一个动作
            int action;
            if (randd() < exploration_prob)
                action = rand() % 7;
            else{
                double max_q_value = -0x7fffffff;
                const auto &tmp = qtable[current_state];
                for (int i = 0; i < 7; ++i)
                    if (tmp[i] > max_q_value)
                        max_q_value = tmp[i], action = i;
            }
            actions.push_back(action);
            // Do act
            // 执行动作
            auto tmp_x = X(dro, des, env);
            env.act(action, dro).actDone();
            auto next_state = env.dronesList[dro].toString();
            if (!qtable.count(next_state)) 
                qtable[next_state].fill(0);
            // Calculate reward
            // 计算收益
            double reward = X(dro, des, env) - tmp_x;
            if (env.onDestination(dro, des) && 
                env.stopCheck(dro)){

                des_reward += std::max(best_time-j, 0) * 100;
                reward = des_reward;
                get_des = true;
            }
            if (!env.dronesList[dro].workingState){
                reward = -500;
                crushed = true;
            }
            if (j+1 == best_time+5){
                reward = -10000;
                crushed = true;
            }
            total_reward += reward;
            // Update Q-Table
            // 更新 Q-Table
            double max_q_next_state = -0x7fffffff;
            for (auto k : qtable[next_state])
                max_q_next_state = std::max(max_q_next_state, k);
            qtable[current_state][action] = 
                (1-learning_rate) 
                * qtable[current_state][action] 
                + learning_rate 
                * (reward + gamma*max_q_next_state);
            // Update cureent_state
            // 更新当前状态
            if (get_des || crushed){
                break;
            }
            else 
                current_state = next_state;
        }
        exploration_prob = std::max(min_exploration_prob, 
            std::exp(-exploration_prob_decay_rate*i));
        // Save the best practice
        // 存储最佳实践
        if (best_time > j+1 && get_des)
            best_total_reward = total_reward, 
            best_actions = actions, 
            best_time = j+1;
        // Analyze the preformance
        // 分析表现
        analyze_total_reward += total_reward;
        if (get_des){
            analyze_total_get_des ++;
            analyze_total_time += j;
        }
        if ((i+1) % analyze_episodes == 0){
            double get_des_rate = 
                analyze_total_get_des/analyze_episodes * 100;
            double avg_time = 
                analyze_total_time/analyze_total_get_des;
            cout << "=========== Episode: " 
                << i+1 << " ===========" << endl; 
            cout << "Mean reward:      "
                << analyze_total_reward/analyze_episodes << endl;
            cout << "Get des rate:     "<< get_des_rate <<endl;
            cout << "Average time:     "<< avg_time <<endl;
            cout << "Exploration prob: "
                << exploration_prob << endl;
            analyze_total_time = 0;
            analyze_total_reward = 0;
            analyze_total_get_des = 0;
            // Update min_exploration_prob
            // 更新 min_exploration_prob
            if (get_des_rate > 50)
                min_exploration_prob += 0.05;
            else
                min_exploration_prob = 
                    std::max(min_exploration_prob - 0.01, 0.0);
        }
    }
    if (best_total_reward == -0x7fffffff){
        // 算法无法找到有效策略...
        cout << "\nNo Available Policy Found!\n";
        cout << "Maybe the random environment is too complicate...";
    }
    else{
        // Start fly!
        // 开始飞行
        cout << "\nPress any key to display the environment.";
        getchar();
        e.display(cout);

        cout << "\n\n============Best Practice============\n";
        cout << "Least Time: " << best_time << "\n";

        cout << "Display the detailed process?";
        cout << " It may be tedious. (yes/no) ";
        std::string tmp1;
        do {
            cin >> tmp1;
        } while (tmp1 != "yes" && tmp1 != "no");
        bool tmp2 = (tmp1 == "yes");

        for (auto i = 0; i < best_actions.size(); ++i){
            cout << "\n\n==== Time: " << i 
            << " ==== Action: " << best_actions[i] <<" ====";
            e.act(best_actions[i], dro).actDone();
            if (tmp2)
                e.dronesList[dro].display(cout);
        }
    }
    return 0;
}
```
