#pragma once

#include <vector>
#include <unordered_set>
#include <map>
#include <fstream>
#include <functional>

#include "encounter.hpp"

using std::vector;
using std::map;
using std::unordered_set;

typedef unsigned int uint;
typedef uint node;
typedef pair<uint, uint> edge;

class graph {
private:
  // similar to a star graph
  // map<node, unordered_set<encounter, encounter::hash> > g;
  unordered_set<reference_wrapper<const encounter>, encounter::hash, encounter::my_compare> g;
  node person;
public:
  graph();
  graph(const uint person);
  
  void add_edge (const encounter&);
  void add_edge (const reference_wrapper<const encounter>&, const reference_wrapper<const encounter>&, const reference_wrapper<const encounter>&);

  void dump(ofstream& f);
  
};
