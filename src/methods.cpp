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
  calc_t(v);
  subtract_min_ti(v);
  calc_t(v);
  set_day(v);
  calc_max_nodes(v);
  merge_encounters(v);
  sort_encounters(v);
}

void calc_t(const vector<encounter>&v){
  for (const encounter& e : v){
    min_ti = min(min_ti, e.get_ti());
    min_tf = min(min_tf, e.get_tf());
  }
  
  cout << "[INFO] min_ti: " << min_ti << '\n';
  cout << "[INFO] min_tf: " << min_tf << '\n';
}

void subtract_min_ti (vector<encounter>& v){
  
  for (encounter &e : v){
    e.set_ti ( e.get_ti() - min_ti );
    e.set_tf ( e.get_tf() - min_ti );
    e.calc_delta();
  }
}

void calc_max_nodes(const vector<encounter>& v){
  set<uint> s;
  for (const encounter &e : v){
    s.insert(e.get_s());
    s.insert(e.get_t());
  }
  max_nodes = s.size();
  max_nodes += 1;
  cout << "[INFO] max_nodes: " << max_nodes << '\n';
}

void merge_encounters(vector<encounter>& v) {
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

  vector<encounter> novo;
  novo.push_back (v[0]);

  for (uint i=1; i<v.size(); i++){
    encounter e0 = novo.back();
    if (can_merge(e0, v[i])){
      // can merge
      if (DEBUG){
        cout << "[merge] prev:\n\t" << e0 << "\n\t" << v[i] << '\n';
      }

      novo.pop_back();
      novo.push_back (merge(e0, v[i]));

      if (DEBUG)
        cout << "[merge] novo: " << novo.back() << "\n\n";
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
    return e1.get_ti() < e2.get_ti();
  });
}

void set_day(vector<encounter>& v){
  for (auto&& e : v){
    e.calc_day();
    // if (e.get_max_day() > 29){
      // cout << "maior " << e << '\n';
    // }
    max_days = (max_days < e.get_max_day()) ? e.get_max_day() : max_days; 
  }
  
  max_days += 1;
  
  cout << "[INFO] max days: " << max_days << '\n';
}


