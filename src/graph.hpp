#pragma once

#include <vector>
#include <unordered_set>
#include <map>

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
  map<node, unordered_set<encounter, encounter::hash> > g;
  node person;  
  uint _size;
public:
  graph();
  graph(const uint person);
  
  void add_edge (const encounter&);
  void add_edge (const encounter&, const encounter&, const encounter&);

  map<node, unordered_set<encounter, encounter::hash>>::size_type size() const;


  void dump();
  
};
