#include "debug.hpp"
#include "graph.hpp"

#include <cassert>
#include <queue>
#include <iostream>

graph::graph(){
  this->_size = 1;
}

graph::graph(const unsigned person){
  g[person] = unordered_set<encounter, encounter::hash>();
  this->person = person;
  this->_size = 1;
}


void graph::add_edge (const encounter& enc1){
  if (DEBUG)
    cout << "[Graph: " << person << "]:\t" << enc1 << endl;
  
  uint s = enc1.get_s(), t = enc1.get_t();

  g[s].insert(enc1);
  g[t].insert(enc1);
}

void graph::add_edge (const encounter& enc0, const encounter& enc1, const encounter& enc2){
  // a -> b => enc0
  // b -> c => enc1
  // c -> a => enc2
  if (DEBUG){
    cout << "[Graph: " << person << "]: " << endl;
    cout << '\t' << enc0 << '\n';
    cout << '\t' << enc1 << '\n';
    cout << '\t' << enc2 << '\n';
    cout << '\n';
  }

  g[enc0.get_s()].insert (enc0);
  g[enc0.get_t()].insert (enc0);

  g[enc1.get_s()].insert (enc1);
  g[enc1.get_t()].insert (enc1);

  g[enc2.get_s()].insert (enc2);
  g[enc2.get_t()].insert (enc2);
}

void graph::dump (ofstream& f) {
  queue<node> q;
  map<node, bool> visited; // default value for bool is guaranteed to be false;
  unordered_set<encounter, encounter::hash> s;

  q.push (this->person);

  while (not q.empty()){
    node n = q.front();
    q.pop();

    if (visited.find(n) != visited.end()){
      continue;
    }

    visited[n] = true;

    // f << "node: " << n << " size: " << g[n].size() << endl;

    for (const encounter enc :  g[n]){
      if (s.find(enc) != s.end())
        continue;

      s.insert(enc);
      // n is the head of enc
      // add all unvisited tail nodes to the queue
      node tail = enc.get_s() != n ? enc.get_s() : enc.get_t();
      if (visited.find(tail) == visited.end())
        q.push(tail);
      
      f << enc << '\n';
    }

  }
}
