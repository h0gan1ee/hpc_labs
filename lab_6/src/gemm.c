#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#ifdef MPI
#include <mpi.h>
#endif

#if defined(ABLAS) || defined(DBG)
#include <Accelerate/Accelerate.h>
#endif

#define WARM_UP 8
#define N 16
#define THREAD_NUM 4

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

void dgemm_partial(Matrix *m1, Matrix *m2, Matrix *m3, int row_s, int row_e)
{
    int n = m2->col, k = m1->col;

    for (int i = row_s; i < row_e; ++i)
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
} dgemm_partial_args_t;

Matrix *g_m1, *g_m2, *g_m3;

void dgemm_multi_partial(dgemm_partial_args_t *args)
{
#ifdef DBG
    printf("\n[ARGS] s: %d, e: %d\n", args->s, args->e);
#endif

    dgemm_partial(g_m1, g_m2, g_m3, args->s, args->e);

    free(args);
}

void dgemm_multi(Matrix *m1, Matrix *m2, Matrix *m3)
{
    g_m1 = m1, g_m2 = m2, g_m3 = m3;

    int row_size = m3->row / THREAD_NUM;
    pthread_t thrds[THREAD_NUM];

    int i, cnt;
    for (i = 0, cnt = 0; cnt < THREAD_NUM - 1; i += row_size, ++cnt)
    {
        int j = i + row_size;
        dgemm_partial_args_t *args = (dgemm_partial_args_t *)malloc(sizeof(dgemm_partial_args_t));
        *args = (dgemm_partial_args_t){i, j};
        pthread_create(&thrds[cnt], NULL, (void *)dgemm_multi_partial, args);
    }
    dgemm_partial_args_t *args = (dgemm_partial_args_t *)malloc(sizeof(dgemm_partial_args_t));
    *args = (dgemm_partial_args_t){i, m3->row};
    pthread_create(&thrds[cnt], NULL, (void *)dgemm_multi_partial, args);

    for (int i = 0; i < THREAD_NUM; ++i)
    {
        pthread_join(thrds[i], NULL);
    }
}

void dgemm_omp(Matrix *m1, Matrix *m2, Matrix *m3)
{
    int m = m1->row, n = m2->col, k = m1->col;

#pragma omp parallel for
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

#ifdef MPI
enum COMM_TAG
{
    JOB_TAG = 0,
    RESULT_TAG = 1,
};

typedef struct
{
    Matrix *m1, *m2, *m3;
    int row_s, row_e;
} dgemm_mpi_args_t;

void gemm_mpi_worker()
{
    Matrix m1, m2;
    int args[3];
    MPI_Bcast(args, 3, MPI_INT, 0, MPI_COMM_WORLD);
    m1.row = args[0], m2.col = args[1], m1.col = m2.row = args[2];
    int size1 = m1.row * m1.col, size2 = m2.row * m2.col;
    m1.val = (double *)malloc(size1 * sizeof(double)), m2.val = (double *)malloc(size2 * sizeof(double));
    MPI_Bcast(m1.val, size1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(m2.val, size2, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Recv(args, 2, MPI_INT, 0, JOB_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    int row_s = args[0], row_e = args[1];

#ifdef DBG
    printf("\n[ARGS] s: %d, e: %d\n", row_s, row_e);
#endif

    int n = m2.col, k = m1.col;

    int buf_len = (row_e - row_s) * m2.col + 1;
    double *buf = (double *)malloc(buf_len * sizeof(double));
    memset(buf, 0, buf_len * sizeof(double));
    buf[0] = row_s * n;
    int offset = -row_s * n + 1;

    for (int i = row_s; i < row_e; ++i)
    {
        for (int kk = 0; kk < k; ++kk)
        {
            for (int j = 0; j < n; ++j)
            {
                buf[i * n + j + offset] += m1.val[i * k + kk] * m2.val[kk * n + j];
            }
        }
    }

    MPI_Send(buf, buf_len, MPI_DOUBLE, 0, RESULT_TAG, MPI_COMM_WORLD);
    free(buf);
}

void dgemm_mpi(Matrix *m1, Matrix *m2, Matrix *m3, int world_size)
{
    int row_size = m3->row / world_size;
    int size1 = m1->row * m1->col, size2 = m2->row * m2->col;

    int args[] = {m3->row, m3->col, m1->col};

    double m2_trans_val = malloc(size2 * sizeof(double));
    for (int i = 0; i < m2->row; ++i)
    {
        for (int j = 0; j < m2->col; ++j)
        {
            m2_trans_val[j * m2->col + i] = m2->val[i * m2->col + j];
        }
    }

    MPI_Bcast(args, 3, MPI_INT, 0, MPI_COMM_WORLD);
    // MPI_Bcast(m1->val, size1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    // MPI_Bcast(m2->val, size2, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int i, cnt;
    for (i = 0, cnt = 1; cnt < world_size; i += row_size, ++cnt)
    {
        int j = i + row_size;
        int args[] = {i, j};
        MPI_Send(args, 2, MPI_INT, cnt, JOB_TAG, MPI_COMM_WORLD);
        MPI_Send(&(m1->val[i * m1.col]), (j - i) * m1.col, MPI_DOUBLE, cnt, )
    }
    dgemm_partial(m1, m2, m3, i, m3->row);

    for (i = 1; i < world_size; ++i)
    {
        int len;
        MPI_Status status;
        MPI_Probe(MPI_ANY_SOURCE, RESULT_TAG, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_DOUBLE, &len);
        double *buf = (double *)malloc(sizeof(double) * len);
        MPI_Recv(buf, len, MPI_DOUBLE, MPI_ANY_SOURCE, RESULT_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int start_index = (int)buf[0];
        memcpy(&(m3->val[start_index]), &buf[1], (len - 1) * sizeof(double));
    }
}
#endif

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
#ifdef MPI
    MPI_Init(NULL, NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    if (world_size < 2)
    {
        if (world_rank == 0)
        {
            printf("\n[ERROR] Too few world(s)\n");
        }
        MPI_Finalize();
        return 1;
    }

    if (world_rank > 0)
    {
        int kase;
        MPI_Bcast(&kase, 1, MPI_INT, 0, MPI_COMM_WORLD);
        int round = kase * (WARM_UP + N);
        while (round--)
        {
            gemm_mpi_worker();
        }
        MPI_Finalize();
        return 0;
    }
#endif

    freopen("./test_case.in.txt", "r", stdin);
    int kase;
    scanf("%d", &kase);
#ifdef MPI
    MPI_Bcast(&kase, 1, MPI_INT, 0, MPI_COMM_WORLD);
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

#ifdef DBG
        Matrix m3_r;
        m3_r.row = m1.row, m3_r.col = m2.col;
        m3_r.val = (double *)malloc(size3 * sizeof(double));
        memset(m3_r.val, 0, size3 * sizeof(double));

        dgemm(&m1, &m2, &m3_r);
        dgemm_mpi(&m1, &m2, &m3, world_size);

        for (int i = 0; i < size3; ++i)
        {
            if (fabs(m3.val[i] - m3_r.val[i]) > 1e-6)
            {
                printf("\n[NOT GOOD]\n");

                printf("\n[Matrix MPI]\n");
                for (int j = 0; j < m3.row; ++j)
                {
                    for (int jj = 0; jj < m3.col; ++jj)
                    {
                        printf("%lf, ", m3.val[j * m3.col + jj]);
                    }
                    printf("\n");
                }

                printf("\n\n[Matrix USER]\n");
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
#ifdef OMP
            dgemm_omp(&m1, &m2, &m3);
#endif
#ifdef MPI
            dgemm_mpi(&m1, &m2, &m3, world_size);
#endif
#ifdef ABLAS
            cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m1.row, m2.col, m1.col, 1, m1.val, m1.col, m2.val, m2.col, 1, m3.val, m3.col);
#endif
#ifdef MULTI
            dgemm_multi(&m1, &m2, &m3);
#endif
        }

        unsigned long time_start, time_end;
        time_start = get_time();
        for (int i = 0; i < N; ++i)
        {
#ifdef USER
            dgemm(&m1, &m2, &m3);
#endif
#ifdef OMP
            dgemm_omp(&m1, &m2, &m3);
#endif
#ifdef MPI
            dgemm_mpi(&m1, &m2, &m3, world_size);
#endif
#ifdef ABLAS
            cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m1.row, m2.col, m1.col, 1, m1.val, m1.col, m2.val, m2.col, 1, m3.val, m3.col);
#endif
#ifdef MULTI
            dgemm_multi(&m1, &m2, &m3);
#endif
        }
        time_end = get_time();

        printf("\n[DURATION] %lu us\n", time_end - time_start);

        free(m1.val);
        free(m2.val);
        free(m3.val);
    }
#ifdef MPI
    MPI_Finalize();
#endif
    return 0;
}