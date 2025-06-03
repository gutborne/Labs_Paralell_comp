#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
typedef struct RegisterIds{
    int first_pos;
    int last_pos;
    int* ids;
}RegisterIds;
MPI_Status status;

int main(int argc, char *argv[]){
    int a = 1;
    int b = a + 1;
    MPI_Init(&argc, &argv);    
    printf("valor de b: %d valor de a: %d\n", b, a);
    int* m = malloc(3 * sizeof(int));
    m[a] = 4;
    printf("valor de m[a]: %d", m[a]);
    RegisterIds* register_ids = malloc(sizeof(RegisterIds));
    MPI_Finalize();
    return 0;
}
