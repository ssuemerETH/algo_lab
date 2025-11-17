#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>

bool int_contains(const std::pair<long, long> &interval, long x) {
  long ai = interval.first; long bi = interval.second;
  if (ai <= bi) return ai <= x && x <= bi;
  else return x >= ai || x <= bi;
}

bool intersect(const std::pair<long, long> &i1, const std::pair<long, long> &i2) {
  long l1 = i1.first; long r1 = i1.second;
  long l2 = i2.first; long r2 = i2.second;
  return !(l1 > r2 || r1 < l2);
}

long tr(long x, long p, long m) {
  return (p - x + m) % m;
}

bool contains_zero(const std::pair<long, long> &interval) {
  long ai = interval.first; long bi = interval.second;
  return ai > bi || ai == 0L;
}

int max_linear(long m, const std::vector<std::pair<long, long>> &intervals, const std::pair<long, long> &forbidden, bool use_forbidden) {
  long fin = -1;
  int max = 0;
  
  bool decomp = false;
  std::pair<long, long> i1, i2;
  if (forbidden.first > forbidden.second) {
    i1.first = forbidden.first; i1.second = m - 1;
    i2.first = 0; i2.second = forbidden.second;
    decomp = true;
  }
  
  for (const std::pair<long, long> &cur: intervals) {
    if (contains_zero(cur) || (use_forbidden && ((!decomp && intersect(cur, forbidden)) || (decomp && (intersect(cur, i1) || intersect(cur, i2)))))) continue;
    if (cur.first > fin) {
      max++;
      fin = cur.second;
    }
  }
  
  return max;
}

void testcase() {
  int n; long m; std::cin >> n >> m;
  // (x, false) indicates that a (circular) interval has its last point at x - 1 (mod...)
  // (x, true) indicates that a (circular) interval starts at x
  std::vector<std::pair<long, bool>> events; events.reserve(4 * n);
  std::vector<std::pair<long, long>> intervals; intervals.reserve(n);
  for (int i = 0; i < n; i++) {
    long ai, bi; std::cin >> ai >> bi;
    ai--; bi--;
    intervals.push_back(std::make_pair(ai, bi));
    if (ai <= bi) {
      events.push_back(std::make_pair(ai, true));
      events.push_back(std::make_pair(bi + 1, false));
    } else {
      // decompose [ai, ..., m - 1, 0, ..., bi] as [ai, ..., m - 1] and [0, ..., bi]
      events.push_back(std::make_pair(ai, true));
      events.push_back(std::make_pair(m, false));
      events.push_back(std::make_pair(0, true));
      events.push_back(std::make_pair(bi + 1, false));
    }
  }
  
  std::sort(events.begin(), events.end());
  
  int amt_so_far = 1;
  long cur_point = events[0].first;
  long best_point = -1;
  int best_amt = -1;
  
  
  for (int i = 1; i < events.size(); i++) {
    long p = events[i].first;
    bool t = events[i].second;
    if (p > cur_point) {
      if (best_amt == -1 || amt_so_far < best_amt) {
        best_point = cur_point;
        best_amt = amt_so_far;
      }
      cur_point = p;
    } 
    
    amt_so_far += t ? 1 : -1;
  }
  
  if (amt_so_far < best_amt && cur_point != m) {
    best_point = cur_point;
    best_amt = amt_so_far;
  }
  
  std::vector<std::pair<long, long>> intersecting_best; intersecting_best.reserve(best_amt);
  for (const std::pair<long, long> &interval: intervals) 
    if (int_contains(interval, best_point)) {
      long ai = interval.first; long bi = interval.second;
      intersecting_best.push_back(std::make_pair(tr(best_point, ai, m), tr(best_point, bi, m)));
    }
  
  for (std::pair<long, long> &interval: intervals) {
    long ai = interval.first; long bi = interval.second;
    interval.first = tr(best_point, ai, m); interval.second = tr(best_point, bi, m);
  }
  
  std::sort(intervals.begin(), intervals.end(), [](const std::pair<long, long>& a, const std::pair<long, long>& b) {
    return a.second < b.second;
  });
  
  int max = max_linear(m, intervals, std::make_pair(0L, 0L), false);
  
  for (const std::pair<long, long> &forbidden: intersecting_best)
    max = std::max(max, max_linear(m, intervals, forbidden, true) + 1);
    
  std::cout << max << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
