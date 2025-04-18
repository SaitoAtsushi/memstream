#include <stdio.h>
#include <stdlib.h>
#include "memstream.h"

int fprint_iota(FILE* fp, int count, int start, int step) {
  int total=0;
  for(int i=0; i<count; i++, start+=step) total += fprintf(fp, "%d ", start);
  fprintf(fp, "\n");
  return total;
}

char* make_string_iota(int count, int start, int step) {
  FILE* fp=open_output_memstream();
  fprint_iota(fp, count, start, step);
  size_t len;
  char* str = mclose(fp, &len);
  return str;
}

int main(void) {
  fprint_iota(stdout, 10, 10, -1);
  char* str=make_string_iota(10, 0, 1);
  printf("%s\n", str);
  free(str);
  return 0;
}
