#include "sparse_gemm.h"
#include <stdio.h>
#include <arm_neon.h>

int gemm_sparse_openmp_tiled_neon(const float *a, const sparse_weights *w,
                                  float *c, size_t m)
{
    (void)a; (void)w; (void)c; (void)m;
    /* TODO: combine the row-tile reuse in gemm_sparse_tiled with the
       four-column NEON update in gemm_sparse_neon. For each independent row
       tile, keep one float32x4_t accumulator per row and output-column group.
       Load each retained 4-value weight row once, then update all rows in the
       four-row tile with vfmaq_f32. Parallelize only the outer row-tile loop
       with a plain '#pragma omp parallel for'. The row tile is fixed at four;
       the benchmark tests thread counts 1/2/4. Use the same packed block
       traversal and ordering as the other kernels. */
    // fputs("TODO: implement tiled NEON sparse GEMM with OpenMP\n", stderr);
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (size_t i = 0; i < m; ++i) {                    
        for (size_t g = 0; g < w->n / 4; ++g) {         
            float32x4_t acc = vdupq_n_f32(0.0f);        
            for (size_t p = w->block_col_ptr[g]; p < w->block_col_ptr[g + 1]; ++p) {   
                size_t k0 = 4 * (size_t)w->block_row_idx[p];                           
                for (size_t r = 0; r < 4; ++r) {       
                    float av;
                    av = a[i * w->k + k0 + r];                                   
                    float32x4_t wv = vld1q_f32(&w->block_values[16 * p + 4 * r]);      
                    float32x4_t av4 = vdupq_n_f32(av);                                 
                    acc = vfmaq_f32(acc, wv, av4);   
                }                                  
                vst1q_f32(&c[i * w->n + 4 * g], acc); 
            }      
        }
    }



    return SPARSE_OK;
}
