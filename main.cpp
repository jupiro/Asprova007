#include <bits/stdc++.h>
using ll = long long;
using std::cin;
using std::cout;
using std::endl;
std::mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());
template<class T> inline bool chmax(T& a, T b) { if (a < b) { a = b; return 1; } return 0; }
template<class T> inline bool chmin(T& a, T b) { if (a > b) { a = b; return 1; } return 0; }
const int inf = (int)1e9 + 7;
const long long INF = 1LL << 60;


int m, S, T, L;
std::vector<std::vector<int>> t;
std::vector<int> x, n, inin;
int pair_loss[41][41];
int triple_loss[21][21][21];
struct KKT89
{
  int r_len, r_time, id;
  KKT89(int r_len, int r_time, int id) : r_len(r_len), r_time(r_time), id(id) {}
};
std::vector<std::deque<KKT89>> vq;
std::vector<int> exe_idx, cnt, loss;

std::pair<double, int> run(const std::vector<int> &v, bool get_losstime = false)
{
  int real_time = 0;
  int run_time = 0;
  vq.clear(); vq.resize(S);
  exe_idx.clear(); exe_idx.resize(S);
  cnt.clear(); cnt.resize(m);
  loss.assign(m, 0);
  int push_time = 0;
  int idx = 0;
  int done = 0;
  while(real_time < L)
  {
    bool stop = false;
    if(push_time == 0 and idx < (int)v.size())
    {
      push_time += T;
      vq[0].emplace_back(x[0] + run_time, t[v[idx]][0], v[idx]);
      idx += 1;
    }
    int exe_time = inf;
    for (int i = 0; i < S; ++i)
    {
      auto &ei = exe_idx[i];
      while(ei < (int)vq[i].size() and vq[i][ei].r_time == 0)
        ei += 1;
      if(not vq[i].empty() and vq[i].front().r_len - run_time == 0 and vq[i].front().r_time > 0)
      {
        stop = true;
        loss[vq[i].front().id] += vq[i].front().r_time;
        if(exe_time == inf)
        {
          exe_time = vq[i].front().r_time;
        }
        else
          chmax(exe_time, vq[i].front().r_time);
      }
    }
    if(not stop)
    {
      for (int i = 0; i < S; ++i)
      {
        auto &ei = exe_idx[i];
        if(not stop and not vq[i].empty())
        {
          chmin(exe_time, vq[i][0].r_len - run_time);
        }
        if(i == S - 1 and ei < (int)vq[i].size())
        {
          chmin(exe_time, vq[i][ei].r_time);
        }
      }
      if(not stop and idx < (int)v.size())
      {
        chmin(exe_time, push_time);
      }
    }
    if(exe_time > L - real_time)
      break;
    if(idx < (int)v.size() and not stop)
    {
      push_time -= exe_time;
    }
    if(not stop)
      run_time += exe_time;
    real_time += exe_time;
    for (int i = S - 1; i >= 0; --i)
    {
      auto &ei = exe_idx[i];
      int r_exe_time = exe_time;
      while(ei < (int)vq[i].size() and r_exe_time > 0)
      {
        const int min = std::min(vq[i][ei].r_time, r_exe_time);
        vq[i][ei].r_time -= min;
        r_exe_time -= min;
        if(i == S - 1 and vq[i][ei].r_time == 0)
        {
          done += 1;
          cnt[vq[i][ei].id] += 1;
        }
        if(vq[i][ei].r_time == 0)
          ei += 1;
      }
      if(not vq[i].empty() and vq[i].front().r_len - run_time == 0 and vq[i].front().r_time == 0)
      {
        const int id = vq[i].front().id;
        vq[i].pop_front();
        if(i + 1 < S)
        {
          vq[i + 1].emplace_back(x[i + 1] + run_time, t[id][i + 1], id);
        }
        if(ei > 0)
          ei -= 1;
      }
    }
  }
  if(get_losstime)
  {
    return std::make_pair(0.0, real_time - run_time);
  }
  double score = 0;
  for (int i = 0; i < m; ++i)
  {
    score += std::sqrt((double)cnt[i] / (double)inin[i]);
  }
  score /= m;
  return std::make_pair(score, done);
}
struct Easy_KKT89_Cuteeeeeee
{
  double score;
  int loss_time;
  int add_id;
  int vector_id;
  Easy_KKT89_Cuteeeeeee(double score, int loss_time, int add_id, int vector_id) : score(score), loss_time(loss_time), add_id(add_id), vector_id(vector_id) {}
  // good < bad
  bool operator< (const Easy_KKT89_Cuteeeeeee &e) const
  {
    if(loss_time == e.loss_time)
    {
      return score > e.score;
    }
    else
    {
      return loss_time < e.loss_time;
    }
  }
  bool operator> (const Easy_KKT89_Cuteeeeeee &e) const
  {
    if(loss_time == e.loss_time)
    {
      return score < e.score;
    }
    else
    {
      return loss_time > e.loss_time;
    }
  }
};

struct KKT89_Cuteeeeeee
{
  int m;
  std::vector<int> done_n;
  double score;
  int loss_time;
  int p1, p2;
  int pre_add_id;
  int pre_vector_id;
  std::vector<int> last10;
  KKT89_Cuteeeeeee() : m(0) {}
  KKT89_Cuteeeeeee(int m) : m(m), done_n(m), score(0.0), loss_time(0.0), p1(-1), p2(-1), pre_add_id(-1), pre_vector_id(-1) {}

  KKT89_Cuteeeeeee add_return(const int add_id, const int vector_id) const
  {
    auto ret = *this;
    ret.score -= (double)done_n[add_id] / inin[add_id];
    ret.done_n[add_id] += 1;
    ret.score += (double)done_n[add_id] / inin[add_id];
    ret.last10.emplace_back(add_id);
    if((int)ret.last10.size() > 10)
    {
      ret.last10.erase(ret.last10.begin());
    }
    ret.loss_time += run(ret.last10, true).second;
    ret.p1 = p2;
    ret.p2 = add_id;
    ret.pre_add_id = add_id;
    ret.pre_vector_id = vector_id;
    return ret;
  }

  Easy_KKT89_Cuteeeeeee calc_change(const int add_id, const int vector_id)
  {
    if(done_n[add_id] == inin[add_id])
    {
      return Easy_KKT89_Cuteeeeeee(-inf, inf, add_id, vector_id);
    }
    int n_loss = loss_time;
    last10.emplace_back(add_id);
    n_loss += run(last10, true).second;
    last10.pop_back();
    double n_score = score;
    n_score -= (double)done_n[add_id] / inin[add_id];
    n_score += (double)(done_n[add_id] + 1) / inin[add_id];
    return Easy_KKT89_Cuteeeeeee(n_score, n_loss, add_id, vector_id);
  }
};

uint32_t xor64(void)
{
  static uint64_t x = 88172645463325252ULL;
  x = x ^ (x << 13); x = x ^ (x >> 7);
  return x = x ^ (x << 17);
}

double start_temp1 = 1, end_temp1 = 0.01, start_temp2 = 1, end_temp2 = 0.01;
std::chrono::system_clock::time_point start, end;
const double deadline = 1870;

void solve_m20()
{
  start = std::chrono::system_clock::now();
  inin = n;
  const int ALL = std::accumulate(n.begin(), n.end(), 0);
  std::vector<std::deque<KKT89>> vq(S);
  std::vector<int> exe_idx(S), cnt(m), loss(m);
  std::vector<int> res;
  std::vector<int> base;


  const int beam_width = 3;
  std::vector<std::vector<KKT89_Cuteeeeeee>> beam(ALL + 1);
  beam[0].resize(1);
  beam[0][0] = KKT89_Cuteeeeeee(m);
  std::priority_queue<Easy_KKT89_Cuteeeeeee> pq;
  for (int jupi = 0; jupi < ALL; ++jupi)
  {
    auto &v = beam[jupi];
    for (int i = 0; i < (int)v.size(); ++i)
    {
      if(v[i].m == 0)
        continue;
      for (int j = 0; j < m; ++j)
      {
        if(v[i].done_n[j] == inin[j])
          continue;
        pq.emplace(v[i].calc_change(j, i));
        if((int)pq.size() > beam_width)
          pq.pop();
      }
    }
    beam[jupi + 1].reserve(pq.size());
    while(not pq.empty())
    {
      beam[jupi + 1].push_back(v[pq.top().vector_id].add_return(pq.top().add_id, pq.top().vector_id));
      pq.pop();
    }
  }
  int cur = (int)beam[ALL].size() - 1;
  for (int i = ALL - 1; i >= 0; i--)
  {
    res.emplace_back(beam[i + 1][cur].pre_add_id);
    cur = beam[i + 1][cur].pre_vector_id;
  }
  std::reverse(res.begin(), res.end());
  auto [bs, bc] = run(res);
  res.resize(bc);
  cout << res.size() << "\n";
  for (int i = 0; i < bc; ++i)
  {
    cout << res[i] + 1 <<" \n"[i + 1== bc];
  }
}

int main([[maybe_unused]]int argc, [[maybe_unused]]char *argv[])
{
  std::cin.tie(nullptr);
  std::ios::sync_with_stdio(false);

  start = std::chrono::system_clock::now();
  cin >> m >> S >> T >> L;
  t.assign(m, std::vector<int>(S));
  for (int i = 0; i < m; ++i)
  {
    for (int j = 0; j < S; ++j)
    {
      cin >> t[i][j];
    }
  }
  x.resize(S), n.resize(m);
  for (int i = 0; i < S; ++i)
  {
    cin >> x[i];
  }
  for (int i = S - 1; i > 0; --i)
  {
    x[i] -= x[i - 1];
  }
  for (int i = 0; i < m; ++i)
  {
    cin >> n[i];
  }
  int kkt = 1; 
  // cin >> kkt;
  while(kkt--)
  {
    if(m == 20)
      solve_m20();
  }
  return 0;
  
}