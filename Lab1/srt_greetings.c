
#include <stdio.h>
#include <string.h> // For strlen /
#include <mpi.h> // For MPI functions, etc


int main(int argc, char *argv[]){
    const int MAX_STRING = 100;
    char greeting[MAX_STRING];
    int total_process; // Number of processes/
    int my_rank; // My process rank 
    MPI_Init(&argc, &argv);
    double start = MPI_Wtime();
    MPI_Comm_size(MPI_COMM_WORLD, &total_process);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Status status;   
    if (my_rank != 0) {
        sprintf(greeting, "Greetings from process %d of %d!\n", my_rank, total_process);
        MPI_Send(greeting, strlen(greeting)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    else {
        printf("Greetings from process %d of %d!\n", my_rank, total_process);
        for (int q = 1; q < total_process; q++){
            MPI_Recv(greeting, MAX_STRING, MPI_CHAR, q, 0, MPI_COMM_WORLD, &status);
            printf("%s\n", greeting);
        } //for
    }//else
    double ending = MPI_Wtime();
    MPI_Finalize();
    double time_elapsed = ending - start;
    printf("%lf", time_elapsed);
    return 0;
}
