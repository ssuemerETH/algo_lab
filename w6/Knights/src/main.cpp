#include <iostream>
// BGL include
#include <boost/graph/adjacency_list.hpp>
// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
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

int in(int i, int j, int w, int h) { return i * w + j; }
int out(int i, int j, int w, int h) { return i * w + j + w * h; }

void testcase() {
  int m, n, k, c; std::cin >> m >> n >> k >> c;
  // 0..(m * n - 1): in vertices, (m * n)...(2 * m * n - 1): out vertices
  graph G(2 * m * n + 2); edge_adder adder(G);
  int source = 2 * m * n; int sink = 2 * m * n + 1;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) {
      int cur_out = out(i, j, m, n);
      int cur_in = in(i, j, m, n);
      if (i > 0) adder.add_edge(cur_out, in(i - 1, j, m, n), 1);
      else adder.add_edge(cur_out, sink, 1);
      if (i < n - 1) adder.add_edge(cur_out, in(i + 1, j, m, n), 1);
      else adder.add_edge(cur_out, sink, 1);
      if (j > 0) adder.add_edge(cur_out, in(i, j - 1, m, n), 1);
      else adder.add_edge(cur_out, sink, 1);
      if (j < m - 1) adder.add_edge(cur_out, in(i, j + 1, m, n), 1);
      else adder.add_edge(cur_out, sink, 1);

      adder.add_edge(cur_in, cur_out, c);
    }

  for (int i = 0; i < k; i++) {
    int col, row; std::cin >> col >> row;
    adder.add_edge(source, in(row, col, m, n), 1);
  }

  long flow = boost::push_relabel_max_flow(G, source, sink);
  std::cout << flow << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
