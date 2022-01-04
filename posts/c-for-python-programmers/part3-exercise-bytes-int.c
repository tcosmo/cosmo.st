#include <limits.h>
#include <stdio.h>

void print_bytes_of_int(int x) {
  unsigned char* b = (unsigned char*)(&x);
  for (int i = 0; i < sizeof(x); i += 1) {
    printf("%x ", *(b + i));
  }
  printf("\n");
}

void main() {
  print_bytes_of_int(12);
  print_bytes_of_int(256);
  print_bytes_of_int(INT_MAX);  // <limits.h>
}