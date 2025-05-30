
#include <stdio.h>
#include <string.h> // For strlen /
#include <mpi.h> // For MPI functions, etc
#include <stdlib.h>

int main(int argc, char *argv[]){
    const int MAX_STRING = 100;
    char greeting[MAX_STRING];
    int total_process; // Number of processes/
    int my_rank; // My process rank
    
    MPI_Init(&argc, &argv);
    int buffer_size = 20*sizeof(double) + MPI_BSEND_OVERHEAD; 
    void* buffer = malloc(buffer_size);

    MPI_Buffer_attach(buffer, buffer_size);
    
    double start = MPI_Wtime();
    MPI_Comm_size(MPI_COMM_WORLD, &total_process);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Status status;   
    if (my_rank != 0) {
        sprintf(greeting, "Greetings from process %d of %d!\n", my_rank, total_process);
        MPI_Bsend(greeting, strlen(greeting)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        MPI_Buffer_detach(&buffer, &buffer_size);
        free(buffer);
    }
    else {
        printf("Greetings from process %d of %d!\n", my_rank, total_process);
        for (int q = 1; q < total_process; q++){
            MPI_Recv(greeting, MAX_STRING, MPI_CHAR, q, 0, MPI_COMM_WORLD, &status);
            printf("%s\n", greeting);
        } //for
    }//else
    double ending = MPI_Wtime();
    double time_elapsed = ending - start;
    printf("process %d: last %f seconds!", my_rank, time_elapsed);
    MPI_Finalize();
    return 0;
}
