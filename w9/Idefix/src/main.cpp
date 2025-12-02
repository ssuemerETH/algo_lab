#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

typedef Triangulation::Finite_vertices_iterator Vertex_iterator;

struct DSU {
    std::vector<int> parent;
    std::vector<int> rank;         // optional but recommended
    std::vector<int> bones;        // bones[root] = number of bones in component root
    int max_bones = 0;

    DSU(int n) : parent(n), rank(n, 0), bones(n, 0) {
        for (int i = 0; i < n; i++) parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    // Merge components of a and b
    int merge(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return a;

        // union by rank
        if (rank[a] < rank[b])
            std::swap(a, b);

        parent[b] = a;
        if (rank[a] == rank[b])
            rank[a]++;

        // merge bone counts
        bones[a] += bones[b];
        bones[b] = 0;   // no longer root, so clear
        
        if (bones[a] > max_bones)
            max_bones = bones[a];

        return a;  // new root
    }

    // Manually increase bone count for component containing x
    void add_bone(int x) {
        int r = find(x);
        bones[r]++;
        if (bones[r] > max_bones)
            max_bones = bones[r];
    }
};


struct Event {
  bool bone_join;
  int u, v; // if bone_join == false, these are the vertices that touch
  int w; // if bone_join == true, this is the vertex that the bone joins to
  long q; // timestep

  bool operator<(const Event& other) const {
    if (q != other.q) return q < other.q;
    return bone_join > other.bone_join;
  }
};

void testcase() {
  int n, m, k; long s; std::cin >> n >> m >> s >> k;
  std::vector<K::Point_2> pts; pts.reserve(n);
  for (int i = 0; i < n; i++) {
    long x, y; std::cin >> x >> y;
    pts.push_back(K::Point_2(x, y));
  }

  std::vector<K::Point_2> bones; bones.reserve(m);
  for (int i = 0; i < m; i++) {
    long x, y; std::cin >> x >> y;
    bones.push_back(K::Point_2(x, y));
  }

  Triangulation t; t.insert(pts.begin(), pts.end());
  int counter = 0;
  for (Vertex_iterator vit = t.finite_vertices_begin(); vit != t.finite_vertices_end(); ++vit)
    vit->info() = counter++;

  std::vector<Event> events;
  for (const K::Point_2& bone: bones) {
    Triangulation::Vertex_handle nearest = t.nearest_vertex(bone);
    long timestep = 4L * CGAL::squared_distance(bone, nearest->point());
    events.push_back({true, -1, -1, nearest->info(), timestep});
  }

  for (Vertex_iterator vit = t.finite_vertices_begin(); vit != t.finite_vertices_end(); ++vit) {
    Triangulation::Vertex_circulator vc_start = t.incident_vertices(vit);
    Triangulation::Vertex_circulator vc = vc_start;
    int cur_ind = vit->info();
    
    if (vc != 0) {
      do {
	int other_ind = vc->info();
	if (cur_ind < other_ind) {
	  long timestep = CGAL::squared_distance(vit->point(), vc->point());
	  events.push_back({false, cur_ind, other_ind, -1, timestep});
	}
      } while (++vc != vc_start);
    }
  }

  std::sort(events.begin(), events.end());
  long res1 = -1;
  long res2 = -1;
  DSU uf(counter);
  for (const Event& e: events) {
    if (res1 == -1 && e.q > s) res1 = uf.max_bones;
    if (e.bone_join) uf.add_bone(e.w);
    else uf.merge(e.u, e.v);

    if (res2 == -1 && uf.max_bones >= k) res2 = e.q;
  }

  if (res1 == -1) res1 = uf.max_bones;
  std::cout << res1 << " " << res2 << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
