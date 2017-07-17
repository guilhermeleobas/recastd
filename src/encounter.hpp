#pragma once

#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <memory>

using namespace std;

typedef unsigned int uint;
typedef pair<uint, uint> edge;

const uint64_t DEFAULT_TIMESTEP =
    60 * 60 * 24;  // 60 seconds x 60 minutes x 24 hours = 86400 seconds
extern uint64_t timestep;
extern uint64_t min_tf;
extern uint64_t min_ti;

class encounter {
 private:
  uint s, t;
  uint64_t tf, ti, delta;
  uint min_day, max_day;

 public:
  encounter();
  // ~encounter(){
  //   std::cout << "Destruindo " << *this << '\n';
  // }
  encounter(uint s, uint t, uint64_t tf, uint64_t ti, uint64_t delta);
  encounter(uint s, uint t, uint64_t f, uint64_t ti, uint64_t delta, uint day_i,
            uint day_f);

  void calc_day();

  void set_min_day(uint);
  void set_max_day(uint);
  uint get_min_day() const;
  uint get_max_day() const;

  uint64_t get_tf() const;
  uint64_t get_ti() const;

  void set_ti(uint64_t);
  void set_tf(uint64_t);

  uint64_t get_delta() const;
  void calc_delta();

  void set_s(const uint);
  void set_t(const uint);
  
  uint get_s() const;
  uint get_t() const;

  edge get_edge() const;
  edge get_reverse_edge() const;

  void print(ofstream&, const uint&) const;

  bool operator==(const encounter&) const;

  struct hash {
   public:
    std::size_t operator()(const weak_ptr<encounter>& enc) const {
      using std::hash;
      return hash<uint>()(enc.lock()->get_s()) ^
             hash<uint>()(enc.lock()->get_t()) ^
             hash<uint>()(enc.lock()->get_ti()) ^
             hash<uint>()(enc.lock()->get_tf());
    }
  };

  struct my_compare {
   public:
    bool operator()(const weak_ptr<encounter>& a,
                    const weak_ptr<encounter>& b) {
      bool match_nodes = false;

      if ((a.lock()->get_s() == b.lock()->get_s() and a.lock()->get_t() ==
           b.lock()->get_t())or(a.lock()->get_s() ==
                                b.lock()->get_t() and a.lock()->get_t() ==
                                b.lock()->get_s()))
        match_nodes = true;

      return (match_nodes and a.lock()->get_ti() == b.lock()->get_ti() and a.lock()->get_tf() ==
              b.lock()->get_tf());
    }
  };

  friend std::ostream& operator<<(std::ostream& out, const encounter&);
};

bool can_merge(const encounter& e1, const encounter& e2);
bool can_merge_no_restriction(const encounter& e1, const encounter& e2);
encounter merge(const encounter& e1, const encounter& e2);
