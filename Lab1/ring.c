#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


MPI_Status status;

void print_helper_send(int* reg_ids, int count, int my_rank, int next_rank){
    printf("rank %d will send the array: [", my_rank);
    for(int i = 0; i <= count; i++ ){
        if(i != count)
            printf("%d, ", reg_ids[i]);
        else
            printf("%d", reg_ids[i]);
    }
    printf("] to rank %d.\n", next_rank);
}

void print_helper_receive(int* reg_ids, int count, int my_rank, int prev_rank){
    printf("rank %d received the array: [", my_rank);
    for(int i = 0; i <= count; i++ ){
        if(i != count)
            printf("%d, ", reg_ids[i]);
        else
            printf("%d", reg_ids[i]);
    }
    printf("] from rank %d\n", prev_rank);

}
void helper_rank_even(int* reg_ids, int my_rank, int total_processes, int count, int tam_reg){
    int next_rank = my_rank + 1;
    int prev_rank = my_rank - 1;
    int* aux_reg = malloc(sizeof(int) * tam_reg);
    if(my_rank == 0){
        prev_rank = total_processes - 1; // receive from the last process, that is, the third
        MPI_Send(reg_ids, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD);
        print_helper_send(reg_ids, count, my_rank, next_rank);
        MPI_Recv(aux_reg, 1, MPI_INT, prev_rank, 0, MPI_COMM_WORLD, &status);
        print_helper_receive(aux_reg, count, my_rank, prev_rank);
    }else{
        MPI_Send(reg_ids, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD);
        print_helper_send(reg_ids, count, my_rank, next_rank);
        MPI_Recv(aux_reg, 1, MPI_INT, prev_rank, 0, MPI_COMM_WORLD, &status);
        print_helper_receive(aux_reg, count, my_rank, prev_rank);
    }
    reg_ids[0] = count + 1;//how many elements we have currently in the register
    reg_ids[count+1] = my_rank;
    reg_ids = aux_reg;
}

void helper_rank_odd(int* reg_ids, int my_rank, int total_processes, int count, int tam_reg){
    int next_rank = my_rank + 1;
    int prev_rank = my_rank - 1;
    int* aux_reg = malloc(sizeof(int) * tam_reg);
    if(my_rank == 3){
        next_rank = 0;
        MPI_Recv(aux_reg, 1, MPI_INT, prev_rank, 0, MPI_COMM_WORLD, &status);
        print_helper_receive(reg_ids, count, my_rank, prev_rank);
        print_helper_send(reg_ids, count, my_rank, next_rank);
        MPI_Send(reg_ids, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD);
    }else{
        MPI_Recv(aux_reg, 1, MPI_INT, prev_rank, 0, MPI_COMM_WORLD, &status);
        print_helper_receive(reg_ids, count, my_rank, prev_rank);
        print_helper_send(reg_ids, count, my_rank, next_rank);
        MPI_Send(reg_ids, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD);
    }
    aux_reg[0] = count + 1;
    aux_reg[count+1] = my_rank;
    reg_ids = aux_reg;
}
void circular_topology(int* reg_ids, int my_rank, int total_processes, int count, int tam_reg){
    if(my_rank % 2 == 0){//check if the current rank is odd
        helper_rank_even(reg_ids, my_rank, total_processes, count, tam_reg);
    }else{
        helper_rank_odd(reg_ids, my_rank, total_processes, count, tam_reg);
    }
}
int main(int argc, char* argv[]){
    int my_rank, total_processes; 
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &total_processes);
    //buffer 1 -> len(buffer) 2-> 
    int count = 1; //how many elements we have currently 
    int tam_reg = 8;
    int* reg_ids = calloc(sizeof(int), tam_reg);
    reg_ids[0] = count;
    reg_ids[count] = my_rank;
    for(int i = 0; i < 3; i++){
        printf("                          %d ROUND.\n", i+1);
        circular_topology(reg_ids, my_rank, total_processes, count, tam_reg);
        count++;
    }
    MPI_Finalize();
    return 0;
}
