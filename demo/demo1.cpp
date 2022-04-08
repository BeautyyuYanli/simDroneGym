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
    // 如果移动障碍物装上边界, 其将被卡住
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