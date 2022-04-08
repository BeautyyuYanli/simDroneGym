#include <gtest/gtest.h>
#include "simdronegym.hpp"
#include <random>
#include <ctime>
#include <vector>
#include <iostream>
using namespace simDroneGym;
environment e;
double randd(){
    double s[2] = {1.0,-1.0};
    return s[(rand() % 2)] * (rand() % 1024);
}
std::vector<double> randv(size_t n){
    std::vector<double> a;
    for (auto i = 0; i < n; ++ i)
        a.push_back(randd());
    return a;
}
triple randt(){
    return triple(randd(), randd(), randd());
}
TEST(numcpp_triple, multiply){
    for (auto z = 0; z < 10000; ++z){
        auto arr = randv(6);
        arr[3] = 0.5;
        triple a = triple(arr[0], arr[1], arr[2]) * arr[3];
        EXPECT_EQ(a[0], arr[0]*arr[3]);
        EXPECT_EQ(a[1], arr[1]*arr[3]);
        EXPECT_EQ(a[2], arr[2]*arr[3]);
    }
}
TEST(numcpp_triple, pluseq){
    auto z = 10000;
    while (z--){
        triple a = randt();
        triple b = a;
        triple c = randt();
        a += c;
        for (auto k = 0; k < 3; ++ k){
            EXPECT_DOUBLE_EQ(a[k], b[k]+c[k]);
        }
    }
}
TEST(numcpp_intervals, union){
    intervals a = intervals(1,3) + intervals(2,4) + intervals(-9, -8);
    std::stringstream oa;
    a.print(oa);
    EXPECT_TRUE(oa.str() == "(-9,0)(-8,1)(1,0)(4,1)");
    intervals b = a + intervals(4,4) + intervals(-8, -7);
    std::stringstream ob;
    a.print(ob);
    EXPECT_TRUE(ob.str() == "(-9,0)(-8,1)(1,0)(4,1)");
}
TEST(numcpp_intervals, intersection){
    intervals a = intervals(1,3) * intervals(2,4);
    std::stringstream oa;
    a.print(oa);
    EXPECT_TRUE(oa.str() == "(2,0)(3,1)");
    intervals b = intervals(3,5) * a;
    std::stringstream ob;
    b.print(ob);
    EXPECT_TRUE(ob.str() == "");
    intervals c = (intervals(1,3) + intervals(7,9))
        * (intervals(2,4) + intervals(6,8));
    std::stringstream oc;
    c.print(oc);
    EXPECT_TRUE(oc.str() == "(2,0)(3,1)(7,0)(8,1)");
}
TEST(numcpp_intervals, empty){
    intervals a = intervals(1,3)*intervals(3,5) + intervals(6,6);
    EXPECT_TRUE(a.empty());
}
TEST(spirit, UAbase){
    for (auto z = 0; z < 10000; ++z){
        baseSpirit k(&e);
        auto a = k.coord = randt();
        auto b = k.velocity = randt();
        auto c = k.acceleration = randt();
        k.dispatcher();
        for (auto j = 0; j < 3; ++j){
            // EXPECT_DOUBLE_EQ(k.coord[j], a[j]+c[j]*double(0.5)+b[j]);
            // EXPECT_DOUBLE_EQ(k.coord[j]*2, a[j]*2+b[j]*2+c[j]);
            EXPECT_DOUBLE_EQ(k.velocity[j], b[j]+c[j]);
            EXPECT_DOUBLE_EQ(k.acceleration[j], c[j]);
        }
    }

}
TEST(spirit, overlap){
    realSpirit a(&e), b(&e);
    b.sp = a.sp = cuboidShape("cuboid_centered", {triple(2,2,2)});
    EXPECT_TRUE(a.overlapCheck(b));
    a.coord = triple(2,0,0);
    EXPECT_FALSE(a.overlapCheck(b));
    b.coord = triple(0,2,0);
    EXPECT_FALSE(b.overlapCheck(a));
    b.coord = triple(0.1,1.9,0);
    EXPECT_TRUE(b.overlapCheck(a));
    b.coord = triple(0.1,1.9,2);
    EXPECT_FALSE(b.overlapCheck(a));
}
TEST(spirit, collision){
    realSpirit a(&e), b(&e);
    a.sp.length = b.sp.length = triple(0.1,0.1,0.1);
    // test 1 
    a.coord = triple(0,0,0);
    b.coord = triple(10,0,0);
    a.velocity = triple(5,0,0);
    b.velocity = triple(-5,0,0);
    EXPECT_TRUE(a.collisionCheck(b));
    // test 2
    a.coord = triple(0,0,0);
    b.coord = triple(10.1,0,0);
    a.velocity = triple(5,0,0);
    b.velocity = triple(-5,0,0);
    EXPECT_FALSE(a.collisionCheck(b));
    // test 3
    a.sp.length = triple(0.5,0.5,0.5);
    a.coord = triple(0,0,0);
    b.coord = triple(10.4,0,0);
    a.velocity = triple(5,0,0);
    b.velocity = triple(-5,0,0);
    EXPECT_TRUE(a.collisionCheck(b));
    // test 4
    a.sp.length = b.sp.length = triple(0.1,0.1,0.1);
    a.coord = triple(0,0,0);
    b.coord = triple(10.1,0,0);
    a.velocity = triple(0,0,0);
    b.velocity = triple(0,0,0);
    a.acceleration = triple(20,0,0);
    b.acceleration = triple(0,0,0);
    EXPECT_FALSE(a.collisionCheck(b));
    // test 5
    a.coord = triple(0,0,0);
    b.coord = triple(10,0,0);
    a.velocity = triple(0,0,0);
    b.velocity = triple(0,0,0);
    a.acceleration = triple(30,0,0);
    b.acceleration = triple(10,0,0);
    EXPECT_TRUE(a.collisionCheck(b));
    // test 6
    a.coord = triple(0,10.1,0);
    b.coord = triple(10,0,0);
    a.velocity = triple(0,-5,0);
    b.velocity = triple(0,5,0);
    a.acceleration = triple(30,0,0);
    b.acceleration = triple(10,0,0);
    EXPECT_FALSE(a.collisionCheck(b));
    // test 7
    a.coord = triple(0,10,0);
    b.coord = triple(10,0,0);
    a.velocity = triple(0,-5,0);
    b.velocity = triple(0,5,0);
    a.acceleration = triple(30,0,0);
    b.acceleration = triple(10,0,0);
    EXPECT_TRUE(a.collisionCheck(b));
    // test 8
    a.coord = triple(0,10,0);
    b.coord = triple(10,0,0);
    a.velocity = triple(0,0,0);
    b.velocity = triple(0,0,0);
    a.acceleration = triple(0,0,0);
    b.acceleration = triple(0,0,0);
    EXPECT_FALSE(a.collisionCheck(b));
}
TEST(spirit, radar_oracle){
    realSpirit a(&e), b(&e);
    a.sp = cuboidShape("radar_oracle", 
        {(e.maxCoord - e.minCoord) * 2,  e.minCoord - e.maxCoord});
    b.sp = cuboidShape("cuboid_centered", {triple(2,2,2)});
    EXPECT_TRUE(a.overlapCheck(b));
    EXPECT_TRUE(b.collisionCheck(a));
    a.velocity = triple(0,-5,0);
    b.velocity = triple(0,5,0);
    EXPECT_TRUE(b.collisionCheck(a));
    a.acceleration = triple(30,0,0);
    b.acceleration = triple(10,0,0);
    EXPECT_TRUE(b.collisionCheck(a));
}
TEST(spirit, radar){
    std::stringstream os1, os2, os3, os4;
    e.dronesList.push_back(droneSpirit(&e));
    e.dronesList[0].coord = randt();
    for (auto i = 0; i < 5; ++i){
        e.obstaclesList.push_back(obstacleSpirit(&e));
        e.obstaclesList[i].sp = cuboidShape("cuboid", {triple(1,1,1)});
        e.obstaclesList[i].coord = randt();
        os1 << e.obstaclesList[i].coord;
    }
    for (auto i = 1; i < 5; ++i){
        e.dronesList.push_back(droneSpirit(&e));
        e.dronesList[i].coord = randt();
        os1 << e.dronesList[i].coord;
    }
    e.dronesList[0].radarDetect();
    for (const auto &i : e.dronesList[0].radarResults){
        os2 << i.coord;
    }
    EXPECT_EQ(os1.str(), os2.str());
    auto &d = e.dronesList[1];
    d.coord = triple(0,0,0);
    d.radarScope = cuboidShape("cuboid_centered", {triple(20,20,20)});
    for (auto i = 1; i < 5; ++i){
        auto a = obstacleSpirit(&e);
        a.sp = cuboidShape("cuboid", {triple(1,1,1)});
        a.coord = triple(i*3, i*2, i*1);
        e.obstaclesList.push_back(a);
        if (i < 4)
            os3 << a.coord;
    }
    d.radarDetect();
    for (const auto &i : d.radarResults){
        os4 << i.coord;
    }
    EXPECT_EQ(os3.str(), os4.str());
}
TEST(spirit, dispatch){
    environment e;
    e.dronesList.push_back(droneSpirit(&e));
    auto &d1 = e.dronesList.back();
    d1.coord = triple(10,10,0);
    d1.velocity = triple(-1,-1,0);

    e.dronesList.push_back(droneSpirit(&e));
    auto &d2 = e.dronesList.back();
    d2.coord = triple(10,0,0);
    d2.velocity = triple(-1,1,0);

    e.dronesList.push_back(droneSpirit(&e));
    auto &d3 = e.dronesList.back();
    d3.coord = triple(0,0,0);
    d3.velocity = triple(1,1,0);

    e.obstaclesList.push_back(obstacleSpirit(&e));
    auto &o1 = e.obstaclesList.back();
    o1.coord = triple(2,3,0);
    o1.sp.length = triple(1,1,1);

    auto &d = e.dronesList;
    for (size_t i = 0; i < 10; ++ i){
        std::cout << i << " " << d[0].coord << d[1].coord << d[2].coord;
        std::cout << d[0].workingState << d[1].workingState << d[2].workingState << std::endl;
        e.actDone();
    }

}
TEST(environment, action_define_err){
    environment e;
    EXPECT_TRUE(e.actionsDefine({
        {"add", "coord", "1 0 0", "move x+1"},
        {"add", "coord", "-1 0 0", "move x-1"},
        {"add", "coord", "0 1 0", "move y+1"},
        {"add", "coord", "0 -1 0", "move y-1"},
        {"add", "coord", "0 0 1", "move z+1"},
        {"add", "coord", "0 0 -1", "move z-1"},
    }));
    EXPECT_FALSE(e.actionsDefine({
        {"add", "coord", "1 0 0", "move x+1"},
        {"add", "coord", "-1 0 0", "move x-1"},
        {"add", "coord", "0 1 0", "move y+1"},
        {"add", "coord", "0 -1 0", "move y-1"},
        {"add", "coord", "0 0 1", "move z+1"},
        {"add", "coord", "0 0 -1", "move z-1"},
        {}
    }));
    EXPECT_FALSE(e.actionsDefine({
        {"add", "coord", "1 0 0", "move x+1"},
        {"add", "coord", "-1 0 0", "move x-1"},
        {"add", "coord", "0 1 0", "move y+1"},
        {"add", "coord", "0 -1 0", "move y-1"},
        {"add", "coord", "0 0 1", "move z+1"},
        {"add", "coor", "0 0 -1", "move z-1"},
    }));
}
int main(int argc, char **argv) {
    srand(time(NULL));
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}