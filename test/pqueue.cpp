#include "catch.hpp"
#include "../src/methods.hpp"

// SCENARIO ("", "[line_sweep]") {
//   line_sweep ls;
//   WHEN ("I add some items"){
//     ls.add_encounter( encounter (1, 2, 12, 0, 12) );
//     ls.add_encounter( encounter (10, 20, 12, 0, 12) );

//     ls.add_encounter( encounter (2, 3, 3, 1, 2) );
//     ls.add_encounter( encounter (2, 3, 5, 4, 1) );
//     ls.add_encounter( encounter (2, 3, 8, 6, 2) );
//     ls.add_encounter( encounter (1, 3, 10, 7, 3) );
//     ls.add_encounter( encounter (2, 3, 11, 9, 2) );

//     THEN(""){
//       REQUIRE();
//       REQUIRE();
//       REQUIRE();
//       REQUIRE();
//     }
//   }
// }

SCENARIO ("Implementation of a priority_queue", "[pqueue]"){
  pqueue pq;

  WHEN("I add some non-overlaping items"){
    
    pq.insert( encounter(1, 2, 12, 0, 12) );
    pq.insert( encounter(10, 20, 12, 0, 12) );

    THEN("The pqueue shoud add then"){
      REQUIRE(pq.size() == 2);
    }

  }

  WHEN("intervals overlap"){
    pq.insert( encounter(2, 3, 3, 1, 2) );
    pq.insert( encounter(2, 3, 5, 4, 1) );
    pq.insert( encounter(2, 3, 8, 6, 2) );
    pq.insert( encounter(1, 3, 10, 7, 3) );
    pq.insert( encounter(2, 3, 11, 9, 2) );

    THEN("remove overlaps"){
      REQUIRE (pq.size() == 2);
    }
  }

}

SCENARIO ("Delete values", "[vector]"){
  vector<encounter> v;

  GIVEN("A vector with some encounters"){
    v.push_back(encounter(2, 3, 3, 1, 2));
    v.push_back(encounter(1, 2, 12, 0, 12));
    v.push_back(encounter(10, 20, 12, 0, 12));

    THEN("the size should be 3"){
      REQUIRE(v.size() == 3);
    }

    WHEN("An item is deleted"){
      delete_encounter_in_vector(v, encounter(10, 20, 12, 0, 12));
      THEN("the size should be 2"){
        REQUIRE(v.size() == 2);
      }
    }

    WHEN("Two items are deleted"){
      delete_encounter_in_vector(v, encounter(10, 20, 12, 0, 12));
      delete_encounter_in_vector(v, encounter(1, 2, 12, 0, 12));
      THEN("the size should be 1"){
        REQUIRE(v.size() == 1);
      }
    }

    WHEN("All items are deleted"){
      delete_encounter_in_vector(v, encounter(10, 20, 12, 0, 12));
      delete_encounter_in_vector(v, encounter(1, 2, 12, 0, 12));
      delete_encounter_in_vector(v, encounter(2, 3, 3, 1, 2));
      THEN("the size should be 0"){
        REQUIRE(v.size() == 0);
      }
    }


    WHEN("Try to delete an encounter that does not exist in v"){
      THEN("the return should be false"){
        REQUIRE(delete_encounter_in_vector(v, encounter(10, 22, 12, 0, 12)) == false);
        REQUIRE(delete_encounter_in_vector(v, encounter(1, 30, 12, 0, 12)) == false);
        REQUIRE(delete_encounter_in_vector(v, encounter(20, 30, 3, 1, 2)) == false);
      }
    }
  }
}