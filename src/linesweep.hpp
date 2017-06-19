#pragma once

#include <vector>
#include <set>
#include <map>
#include <tuple>
#include <functional>

#include "encounter.hpp"
#include "pqueue.hpp"

using std::multiset;
using std::vector;
using std::map;


typedef unsigned uint;

typedef uint node;
typedef pair<node, node> edge;

class line_sweep {
private:
  map<node, set<node>> node_map;
  map<edge, const encounter> encounter_map;
  pqueue pq;
public:
  line_sweep();
  const encounter get_encounter (const node a, const node b);
  vector<node> node_intersection(const node a, const node b);
  void add_encounter(const encounter& enc1);
  void delete_encounter(const encounter& enc1);
  const encounter get_encounter (const pair<node, node>& e);
  set<node> get_nodes (const node& node);

  const pqueue get_pqueue() const;

  void print_node_map() const;
};
