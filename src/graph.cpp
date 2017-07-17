#include "debug.hpp"
#include "graph.hpp"

#include "small_queue.hpp"

#include <cassert>
#include <queue>
#include <iostream>
#include "methods.hpp"

#include "roaring.c"

inline int calc_node(const int pos) { return floor(pos / max_days); }

inline int calc_day(const int pos) { return pos % max_days; }

graph::graph() {}

graph::graph(const uint ego, const uint max_days, const uint max_nodes) {
  this->ego = ego;
  gg = vector<Roaring>(max_nodes);
  for (auto &&r : gg) {
    r.setCopyOnWrite(true);
  }

  latest = 0;
}

graph::graph(const uint ego, const uint max_days, const uint max_nodes,
             const uint max_len) {

  this->ego = ego;
  gg = vector<Roaring>(max_nodes);
  for (auto &&r : gg) {
    r.setCopyOnWrite(true);
  }

  latest = 0;
  // graph(ego, max_days, max_nodes);
  sq = small_queue<encounter>(max_len);
}

void graph::add_encounter_to_queue(const encounter &e) { this->sq.push(e); }

void graph::set_ego(const node ego) { this->ego = ego; }

void graph::add_encounter(const encounter &e) {
  if (DEBUG) cout << "[Graph: " << this->ego << "] Adding: " << e << endl;

  for (uint day = e.get_min_day(); day <= e.get_max_day(); day++) {

    node s = e.get_s(), t = e.get_t();

    // node n = (e.get_s() == ego) ? e.get_t() : e.get_s();
    gg[s].add(max_days * t + day);
    gg[t].add(max_days * s + day);

    if (DEBUG) {
      cout << "\t[Graph: " << this->ego << '\n';
      cout << "\t\tRow: " << s << "] Set: " << max_days * t + day << ' '
           << " node: " << t << " day: " << day << " max_days: " << max_days << '\n';
      cout << "\t\tRow: " << t << "] Set: " << max_days * s + day << ' '
           << " node: " << s << " day: " << day << " max_days: " << max_days << '\n';
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
    cout << "[Graph: " << ego << " clique]: " << endl;
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

    for (auto bs : gg[i]) {
      int s = i;
      int t = calc_node(bs);
      int day_i = calc_day(bs);
      int day_f = day_i;
      int tf = timestep * (day_f + 1) - 1;
      int ti = timestep * day_i;
      int delta = tf - ti;

      if (DEBUG)
        cout << "cout " << encounter(s, t, tf, ti, delta, day_i, day_f)
             << " with " << bs << " max_days: " << max_days << '\n';

      v.push_back(encounter(s, t, tf, ti, delta, day_i, day_f));
    }

    merge_encounters(v);

    for (const encounter &e : v) f << e << '\n';
  }
}

node graph::get_latest() const { return this->latest; }

node graph::get_ego() const { return this->ego; }

void graph::set_latest(const node n) { this->latest = n; }

const small_queue<encounter> &graph::get_small_queue() const {
  return this->sq;
}
small_queue<encounter> &graph::get_small_queue() { return this->sq; }

// last k encounters strategy
void merge_graphs_share_last_k(graph &g1, graph &g2, string share_graph,
                               ofstream &f) {
  assert(share_graph == "lastk");

  if (g1.get_latest() == g2.get_ego() && g2.get_latest() == g1.get_ego()) {
    // cout << "chegou aqui\n";
    return;
  }

  node pa = g1.get_ego();
  node pb = g2.get_ego();

  small_queue<encounter> &qa = g1.get_small_queue();
  small_queue<encounter> &qb = g2.get_small_queue();

  // cout << "size qa: " << qa.size() << '\n';
  // cout << "size qb: " << qb.size() << '\n';

  for (const encounter &e : qa) {
    // cout << "chegou aqui1\n";
    g2.add_encounter(e);
  }

  for (const encounter &e : qb) {
    // cout << "chegou aqui2\n";
    g1.add_encounter(e);
  }

  for (uint i = 0; i < qa.size() && qb.size() < qb.get_max_len(); i++) {
    encounter e = qa[i];
    // cout << "Pushing " << e << '\n';
    qb.push(e);
  }

  for (uint i = 0; i < qb.size() && qa.size() < qa.get_max_len(); i++) {
    encounter e = qb[i];
    qa.push(e);
  }

  // cout << qa.size() << ' ' << qb.size() << '\n';
  
  // cout << "passou\n";
  
  g1.set_latest(pb);
  g2.set_latest(pa);
}

void merge_graphs_share_graph(graph &g1, graph &g2, string share_graph,
                              ofstream &f) {
  assert(share_graph == "graph");

  if (g1.get_latest() == g2.get_ego() && g2.get_latest() == g1.get_ego()) {
    // cout << "chegou aqui\n";
    return;
  }

  node pa = g1.get_ego();
  node pb = g2.get_ego();

  uint64_t len_g1 = 0, len_g2 = 0;

  for (uint i = 0; i < max_nodes; i++) {
    len_g1 += g1[i].cardinality();
    len_g2 += g2[i].cardinality();
    g1[i] |= g2[i];
  }
  g2 = g1;
  f << len_g1 << '\n' << len_g2 << '\n';
  g2.set_ego(pb);

  g1.set_latest(pb);
  g2.set_latest(pa);
}

void merge_graphs_share_neighbors(graph &g1, graph &g2, string share_graph,
                                  ofstream &f) {
  assert(share_graph == "neighbors");

  if (g1.get_latest() == g2.get_ego() && g2.get_latest() == g1.get_ego()) {
    // cout << "chegou aqui\n";
    return;
  }

  node pa = g1.get_ego();
  node pb = g2.get_ego();

  // share neighbors
  f << g2[g2.get_ego()].cardinality() << ' ' << g1[g1.get_ego()].cardinality()
    << '\n';
  g1[g2.get_ego()] |= g2[g2.get_ego()];
  g2[g1.get_ego()] |= g1[g1.get_ego()];

  g1.set_latest(pb);
  g2.set_latest(pa);
}
