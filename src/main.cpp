#include "debug.hpp"
#include "encounter.hpp"
#include "graph.hpp"
#include "methods.hpp"
#include "linesweep.hpp"

#include "argh.h"

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
int max_len= 1;

vector<encounter> read_encounters() {
  vector<encounter> v;

  ifstream f;
  f.tie(NULL);
  f.sync_with_stdio(false);

  f.open(input_filename, ios_base::in);

  while (true) {
    uint s, t;
    uint64_t tf, ti, delta;
    f >> s >> t >> tf >> ti >> delta;
    if (not f) break;
    if (DEBUG)
      cout << "read: " << s << ' ' << t << ' ' << tf << ' ' << ti << ' '
           << delta << '\n';

    if (s > t) swap(s, t);

    v.push_back(encounter(s, t, tf, ti, delta));
  }

  return v;
}

map<node, graph> build_graphs_share_neighbors(const vector<encounter> &v) {
  map<node, graph> mapa;

  ofstream f;
  string s = "bytes_";
  s += share_graph;
  s += ".txt";
  cout << s << '\n';
  f.open(s, ios_base::out);

  for (uint i = 0; i < v.size(); i++) {
    const encounter e = v[i];
    node a = e.get_s(), b = e.get_t();

    if (i % 10000 == 0) cout << "len: " << i << '/' << v.size() << '\n';

    if (DEBUG) {
      cout << "nodes: " << a << ' ' << b << '\n';
    }

    if (mapa.find(a) == mapa.end()) mapa[a] = graph(a, max_days, max_nodes);
    if (mapa.find(b) == mapa.end()) mapa[b] = graph(b, max_days, max_nodes);

    mapa[a].add_encounter(e);
    mapa[b].add_encounter(e);

    merge_graphs_share_neighbors(mapa[a], mapa[b], share_graph, f);
  }

  f.close();

  return mapa;
}

map<node, graph> build_graphs_share_graph(const vector<encounter> &v) {
  map<node, graph> mapa;
  line_sweep ls;

  ofstream f;
  string s = "bytes_";
  s += share_graph;
  s += ".txt";
  f.open(s, ios_base::out);

  for (uint i = 0; i < v.size(); i++) {
    const encounter e = v[i];
    node a = e.get_s(), b = e.get_t();

    if (i % 10000 == 0) cout << "len: " << i << '/' << v.size() << '\n';

    if (DEBUG) {
      cout << "nodes: " << a << ' ' << b << '\n';
    }

    if (mapa.find(a) == mapa.end()) mapa[a] = graph(a, max_days, max_nodes);
    if (mapa.find(b) == mapa.end()) mapa[b] = graph(b, max_days, max_nodes);

    mapa[a].add_encounter(e);
    mapa[b].add_encounter(e);

    merge_graphs_share_graph(mapa[a], mapa[b], share_graph, f);
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

    if (DEBUG) cout << "comecou\n";
    ls.add_encounter(e);
    if (DEBUG) cout << "terminou\n";

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

map<node, graph> build_graphs_share_last_k(const vector<encounter> &v) {
  map<node, graph> mapa;

  ofstream f;
  string s = "bytes_";
  s += share_graph;
  s += ".txt";
  cout << s << '\n';
  f.open(s, ios_base::out);

  for (uint i = 0; i < v.size(); i++) {
    const encounter e = v[i];
    node a = e.get_s(), b = e.get_t();

    if (i % 10000 == 0) cout << "len: " << i << '/' << v.size() << '\n';

    if (DEBUG) {
      cout << "nodes: " << a << ' ' << b << '\n';
    }

    if (mapa.find(a) == mapa.end()) mapa[a] = graph(a, max_days, max_nodes, max_len);
    if (mapa.find(b) == mapa.end()) mapa[b] = graph(b, max_days, max_nodes, max_len);
    
    // cout << mapa[a].get_ego() << endl;
    // cout << mapa[b].get_ego() << endl;

    merge_graphs_share_last_k(mapa[a], mapa[b], share_graph, f);
    
    mapa[a].add_encounter(e);
    mapa[b].add_encounter(e);
    
    mapa[a].add_encounter_to_queue(e);
    mapa[b].add_encounter_to_queue(e);

  }

  f.close();

  return mapa;
}

void parse_st(argh::parser &cmdl) {
  if (!(cmdl("st", "node") >> share_graph ||
        cmdl("share_type", "node") >> share_graph)) {
    cerr << "share_type must be a string\n";
    exit(1);
  }

  if (share_graph == "lastk") {
    if (!cmdl("maxlen") or !(cmdl("maxlen") >> max_len)) {
      cerr << "Missing integer maxlen value\n";
      exit(1);
    }
  }
}

void parse_input(argh::parser &cmdl) {
  if (!(cmdl("in") >> input_filename || cmdl("input") >> input_filename)) {
    cerr << "input must be a string\n";
    exit(1);
  }
}

void parse_output(argh::parser &cmdl) {
  if (!(cmdl("out", "out") >> filename || cmdl("output", "out") >> filename)) {
    cerr << "output must be a string\n";
    exit(1);
  }
}

void parse_timestep(argh::parser &cmdl) {
  if (!(cmdl("t", 86400) >> timestep || cmdl("timestep", 86400) >> timestep)) {
    cerr << "Timestep must be an integer\n";
    exit(1);
  }
}

void parse_arguments(argh::parser cmdl) {

  if (cmdl("help")) {
    cerr << "USAGE:\n";
    cerr << "./bazel/bin/src/recast < -in, --input input\n";
    cerr << '\n';
    cerr << "Optional arguments:\n";
    cerr << "-t, --timestep\t\tSpecify a timestep. Default value is 86400.\n";
    cerr << "-out, --output\t\tSpecify a folder name to output files. Default "
            "value is '/out' created in the root of your project\n";
    cerr << "-st, --share_type {node, neighbors, graph, lastk}\t\t How the "
            "sharing is made. "
            "Default value is node\n";
    cerr << "if lastk mode is used, you must specify a second parameter "
            "-maxlen. "
            "This parameter (maxlen) is the length of the queue.";

    exit(1);
  }

  parse_st(cmdl);
  parse_input(cmdl);
  parse_output(cmdl);
  parse_timestep(cmdl);

  cout << "[INFO] Timestep: " << timestep << '\n';
  cout << "[INFO] Input: " << input_filename << '\n';
  cout << "[INFO] Output: " << filename << '\n';
  cout << "[INFO] Sharing: " << share_graph << '\n';
  if (share_graph == "lastk"){
    cout << "[INFO] max len: " << max_len << '\n';
  }
}

int main(int argc, char *argv[]) {

  ios::sync_with_stdio(false);
  cin.tie(0);

  cout << "-----\n";

  argh::parser cmdl =
      argh::parser(argc, argv, argh::parser::PREFER_PARAM_FOR_UNREG_OPTION);

  parse_arguments(cmdl);

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
  if (share_graph == "graph") {
    mapa = build_graphs_share_graph(v);
  } else if (share_graph == "neighbors") {
    mapa = build_graphs_share_neighbors(v);
  } else if (share_graph == "node") {
    mapa = build_graphs_share_node(v);
  } else {
    mapa = build_graphs_share_last_k(v);
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
