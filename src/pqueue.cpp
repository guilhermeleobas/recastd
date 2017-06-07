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

bool orderByTf::operator()(const weak_ptr<encounter> &e1, const weak_ptr<encounter> &e2) const{
  return e1.lock()->get_tf() < e2.lock()->get_tf();
}

vector<weak_ptr<encounter>> pqueue::remove_unvalid_intervals (const weak_ptr<encounter>& e1){
  vector<weak_ptr<encounter>> v;
  while (not this->empty()){
    const weak_ptr<encounter>& topo = *(this->begin());

    if (topo.lock()->get_tf() < e1.lock()->get_ti()){
      if (DEBUG)
        cout << "\t[Pqueue] removeu: " << *(topo.lock()) << endl;

      v.push_back (topo);

      this->erase(this->begin());
    }
    else{
      break;
    }
  }
  return v;
}

vector<weak_ptr<encounter>> pqueue::insert (const pqueue::value_type& e1){
  if (DEBUG)
    cout << "\t[Pqueue] adicionando: " << *(e1.lock()) << endl;

  vector<weak_ptr<encounter>> v = this->remove_unvalid_intervals(e1);
  multiset::insert(e1);
  return v;
}

void pqueue::print() const {
  for (auto&& it : *this){
    cout << *(it.lock()) << '\n';
  }
}
