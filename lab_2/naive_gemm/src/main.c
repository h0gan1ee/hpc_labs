#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef OBLAS
#include <cblas.h>
#endif

#ifdef ABLAS
#include <Accelerate/Accelerate.h>
#endif

#define N 114514

struct timeval;
struct timezone;

typedef struct
{
    double *val;
    int row, col;
} Matrix;

unsigned long get_time()
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    return 1000000 * tv.tv_sec + tv.tv_usec;
}

void gemm(Matrix *m1, Matrix *m2, Matrix *m3)
{
    int m = m1->row, n = m1->col, k = m2->col;
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            for (int kk = 0; kk < k; ++kk)
            {
                m3->val[i * n + j] += m1->val[i * n + kk] * m2->val[kk * n + j];
            }
        }
    }
}

int main()
{
    freopen("../test_case.in.txt", "r", stdin);
    int kase;
    scanf("%d", &kase);
#ifdef DBG
    kase = 1;
#endif
    while (kase--)
    {
        Matrix m1, m2, m3;
        scanf("%d%d", &(m1.row), &(m1.col));
        int size1 = m1.row * m1.col;
        m1.val = (double *)malloc(size1 * sizeof(double));
        for (int i = 0; i < size1; ++i)
        {
            scanf("%lf", &(m1.val[i]));
        }
        scanf("%d%d", &(m2.row), &(m2.col));
        int size2 = m2.row * m2.col;
        m2.val = (double *)malloc(size2 * sizeof(double));
        for (int i = 0; i < size2; ++i)
        {
            scanf("%lf", &(m2.val[i]));
        }

        m3.row = m1.row, m3.col = m2.col;
        int size3 = m3.row * m3.col;
        m3.val = (double *)malloc(size3 * sizeof(double));
        memset(m3.val, 0, size3 * sizeof(double));

        unsigned long time_start, time_end;
        time_start = get_time();
        for (int i = 0; i < N; ++i)
        {
#ifdef USER
            gemm(&m1, &m2, &m3);
#endif
#if defined(OBLAS) || defined(ABLAS)
            cblas_dgemm(CblasColMajor, CblasNoTrans, CblasTrans, m1.row, m2.col, m1.col, 1, m1.val, size1, m2.val, size2, 1, m3.val, size3);
#endif
        }
        time_end = get_time();

#ifdef DBG
        printf("\n[DEBUG] m3:\n");
        for (int i = 0; i < m3.row; ++i)
        {
            for (int j = 0; j < m3.col; ++j)
            {
                printf("%lf ", m3.val[i * m3.row + j]);
            }
            printf("\n");
        }
#endif

        printf("\n[DURATION] %lu us\n", time_end - time_start);

        free(m1.val);
        free(m2.val);
        free(m3.val);
    }
    return 0;
}