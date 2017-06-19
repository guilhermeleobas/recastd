#include "debug.hpp"
#include "pqueue.hpp"

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
        cout << "\t[Pqueue] removeu: " << topo << endl;

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
    cout << "\t[Pqueue] adicionando: " << e1 << endl;

  vector<encounter> v = this->remove_unvalid_intervals(e1);
  multiset::insert(e1);
  return v;
}

void pqueue::print() const {
  for (auto&& it : *this){
    cout << it << '\n';
  }
}
