#include "debug.hpp"
#include "encounter.hpp"
#include "graph.hpp"
#include "methods.hpp"
#include "linesweep.hpp"

#include <iostream>
#include <map>
#include <unordered_set>
#include <sys/stat.h>

using namespace std;

typedef unsigned uint;

string filename = "out/";
string input_filename;

vector<encounter> read_encounters (){
  vector<encounter> v;

  // ios::sync_with_stdio(false);
  // cin.tie(NULL);

  ifstream f;
  f.tie(NULL);
  f.sync_with_stdio(false);

  f.open(input_filename, ios_base::in);
  
  while(true){
    uint s, t, tf, ti, delta;
    f >> s >> t >> tf >> ti >> delta;
    if (not f)
      break;

    if (s > t)
      swap (s, t);
    
    v.push_back (encounter(s, t, tf, ti, delta));
    
  }
  
  return v;
}

map<node, graph> build_graphs(const vector<encounter> &v){
  map<node, graph> mapa;
  line_sweep ls;

  for (const encounter &e : v){
    node a = e.get_s(), b = e.get_t();

    if (mapa.find(a) == mapa.end())
      mapa[a] = graph(a);
    if (mapa.find(b) == mapa.end())
      mapa[b] = graph(b);

    ls.add_encounter(e);

    vector<node> nodes = ls.node_intersection(a, b);

    if (nodes.empty()){
      mapa[a].add_edge(e);
      mapa[b].add_edge(e);
    }

    for (const node c : nodes){
      const reference_wrapper<const encounter> e0 = ls.get_encounter(a, b);
      const reference_wrapper<const encounter> e1 = ls.get_encounter(a, c);
      const reference_wrapper<const encounter> e2 = ls.get_encounter(b, c);

      mapa[a].add_edge(e0, e1, e2);
      mapa[b].add_edge(e0, e1, e2);
      mapa[c].add_edge(e0, e1, e2);
    }
  }

  return mapa;
}

bool parse_arguments (int argc, char* argv[]){

  if (argc != 3 and argc != 5 and argc != 7)
    return false;

  for (int i=1; i<=argc-2; i+=2){
    string arg = string(argv[i]);
    if (arg == "-t" || arg == "--timestep"){
      timestep = atoi(argv[i+1]);
      cout << "[INFO] Setting timestep to: " << timestep << '\n';
    }
    else if (arg == "-o" || arg == "--output" || arg=="-out"){
      filename = string(argv[i+1]);
      cout << "[INFO] Setting filename to: " << filename << '\n';
    }
    else if (arg == "-in" || arg == "--input"){
      input_filename = string(argv[i+1]);
      cout << "[INFO] Setting input filename to: " << input_filename << '\n';
    }
  }

  if (input_filename.empty())
    return false;

  cout << "[INFO] Timestep: " << timestep << endl;
  cout << "[INFO] Input: " << input_filename << endl;
  cout << "[INFO] Output: " << filename << endl;

  return true;
}

int main (int argc, char *argv[]){

  ios::sync_with_stdio(false);
  cin.tie(0);

  cout << "-----\n";
  
  if (parse_arguments(argc, argv)){

  }
  else {
    cerr << "USAGE:\n";
    cerr << "./bazel/bin/src/recast < -in|--input input\n";
    cerr << '\n';
    cerr << "Optional arguments:\n";
    cerr << "-t, --timestep\t\tSpecify a timestep. Default value is 86400.\n";
    cerr << "-o, --output\t\tSpecify a folder name to output files. Default value is '/out' created in the root of your project\n";

    exit(1);
  }

  mkdir (filename.c_str(), S_IRUSR | S_IWUSR | S_IXUSR);

  vector<encounter> v = read_encounters();
  set_properties(v);

  map<node, graph> mapa = build_graphs(v);

  for (auto&& it : mapa){
    // cout << "dumping: " << it.first << endl;
    ofstream f;
    f.sync_with_stdio(false);
    f.tie(NULL);
    f.open (filename + '/' + to_string(it.first) + ".txt");
    it.second.dump(f);
  }

  return 0;
}
