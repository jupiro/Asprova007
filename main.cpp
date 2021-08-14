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
  std::vector<int> exe_idx(S), cnt(m), loss(m), use_id, next_erase, next_add, next_use;
  auto run = [&](const std::vector<int> &v)->std::pair<double, int>
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
    double score = 0;
    for (int i = 0; i < m; ++i)
    {
      score += std::sqrt((double)cnt[i] / (double)inin[i]);
    }
    score /= m;
    if(done == 1000)
    {
      score = 2.0 - (double)real_time / L;
    }
    return std::make_pair(score, done);
  };

  auto run_short = [&](const std::vector<int> &v)->int
  {
    int real_time = 0;
    int run_time = 0;
    vq.clear(); vq.resize(S);
    exe_idx.clear(); exe_idx.resize(S);
    // lane にいる数
    cnt.clear(); cnt.resize(S);
    loss.assign(m, 0);
    use_id.clear();
    int push_time = 0;
    int idx = 0;
    int done = 0;
    while(real_time < L)
    {
      next_erase.clear();
      next_add.clear();
      next_use.clear();
      bool stop = false;
      if(push_time == 0 and idx < (int)v.size())
      {
        push_time += T;
        vq[0].emplace_back(x[0] + run_time, t[v[idx]][0], v[idx]);
        cnt[0] += 1;
        if(cnt[0] == 1)
          use_id.emplace_back(0);
        idx += 1;
      }
      std::sort(use_id.begin(), use_id.end());
      use_id.erase(std::unique(use_id.begin(), use_id.end()), use_id.end());
      int exe_time = inf;
      for (const auto &i : use_id)
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
        for (const auto &i : use_id)
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
      for (int kkt = (int)use_id.size() - 1; kkt >= 0; --kkt)
      {
        const int i = use_id[kkt];
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
          }
          if(vq[i][ei].r_time == 0)
            ei += 1;
        }
        if(not vq[i].empty() and vq[i].front().r_len - run_time == 0 and vq[i].front().r_time == 0)
        {
          const int id = vq[i].front().id;
          vq[i].pop_front();
          cnt[i] -= 1;
          if(cnt[i] == 0)
          {
            next_erase.emplace_back(i);
          }
          if(i + 1 < S)
          {
            vq[i + 1].emplace_back(x[i + 1] + run_time, t[id][i + 1], id);
            cnt[i + 1] += 1;
            if(cnt[i + 1] == 1)
            {
              next_add.emplace_back(i + 1);
            }
          }
          if(ei > 0)
            ei -= 1;
        }
      }
      std::sort(next_erase.begin(), next_erase.end());
      std::sort(next_add.begin(), next_add.end());
      for (const auto &e : use_id)
      {
        if(not std::binary_search(next_erase.begin(), next_erase.end(), e))
        {
          next_use.emplace_back(e);
        }
      }
      for (const auto &e : next_add)
      {
        next_use.emplace_back(e);
      }
      std::swap(use_id, next_use);
    }
    return real_time - run_time;
  };
  std::vector<int> res;
  std::vector<int> base;
  std::vector triple_loss(m, std::vector(m, std::vector<int>(m, inf)));
  std::vector pair_loss(m, std::vector(m, inf));
  // {
  //   std::vector<int> v;
  //   for (int i = 0; i < m; ++i)
  //   {
  //     v.emplace_back(i);
  //     for (int j = 0; j < m; ++j)
  //     {
  //       v.emplace_back(j);
  //       pair_loss[i][j] = run_short(v);
  //       v.pop_back();
  //     }
  //     v.pop_back();
  //   }
  // }
  {
    std::vector<int> v;
    int s1 = std::min_element(n.begin(), n.end()) - n.begin();
    int s2 = -1;
    v.emplace_back(s1);
    res.emplace_back(s1);
    std::vector<int> done_n(m);
    done_n[s1] += 1;
    {
      int min_loss = inf;
      int min_n = inf;
      for (int i = 0; i < m; ++i)
      {
        if(done_n[i] == n[i])
          continue;
        v.emplace_back(i);
        const int loss = run_short(v);
        v.pop_back();
        if(chmin(min_loss, loss))
        {
          s2 = i;
          min_n = n[i];
        }
        else if(min_loss == loss)
        {
          if(chmin(min_n, n[i]))
          {
            s2 = i;
          }
        }
      }
    }
    v.emplace_back(s2);
    res.emplace_back(s2);
    done_n[s2] += 1;
    while((int)res.size() < ALL)
    {
      int min_loss = inf;
      int id = -1;
      double max_par = 0.0;
      int loss = 0;
      for (int i = 0; i < m; ++i)
      {
        if(done_n[i] < inin[i])
        {
          v.emplace_back(i);
          if(m == 20)
            loss = run_short(v);
          else
          {
            if((int)v.size() != 3)
              assert(false);
            if(triple_loss[v[0]][v[1]][v[2]] != inf)
            {
              loss = triple_loss[v[0]][v[1]][v[2]];
            }
            else
            {
              loss = triple_loss[v[0]][v[1]][v[2]] = run_short(v);
            }
          }
          v.pop_back();
          if(chmin(min_loss, loss))
          {
            max_par = (double)(inin[i] - done_n[i]) / inin[i];
            id = i;
          }
          else if(min_loss == loss)
          {
            if(chmax(max_par, (double)(inin[i] - done_n[i]) / inin[i]))
            {
              id = i;
            }
          }
        }
      }
      res.emplace_back(id);
      if(m == 20)
      {
        if((int)v.size() >= 6)
          v.erase(v.begin());
      }
      else
      {
        if((int)v.size() >= 2)
          v.erase(v.begin());
      }
      v.emplace_back(id);
      done_n[id] += 1;
    }
  }

  auto annealing = [&](std::vector<int> &v, double end_time = 1750)
  {
    auto [best_get_score, best_c] = run(v);

    int id[2] = {};
    for (int jupi_loves_kkt = 0;; ++jupi_loves_kkt)
    {
      auto end = std::chrono::system_clock::now();
      const double time = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
      if(time > end_time)
        break;
      id[0] = xor64() % (int)best_c;
      id[1] = xor64() % (int)v.size();
      if(best_c < ALL)
      {
        id[1] = xor64() % ((int)v.size() - best_c) + best_c;
      }
      std::swap(v[id[0]], v[id[1]]);
      auto [get_score, get_c] = run(v);
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
  };
  double bs = 0.0;
  int bc = 0;
  std::tie(bs, bc) = run(res);
  if(bc < 1000)
  {
    auto pres = res;
    auto pbs = bs;
    auto pbc = bc;
    {
      res.clear();
      std::vector<int> v;
      int s1 = std::min_element(n.begin(), n.end()) - n.begin();
      int s2 = -1;
      v.emplace_back(s1);
      res.emplace_back(s1);
      std::vector<int> done_n(m);
      done_n[s1] += 1;
      {
        int min_loss = inf;
        int min_n = inf;
        for (int i = 0; i < m; ++i)
        {
          if(done_n[i] == n[i])
            continue;
          v.emplace_back(i);
          const int loss = run_short(v);
          v.pop_back();
          if(chmin(min_loss, loss))
          {
            s2 = i;
            min_n = n[i];
          }
          else if(min_loss == loss)
          {
            if(chmin(min_n, n[i]))
            {
              s2 = i;
            }
          }
        }
      }
      v.emplace_back(s2);
      res.emplace_back(s2);
      done_n[s2] += 1;
      std::vector<int> vloss(m);
      double score = 0.0;
      while((int)res.size() < ALL)
      {
        int min_loss = inf;
        int id = -1;
        double max_score = 0.0;
        vloss.clear();
        vloss.resize(m, inf);
        for (int i = 0; i < m; ++i)
        {
          if(done_n[i] < inin[i])
          {
            v.emplace_back(i);
            vloss[i] = run_short(v);
            chmin(min_loss, vloss[i]);
            v.pop_back();
          }
        }

        for (int i = 0; i < m; ++i)
        {
          if(done_n[i] == inin[i])
            continue;
          {
            double nscore = score;
            nscore -= std::sqrt((double)done_n[i] / inin[i]);
            nscore += std::sqrt((double)(done_n[i] + 1) / inin[i]);
            int lm = 5;
            if(m == 20 and bs < 0.93)
              lm = 30;
            if(vloss[i] <= min_loss + lm)
            {
              if(chmax(max_score, nscore))
              {
                id = i;
              }
            }
          }
        }
        if(m == 20)
        {
          if((int)v.size() >= 6)
            v.erase(v.begin());
          v.emplace_back(id);
        }
        else
        {
          if((int)v.size() >= 3)
            v.erase(v.begin());
          v.emplace_back(id);
        }
        res.emplace_back(id);
        done_n[id] += 1;
      }
    }
    annealing(res);
    std::tie(bs, bc) = run(res);
    if(bs < pbs)
    {
      res = pres;
      bs = pbs;
      bc = pbc;
    }
  }
  else
  {
    annealing(res);
  }
  res.resize(bc);
  cout << res.size() << "\n";
  for (int i = 0; i < (int)res.size(); ++i)
  {
    cout << res[i] + 1 << " \n"[i + 1 == (int)res.size()];
  }
}
int main([[maybe_unused]]int argc, [[maybe_unused]]char *argv[])
{
  std::cin.tie(nullptr);
  std::ios::sync_with_stdio(false);

  int kkt = 1; 
  // cin >> kkt;
  while(kkt--)
    solve();
  return 0;
  
}