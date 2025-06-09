#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>

MPI_Status status;

void print_helper_send(int* reg_ids, int count, int my_rank, int next_rank){
    printf("rank %d will send the array: [", my_rank);
    for(int i = 0; i < count; i++ ){
        if(i == count - 1)
            printf("%d", reg_ids[i]);
        else
            printf("%d, ", reg_ids[i]);
    }
    printf("] located at %p to rank %d\n", reg_ids, next_rank);
}

void print_helper_receive(int* reg_ids, int count, int my_rank, int prev_rank){
    printf("rank %d received the array: [", my_rank);
    for(int i = 0; i < count; i++ ){
        if(i == count - 1)
            printf("%d", reg_ids[i]);
        else
            printf("%d, ", reg_ids[i]);
    }
    printf("] located at %p from rank %d\n", reg_ids, prev_rank);

}
void helper_rank_even(int* reg_ids, int* aux_reg, int my_rank, int total_processes, int count, int tam_reg){
    int next_rank = (my_rank + 1) % total_processes;
    int prev_rank = my_rank - 1;
    if(my_rank == 0) prev_rank = total_processes - 1; // receive from the last process, that is, the third
    
    MPI_Send(reg_ids, count, MPI_INT, next_rank, 0, MPI_COMM_WORLD);
    print_helper_send(reg_ids, count, my_rank, next_rank);
    MPI_Recv(reg_ids, count, MPI_INT, prev_rank, 0, MPI_COMM_WORLD, &status);
    print_helper_receive(reg_ids, count, my_rank, prev_rank);
    reg_ids[0] += 1;//how many elements we have currently in the register
    reg_ids[count] = my_rank;
}

void helper_rank_odd(int* reg_ids, int* aux_reg, int my_rank, int total_processes, int count, int tam_reg){
    int next_rank = (my_rank + 1) % total_processes;
    int prev_rank = my_rank - 1;
    
    MPI_Recv(aux_reg, count, MPI_INT, prev_rank, 0, MPI_COMM_WORLD, &status);
    print_helper_receive(aux_reg, count, my_rank, prev_rank);
    print_helper_send(reg_ids, count, my_rank, next_rank);
    MPI_Send(reg_ids, count, MPI_INT, next_rank, 0, MPI_COMM_WORLD);
    
    aux_reg[0] += 1;
    aux_reg[count] = my_rank;
    memcpy(reg_ids, aux_reg, tam_reg * sizeof(int));
}
void circular_topology(int* reg_ids, int* aux_reg, int my_rank, int total_processes, int count, int tam_reg){
    if(my_rank % 2 == 0){//check if the current rank is even
        helper_rank_even(reg_ids, aux_reg, my_rank, total_processes, count, tam_reg);
    }else{
        helper_rank_odd(reg_ids, aux_reg, my_rank, total_processes, count, tam_reg);
    }
}
int main(int argc, char* argv[]){
    int my_rank, total_processes; 
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &total_processes);
    double start = MPI_Wtime();
    int count = 2; //how many elements we have currently
    int tam_reg = 8; 
    int* reg_ids = calloc(tam_reg, sizeof(int));
    int* aux_reg = calloc(tam_reg, sizeof(int)); 
    reg_ids[0] = 1;
    reg_ids[count-1] = my_rank;
    for(int i = 0; i < 4; i++){
        printf("                          %d ROUND.\n", i+1);
        printf("--->Im rank %d and my reg_ids is pointing to: %p\n", my_rank, reg_ids);
        if(my_rank % 2 != 0)
            printf("--->Im rank %d and my aux_reg is pointing to: %p\n", my_rank, aux_reg);
            circular_topology(reg_ids, aux_reg, my_rank, total_processes, count, tam_reg);
        count++;
    }
    double ending = MPI_Wtime();
    double time_elapsed = ending - start;
    printf("RANK %d: FINISHED WITH %lf SECONDS.\n", my_rank, time_elapsed);
    MPI_Finalize();
    return 0;
}


