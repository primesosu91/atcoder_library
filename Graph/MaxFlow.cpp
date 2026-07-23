// 最大流 (Dinic法)
//
// example codes:
//

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Edge {
    ll from;
    ll to;
    ll cap;
    ll flow;
};

struct MaxFlow {
    struct _Edge {
        ll to;
        ll rev;
        ll cap;
    };

    ll _n;
    vector<pair<ll, ll>> pos;
    vector<vector<_Edge>> g;

    // 指定頂点数で初期化
    MaxFlow(ll n) : _n(n), g(n) {}

    // 辺の追加
    ll add_edge(ll from, ll to, ll cap) {
        ll m = pos.size();
        pos.push_back({from, (ll)g[from].size()});
        ll from_id = g[from].size();
        ll to_id = g[to].size();
        if (from == to) to_id++;
        g[from].push_back({to, to_id, cap});
        g[to].push_back({from, from_id, 0});
        return m;
    }

    // 辺の状態取得
    Edge get_edge(ll i) const {
        auto _e = g[pos[i].first][pos[i].second];
        auto _re = g[_e.to][_e.rev];
        return Edge{pos[i].first, _e.to, _e.cap + _re.cap, _re.cap};
    }

    // 全ての辺の状態取得
    vector<Edge> get_edges() const {
        ll m = pos.size();
        vector<Edge> res(m);
        for (ll i = 0; i < m; i++) {
            res[i] = get_edge(i);
        }
        return res;
    }

    // 最大流の計算 (Dinic法)
    ll build(ll s, ll t) {
        vector<ll> level(_n);
        vector<ll> iter(_n);
        
        auto bfs = [&]() {
            fill(level.begin(), level.end(), -1);
            level[s] = 0;
            queue<ll> que;
            que.push(s);
            while (!que.empty()) {
                ll v = que.front();
                que.pop();
                for (auto& e : g[v]) {
                    if (e.cap > 0 && level[e.to] < 0) {
                        level[e.to] = level[v] + 1;
                        que.push(e.to);
                    }
                }
            }
        };

        auto dfs = [&](auto self, ll v, ll up) -> ll {
            if (v == t) return up;
            ll res = 0;
            ll level_v = level[v];
            for (ll& i = iter[v]; i < (ll)g[v].size(); i++) {
                _Edge& e = g[v][i];
                if (level_v >= level[e.to] || e.cap == 0) continue;
                ll d = self(self, e.to, min(up - res, e.cap));
                if (d <= 0) continue;
                e.cap -= d;
                g[e.to][e.rev].cap += d;
                res += d;
                if (res == up) return res;
            }
            level[v] = -1;
            return res;
        };

        ll flow = 0;
        while (true) {
            bfs();
            if (level[t] < 0) break;
            fill(iter.begin(), iter.end(), 0);
            while (true) {
                ll f = dfs(dfs, s, numeric_limits<ll>::max());
                if (f == 0) break;
                flow += f;
            }
        }
        return flow;
    }

    // sから到達可能な頂点リストの取得 (最小カット)
    vector<ll> get(ll s) const {
        vector<bool> visited(_n, false);
        queue<ll> que;
        que.push(s);
        visited[s] = true;
        while (!que.empty()) {
            ll v = que.front();
            que.pop();
            for (auto& e : g[v]) {
                if (e.cap > 0 && !visited[e.to]) {
                    visited[e.to] = true;
                    que.push(e.to);
                }
            }
        }
        vector<ll> res;
        for (ll i = 0; i < _n; i++) {
            if (visited[i]) res.push_back(i);
        }
        return res;
    }
};