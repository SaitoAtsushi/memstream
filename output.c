
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
  HANDLE thread_handle;
  size_t size;
  char* data;
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
  rargs->size = total_size;
  rargs->data = data_block;

  _endthread();
}

struct handle_info_node {
  HANDLE write_handle;
  struct reader_args* reader;
  struct handle_info_node* next;
};

static struct handle_info_node* handle_store = NULL;
static struct handle_info_node** handle_info_head = &handle_store;

static struct handle_info_node*
make_handle_info_node(HANDLE wh, struct reader_args* ra) {
  struct handle_info_node* n = malloc(sizeof(struct handle_info_node));
  n->write_handle = wh;
  n->reader = ra;
  n->next = NULL;
  return n;
}

static void push_handle_info(HANDLE wh, struct reader_args* ra) {
  struct handle_info_node* n = make_handle_info_node(wh, ra);
  n->next = *handle_info_head;
  *handle_info_head = n;
}

static struct reader_args* pop_handle_info(HANDLE wh) {
  for(struct handle_info_node** i= handle_info_head; *i; i=&((*i)->next)) {
    if((*i)->write_handle == wh) {
      struct reader_args* ra = (*i)->reader;
      (*i) = (*i)->next;
      return ra;
    }
  }
  return NULL;
}

static HANDLE CreateWriteMemoryFile(void) {
  HANDLE rp, wp;
  BOOL result = CreatePipe(&rp, &wp, NULL, 0);
  if(!result) return INVALID_HANDLE_VALUE;
  struct reader_args* rargs = malloc(sizeof(struct reader_args));
  rargs->hReadPipe = rp;
  rargs->data = NULL;
  rargs->size = 0;
  HANDLE th = (HANDLE)_beginthread(reader, 0, rargs);
  rargs->thread_handle = th;
  push_handle_info(wp, rargs);
  return wp;
}

#include <io.h>
#include <fcntl.h>

FILE* open_output_memstream(void) {
  HANDLE mfh = CreateWriteMemoryFile();
  int fd = _open_osfhandle((int)mfh, _O_WRONLY);
  return _fdopen(fd, "w");
}

char* mclose(FILE* wf, size_t* plen) {
  HANDLE wh = (HANDLE)_get_osfhandle(_fileno(wf));
  struct reader_args* ra = pop_handle_info(wh);
  char* data;
  if(ra) {
    fclose(wf);
    WaitForSingleObject(ra->thread_handle, INFINITE);
    *plen = ra->size;
    data = ra->data;
    free(ra);
  } else {
    data = NULL;
  }
  return data;
}
