#include "encounter.hpp"

#include <iostream>
#include <sstream>
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

encounter::encounter(uint s, uint t, uint tf, uint ti, uint delta, uint day_i, uint day_f) 
  : s(s), t(t), tf(tf), ti(ti), delta(delta), 
    day(0), min_day(day_i), max_day(day_f)
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


void encounter::set_ti(uint ti){
  this->ti = ti;
}

void encounter::set_tf(uint tf){
  this->tf = tf;
}

uint encounter::get_delta() const{
  return this->delta;
}

void encounter::set_delta() {
  delta = tf - ti;
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

void encounter::print (ofstream& f, const uint& node) const {
  
  if (get_s() == node){
    f << get_s() << ' ' << get_t() << ' ';
  }
  else{
    f << get_t() << ' ' << get_s() << ' ';
  }
    
  f << get_tf() << ' ' << get_ti() << ' ' << get_delta() 
    << ' ' << get_min_day() << ' ' << get_max_day() << '\n';
  
}

bool encounter::operator == (const encounter& other) const{
  bool match_nodes = false;

  if ((s == other.get_s() and t == other.get_t()) or (s == other.get_t() and t == other.get_s()))
    match_nodes = true;

  return (match_nodes and ti == other.get_ti() and tf == other.get_tf());
}

std::ostream& operator<< (std::ostream &out, const encounter &e){
  out << e.get_s() << ' ' << e.get_t() << ' ' << e.get_tf() << ' ' << e.get_ti() 
      << ' ' << e.get_delta() << ' ' << e.get_min_day() << ' ' << e.get_max_day();
  // out << e.get_s() << " <-> " << e.get_t() << "\ttime window: " << '[' << e.get_ti() << "," << e.get_tf() 
  //     << "] day window: [" << e.get_min_day() << '-' << e.get_max_day() << "]" ;
  return out;
}

bool can_merge (const encounter& e1, const encounter& e2){
  return (
    e1.get_s() == e2.get_s() and
    e1.get_t() == e2.get_t() and
    e1.get_tf() == e2.get_ti()
  );
}

encounter merge (const encounter& e1, const encounter& e2){
  encounter novo = e1;
  novo.set_tf(e2.get_tf());
  novo.set_delta();
  return novo;
}
