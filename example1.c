
#include <stdio.h>
#include <string.h>
#include "memstream.h"

int main(void) {
  const char* sample_str = "Hello, This is a sample that accesses memory via the stream.";
  int ch;
  char *d;
  size_t n = 0;
  FILE* fp = open_input_memstream(sample_str, strlen(sample_str)+1);
  FILE* wp = open_output_memstream(&d, &n);
  while((ch=getc(fp)) != EOF) fputc(ch, wp);
  fclose(fp);
  mclose(wp);
  printf("\"%s\"\n", d);
  printf("size = %d", n);
  return 0;
}
