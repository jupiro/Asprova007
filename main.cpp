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

struct KKT89
{
  int r_len, r_time, id;
  KKT89(int r_len, int r_time, int id) : r_len(r_len), r_time(r_time), id(id) {}
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
void solve()
{
  start = std::chrono::system_clock::now();
  int m, S, T, L; cin >> m >> S >> T >> L;
  std::vector t(m, std::vector<int>(S));
  for (int i = 0; i < m; ++i)
  {
    for (int j = 0; j < S; ++j)
    {
      cin >> t[i][j];
    }
  }
  std::vector<int> x(S), n(m);
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
  const auto inin = n;
  const int ALL = std::accumulate(n.begin(), n.end(), 0);
  std::vector<std::deque<KKT89>> vq(S);
  std::vector<int> exe_idx(S), cnt(m), loss(m);
  auto run = [&](const std::vector<int> &v, bool get_losstime = false)->std::pair<double, int>
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
  };
  std::vector<int> res;
  std::vector<int> base;
  std::vector pair_loss(m, std::vector(m, std::vector<int>(m)));
  {
    std::vector<int> v;
    for (int i = 0; i < m; ++i)
    {
      v.emplace_back(i);
      for (int j = 0; j < m; ++j)
      {
        v.emplace_back(j);
        for (int k = 0; k < m; ++k)
        {
          v.emplace_back(k);
          pair_loss[i][j][k] = run(v, true).second;
          v.pop_back();
        }
        v.pop_back();
      }
      v.pop_back();
    }
  }
  auto annealing = [&](std::vector<int> &v, double d_time = 800, bool climing = false, double start_temp = 1000, double end_temp = 0.01)
  {
    auto start = std::chrono::system_clock::now();
    auto [best_get_score, best_c] = run(v);
    // int pre_get_time = best_get_time;
    // auto best_v = v;
    int id[2] = {};
    for (int jupi_loves_kkt = 0;; ++jupi_loves_kkt)
    {
      auto end = std::chrono::system_clock::now();
      const double time = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
      if(time > d_time)
        break;
      id[0] = xor64() % (int)best_c;
      id[1] = xor64() % (int)v.size();
      if(best_c < ALL)
      {
        id[1] = xor64() % ((int)v.size() - best_c) + best_c;
      }
      std::swap(v[id[0]], v[id[1]]);
      auto [get_score, get_c] = run(v);
      if(climing)
      {
        if(get_score < best_get_score)
        {
          std::swap(v[id[0]], v[id[1]]);
        }
        else
        {
          best_get_score = get_score;
          best_c = get_c;
        }
      }
      // else
      // {
      //   const double temp = start_temp + (end_temp - start_temp) * time / d_time;
      //   const double prob = std::exp((pre_get_time - get_time) / temp);
      //   if(get_time < best_get_time)
      //   {
      //     best_v = v;
      //     best_get_time = get_time;
      //     pre_get_time = get_time;
      //   }
      //   else if(prob < (double)(xor64() % inf) / (double)inf)
      //   {
      //     std::swap(v[id1], v[id2]);
      //   }
      //   else
      //   {
      //     pre_get_time = get_time;
      //   }
      // }
    }
    // if(not climing)
    //   std::swap(v, best_v);
  };
  {
    int s1 = std::min_element(n.begin(), n.end()) - n.begin();
    int s2 = std::max_element(n.begin(), n.end()) - n.begin();
    res.emplace_back(s1);
    res.emplace_back(s2);
    n[s1] -= 1;
    n[s2] -= 1;
    while((int)res.size() < ALL)
    {
      int min_loss = inf;
      double max_par = 0;
      int id = -1;
      for (int i = 0; i < m; ++i)
      {
        if(n[i] > 0)
        {
          if(chmin(min_loss, pair_loss[s1][s2][i]))
          {
            max_par = (double)n[i] / (double)inin[i];
            id = i;
          }
          else if(min_loss == pair_loss[s1][s2][i])
          {
            if(chmax(max_par, (double)n[i] / (double)inin[i]))
            {
              id = i;
            }
          }
        }
      }
      res.emplace_back(id);
      s1 = s2;
      s2 = id;
      n[id] -= 1;
    }
  }
  // annealing(res, 1400, true);
  auto [bs, bc] = run(res);
  if(bs < 0.98)
  {
    auto pres = res;
    auto pbs = bs;
    auto pbc = bc;
    res.clear();
    n = inin;
    {
      int s1 = std::min_element(n.begin(), n.end()) - n.begin();
      int s2 = std::max_element(n.begin(), n.end()) - n.begin();
      res.emplace_back(s1);
      res.emplace_back(s2);
      n[s1] -= 1;
      n[s2] -= 1;
      while((int)res.size() < ALL)
      {
        int min_loss = inf;
        double max_par = 0;
        int id = -1;
        if((int)res.size() < 100)
        {
          for (int i = 0; i < m; ++i)
          {
            if(n[i] > 0)
            {
              if(chmin(min_loss, pair_loss[s1][s2][i]))
              {
                max_par = (double)n[i] / (double)inin[i];
                id = i;
              }
              else if(min_loss == pair_loss[s1][s2][i])
              {
                if(chmax(max_par, (double)n[i] / (double)inin[i]))
                {
                  id = i;
                }
              }
            }
          }
        }
        else
        {
          for (int i = 0; i < m; ++i)
          {
            if(n[i] > 0)
            {
              chmin(min_loss, pair_loss[s1][s2][i]);
            }
          }
          for (int i = 0; i < m; ++i)
          {
            if(n[i] > 0 and pair_loss[s1][s2][i] <= min_loss + 10)
            {
              if(chmax(max_par, (double)n[i] / (double)inin[i]))
              {
                id = i;
              }
            }
          }
        }
        res.emplace_back(id);
        s1 = s2;
        s2 = id;
        n[id] -= 1;
      }
    }
    annealing(res, 1400, true);
    std::tie(bs, bc) = run(res);
    if(bs < pbs)
    {
      res = pres;
      bs = pbs;
      bc = pbc;
    }
  }
  res.resize(bc);
  // cout << "bs:" << bs << endl;
  cout << res.size() << "\n";
  for (int i = 0; i < (int)res.size(); ++i)
  {
    cout << res[i] + 1 << " \n"[i + 1 == (int)res.size()];
  }
}
int main(int argc, char *argv[])
{
  std::cin.tie(nullptr);
  std::ios::sync_with_stdio(false);

  if(argc > 1)
  {
    start_temp1 = std::stod(std::string(argv[1]));
    end_temp1 = std::stod(std::string(argv[2]));
    start_temp2 = std::stod(std::string(argv[3]));
    end_temp2 = std::stod(std::string(argv[4]));
  }
  int kkt = 1; 
  // cin >> kkt;
  while(kkt--)
    solve();
  return 0;
  
}