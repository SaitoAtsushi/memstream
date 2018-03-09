#include <windows.h>

static const int perround = 1024;

struct writer_args {
  HANDLE hWritePipe;
  LPVOID data;
  DWORD data_size;
};

#include <process.h>

static void writer(void* args) {
  struct writer_args* wargs = args;
  DWORD written_size;
  
  for(DWORD rest = wargs->data_size;
      rest>0 && WriteFile(wargs->hWritePipe,
                          wargs->data,
                          rest<perround ? rest : perround,
                          &written_size,
                          NULL);
      wargs->data += written_size, rest -= written_size);

  CloseHandle(wargs->hWritePipe);
  free(wargs);
  _endthread();
}

static HANDLE CreateMemoryFile(const LPVOID data, const DWORD data_size) {
  HANDLE rp, wp;
  BOOL result = CreatePipe(&rp, &wp, NULL, 0);
  if(!result) return INVALID_HANDLE_VALUE;
  struct writer_args* wargs = malloc(sizeof(struct writer_args));
  wargs->hWritePipe = wp;
  wargs->data = data;
  wargs->data_size = data_size;
  _beginthread(writer, 0, wargs);
  return rp;
}

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include "memstream.h"

FILE* open_input_memstream(const char* str, size_t len) {
  HANDLE mfh = CreateMemoryFile((const LPVOID) str, (DWORD) len);
  int fd = _open_osfhandle((int)mfh, _O_RDONLY);
  return _fdopen(fd, "r");
}
