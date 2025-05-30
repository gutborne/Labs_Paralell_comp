#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void circular_topology(int my_rank, int total_processes, MPI_Status* status){
    int data = my_rank;
    int next_rank = my_rank + 1;
    int prev_rank = my_rank - 1;
    if(my_rank % 2 == 0){//check if the current rank is even
        printf("rank %d (start) data: %d\n", my_rank, data);
        if(my_rank == 0){
            prev_rank = total_processes - 1; // receive from the last process, that is, the third
            MPI_Send(&data, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD);
            printf("rank %d send %d to rank %d\n", my_rank, data, next_rank);
            MPI_Recv(&data, 1, MPI_INT, prev_rank, 0, MPI_COMM_WORLD, status);
            printf("rank %d receive %d from rank %d\n", my_rank, data, prev_rank);
            printf("rank %d (ending) data: %d\n", my_rank, data);
        }else{
            MPI_Send(&data, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD);
            MPI_Recv(&data, 1, MPI_INT, prev_rank, 0, MPI_COMM_WORLD, status);
            printf("rank %d receive %d from rank %d\n", my_rank, data, prev_rank);
            printf("rank %d (ending) data: %d\n", my_rank, data);
        }
    }else if(my_rank % 2 != 0){//check if the current rank is odd
        printf("rank %d (start) data: %d\n", my_rank, data);
        int aux_mem = data;
        if(my_rank == 3){
            next_rank = 0;
            MPI_Recv(&data, 1, MPI_INT, prev_rank, 0, MPI_COMM_WORLD, status);
            printf("rank %d receive %d from rank %d\n", my_rank, data, prev_rank);
            MPI_Send(&aux_mem, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD);
            printf("rank %d send %d to rank %d\n", my_rank, data, next_rank);
            printf("rank %d (ending) data: %d\n", my_rank, data);
        }else{
            MPI_Recv(&data, 1, MPI_INT, prev_rank, 0, MPI_COMM_WORLD, status);
            printf("rank %d receive %d from rank %d\n", my_rank, data, prev_rank);
            MPI_Send(&aux_mem, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD);
            printf("rank %d send %d to rank %d\n", my_rank, data, next_rank);
            printf("rank %d (ending) data: %d\n", my_rank, data);
        }
    }
}
int main(int argc, char* argv[]){
    int my_rank, total_processes; 
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &total_processes);
    MPI_Status status;
    circular_topology(my_rank, total_processes, &status);
    MPI_Finalize();
    return 0;
}
