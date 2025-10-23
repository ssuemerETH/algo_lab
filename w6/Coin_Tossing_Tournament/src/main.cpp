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
  int n, m; std::cin >> n >> m;
  // player i at index i, game j at index n + j, source at index n + m, sink at index n + m + 1
  graph G(n + m + 2);
  edge_adder adder(G);
  std::vector<int> cur_scores(n, 0);

  int unplayed = 0;
  for (int i = 0; i < m; i++) {
    int a, b, c; std::cin >> a >> b >> c;
    if (c == 0) {
      unplayed++;
      adder.add_edge(n + m, n + i, 1);
      adder.add_edge(n + i, a, 1);
      adder.add_edge(n + i, b, 1);
    } else if (c == 1) cur_scores[a]++;
    else cur_scores[b]++;
  }

  bool too_few = false;
  int total_needs = 0;
  for (int i = 0; i < n; i++) {
    int si; std::cin >> si;
    if (cur_scores[i] > si) {
      too_few = true;
      continue;
    }

    int i_needs = si - cur_scores[i];
    total_needs += i_needs;
    adder.add_edge(i, n + m + 1, i_needs);
  }

  if (too_few) {
    std::cout << "no\n";
    return;
  }
  
  long flow = boost::push_relabel_max_flow(G, n + m, n + m + 1);
  std::cout << ((flow == total_needs && unplayed == total_needs) ? "yes\n" : "no\n");
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
