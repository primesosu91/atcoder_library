// 素集合データ構造 (Union-Find)
//
// example codes:
//
// ABC 372 E - K-th Largest Connected Components (1Q)
// https://atcoder.jp/contests/abc372/submissions/77773558
// 連結成分の頂点番号が大きい方からk番目の頂点を求める
//
// ABC 120 D - Decayed Bridges (1Q)
// https://atcoder.jp/contests/abc120/submissions/77773679
// 辺削除 -> 辺追加への言い換え
//

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct UnionFind {
    vector<ll> parent_or_size;
    ll group_cnt;

    // 指定サイズで初期化
    UnionFind(ll n) : parent_or_size(n, -1), group_cnt(n) {}

    // 根を探索 (経路圧縮あり)
    ll root(ll x) {
        if (parent_or_size[x] < 0) return x;
        return parent_or_size[x] = root(parent_or_size[x]);
    }

    // 同一グループか判定
    bool same(ll x, ll y) {
        return root(x) == root(y);
    }

    // 集合を併合 (サイズによる統合)
    bool unite(ll x, ll y) {
        x = root(x);
        y = root(y);
        if (x == y) return false;
        if (-parent_or_size[x] < -parent_or_size[y]) swap(x, y);
        parent_or_size[x] += parent_or_size[y];
        parent_or_size[y] = x;
        group_cnt--;
        return true;
    }

    // グループのサイズを取得
    ll size(ll x) {
        return -parent_or_size[root(x)];
    }

    // 全グループ数を取得
    ll count() const {
        return group_cnt;
    }

    // 全グループの根のリストを取得
    vector<ll> roots() {
        vector<ll> res;
        ll n = parent_or_size.size();
        for (ll i = 0; i < n; i++) {
            if (parent_or_size[i] < 0) res.push_back(i);
        }
        return res;
    }

    // 全グループの要素リストを取得
    vector<vector<ll>> groups() {
        ll n = parent_or_size.size();
        vector<vector<ll>> res(n);
        for (ll i = 0; i < n; i++) {
            res[root(i)].push_back(i);
        }
        vector<vector<ll>> result;
        for (ll i = 0; i < n; i++) {
            if (!res[i].empty()) {
                result.push_back(res[i]);
            }
        }
        return result;
    }
};