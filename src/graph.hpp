#pragma once

#include <vector>
#include <unordered_set>
#include <map>
#include <fstream>
#include <functional>
#include <queue>

#include "small_queue.hpp"
#include "encounter.hpp"

#include "roaring.hh"
// #include "roaring.c"

using std::vector;
using std::map;
using std::queue;

typedef unsigned int uint;
typedef uint node;
typedef pair<uint, uint> edge;

#define MAX_DAYS 28

class graph {
 private:
  typedef vector<Roaring> tgraph;

  node ego;
  tgraph gg;
  node latest;
  
  small_queue<encounter> sq;

 public:
  graph();
  graph(const uint ego, const uint max_days, const uint max_nodes);
  graph(const uint ego, const uint max_days, const uint max_nodes, const uint max_len);
  
  node get_latest() const;
  node get_ego() const;
  void set_ego (const node);
  void set_latest(const node n);
  
  const small_queue<encounter>& get_small_queue() const;
  small_queue<encounter>& get_small_queue();
  void add_encounter_to_queue(const encounter&);
  
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

void merge_graphs_share_graph (graph&, graph&, string share_graph, ofstream&);
void merge_graphs_share_neighbors (graph&, graph&, string share_graph, ofstream&);
void merge_graphs_share_last_k (graph&, graph&, string share_graph, ofstream&);
