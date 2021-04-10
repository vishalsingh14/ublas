
//  Boost Competency test
//  vector.hpp Definition for the class vector
//  Copyright (c) Vishal Singh 

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include<vector>
#include<iostream>
#include<cassert>
#include<cmath>
#include<tuple>

namespace comptest{

struct span {
    size_t l, r, len;
    span(size_t l, size_t r) 
        : l(l), r(r), len(r-l+1) 
    {
        assert(l>=0 && r>=l);
    }
};

template<typename T>
class matrix {

public:
    // default construtor
    matrix() 
        : R(0), C(0) 
    {}

    ~matrix() = default;

    // initialize with value 
    matrix(size_t r,size_t c, T x=0) 
        : R(r), C(c) 
    {
        M.assign(R, std::vector<T>(C,x));
    }

    // initialize with intializer list
    matrix(std::initializer_list<std::vector<T>> m) 
        : M(m)
    {
        R = M.size(), C = R > 0 ? M[0].size() : 0;
    }

    // check empty 
    bool empty() const {
        return R*C == 0;
    }

    // return size of matrix
    std::tuple<size_t,size_t> size() const {
        return std::make_tuple(R,C);
    } 

    // resize the matrix
    void resize(size_t r, size_t c, T x=0) {
        R = r, C = c;
        M.resize(R,std::vector<T>(C,x));
    }

    // Add, Subtract, Multiplixation 
    matrix operator +=(const matrix &rhs){
        assert(R == rhs.R && C == rhs.C);
        for(size_t r = 0; r < R; ++r) {
            for(size_t c = 0; c < C; ++c) {
                M[r][c] += rhs(r,c);
            }
        }
        return *this;
    }
    matrix operator -=(const matrix &rhs) {
        assert(R == rhs.R && C == rhs.C);
        for(size_t r = 0; r < R; ++r) {
            for(size_t c = 0; c < C; ++c) {
                M[r][c] -= rhs(r,c);
            }
        }
        return *this;
    }
    matrix operator*=(const matrix &rhs) {
        assert(C == rhs.R);
        matrix res(R, rhs.C, 0);
        for (size_t r = 0; r < res.R; ++r) {
            for (size_t c = 0; c<res.C; ++c) {
                for (size_t i = 0; i < C; ++i) {
                    res(r, c) += M[r][i] * rhs(i, c);
                }
            }
        }
        return res;
    }
    matrix operator /(const T &rhs) {
        matrix res = *this;
        for(size_t r = 0; r < R; ++r) {
            for(size_t c = 0; c < C; ++c) {
                res.M[r][c] /= rhs;
            }
        }
        return res;
    }

    matrix operator+(const matrix &rhs) const{
        return matrix(*this) += rhs;
    }
    matrix operator-(const matrix &rhs) const{
        return matrix(*this) -= rhs; 
    }
    matrix operator*(const matrix &rhs) const{
        return matrix(*this) *= rhs;
    }
    matrix operator-() const {
        return matrix(R, C, 0) - *this;
    }
    // multiply element by rhs
    matrix operator*=(const T &rhs) {
        for(size_t r = 0; r < R; ++r) {
            for(size_t c = 0; c < C; ++c) {
                M[r][c] *= rhs;
            }
        }
        return *this;
    }
    matrix operator*(const T &rhs) const {
        return matrix(*this) *= rhs;
    }
    
    friend matrix<T> operator *(const T &lhs, const matrix<T> &rhs) {
        return rhs * lhs;
    }

    // comparision of matrix elementwise
    bool operator==(const matrix &rhs) const {
        if(empty() || R!=rhs.R || C!=rhs.C) {
            return false;
        }
        for(size_t i = 0; i < R; ++i) {
            for(size_t j=0; j < C; ++j) {
                if(ceil(M[i][j]) != ceil(rhs(i,j))) {
                    return false;
                }
            }
        }
        return true;
    }
    bool operator!=(const matrix &rhs) const {
        return !(*this==rhs);
    }

    // access element of matrix
    T operator()(size_t r, size_t c) const {
        assert(r >= 0 && r < R && c >= 0 && c < C);
        return M[r][c];
    }
    T& operator()(size_t r, size_t c) {
        assert(0 <= r && r < R && 0 <= c && c < C);
        return M[r][c];
    }

    matrix operator()(const span &row_range, const span &col_range) const {
        assert(row_range.r < R && col_range.r < C);
        matrix res(row_range.len, col_range.len);
        for(size_t r = 0; r < row_range.len; ++r) {
            for(size_t c = 0; c <  col_range.len; ++c) {
                res(r,c) = M[row_range.l + r][col_range.l + c];
            }
        }
        return res;
    }
    T& operator[](size_t i) {
        return M[i % R][i / R];
    }

    // transpose of matrix
    matrix t() {
        matrix res(C,R);
        for(size_t r = 0; r < R; ++r) {
            for(size_t c = 0; c < C; ++c) {
                res(c,r) = M[r][c];
            }
        }
        return res;
    }

    // return rth row of matrix
    matrix row(int r) const {
        return {M[r]};
    }

    // return cth column of matrix
    matrix column(int c) const {
        matrix<T> col(R,1);
        for(size_t r = 0; r < R; ++r) {
            col(r,0) = M[r][c];
        }
        return col;
    }    

    // print matrix
    friend std::ostream& operator<<(std::ostream &out, const matrix &m) {
        std::tuple<size_t,size_t> sze = m.size();
        size_t r = std::get<0>(sze), c = std::get<1>(sze);
        out << "[" << r << "," << c << "]" << "(";
        for(size_t i = 0; i < r; ++i) {
            out << "(";
            for(size_t j = 0; j < c; ++j) {
                out << m(i,j);
                if (j+1 != c) {
                    out << ",";
                }
            }
            out << ")";
            if (i+1 != r) {
                out << ",";
            }
        }
        out << ")";
        return out;
    }

private: 
    size_t R, C;
    std::vector<std::vector<T>> M;
};

// zero matrix of size r*c
template<typename T>
matrix<T> zeros(size_t r, size_t c) {
    return matrix<T>(r,c,0);
}

// range funtion same as python range function
template<typename T>
matrix<T> range(T begin, T end, T stepsize = 1) {
    assert(stepsize != 0);
    int n = ceil(1.0 * (end-begin) / stepsize);
    matrix<T> res(1, n);
    for(size_t i = 0; i < n; ++i) {
        res(0, i) = begin + i * stepsize;
    }
    return res;
}
template<typename T>
matrix<T> range(T end) {
    return range(0, end, 1);
}

// Matrix Norms (same as MATLAB)
// 1 -> 1-norm 
// fro -> Frobenius Norm 
// inf -> infinity-norm
template<typename T>
double norm(const matrix<T> &m, std::string p = "fro"){
    std::tuple<size_t,size_t> sze = m.size();
    double res = 0;
    size_t R = std::get<0>(sze), C = std::get<1>(sze);
    if(p == "1") {
        for(size_t c = 0; c < C; ++c) {
            double sum = 0;
            for(size_t r = 0; r < R; ++r) {
                sum += abs(m(r,c));
            }
            res = std::max(res, sum);
        }
    }
    else if(p == "fro"){
        for(size_t r = 0; r < R; ++r) {
            for(size_t c = 0; c < C; ++c) {
                res += (m(r,c) * m(r,c));
            }
        }
        res = sqrt(res);
    }
    else if(p == "inf") {
        for(size_t r = 0; r < R; ++r) {
            double sum = 0;
            for(size_t c = 0; c < R; ++c) {
                sum += abs(m(r,c));
            }
            res = std::max(res, sum);
        }
    }
    else {
        assert(0);
    }
    return res;
}

// QR decomposition
template<typename T>
std::tuple<matrix<T>, matrix<T>> qr(matrix<T> A) {
    std::tuple<size_t,size_t> sze = A.size();
    size_t m = std::get<0>(sze), n = std::get<1>(sze);
    matrix<T> Q(m, n, 0), R(n, n, 0);
    for (size_t k = 0; k < n; k++) {
        matrix<T> v = A({0, m-1}, {k, k});
        if (k >= 1) {
            matrix<T> qv = Q({0, m-1}, {0, k-1}).t() * v;
            for (size_t i = 0; i < k; i++) {
                R(i, k) = qv(i, 0);
            }
            v -= Q({0, m-1}, {0, k-1}) * R({0, k-1}, {k, k});
        }
        R(k, k) = norm(v);
        for (size_t i = 0; i < m; i++) {
            Q(i, k) = v(i, 0) / R(k, k);
        }
    }
    return std::make_tuple(Q, R);
}

} // namespace comptest

#endif
