#include "debug.hpp"
#include "encounter.hpp"
#include "methods.hpp"

#include <iostream>
#include <map>
#include <unordered_set>
#include <memory>
#include <sys/stat.h>
#include <string>
#include <algorithm>

using namespace std;

typedef unsigned uint;

vector<encounter> read_encounters() {
  vector<encounter> v;

  cin.tie(NULL);
  cin.sync_with_stdio(false);

  while (true) {
    uint s, t;
    uint64_t tf, ti, delta;
    cin >> s >> t >> tf >> ti >> delta;
    if (not cin) break;
    if (DEBUG)
      cout << "read: " << s << ' ' << t << ' ' << tf << ' ' << ti << ' '
           << delta << '\n';

    if (s > t) swap(s, t);

    v.push_back(encounter(s, t, tf, ti, delta));
  }

  return v;
}

int main(int argc, char *argv[]) {

  ios::sync_with_stdio(false);
  cin.tie(0);

  vector<encounter> v = read_encounters();
  set_properties(v);
  
  vector<encounter> encs;
  
  map<int, int> map_nodes;
  map<edge, vector<encounter>> map_encounters;
  
  for (encounter &e : v){
    if (e.get_max_day() > 28) continue;
    
    map_encounters[e.get_edge()].push_back (e); 
  }
  
  v.clear();
  
  for (auto &it : map_encounters){
    
    for (uint i=0; i<it.second.size(); i++){
      for (uint j=i+1; j<it.second.size(); j++){
        if (i == j) continue;
        if (can_merge_no_restriction(it.second[i], it.second[j])){
          encounter e = merge(it.second[i], it.second[j]);
          it.second[i] = e;
          // cout << "juntou e deletou: " << j << ' ' << it.second.size() << endl;
          it.second.erase(it.second.begin() + j);
          j--;
        }
      }
    }
    
  }
  
  for (auto &it : map_encounters){
    for (encounter &e : it.second){
      if (map_nodes.find(e.get_s()) == map_nodes.end())
        map_nodes[e.get_s()] = map_nodes.size();

      if (map_nodes.find(e.get_t()) == map_nodes.end())
        map_nodes[e.get_t()] = map_nodes.size();

      e.set_s(map_nodes[e.get_s()]);
      e.set_t(map_nodes[e.get_t()]);

      encs.push_back (e);
    }
  }
  
  for (const encounter &e : encs){
    cout << e.get_s() << ' ' << e.get_t() << ' ' << e.get_tf() << ' ' << e.get_ti() << ' ' << e.get_delta() << '\n';
  }

  return 0;
}
