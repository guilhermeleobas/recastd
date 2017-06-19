#pragma once

#include <vector>
#include <unordered_set>
#include <map>
#include <fstream>
#include <functional>
#include <boost/dynamic_bitset.hpp>

#include "encounter.hpp"

#include "roaring.hh"
// #include "roaring.c"

using std::vector;
using std::map;
using std::unordered_set;

typedef unsigned int uint;
typedef uint node;
typedef pair<uint, uint> edge;

#define MAX_DAYS 28

class graph {
 private:
  // similar to a star graph
  // map<node, unordered_set<encounter, encounter::hash> > g;
  // typedef unordered_set<weak_ptr<encounter>, encounter::hash,
                        // encounter::my_compare> muset;  // my unordered set


  // typedef set<int> muset;
  // muset g;
  
  // typedef vector<vector<bitset<28>>> tgraph;
  typedef vector<Roaring> tgraph;

  node person;
  tgraph gg;

  node latest;

 public:
  graph();
  graph(const uint person, const uint max_days, const uint max_nodes);
  
  node get_latest() const;
  node get_person() const;
  void set_person (const node);
  void set_latest(const node n);

  void add_encounter(const encounter&);
  void add_encounter(const encounter&, const encounter&, const encounter&);

  void dump(ofstream& f);

  // iterators
  using iterator = tgraph::iterator;
  using reference = tgraph::reference;
  // using pointer = tgraph::value_type;
  // using value_type = tgraph::value_type;

  using const_iterator = tgraph::const_iterator;
  using const_reference = tgraph::const_reference;

  reference operator[] (uint index){ return gg[index]; }
  const_reference operator[] (uint index) const { return gg[index]; }

  iterator begin() { return gg.begin(); }
  iterator end() { return gg.end(); }
  
  const_iterator begin() const { return gg.begin(); }
  const_iterator end() const { return gg.end(); }
  const_iterator cbegin() const { return gg.cbegin(); }
  const_iterator cend() const { return gg.cend(); }


};

void merge_graphs (graph&, graph&, bool share_graph, ofstream&);
// graph merge_graphs (const graph&, const graph&, const graph&);
