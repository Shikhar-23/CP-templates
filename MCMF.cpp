const static int INF = 1e12;
// const static double INF1 = 1e12;
  static const ll InfCapacity = INF;
 
long double eps = 1e-8;
struct MinimumCostMaximumFlow {
  typedef ll Index; typedef ll Flow; typedef long double Cost;
  struct Edge {
    Index to; Index rev;
    Flow capacity; Cost cost;
  };
  vector<vector<Edge> > g;
  void init(Index n) { g.assign(n, vector<Edge>()); }
  void addEdge(Index i, Index j, Flow capacity = InfCapacity, Cost cost = Cost() ) {
    Edge e, f; e.to = j, f.to = i; e.capacity = capacity, f.capacity = 0; e.cost = cost, f.cost = -cost;
    g[i].push_back(e); g[j].push_back(f);
    g[i].back().rev = (Index)g[j].size() - 1; g[j].back().rev = (Index)g[i].size() - 1;
  }
  void addB(Index i, Index j, Flow capacity = InfCapacity, Cost cost = Cost()) {
    addEdge(i, j, capacity, cost);
    addEdge(j, i, capacity, cost);
  }
 
  pair<Cost, Flow> minimumCostMaximumFlow(Index s, Index t, Flow f = InfCapacity, bool useSPFA = false) {
    ll n = g.size();
    vector<Cost> dist(n); vector<Index> prev(n); vector<Index> prevEdge(n);
    pair<Cost, Flow> total = make_pair(0, 0);
    vector<Cost> potential(n);
    while(f > 0) {
        // trace(dist);
 
      fill(dist.begin(), dist.end(), INF);
      if(useSPFA || total.second == 0) {
        deque<Index> q;
        q.push_back(s); dist[s] = 0; vector<bool> inqueue(n);
        while(!q.empty()) {
          Index i = q.front(); q.pop_front(); inqueue[i] = false;
          for(Index ei = 0; ei < (Index)g[i].size(); ei ++) {
            const Edge &e = g[i][ei]; Index j = e.to; Cost d = dist[i] + e.cost;
            if(e.capacity > 0 && d < dist[j]) {
              if(!inqueue[j]) {
                inqueue[j] = true;
                q.push_back(j);
              }
              dist[j] = d; prev[j] = i; prevEdge[j] = ei;
            }
          }
        }
      } else {
        vector<bool> vis(n);
        priority_queue<pair<Cost, Index> > q;
        q.push(make_pair(-0, s)); dist[s] = 0;
        while(!q.empty()) {
          Index i = q.top().second; q.pop();
          if(vis[i]) continue;
          vis[i] = true;
          for(Index ei = 0; ei < (Index)g[i].size(); ei ++) {
            const Edge &e = g[i][ei];
            if(e.capacity <= 0) continue;
            Index j = e.to; Cost d = dist[i] + e.cost + potential[i] - potential[j];
            if(dist[j] > d) {
              dist[j] = d; prev[j] = i; prevEdge[j] = ei;
              q.push(make_pair(-d, j));
            }
          }
        }
      }
      if(abs(dist[t] - INF) < eps) {
        break;
      }
      if(!useSPFA) for(Index i = 0; i < n; i ++) potential[i] += dist[i];
 
      Flow d = f; Cost distt = 0.0;
      for(Index v = t; v != s; ) {
        Index u = prev[v]; const Edge &e = g[u][prevEdge[v]];
        d = min(d, e.capacity); distt += e.cost; v = u;
      }
      // trace(d, distt);
      f -= d; total.first += d * distt; total.second += d;
      // trace(total.first);
      for(Index v = t; v != s; v = prev[v]) {
        Edge &e = g[prev[v]][prevEdge[v]];
        e.capacity -= d; g[e.to][e.rev].capacity += d;
      }
    }
    // trace(total);
    return total;
  }
};
