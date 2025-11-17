///4
#include <iostream>
#include <vector>
#include <cassert>

typedef std::vector<std::vector<int>> IM2D;
typedef std::vector<std::vector<IM2D>> IM4D;
typedef std::vector<std::vector<IM4D>> IM6D;

int num_diff(int t1, int t2, int t3, int k) {
  std::vector<bool> mark(k, false);
  if (t1 < k) mark[t1] = true;
  if (t2 < k) mark[t2] = true;
  if (t3 < k) mark[t3] = true;
  
  int res = 0;
  for (bool b: mark)
    if (b) res++;
  return res;
}

void testcase2(int, int, const std::vector<int>&);
void testcase3(int, int, const std::vector<int>&);

void testcase() {
  int n, k, m; std::cin >> n >> k >> m;
  std::vector<int> x; x.reserve(n);
  for (int i = 0; i < n; i++) {
    int xi; std::cin >> xi;
    x.push_back(xi);
  }
  
  if (m == 2) testcase2(n, k, x);
  else testcase3(n, k, x);
}

void testcase2(int n, int k, const std::vector<int> &x) {
  int max_abs_imb = 10;
  int snd_dim_size = 2 * max_abs_imb + 1;

  // dp[i][q][tn][ts] = max. achievable excitement if x[i..(n - 1)] is available, q = #sent north - #sent south + 10
  // tn is the type of person sent north last, and ts is the type of person sent south last
  // -1 if not possible without violating constraints
  IM4D dp(n + 1, std::vector<IM2D>(snd_dim_size, IM2D(k + 1, std::vector<int>(k + 1, -1))));
  
  // tn/ts == k indicates that no one was sent north/south
  for (int q = 0; q < snd_dim_size; q++) 
    for (int tn = 0; tn <= k; tn++)
      for (int ts = 0; ts <= k; ts++)
        dp[n][q][tn][ts] = 0;
  
  for (int i = n - 1; i >= 0; i--) 
    for (int q = 0; q < snd_dim_size; q++) 
      for (int tn = 0; tn <= k; tn++)
        for (int ts = 0; ts <= k; ts++) {
            // compute dp[i][q][tn][ts]
            int tcur = x[i];
            // compute best amount by sending north
            int north_max = -1;
            // imbalance north - south increases
            int new_actual_imbalance_north = q - max_abs_imb + 1;
            // calculate obtained excitement in this round
            int cur_exc_north = ((tcur == tn || tn == k) ? 1000 : 2000) - (1 << std::abs(new_actual_imbalance_north));
            if (cur_exc_north >= 0) {
              int rec_north = dp[i + 1][q + 1][tcur][ts];
              if (rec_north != -1) north_max = cur_exc_north + rec_north;
            }
            
            // compute best amount by sending south
            int south_max = -1;
            // imbalance north - south decreases
            int new_actual_imbalance_south = q - max_abs_imb - 1;
            // calculate obtained excitement in this round
            int cur_exc_south = ((tcur == ts || ts == k) ? 1000 : 2000) - (1 << std::abs(new_actual_imbalance_south));
            if (cur_exc_south >= 0) {
              int rec_south = dp[i + 1][q - 1][tn][tcur];
              if (rec_south != -1) south_max = cur_exc_south + rec_south;
            }
            
            dp[i][q][tn][ts] = std::max(north_max, south_max);
        }

  int res = dp[0][max_abs_imb][k][k];
  std::cout << res << "\n";
}

std::vector<int> decode_state(int b1, int b2, int t1, int t2, int i, const std::vector<int> &x, int k) {
  int tn, tnp, ts, tsp; tn = tnp = ts = tsp = k;
  if (i == 1)
    if (b1 == 1) tn = x[0];
    else ts = x[0];
  else if (i > 1) {
    if (b1 == 1 && b2 == 1) {
      tn = x[i - 1]; tnp = x[i - 2];
      ts = t1; tsp = t2;
    } else if (b1 == 1 && b2 == 0) {
      tn = x[i - 1]; tnp = t2;
      ts = x[i - 2]; tsp = t1;
    } else if (b1 == 0 && b2 == 1) {
      tn = x[i - 2]; tnp = t1;
      ts = x[i - 1]; tsp = t2;
    } else {
      tn = t1; tnp = t2;
      ts = x[i - 1]; tsp = x[i - 2];
    }
  }
  
  return {tn, tnp, ts, tsp};
}

std::vector<int> update_state(int b1, int b2, int t1, int t2, bool send_north, int i, int k, const std::vector<int> &x) {
  if (i == 0) {
    if (send_north) return {1, 1, k, k};
    else return {0, 0, k, k};
  }
  
  if (i == 1) {
    if (send_north) return {1, b1, k, k};
    else return {0, b1, k, k};
  }
  
  if (b1 == 1 && b2 == 1) {
    if (send_north) return {1, 1, t1, t2};
    else return {0, 1, x[i - 2], t1};
  } else if (b1 == 1 && b2 == 0) {
    if (send_north) return {1, 1, x[i - 2], t1};
    else return {0, 1, t2, x[i - 2]};
  } else if (b1 == 0 && b2 == 1) {
    if (send_north) return {1, 0, t2, x[i - 2]};
    else return {0, 0, x[i - 2], t1};
  } else {
    if (send_north) return {1, 0, x[i - 2], t1};
    else return {0, 0, t1, t2};
  }
}

int solve(int i, int q, int b1, int b2, int t1, int t2, IM6D &dp, const std::vector<int> &x, int k, int max_abs_imb) {
  // compute dp[l][q][b1][b2][t1][t2]
  if (dp[i][q][b1][b2][t1][t2] > -2)
    return dp[i][q][b1][b2][t1][t2];
 
  int tcur = x[i];
  std::vector<int> st = decode_state(b1, b2, t1, t2, i, x, k);
  int tn = st[0]; int tnp = st[1]; int ts = st[2]; int tsp = st[3];
  // compute best amount by sending north
  int north_max = -1;
  // imbalance north - south increases
  int new_actual_imbalance_north = q - max_abs_imb + 1;
  // calculate obtained excitement in this round
  int diff_types_north = num_diff(tn, tnp, tcur, k);
  int cur_exc_north = diff_types_north * 1000 - (1 << std::abs(new_actual_imbalance_north));

  if (cur_exc_north >= 0) {
    // updated state north
    std::vector<int> usn = update_state(b1, b2, t1, t2, true, i, k, x);
    int rec_north = solve(i + 1, q + 1, usn[0], usn[1], usn[2], usn[3], dp, x, k, max_abs_imb);
    if (rec_north != -1) north_max = cur_exc_north + rec_north;
  }

  // compute best amount by sending south
  int south_max = -1;
  // imbalance north - south decreases
  int new_actual_imbalance_south = q - max_abs_imb - 1;
  // calculate obtained excitement in this round
  int diff_types_south = num_diff(ts, tsp, tcur, k);
  int cur_exc_south = diff_types_south * 1000 - (1 << std::abs(new_actual_imbalance_south));
  if (cur_exc_south >= 0) {
    // updated state south
    std::vector<int> uss = update_state(b1, b2, t1, t2, false, i, k, x);
    int rec_south = solve(i + 1, q - 1, uss[0], uss[1], uss[2], uss[3], dp, x, k, max_abs_imb);
    if (rec_south != -1) south_max = cur_exc_south + rec_south;
  }
  
  dp[i][q][b1][b2][t1][t2] = std::max(north_max, south_max);
  return dp[i][q][b1][b2][t1][t2];
}

int solve_s(int i, int q, int tn, int tnp, int ts, int tsp, IM6D &dp, const std::vector<int> &x, int k, int max_abs_imb) {
  // compute dp[l][q][b1][b2][t1][t2]
  if (i == x.size()) return 0;
  
  if (dp[i][q][tn][tnp][ts][tsp] > -2)
    return dp[i][q][tn][tnp][ts][tsp];
 
  int tcur = x[i];
  // compute best amount by sending north
  int north_max = -1;
  // imbalance north - south increases
  int new_actual_imbalance_north = q - max_abs_imb + 1;
  // calculate obtained excitement in this round
  int diff_types_north = num_diff(tn, tnp, tcur, k);
  int cur_exc_north = diff_types_north * 1000 - (1 << std::abs(new_actual_imbalance_north));

  if (cur_exc_north >= 0) {
    int rec_north = solve_s(i + 1, q + 1, tcur, tn, ts, tsp, dp, x, k, max_abs_imb);
    if (rec_north != -1) north_max = cur_exc_north + rec_north;
  }

  // compute best amount by sending south
  int south_max = -1;
  // imbalance north - south decreases
  int new_actual_imbalance_south = q - max_abs_imb - 1;
  // calculate obtained excitement in this round
  int diff_types_south = num_diff(ts, tsp, tcur, k);
  int cur_exc_south = diff_types_south * 1000 - (1 << std::abs(new_actual_imbalance_south));
  if (cur_exc_south >= 0) {
    // updated state south
    int rec_south = solve_s(i + 1, q - 1, tn, tnp, tcur, ts, dp, x, k, max_abs_imb);
    if (rec_south != -1) south_max = cur_exc_south + rec_south;
  }
  
  dp[i][q][tn][tnp][ts][tsp] = std::max(north_max, south_max);
  return dp[i][q][tn][tnp][ts][tsp];
}

void testcase3(int n, int k, const std::vector<int> &x) {
  int max_abs_imb = 11;
  int snd_dim_size = 2 * max_abs_imb + 1;
  
  IM6D dp(n + 1, std::vector<IM4D>(snd_dim_size, IM4D(2, std::vector<IM2D>(2, IM2D(k + 1, std::vector<int>(k + 1, -2))))));

  for (int q = 0; q < snd_dim_size; q++)
    for (int b1 = 0; b1 < 2; b1++)
      for (int b2 = 0; b2 < 2; b2++)
        for (int t1 = 0; t1 <= k; t1++)
          for (int t2 = 0; t2 <= k; t2++)
            dp[n][q][b1][b2][t1][t2] = 0;
  
  int res = solve(0, max_abs_imb, 0, 0, k, k, dp, x, k, max_abs_imb);

  /*
  IM6D dp(n + 1, std::vector<IM4D>(snd_dim_size, IM4D(k + 1, std::vector<IM2D>(k + 1, IM2D(k + 1, std::vector<int>(k + 1, -2))))));
  
  int res = solve_s(0, max_abs_imb, k, k, k, k, dp, x, k, max_abs_imb);
  */
  std::cout << res << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}