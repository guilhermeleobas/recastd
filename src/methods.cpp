#include "debug.hpp"
#include "methods.hpp"

#include <vector>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;

void set_properties(vector<encounter>& v){
  merge_encounters(v);
  sort_encounters(v);

  set_min_max(v);
  set_day(v);
}

void merge_encounters (vector<encounter>& v){
  sort (v.begin(), v.end(),
  [](const encounter& e1, const encounter& e2) mutable -> bool {
    if (e1.get_s() == e2.get_s() and e1.get_t() == e2.get_t() and e1.get_ti() == e2.get_ti())
      return e1.get_tf() < e2.get_tf();

    if (e1.get_s() == e2.get_s() and e1.get_t() == e2.get_t())
      return e1.get_ti() < e2.get_ti();

    if (e1.get_s() == e2.get_s())
      return e1.get_t() < e2.get_t();

    return e1.get_s() < e2.get_s();
  });

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
  
  [](const encounter& e1, const encounter& e2) mutable -> bool {
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
  for (auto&& e : v)
    e.set_day();
}


