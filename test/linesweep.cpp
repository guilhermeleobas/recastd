#include "catch.hpp"
#include "../src/methods.hpp"
#include <algorithm>
#include <iostream>

using std::cout;
using std::endl;

SCENARIO ("line_sweep algorithm", "[line_sweep]") {
  line_sweep ls;
  WHEN ("I add some items"){
    ls.add_encounter( encounter (1, 2, 12, 0, 12) );
    ls.add_encounter( encounter (10, 20, 12, 0, 12) );

    THEN("ls should add them"){
      
      REQUIRE(ls.get_nodes(1).size() == 1);
      REQUIRE(ls.get_nodes(2).size() == 1);

      REQUIRE(set<node> ({2}) == ls.get_nodes(1));
      REQUIRE(set<node> ({1}) == ls.get_nodes(2));

      REQUIRE(set<node> ({20}) == ls.get_nodes(10));
      REQUIRE(set<node> ({10}) == ls.get_nodes(20));
    }

  }

  WHEN("More items are added"){
    ls.add_encounter( encounter (1, 2, 12, 0, 12));
    ls.add_encounter( encounter (2, 3, 3, 1, 2));
    ls.add_encounter( encounter (2, 3, 5, 4, 1));
    ls.add_encounter( encounter (2, 3, 8, 6, 2));
    ls.add_encounter( encounter (1, 3, 10, 7, 3));
    ls.add_encounter( encounter (2, 3, 11, 9, 2));

    THEN("remove conflicts"){
      REQUIRE(ls.get_nodes(1).size() == 2);
      REQUIRE(ls.get_nodes(1) == set<node> ({2, 3}));
      
      REQUIRE(ls.get_nodes(2).size() == 2);
      REQUIRE(ls.get_nodes(2) == set<node> ({1, 3}));

      REQUIRE(ls.get_nodes(3).size() == 2);
      REQUIRE(ls.get_nodes(3) == set<node> ({1, 2}));
    }
  }
}
