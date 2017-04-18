#include "encounter.hpp"
#include "simple_graph.hpp"
#include <iostream>

using namespace std;

void find_encounters (const simple_graph& g){
  // for each encounter
}

int main (){
  
  simple_graph g(2000);
  vector<encounter> v;
  
  ios::sync_with_stdio(false);
  cin.tie(NULL);
  
  while (true){
    unsigned s, t, tf, ti, delta;
    
    cin >> s >> t >> tf >> ti >> delta;
    
    if (not cin)
      break;
    
    g[s].push_back (encounter (s, t, tf, ti));
    g[t].push_back (encounter (t, s, tf, ti));
    v.push_back (encounter (s, t, tf, ti));
  }

  return 0;
}
