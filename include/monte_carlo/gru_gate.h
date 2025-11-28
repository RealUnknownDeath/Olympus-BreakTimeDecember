#pragma once
#include <vector>
#include <array>
#include <cmath>
#include <algorithm>

// Minimal GRU for sensor gating: input D -> hidden H -> output 5 gates in (0,1)
struct GatingGRU {
    int D = 0, H = 0, O = 5;
    // Weights are row-major [rows x cols]
    std::vector<float> Wz, Uz, bz;
    std::vector<float> Wr, Ur, br;
    std::vector<float> Wn, Un, bn;
    std::vector<float> Wo, bo;    // output head (logits) size 5
    std::vector<float> h;         // hidden state size H
    bool ready = false;

    static inline float sigmoid(float x){ return 1.f / (1.f + std::exp(-x)); }
    static inline float tanhf_fast(float x){ return std::tanh(x); }

    void init(int d, int h) {
        D = d; H = h;
        h.assign(H, 0.f);
        ready = true;
    }
    void reset(){ std::fill(h.begin(), h.end(), 0.f); }

    // y = W x + U v + b
    static void affine(const std::vector<float>& W, int rows, int cols,
                       const std::vector<float>& x,
                       const std::vector<float>& U, const std::vector<float>& v,
                       const std::vector<float>& b,
                       std::vector<float>& y)
    {
        y.assign(rows, 0.f);
        // W x
        for (int r=0; r<rows; ++r){
            float s=0.f;
            const float* Wr = &W[r*cols];
            for (int c=0; c<cols; ++c) s += Wr[c]*x[c];
            y[r] = s;
        }
        // U v
        for (int r=0; r<rows; ++r){
            float s=0.f;
            const float* Ur = &U[r*rows];
            for (int c=0; c<rows; ++c) s += Ur[c]*v[c];
            y[r] += s + b[r];
        }
    }

    // Single step: returns gates for {front, back, left, right, gps}
    std::array<float,5> step(const std::vector<float>& x) {
        std::array<float,5> g{0.8f,0.8f,0.8f,0.8f,0.8f}; // safe defaults
        if (!ready || (int)x.size()!=D) return g;

        std::vector<float> zt, rt, nt;
        // z_t
        affine(Wz,H,D, x, Uz,h, bz, zt);
        for (int i=0;i<H;++i) zt[i] = sigmoid(zt[i]);

        // r_t
        affine(Wr,H,D, x, Ur,h, br, rt);
        for (int i=0;i<H;++i) rt[i] = sigmoid(rt[i]);

        // n_t = tanh(Wn x + Un (r ⊙ h) + bn)
        std::vector<float> rh(H);
        for (int i=0;i<H;++i) rh[i] = rt[i]*h[i];
        // reuse 'nt' buffer
        nt.assign(H,0.f);
        // Wn x
        for (int r=0;r<H;++r){
            float s=0.f;
            const float* Wrp=&Wn[r*D];
            for (int c=0;c<D;++c) s += Wrp[c]*x[c];
            nt[r]=s;
        }
        // Un (r⊙h) + bn
        for (int r=0;r<H;++r){
            float s=0.f;
            const float* Urp=&Un[r*H];
            for (int c=0;c<H;++c) s += Urp[c]*rh[c];
            nt[r]=tanhf_fast(nt[r]+s+bn[r]);
        }

        // h' = (1-z) ⊙ n + z ⊙ h
        for (int i=0;i<H;++i) h[i] = (1.f-zt[i])*nt[i] + zt[i]*h[i];

        // head: logits = Wo h + bo  -> gates = sigmoid(logits)
        std::array<float,5> out{};
        for (int r=0;r<O;++r){
            float s=bo[r];
            const float* Wor=&Wo[r*H];
            for (int c=0;c<H;++c) s += Wor[c]*h[c];
            out[r] = sigmoid(s); // (0,1)
        }
        // map to a gentle range to avoid extremes; [0.25, 4.0] as multiplier later
        for (float& v : out){
            float m = 0.25f + 3.75f*v; // 0.25..4.0
            v = m;
        }
        return out;
    }
};
