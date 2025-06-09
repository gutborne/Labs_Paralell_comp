#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int* change_pointer(int* a, int* b) {
    a = b;
}

void copy_data(int* a, int* b, int size) {
    memcpy(a, b, size * sizeof(int));  // This actually changes the data `a` points to
}

int main() {
    int* original = malloc(3 * sizeof(int));
    int* new_data = malloc(3 * sizeof(int));
    int a = 10;
    int* ptr = &a;
    int my_arr[3] = {1,2,3};
    printf("&ptr: %p ptr: %p &a: %p \n", &ptr, ptr, &a);
    printf("&my_arr: %p my_arr: %p \n", &my_arr, my_arr);
    
    original[0] = 1;
    original[1] = 2;
    original[2] = 3;

    new_data[0] = 10;
    new_data[1] = 20;
    new_data[2] = 30;

    printf("Before pointer change:\n");
    printf("original: %d, %d, %d\n", original[0], original[1], original[2]);

    change_pointer(original, new_data);  // this will NOT affect original

    printf("After pointer change (no effect):\n");
    printf("original: %d, %d, %d\n", original[0], original[1], original[2]);

    copy_data(original, new_data, 3);  // this WILL affect original

    printf("After data copy:\n");
    printf("original: %d, %d, %d\n", original[0], original[1], original[2]);

    free(original);
    free(new_data);
    return 0;
}
