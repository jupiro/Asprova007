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


void solve()
{
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
  std::vector<std::deque<KKT89>> vq(S);
  auto run = [&](const std::vector<int> &v)->int
  {
    int real_time = 0;
    // int idle_time = 0;
    vq.clear(); vq.resize(S);
    int push_time = 0;
    int idx = 0;
    int done = 0;
    while(real_time < L)
    {
      bool stop = false;
      if(push_time == 0 and idx < (int)v.size())
      {
        push_time += T;
        vq[0].emplace_back(x[0], t[v[idx]][0], v[idx]);
        idx += 1;
      }
      int exe_time = inf;
      for (int i = 0; i < S; ++i)
      {
        if(not vq[i].empty() and vq[i].front().r_len == 0)
          stop = true;
      }
      for (int i = 0; i < S; ++i)
      {
        for (int j = 0; j < (int)vq[i].size(); ++j)
        {
          if(not stop)
          {
            chmin(exe_time, vq[i][j].r_len);
          }
          if(vq[i][j].r_time > 0)
          {
            chmin(exe_time, vq[i][j].r_time);
            break;
          }
        }
      }
      if(idx < (int)v.size())
      {
        chmin(exe_time, push_time);
      }
      if(exe_time > L - real_time)
        break;
      real_time += exe_time;
      if(idx < (int)v.size())
      {
        push_time -= exe_time;
      }
      for (int i = S - 1; i >= 0; --i)
      {
        bool exe = true;
        for (int j = 0; j < (int)vq[i].size(); ++j)
        {
          if(not stop)
          {
            vq[i][j].r_len -= exe_time;
          }
          if(vq[i][j].r_time > 0 and exe)
          {
            exe = false;
            vq[i][j].r_time -= exe_time;
            if(vq[i][j].r_time == 0 and i == S - 1)
            {
              done += 1;
            }
          }
        }
        if(not vq[i].empty() and vq[i].front().r_len == 0 and vq[i].front().r_time == 0)
        {
          const int id = vq[i].front().id;
          vq[i].pop_front();
          if(i + 1 < S)
          {
            vq[i + 1].emplace_back(x[i + 1], t[id][i + 1], id);
          }
        }
      }
    }
    return done;
  };
  std::vector<int> stime(m);
  for (int i = 0; i < m; ++i)
  {
    for (int j = 0; j < S; ++j)
    {
      stime[i] += t[i][j];
    }
  }
  std::vector<int> iota(m);
  for (int i = 0; i < m; ++i)
  {
    iota[i] = i;
  }
  std::sort(iota.begin(), iota.end(), [&](auto i, auto j)
  {
    return stime[i] < stime[j];
  });
  std::vector<int> res;
  for (const auto &e : iota)
  {
    for (int i = 0; i < n[e]; ++i)
    {
      res.emplace_back(e);
    }
  }
  
  const int done = run(res);
  res.resize(done);
  cout << res.size() << "\n";
  for (int i = 0; i < (int)res.size(); ++i)
  {
    cout << res[i] + 1 << " \n"[i + 1 == (int)res.size()];
  }
}
int main()
{
  std::cin.tie(nullptr);
  std::ios::sync_with_stdio(false);

  int kkt = 1; 
  // cin >> kkt;
  while(kkt--)
    solve();
  return 0;
  
}