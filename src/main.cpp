#include "debug.hpp"
#include "encounter.hpp"
#include "graph.hpp"
#include "methods.hpp" // pqueue
#include <iostream>
#include <map>
#include <unordered_set>

using namespace std;

typedef unsigned uint;

void set_min_max (const vector<encounter>& v){
  // for (auto&& e : v){
  //   min_tf = min (min_tf, e.get_tf());
  //   min_ti = min (min_ti, e.get_ti());
  // }

  min_ti = v[0].get_ti();
  min_tf = v[0].get_tf();

  cout << "-----\n";
  cout << "min_ti: " << min_ti << endl;
  cout << "min_tf: " << min_tf << endl;
}

void set_day(vector<encounter>& v){
  for (auto&& e : v)
    e.set_day();
}

vector<encounter> merge_encounters (vector<encounter>& v){
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
    if (e0.get_s() == v[i].get_s() and 
        e0.get_t() == v[i].get_t() and
        e0.get_tf() == v[i].get_ti()){
      // can merge
      if (DEBUG)
        cout << "[merge] prev: " << e0 << endl;

      e0.set_tf(v[i].get_tf());
      novo.pop_back();
      novo.push_back (e0);

      if (DEBUG)
        cout << "[merge] novo: " << novo.back() << endl;
    }
    else{
      novo.push_back (v[i]);
    }
  }

  return novo;
}

vector<encounter> read_encounters (){
  vector<encounter> v;

  ios::sync_with_stdio(false);
  cin.tie(NULL);
  
  while(true){
    uint s, t, tf, ti, delta;
    cin >> s >> t >> tf >> ti >> delta;
    if (not cin)
      break;

    if (s > t)
      swap (s, t);
    
    v.push_back (encounter(s, t, tf, ti, delta));
    
  }

  v = merge_encounters(v);

  sort(v.begin(), v.end(), 
  [](const encounter& e1, const encounter& e2) mutable -> bool {
    if (e1.get_ti() == e2.get_ti())
      return e1.get_tf() < e2.get_tf();
    return e1.get_ti() < e2.get_ti();
  });

  set_min_max(v);
  set_day(v);
  
  return v;
}

bool contact (const encounter& enc0, const encounter& enc1){
  if (enc0.get_tf() >= enc1.get_tf()){
    if (enc0.get_ti() <= enc1.get_tf())
      return true;
  }
  else {
    if (enc1.get_ti() <= enc0.get_tf())
      return true;
  }
  return false;
}

bool contact (const encounter& enc0, const encounter& enc1, const encounter& enc2){
  uint s0 = enc0.get_s(), s1 = enc1.get_s(), s2 = enc2.get_s();
  uint t0 = enc0.get_t(), t1 = enc1.get_t(), t2 = enc2.get_t();
  
  bool match = false;

  // s0 -> t0/s1 -> t1/s2 -> t2/s0;
  if (t0 == s1 and t1 == s2 and t2 == s0)
    match = true;

  // s0 -> t0/s1 -> t1/t2 -> s2/s0;
  if (t0 == s1 and t1 == t2 and s2 == s0)
    match = true;

  // s0 -> t0/t1 -> s1/t2 -> s2/s0;
  if (t0 == t1 and s1 == s2 and t2 == s0)
    match = true;

  // s0 -> t0/t1 -> s1/s2 -> t2/s0;
  if (t0 == t1 and s1 == t2 and s2 == s0)
    match = true;

  // t0 -> s0/s1 -> t1/s2 -> t2/t0;
  if (s0 == s1 and t1 == s2 and t2 == t0)
    match = true;

  // t0 -> s0/s1 -> t1/t2 -> s2/t0;
  if (s0 == s1 and t1 == t2 and s2 == t0)
    match = true;

  // t0 -> s0/t1 -> s1/s2 -> t2/t0;
  if (s0 == t1 and s1 == s2 and t2 == t0)
    match = true;

  // t0 -> s0/t1 -> s1/t2 -> s2/t0;
  if (s0 == t1 and s1 == t2 and s2 == t0)
    match = true;

  if (!match)
    return false;

  // here I know that there is a cycle in the graph;
  // now I need to check if that cycle happens in the same time
  if (contact(enc0, enc1) and   // p0 meets p1
      contact(enc0, enc2) and   // p0 meets p2
      contact(enc1, enc2))      // p1 meets p2
    return true;
  
  return false;
}

// this is where the magic happens :)
void match_encounters (
  const node& a, const node& b, const node& c,
  map<node, graph>& graphs,
  const unordered_set<encounter, encounter::hash>& us0,
  const unordered_set<encounter, encounter::hash>& us1,
  const unordered_set<encounter, encounter::hash>& us2){

  for (const encounter& enc0 : us0){
    for (const encounter& enc1 : us1){
      for (const encounter& enc2 : us2){
        cout << "checking contact for " << a << ' ' << b << ' ' << c << endl;
        if (contact(enc0, enc1, enc2)){
          cout << "found encounter for " << a << " <-> " << b << " <-> " << c << endl;
          graphs[a].add_edge (enc0);
          graphs[a].add_edge (enc1);
          graphs[a].add_edge (enc2);

          graphs[b].add_edge (enc0);
          graphs[b].add_edge (enc1);
          graphs[b].add_edge (enc2);

          graphs[c].add_edge (enc0);
          graphs[c].add_edge (enc1);
          graphs[c].add_edge (enc2);
        }
      }
    }
  }
}


int main (int argc, char* argv[]){

  ios::sync_with_stdio(false);
  cin.tie(0);

  cout << "-----\n";
  
  if (argc == 1){
    timestep = DEFAULT_TIMESTEP;
    cout << "setting timestep to: " << DEFAULT_TIMESTEP << endl;
  }
  else if (argc == 3 and 
      (string(argv[1]) == "-t" || string(argv[1]) == "--timestep")){
    timestep = atoi(argv[2]);
    cout << "setting timestep to: " << timestep << endl;
  }
  else {
    cerr << "USAGE: \n";
    cerr << "./recast -t timestep < input\n";
    cerr << "./recast < input\n";
    exit(1);
  }

  // pqueue pq;

  // pq.insert (encounter (1, 2, 12, 0, 12));
  // pq.insert (encounter (10, 20, 12, 0, 12));

  // pq.insert (encounter (2, 3, 3, 1, 2));
  // pq.insert (encounter (2, 3, 5, 4, 1));
  // pq.insert (encounter (2, 3, 8, 6, 2));
  // pq.insert (encounter (1, 3, 10, 7, 3));
  // pq.insert (encounter (2, 3, 11, 9, 2));

  
  vector<encounter> v = read_encounters();



  line_sweep ls;

  for (auto&& e : v){
    ls.add_encounter(e);
  }



  return 0;
}
