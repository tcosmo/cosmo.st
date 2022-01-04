#include <stdint.h>
#include <stdio.h>

typedef struct {
  uint8_t h, w;
  int m[100][100];
} Matrix;

void print_matrix(const Matrix* A) {
  for (int i = 0; i < A->h; i += 1) {
    for (int j = 0; j < A->w; j += 1)
      printf("%d ", A->m[i][j]);
    printf("\n");
  }
}

Matrix multiply_scalar(const Matrix* A, int scalar) {
  Matrix to_return;
  to_return.h = A->h;
  to_return.w = A->w;

  for (int i = 0; i < A->h; i += 1)
    for (int j = 0; j < A->w; j += 1)
      to_return.m[i][j] = A->m[i][j] * scalar;

  return to_return;
}

void multiply_scalar_inplace(Matrix* A, int scalar) {
  for (int i = 0; i < A->h; i += 1)
    for (int j = 0; j < A->w; j += 1)
      A->m[i][j] *= scalar;
}

void main() {
  Matrix A;
  A.h = 10;
  A.w = 7;

  for (int i = 0; i < A.h; i += 1)
    for (int j = 0; j < A.w; j += 1)
      A.m[i][j] = i * j;

  print_matrix(&A);
  printf("\n");

  Matrix A_times_two = multiply_scalar(&A, 2);
  print_matrix(&A_times_two);
  printf("\n");

  multiply_scalar_inplace(&A, 3);
  print_matrix(&A);
}