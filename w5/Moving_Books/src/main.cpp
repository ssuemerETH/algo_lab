#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

// O((n + m)log(n + m))

/* Strategy: Combine all jobs, workers in a vector, sort by descending weight/strength (if comparing a job and a worker with equal values, worker comes first)
Whenever a new job is encountered, assign it to the worker doing the least amount of work so far.
If everyone turns out to be doing the same amount of work, increase the maximum by one.
Whenever a new worker is encountered, increase the capacity by max amt so far. 
*/

void testcase() {
  int n, m; std::cin >> n >> m;
  // (x, true) is a strength, (x, false) is a weight
  std::vector<std::pair<int, bool>> sw; sw.reserve(n + m);
  
  for (int i = 0; i < n; i++) {
    int si; std::cin >> si;
    sw.push_back(std::make_pair(si, true));
  }
  
  for (int i = 0; i < m; i++) {
    int wi; std::cin >> wi;
    sw.push_back(std::make_pair(wi, false));
  }
  
  std::sort(sw.begin(), sw.end(), std::greater<std::pair<int, bool>>());
  
  // max is the maximum number of books someone carries so far.
  // workers is the number of workers found so far.
  // capacity is (max * workers - (found jobs so far))
  int max, workers, capacity; max = workers = capacity = 0;
  
  // If largest amount is a job, no worker can do it.
  if (!sw[0].second) {
    std::cout << "impossible\n";
    return;
  } else workers = 1;

  for (int i = 1; i < n + m; i++) {
    std::pair<int, bool> p = sw[i];
    // found a worker
    if (p.second) {
      workers++;
      capacity += max;
    } else if (capacity == 0) { // if all workers at max, increase max and adjust capacity
        max++;
        capacity += workers - 1;
    } else capacity--;
  }
  
  // each job except the last one requires 3 units of time, last job does not require going back up so subtract 1
  std::cout << (3 * max - 1) << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for(; t > 0; t--) testcase();
  return 0;
}