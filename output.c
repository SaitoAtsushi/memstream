
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "memstream.h"

#define perround 1024

struct chunk {
  char data[perround];
  size_t size;
  struct chunk *next;
};

struct chunk* make_chunk(void) {
  struct chunk* n = malloc(sizeof(struct chunk));
  n->size = 0;
  n->next = NULL;
  return n;
}

struct reader_args {
  HANDLE hReadPipe;
  size_t* size;
  char** data;
};

#include <process.h>

static void reader(void* args) {
  struct reader_args* rargs = args;
  DWORD read_size;
  size_t total_size=0;
  struct chunk* chunk_head;
  struct chunk** hp = &chunk_head;

  for(chunk_head = *hp = make_chunk();
      ReadFile(rargs->hReadPipe, &((*hp)->data), perround, &read_size, NULL)
        && read_size;
      hp = &((*hp)->next), *hp = make_chunk()) {
    total_size += read_size;
    ((*hp)->size) = read_size;
  }

  CloseHandle(rargs->hReadPipe);

  char* data_block = malloc(total_size);
  struct chunk* i=chunk_head;
  for(size_t pos = 0; i;) {
    memcpy(&data_block[pos], i->data, i->size);
    pos+=i->size;
    struct chunk* t=i->next;
    free(i);
    i=t;
  }
  *(rargs->size) = total_size;
  *(rargs->data) = data_block;
  
  free(rargs);
  _endthread();
}

struct handle_info_node {
  HANDLE write_handle;
  HANDLE thread_handle;
  struct handle_info_node* next;
};

static struct handle_info_node* handle_store = NULL;
static struct handle_info_node** handle_info_head = &handle_store;

static struct handle_info_node* make_handle_info_node(HANDLE wh, HANDLE th) {
  struct handle_info_node* n = malloc(sizeof(struct handle_info_node));
  n->write_handle = wh;
  n->thread_handle = th;
  n->next = NULL;
  return n;
}

static void push_handle_info(HANDLE wh, HANDLE rh) {
  struct handle_info_node* n = make_handle_info_node(wh, rh);
  n->next = *handle_info_head;
  *handle_info_head = n;
}

static HANDLE pop_handle_info(HANDLE wh) {
  for(struct handle_info_node** i= handle_info_head; *i; i=&((*i)->next)) {
    if((*i)->write_handle == wh) {
      HANDLE th = (*i)->thread_handle;
      (*i) = (*i)->next;
      return th;
    }
  }
  return INVALID_HANDLE_VALUE;
}

static HANDLE CreateWriteMemoryFile(LPVOID* data, size_t* n) {
  HANDLE rp, wp;
  BOOL result = CreatePipe(&rp, &wp, NULL, 0);
  if(!result) return INVALID_HANDLE_VALUE;
  struct reader_args* rargs = malloc(sizeof(struct reader_args));
  *data = NULL;
  rargs->hReadPipe = rp;
  rargs->data = (char**)data;
  rargs->size = n;
  HANDLE th = (HANDLE)_beginthread(reader, 0, rargs);
  push_handle_info(wp, th);
  return wp;
}

#include <io.h>
#include <fcntl.h>

FILE* open_output_memstream(char** str, size_t* n) {
  HANDLE mfh = CreateWriteMemoryFile((LPVOID*) str, n);
  int fd = _open_osfhandle((int)mfh, _O_WRONLY);
  return _fdopen(fd, "w");
}

int mclose(FILE* wf) {
  HANDLE wh = (HANDLE)_get_osfhandle(_fileno(wf));
  HANDLE th = pop_handle_info(wh);
  int r = fclose(wf);
  WaitForSingleObject(th, INFINITE);
  return r;
}
