#ifndef NUMCPP_HPP
#define NUMCPP_HPP
#include <iostream>
#include <vector>
namespace simDroneGym{
    class triple{
    public:
        double val[3];
        triple();
        triple(double val_1, double val_2, double val_3);
        triple(const triple &);
        double& operator[](size_t index);
        const double& operator[](size_t index)const;
        triple& operator=(const triple &);
        bool operator==(const triple &);
        triple operator+(const triple &) const;
        triple operator-(const triple &) const;
        triple operator*(double) const;
        triple& operator+=(const triple &);
        triple& operator-=(const triple &);
    };
    class intervals{
        std::vector<std::pair<double, bool>> v;
        intervals baseArithmetic(const intervals &, bool mode) const;
    public:
        intervals() = default;
        intervals(double left, double right);
        intervals operator+(const intervals &) const;
        intervals operator*(const intervals &) const;
        bool empty() const;
        void print(std::ostream &os) const;
    };
}
std::istream& operator>>(std::istream &, simDroneGym::triple &);
std::ostream& operator<<(std::ostream &, const simDroneGym::triple &);
std::ostream& operator<<(std::ostream &, const simDroneGym::intervals &);
#endif