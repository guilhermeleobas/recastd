#include "debug.hpp"
#include "encounter.hpp"
#include "graph.hpp"
#include "methods.hpp"
#include "linesweep.hpp"

#include <iostream>
#include <map>
#include <unordered_set>
#include <memory>
#include <sys/stat.h>
#include <string>
#include <algorithm>

using namespace std;

typedef unsigned uint;

string filename = "out/";
string input_filename;
string share_graph = "node";

vector<encounter> read_encounters() {
  vector<encounter> v;

  ifstream f;
  f.tie(NULL);
  f.sync_with_stdio(false);

  f.open(input_filename, ios_base::in);
  
  while (true) {
    uint s, t, tf, ti, delta;
    f >> s >> t >> tf >> ti >> delta;
    if (not f) break;
    if (DEBUG)
      cout << "read: " << s << ' ' << t << ' ' << tf << ' ' << ti << ' '
           << delta << '\n';

    if (s > t) swap(s, t);

    min_ti = min(min_ti, ti);
    min_tf = min(min_tf, tf);
    v.push_back(encounter(s, t, tf, ti, delta));
  }

  cout << "[INFO] min_ti: " << min_ti << '\n';
  cout << "[INFO] min_tf: " << min_tf << '\n';

  return v;
}

map<node, graph> build_graphs_share_neighbors(const vector<encounter> &v){
  map<node, graph> mapa;

  bool sg = false; // share_neighbors
  
  ofstream f;
  string s = "bytes_";
  s += share_graph;
  s += ".txt";
  cout << s << '\n';
  f.open(s, ios_base::out);

  for (int i=0; i<v.size(); i++){
   const encounter e = v[i];
    node a = e.get_s(), b = e.get_t();

    if (i % 10000 == 0)
      cout << "len: " << i << '/' << v.size() << '\n';

    if (DEBUG) {
      cout << "nodes: " << a << ' ' << b << '\n';
    }

    if (mapa.find(a) == mapa.end()) mapa[a] = graph(a, max_days, max_nodes);
    if (mapa.find(b) == mapa.end()) mapa[b] = graph(b, max_days, max_nodes);

    mapa[a].add_encounter(e);
    mapa[b].add_encounter(e);

    merge_graphs(mapa[a], mapa[b], sg, f);
  }
  
  f.close();

  return mapa;
}

map<node, graph> build_graphs_share_graph(const vector<encounter> &v) {
  map<node, graph> mapa;
  line_sweep ls;

  bool sg = true;

  ofstream f;
  string s = "bytes_";
  s += share_graph;
  s += ".txt";
  f.open(s, ios_base::out);
  
  for (int i=0; i<v.size(); i++) {
    const encounter e = v[i];
    node a = e.get_s(), b = e.get_t();

    if (i % 10000 == 0)
      cout << "len: " << i << '/' << v.size() << '\n';

    if (DEBUG) {
      cout << "nodes: " << a << ' ' << b << '\n';
    }

    if (mapa.find(a) == mapa.end()) mapa[a] = graph(a, max_days, max_nodes);
    if (mapa.find(b) == mapa.end()) mapa[b] = graph(b, max_days, max_nodes);

    // ls.add_encounter(e);

    // vector<node> nodes = ls.node_intersection(a, b);

    mapa[a].add_encounter(e);
    mapa[b].add_encounter(e);

    merge_graphs(mapa[a], mapa[b], sg, f);

    // if (nodes.empty()) {
    //   graph g = merge_graphs(mapa[a], mapa[b]);
    //   g.add_encounter(e);
    //   mapa[a] = g;
    //   mapa[b] = g;
    // }

    // for (const node c : nodes) {
    //   graph g = merge_graphs(mapa[a], mapa[b], mapa[c]);

    //   mapa[a] = g;
    //   mapa[b] = g;
    //   mapa[c] = g;
    // }
  }

  f.close();
  
  return mapa;
}

map<node, graph> build_graphs_share_node(const vector<encounter> &v) {
  map<node, graph> mapa;
  line_sweep ls;

  ofstream f;
  string s = "bytes_";
  s += share_graph;
  s += ".txt";
  f.open(s, ios_base::out);

  for (const encounter &e : v) {
    node a = e.get_s(), b = e.get_t();

    if (DEBUG) {
      cout << "nodes: " << a << ' ' << b << '\n';
    }

    if (mapa.find(a) == mapa.end()) mapa[a] = graph(a, max_days, max_nodes);
    if (mapa.find(b) == mapa.end()) mapa[b] = graph(b, max_days, max_nodes);

    if (DEBUG)
      cout << "comecou\n";
    ls.add_encounter(e);
    if (DEBUG)
      cout << "terminou\n";

    vector<node> nodes = ls.node_intersection(a, b);

    if (nodes.empty()) {
      f << 1 << '\n' << 1 << '\n';
      mapa[a].add_encounter(e);
      mapa[b].add_encounter(e);
    }

    for (const node c : nodes) {
      const encounter e0 = ls.get_encounter(a, b);
      const encounter e1 = ls.get_encounter(a, c);
      const encounter e2 = ls.get_encounter(b, c);

      f << 1 << '\n' << 1 << '\n' << 1 << '\n';

      mapa[a].add_encounter(e0, e1, e2);
      mapa[b].add_encounter(e0, e1, e2);
      mapa[c].add_encounter(e0, e1, e2);
    }
  }

  return mapa;
}

bool parse_arguments(int argc, char *argv[]) {

  // if (argc != 3 and argc != 5 and argc != 7) return false;

  for (int i = 1; i <= argc - 2; i += 2) {
    string arg = string(argv[i]);
    if (arg == "-t" || arg == "--timestep") {
      timestep = atoi(argv[i + 1]);
      cout << "[INFO] Setting timestep to: " << timestep << '\n';
    } else if (arg == "-o" || arg == "--output" || arg == "-out") {
      filename = string(argv[i + 1]);
      cout << "[INFO] Setting output folder to: " << filename << '\n';
    } else if (arg == "-in" || arg == "--input") {
      input_filename = string(argv[i + 1]);
      cout << "[INFO] Setting input filename to: " << input_filename << '\n';
    } else if (arg == "-st" || arg == "--share_type") {
      string s = argv[i+1];
      std::transform(begin(s), end(s), begin(s), ::tolower);
      if (s == "node")
        share_graph = "node";
      else if (s == "graph")
        share_graph = "graph";
      else if (s == "neighbors")
        share_graph = "neighbors";
      else {
        cerr << "Cannot match s in share_type\n";
        exit(1);
      }
    }
  }

  if (input_filename.empty()) return false;

  cout << "[INFO] Timestep: " << timestep << '\n';
  cout << "[INFO] Input: " << input_filename << '\n';
  cout << "[INFO] Output: " << filename << '\n';
  cout << "[INFO] Sharing: " << share_graph  << '\n';

  return true;
}

int main(int argc, char *argv[]) {

  ios::sync_with_stdio(false);
  cin.tie(0);

  cout << "-----\n";

  if (parse_arguments(argc, argv)) {

  } else {
    cerr << "USAGE:\n";
    cerr << "./bazel/bin/src/recast < -in|--input input\n";
    cerr << '\n';
    cerr << "Optional arguments:\n";
    cerr << "-t, --timestep\t\tSpecify a timestep. Default value is 86400.\n";
    cerr << "-out, --output\t\tSpecify a folder name to output files. Default "
            "value is '/out' created in the root of your project\n";
    cerr << "-st, --share_type {node, neighbors, graph}\t\t How the sharing is made. "
            "Default value is node\n";

    exit(1);
  }

  // graph g1 (2, 10, 10);
  // graph g2 (1, 10, 10);

  // merge_graphs (g1, g2);

  // encounter e (0, 1, timestep-1, 0, 20);
  // min_ti = 0;
  // e.set_day();
  // cout << e << endl;

  mkdir(filename.c_str(), S_IRUSR | S_IWUSR | S_IXUSR);

  vector<encounter> v = read_encounters();
  set_properties(v);
  

  map<node, graph> mapa;
  if (share_graph == "graph"){
    mapa = build_graphs_share_graph(v);
  }
  else if (share_graph == "neighbors"){
    mapa = build_graphs_share_neighbors(v); 
  }
  else{
    mapa = build_graphs_share_node(v);
  }
  
  cout << mapa.size() << endl;

  for (auto &&it : mapa) {
    if (DEBUG) cout << "dumping: " << it.first << endl;
    ofstream f;
    f.sync_with_stdio(false);
    f.tie(NULL);
    f.open(filename + '/' + to_string(it.first) + ".txt");
    it.second.dump(f);
  }

  return 0;
}
