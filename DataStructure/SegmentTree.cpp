// セグメントツリー (Segment Tree)
//
// example codes:
//
// ABC 343 F - Second Largest Query (1D)
// https://atcoder.jp/contests/abc343/submissions/77771841
// 区間内の2番目に大きい数の要素数を求める
//
// ARC 033 C - データ構造 (1D)
// https://atcoder.jp/contests/arc033/submissions/77772133
// x番目に小さい値を求める
//
// ABC 452 F - Interval Inversion Count (2D)
// https://atcoder.jp/contests/abc452/submissions/77773379
// 転倒数がちょうどkの区間を尺取りで求める。
//

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <class S, S (*op)(S, S), S (*e)()>
struct SegmentTree {
    ll _n, size, log;
    vector<S> d;

    // 子ノードの値から親ノードを更新
    void update(ll k) {
        d[k] = op(d[2 * k], d[2 * k + 1]);
    }

    // 指定サイズで初期化
    SegmentTree(ll n) : SegmentTree(vector<S>(n, e())) {}

    // 配列から初期化して構築
    SegmentTree(const vector<S>& v) {
        _n = v.size();
        size = 1;
        log = 0;
        while (size < _n) {
            size *= 2;
            log++;
        }
        d = vector<S>(2 * size, e());
        for (ll i = 0; i < _n; i++) {
            d[size + i] = v[i];
        }
        for (ll i = size - 1; i >= 1; i--) {
            update(i);
        }
    }

    // 1点更新
    void set(ll p, S x) {
        p += size;
        d[p] = x;
        for (ll i = 1; i <= log; i++) {
            update(p >> i);
        }
    }

    // 1点取得
    S get(ll p) const {
        return d[p + size];
    }

    // 区間取得 [l, r)
    S get(ll l, ll r) const {
        S sml = e(), smr = e();
        l += size;
        r += size;
        while (l < r) {
            if (l & 1) sml = op(sml, d[l++]);
            if (r & 1) smr = op(d[--r], smr);
            l >>= 1;
            r >>= 1;
        }
        return op(sml, smr);
    }

    // 全区間取得
    S get_all() const {
        return d[1];
    }

    // 二分探索 (右側)
    template <bool (*f)(S)>
    ll max_right(ll l) const {
        return max_right(l, [](S x) { return f(x); });
    }

    // 二分探索 (右側) - ラムダ式用
    template <class F>
    ll max_right(ll l, F f) const {
        if (l == _n) return _n;
        l += size;
        S sm = e();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!f(op(sm, d[l]))) {
                while (l < size) {
                    l = (2 * l);
                    if (f(op(sm, d[l]))) {
                        sm = op(sm, d[l]);
                        l++;
                    }
                }
                return l - size;
            }
            sm = op(sm, d[l]);
            l++;
        } while ((l & -l) != l);
        return _n;
    }

    // 二分探索 (左側)
    template <bool (*f)(S)>
    ll min_left(ll r) const {
        return min_left(r, [](S x) { return f(x); });
    }

    // 二分探索 (左側) - ラムダ式用
    template <class F>
    ll min_left(ll r, F f) const {
        if (r == 0) return 0;
        r += size;
        S sm = e();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!f(op(d[r], sm))) {
                while (r < size) {
                    r = (2 * r + 1);
                    if (f(op(d[r], sm))) {
                        sm = op(d[r], sm);
                        r--;
                    }
                }
                return r + 1 - size;
            }
            sm = op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }
};