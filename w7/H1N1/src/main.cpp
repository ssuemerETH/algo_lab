#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <vector>
#include <queue>

#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/adjacency_list.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<long ,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
boost::no_property,
boost::property<boost::edge_weight_t, long>
> graph;

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;

void testcase(int n) {
  std::vector<K::Point_2> pts; pts.reserve(n);
  for (int i = 0; i < n; i++) {
    long xi, yi; std::cin >> xi >> yi;
    pts.push_back(K::Point_2(xi, yi));
  }

  Triangulation tri;
  tri.insert(pts.begin(), pts.end());
  
  long fi = 0;
  for (Face_iterator f = tri.finite_faces_begin(); f != tri.finite_faces_end(); f++)
    f->info() = fi++;
  
  // at this point, fi stores the number of finite faces
  graph G(fi + 1);
  // infinite face at index fi
  
  for (Face_iterator f = tri.finite_faces_begin(); f != tri.finite_faces_end(); f++) {
    long find = f->info();
    long best_w_for_inf_nbor = -1;
    for (int i = 0; i < 3; i++) {
      long w = CGAL::squared_distance(f->vertex((i + 1) % 3)->point(), f->vertex((i + 2) % 3)->point());
      if (tri.is_infinite(f->neighbor(i))) 
	best_w_for_inf_nbor = std::max(best_w_for_inf_nbor, w);
      else {
	long gind = f->neighbor(i)->info();
	if (find < gind) { 
	  boost::add_edge(find, gind, -w, G);
	}
      }
    }
    
    if (best_w_for_inf_nbor != -1)
      boost::add_edge(find, fi, -best_w_for_inf_nbor, G);
  }

  std::vector<edge_desc> mst;
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
  auto weight_map = boost::get(boost::edge_weight, G);
  
  std::vector<std::vector<std::pair<long, long>>> adj(fi + 1, std::vector<std::pair<long, long>>());
  for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); it++) {
    long u = boost::source(*it, G); long v = boost::target(*it, G);
    long w = -weight_map[*it];
    adj[u].push_back(std::make_pair(v, w));
    adj[v].push_back(std::make_pair(u, w));
  }

  // -2: not visited, -1: infinity, other values are actual bottlenecks
  std::vector<long> bottleneck_w(fi + 1, -2);
  bottleneck_w[fi] = -1;
  std::queue<long> q;
  q.push(fi);
  while (!q.empty()) {
    long x = q.front(); q.pop();
    for (const std::pair<long, long> &nbor: adj[x]) {
      long v = nbor.first; long w = nbor.second;
      if (bottleneck_w[v] == -2) {
	q.push(v);
	bottleneck_w[v] = (bottleneck_w[x] == -1) ? w : std::min(bottleneck_w[x], w);
      }
    }
  }

  int m; std::cin >> m;
  for (int i = 0; i < m; i++) {
    long xi, yi, d; std::cin >> xi >> yi >> d;
    K::Point_2 p(xi, yi);
    auto q = tri.nearest_vertex(p);
    long dist = CGAL::squared_distance(p, q->point());
    if (dist < d) {
      std::cout << "n";
      continue;
    }
    Triangulation::Face_handle fh = tri.locate(p);
    long ind = fi;
    if (!tri.is_infinite(fh)) ind = fh->info();
    bool possible = bottleneck_w[ind] == -1 || (4 * d <= bottleneck_w[ind]);
    std::cout << (possible ? "y" : "n");
  }
  
  std::cout << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n; std::cin >> n;
  while (n != 0) {
    testcase(n); std::cin >> n;
  }
  return 0;
}
