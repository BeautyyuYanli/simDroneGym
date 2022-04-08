#include "../include/shape.hpp"
using namespace simDroneGym;
cuboidShape::cuboidShape() = default;
cuboidShape::cuboidShape(std::string presetMode, std::initializer_list<triple> argv){
    auto arg = argv.begin();
    if (presetMode == "cuboid"){
        // argv[0]: length of edges
        length = triple(arg[0]);
    }
    else if (presetMode == "radar" || presetMode == "cuboid_centered"){
        // argv[0]: length of edges
        length = triple(arg[0]);
        origin = triple(0,0,0) - length * 0.5;
    }
    else if (presetMode == "radar_oracle" || presetMode == "manual"){
        // agv[0]: length of edges, [1]: origin
        length = arg[0];
        origin = arg[1];
    }
    else{
        std::cerr << "no preset shape mode found, using defult instead.";
    }
}