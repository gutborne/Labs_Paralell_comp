#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(){
    int a = 1;
    int b = a + 1;
    printf("valor de b: %d valor de a: %d", b, a);
    return 0;
}
