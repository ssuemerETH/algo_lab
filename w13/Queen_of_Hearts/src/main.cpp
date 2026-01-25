#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
boost::property<boost::edge_capacity_t, long,
boost::property<boost::edge_residual_capacity_t, long,
boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class edge_adder {
  graph &G;
public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

typedef boost::adjacency_list<
  boost::vecS,
  boost::vecS,
  boost::directedS,
  boost::no_property, // no vertex property
  boost::property <boost::edge_weight_t, int> // edge property (interior)
  > weighted_graph;


void testcase() {
  int r, m, d; std::cin >> r >> m >> d;
  std::vector<int> carrots;
  for (int i = 0; i < r; i++) {
    int ni, mi, ci; std::cin >> ni >> mi >> ci;
    weighted_graph G(ni);
    for (int j = 0; j < mi; j++) {
      int u, v; std::cin >> u >> v;
      boost::add_edge(u, v, 1, G);
    }

    std::vector<int> dist_map(ni); // exterior property
    boost::dijkstra_shortest_paths(G, 0,
			       boost::distance_map(boost::make_iterator_property_map(dist_map.begin(),
						   boost::get(boost::vertex_index, G))));
    carrots.push_back(std::max(0, ci - dist_map[ni - 1]));
  }

  // 0..(r + 1): in vertices, (r + 2)...(2r + 3): out vertices
  graph wfG(2 * (r + 2)), ufG(2 * (r + 2));
  edge_adder wf_adder(wfG), uf_adder(ufG);
  const int MAX = 600001;
  
  for (int i = 0; i < m; i++) {
    int u, v; std::cin >> u >> v;
    // add edge from out(u) to in(v) with max. cap
    wf_adder.add_edge(u + r + 2, v, MAX);
    uf_adder.add_edge(u + r + 2, v, MAX);
  }

  // add intra-vertex edges
  wf_adder.add_edge(0, r + 2, MAX);
  uf_adder.add_edge(0, r + 2, MAX);
  
  wf_adder.add_edge(r + 1, 2 * r + 3, MAX);
  uf_adder.add_edge(r + 1, 2 * r + 3, MAX);

  for (int i = 1; i <= r; i++) {
    wf_adder.add_edge(i, i + r + 2, carrots[i - 1]);
    uf_adder.add_edge(i, i + r + 2, carrots[i - 1] == 0 ? 0 : 1);
  }

  long c = boost::push_relabel_max_flow(wfG, 0, 2 * r + 3);
  long g = boost::push_relabel_max_flow(ufG, 0, 2 * r + 3);

  std::cout << c << " " << (d == 0 ? 0 : g) << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
