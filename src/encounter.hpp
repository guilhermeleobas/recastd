#pragma once

#include <vector>
#include <map>
#include <set>

using namespace std;

typedef unsigned int uint;
typedef pair<uint, uint> edge;

const uint DEFAULT_TIMESTEP = 60*60*24; // 60 seconds x 60 minutes x 24 hours = 86400 seconds
extern uint timestep;
extern uint min_tf;
extern uint min_ti;


class encounter {
private:
  uint s, t, tf, ti, delta, day;
  uint min_day, max_day;
public:
  
  encounter();
  encounter (uint, uint, uint, uint, uint);
  
  void set_day ();
  uint get_day() const;

  uint get_min_day () const;
  uint get_max_day () const;
  
  uint get_tf() const;
  uint get_ti() const;

  void set_ti(uint);
  void set_tf(uint);

  uint get_delta() const;
  
  uint get_s() const;
  uint get_t() const;
 
  edge get_edge() const;
  edge get_reverse_edge() const;

  bool operator == (const encounter&) const;

  struct hash{
  public:
    std::size_t operator () (const encounter& enc) const {
      using std::hash;
      return 
        hash<uint>()(enc.get_s())  ^ 
        hash<uint>()(enc.get_t())  ^
        hash<uint>()(enc.get_ti()) ^
        hash<uint>()(enc.get_tf());
    }
  };
  
  
  friend std::ostream& operator<< (std::ostream &out, const encounter&);
};
