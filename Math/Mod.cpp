// 自動剰余演算 (Mod):
//
// example codes:
//

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Mod {
    static ll MOD;
    ll v;

    // 全体の法 (MOD) を変更
    static void set_mod(ll m) {
        MOD = m;
    }

    // 初期化 (自動的に [0, MOD) に正規化)
    Mod(ll _v = 0) {
        v = _v % MOD;
        if (v < 0) v += MOD;
    }

    // 値の取得
    ll get() const {
        return v;
    }

    // 演算子のオーバーロード (複合代入)
    Mod& operator+=(const Mod& a) {
        v += a.v;
        if (v >= MOD) v -= MOD;
        return *this;
    }

    Mod& operator-=(const Mod& a) {
        v -= a.v;
        if (v < 0) v += MOD;
        return *this;
    }

    Mod& operator*=(const Mod& a) {
        v = v * a.v % MOD;
        return *this;
    }

    Mod& operator/=(const Mod& a) {
        return *this *= a.inv();
    }

    // 演算子のオーバーロード (四則演算)
    Mod operator+(const Mod& a) const { return Mod(*this) += a; }
    Mod operator-(const Mod& a) const { return Mod(*this) -= a; }
    Mod operator*(const Mod& a) const { return Mod(*this) *= a; }
    Mod operator/(const Mod& a) const { return Mod(*this) /= a; }

    // 比較演算子
    bool operator==(const Mod& a) const { return v == a.v; }
    bool operator!=(const Mod& a) const { return v != a.v; }

    // 累乗 (負の指数にも対応)
    Mod pow(ll n) const {
        if (n < 0) return inv().pow(-n);
        Mod res = 1, a = *this;
        while (n > 0) {
            if (n & 1) res *= a;
            a *= a;
            n >>= 1;
        }
        return res;
    }

    // 逆元 (拡張ユークリッドの互除法)
    Mod inv() const {
        ll a = v, b = MOD, u = 1, x = 0;
        while (b > 0) {
            ll t = a / b;
            a -= t * b; swap(a, b);
            u -= t * x; swap(u, x);
        }
        return Mod(u);
    }

    // 入出力ストリーム
    friend ostream& operator<<(ostream& os, const Mod& m) { return os << m.v; }
    friend istream& operator>>(istream& is, Mod& m) {
        ll val;
        is >> val;
        m = Mod(val);
        return is;
    }
};
ll Mod::MOD = 998244353;