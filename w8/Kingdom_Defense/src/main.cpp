#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <vector>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
boost::property<boost::edge_capacity_t, long,
boost::property<boost::edge_residual_capacity_t, long,
boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

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
  int l, p; std::cin >> l >> p;
  graph G(l + 2); // 0, ..., (l - 1) are given vertices, l: source, l + 1: sink
  edge_adder adder(G);
  int source = l; int sink = l + 1;
  std::vector<int> s, d; s.reserve(l); d.reserve(l);
  for (int i = 0; i < l; i++) {
    int si, di; std::cin >> si >> di;
    s.push_back(si); d.push_back(di);
  }

  for (int j = 0; j < p; j++) {
    int fj, tj, cj, Cj; std::cin >> fj >> tj >> cj >> Cj;
    d[fj] += cj; s[tj] += cj;
    adder.add_edge(fj, tj, Cj - cj);
  }

  long total_d = 0;
  for (int i = 0; i < l; i++) {
    total_d += d[i];
    adder.add_edge(source, i, s[i]);
    adder.add_edge(i, sink, d[i]);
  }
  
  long flow = boost::push_relabel_max_flow(G, source, sink);
  if (flow == total_d) std::cout << "yes\n";
  else std::cout << "no\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
