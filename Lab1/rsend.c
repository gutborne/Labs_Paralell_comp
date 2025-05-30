#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv){
    int numtasks, rank, dest, source, rc, count, tag = 1;
    char my_buffer, message = 'x'; //inmsg: in message oumsg: out message
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank == 0){
        dest = source = 1;
        MPI_Recv(&my_buffer, 1, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
    }else if(rank == 1){
        dest = source = 0;
        MPI_Rsend(&message, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
    }
    MPI_Get_count(&status, MPI_CHAR, &count);
    printf("Process %d: received %d char from process %d with tag %d.\n", rank, count, status.MPI_SOURCE, status.MPI_TAG);
    printf("Process %d received %c from process %d.", rank, my_buffer, status.MPI_SOURCE);
    MPI_Finalize();
    return 0;
}

