#include <iostream>
// BGL include
#include <boost/graph/adjacency_list.hpp>
// BGL flow include *NEW*
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

void testcase() {
  int n, m; std::cin >> n >> m;
  // 0..(n - 1): players, n, ..., (n + m - 1): games
  graph G(n + m + 2); edge_adder adder(G);
  int source = n + m; int sink = n + m + 1;
  for (int i = 0; i < m; i++) adder.add_edge(source, n + i, 1);
  
  for (int i = 0; i < m; i++) {
    int a, b, c; std::cin >> a >> b >> c;
    if (c == 0 || c == 1) adder.add_edge(n + i, a, 1);
    if (c == 0 || c == 2) adder.add_edge(n + i, b, 1);
  }

  int total_s = 0;
  for (int i = 0; i < n; i++) {
    int si; std::cin >> si;
    total_s += si;
    adder.add_edge(i, sink, si);
  }

  if (total_s != m) {
    std::cout << "no\n";
    return;
  }

  long flow = boost::push_relabel_max_flow(G, source, sink);
  std::cout << (flow == m ? "yes\n" : "no\n");
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
