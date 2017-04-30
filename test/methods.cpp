#include "catch.hpp"
#include "../src/methods.hpp"

unsigned int Factorial( unsigned int number ) {
  return number <= 1 ? number : Factorial(number-1)*number;
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
  REQUIRE( Factorial(1) == 1 );
  REQUIRE( Factorial(2) == 2 );
  REQUIRE( Factorial(3) == 6 );
  REQUIRE( Factorial(10) == 3628800 );
}

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

}