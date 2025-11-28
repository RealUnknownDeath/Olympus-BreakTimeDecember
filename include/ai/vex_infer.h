// include/ai/vex_infer.h
#pragma once
#include "ai/model_types.h"
#include "ai/mlp_flex.h"
#include "ai/model.h"
#include "ai/model_io.h"

#include <vector>

struct InferenceResult {
  double y = 0.0;
  int    y_label = 0;
  std::vector<double> y_vec;
};

inline std::vector<double> standardize(const std::vector<double>& x,
                                       const std::vector<double>& mean,
                                       const std::vector<double>& stdev) {
  if (mean.empty() || stdev.empty()) return x;
  std::vector<double> z(x.size());
  for (size_t i = 0; i < x.size(); ++i) {
    const double mu = (i < mean.size()  ? mean[i]  : mean.back());
    const double sd = (i < stdev.size() ? stdev[i] : stdev.back());
    z[i] = (sd != 0.0) ? ((x[i] - mu) / sd) : (x[i] - mu);
  }
  return z;
}

inline InferenceResult infer(const SavedModel& m,
                             const std::vector<double>& x_raw) {
  const std::vector<double> x = standardize(x_raw, m.mean, m.stdev);
  InferenceResult out;

  switch (m.type) {
    case ModelType::LINREG:
    case ModelType::LOGREG:
      // Not used on Brain – just return 0
      out.y = 0.0;
      out.y_label = 0;
      return out;


    case ModelType::MLP:
    case ModelType::MLP_REG:
    case ModelType::MLP_FLEX: {
      Shape s = m.fshape;
      if (s.D == 0) s.D = x.size();

      if (s.regression) {
        out.y_vec = predict_values(m.params, x, s);
        out.y = out.y_vec.empty() ? 0.0 : out.y_vec[0];
      } else {
        out.y = predict_proba(m.params, x, s);
        out.y_label = (out.y >= 0.5);
      }
      return out;
    }

    default:
      // unknown type -> zeros
      return out;
  }
}
