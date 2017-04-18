#include "graph.hpp"

graph::graph(){}

graph::graph(const unsigned person){
  this->person = person;
}

void graph::add_edge(const unsigned p1){
  g[person].insert(p1); 
}

void graph::add_edge(const unsigned p1, const unsigned p2){
  g[person].insert(p1); 
  g[person].insert(p2); 
}
