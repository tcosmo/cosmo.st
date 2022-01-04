int* f() {
  int val = 3;
  return &val;
}

void main() {
  int* pointer_to_val = f();
  printf("%d\n", *pointer_to_val);
}