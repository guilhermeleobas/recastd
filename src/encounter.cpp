#include "encounter.hpp"

#include <iostream>
#include <sstream>
#include <cmath>

// definition of timestep;
uint64_t timestep = DEFAULT_TIMESTEP;
uint64_t min_tf = 0x3f3f3f3f;
uint64_t min_ti = 0x3f3f3f3f;

encounter::encounter(){}

encounter::encounter(uint s, uint t, uint64_t tf, uint64_t ti, uint64_t delta) 
  : s(s), t(t), tf(tf), ti(ti), delta(delta), 
    min_day(0), max_day(0x3f3f3f3f)
  {}

encounter::encounter(uint s, uint t, uint64_t tf, uint64_t ti, uint64_t delta, uint day_i, uint day_f) 
  : s(s), t(t), tf(tf), ti(ti), delta(delta), 
    min_day(day_i), max_day(day_f)
  {}
  
void encounter::calc_day(){
  // [min_ti, timestep] are both global variables exposed as extern in encounter.hpp
  // in the original paper, for some reason the day is calculated using tf, for instance:
  // day = (tf - min_tf) / timestep
  
  this->min_day = ceil ( (ti - min_ti) / timestep );
  this->max_day = ceil ( (tf - min_ti) / timestep );
}

void encounter::set_s( const uint s){
  this->s = s;
}

void encounter::set_t( const uint t){
  this->t = t;
}


void encounter::set_min_day(uint min_day){
  this->min_day = min_day;
}

void encounter::set_max_day(uint max_day){
  this->max_day = max_day;
}

uint encounter::get_min_day() const{
  return this->min_day;
}

uint encounter::get_max_day() const{
  return this->max_day;
}

uint64_t encounter::get_tf() const{
  return this->tf;
}

uint64_t encounter::get_ti() const{
  return this->ti;
}


void encounter::set_ti(uint64_t ti){
  this->ti = ti;
}

void encounter::set_tf(uint64_t tf){
  this->tf = tf;
}

uint64_t encounter::get_delta() const{
  return this->delta;
}

void encounter::calc_delta() {
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
    (e2.get_ti() - e1.get_tf() <= 1)
  );
}


bool can_merge_no_restriction(const encounter& e1, const encounter& e2){
  return (
    e1.get_s() == e2.get_s() and
    e1.get_t() == e2.get_t() and
    (e2.get_ti() <= e1.get_tf() and e2.get_tf() >= e1.get_tf())
  );
}

encounter merge (const encounter& e1, const encounter& e2){
  encounter novo = e1;
  novo.set_tf(e2.get_tf());
  novo.calc_delta();
  novo.calc_day();
  novo.set_min_day(min(e1.get_min_day(), e2.get_min_day()));
  novo.set_max_day(max(e1.get_max_day(), e2.get_max_day()));
  return novo;
}
