#include "AsciiColors.h"
#include <stdio.h>
// Reusable function to print bytes with their total count
void print_bytes(const void *ptr, size_t size) {
  const unsigned char *byte_ptr = (const unsigned char *)ptr;

  // Explicitly output the total byte count first
  printf("%s took %zu bytes: ", AC_YELLOW, size);

  for (size_t i = 0; i < size; i++) {
    printf("%02x ", byte_ptr[i]);
  }
  printf("\n");
}
