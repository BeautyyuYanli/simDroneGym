#include "../include/environment.hpp"
#include <set>
#include <sstream>
using namespace simDroneGym;
std::vector<realSpirit> environment::radarRespond(const droneSpirit &detector)const{
    realSpirit tmp(this);
    tmp.coord = detector.coord;
    tmp.sp = detector.radarScope;
    std::vector<realSpirit> ans;
    for (const auto &i : obstaclesList){
        if (i.overlapCheck(tmp))
            ans.push_back(realSpirit(i));
    }
    for (const auto &i : dronesList){
        if (i.overlapCheck(tmp))
            if (&i != &detector)
                ans.push_back(realSpirit(i));
    }
    return ans;
}
size_t environment::droneCreate(){
    dronesList.push_back(droneSpirit(this));
    return dronesList.size() - 1;
}
size_t environment::obstacleCreate(){
    obstaclesList.push_back(obstacleSpirit(this));
    return obstaclesList.size() - 1;
}
size_t environment::destinationCreate(){
    destinationsList.push_back(realSpirit(this));
    return destinationsList.size() - 1;
}
bool environment::actionsDefine(std::vector<std::vector<std::string>> l){
    std::set<std::string> actionType = {"set", "add"};
    std::set<std::string> actionTarget = {"coord", "acceleration", "velocity", "workingState"};
    bool flag = 1;
    for (const auto &i : l){
        if (i.size() < 3) {flag = 0; break;}
        if (!actionType.count(i[0])) {flag = 0; break;}
        if (!actionTarget.count(i[1])) {flag = 0; break;}
    }
    if (flag)
        {actionsList = l; return 1;}
    else{
        std::cerr << "Actions list doesn't match the requirement. No action is added.\n";
        return 0;
    }
}
environment& environment::act(size_t actionIndex, size_t droneIndex){
    if (actionIndex >= actionsList.size() || droneIndex >= dronesList.size()){
        std::cerr << "The action undefined or the drone doesn't exist. Nothing happened.\n";
        return *this;
    }
    const auto &item = actionsList[actionIndex];
    auto &droneTarget = dronesList[droneIndex];
    void *dataTarget = nullptr;

    // dataType: 0:triple | 1:double | 2:bool
    size_t dataType;
    std::stringstream ss(item[2]);

    // item[1]: action target | "coord", "acceleration", "velocity", "workingState"
    if (item[1] == "coord")
        dataTarget = &(droneTarget.coord), dataType = 0;
    else if (item[1] == "acceleration")
        dataTarget = &(droneTarget.acceleration), dataType = 0;
    else if (item[1] == "velocity")
        dataTarget = &(droneTarget.velocity), dataType = 0;
    else if (item[1] == "workingState")
        dataTarget = &(droneTarget.workingState), dataType = 2;

    // item[0]: action type | "set", "add"
    if (dataType == 0){
        auto &realDataTarget = *static_cast<triple*>(dataTarget);
        triple tmp; ss >> tmp;
        if (item[0] == "set") realDataTarget = tmp;
        else if (item[0] == "add") realDataTarget += tmp;
    }
    else if (dataType == 1){
        auto &realDataTarget = *static_cast<double*>(dataTarget);
        double tmp; ss >> tmp;
        if (item[0] == "set") realDataTarget = tmp;
        else if (item[0] == "add") realDataTarget += tmp;
    }
    else if (dataType == 2){
        auto &realDataTarget = *static_cast<bool*>(dataTarget);
        bool tmp; ss >> tmp;
        if (item[0] == "set") realDataTarget = tmp;
        else if (item[0] == "add") realDataTarget ^= tmp;
    }
    return *this;
}
environment& environment::actDone(){
    // event: drones crush
    std::vector<decltype(dronesList.begin())> workingStateUpdate;
    for (auto i = dronesList.begin(); i != dronesList.end(); ++i){
        if (!i->workingState)
            continue;
        for (auto j = i + 1; j != dronesList.end(); ++j){
            if (!j->workingState)
                continue;
            if (i->collisionCheck(*j)){
                workingStateUpdate.push_back(i);
                workingStateUpdate.push_back(j);
            }
        }
        for (const auto &j : obstaclesList){
            if (i->collisionCheck(j))
                workingStateUpdate.push_back(i);
        }
    }
    for (auto &i : workingStateUpdate)
        i->workingState = 0;
    // dispatch all spirits for 1 second
    for (auto &i : dronesList)
        i.dispatcher();
    for (auto &i : obstaclesList)
        i.dispatcher();
    return *this;
}
void environment::display(std::ostream &os)const{
    for (auto i = 0; i < dronesList.size(); ++i){
        os << "\n============drone No. " << i << "============";
        dronesList[i].display(os);
    }
    for (auto i = 0; i < obstaclesList.size(); ++i){
        os << "\n===========obstacle No. " << i << "==========";
        obstaclesList[i].display(os);
    }
    for (auto i = 0; i < destinationsList.size(); ++i){
        os << "\n=========destination No. " << i << "=========";
        destinationsList[i].display(os);
    }
}
std::string environment::toString()const{
    std::stringstream os;
    display(os);
    return os.str();
}
bool environment::stopCheck(size_t droneIndex)const{
    const auto &tmp_velocity = dronesList[droneIndex].velocity;
    const auto &tmp_acceleration = dronesList[droneIndex].acceleration;
    for (auto k = 0; k < 3; ++k){
        if (std::abs(tmp_acceleration[k]) > 0.01 || 
            std::abs(tmp_velocity[k]) > 0.01)
            return 0;
    }
    return 1;
}
bool environment::onDestination(size_t droneIndex, size_t destinarionIndex)const{
    return dronesList[droneIndex].overlapCheck(destinationsList[destinarionIndex]);
}