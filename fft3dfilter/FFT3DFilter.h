#ifndef __FFT3DFILTER_H__
#define __FFT3DFILTER_H__

#include "common.h"
#include <avisynth.h>
#include "info.h"
#include <emmintrin.h>
#include <mmintrin.h> // _mm_empty
#include <algorithm>
#include <atomic>

// declarations of filtering functions:
void ApplyKalman_SSE2_simd(fftwf_complex *out, fftwf_complex *outLast, fftwf_complex *covar, fftwf_complex *covarProcess, int outwidth, int outpitch, int bh, int howmanyblocks, float covarNoiseNormed, float kratio2);
// SSE
void ApplyWiener3D2_SSE_simd(fftwf_complex *out, fftwf_complex *outprev, int outwidth, int outpitch, int bh, int howmanyblocks, float sigmaSquaredNoiseNormed, float beta);
void ApplyPattern3D2_SSE(fftwf_complex *out, fftwf_complex *outprev, int outwidth, int outpitch, int bh, int howmanyblocks, float * pattern3d, float beta);
void ApplyWiener3D3_SSE(fftwf_complex *out, fftwf_complex *outprev, fftwf_complex *outnext, int outwidth, int outpitch, int bh, int howmanyblocks, float sigmaSquaredNoiseNormed, float beta);
void ApplyPattern3D3_SSE(fftwf_complex *out, fftwf_complex *outprev, fftwf_complex *outnext, int outwidth, int outpitch, int bh, int howmanyblocks, float *pattern3d, float beta);
void Sharpen_SSE(fftwf_complex *out, int outwidth, int outpitch, int bh, int howmanyblocks, float sharpen, float sigmaSquaredSharpenMin, float sigmaSquaredSharpenMax, float *wsharpen, float dehalo, float *wdehalo, float ht2n);
// C
void ApplyWiener2D_C(fftwf_complex *out, int outwidth, int outpitch, int bh, int howmanyblocks, float sigmaSquaredNoiseNormed, float beta, float sharpen, float sigmaSquaredSharpenMin, float sigmaSquaredSharpenMax, float *wsharpen, float dehalo, float *wdehalo, float ht2n);
void ApplyPattern2D_C(fftwf_complex *out, int outwidth, int outpitch, int bh, int howmanyblocks, float pfactor, float *pattern2d0, float beta);
void ApplyWiener3D2_C(fftwf_complex *out, fftwf_complex *outprev, int outwidth, int outpitch, int bh, int howmanyblocks, float sigmaSquaredNoiseNormed, float beta);
void ApplyPattern3D2_C(fftwf_complex *out, fftwf_complex *outprev, int outwidth, int outpitch, int bh, int howmanyblocks, float *pattern3d, float beta);
void ApplyWiener3D3_C(fftwf_complex *out, fftwf_complex *outprev, fftwf_complex *outnext, int outwidth, int outpitch, int bh, int howmanyblocks, float sigmaSquaredNoiseNormed, float beta);
void ApplyPattern3D3_C(fftwf_complex *out, fftwf_complex *outprev, fftwf_complex *outnext, int outwidth, int outpitch, int bh, int howmanyblocks, float *pattern3d, float beta);
void ApplyWiener3D4_C(fftwf_complex *out, fftwf_complex *outprev2, fftwf_complex *outprev, fftwf_complex *outnext, int outwidth, int outpitch, int bh, int howmanyblocks, float sigmaSquaredNoiseNormed, float beta);
void ApplyPattern3D4_C(fftwf_complex *out, fftwf_complex *outprev2, fftwf_complex *outprev, fftwf_complex *outnext, int outwidth, int outpitch, int bh, int howmanyblocks, float* pattern3d, float beta);
void ApplyWiener3D5_C(fftwf_complex *out, fftwf_complex *outprev2, fftwf_complex *outprev, fftwf_complex *outnext, fftwf_complex *outnext2, int outwidth, int outpitch, int bh, int howmanyblocks, float sigmaSquaredNoiseNormed, float beta);
void ApplyPattern3D5_C(fftwf_complex *out, fftwf_complex *outprev2, fftwf_complex *outprev, fftwf_complex *outnext, fftwf_complex *outnext2, int outwidth, int outpitch, int bh, int howmanyblocks, float* pattern3d, float beta);
void ApplyKalmanPattern_C(fftwf_complex *out, fftwf_complex *outLast, fftwf_complex *covar, fftwf_complex *covarProcess, int outwidth, int outpitch, int bh, int howmanyblocks, float *covarNoiseNormed, float kratio2);
void ApplyKalman_C(fftwf_complex *out, fftwf_complex *outLast, fftwf_complex *covar, fftwf_complex *covarProcess, int outwidth, int outpitch, int bh, int howmanyblocks, float covarNoiseNormed, float kratio2);
void Sharpen_C(fftwf_complex *out, int outwidth, int outpitch, int bh, int howmanyblocks, float sharpen, float sigmaSquaredSharpenMin, float sigmaSquaredSharpenMax, float *wsharpen, float dehalo, float *wdehalo, float ht2n);
// degrid_C
void ApplyWiener2D_degrid_C(fftwf_complex *out, int outwidth, int outpitch, int bh, int howmanyblocks, float sigmaSquaredNoiseNormed, float beta, float sharpen, float sigmaSquaredSharpenMin, float sigmaSquaredSharpenMax, float *wsharpen, float degrid, fftwf_complex *gridsample, float dehalo, float *wdehalo, float ht2n);
void ApplyWiener3D2_degrid_C(fftwf_complex *out, fftwf_complex *outprev, int outwidth, int outpitch, int bh, int howmanyblocks, float sigmaSquaredNoiseNormed, float beta, float degrid, fftwf_complex *gridsample);
void ApplyWiener3D3_degrid_C(fftwf_complex *out, fftwf_complex *outprev, fftwf_complex *outnext, int outwidth, int outpitch, int bh, int howmanyblocks, float sigmaSquaredNoiseNormed, float beta, float degrid, fftwf_complex *gridsample);
void ApplyWiener3D4_degrid_C(fftwf_complex *out, fftwf_complex *outprev2, fftwf_complex *outprev, fftwf_complex *outnext, int outwidth, int outpitch, int bh, int howmanyblocks, float sigmaSquaredNoiseNormed, float beta, float degrid, fftwf_complex *gridsample);
void ApplyWiener3D5_degrid_C(fftwf_complex *out, fftwf_complex *outprev2, fftwf_complex *outprev, fftwf_complex *outnext, fftwf_complex *outnext2, int outwidth, int outpitch, int bh, int howmanyblocks, float sigmaSquaredNoiseNormed, float beta, float degrid, fftwf_complex *gridsample);
void Sharpen_degrid_C(fftwf_complex *out, int outwidth, int outpitch, int bh, int howmanyblocks, float sharpen, float sigmaSquaredSharpenMin, float sigmaSquaredSharpenMax, float *wsharpen, float degrid, fftwf_complex *gridsample, float dehalo, float *wdehalo, float ht2n);
void ApplyPattern2D_degrid_C(fftwf_complex *out, int outwidth, int outpitch, int bh, int howmanyblocks, float pfactor, float *pattern2d0, float beta, float degrid, fftwf_complex *gridsample);
void ApplyPattern3D2_degrid_C(fftwf_complex *out, fftwf_complex *outprev, int outwidth, int outpitch, int bh, int howmanyblocks, float *pattern3d, float beta, float degrid, fftwf_complex *gridsample);
void ApplyPattern3D3_degrid_C(fftwf_complex *out, fftwf_complex *outprev, fftwf_complex *outnext, int outwidth, int outpitch, int bh, int howmanyblocks, float *pattern3d, float beta, float degrid, fftwf_complex *gridsample);
void ApplyPattern3D4_degrid_C(fftwf_complex *out, fftwf_complex *outprev2, fftwf_complex *outprev, fftwf_complex *outnext, int outwidth, int outpitch, int bh, int howmanyblocks, float* pattern3d, float beta, float degrid, fftwf_complex *gridsample);
void ApplyPattern3D5_degrid_C(fftwf_complex *out, fftwf_complex *outprev2, fftwf_complex *outprev, fftwf_complex *outnext, fftwf_complex *outnext2, int outwidth, int outpitch, int bh, int howmanyblocks, float* pattern3d, float beta, float degrid, fftwf_complex *gridsample);
// degrid_SSE
void ApplyWiener3D3_degrid_SSE_simd(fftwf_complex *out, fftwf_complex *outprev, fftwf_complex *outnext, int outwidth, int outpitch, int bh, int howmanyblocks, float sigmaSquaredNoiseNormed, float beta, float degrid, fftwf_complex *gridsample);
void ApplyPattern3D3_degrid_SSE(fftwf_complex *out, fftwf_complex *outprev, fftwf_complex *outnext, int outwidth, int outpitch, int bh, int howmanyblocks, float *pattern3d, float beta, float degrid, fftwf_complex *gridsample);
void Sharpen_degrid_SSE_simd(fftwf_complex *out, int outwidth, int outpitch, int bh, int howmanyblocks, float sharpen, float sigmaSquaredSharpenMin, float sigmaSquaredSharpenMax, float *wsharpen, float degrid, fftwf_complex *gridsample, float dehalo, float *wdehalo, float ht2n);
void ApplyWiener3D4_degrid_SSE(fftwf_complex *out, fftwf_complex *outprev2, fftwf_complex *outprev, fftwf_complex *outnext, int outwidth, int outpitch, int bh, int howmanyblocks, float sigmaSquaredNoiseNormed, float beta, float degrid, fftwf_complex *gridsample);
void ApplyPattern3D4_degrid_SSE(fftwf_complex *out, fftwf_complex *outprev2, fftwf_complex *outprev, fftwf_complex *outnext, int outwidth, int outpitch, int bh, int howmanyblocks, float *pattern3d, float beta, float degrid, fftwf_complex *gridsample);

struct FilterFunctionPointers {
  // Wiener
    void (*ApplyWiener2D)
        (fftwf_complex *out, int outwidth, int outpitch, int bh, int howmanyblocks, float sigmaSquaredNoiseNormed, float beta, float sharpen, float sigmaSquaredSharpenMin, float sigmaSquaredSharpenMax, float *wsharpen, float dehalo, float *wdehalo, float ht2n);
    void (*ApplyWiener3D2)
        (fftwf_complex *out, fftwf_complex *outprev, int outwidth, int outpitch, int bh, int howmanyblocks, float sigmaSquaredNoiseNormed, float beta);
    void (*ApplyWiener3D3)
        (fftwf_complex *out, fftwf_complex *outprev, fftwf_complex *outnext, int outwidth, int outpitch, int bh, int howmanyblocks, float sigmaSquaredNoiseNormed, float beta);
    void (*ApplyWiener3D4)
        (fftwf_complex *out, fftwf_complex *outprev2, fftwf_complex *outprev, fftwf_complex *outnext, int outwidth, int outpitch, int bh, int howmanyblocks, float sigmaSquaredNoiseNormed, float beta);
    void (*ApplyWiener3D5)
        (fftwf_complex *out, fftwf_complex *outprev2, fftwf_complex *outprev, fftwf_complex *outnext, fftwf_complex *outnext2, int outwidth, int outpitch, int bh, int howmanyblocks, float sigmaSquaredNoiseNormed, float beta);

  // Wiener Degrid
    void (*ApplyWiener2D_degrid)
        (fftwf_complex *out, int outwidth, int outpitch, int bh, int howmanyblocks, float sigmaSquaredNoiseNormed, float beta, float sharpen, float sigmaSquaredSharpenMin, float sigmaSquaredSharpenMax, float *wsharpen, float degrid, fftwf_complex *gridsample, float dehalo, float *wdehalo, float ht2n);
    void (*ApplyWiener3D2_degrid)
        (fftwf_complex *out, fftwf_complex *outprev, int outwidth, int outpitch, int bh, int howmanyblocks, float sigmaSquaredNoiseNormed, float beta, float degrid, fftwf_complex *gridsample);
    void (*ApplyWiener3D3_degrid)
        (fftwf_complex *out, fftwf_complex *outprev, fftwf_complex *outnext, int outwidth, int outpitch, int bh, int howmanyblocks, float sigmaSquaredNoiseNormed, float beta, float degrid, fftwf_complex *gridsample);
    void (*ApplyWiener3D4_degrid)
        (fftwf_complex *out, fftwf_complex *outprev2, fftwf_complex *outprev, fftwf_complex *outnext, int outwidth, int outpitch, int bh, int howmanyblocks, float sigmaSquaredNoiseNormed, float beta, float degrid, fftwf_complex *gridsample);
    void (*ApplyWiener3D5_degrid)
        (fftwf_complex *out, fftwf_complex *outprev2, fftwf_complex *outprev, fftwf_complex *outnext, fftwf_complex *outnext2, int outwidth, int outpitch, int bh, int howmanyblocks, float sigmaSquaredNoiseNormed, float beta, float degrid, fftwf_complex *gridsample);

  // Pattern
    void (*ApplyPattern2D)
        (fftwf_complex *out, int outwidth, int outpitch, int bh, int howmanyblocks, float pfactor, float *pattern2d0, float beta);
    void (*ApplyPattern3D2)
        (fftwf_complex *out, fftwf_complex *outprev, int outwidth, int outpitch, int bh, int howmanyblocks, float *pattern3d, float beta);
    void (*ApplyPattern3D3)
        (fftwf_complex *out, fftwf_complex *outprev, fftwf_complex *outnext, int outwidth, int outpitch, int bh, int howmanyblocks, float *pattern3d, float beta);
    void (*ApplyPattern3D4)
        (fftwf_complex *out, fftwf_complex *outprev2, fftwf_complex *outprev, fftwf_complex *outnext, int outwidth, int outpitch, int bh, int howmanyblocks, float* pattern3d, float beta);
    void (*ApplyPattern3D5)
        (fftwf_complex *out, fftwf_complex *outprev2, fftwf_complex *outprev, fftwf_complex *outnext, fftwf_complex *outnext2, int outwidth, int outpitch, int bh, int howmanyblocks, float* pattern3d, float beta);

  // Pattern Degrid
    void (*ApplyPattern2D_degrid)
        (fftwf_complex *out, int outwidth, int outpitch, int bh, int howmanyblocks, float pfactor, float *pattern2d0, float beta, float degrid, fftwf_complex *gridsample);
    void (*ApplyPattern3D2_degrid)
        (fftwf_complex *out, fftwf_complex *outprev, int outwidth, int outpitch, int bh, int howmanyblocks, float *pattern3d, float beta, float degrid, fftwf_complex *gridsample);
    void (*ApplyPattern3D3_degrid)
        (fftwf_complex *out, fftwf_complex *outprev, fftwf_complex *outnext, int outwidth, int outpitch, int bh, int howmanyblocks, float *pattern3d, float beta, float degrid, fftwf_complex *gridsample);
    void (*ApplyPattern3D4_degrid)
        (fftwf_complex *out, fftwf_complex *outprev2, fftwf_complex *outprev, fftwf_complex *outnext, int outwidth, int outpitch, int bh, int howmanyblocks, float *pattern3d, float beta, float degrid, fftwf_complex *gridsample);
    void (*ApplyPattern3D5_degrid)
        (fftwf_complex *out, fftwf_complex *outprev2, fftwf_complex *outprev, fftwf_complex *outnext, fftwf_complex *outnext2, int outwidth, int outpitch, int bh, int howmanyblocks, float* pattern3d, float beta, float degrid, fftwf_complex *gridsample);

  // Kalman
    void (*ApplyKalman)
        (fftwf_complex *out, fftwf_complex *outLast, fftwf_complex *covar, fftwf_complex *covarProcess, int outwidth, int outpitch, int bh, int howmanyblocks, float covarNoiseNormed, float kratio2);
    void (*ApplyKalmanPattern)
        (fftwf_complex *out, fftwf_complex *outLast, fftwf_complex *covar, fftwf_complex *covarProcess, int outwidth, int outpitch, int bh, int howmanyblocks, float *covarNoiseNormed, float kratio2);

  // Sharpen
    void (*Sharpen)
        (fftwf_complex *out, int outwidth, int outpitch, int bh, int howmanyblocks, float sharpen, float sigmaSquaredSharpenMin, float sigmaSquaredSharpenMax, float *wsharpen, float dehalo, float *wdehalo, float ht2n);
    void (*Sharpen_degrid)
        (fftwf_complex *out, int outwidth, int outpitch, int bh, int howmanyblocks, float sharpen, float sigmaSquaredSharpenMin, float sigmaSquaredSharpenMax, float *wsharpen, float degrid, fftwf_complex *gridsample, float dehalo, float *wdehalo, float ht2n);

  void set_ffp(int CPUFlags)
  {
    ApplyWiener2D = ApplyWiener2D_C;
    ApplyWiener3D2 = ApplyWiener3D2_C;
    ApplyWiener3D3 = ApplyWiener3D3_C;
    ApplyWiener3D4 = ApplyWiener3D4_C;
    ApplyWiener3D5 = ApplyWiener3D5_C;
    ApplyWiener2D_degrid = ApplyWiener2D_degrid_C;
    ApplyWiener3D2_degrid = ApplyWiener3D2_degrid_C;
    ApplyWiener3D3_degrid = ApplyWiener3D3_degrid_C;
    ApplyWiener3D4_degrid = ApplyWiener3D4_degrid_C;
    ApplyWiener3D5_degrid = ApplyWiener3D5_degrid_C;
    ApplyWiener3D3_degrid = ApplyWiener3D3_degrid_C;
    ApplyPattern2D = ApplyPattern2D_C;
    ApplyPattern3D2 = ApplyPattern3D2_C;
    ApplyPattern3D3 = ApplyPattern3D3_C;
    ApplyPattern3D4 = ApplyPattern3D4_C;
    ApplyPattern3D5 = ApplyPattern3D5_C;
    ApplyPattern2D_degrid = ApplyPattern2D_degrid_C;
    ApplyPattern3D2_degrid = ApplyPattern3D2_degrid_C;
    ApplyPattern3D3_degrid = ApplyPattern3D3_degrid_C;
    ApplyPattern3D4_degrid = ApplyPattern3D4_degrid_C;
    ApplyPattern3D5_degrid = ApplyPattern3D5_degrid_C;
    ApplyKalman = ApplyKalman_C;
    ApplyKalmanPattern = ApplyKalmanPattern_C;
    Sharpen = Sharpen_C;
    Sharpen_degrid = Sharpen_degrid_C;

    #ifndef X86_64
      if (CPUFlags & CPUF_SSE) {
        ApplyWiener3D3 = ApplyWiener3D3_SSE;
        ApplyWiener3D3_degrid = ApplyWiener3D3_degrid_SSE_simd;
        ApplyWiener3D4_degrid = ApplyWiener3D4_degrid_SSE;
        ApplyPattern3D2 = ApplyPattern3D2_SSE;
        ApplyPattern3D3 = ApplyPattern3D3_SSE;
        ApplyPattern3D3_degrid = ApplyPattern3D3_degrid_SSE;
        ApplyPattern3D4_degrid = ApplyPattern3D4_degrid_SSE;
        Sharpen = Sharpen_SSE;
      }
    #endif

    if (CPUFlags & CPUF_SSE2) { // 170302 simd, SSE2
      ApplyWiener3D2 = ApplyWiener3D2_SSE_simd;
      ApplyWiener3D3_degrid = ApplyWiener3D3_degrid_SSE_simd;
      ApplyKalman = ApplyKalman_SSE2_simd;
      Sharpen_degrid = Sharpen_degrid_SSE_simd;
    }
  }
};

#endif