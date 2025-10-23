#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef std::vector<std::vector<bool>> BM;
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

int encode(int i, int j, int w) {
  return i * w + j;
}

void testcase() {
  int w, h; std::cin >> w >> h;
  BM is_free(h, std::vector<bool>(w, false));
  int total_free = 0;
  for (int i = 0; i < h; i++)
    for (int j = 0; j < w; j++) {
      char c; std::cin >> c;
      if (c == '.') {
	is_free[i][j] = true;
	total_free++;
      }
    }

  int n = w * h;
  // vertex n: source, vertex n + 1: sink
  graph G(2 + n);
  edge_adder adder(G);
  for (int i = 0; i < h; i++)
    for (int j = 0; j < w; j++) {
      if (!is_free[i][j]) continue;
      int cur = encode(i, j, w);
      if ((i + j) % 2 == 0) {
	adder.add_edge(n, cur, 1);
	if (i - 1 >= 0) adder.add_edge(cur, encode(i - 1, j, w), 1);
	if (i + 1 < h) adder.add_edge(cur, encode(i + 1, j, w), 1);
	if (j - 1 >= 0) adder.add_edge(cur, encode(i, j - 1, w), 1);
	if (j + 1 < w) adder.add_edge(cur, encode(i, j + 1, w), 1);
      } else adder.add_edge(cur, n + 1, 1);
    }

  long flow = boost::push_relabel_max_flow(G, n, n + 1);
  std::cout << ((flow * 2 == total_free) ? "yes\n" : "no\n");
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
