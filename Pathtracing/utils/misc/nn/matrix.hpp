#pragma once

#include <vector>
#include <cassert>
#include <iostream>

#include "utils/math/random.hpp"

struct Matrix {
    size_t rows;
    size_t cols;
    std::vector<double> data;

    Matrix(size_t r, size_t c) : rows(r), cols(c), data(r * c, 0.0) {}

    double& operator()(size_t row, size_t col) {
        return data[row * cols + col];
    }
    const double& operator()(size_t row, size_t col) const {
        return data[row * cols + col];
    }

    double& operator()(size_t idx) {
        return data[idx];
    }
    const double& operator()(size_t idx) const {
        return data[idx];
    }

    void Randomize() {
        for (auto& val : data)
            val = Random::Range(-0.5, 0.5);
    }

    Matrix operator+(const Matrix& other) const {
        assert(cols == other.cols && rows == other.rows && "Dimensions of added matrices do not match!");

        Matrix result(rows, cols);
        for (size_t i = 0; i < data.size(); ++i)
            result(i) = this->data[i] + other.data[i];
        return result;
    }

    Matrix operator*(const Matrix& other) const {
        assert(cols == other.rows && "Inner row of matrices do not match!");

        Matrix result(rows, other.cols);

        for (size_t r = 0; r < rows; ++r)
        for (size_t c = 0; c < other.cols; ++c) {
            double sum = 0.0;
            for (size_t i = 0; i < cols; ++i)
                sum += (*this)(r, i) * other(i, c);
            result(r, c) = sum;
        }

        return result;
    }

    void ApplyActivation(double (*activation_func)(double)) {
        for (auto& val : data)
            val = activation_func(val);
    }

    void Print() const {
        for (size_t r = 0; r < rows; ++r) {
            for (size_t c = 0; c < cols; ++c) {
                std::cout << (*this)(r, c) << " ";
            }
            std::cout << "\n";
        }
    }
};