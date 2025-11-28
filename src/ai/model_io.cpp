#include "ai/model_io.h"
#include "ai/model_types.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

bool load_model(const char* filename, SavedModel& m) {
    std::ifstream in(filename);
    if (!in.is_open()) return false;

    std::string header;
    if (!(in >> header)) return false;

    if (header != "V1") {
        // You can add a fallback for old formats here if you want.
        return false;
    }

    // ----- model type -----
    std::string tag;
    in >> tag;
    if (tag == "mlp")       m.type = ModelType::MLP;
    else if (tag == "mlpreg") m.type = ModelType::MLP_REG;
    else if (tag == "mlp_flex") m.type = ModelType::MLP_FLEX;
    else if (tag == "linreg")   m.type = ModelType::LINREG;
    else if (tag == "logreg")   m.type = ModelType::LOGREG;
    else                        m.type = ModelType::MLP_FLEX; // safe default

    // ----- shape line: D, regression flag, O -----
    std::size_t D, O;
    int regFlag;
    in >> D >> regFlag >> O;

    m.fshape.D          = D;
    m.fshape.regression = (regFlag != 0);
    m.fshape.O          = O;

    // ----- hidden sizes: first integer = count -----
    std::size_t nH;
    in >> nH;
    m.fshape.H.clear();
    for (std::size_t i = 0; i < nH; ++i) {
        std::size_t h;
        in >> h;
        m.fshape.H.push_back(h);
    }

    // ----- params -----
    std::size_t nParams;
    in >> nParams;
    m.params.resize(nParams);
    for (std::size_t i = 0; i < nParams; ++i) {
        in >> m.params[i];
    }

    // ----- mean -----
    std::size_t nMean;
    in >> nMean;
    m.mean.resize(nMean);
    for (std::size_t i = 0; i < nMean; ++i) {
        in >> m.mean[i];
    }

    // ----- stdev -----
    std::size_t nStd;
    in >> nStd;
    m.stdev.resize(nStd);
    for (std::size_t i = 0; i < nStd; ++i) {
        in >> m.stdev[i];
    }

    return true;
}
