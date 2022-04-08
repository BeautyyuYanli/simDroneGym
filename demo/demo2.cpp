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