#include "catch.hpp"
#include "../src/linesweep.hpp"
#include "../src/methods.hpp"
#include <algorithm>
#include <iostream>
#include <tuple>

using std::cout;
using std::endl;
using std::tuple;

typedef tuple<encounter, encounter, encounter> tupla;
bool checkTuple(const tupla &t, const encounter &e){
  
  if (std::get<0>(t) == e)
    return true;
  if (std::get<1>(t) == e)
    return true;
  if (std::get<2>(t) == e)
    return true;
  
  return false;
}

SCENARIO ("line_sweep algorithm", "[line_sweep]") {
  line_sweep ls;
  WHEN ("Add some items"){
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

SCENARIO ("A 3-clique"){
  line_sweep ls;

  ls.add_encounter(encounter(1, 2, 8, 5, 3));
  ls.add_encounter(encounter(0, 1, 25, 5, 20));
  ls.add_encounter(encounter(0, 2, 10, 6, 4));

  THEN("find clique"){
    vector<node> t = ls.node_intersection(0,1);
    REQUIRE(t.size() > 0);
    REQUIRE(t[0] == 2);
  }

}

SCENARIO ("Multiple 3-clique", "clique"){

  GIVEN ("Encounters"){  

    vector<encounter> v;
    v.push_back(encounter(0, 1, 25, 5, 20));

    v.push_back(encounter(0, 2, 10, 6, 4));
    v.push_back(encounter(0, 3, 16, 12, 4));
    v.push_back(encounter(0, 4, 26, 16, 10));

    v.push_back(encounter(1, 2, 8, 5, 3));
    v.push_back(encounter(1, 4, 30, 4, 26));
    
    v.push_back(encounter(1, 3, 12, 7, 5));

    set_properties(v); // this will also sort encounters using ti in ascending order;

    line_sweep ls;
    WHEN("Add 4 encounters"){
      ls.add_encounter(v[0]);
      ls.add_encounter(v[1]);
      ls.add_encounter(v[2]);
      ls.add_encounter(v[3]);

      THEN("Find a clique with nodes {0, 1, 2}"){
        REQUIRE (ls.node_intersection(0, 1).size() == 1);
        REQUIRE (ls.node_intersection(0, 1)[0] == 2);
        
        vector<tupla> vt = ls.encounter_intersection(0, 1);
        CHECK(vt.size() == 1);
        CHECK(checkTuple(vt[0], v[1]) == true);
        CHECK(checkTuple(vt[0], v[2]) == true);
        CHECK(checkTuple(vt[0], v[3]) == true);

        vt = ls.encounter_intersection(0, 2);
        CHECK(vt.size() == 1);
        CHECK(checkTuple(vt[0], v[1]) == true);
        CHECK(checkTuple(vt[0], v[2]) == true);
        CHECK(checkTuple(vt[0], v[3]) == true);
        
        vt = ls.encounter_intersection(1, 2);
        CHECK(vt.size() == 1);
        CHECK(checkTuple(vt[0], v[1]) == true);
        CHECK(checkTuple(vt[0], v[2]) == true);
        CHECK(checkTuple(vt[0], v[3]) == true);
        
        REQUIRE (ls.node_intersection(0, 2).size() == 1);
        REQUIRE (ls.node_intersection(0, 2)[0] == 1);

        REQUIRE (ls.node_intersection(1, 2).size() == 1);
        REQUIRE (ls.node_intersection(1, 2)[0] == 0);

        AND_WHEN("Add more two encounters"){
          ls.add_encounter(v[4]);
          ls.add_encounter(v[5]);

          THEN("Remove invalid encounters"){
            REQUIRE (ls.node_intersection(0, 2).size() == 0);
            REQUIRE (ls.node_intersection(1, 2).size() == 0);
            
            CHECK(ls.encounter_intersection(0, 2).size() == 0);
            CHECK(ls.encounter_intersection(1, 2).size() == 0);
            
            AND_THEN("Check for a new clique between nodes {0, 1, 3}"){
              REQUIRE (ls.node_intersection(0, 1).size() == 1);
              REQUIRE (ls.node_intersection(0, 1)[0] == 3);
              
              AND_WHEN("Add the last encounter between {1, 4}"){
                ls.add_encounter(v[6]);
                
                THEN ("The encounter between {0, 1, 3} must be deleted"){
                  REQUIRE(ls.node_intersection(0, 1).size() == 1);
                }
                AND_THEN("The new encounter must exists"){
                  CHECK(ls.node_intersection(0, 1)[0] == 4);
                  CHECK(ls.node_intersection(4, 0)[0] == 1);
                  CHECK(ls.node_intersection(4, 1)[0] == 0);
                  
                  CHECK (ls.encounter_intersection(0, 1).size() == 1);
                  CHECK ( checkTuple(ls.encounter_intersection(0, 1)[0], v[6]) );
                  CHECK ( checkTuple(ls.encounter_intersection(0, 1)[0], v[2]) );
                  CHECK ( checkTuple(ls.encounter_intersection(0, 1)[0], v[0]) );
                }
              }
            }
          }
        }
      }
    }
  }
}

















