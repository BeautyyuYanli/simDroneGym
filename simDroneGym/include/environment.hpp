#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP
#include "spirit.hpp"
#include <vector>
namespace simDroneGym{
    class environment{
        std::vector<std::vector<std::string>> actionsList;
    public:
        std::vector<realSpirit> radarRespond(const droneSpirit &detector)const;
        // Interface
        triple minCoord = triple(-1024,-1024,-1024),
            maxCoord = triple(1024,1024,1024);
        std::vector<obstacleSpirit> obstaclesList;
        std::vector<droneSpirit> dronesList;
        std::vector<realSpirit> destinationsList;
        size_t droneCreate();
        size_t obstacleCreate();
        size_t destinationCreate();
        bool actionsDefine(std::vector<std::vector<std::string>>);
        environment& act(size_t actionIndex, size_t droneIndex);
        environment& actDone();
        void display(std::ostream &os)const;
        std::string toString()const;
        bool stopCheck(size_t droneIndex)const;
        bool onDestination(size_t droneIndex, size_t destinarionIndex)const;
    };
}
#endif