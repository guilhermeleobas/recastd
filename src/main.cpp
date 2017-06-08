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

vector<encounter> read_encounters() {
  vector<encounter> v;

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

    v.push_back(encounter(s, t, tf, ti, delta));
  }

  return v;
}

map<node, graph> build_graphs_share_graph(const vector<encounter> &v) {
  map<node, graph> mapa;
  line_sweep ls;

  for (int i=0; i<v.size(); i++) {
    const encounter e = v[i];
    node a = e.get_s(), b = e.get_t();

    cout << "len: " << i << '/' << v.size() << '\n';

    if (DEBUG) {
      cout << "nodes: " << a << ' ' << b << '\n';
    }

    if (mapa.find(a) == mapa.end()) mapa[a] = graph(a, max_days, max_nodes);
    if (mapa.find(b) == mapa.end()) mapa[b] = graph(b, max_days, max_nodes);

    // ls.add_encounter(e);

    // vector<node> nodes = ls.node_intersection(a, b);

    graph g = merge_graphs(mapa[a], mapa[b]);
    g.add_encounter(e);
    mapa[a] = g;
    mapa[b] = g;

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

  return mapa;
}

map<node, graph> build_graphs_share_node(const vector<encounter> &v) {
  map<node, graph> mapa;
  line_sweep ls;

  for (const encounter &e : v) {
    node a = e.get_s(), b = e.get_t();

    if (DEBUG) {
      cout << "nodes: " << a << ' ' << b << '\n';
    }

    if (mapa.find(a) == mapa.end()) mapa[a] = graph(a, max_days, max_nodes);
    if (mapa.find(b) == mapa.end()) mapa[b] = graph(b, max_days, max_nodes);

    ls.add_encounter(e);

    vector<node> nodes = ls.node_intersection(a, b);

    if (nodes.empty()) {
      mapa[a].add_encounter(e);
      mapa[b].add_encounter(e);
    }

    for (const node c : nodes) {
      const reference_wrapper<const encounter> e0 = ls.get_encounter(a, b);
      const reference_wrapper<const encounter> e1 = ls.get_encounter(a, c);
      const reference_wrapper<const encounter> e2 = ls.get_encounter(b, c);

      mapa[a].add_encounter(e0.get(), e1.get(), e2.get());
      mapa[b].add_encounter(e0.get(), e1.get(), e2.get());
      mapa[c].add_encounter(e0.get(), e1.get(), e2.get());
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

  // map<node, graph> mapa;
  // if (share_graph)
  //   mapa = build_graphs_share_graph(v);
  // else
  map<node, graph> mapa = build_graphs_share_graph(v);

  cout << mapa.size() << '\n';

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
