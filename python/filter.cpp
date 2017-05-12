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

int discrete (int ti, int timestep = 86400){
  return (ti/timestep);
}

int main (int argc, char* argv[]){

  string filename = argv[1];

  ifstream f (filename);
  
  while (true){
    int s, t, ti, tf, delta;
    int x1, y1, x2, y2;

    f >> s >> t >> tf >> ti >> delta;

    if (not f)
      break;

    int td = discrete (ti);

    if (td > 28)
      continue;

    cout << s << ' ' << t << ' ' << tf << ' ' << ti << ' ' << delta << endl;
  }

  return 0;
}