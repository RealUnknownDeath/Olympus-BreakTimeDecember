#pragma once
#include <vector>
#include "ai/model_types.h"  // for Shape

std::vector<double> predict_values(const std::vector<double>& params,
                                   const std::vector<double>& x,
                                   const Shape& s);

double predict_value(const std::vector<double>& params,
                     const std::vector<double>& x,
                     const Shape& s);

double predict_proba(const std::vector<double>& params,
                     const std::vector<double>& x,
                     const Shape& s);
