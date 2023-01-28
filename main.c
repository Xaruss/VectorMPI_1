#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int n, i;
    double *vec, local_sum, global_sum;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        printf("Enter the number of elements in the vector: ");
        scanf("%d", &n);
        vec = (double *) malloc(n * sizeof(double));
        printf("Enter the elements of the vector: ");
        for (i = 0; i < n; i++)
            scanf("%lf", &vec[i]);
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    double *local_vec = (double *) malloc(n * sizeof(double) / size);
    MPI_Scatter(vec, n * sizeof(double) / size, MPI_DOUBLE, local_vec, n * sizeof(double) / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    local_sum = 0;
    for (i = 0; i < n / size; i++)
        local_sum += local_vec[i];

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("The sum of the elements in the vector is: %lf\n", global_sum);
        free(vec);
    }

    free(local_vec);
    MPI_Finalize();
    return 0;
}
