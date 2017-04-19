#include "encounter.hpp"
#include <iostream>

using namespace std;

typedef unsigned uint;

vector<encounter> read_encounters (){
  vector<encounter> v;
  
  ios::sync_with_stdio(false);
  cin.tie(NULL);
  
  uint min_tf = 0x3f3f3f3f;
  
  while(true){
    uint s, t, tf, ti, delta;
    cin >> s >> t >> tf >> ti >> delta;
    if (not cin)
      break;
    v.push_back (encounter(s, t, tf, ti, delta));
    
    min_tf = min(min_tf, tf);
  }
  
  // apply min_tf to each value in vector<encounter>
  std::for_each(v.begin(), v.end(),
    [min_tf] (encounter e){
      e.set_day(min_tf); 
    }
  );
  
  return v;
}

int main (){
  
  vector<encounter> v = read_encounters();
  
  return 0;
}
