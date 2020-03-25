#include "code_impl_SSE2.h"

// Profiling Infomation
// Hardware: Ryzen 3600
// Software: AVSMeter64
// Source: 1280x720 1740 frames YUV420P8


// 3D2 SSE2 Profiling
// Profiling time: 13,830ms -> 3,480ms
template <bool pattern, bool degrid>
void Apply3D2_SSE2(
  fftwf_complex *outcur,
  fftwf_complex *outprev,
  SharedFunctionParams sfp)
{
  loop_wrapper_SSE2(
    [&](LambdaFunctionParams lfp) {
      __m128 gridcorrection;
      constexpr float scale = 2.0f;

      if (degrid) {
        gridcorrection = lfp.m_gridcorrection * _mm_set1_ps(scale);
      }

      __m128 cur = _mm_load_ps((const float*)outcur);
      __m128 prev = _mm_load_ps((const float*)outprev);

      __m128 f3d0 = cur + prev;
      if (degrid) {
        f3d0 -= gridcorrection;
      }
      __m128 f3d1 = cur - prev;

      lfp.wiener_factor_3d<pattern>(f3d0);
      lfp.wiener_factor_3d<pattern>(f3d1);

      __m128 result = f3d0 + f3d1;
      if (degrid) {
        result += gridcorrection;
      }

      result *= _mm_set1_ps(1 / scale);
      _mm_store_ps((float*)outprev, result);
    }, sfp, outcur, outprev
  );
}

// 3D3 SSE2 Profiling
// Profiling time: 20,865ms -> 8,779ms
template <bool pattern, bool degrid>
void Apply3D3_SSE2(
  fftwf_complex *outcur,
  fftwf_complex *outprev,
  fftwf_complex *outnext,
  SharedFunctionParams sfp)
{
  loop_wrapper_SSE2(
    [&](LambdaFunctionParams lfp) {
      __m128 gridcorrection;
      constexpr float scale = 3.0f;
      const __m128 sin120 = _mm_set1_ps(0.86602540378443864676372317075294f);//sqrtf(3.0f)*0.5f;
      const __m128 m_0_5 = _mm_set1_ps(0.5f);

      if (degrid) {
        gridcorrection = lfp.m_gridcorrection * _mm_set1_ps(scale);
      }

      __m128 cur = _mm_load_ps((const float*)outcur);
      __m128 prev = _mm_load_ps((const float*)outprev);
      __m128 next = _mm_load_ps((const float*)outnext);

      __m128 pn = prev + next;
      __m128 fc = cur + pn;
      if (degrid) {
        fc -= gridcorrection;
      }
      __m128 diff = (prev - next) * sin120;
      __m128 d_ir = _mm_swap_ri(_mm_sign_r(diff));

      __m128 tempcur = cur - pn * m_0_5;
      __m128 fp = tempcur + d_ir;
      __m128 fn = tempcur - d_ir;

      lfp.wiener_factor_3d<pattern>(fc);
      lfp.wiener_factor_3d<pattern>(fp);
      lfp.wiener_factor_3d<pattern>(fn);

      __m128 result = fc + fp + fn;
      if (degrid) {
        result += gridcorrection;
      }

      result *= _mm_set1_ps(0.33333333333f);
      _mm_store_ps((float*)outprev, result);
    }, sfp, outcur, outprev, outnext
  );
}

// 3D4 SSE2 Profiling
// Profiling time: 41,681ms -> 10,441ms
template <bool pattern, bool degrid>
void Apply3D4_SSE2(
  fftwf_complex *outcur,
  fftwf_complex *outprev2,
  fftwf_complex *outprev,
  fftwf_complex *outnext,
  SharedFunctionParams sfp)
{
  loop_wrapper_SSE2(
    [&](LambdaFunctionParams lfp) {
      __m128 gridcorrection;
      constexpr float scale = 4.0f;

      if (degrid) {
        gridcorrection = lfp.m_gridcorrection * _mm_set1_ps(scale);
      }

      __m128 cur = _mm_load_ps((const float*)outcur);
      __m128 prev = _mm_load_ps((const float*)outprev);
      __m128 next = _mm_load_ps((const float*)outnext);
      __m128 prev2 = _mm_load_ps((const float*)outprev2);

      __m128 p_n = _mm_swap_ri(_mm_sign_r(prev - next));

      __m128 fp2 = (cur + prev2) - (prev + next);
      __m128 fp  = (cur - prev2) + p_n;
      __m128 fc  = (cur + prev2) + (prev + next);
      __m128 fn  = (cur - prev2) - p_n;

      if (degrid) {
        fc -= gridcorrection;
      }

      lfp.wiener_factor_3d<pattern>(fp2);
      lfp.wiener_factor_3d<pattern>(fp);
      lfp.wiener_factor_3d<pattern>(fc);
      lfp.wiener_factor_3d<pattern>(fn);

      __m128 result = (fp2 + fp) + (fc + fn);
      if (degrid) {
        result += gridcorrection;
      }

      result *= _mm_set1_ps(1/scale);
      _mm_store_ps((float*)outprev2, result);
    }, sfp, outcur, outprev2, outprev, outnext
  );
}

// 3D5 SSE2 Profiling
// Profiling time: 85,539ms -> 13,925ms
template <bool pattern, bool degrid>
void Apply3D5_SSE2(
  fftwf_complex *outcur,
  fftwf_complex *outprev2,
  fftwf_complex *outprev,
  fftwf_complex *outnext,
  fftwf_complex *outnext2,
  SharedFunctionParams sfp)
{
  loop_wrapper_SSE2(
    [&](LambdaFunctionParams lfp) {
      __m128 gridcorrection;
      constexpr float scale = 5.0f;
      __m128 sin72 = _mm_set1_ps(0.95105651629515357211643933337938f);
      __m128 cos72 = _mm_set1_ps(0.30901699437494742410229341718282f);
      __m128 sin144 = _mm_set1_ps(0.58778525229247312916870595463907f);
      __m128 cos144 = _mm_set1_ps(-0.80901699437494742410229341718282f);

      if (degrid) {
        gridcorrection = lfp.m_gridcorrection * _mm_set1_ps(scale);
      }

      __m128 cur = _mm_load_ps((const float*)outcur);
      __m128 prev = _mm_load_ps((const float*)outprev);
      __m128 next = _mm_load_ps((const float*)outnext);
      __m128 prev2 = _mm_load_ps((const float*)outprev2);
      __m128 next2 = _mm_load_ps((const float*)outnext2);

      __m128 p_n = _mm_swap_ri(_mm_sign_r(_mm_sub_ps(prev, next)));

      __m128 sum2 = (next2 + prev2) * cos72
                  + (prev + next) * cos144
                  + cur;
      __m128 dif2 = _mm_swap_ri(_mm_sign_i(
                    (prev2 - next2) * sin72
                  + (next - prev) * sin144
                  ));

      __m128 fp2 = sum2 + dif2;
      __m128 fn2 = sum2 - dif2;

      __m128 sum1 = (next2 + prev2) * cos144
                  + (prev + next) * cos72
                  + cur;
      __m128 dif1 = _mm_swap_ri(_mm_sign_i(
                    (next2 - prev2) * sin144
                  + (next - prev) * sin72
                  ));

      __m128 fp = sum1 + dif1;
      __m128 fn = sum1 - dif1;

      __m128 fc = (prev2 + prev) + cur + (next + next2);

      if (degrid) {
        fc = _mm_sub_ps(fc, gridcorrection);
      }

      lfp.wiener_factor_3d<pattern>(fp2);
      lfp.wiener_factor_3d<pattern>(fp);
      lfp.wiener_factor_3d<pattern>(fc);
      lfp.wiener_factor_3d<pattern>(fn);
      lfp.wiener_factor_3d<pattern>(fn2);

      __m128 result = (fp2 + fp) + (fc + fn) + fn2;
      if (degrid) {
        result += gridcorrection;
      }

      result *= _mm_set1_ps(1/scale);
      _mm_store_ps((float*)outprev2, result);
    }, sfp, outcur, outprev2, outprev, outnext, outnext2
  );
}


#define DECLARE(pattern, degrid) \
  template void Apply3D2_SSE2<pattern, degrid>(fftwf_complex*, fftwf_complex*, SharedFunctionParams);\
  template void Apply3D3_SSE2<pattern, degrid>(fftwf_complex*, fftwf_complex*, fftwf_complex*, SharedFunctionParams);\
  template void Apply3D4_SSE2<pattern, degrid>(fftwf_complex*, fftwf_complex*, fftwf_complex*, fftwf_complex*, SharedFunctionParams);\
  template void Apply3D5_SSE2<pattern, degrid>(fftwf_complex*, fftwf_complex*, fftwf_complex*, fftwf_complex*, fftwf_complex*, SharedFunctionParams);\

DECLARE(true, true)
DECLARE(false, true)
DECLARE(true, false)
DECLARE(false, false)
#undef DECLARE