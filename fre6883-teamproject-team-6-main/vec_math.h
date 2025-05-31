// vec_math.h
// by Chris Li
#pragma once

#include <vector>
#include <numeric>
#include <cassert>
#include <cmath>

using vec = std::vector<double>;
using mat = std::vector<vec>;

// ----------------------------------------
// 1) Element-wise arithmetic operator overloads for vec
// ----------------------------------------
inline vec operator+(const vec& a, const vec& b) {
    assert(a.size() == b.size());
    vec r(a.size());
    for (size_t i = 0; i < a.size(); ++i) r[i] = a[i] + b[i];
    return r;
}
inline vec operator-(const vec& a, const vec& b) {
    assert(a.size() == b.size());
    vec r(a.size());
    for (size_t i = 0; i < a.size(); ++i) r[i] = a[i] - b[i];
    return r;
}
inline vec operator*(const vec& a, double c) {
    vec r(a.size());
    for (size_t i = 0; i < a.size(); ++i) r[i] = a[i] * c;
    return r;
}
inline vec operator*(double c, const vec& a) {
    return a * c;
}
inline vec operator/(const vec& a, double c) {
    assert(c != 0.0);
    vec r(a.size());
    for (size_t i = 0; i < a.size(); ++i) r[i] = a[i] / c;
    return r;
}
// Element-wise multiplication (used for squaring)
inline vec operator*(const vec& a, const vec& b) {
    assert(a.size() == b.size());
    vec r(a.size());
    for (size_t i = 0; i < a.size(); ++i) r[i] = a[i] * b[i];
    return r;
}

// ----------------------------------------
// 2) Statistical functions
// ----------------------------------------

// Row-wise mean, returns a vec of length equal to number of columns
inline vec mean(const mat& M) {
    assert(!M.empty());
    vec sum(M[0].size(), 0.0);
    for (const auto& row : M) sum = sum + row;
    return sum / static_cast<double>(M.size());
}
// Mean of a vec (scalar)
inline double mean(const vec& v) {
    assert(!v.empty());
    double s = std::accumulate(v.begin(), v.end(), 0.0);
    return s / static_cast<double>(v.size());
}
// Prefix sum
inline vec cumm(const vec& v) {
    vec r(v.size());
    double s = 0.0;
    for (size_t i = 0; i < v.size(); ++i) {
        s += v[i];
        r[i] = s;
    }
    return r;
}
// Unbiased sample standard deviation for a vec
inline double stddev(const vec& v) {
    assert(v.size() > 1);
    double m = mean(v);
    double ss = 0.0;
    for (double x : v) ss += (x - m) * (x - m);
    return std::sqrt(ss / (static_cast<double>(v.size()) - 1.0));
}
// Unbiased sample standard deviation for each column in a mat
inline vec stddev(const mat& M) {
    assert(M.size() > 1);
    vec mu = mean(M);
    vec sumsq(mu.size(), 0.0);
    for (const auto& row : M) {
        vec d = row - mu;
        sumsq = sumsq + (d * d);
    }
    double inv = 1.0 / (static_cast<double>(M.size()) - 1.0);
    for (size_t i = 0; i < sumsq.size(); ++i)
        sumsq[i] = std::sqrt(sumsq[i] * inv);
    return sumsq;
}
