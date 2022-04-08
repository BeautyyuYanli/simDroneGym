#ifndef SPIRIT_HPP
#define SPIRIT_HPP
#include "numcpp.hpp"
#include "shape.hpp"
namespace simDroneGym{
    class environment;
    class baseSpirit{
    protected:
        const environment *env = nullptr;
    public:
        baseSpirit() = default;
        baseSpirit(const environment *);
        virtual void dispatcher();
        // Interface
        triple coord, velocity, acceleration;
        virtual void display(std::ostream &os)const;
    };
    class realSpirit : public baseSpirit{
    public:
        realSpirit(const environment *);
        realSpirit(const realSpirit *);
        virtual void dispatcher();
        // Interface
        cuboidShape sp;
        bool overlapCheck(const realSpirit &)const;
        bool collisionCheck(const realSpirit &)const;
        virtual void display(std::ostream &os)const;
    };
    class obstacleSpirit : public realSpirit{
    public:
        obstacleSpirit(const environment *);
        // Interface
        void display(std::ostream &os)const;
    };
    class droneSpirit : public realSpirit{
    public:
        droneSpirit(const environment *);
        void dispatcher();
        // Interface
        bool workingState = true;
        cuboidShape radarScope;
        std::vector<realSpirit> radarResults;
        void radarDetect();
        std::string radarResultsToString();
        void display(std::ostream &os)const;
        std::string toString()const;
    };
}
#endif