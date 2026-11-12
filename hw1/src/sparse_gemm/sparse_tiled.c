#include "sparse_gemm.h"
#include <stdio.h>

int gemm_sparse_tiled(const float *a, const sparse_weights *w, float *c,
                        size_t m)
{
    (void)a; (void)w; (void)c; (void)m;
    /* TODO: use a row tile of exactly four input rows. Keep four 4-column
       output accumulators in local storage. For every retained block, load
       its weight values once and reuse them for all four rows. */
    // fputs("TODO: implement gemm_sparse_tiled and reuse weights across rows\n", stderr);
    if (!a || !w || !c) return SPARSE_ERROR;
    if (m % 4 != 0) return SPARSE_ERROR;

    for (size_t i0 = 0; i0 < m; i0 += 4) {
      for (size_t g = 0; g < w->n / 4; ++g) {
        float acc[4][4] = {{0.0f}};
        for (size_t p = w->block_col_ptr[g]; p < w->block_col_ptr[g + 1]; ++p) {
          size_t k0 = 4 * (size_t)w->block_row_idx[p];
          for (size_t r = 0; r < 4; ++r) {
            float wr[4];
            for (size_t col = 0; col < 4; ++col)
              wr[col] = w->block_values[16 * p + 4 * r + col];
            for (size_t t = 0; t < 4; ++t) {
              float av = a[(i0 + t) * w->k + k0 + r];
              for (size_t col = 0; col < 4; ++col)
                acc[t][col] += av * wr[col];
            }
          }
        }
        for (size_t t = 0; t < 4; ++t)
          for (size_t col = 0; col < 4; ++col)
            c[(i0 + t) * w->n + 4 * g + col] = acc[t][col];
      }
    }
    return SPARSE_OK;;
}
