#include "encounter.hpp"

encounter::encounter(){}

encounter::encounter(uint s, uint t, uint tf, uint ti){
  this->s = s;
  this-> t = t;
  this->tf = tf;
  this->ti = ti;
  this->delta = tf - ti;
}
