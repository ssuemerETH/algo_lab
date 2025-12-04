#include <iostream>
#include <vector>
#include <queue>

typedef std::pair<int, int> PI;

void kill(std::vector<bool> &alive, int ind, int &time) {
  int n = alive.size();
  if (alive[ind]) {
    if (2 * ind + 1 < n) kill(alive, 2 * ind + 1, time);
    if (2 * ind + 2 < n) kill(alive, 2 * ind + 2, time);
    alive[ind] = false;
    time++;
  }
}

void testcase() {
  int n; std::cin >> n;
  std::priority_queue<PI, std::vector<PI>, std::greater<PI>> min_heap;

  for (int i = 0; i < n; i++) {
    int ti; std::cin >> ti;
    min_heap.push(std::make_pair(ti, i));
  }

  // alive[i] iff bird i is alive
  std::vector<bool> alive(n, true);

  int time = 0;
  while (!min_heap.empty()) {
    // pick bird that is about to attack
    // at the nearest time
    PI cur_bird = min_heap.top(); min_heap.pop();
    int cur_t = cur_bird.first;
    int cur_ind = cur_bird.second;

    if (!alive[cur_ind]) continue;
    kill(alive, cur_ind, time);
    if (time > cur_t) {
      std::cout << "no\n";
      return;
    }
  }

  std::cout << "yes\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
