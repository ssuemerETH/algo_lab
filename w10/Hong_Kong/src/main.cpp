#include <iostream>
#include <vector>
#include <queue>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
			      boost::no_property, boost::property<boost::edge_weight_t, K::FT>
			      > graph;

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;

typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

typedef Triangulation::Finite_faces_iterator Face_iterator;

typedef std::vector<std::vector<std::pair<int, K::FT>>> ADJ;

void testcase() {
  int n, m; K::FT r; std::cin >> n >> m >> r;
  Triangulation t;
  std::vector<K::Point_2> pts; pts.reserve(n);
  for (int i = 0; i < n; i++) {
    K::FT x, y; std::cin >> x >> y;
    pts.push_back(K::Point_2(x, y));
  }

  t.insert(pts.begin(), pts.end());
  int counter = 0;
  for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f)
    f->info() = counter++;

  // at this point, counter stores the number of finite faces
  // infinite face has index counter
  // also model INF as the sink for take off available faces
  const int INF = counter;
  graph G(INF + 1);

  for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) {
    int cur_ind = f->info();

    // First add edge to sink
    K::FT cap = CGAL::squared_distance(f->vertex(0)->point(), t.circumcenter(f));
    K::FT best_w_for_inf_nbor = cap;

    for (int i = 0; i < 3; i++) {
      K::Point_2 p = f->vertex((i + 1) % 3)->point();
      K::Point_2 q = f->vertex((i + 2) % 3)->point();
      K::FT bn = CGAL::squared_distance(p, q);
      auto nbor_face = f->neighbor(i);
      if (t.is_infinite(nbor_face)) best_w_for_inf_nbor = std::max(best_w_for_inf_nbor, bn);
      else {
	int nbor_ind = nbor_face->info();
	if (cur_ind < nbor_ind) boost::add_edge(cur_ind, nbor_ind, -bn, G);
      }
    }

    boost::add_edge(cur_ind, INF, -best_w_for_inf_nbor, G);
  }

  std::vector<edge_desc> mst;
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));

  ADJ adj(INF + 1, std::vector<std::pair<int, K::FT>>());
  auto weight_map = boost::get(boost::edge_weight, G);
  for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
    int u = boost::source(*it, G); int v = boost::target(*it, G);
    K::FT w = -weight_map[*it];
    adj[u].push_back(std::make_pair(v, w));
    adj[v].push_back(std::make_pair(u, w));
  }

  std::vector<K::FT> bottleneck_w(INF + 1, K::FT(-2));
  bottleneck_w[INF] = K::FT(-1);
  std::queue<int> q;
  q.push(INF);
  while (!q.empty()) {
    int x = q.front(); q.pop();
    for (const std::pair<int, K::FT> &nbor: adj[x]) {
      int v = nbor.first; K::FT w = nbor.second;
      if (bottleneck_w[v] == -2) {
        q.push(v);
        bottleneck_w[v] = (bottleneck_w[x] == -1) ? w : std::min(bottleneck_w[x], w);
      }
    }
  }

  
  for (int i = 0; i < m; i++) {
    long x, y; K::FT s; std::cin >> x >> y >> s;
    K::Point_2 cur(x, y);
    K::Point_2 nearest = t.nearest_vertex(cur)->point();
    if (CGAL::squared_distance(cur, nearest) < (r + s) * (r + s)) {
      std::cout << "n";
    } else {
      Triangulation::Face_handle fh = t.locate(cur);
      if (t.is_infinite(fh)) std::cout << "y";
      else {
	int ind = fh->info();
	K::FT cur_cap = 4 * (r + s) * (r + s);
	if (cur_cap <= bottleneck_w[ind]) std::cout << "y";
	else std::cout << "n";
      }
    }
  }

  std::cout << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
