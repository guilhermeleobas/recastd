#include "debug.hpp"
#include "graph.hpp"

#include <cassert>
#include <queue>
#include <iostream>

graph::graph(){ }

graph::graph(const unsigned person){
  // g[person] = unordered_set<encounter, encounter::hash>();
  this->person = person;
}


void graph::add_edge (const encounter& enc1){
  if (DEBUG)
    cout << "[Graph: " << person << "] Adding:\t" << enc1 << endl;

  g.insert(enc1);
}

void graph::add_edge (const reference_wrapper<const encounter>& enc0, const reference_wrapper<const encounter>& enc1, const reference_wrapper<const encounter>& enc2){
  // a -> b => enc0
  // b -> c => enc1
  // c -> a => enc2
  if (DEBUG){
    cout << "[Graph: " << person << " clique]: " << endl;
    cout << '\t' << enc0.get() << '\n';
    cout << '\t' << enc1.get() << '\n';
    cout << '\t' << enc2.get() << '\n';
    cout << '\n';
  }

  g.insert(enc0);
  g.insert(enc1);
  g.insert(enc2);
}

void graph::dump (ofstream& f) {
  
  for (const reference_wrapper<const encounter>& e : g){
    f << e.get() << '\n';
  }
  f.close();
}
