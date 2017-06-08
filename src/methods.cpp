#include "debug.hpp"
#include "methods.hpp"

#include <vector>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;

uint max_nodes = 0;
uint max_days = 0;

void set_properties(vector<encounter>& v){
  merge_encounters(v);
  sort_encounters(v);

  set_min_max(v);
  set_day(v);
  set_index(v);
}

void set_index(vector<encounter> &v){
  for (int i=0; i<v.size(); i++)
    v[i].set_index(i);
}

void merge_encounters (vector<encounter>& v){
  sort (v.begin(), v.end(),
  [](const encounter& e1, const encounter& e2) {
    if (e1.get_s() == e2.get_s() and e1.get_t() == e2.get_t() and e1.get_ti() == e2.get_ti())
      return e1.get_tf() < e2.get_tf();

    if (e1.get_s() == e2.get_s() and e1.get_t() == e2.get_t())
      return e1.get_ti() < e2.get_ti();

    if (e1.get_s() == e2.get_s())
      return e1.get_t() < e2.get_t();

    return e1.get_s() < e2.get_s();
  });

  set<uint> s;
  for (const encounter &e : v){
    s.insert(e.get_s());
    s.insert(e.get_t());
  }
  max_nodes = s.size();

  vector<encounter> novo;
  novo.push_back (v[0]);

  for (uint i=1; i<v.size(); i++){
    encounter e0 = novo.back();
    if (can_merge(e0, v[i])){
      // can merge
      if (DEBUG)
        cout << "[merge] prev: " << e0 << endl;

      novo.pop_back();
      novo.push_back (merge(e0, v[i]));

      if (DEBUG)
        cout << "[merge] novo: " << novo.back() << endl;
    }
    else{
      novo.push_back (v[i]);
    }
  }

  v = novo;
}

void sort_encounters (vector<encounter>& v){
  sort (v.begin(), v.end(),
  
  [](const encounter &e1, const encounter &e2) {
    if (e1.get_ti() == e2.get_ti())
      return e1.get_tf() < e2.get_tf();
    return e1.get_ti() < e2.get_ti();
  });
}

void set_min_max (const vector<encounter>& v){
  min_ti = v[0].get_ti();
  min_tf = v[0].get_tf();

  if (DEBUG){
    cout << "-----\n";
    cout << "min_ti: " << min_ti << endl;
    cout << "min_tf: " << min_tf << endl;
  }
}

void set_day(vector<encounter>& v){
  for (auto&& e : v){
    e.set_day();
    max_days = (max_days < e.get_max_day()) ? e.get_max_day() : max_days; 
  }
}


