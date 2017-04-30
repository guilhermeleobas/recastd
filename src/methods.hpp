#pragma once

#include <vector>
#include <set>
#include <map>


#include "encounter.hpp"

using std::multiset;
using std::vector;
using std::map;


typedef unsigned uint;
typedef uint node;
typedef pair<node, node> edge;

struct orderByTf {
  bool operator () (const encounter& e1, const encounter& e2) const;
};

/*
 * This class is used to store the intervals
 * I am assuming that all traces were correctly generated and the type of thing below would never happen:
 *
 * 1 2 3 4 5 6 7 (a <-> b) 
 *     3 4 5     (a <-> b)
 *
 * There are two meetings with the same node id happening simutaneously.
 *
 * Whenever I process an interval (meeting), I delete previous intervals in the multiset
 * that have tf_previous < ti_new
 * 
 * Intervals are sorted in creasing way using "tf" as parameter (orderByTf).
*/
class pqueue : public multiset<encounter, orderByTf> {
public:
  vector<encounter> remove_unvalid_intervals (const encounter& enc);
  vector<encounter> insert (const value_type& val);
  void print() const;
};

// -====-

class line_sweep {
private:
  map<node, set<node>> node_map;
  map<edge, encounter> encounter_map;
  pqueue pq;
public:
  line_sweep();
  bool can_merge (const encounter& enc1, const encounter& enc2) const;
  vector<encounter> find_contacts (const encounter& e1);
  void add_encounter(const encounter& enc1);
  void delete_encounter(const encounter& enc1);
  encounter get_encounter (const pair<node, node>& e);
  set<node> get_nodes (const node& node);

  const pqueue get_pqueue() const;

  void print_node_map() const;
};