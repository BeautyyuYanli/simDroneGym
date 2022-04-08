#ifndef SHAPE_HPP
#define SHAPE_HPP
#include "numcpp.hpp"
namespace simDroneGym{
    class baseShape{
    public:
        triple origin; // relative to the spirit coordination
    };
    class cuboidShape : public baseShape{
    public:
        triple length;
        cuboidShape();
        cuboidShape(std::string presetMode, std::initializer_list<triple> argv);
    };
}
#endif