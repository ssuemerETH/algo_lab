#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

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

int vin(int i, int j, int w, int h) {return i * w + j;}
int vout(int i, int j, int w, int h) {return i * w + j + h * w;}

void testcase() {
  int w, h, k, C; std::cin >> w >> h >> k >> C;
  if (w == 0 || h == 0) {
    for (int q = 0; q < k; q++) { 
      int i, j; std::cin >> i >> j;
    }
    std::cout << 0 << "\n";
    return;
  }

  // vertices 0, ..., (h * w - 1) are in, vertices h * w, ..., (2 * h * w - 1) are out, 2 * h * w is source, 2 * h * w + 1 is sink
  graph G(2 * h * w + 2);
  edge_adder adder(G);
  int source = 2 * h * w;
  int sink = 2 * h * w + 1;

  for (int i = 0; i < h - 1; i++)
    for (int j = 0; j < w - 1; j++) {
      adder.add_edge(vout(i, j, w, h), vin(i + 1, j, w, h), 1);
      adder.add_edge(vout(i + 1, j, w, h), vin(i, j, w, h), 1);

      adder.add_edge(vout(i, j, w, h), vin(i, j + 1, w, h), 1);
      adder.add_edge(vout(i, j + 1, w, h), vin(i, j, w, h), 1);
    }

  for (int j = 0; j < w - 1; j++) {
    adder.add_edge(vout(h - 1, j, w, h), vin(h - 1, j + 1, w, h), 1);
    adder.add_edge(vout(h - 1, j + 1, w, h), vin(h - 1, j, w, h), 1);
  }

  for (int i = 0; i < h - 1; i++) {
    adder.add_edge(vout(i, w - 1, w, h), vin(i + 1, w - 1, w, h), 1);
    adder.add_edge(vout(i + 1, w - 1, w, h), vin(i, w - 1, w, h), 1);
  }

  for (int i = 0; i < h; i++)
    for (int j = 0; j < w; j++) {
      adder.add_edge(vin(i, j, w, h), vout(i, j, w, h), C);
      int outc = 0;
      if (i == 0) outc++;
      if (i == h - 1) outc++;
      if (j == 0) outc++;
      if (j == w - 1) outc++;
      if (outc > 0) adder.add_edge(vout(i, j, w, h), sink, outc);
    }

  for (int q = 0; q < k; q++) {
    int i, j; std::cin >> j >> i;
    adder.add_edge(source, vin(i, j, w, h), 1);
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
