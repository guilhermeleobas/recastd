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
  typedef unordered_set<weak_ptr<encounter>, encounter::hash, encounter::my_compare>
      muset;  // my unordered set

  unordered_set<weak_ptr<encounter>, encounter::hash, encounter::my_compare> g;
  node person;

 public:
  graph();
  graph(const uint person);

  void add_edge(const weak_ptr<encounter>&);
  void add_edge(const weak_ptr<encounter>&, const weak_ptr<encounter>&, const weak_ptr<encounter>&);

  void dump(ofstream& f);

  // iterators
  using iterator = muset::iterator;
  using const_iterator = muset::const_iterator;

  iterator begin() { return g.begin(); }
  iterator end() { return g.end(); }
  const_iterator begin() const { return g.begin(); }
  const_iterator end() const { return g.end(); }
  const_iterator cbegin() const { return g.cbegin(); }
  const_iterator cend() const { return g.cend(); }
  
} ;
