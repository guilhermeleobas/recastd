#pragma once

#include <vector>
#include <map>
#include <set>

using namespace std;

typedef unsigned int uint;

class encounter {
private:
  static const uint timestep = 60*60*24;
  uint s, t, tf, ti, delta, day;
public:
  encounter();
  encounter (uint, uint, uint, uint, uint);
  
  void set_day (const uint);
  uint get_day() const;
  
  uint get_tf() const;
  uint get_ti() const;
  
  bool contact(const encounter&) const;
};
