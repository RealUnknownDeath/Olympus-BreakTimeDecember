// src/ai/mlp_flex.cpp
#include "ai/mlp_flex.h"
#include <cmath>
#include <vector>

// ReLU activation for hidden layers
static inline double act(double z) {
    return (z > 0.0) ? z : 0.0;
}

std::vector<double> predict_values(const std::vector<double>& params,
                                   const std::vector<double>& x,
                                   const Shape& s) {
    // If D isn't set, fall back to x.size()
    std::size_t n_in0 = (s.D != 0) ? s.D : x.size();

    std::vector<double> layer_in = x;
    std::vector<double> layer_out;
    std::size_t offset = 0;

    auto forward_layer = [&](std::size_t n_in,
                             std::size_t n_out,
                             bool apply_activation) {
        layer_out.assign(n_out, 0.0);
        for (std::size_t j = 0; j < n_out; ++j) {
            double sum = 0.0;

            // weights
            for (std::size_t i = 0; i < n_in; ++i) {
                double w = 0.0;
                if (offset < params.size()) {
                    w = params[offset++];
                }
                sum += w * layer_in[i];
            }

            // bias
            double b = 0.0;
            if (offset < params.size()) {
                b = params[offset++];
            }
            sum += b;

            if (apply_activation) {
                sum = act(sum);  // ReLU
            }
            layer_out[j] = sum;
        }
        layer_in.swap(layer_out);
    };

    std::size_t n_in = n_in0;

    // Hidden layers
    for (std::size_t li = 0; li < s.H.size(); ++li) {
        std::size_t n_out = s.H[li];
        forward_layer(n_in, n_out, /*apply_activation=*/true);
        n_in = n_out;
    }

    // Output layer
    std::size_t O = s.O;
    if (O == 0) O = 1; // fallback

    // Try to infer O from leftover params if not set
    std::size_t remaining =
        (offset < params.size()) ? (params.size() - offset) : 0;

    if (remaining >= (n_in + 1) && (remaining % (n_in + 1) == 0)) {
        std::size_t inferred = remaining / (n_in + 1);
        if (s.O == 0) {
            O = inferred;
        }
    }

    forward_layer(n_in, O, /*apply_activation=*/false);

    // layer_in now holds final outputs
    return layer_in;
}

double predict_value(const std::vector<double>& params,
                     const std::vector<double>& x,
                     const Shape& s) {
    std::vector<double> ys = predict_values(params, x, s);
    return ys.empty() ? 0.0 : ys[0];
}

double predict_proba(const std::vector<double>& params,
                     const std::vector<double>& x,
                     const Shape& s) {
    double val = predict_value(params, x, s);
    return 1.0 / (1.0 + std::exp(-val));
}
