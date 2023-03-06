struct FlowEdge
{
	int u, v;
	long long capacity, flow;
	// int index;  //maybe?
	FlowEdge(int _u, int _v, long long _cap)
	{
		u = _u;
		v = _v;
		capacity = _cap;
		flow = 0;
	}
	bool can_use()
	{
		return (capacity-flow)>0;
	}
	long long residual()
	{
		return capacity-flow;
	}
};
 
struct Dinic
{
	int s, t;
	int n, m;
	vector<FlowEdge> edges;
	vector<vector<int>> adj;
	vector<int> level, ptr;
	const long long flow_inf = 1e18;
	Dinic(int nx, int source, int sink)
	{
		n = nx;
		m = 0;
		adj.resize(nx);
		level.resize(nx);
		ptr.resize(nx);
		s = source;
		t = sink;
	}
	void add_edge(int from, int to, long long cap)
	{
		//Note that we are storing ids of edges in adjacency list instead of neighbour
		adj[from].push_back(m);
		edges.emplace_back(from, to, cap);
		adj[to].push_back(m+1);
		edges.emplace_back(to, from, 0ll);
		m += 2;
	}
	bool bfs()
	{
		fill(level.begin(), level.end(), -1);
		level[s] = 0;		//source has level 0
		queue<int> q;
		q.push(s);
		while(!q.empty())
		{
			int f = q.front();
			q.pop();
			for(auto &p: adj[f])
			{
				if(level[edges[p].v] != -1 || !edges[p].can_use())
				{
					continue;
				}
				level[edges[p].v] = level[f] + 1;
				q.push(edges[p].v);
			}
		}
		return (level[t] != -1); 			//return whether sink was reachable or not
	}
 
	long long dfs(int vx, long long pushed)
	{
		if(!pushed) return 0;				//no flow entering means nothing can be pushed ahead
		if(vx == t) return pushed;			//sink can absorb all flow
 
		//find the first neighbour to which you can push the flow and push as much flow as can be pushed
		for(int& i=ptr[vx]; i<(int)(adj[vx].size()); ++i)		//note that i is a reference so ptr[vx] changes with i
		{
			int edge_id = adj[vx][i];
			int tx = edges[edge_id].v;
			if(level[tx] != level[vx]+1 || !edges[edge_id].can_use()) 
				continue;
			
			long long what = dfs(tx, min(pushed, edges[edge_id].residual()));	//how much flow was used in further dfs
 
			if(what == 0) continue;
 
			edges[edge_id].flow += what;
			edges[edge_id^1].flow -= what;
			return what;
		}
		return 0;
	}
 
	long long flow()
	{
		long long cur_flow = 0;
		while(bfs())
		{
			long long add;
			fill(ptr.begin(), ptr.end(), 0);
			while((add = dfs(s, flow_inf)))
			{
				cur_flow += add;
			}
		}
		return cur_flow;
	}
};
