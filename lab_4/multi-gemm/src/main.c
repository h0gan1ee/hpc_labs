#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#if defined(ABLAS) || defined(DBG)
#include <Accelerate/Accelerate.h>
#endif

#define WARM_UP 1
#define N 1
#define THREAD_NUM 8

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

typedef struct
{
    int s, e;
} dgemm_partial_args;

Matrix *g_m1, *g_m2, *g_m3;

void dgemm_partial(dgemm_partial_args *args)
{
#ifdef DBG
    printf("\n[ARGS] s: %d, e: %d\n", args->s, args->e);
#endif

    int cur = args->s, k = g_m1->col, cc = g_m3->col;

    while (cur < args->e)
    {
        int r = cur / g_m3->col, c = cur % g_m3->col;
        int cur1 = r * k, cur2 = c;
        for (int kk = 0; kk < k; ++kk)
        {
            g_m3->val[cur] += g_m1->val[cur1] * g_m2->val[cur2];
            ++cur1, cur2 += cc;
        }
        ++cur;
    }

    free(args);
}

void dgemm_multi(Matrix *m1, Matrix *m2, Matrix *m3)
{
    g_m1 = m1, g_m2 = m2, g_m3 = m3;
    int size3 = m3->row * m3->col;

    int thrd_size = THREAD_NUM;
    int chunk_size = size3 / THREAD_NUM;
    pthread_t thrds[thrd_size];

    int i, cnt;
    for (i = 0, cnt = 0; cnt < thrd_size - 1; i += chunk_size, ++cnt)
    {
        int j = i + chunk_size;

        dgemm_partial_args *args = (dgemm_partial_args *)malloc(sizeof(dgemm_partial_args));
        *args = (dgemm_partial_args){i, j};
        pthread_create(&thrds[cnt], NULL, (void *)dgemm_partial, args);
    }
    dgemm_partial_args *args = (dgemm_partial_args *)malloc(sizeof(dgemm_partial_args));
    *args = (dgemm_partial_args){i, size3};
    pthread_create(&thrds[cnt], NULL, (void *)dgemm_partial, args);

    for (int i = 0; i < thrd_size; ++i)
    {
        pthread_join(thrds[i], NULL);
    }
}

int main()
{
    freopen("./test_case.in.txt", "r", stdin);
    int kase;
    scanf("%d", &kase);
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

#ifdef DBG
        Matrix m3_r;
        m3_r.row = m1.row, m3_r.col = m2.col;
        m3_r.val = (double *)malloc(size3 * sizeof(double));
        memset(m3_r.val, 0, size3 * sizeof(double));

        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m1.row, m2.col, m1.col, 1, m1.val, m1.col, m2.val, m2.col, 1, m3_r.val, m3_r.col);
        dgemm_multi(&m1, &m2, &m3);

        for (int i = 0; i < size3; ++i)
        {
            if (fabs(m3.val[i] - m3_r.val[i]) > 1e-6)
            {
                printf("\n[NOT GOOD]\n");

                printf("\n[Matrix 3 MULTI]\n");
                for (int j = 0; j < m3.row; ++j)
                {
                    for (int jj = 0; jj < m3.col; ++jj)
                    {
                        printf("%lf, ", m3.val[j * m3.col + jj]);
                    }
                    printf("\n");
                }

                printf("\n\n[Matrix 3 ABLAS]\n");
                for (int j = 0; j < m3_r.row; ++j)
                {
                    for (int jj = 0; jj < m3_r.col; ++jj)
                    {
                        printf("%lf, ", m3_r.val[j * m3_r.col + jj]);
                    }
                    printf("\n");
                }

                exit(1);
            }
        }

        printf("\n[PASS]\n");
        exit(0);
#endif

        for (int i = 0; i < WARM_UP; ++i)
        {
#ifdef USER
            dgemm(&m1, &m2, &m3);
#endif
#ifdef MULTI
            dgemm_multi(&m1, &m2, &m3);
#endif
#ifdef ABLAS
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
#ifdef MULTI
            dgemm_multi(&m1, &m2, &m3);
#endif
#ifdef ABLAS
            cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m1.row, m2.col, m1.col, 1, m1.val, m1.col, m2.val, m2.col, 1, m3.val, m3.col);
#endif
        }
        time_end = get_time();

        printf("\n[DURATION] %lu us\n", time_end - time_start);

        free(m1.val);
        free(m2.val);
        free(m3.val);
    }
    return 0;
}