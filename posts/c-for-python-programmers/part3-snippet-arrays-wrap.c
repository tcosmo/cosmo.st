#include <stdio.h>

typedef struct {
  int arr[5];
} ArrWrap;

void f(ArrWrap wrap) {
  wrap.arr[0] = 9000;
}

void main() {
  ArrWrap wrap = {.arr = {123, 23, 343, 87, 45}};
  printf("%d\n", wrap.arr[0]);
  f(wrap);
  printf("%d\n", wrap.arr[0]);
}