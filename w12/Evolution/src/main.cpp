#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

struct Query {
  int query_index;
  int species_index;
  int age_limit;

  bool operator<(const Query& other) const {
    return age_limit < other.age_limit;
  }
};

struct Answer {
  int query_index;
  int answer_species_index;

  bool operator<(const Answer& other) const {
    return query_index < other.query_index;
  }
};

void testcase() {
  int n, q; std::cin >> n >> q;
  std::vector<std::string> ind_to_str; ind_to_str.reserve(n);
  std::map<std::string, int> str_to_int;
  std::vector<int> age; age.reserve(n);
  for (int i = 0; i < n; i++) {
    std::string s; int a; std::cin >> s >> a;
    ind_to_str.push_back(s);
    age.push_back(a);
    str_to_int[s] = i;
  }

  // -1 means no parent, LUCA
  std::vector<int> parent(n, -1);
  for (int i = 0; i < n - 1; i++) {
    std::string s, p; std::cin >> s >> p;
    parent[str_to_int[s]] = str_to_int[p];
  }


  std::vector<Query> queries; queries.reserve(q);
  for (int i = 0; i < q; i++) {
    std::string s; int b; std::cin >> s >> b;
    queries.push_back({i, str_to_int[s], b});
  }

  std::sort(queries.begin(), queries.end());

  // -1 means no pointer yet
  std::vector<int> last_pointers(n, -1);
  std::vector<Answer> answers; answers.reserve(q);
  
  for (int i = 0; i < q; i++) {
    Query query = queries[i];
    int start = query.species_index;
    int query_index = query.query_index;
    int age_limit = query.age_limit;

    int cur = start;
    int next;
    if (last_pointers[cur] != -1) next = last_pointers[cur];
    else next = parent[cur];

    while (next != -1 && age[next] <= age_limit) {
      cur = next;
      if (last_pointers[cur] != -1) next = last_pointers[cur];
      else next = parent[cur];
    }

    int cur_res = cur;

    // now, make all vertices traversed above point to cur_res
    cur = start;
    if (last_pointers[cur] != -1) next = last_pointers[cur];
    else next = parent[cur];
    if (cur != cur_res) last_pointers[cur] = cur_res;
    while (next != -1 && age[next] <= age_limit) {
      cur = next;
      if (last_pointers[cur] != -1) next = last_pointers[cur];
      else next = parent[cur];

      if (cur != cur_res) last_pointers[cur] = cur_res;
    }

    answers.push_back({query_index, cur_res});
  }

  std::sort(answers.begin(), answers.end());
  for (const Answer& a: answers) 
    std::cout << ind_to_str[a.answer_species_index] << " ";
  std::cout << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
