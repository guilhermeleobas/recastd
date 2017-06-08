#include "debug.hpp"
#include "graph.hpp"

#include <cassert>
#include <queue>
#include <iostream>
#include "methods.hpp"

graph::graph() {}

graph::graph(const uint person, const uint max_days, const uint max_nodes) {
  // g[person] = unordered_set<encounter, encounter::hash>();
  this->person = person;
  gg = vector<boost::dynamic_bitset<>>(max_nodes + 1,
                                       boost::dynamic_bitset<>(max_days + 1));
}

void graph::add_encounter(const encounter &e) {
  if (DEBUG) cout << "[Graph: " << person << "] Adding:\t" << e << endl;

  for (uint day = e.get_min_day(); day <= e.get_max_day(); day++) {
    node n = (e.get_s() != person) ? e.get_s() : e.get_t();
    gg[n][day] = 1;
    if (DEBUG)
      cout << "[Graph: " << person << "] Set:\t" << gg[n][day] << '\t' << e
           << endl;
  }
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

  for (uint i = 0; i < max_nodes; i++) {
    if (!gg[i].any()) continue;

    int day = -1;
    for (uint j = 1; j < max_days; j++) {

      if (day == -1 && gg[i][j]) day = j;

      if ((!gg[i][j] && day != -1)or(day != -1 and j == max_days - 1)) {
        f << person << ' ' << i << ' ' << (timestep * (j)) - 1 << ' '
          << (timestep * day) << ' ' << timestep - 1 << ' ' << day << ' '
          << j - 1 << '\n';
        day = -1;
      }

      // if (gg[i][j])
      //   f << person << ' ' << p2 << ' ' << (timestep * (day + 1)) - 1 << ' '
      //     << (timestep * day) << ' ' << timestep - 1 << ' ' << day << ' ' <<
      // day
      //     << '\n';
    }
  }

  // for (const uint& n : gg) {
  //   // f << e << '\n';
  //   v[n].print(f, person);
  // }
  // f.close();
}

graph merge_graphs(const graph &g1, const graph &g2) {
  graph g = g1;
  for (int i = 0; i < max_days; i++) {
    g[i] |= g2[i];
  }
  return g;
}

graph merge_graphs(const graph &g1, const graph &g2, const graph &g3) {
  graph g = g1;
  for (int i = 0; i < max_days; i++) {
    g[i] |= g2[i];
    g[i] |= g3[i];
  }
  return g;
}
