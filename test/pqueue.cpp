#include "catch.hpp"
#include "../src/pqueue.hpp"

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
  }
}