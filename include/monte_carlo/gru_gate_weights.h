#pragma once
#include "gru_gate.h"

// Fill these with your trained tensors (row-major).
inline void load_gru_weights(GatingGRU& g){
    const int D = /* feature_dim */ 28;
    const int H = /* hidden_dim  */ 16;
    g.init(D,H);

    g.Wz = {/* H*D values */}; g.Uz = {/* H*H */}; g.bz = {/* H */};
    g.Wr = {/* H*D */};        g.Ur = {/* H*H */}; g.br = {/* H */};
    g.Wn = {/* H*D */};        g.Un = {/* H*H */}; g.bn = {/* H */};

    g.Wo = {/* 5*H */};        g.bo = {/* 5   */};
}
