#include "methods.hpp"

#include <iostream>
#include <vector>
#include <algorithm> //std::set_intersection

using std::cout;
using std::endl;
using std::vector;
using std::set_intersection;

bool orderByTf::operator()(const encounter &e1, const encounter &e2) const{
  return e1.get_tf() < e2.get_tf();
}

void pqueue::remove_unvalid_intervals (const encounter& e1){
  while (not this->empty()){
    const encounter& topo = *(this->begin());

    if (topo.get_tf() < e1.get_ti()){
      cout << "\tremoveu: " << topo << endl;
      this->erase(this->begin());
    }
    else{
      break;
    }
  }
}

pqueue::iterator pqueue::insert (const pqueue::value_type& e1){
  cout << "adicionando: " << e1 << endl;
  this->remove_unvalid_intervals(e1);
  return multiset::insert(e1);
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
  node s = enc1.get_s(), t = enc1.get_t();

  node_map[s].insert(t);
  node_map[t].insert(s);

  encounter_map[make_pair(s, t)].push_back (enc1);
  encounter_map[make_pair(t, s)].push_back (enc1);
}

void line_sweep::delete_encounter(const encounter& enc1){
  node s = enc1.get_s(), t = enc1.get_t();
  node_map[s].erase(t);
  node_map[t].erase(s);

  // encounter_map[make_pair(s, t)].erase(enc1);
  // encounter_map[make_pair(s, t)].erase(enc1);

}