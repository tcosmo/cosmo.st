#include <stdio.h>

void f(int arr[]) {
  printf("%ld\n", sizeof(arr));
  arr[0] = 9000;
}

void main() {
  int* pointer;
  int array[5] = {123, 23, 343, 87, 45};
  printf("%ld %ld\n", sizeof(pointer), sizeof(array));
  printf("%d %d\n", array[0], *array);
  printf("%d %d\n", array[2], *(array + 2));

  printf("%d\n", array[0]);
  f(array);
  printf("%d\n", array[0]);
}