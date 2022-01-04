#include <stdio.h>

int main() {
  FILE* file_ptr = fopen("inexistant_file.txt", "r");

  if (file_ptr == NULL) {
    printf("Couldn't open the file!");
    return -1;  // return error code
  }

  fclose(file_ptr);
  return 0;
}