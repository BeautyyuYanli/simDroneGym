#include "../include/spirit.hpp"
#include "../include/environment.hpp"
#include <cmath>
#include <sstream>
using namespace simDroneGym;
baseSpirit::baseSpirit(const environment *e):
    env(e) {}
void baseSpirit::dispatcher(){
    for (auto k = 0; k < 3; ++k){
        if (coord[k] > env->maxCoord[k] || coord[k] < env->minCoord[k])
            return;
    }
    coord += velocity + acceleration * double(0.5);
    velocity += acceleration;
}
void baseSpirit::display(std::ostream &os)const{
    os << "\ncoordinate:" << coord << " velocity:" << velocity << " acceleration:" << acceleration;
}
realSpirit::realSpirit(const environment *e):
    baseSpirit(e) {}
realSpirit::realSpirit(const realSpirit *be):
    realSpirit(*be) {}
void realSpirit::dispatcher(){
    auto tmp1 = coord + sp.origin, tmp2 = tmp1 + sp.length;
    for (auto k = 0; k < 3; ++k){
        if (tmp2[k] > env->maxCoord[k] || tmp1[k] < env->minCoord[k])
            return;
    }
    baseSpirit::dispatcher();
}
bool realSpirit::overlapCheck(const realSpirit &be)const{
    auto 
        c = (this->coord + this->sp.origin) - (be.coord + be.sp.origin);
    bool flag = 1;
    for (int k = 0; k < 3; ++k){
        if (c[k] <= -(this->sp.length[k]) || c[k] >= be.sp.length[k]){
            flag = 0;
            break;
        }
    }
    return flag;
}
bool realSpirit::collisionCheck(const realSpirit &be)const{
    auto
        a = this->velocity - be.velocity,
        b = this->acceleration - be.acceleration,
        c = (this->coord + this->sp.origin) - (be.coord + be.sp.origin);
    triple cc[2] = {c + this->sp.length, c - be.sp.length};
    intervals timeInterval[3];
    // the intervals when the spirits overlap in k-th dimension
    // al.coord + al.velocity * t + 0.5 * al.acceleration * t^2 \in (-this->sp.length, be.sp.length);
    for (auto k = 0; k < 3; ++k){
        bool ind = (b[k] > 0);
        if (b[k] != 0){
            double lambda[2] = {a[k]*a[k] - 2*b[k]*cc[!ind][k], 
                a[k]*a[k] - 2*b[k]*cc[ind][k]};
            if (lambda[!ind] <= 0){
            }
            else if (lambda[ind] <= 0){
                timeInterval[k] = intervals((-a[k]-std::sqrt(lambda[!ind]))/b[k],
                    (-a[k]+std::sqrt(lambda[!ind]))/b[k]);
            }
            else{
                timeInterval[k] = intervals((-a[k]-std::sqrt(lambda[ind]))/b[k], 
                    (-a[k]-std::sqrt(lambda[!ind]))/b[k])
                    + intervals((-a[k]+std::sqrt(lambda[!ind]))/b[k],
                    (-a[k]+std::sqrt(lambda[ind]))/b[k]);
            }
        }
        else{
            ind = (a[k] < 0);
            if (a[k] != 0)
                timeInterval[k] = intervals(-cc[ind][k]/a[k], -cc[!ind][k]/a[k]);
            else {
                if (cc[0][k] > 0 && cc[1][k] < 0)
                    timeInterval[k] = intervals(0,1);
            }
        }
    }
    intervals ans = intervals(0,1);
    for (const auto &i : timeInterval){
        ans = ans * i;
    }
    return !ans.empty();
}
void realSpirit::display(std::ostream &os)const{
    baseSpirit::display(os);
    os << "\nshape: from " << coord + sp.origin << " to " << coord + sp.origin + sp.length;
}
obstacleSpirit::obstacleSpirit(const environment *e):
    realSpirit(e) {}
void obstacleSpirit::display(std::ostream &os)const{
    realSpirit::display(os);
}
droneSpirit::droneSpirit(const environment *e):
    realSpirit(e), 
    radarScope("radar_oracle", 
        {(env->maxCoord - env->minCoord) * 2,  env->minCoord - env->maxCoord})
{
        sp = cuboidShape("cuboid", {triple(1,1,1)});
}
void droneSpirit::dispatcher(){
    if (workingState){
        realSpirit::dispatcher();
        auto tmp1 = coord + sp.origin, tmp2 = tmp1 + sp.length;
        for (auto k = 0; k < 3; ++k){
            if (tmp2[k] > env->maxCoord[k] || tmp1[k] < env->minCoord[k])
                workingState = 0;
        }
    }
    if (!workingState){
        sp = cuboidShape();
        coord = acceleration = velocity = triple();
    }
}
void droneSpirit::radarDetect(){
    radarResults = env->radarRespond(*this);
}
std::string droneSpirit::radarResultsToString(){
    std::stringstream os;
    for (auto i = 0; i < radarResults.size(); ++i){
        os << "\n============object No. " << i << "============";
        radarResults[i].display(os);
    }
    return os.str();
}
void droneSpirit::display(std::ostream &os)const{
    realSpirit::display(os);
    os << "\nworkingState: " << workingState;
}
std::string droneSpirit::toString()const{
    std::stringstream os;
    display(os);
    return os.str();
}