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

using namespace std;

typedef unsigned uint;

string filename = "out/";
string input_filename;
bool share_graph = false;

vector<shared_ptr<encounter>> read_encounters() {
  vector<shared_ptr<encounter>> v;

  ifstream f;
  f.tie(NULL);
  f.sync_with_stdio(false);

  f.open(input_filename, ios_base::in);

  while (true) {
    uint s, t, tf, ti, delta;
    f >> s >> t >> tf >> ti >> delta;
    if (DEBUG)
      cout << "read: " << s << ' ' << t << ' ' << tf << ' ' << ti << ' '
           << delta << '\n';
    if (not f) break;

    if (s > t) swap(s, t);
    
    v.push_back(make_shared<encounter>(encounter(s, t, tf, ti, delta)));
  }

  return v;
}

map<node, graph> build_graphs_share_graph(const vector<shared_ptr<encounter>> &v) {
  map<node, graph> mapa;
  line_sweep ls;

  for (const shared_ptr<encounter> &e : v) {
    node a = e->get_s(), b = e->get_t();

    if (DEBUG) {
      cout << "nodes: " << a << ' ' << b << '\n';
    }

    if (mapa.find(a) == mapa.end()) mapa[a] = graph(a);
    if (mapa.find(b) == mapa.end()) mapa[b] = graph(b);

    ls.add_encounter(e);

    vector<node> nodes = ls.node_intersection(a, b);

    if (nodes.empty()) {

      for (const weak_ptr<encounter> &e_a : mapa[a]) mapa[b].add_edge(e_a);
      for (const weak_ptr<encounter> &e_b : mapa[b]) mapa[a].add_edge(e_b);

      mapa[a].add_edge(e);
      mapa[b].add_edge(e);
    }

    for (const node c : nodes) {

      for (const weak_ptr<encounter> &e_a : mapa[a]) {
        mapa[b].add_edge(e_a);
        mapa[c].add_edge(e_a);
      }

      for (const weak_ptr<encounter> &e_b : mapa[b]) {
        mapa[a].add_edge(e_b);
        mapa[c].add_edge(e_b);
      }

      for (const weak_ptr<encounter> &e_c : mapa[c]) {
        mapa[b].add_edge(e_c);
        mapa[a].add_edge(e_c);
      }

      const weak_ptr<encounter> e0 = ls.get_encounter(a, b);
      const weak_ptr<encounter> e1 = ls.get_encounter(a, c);
      const weak_ptr<encounter> e2 = ls.get_encounter(b, c);

      mapa[a].add_edge(e0, e1, e2);
      mapa[b].add_edge(e0, e1, e2);
      mapa[c].add_edge(e0, e1, e2);
    }
  }

  return mapa;
}

map<node, graph> build_graphs_share_node(const vector<shared_ptr<encounter>> &v) {
  map<node, graph> mapa;
  line_sweep ls;

  for (const shared_ptr<encounter> &e : v) {
    node a = e->get_s(), b = e->get_t();

    if (DEBUG) {
      cout << "nodes: " << a << ' ' << b << '\n';
    }

    if (mapa.find(a) == mapa.end()) mapa[a] = graph(a);
    if (mapa.find(b) == mapa.end()) mapa[b] = graph(b);

    ls.add_encounter(e);

    vector<node> nodes = ls.node_intersection(a, b);

    if (nodes.empty()) {
      mapa[a].add_edge(e);
      mapa[b].add_edge(e);
    }

    for (const node c : nodes) {
      const weak_ptr<encounter> e0 = ls.get_encounter(a, b);
      const weak_ptr<encounter> e1 = ls.get_encounter(a, c);
      const weak_ptr<encounter> e2 = ls.get_encounter(b, c);

      mapa[a].add_edge(e0, e1, e2);
      mapa[b].add_edge(e0, e1, e2);
      mapa[c].add_edge(e0, e1, e2);
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
      cout << "[INFO] Setting filename to: " << filename << '\n';
    } else if (arg == "-in" || arg == "--input") {
      input_filename = string(argv[i + 1]);
      cout << "[INFO] Setting input filename to: " << input_filename << '\n';
    } else if (arg == "-gs" || arg == "--graph_sharing") {
      share_graph = true;
      cout << "[INFO] Setting share mode to \"graph sharing\"\n";
    }

  }

  if (input_filename.empty()) return false;

  cout << "[INFO] Timestep: " << timestep << '\n';
  cout << "[INFO] Input: " << input_filename << '\n';
  cout << "[INFO] Output: " << filename << '\n';
  cout << "[INFO] Sharing: " << (share_graph ? "True" : "False") << '\n';

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
    cerr << "-gs, --graph_sharing True\t\tNodes will share their ego_graph. "
            "Default "
            "value is False\n";

    exit(1);
  }

  mkdir(filename.c_str(), S_IRUSR | S_IWUSR | S_IXUSR);

  vector<shared_ptr<encounter>> v = read_encounters();
  set_properties(v);

  map<node, graph> mapa;
  if (share_graph)
    mapa = build_graphs_share_graph(v);
  else
    map<node, graph> mapa = build_graphs_share_node(v);

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
