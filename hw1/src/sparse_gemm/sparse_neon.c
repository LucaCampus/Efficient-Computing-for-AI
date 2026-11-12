#include "sparse_gemm.h"
#include <stdio.h>
#include <arm_neon.h>

int gemm_sparse_neon(const float *a, const sparse_weights *w, float *c, size_t m)
{
    /* TODO: for each output row and 4-column group, keep a float32x4_t
       accumulator. For each stored block/reduction row, broadcast A with
       vdupq_n_f32, load four adjacent block values with vld1q_f32, and use
       vfmaq_f32. Store once with vst1q_f32. See gemm_sparse_scalar for the
       compressed-column traversal and preserve the same block ordering. */
    // fputs("TODO: implement gemm_sparse_neon with four adjacent FP32 lanes\n", stderr);
    if (!a || !w || !c){                                
        return SPARSE_ERROR;}

    for (size_t i = 0; i < m; ++i) {                    /* [3] una riga di A/C alla volta */
        for (size_t g = 0; g < w->n / 4; ++g) {         /* [3] un gruppo di 4 colonne di C */
            float32x4_t acc = vdupq_n_f32(0.0f);        /* [4] 4 accumulatori a zero */
            for (size_t p = w->block_col_ptr[g]; p < w->block_col_ptr[g + 1]; ++p) {   /* [5] blocchi del gruppo */
                size_t k0 = 4 * (size_t)w->block_row_idx[p];                           /* [5] prima colonna di A da usare */
                for (size_t r = 0; r < 4; ++r) {                                       /* [6] le 4 righe del blocco */
                    float av = a[i * w->k + k0 + r];                                   /* [6] un elemento di A */
                    float32x4_t wv = vld1q_f32(&w->block_values[16 * p + 4 * r]);      /* [6] 4 pesi della riga r */
                    float32x4_t av4 = vdupq_n_f32(av);                                 /* [6] av copiato in 4 lane */
                    acc = vfmaq_f32(acc, wv, av4);                                     /* [6] acc += wv * av4 */
                }
            }
            vst1q_f32(&c[i * w->n + 4 * g], acc);       /* [7] scrive le 4 uscite una sola volta */
        }
    }
    return SPARSE_OK;
}
