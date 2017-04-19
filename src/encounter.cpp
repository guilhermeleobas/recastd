#include "encounter.hpp"

encounter::encounter(){}

encounter::encounter(uint s, uint t, uint tf, uint ti, uint delta) 
  : s(s), t(t), tf(tf), ti(ti), delta(delta){}

uint encounter::get_day() const{
  return this->day;
}

void encounter::set_day(const uint min_tf){
  this->day = (this->tf - min_tf)/timestep;
}

uint encounter::get_tf() const{
  return this->tf;
}

uint encounter::get_ti() const{
  return this->ti;
}

bool encounter::contact(const encounter &other) const{
  return other.get_ti() <= this->tf ||
    this->ti <= other.get_tf();
}
