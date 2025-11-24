#include <iostream>
#include <boost/graph/adjacency_list.hpp>
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
  int n, b, k, m; std::cin >> n >> b >> k >> m;
  graph G1(n + 2), G2(n + 2); int s = n; int t = n + 1;
  edge_adder adder1(G1), adder2(G2);
  int total_a = 0; int total_f = 0; 
  for (int i = 0; i < n; i++) {
    int ai; std::cin >> ai;
    
    if (ai > 0) {
      total_a += ai;
      adder1.add_edge(s, i, ai);
      adder2.add_edge(s, i, ai);
    }

    if (ai < 0) {
      total_a -= ai;
      adder1.add_edge(i, t, -ai);
      adder2.add_edge(i, t, -ai);
    }
  }

  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++) {
      int fij; std::cin >> fij;
      total_f += fij;
      if (i != j) {
	adder1.add_edge(i, j, fij);
	adder2.add_edge(i, j, fij);
      }
    }

  int INF = total_a + total_f;
  adder1.add_edge(s, k, INF);
  adder1.add_edge(m, t, INF);

  adder2.add_edge(k, t, INF);
  adder2.add_edge(s, m, INF);
  
  long res_A = boost::push_relabel_max_flow(G1, s, t);
  long res_B = boost::push_relabel_max_flow(G2, s, t);
  long res = std::min(res_A, res_B);

  if (b == 0) res = total_a + total_f / 2 - 2 * res;
  std::cout << res << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
