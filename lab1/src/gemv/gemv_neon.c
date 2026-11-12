#include "kernels.h"
#include "stdio.h"
#include <arm_neon.h>

void gemv_neon(const float *a, const float *x, float *y, size_t m, size_t k) {
    /* TODO 4 — NEON GEMV
       Arguments and result layout are the same as gemv_scalar: row-major
       A[M,K], vector x[K], and output y[M]. For each row, multiply groups of
       four A values and x values and accumulate in a float32x4_t; horizontally
       sum its four lanes, then include any K%4 remaining columns.
       Pseudocode:
         for each row i:
             vector_sum = [0,0,0,0]; j = 0
             while j+4 <= K: vector_sum += load4(Arow+j) * load4(x+j); j += 4
             sum = horizontal_sum(vector_sum)
             while j < K: sum += Arow[j] * x[j]; j += 1
             y[i] = sum */
    /* The placeholder does not read these inputs/dimensions yet. The casts
       suppress unused-parameter warnings; remove them when implementing. */

       for (size_t i = 0; i < m; ++i) {
            //printf("Siamo dentro\n");
            float sum=0;
            float32x4_t vec_sum = vdupq_n_f32(0.0f);
            size_t j = 0;
            //printf("Vec sum definito\n");
            
            while(j+4 <= k){
                    //printf("Dentro il primo ciclo\n");
                    vec_sum = vfmaq_f32(vec_sum, vld1q_f32(a + i*k + j), vld1q_f32(x + j));
                    j += 4;
            }

            sum = vaddvq_f32(vec_sum);

            while (j<k){
                //printf("Dentro tail\n");
                sum += a[i*k + j] * x[j];
                j++;
            }

            
           y[i] = sum;
           
    }
}
