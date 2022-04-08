#include "../include/numcpp.hpp"
using namespace simDroneGym;
triple::triple():
    val{0,0,0} {}
triple::triple(double al, double be, double ge):
    val{al,be,ge} {}
triple::triple(const triple &be):
    val{be.val[0], be.val[1], be.val[2]} {}
double& triple::operator[](size_t idx){
    return val[idx];
}
const double& triple::operator[](size_t idx)const{
    return val[idx];
}
triple& triple::operator=(const triple &be){
    if (this == &be)
        return *this;
    val[0] = be.val[0], val[1] = be.val[1], val[2] = be.val[2];
    return *this;
}
bool triple::operator==(const triple &be){
    return (val[0]==be.val[0] && val[1]==be.val[1] && val[2]==be.val[2]);
}
triple triple::operator+(const triple &be) const{
    triple ans(*this);
    for (auto i = 0; i < 3; ++i)
        ans.val[i] += be.val[i];
    return ans;
}
triple triple::operator-(const triple &be) const{
    triple ans(*this);
    for (auto i = 0; i < 3; ++i)
        ans.val[i] -= be.val[i];
    return ans;
}
triple triple::operator*(double be) const{
    triple ans(*this);
    for (auto &i : ans.val)
        i *= be;
    return ans;
}
triple& triple::operator+=(const triple &be){
    *this = *this + be;
    return *this;
}
triple& triple::operator-=(const triple &be){
    *this = *this - be;
    return *this;
}
std::istream& operator>>(std::istream &is, simDroneGym::triple &be){
    double a, b, c;
    is >> a >> b >> c;
    if (is.fail()){}
    else
        be = triple(a,b,c);
    return is;
}
std::ostream &operator<<(std::ostream &os, const triple &be){
    os << "(" << be.val[0] << ", " << be.val[1] << ", " << be.val[2] << ")";
    return os;
}

intervals::intervals(double left, double right){
    if (left > right)
        std::swap(left, right);
    v.push_back(std::make_pair(left, 0));
    v.push_back(std::make_pair(right, 1));
}
intervals intervals::baseArithmetic(const intervals &be, bool mode) const{
    intervals ans;
    auto p1 = this->v.begin(), p2 = be.v.begin();
    bool flag1 = 0, flag2 = 0;
    bool lastState = 0;
    while (p1 != this->v.end() || p2 != be.v.end()){
        auto *p = &p2;
        auto *flag = &flag2;
        if ( p1 != this->v.end() &&
            (p2 == be.v.end() || p1->first <= p2->first))
            p = &p1, flag = &flag1;
        *flag = !(*p)->second;
        bool currentState = (mode) ? (flag1 || flag2) : (flag1 && flag2);
        if (currentState ^ lastState){
            lastState = currentState;
            ans.v.push_back(**p);
        }
        (*p)++;
    }
    intervals tmp;
    for (auto i = ans.v.begin(); i != ans.v.end(); ++i){
        if (i+1 != ans.v.end() && i->first == (i+1)->first){
            ++i;
            continue;
        }
        else {
            tmp.v.push_back(*i);
        }
    }
    return tmp;
}
intervals intervals::operator+(const intervals &be) const{
    return this->baseArithmetic(be, 1);
}
intervals intervals::operator*(const intervals &be) const{
    return this->baseArithmetic(be, 0);
}
bool intervals::empty() const{
    return v.empty();
}
void intervals::print(std::ostream &os) const{
    for (const auto &i : v){
        os << "(" << i.first << "," << i.second << ")";
    }
}
std::ostream &operator<<(std::ostream &os, const simDroneGym::intervals &be){
    be.print(os);
    return(os);
}