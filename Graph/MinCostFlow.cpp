// 最小費用流 (Successive Shortest Path 法)
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
    ll cost;
};

struct MinCostFlow {
    struct _Edge {
        ll to;
        ll rev;
        ll cap;
        ll cost;
    };

    ll _n;
    vector<pair<ll, ll>> pos;
    vector<vector<_Edge>> g;

    // 指定頂点数で初期化
    MinCostFlow(ll n) : _n(n), g(n) {}

    // 辺の追加
    ll add_edge(ll from, ll to, ll cap, ll cost) {
        ll m = pos.size();
        pos.push_back({from, (ll)g[from].size()});
        ll from_id = g[from].size();
        ll to_id = g[to].size();
        if (from == to) to_id++;
        g[from].push_back({to, to_id, cap, cost});
        g[to].push_back({from, from_id, 0, -cost});
        return m;
    }

    // 辺の状態取得
    Edge get_edge(ll i) const {
        auto _e = g[pos[i].first][pos[i].second];
        auto _re = g[_e.to][_e.rev];
        return Edge{pos[i].first, _e.to, _e.cap + _re.cap, _re.cap, _e.cost};
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

    // 最小費用流の計算 (流量指定)
    pair<ll, ll> build(ll s, ll t, ll flow) {
        ll res_flow = 0;
        ll res_cost = 0;
        vector<ll> h(_n, 0);
        vector<ll> dist(_n);
        vector<pair<ll, ll>> pre(_n); // 経路復元用: pre[to] = {from, edge_index}

        while (flow > 0) {
            priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, greater<pair<ll, ll>>> que;
            fill(dist.begin(), dist.end(), numeric_limits<ll>::max() / 2);
            dist[s] = 0;
            que.push({0, s});

            while (!que.empty()) {
                auto [d, v] = que.top();
                que.pop();
                if (dist[v] < d) continue;
                for (ll i = 0; i < (ll)g[v].size(); i++) {
                    auto& e = g[v][i];
                    if (e.cap > 0 && dist[e.to] > dist[v] + e.cost + h[v] - h[e.to]) {
                        dist[e.to] = dist[v] + e.cost + h[v] - h[e.to];
                        pre[e.to] = {v, i};
                        que.push({dist[e.to], e.to});
                    }
                }
            }

            if (dist[t] == numeric_limits<ll>::max() / 2) break;

            for (ll v = 0; v < _n; v++) {
                if (dist[v] != numeric_limits<ll>::max() / 2) h[v] += dist[v];
            }

            ll f = flow;
            for (ll v = t; v != s; v = pre[v].first) {
                f = min(f, g[pre[v].first][pre[v].second].cap);
            }
            for (ll v = t; v != s; v = pre[v].first) {
                auto& e = g[pre[v].first][pre[v].second];
                e.cap -= f;
                g[v][e.rev].cap += f;
            }
            res_flow += f;
            res_cost += f * h[t];
            flow -= f;
        }
        return {res_flow, res_cost};
    }

    // 最小費用流の計算 (最大流量)
    pair<ll, ll> build(ll s, ll t) {
        return build(s, t, numeric_limits<ll>::max());
    }
};