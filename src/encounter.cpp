#include "encounter.hpp"

#include <iostream>
#include <cmath>

// definition of timestep;
uint timestep = DEFAULT_TIMESTEP;
uint min_tf = 0x3f3f3f3f;
uint min_ti = 0x3f3f3f3f;

encounter::encounter(){}

encounter::encounter(uint s, uint t, uint tf, uint ti, uint delta) 
  : s(s), t(t), tf(tf), ti(ti), delta(delta), 
    day(0), min_day(0), max_day(0x3f3f3f3f)
  {}

uint encounter::get_day() const{
  return this->day;
}

void encounter::set_day(){
  // [min_ti, timestep] are both global variables exposed as extern in encounter.hpp
  // in the original paper, for some reason the day is calculated using tf, for instance:
  // day = (tf - min_tf) / timestep
  
  min_day = ceil ( (ti - min_ti) / timestep );
  max_day = ceil ( (tf - min_ti) / timestep );
  day = min_day;

}

uint encounter::get_min_day() const{
  return this->min_day;
}

uint encounter::get_max_day() const{
  return this->max_day;
}

uint encounter::get_tf() const{
  return this->tf;
}

uint encounter::get_ti() const{
  return this->ti;
}


uint encounter::get_s() const{
  return this->s;
}

uint encounter::get_t() const{
  return this->t;
}


edge encounter::get_edge() const {
  return make_pair(this->s, this->t);
}

edge encounter::get_reverse_edge() const {
  return make_pair(this->t, this->s);
}

bool encounter::operator == (const encounter& other) const{
  return (this->s == other.get_s() and 
    this->t == other.get_t() and
    this->ti == other.get_ti() and
    this->tf == other.get_tf());
}

std::ostream& operator<< (std::ostream &out, const encounter &e){
  out << e.get_s() << " <-> " << e.get_t() << " : " << '[' << e.get_ti() << "," << e.get_tf() 
      << "] day window: [" << e.get_min_day() << '-' << e.get_max_day() << "]" ;
  return out;
}
