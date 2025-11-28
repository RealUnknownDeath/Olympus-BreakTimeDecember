#pragma once
#include "vex.h"
#include <vector>

// ---------- LOGISTIC REGRESSION ----------
namespace logreg {
// params layout: [w1..wd, b]
double predict_proba(const std::vector<double>& params, const std::vector<double>& x);
int    predict(const std::vector<double>& params, const std::vector<double>& x, double threshold = 0.5);
double loss(const std::vector<double>& params,
            const std::vector<std::vector<double>>& X,
            const std::vector<int>& y);
std::vector<double> grad(const std::vector<double>& params,
                         const std::vector<std::vector<double>>& X,
                         const std::vector<int>& y);
}

// ---------- LINEAR REGRESSION ----------
namespace linreg {
// params layout: [w1..wd, b]
double predict_value(const std::vector<double>& params, const std::vector<double>& x);
double loss(const std::vector<double>& params,
            const std::vector<std::vector<double>>& X,
            const std::vector<double>& y);
std::vector<double> grad(const std::vector<double>& params,
                         const std::vector<std::vector<double>>& X,
                         const std::vector<double>& y);
}
