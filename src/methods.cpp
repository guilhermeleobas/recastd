#include "debug.hpp"
#include "methods.hpp"

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm> //std::set_intersection

using std::cout;
using std::endl;
using std::vector;
using std::set_intersection;

bool orderByTf::operator()(const encounter &e1, const encounter &e2) const{
  return e1.get_tf() < e2.get_tf();
}

vector<encounter> pqueue::remove_unvalid_intervals (const encounter& e1){
  vector<encounter> v;
  while (not this->empty()){
    const encounter& topo = *(this->begin());

    if (topo.get_tf() < e1.get_ti()){
      if (DEBUG)
        cout << "\tremoveu: " << topo << endl;

      v.push_back (topo);

      this->erase(this->begin());
    }
    else{
      break;
    }
  }
  return v;
}

vector<encounter> pqueue::insert (const pqueue::value_type& e1){
  if (DEBUG)
    cout << "adicionando: " << e1 << endl;

  vector<encounter> v = this->remove_unvalid_intervals(e1);
  multiset::insert(e1);
  return v;
}

void pqueue::print() const {
  for (auto&& it : *this){
    cout << it << '\n';
  }
}

// -=========-

line_sweep::line_sweep(){}

vector<encounter> line_sweep::find_contacts (const encounter& e1){
  node a = e1.get_s();
  node b = e1.get_t();

  vector<node> v;

  // find all c \in set_a and set_b
  set_intersection(
    begin(node_map[a]), end(node_map[a]),
    begin(node_map[b]), end(node_map[b]),
    std::back_inserter (v));


  for (node i : v){
    cout << i << endl;
  }

  return vector<encounter>(); 
}

void line_sweep::add_encounter(const encounter& enc1){
  vector<encounter> v = pq.insert(enc1);

  for (encounter& e : v)
    delete_encounter(e);

  node s = enc1.get_s(), t = enc1.get_t();
  const edge st = make_pair (s, t);
  const edge ts = make_pair (t, s);

  node_map[s].insert(t);
  node_map[t].insert(s);

  encounter_map[st] = enc1;
  encounter_map[ts] = enc1;
}


void line_sweep::delete_encounter(const encounter& enc1){
  node s = enc1.get_s(), t = enc1.get_t();
  node_map[s].erase(t);
  node_map[t].erase(s);

  assert (encounter_map.find(make_pair(s, t)) != encounter_map.end());
  assert (encounter_map.find(make_pair(t, s)) != encounter_map.end());

  bool t1 = encounter_map.erase(make_pair(s, t));
  bool t2 = encounter_map.erase(make_pair(t, s));

  assert (t1);
  assert (t2);

}

encounter line_sweep::get_encounter(const pair<node, node>& e){
  return encounter_map[e];
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

// -=====-

bool delete_encounter_in_vector (vector<encounter>& v, const encounter& e){
  for (int i=0; i<v.size(); i++){
    if (v[i] == e){

      if (DEBUG)
        cout << "[vector] Deleting: " << e << endl;

      iter_swap(v.begin() + i, v.begin()+v.size()-1); 
      v.pop_back();
      return true;
    }
  }
  return false;
}