int sum_and_diff(int a, int b, int* ret_diff) {
  *ret_diff = a - b;
  return a + b;
}

void main() {
  int a = 2;
  int b = 3;
  int diff = 0;

  int sum = sum_and_product(a, b, &diff);

  printf("%d %d\n", sum, diff);
}