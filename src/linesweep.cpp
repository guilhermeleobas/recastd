#include "debug.hpp"
#include "linesweep.hpp"

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm> //std::set_intersection

using std::cout;
using std::endl;
using std::vector;
using std::set_intersection;


// -=========-

line_sweep::line_sweep(){}

vector<node> line_sweep::node_intersection(const node a, const node b){
  vector<node> v;

  // find all c \in set_a and set_b
  set_intersection(
    begin(node_map[a]), end(node_map[a]),
    begin(node_map[b]), end(node_map[b]),
    std::back_inserter (v));

  return v;
}

// vector<triangle> line_sweep::encounter_intersection (const node a, const node b){
  
//   vector<node> v = node_intersection(a, b);

//   vector<triangle> ret;

//   for (node c : v){
//     encounter e1 = encounter_map[make_pair(a, b)];
//     encounter e2 = encounter_map[make_pair(a, c)];
//     encounter e3 = encounter_map[make_pair(b, c)];
//     ret.push_back (make_tuple(e1, e2, e3));
//   }

//   return ret;
// }

const weak_ptr<encounter> line_sweep::get_encounter (const node a, const node b){
  if (encounter_map.find(make_pair(a, b)) != encounter_map.end())
    return encounter_map.at(make_pair(a, b));

  std::cerr << "cannot find encounter for pair [" << a << ',' << b << "]\n";
  exit(1);
}

void line_sweep::add_encounter(const weak_ptr<encounter>& enc1){
  if (DEBUG)
    cout << "[Line_sweep] adding: " << *(enc1.lock())<< endl;
  
  vector<weak_ptr<encounter>> v = pq.insert(enc1);

  for (const weak_ptr<encounter>& e : v)
    delete_encounter(e);

  node s = enc1.lock()->get_s(), t = enc1.lock()->get_t();
  const edge st = make_pair (s, t);
  const edge ts = make_pair (t, s);

  node_map[s].insert(t);
  node_map[t].insert(s);

  encounter_map.emplace(st, enc1);
  encounter_map.emplace(ts, enc1);
}


void line_sweep::delete_encounter(const weak_ptr<encounter>& enc1){
  auto senc1 = enc1.lock();
  node s = senc1->get_s(), t = senc1->get_t();
  node_map[s].erase(t);
  node_map[t].erase(s);
  

  if (encounter_map.find(make_pair(s, t)) == encounter_map.end()){
    auto e = encounter_map.find(make_pair(s, t));
    cout << "[Checando] " << s << ' ' << t << " -- " << *(e->second.lock()) << endl;
  }
  
  assert (encounter_map.find(make_pair(s, t)) != encounter_map.end());
  assert (encounter_map.find(make_pair(t, s)) != encounter_map.end());

  bool t1 = encounter_map.erase(make_pair(s, t));
  bool t2 = encounter_map.erase(make_pair(t, s));

  assert (t1);
  assert (t2);

}

const weak_ptr<encounter> line_sweep::get_encounter(const pair<node, node>& e){
  return encounter_map.at(e);
}

set<node> line_sweep::get_nodes(const node& node){
  return node_map[node];
}

void line_sweep::print_node_map() const{
  for (auto&& it : node_map){
    cout << it.first << ": ";
    for (auto&& j : it.second)
      cout << j << ' ';
    cout << endl;
  }
}

const pqueue line_sweep::get_pqueue() const{
  return pq;
}
