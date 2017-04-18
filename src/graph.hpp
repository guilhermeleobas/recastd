#pragma once

#include <vector>
#include <map>
#include <set>

using namespace std;

class graph {
private:
  // similar to a star graph
  map<unsigned, set<unsigned> > g;
  unsigned person;  
public:
  graph();
  graph(const unsigned person);
  void add_edge (const unsigned p1);
  void add_edge (const unsigned p1, const unsigned p2);
  
};
