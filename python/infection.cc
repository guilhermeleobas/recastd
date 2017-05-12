#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <set>
#include <map>
#include <string>
#include <cstdlib>
#include <queue>
#include <stack>
#include <fstream>

using namespace std;

typedef vector<vector<int> > graph;
typedef pair<int, int> edge;

struct tempo{
  int ti, tf, delta;

  tempo (int ti, int tf, int delta): ti(ti), tf(tf), delta(delta)
  {}

  tempo (): ti(0), tf(0), delta(0){}

  void clone(const tempo& other){
    this->ti = other.ti;
    this->tf = other.tf;
    this->delta = other.delta;
  }

  void print () const{
    cout << "tempo: " << ti << ' ' << tf << ' ' << delta << endl;
  }
};

map<edge, vector<tempo> > time_per_edge;

map<int, int> id_v; // map the id to a vertice
map<int, int> v_id; // map the vertice to an id

int discrete (int ti, int timestep = 86400){
  return (ti/timestep);
}

int get_id(int vertex){
  if (id_v.find(vertex) == id_v.end()){
    int id = id_v.size();
    id_v[vertex] = id;
    v_id[id] = vertex;

    // cout << vertex << '/' << id << endl;
  }

  return id_v[vertex];
}

int get_vertex (int id){
  if (v_id.find(id) == v_id.end()){
    cerr << "erro in find id " << id << "\n";
    exit(1);
  }
  return v_id[id];
}

bool intersection (const tempo& actual_time, const tempo& t2){
  return (actual_time.ti <= t2.tf) and (actual_time.tf >= t2.ti);
}

bool is_inside_window (const tempo& prop_window, const tempo& t){
  return (prop_window.ti <= t.ti) and (prop_window.tf >= t.tf);
}

/*
  parameters:
    edge e
    actual time (t1)
    window => window time for the message to propagate

  any time x to be valid must obey the rule 
  window.ti >= x.ti and x.tf <= window.tf

*/
vector<tempo> valid_node (const edge& e, const tempo& actual_time, const tempo& prop_window){
  vector<tempo> v;

  tempo *t_min = new tempo (0x3f3f3f3f, 0x3f3f3f3f, 0);

  // cout << "verificando aresta " << e.first << ' ' << e.second << " --- com tempo " << actual_time.ti << ' ' << actual_time.tf << endl;

  for (auto t : time_per_edge[e]){

    // t.print();

    // time not inside propagation window
    if (!is_inside_window (prop_window, t))
      continue;

    if (!intersection (actual_time, t)){
      continue;
    }

    // cout << "achou um valido com ti = " << t.ti << endl;

    v.push_back (t);

    // valid time
    // if (t_min->ti > t.ti)
    //   t_min->clone(t);
  }

  return v;


  // if (t_min->ti == 0x3f3f3f3f)
  //   return nullptr;
  // else{
  //   // cout << "returnando ti " << t_min->ti << ' ' << t_min->tf << ' ' << t_min->delta << endl;
  //   // cout << &t_min << endl;
  //   return t_min;
  // }

  // for (int i=0; i<=6; i++){
    
  //   if (time_now + i > ctime)
  //     break;

  //   pair<edge, int> p = make_pair(e, time_now+i);

  //   if (time_per_edge.find(p) != time_per_edge.end())
  //     return &time_per_edge[p];
  // }

  // return nullptr;
}

// return a boolean value indicating if source can reach target within a week.
// t_begin => value choosed randomly between [0, 6]
// bfs returns true if source node can reach target within a week (t_begin+7)
// otherwise, returns false
vector<int> bfs (const graph& g, int source, int target, const tempo& prop_window, int timestep = 86400){

  queue<pair<int, tempo> > q;
  vector<bool> visited(g.size(), false);
  vector<int> parent (g.size(), 0);

  for (int i=0; i<parent.size(); i++)
    parent[i] = i;

  tempo t_first = prop_window;

  q.push(make_pair (source, t_first));
  visited[source] = true;

  while (not q.empty()){
    int node = q.front().first;
    tempo actual_time = q.front().second;
    q.pop();

    visited[node] = true;

    if (node == target)
      break;

    for (int i=0; i<g[node].size(); i++){
      int next = g[node][i];

      if (node == source and next == target)
        continue;

      if (visited[next])
        continue;

      // cout << "- TENTANDO node = " << node << " next = " << next << " tempo = " << actual_time.ti << ' ' << actual_time.tf << endl;

      vector<tempo> v = valid_node(make_pair(node, next), actual_time, prop_window);

      if (v.size() == 0)
        continue;

      // cout << "-- visitou valid_node = " << get_vertex(node) << " next = " << get_vertex(next) << endl;
      // cout << "--  tempo " << v->ti << ' ' << v->tf << ' ' << v->delta << endl;
      // cout << endl;

      visited[next] = true;

      parent[next] = node;

      if (next == target)
        break;

      for (auto i: v)
        q.push (make_pair (next, i));
    }
  }

  if (not visited[target])
    return vector<int>();

  vector<int> p;
  int n = target;

  while (parent[n] != source){
    p.push_back (n);
    n = parent[n];
  }

  p.push_back (n);

  p.push_back (source);

  reverse (p.begin(), p.end());

  return p;
}

void print_graph (const graph& g){
  for (int i=0; i<g.size(); i++){
    // cout << i << ": " << g[i].size() << endl;
    cout << get_vertex(i) << ": ";
    for (int j=0; j<g[i].size(); j++){
      cout << get_vertex(g[i][j]) << ' ';
    }

    cout << endl;
  }
  cout << endl;
}

void print_map (){
  // for (auto it = time_per_edge.begin(); it != time_per_edge.end(); it++){
  //   cout << it->first.first.first << ' ' << it->first.first.second << " com tempo: " << it->first.second << endl;
  //   cout << it->second.ti << ' ' << it->second.tf << ' ' << it->second.delta << endl;
  // }
}


int main (int argc, char* argv[]){

  string filename = argv[1];
  int n = atoi(argv[2]);
  int timestep = 86400;

  if (argc >= 4)
    timestep = atoi(argv[3]);

  ifstream f (filename);

  graph g (n);

  set<edge> edge_set;

  while (true){
    int s, t, ti, tf, delta;
    int x1, y1, x2, y2;

    f >> s >> t >> tf >> ti >> delta;

    if (not f)
      break;

    int td = discrete (ti);

    // Window is 
    if (td >= 28)
      continue;

    /*
      Edges are mapped to 0-base index
    */
    int source = get_id(s);
    int target = get_id(t);
    edge e = make_pair(source, target);
    edge re = make_pair(target, source);

    /*
    Properties
    */
    time_per_edge[e].push_back( tempo (ti, tf, delta) );
    time_per_edge[re].push_back ( tempo (ti, tf, delta) );


    /*
      Since I don't keep the properties (time, ...) of an edge in the graph 
      it's not necessary to add the same edge twice.
    */
    if (edge_set.find(make_pair (source, target)) == edge_set.end()){
      g[source].push_back (target);
      g[target].push_back (source);
      edge_set.insert(make_pair (source, target));
      edge_set.insert(make_pair (target, source));
    }

  }

  // print_graph(g);

  int success = 0;

  srand (2);
  for (int i=0; i<id_v.size(); i++){

    /*
      as arestas escolhidas (s, t) devem existir no edge_set!!!!!!
      motivo: necessario para plotar o grafo de barras
    */
    int rtarget = i;
    while (rtarget == i) rtarget = g[i][rand()%g[i].size()];

    int ti = timestep * (rand()%7);
    int tf = ti + timestep*8;
    int delta = tf - ti;

    /*
      first line is
      source target time
    */    
    cout << get_vertex(i) << ' ' << get_vertex(rtarget) << ' ' << ti << endl;
    vector<int> path = bfs (g, i, rtarget, tempo (ti, tf, delta) );

    if (path.size() > 0){
      success += 1;
      for (auto i: path)
        cout << get_vertex(i) << ' ';
      cout << endl;
    }
    else {
      cout << "-1" << endl;
    }

  }

  // cout << "success = " << success << endl;

  return 0;
}