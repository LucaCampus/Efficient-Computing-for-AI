#include "sparse_gemm.h"
#include <stdio.h>
#include <arm_neon.h>

int gemm_sparse_openmp_tiled_neon(const float *a, const sparse_weights *w,
                                  float *c, size_t m)
{
    /* TODO: combine the row-tile reuse in gemm_sparse_tiled with the
       four-column NEON update in gemm_sparse_neon. For each independent row
       tile, keep one float32x4_t accumulator per row and output-column group.
       Load each retained 4-value weight row once, then update all rows in the
       four-row tile with vfmaq_f32. Parallelize only the outer row-tile loop
       with a plain '#pragma omp parallel for'. The row tile is fixed at four;
       the benchmark tests thread counts 1/2/4. Use the same packed block
       traversal and ordering as the other kernels. */
    // fputs("TODO: implement tiled NEON sparse GEMM with OpenMP\n", stderr);
    if (!a || !w || !c) return SPARSE_ERROR;
    if (m % 4 != 0) return SPARSE_ERROR;   /* tile of exactly 4 rows */
    const size_t tiles = m / 4;

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (size_t tile = 0; tile < tiles; ++tile) {
        const size_t i0 = 4 * tile;   /* first row of this 4-row tile */
        for (size_t g = 0; g < w->n / 4; ++g) {
            /* one 4-lane accumulator per row of the tile */
            float32x4_t acc[4] = {vdupq_n_f32(0.0f), vdupq_n_f32(0.0f),
                                  vdupq_n_f32(0.0f), vdupq_n_f32(0.0f)};
            for (size_t p = w->block_col_ptr[g]; p < w->block_col_ptr[g + 1]; ++p) {
                size_t k0 = 4 * (size_t)w->block_row_idx[p];
                for (size_t r = 0; r < 4; ++r) {
                    /* weight row loaded once, reused for the 4 input rows */
                    float32x4_t wv = vld1q_f32(&w->block_values[16 * p + 4 * r]);
                    for (size_t t = 0; t < 4; ++t) {
                        float32x4_t av4 = vdupq_n_f32(a[(i0 + t) * w->k + k0 + r]);
                        acc[t] = vfmaq_f32(acc[t], wv, av4);
                    }
                }
            }
            /* single store per row, after all blocks of the group */
            for (size_t t = 0; t < 4; ++t)
                vst1q_f32(&c[(i0 + t) * w->n + 4 * g], acc[t]);
        }
    }
    return SPARSE_OK;
}
