#include "minicrt.h"

long mini_crt_init_io(){
  return 1;
}

static long open(const char* filepath, long flags, long mode){
  long fd = 0;
  asm(
      "movq $2, %%rax   \n\t"
      "movq %1, %%rbx   \n\t"
      "movq %2, %%rcx   \n\t"
      "movq %3, %%rdx   \n\t"
      "int $0x80        \n\t"
      "movq %%rax, %0   \n\t"
      :"=m"(fd):"m"(filepath), "m"(flags), "m"(mode));

}

static long read(long fd, void* buffer, long size){
  long ret = 0;
  asm("movl $0, %%eax   \n\t"
      "movl %1, %%ebx   \n\t"
      "movl %2, %%ecx   \n\t"
      "movl %3, %%edx   \n\t"
      "int $0x80         \n\t"
      "movl %%eax, %0   \n\t"
      :"=m"(ret):"m"(fd), "m"(buffer), "m"(size)
      );
  return ret;
}


static long write(long fd, const void* buffer, long size){
  long ret = 0;
  asm("movl $1, %%eax   \n\t"
      "movl %1, %%ebx   \n\t"
      "movl %2, %%ecx   \n\t"
      "movl %3, %%edx   \n\t"
      "int $0x80        \n\t"
      "movl %%eax, %0   \n\t"
      :"=m"(ret):"m"(fd), "m"(buffer), "m"(size)
      );
  return ret;
  
}

static long close(long fd){
  long ret = 0;
  asm("movq $3, %%rax   \n\t"
      "movq %1, %%rbx   \n\t"
      "int $0x80        \n\t"
      "movl %%eax, %0   \n\t"
      :"=m"(ret):"m"(fd)
      );
  return ret;
}

static long seek(long fd, long offset, long mode){
  long ret = 0;
  asm("movl $8, %%eax   \n\t"
      "movl %1, %%ebx   \n\t"
      "movl %2, %%ecx   \n\t"
      "movl %3, %%edx   \n\t"
      "int $0x80        \n\t"
      "movl %%eax, %0   \n\t"
      :"=m"(ret):"m"(fd), "m"(offset), "m"(mode)
      );
  return ret;
}

FILE* fopen(const char* filename, const char* mode){
  long fd = -1;
  long flags = 0;
  long access = 00700;
#define O_RDONLY           00
#define O_WRONLY           01
#define O_RDWR             02
#define O_CREATE         0100 
#define O_TRUNC         01000
#define O_APPEND        02000
  if(strcmp(mode, "w") == 0)
    flags |= O_WRONLY | O_CREATE | O_TRUNC;
  if(strcmp(mode, "w+") == 0)
    flags |= O_RDWR | O_CREATE | O_TRUNC;
  if(strcmp(mode, "r") == 0)
    flags |= O_RDONLY | O_CREATE | O_TRUNC;
  if(strcmp(mode, "r+"))
    flags |= O_RDWR | O_CREATE;
  fd = open(filename, flags, access);
  return (long*)fd;
}

long fread(void* buffer, long size, long count , FILE* stream){
  return read((long)stream, buffer, size*count);
}

long fwrite(const void* buffer, long size, long count, FILE* stream){
  return write((long)stream, buffer, size*count);
}

long fclose(FILE* pfile){
  return close((long)pfile);
}

long fseek(FILE* pfile, long offset, long set){
  return seek((long)pfile, offset, set);
}

