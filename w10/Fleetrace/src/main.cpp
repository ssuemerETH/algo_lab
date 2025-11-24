#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;

// new: weightmap for costs
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
boost::property<boost::edge_capacity_t, long,
boost::property<boost::edge_residual_capacity_t, long,
boost::property<boost::edge_reverse_t, traits::edge_descriptor,
boost::property<boost::edge_weight_t, long>>>>> graph;

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
  graph &G;
public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost; w_map[rev_e] = -cost; // new assign cost
    // new negative cost
  }
};

void testcase() {
  int b, s, p; std::cin >> b >> s >> p;
  graph G(2 * (b + s) + 2); edge_adder adder(G);
  int source = 2 * (b + s); int sink = 2 * (b + s) + 1;
  
  long max_w = 50;

  // i-th original boat at: i
  // i-th original sailor at: b + i
  // i-th copy boat at: b + s + i
  // i-th copy sailor at: 2 * b + s + i

  // connect original vertices to copies
  for (int i = 0; i < b; i++) {
    adder.add_edge(i, b + s + i, 1, max_w);
    adder.add_edge(source, i, 1, 0);
    adder.add_edge(b + s + i, sink, 1, 0);
  }

  for (int i = 0; i < s; i++) {
    adder.add_edge(2 * b + s + i, b + i, 1, max_w);
    adder.add_edge(b + i, sink, 1, 0);
    adder.add_edge(source, 2 * b + s + i, 1, 0);
  }
  
  for (int i = 0; i < p; i++) {
    int bi, si, ci; std::cin >> bi >> si >> ci;
    adder.add_edge(bi, b + si, 1, max_w - ci);
    adder.add_edge(2 * b + s + si, b + s + bi, 1, max_w);
  }

  boost::successive_shortest_path_nonnegative_weights(G, source, sink);
  long cost = boost::find_flow_cost(G);
  long res = 50 * (b + s) - cost;

  auto c_map = boost::get(boost::edge_capacity, G);
  auto r_map = boost::get(boost::edge_reverse, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  
  int s_flow = 0;
  out_edge_it e, eend;
  for (boost::tie(e, eend) = boost::out_edges(boost::vertex(source, G), G); e != eend; ++e)
    s_flow += c_map[*e] - rc_map[*e];
  // std::cout << "total vertices: " << (b + s) << " total flow: " << s_flow << "\n";
  std::cout << res << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
