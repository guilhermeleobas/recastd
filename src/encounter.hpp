#pragma once

#include <vector>
#include <map>
#include <set>

using namespace std;

class encounter {
  typedef unsigned int uint;
private:
  uint s, t, tf, ti, delta;
  
public:
  encounter();
  encounter (uint s, uint t, uint ti, uint tf);
};
