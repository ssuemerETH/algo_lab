#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

struct RayInfo {
  int index;
  long y0, x1, y1;

  bool operator<(const RayInfo &other) const {
    return y0 < other.y0;
  }
};

// 0: no intersection, 1: oldr kills newr, 2: newr kills oldr
int intersect_rays(const RayInfo &oldr, const RayInfo &newr) {
  K::Point_2 old_source(0L, oldr.y0), new_source(0L, newr.y0);
  K::Ray_2 oldray(old_source, K::Point_2(oldr.x1, oldr.y1));
  K::Ray_2 newray(new_source, K::Point_2(newr.x1, newr.y1));

  if (CGAL::do_intersect(oldray, newray)) {
    auto o = CGAL::intersection(oldray, newray);
    if (const K::Point_2* op = boost::get<K::Point_2>(&*o)) {
      K::FT old_dist = CGAL::squared_distance(old_source, *op);
      K::FT new_dist = CGAL::squared_distance(new_source, *op);
      return old_dist <= new_dist ? 1 : 2;
    } else throw std::runtime_error("rays intersecting at non-point");
  } else return 0;
}

void testcase() {
  int n; std::cin >> n;
  std::vector<RayInfo> rays; rays.reserve(n);
  for (int i = 0; i < n; i++) {
    long y0, x1, y1; std::cin >> y0 >> x1 >> y1;
    rays.push_back(RayInfo{i, y0, x1, y1});
  }

  std::sort(rays.begin(), rays.end());
  std::vector<int> survivor_indices; survivor_indices.reserve(n);
  RayInfo last_survivor = rays[0];
  for (int i = 1; i < n; i++) {
    RayInfo cur = rays[i];
    int res = intersect_rays(last_survivor, cur);
    if (res == 0) {
      survivor_indices.push_back(last_survivor.index);
      last_survivor = cur;
    } else if (res == 2) {
      last_survivor = cur;
    }
  }

  survivor_indices.push_back(last_survivor.index);
  std::sort(survivor_indices.begin(), survivor_indices.end());
  for (int i: survivor_indices) std::cout << i << " ";
  std::cout << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
