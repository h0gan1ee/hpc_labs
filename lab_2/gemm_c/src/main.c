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

#define WARM_UP 11
#define N 45

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

void dgemm(Matrix *m1, Matrix *m2, Matrix *m3)
{
    int m = m1->row, n = m2->col, k = m1->col;

    for (int i = 0; i < m; ++i)
    {
        for (int kk = 0; kk < k; ++kk)
        {
            for (int j = 0; j < n; ++j)
            {
                m3->val[i * n + j] += m1->val[i * k + kk] * m2->val[kk * n + j];
            }
        }
    }
}

int main()
{
    freopen("./test_case.in.txt", "r", stdin);
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

        for (int i = 0; i < WARM_UP; ++i)
        {
#ifdef USER
            dgemm(&m1, &m2, &m3);
#endif
#if defined(OBLAS) || defined(ABLAS)
            cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m1.row, m2.col, m1.col, 1, m1.val, m1.col, m2.val, m2.col, 1, m3.val, m3.col);
#endif
        }

        unsigned long time_start, time_end;
        time_start = get_time();
        for (int i = 0; i < N; ++i)
        {
#ifdef USER
            dgemm(&m1, &m2, &m3);
#endif
#if defined(OBLAS) || defined(ABLAS)
            cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m1.row, m2.col, m1.col, 1, m1.val, m1.col, m2.val, m2.col, 1, m3.val, m3.col);
#endif
        }
        time_end = get_time();

#ifdef DBG
        printf("\n[DEBUG] m1:\n");
        for (int i = 0; i < size1; ++i)
        {
            printf("%lf ", m1.val[i]);
        }
        printf("\n[DEBUG] m2:\n");
        for (int i = 0; i < size2; ++i)
        {
            printf("%lf ", m2.val[i]);
        }
        printf("\n[DEBUG] m3:\n");
        printf("size: %d\n", size3);
        for (int i = 0; i < size3; ++i)
        {
            printf("%lf ", m3.val[i]);
        }
#endif

        printf("\n[DURATION] %lu us\n", time_end - time_start);

        free(m1.val);
        free(m2.val);
        free(m3.val);
    }
    return 0;
}