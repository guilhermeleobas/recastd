#pragma once

#include <vector>
#include <set>
#include <map>
#include <functional>


#include "encounter.hpp"

using std::multiset;
using std::vector;
using std::map;


typedef unsigned uint;
typedef uint node;
typedef pair<node, node> edge;

struct orderByTf {
  bool operator () (const weak_ptr<encounter>& e1, const weak_ptr<encounter>& e2) const;
};

/*
 * This class is used to store the intervals
 * I am assuming that all traces were correctly generated and the case below never happens:
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
class pqueue : public multiset<const weak_ptr<encounter>, orderByTf> {
public:
  vector<weak_ptr<encounter>> remove_unvalid_intervals (const weak_ptr<encounter>& enc);
  vector<weak_ptr<encounter>> insert (const value_type& val);
  void print() const;
};
