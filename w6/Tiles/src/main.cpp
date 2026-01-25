#include <iostream>
#include <vector>
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

int enc(int i, int j, int w) { return i * w + j; }

void testcase() {
  int w, h; std::cin >> w >> h;
  graph G(w * h + 2); edge_adder adder(G);
  int source = w * h; int sink = w * h + 1;
  for (int i = 0; i < h; i++)
    for (int j = i % 2; j < w; j += 2) {
      int cur = enc(i, j, w);
      if (i > 0) adder.add_edge(cur, enc(i - 1, j, w), 1);
      if (i < h - 1) adder.add_edge(cur, enc(i + 1, j, w), 1);
      if (j > 0) adder.add_edge(cur, enc(i, j - 1, w), 1);
      if (j < w - 1) adder.add_edge(cur, enc(i, j + 1, w), 1);
    }

  int empty = 0;
  for (int i = 0; i < h; i++)
    for (int j = 0; j < w; j++) {
      char c; std::cin >> c;
      if (c == '.') {
	empty++;
	if ((i + j) % 2 == 0) adder.add_edge(source, enc(i, j, w), 1);
	else adder.add_edge(enc(i, j, w), sink, 1);
      }
    }

  long flow = boost::push_relabel_max_flow(G, source, sink);
  std::cout << ((2 * flow == empty) ? "yes\n" : "no\n");
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
