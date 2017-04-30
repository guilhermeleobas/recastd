#include "encounter.hpp"
#include "graph.hpp"
#include "methods.hpp" // pqueue
#include <iostream>
#include <map>
#include <unordered_set>

using namespace std;

typedef unsigned uint;

map<node, unordered_set<node>> node_map;
map<edge, unordered_set<encounter, encounter::hash>> encounter_map;


void print_node_map() {
  for (auto&& it : node_map){
    cout << it.first << ": ";
    for (auto&& i : it.second)
      cout << i << ' ';
    cout << endl;
  }
}

void print_encounter_map(){
  for (auto&& it : encounter_map){
    cout << it.first.first << " <-> " << it.first.second << ": \n";
    for (auto&& i : it.second)
      cout << '\t' << i << '\n';
    cout << endl;
  }
}

void set_min_max (const vector<encounter>& v){
  for (auto&& e : v){
    min_tf = min (min_tf, e.get_tf());
    min_ti = min (min_ti, e.get_ti());
  }

  cout << "-----\n";
  cout << "min_ti: " << min_ti << endl;
  cout << "min_tf: " << min_tf << endl;
}

void set_day(vector<encounter>& v){
  for (auto&& e : v)
    e.set_day();
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
    
    v.push_back (encounter(s, t, tf, ti, delta));
    
  }

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

map<node, graph> build_graph (){
  // get a list of nodes
  vector<node> nodes;

  for_each (begin(node_map), end(node_map),
    [&nodes] (const pair<node, unordered_set<node>> &i) mutable {
      nodes.push_back(i.first);
    });

  map<node, graph> graphs;

  for (node n : nodes){
    // for every node a: create a graph
    graphs[n] = graph(n);
  }

  for (node a : nodes){
    // iterate over every neighbor a -> b
    for (node b : node_map[a]){
      // iterate over every encounter in encounter_map[edge(a, b)]
      for (auto&& enc : encounter_map[make_pair(a, b)]){
        // add encounters to the graph g(a);
        // graphs[b] will add this same edge in a future iteration 
        graphs[a].add_edge(enc);
      }
    }
    cout << endl;
  }

  for (node a : nodes){
    for (node b : node_map[a]){
      for (node c : node_map[b]){
        // unordered_set<encounter, encounter::hash>...
        auto us0 = encounter_map[make_pair(a, b)];
        auto us1 = encounter_map[make_pair(b, c)];
        auto us2 = encounter_map[make_pair(c, a)];

        match_encounters (a, b, c, graphs, us0, us1, us2);
      }
    }
    
  }

  for (auto&& it : graphs){
    cout << "dumping graph for node: " << it.first << endl;
    it.second.dump();
    cout << endl;
  }

  return graphs;

}

void map_nodes (const vector<encounter> &v){

  // edges are undirected
  for (auto&& e : v){
    node_map[e.get_s()].insert (e.get_t());
    node_map[e.get_t()].insert (e.get_s());
  }

}

void map_encounters (const vector<encounter> &v){
  
  // edges are undirected
  for (auto&& e : v){
    uint s = e.get_s(), t = e.get_t();
    encounter_map[make_pair(s, t)].insert(e);
    encounter_map[make_pair(t, s)].insert(e);
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

  pqueue pq;

  pq.insert (encounter (1, 2, 12, 0, 12));
  pq.insert (encounter (10, 20, 12, 0, 12));

  pq.insert (encounter (2, 3, 3, 1, 2));
  pq.insert (encounter (2, 3, 5, 4, 1));
  pq.insert (encounter (2, 3, 8, 6, 2));
  pq.insert (encounter (1, 3, 10, 7, 3));
  pq.insert (encounter (2, 3, 11, 9, 2));


  pq.print();


  
  // vector<encounter> v = read_encounters();
  // map_nodes(v);
  // map_encounters(v);

  // sort(v.begin(), v.end(), 
  //   [](const encounter& e1, const encounter& e2) mutable -> bool {
  //     if (e1.get_ti() == e2.get_ti())
  //       return e1.get_tf() < e2.get_tf();
  //     return e1.get_ti() < e2.get_ti();
  //   });

  // map<int, int> m;

  // for (int i=0; i<v.size(); i++){
  //   vector<encounter>::iterator it = upper_bound(v.begin()+i+1, v.end(), v[i], 
  //     [] (const encounter& e1, const encounter& e2) -> bool {
  //       return e1.get_tf() < e2.get_ti();
  //     });
  //     cout << v[i] << ' ' << it - v.begin() - i - 1 << endl;
  // }

  // for (auto it : m){
  //   cout << it.first << ' ' << it.second << endl;
  // }

  // vector<encounter> v;

  // v.push_back (encounter (1, 2, 4, 3, 1));
  // v.push_back (encounter (1, 2, 6, 3, 3));
  
  // v.push_back (encounter (1, 3, 10, 2, 8));
  // v.push_back (encounter (3, 4, 8, 5, 3));
  // v.push_back (encounter (4, 1, 6, 4, 2));

  // set_min_max(v);
  // set_day(v);

  // map_nodes(v);
  // map_encounters(v);

  // cout << "------\n";
  // print_node_map();
  // cout << "------\n";
  // print_encounter_map();
  // cout << "------\n";
  // build_graph();


  // graph g (1);

  // g.add_edge (v[0]);
  // g.add_edge (v[1]);

  // g.add_edge (v[2], v[3], v[4]);


  // g.dump();


  return 0;
}
