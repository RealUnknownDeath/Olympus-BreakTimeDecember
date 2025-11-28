// src/ai/simple_linear.cpp
#include <vector>
#include <cmath>

namespace linreg {

// Simple linear regression: y = w·x + b
double predict_value(const std::vector<double>& params,
                     const std::vector<double>& x) {
    if (params.empty()) return 0.0;

    double y = 0.0;
    std::size_t n = x.size();
    // Ensure we don't read past params if shapes mismatch
    if (n + 1 > params.size()) {
        n = (params.size() > 0) ? params.size() - 1 : 0;
    }

    for (std::size_t i = 0; i < n; ++i) {
        y += params[i] * x[i];
    }

    // last param is bias
    y += params.back();
    return y;
}

} // namespace linreg

namespace logreg {

// Logistic regression: sigmoid(linreg score)
double predict_proba(const std::vector<double>& params,
                     const std::vector<double>& x) {
    double z = linreg::predict_value(params, x);
    return 1.0 / (1.0 + std::exp(-z));
}

} // namespace logreg
