#include <stdio.h>

int main() {
  FILE* blank_file = fopen("ff.bin", "wb");

  unsigned char blank_chunk = 0xFF;
  long int file_size = 1024*1024;

  for( int i = 0; i < file_size; i++ ) {
    fwrite(&blank_chunk, sizeof(blank_chunk), 1, blank_file);
  }

  return 0;
}
