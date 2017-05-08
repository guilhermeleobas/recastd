#include "catch.hpp"
#include "../src/encounter.hpp"
#include "../src/methods.hpp"
#include <iostream>
#include <vector>

SCENARIO ("Merge encounters", "[merge]") {
  // two encounters (a, b) can be merged if the end(a) == begin(b)

  WHEN ("encounters can be merged"){
    encounter e1 (1, 2, 20, 10, 10);
    encounter e2 (1, 2, 30, 20, 10);
    THEN("Merge them"){
      REQUIRE(can_merge(e1, e2) == true);
      REQUIRE(merge(e1, e2) == encounter(1, 2, 30, 10, 20));
    }
  }

  WHEN ("encounters cannot be merged"){
    // e3 0 -------------- 30 (1 <-> 3)
    // e1    10 -- 20         (1 <-> 2)
    // e2          20 ---- 30 (1 <-> 2)
    encounter e1 (1, 2, 19, 10, 10);
    encounter e2 (1, 2, 30, 20, 10);
    encounter e3 (1, 3, 30, 0, 30);
    THEN("Return false"){
      REQUIRE(can_merge(e1, e2) == false);
      REQUIRE(can_merge(e1, e3) == false);
    }
  }

  WHEN("given a list of encounters"){
    vector<encounter> v;
    v.push_back (encounter(1, 2, 20, 10, 10));
    v.push_back (encounter(1, 3, 20, 10, 10));
    v.push_back (encounter(1, 4, 21, 11, 10));
    v.push_back (encounter(1, 2, 30, 20, 10));
    v.push_back (encounter(1, 2, 40, 30, 10));
    v.push_back (encounter(1, 2, 50, 40, 10));
    v.push_back (encounter(1, 2, 51, 50, 1));

    THEN("merge some of them"){
      merge_encounters(v);
      REQUIRE(v.size() == 3);
    }
  }
}

