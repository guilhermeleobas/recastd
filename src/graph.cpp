#include "debug.hpp"
#include "graph.hpp"

#include <cassert>
#include <queue>
#include <iostream>
#include "methods.hpp"

#include "roaring.c"

inline int calc_node(const int pos) { return pos / max_nodes; }

inline int calc_day(const int pos) { return pos % max_nodes; }

graph::graph() {}

graph::graph(const uint person, const uint max_days, const uint max_nodes) {
  // g[person] = unordered_set<encounter, encounter::hash>();
  this->person = person;
  gg = vector<Roaring>(max_nodes + 1);
  for (auto &&r : gg) {
    r.setCopyOnWrite(true);
  }

  latest = 0;
}

void graph::set_person(const node person) { this->person = person; }

void graph::add_encounter(const encounter &e) {
  if (DEBUG) cout << "[Graph: " << person << "] Adding: " << e << endl;

  for (uint day = e.get_min_day(); day <= e.get_max_day(); day++) {

    node n = (e.get_s() == person) ? e.get_t() : e.get_s();
    gg[person].add(max_nodes * n + day);
    gg[n].add(max_nodes*person + day);

    if (DEBUG) {
      cout << "\t[Row: " << person << "] Set: " << max_nodes *n + day << ' '
           << " node: " << n << " day: " << day << '\n';
    }
  }

  if (DEBUG) cout << '\n';
}

void graph::add_encounter(const encounter &e0, const encounter &e1,
                          const encounter &e2) {
  // a -> b => enc0
  // b -> c => enc1
  // c -> a => enc2
  if (DEBUG) {
    cout << "[Graph: " << person << " clique]: " << endl;
    cout << '\t' << e0 << '\n';
    cout << '\t' << e1 << '\n';
    cout << '\t' << e2 << '\n';
    cout << '\n';
  }

  add_encounter(e0);
  add_encounter(e1);
  add_encounter(e2);
}

void graph::dump(ofstream &f) {
  vector<int> v;
  for (uint i = 0; i < max_nodes; i++) {
    if (gg[i].cardinality() == 0) continue;

    vector<encounter> v;
    
    for (auto bs : gg[i]){
      int s = i;
      int t = calc_node(bs);
      int day_i = calc_day(bs);
      int day_f = day_i;
      int tf = timestep * (day_f + 1) - 1;
      int ti = timestep * day_i;
      int delta = tf - ti;

      v.push_back(encounter(s, t, tf, ti, delta, day_i, day_f));
    }

    merge_encounters(v);

    for (const encounter &e : v)
      f << e << '\n';
  }
}

node graph::get_latest() const { return this->latest; }

node graph::get_person() const { return this->person; }

void graph::set_latest(const node n) { this->latest = n; }

void merge_graphs(graph &g1, graph &g2, bool share_graph, ofstream &f) {
  if (g1.get_latest() == g2.get_person() &&
      g2.get_latest() == g1.get_person()) {
    // cout << "chegou aqui\n";
    return;
  }

  node pa = g1.get_person();
  node pb = g2.get_person();

  if (share_graph){
    uint64_t len_g1 = 0, len_g2 = 0;
    
    for (uint i = 0; i < max_nodes; i++) {
      len_g1 += g1[i].cardinality();
      len_g2 += g2[i].cardinality();
      g1[i] |= g2[i];
    }
    g2 = g1;
    f << len_g1 << '\n' << len_g2 << '\n';
    g2.set_person(pb);
  }
  else{
    // share neighbors
    f << g2[g2.get_person()].cardinality() << ' ' << g1[g1.get_person()].cardinality() << '\n';
    g1[g2.get_person()] |= g2[g2.get_person()];
    g2[g1.get_person()] |= g1[g1.get_person()];
  }


  g1.set_latest(pb);
  g2.set_latest(pa);
}

// graph merge_graphs(const graph &g1, const graph &g2, const graph &g3) {
//   graph g = g1;
//   for (uint i = 0; i < max_nodes; i++) {
//     g[i] |= g2[i];
//     g[i] |= g3[i];
//   }
//   return g;
// }
