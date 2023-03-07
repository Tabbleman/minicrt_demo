#include "minicrt.h"

typedef struct _heap_header {
  enum {
    HEAP_BLOCK_FREE = 0xABABABAB,
    HEAP_BLOCK_USED = 0xCDCDCDCD
  } type;
  long size;
  struct _heap_header* next;
  struct _heap_header* prev;
} heap_header;

#define ADDR_ADD(a,o) (((char*)(a))+(o))
#define HEADER_SIZE (sizeof( heap_header))
static heap_header* list_head = NULL;
void free(void* memory) {
  heap_header* header = (heap_header*)ADDR_ADD(memory, -HEADER_SIZE);
  if (header->type != HEAP_BLOCK_USED) return;
  header->type = HEAP_BLOCK_FREE;
  if(header->prev != NULL && header->prev->type == HEAP_BLOCK_FREE){
    header->prev->next = header->next;
    if(header->next != NULL)
      header->next->prev = header->prev;
    header->prev->size += header->size;
    header = header->prev;
  }
  if(header->next != NULL && header->next->type == HEAP_BLOCK_FREE){
    header->size += header->next->size;
    header->next = header->next->next;
  }
}


void* malloc(unsigned long size) {
  heap_header* header;
  if(size == 0)
    return ;

  header = list_head;
  while(header != NULL){
    if(header->type == HEAP_BLOCK_USED){
      header = header->next;
      continue;
    }

    if( header->size > size + HEADER_SIZE &&
        header->size <= size + HEADER_SIZE * 2)
    {
      header->type = HEAP_BLOCK_USED;
    }

    if(header->size > size + HEADER_SIZE * 2){
      heap_header* next = header->next;
      next->next = header->next;
      next->prev = header;
      next->type = HEAP_BLOCK_FREE;
      next->size = header->size - (size + HEADER_SIZE);
      header->next = next;
      header->size = size + HEADER_SIZE;
      header->type = HEAP_BLOCK_USED;
      return ADDR_ADD(header, HEADER_SIZE);
    }
    header = header->next;
  }
  return NULL;
}

static long brk(void* end_data_segment){
  long ret = 0;
  
  asm(
      "movq $12 , %%rax    \n\t"
      "movq %1  , %%rbx    \n\t"
      "int $0x80            \n\t"
      "movq %%rax, %0      \n\t"
      : "=r"(ret):"m"(end_data_segment));
  
}

int mini_crt_heap_init() {
  void* base = NULL;
  heap_header* header = NULL;
  // 32MB heap size
  long heap_size = 1024 * 1024 * 32;

  base = (void*)brk(0);
  void* end = ADDR_ADD(base, heap_size);
  header = (heap_header*)base;
  header->size = heap_size;
  header->type = HEAP_BLOCK_FREE;
  header->next = NULL;
  header->prev = NULL;

  list_head = header;
  return 1;
}
