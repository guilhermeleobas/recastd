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
    uint s, t, tf, ti, delta;
    cin >> s >> t >> tf >> ti >> delta;
    if (not cin) break;
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

int main(int argc, char *argv[]) {

  ios::sync_with_stdio(false);
  cin.tie(0);

  vector<encounter> v = read_encounters();
  set_properties(v);

  for (const encounter &e : v){
    if (e.get_max_day() > 28) continue;
    
    cout << e.get_s() << ' ' << e.get_t() << ' ' << e.get_tf() << ' ' << e.get_ti() << ' ' << e.get_delta() << '\n';
  }

  return 0;
}
