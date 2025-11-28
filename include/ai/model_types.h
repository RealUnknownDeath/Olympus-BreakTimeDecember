// include/ai/model_types.h
#pragma once
#include <vector>
#include <cstddef>

enum class ModelType { LINREG, LOGREG, MLP, MLP_REG, MLP_FLEX };

struct Shape {
  std::size_t D = 0;                 // input dimension
  std::vector<std::size_t> H;        // hidden layer sizes
  std::size_t O = 2;                 // *** output dimension ***
  bool regression = true;           // true for regression nets
};

struct SavedModel {
  ModelType type = ModelType::LINREG;
  Shape fshape;

  std::vector<double> mean;
  std::vector<double> stdev;
  std::vector<double> params;
};
