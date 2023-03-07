#pragma once
/**
 *
 *Filename: minicrt.h
 *created in 2023/03/06 20:48:06
 *Author: tabbleman
 *
 */
#ifndef MINICRT_H
#define MINICRT_H
#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL 
#define NULL 0
#endif

/**
 * memory.h
*/
void free(void* ptr);//ok
void* malloc(unsigned long );//ok
static long brk(void* end_data_segment);//ok
int mini_crt_init_heap();//ok
/**
 * string.h
*/
char* itoa(int n, char* str, long radix);
int strcmp(const char*src, const char* dst);
char* strcpy(char* dst, const char* src);
long strlen(const char* str);

/**
 * file.c and io.c
*/
typedef long FILE;

#define EOF -1
#define stdin   ((FILE*)0)
#define stdout  ((FILE*)1)
#define stderr  ((FILE*)2)

long mini_crt_init_io();
FILE* fopen(const char* filename, const char* mode);
long fread(void* buffer, long size, long count, FILE* stream);
long fwrite(const void* buffer, long size, long count, FILE* stream);
long fclose(FILE* pfile);
long fseek(FILE* pfile, long offset, long set);

/**
 * printf(..).
*/

long fputc(int c, FILE* stream);
long fputs(const char* str, FILE* stream);
long printf(const char* format, ...);
long fprintf(FILE* stream, const char* format, ...);

/**
 * internal
*/

void do_global_ctors();
void mini_crt_call_exit_routine();
typedef void (*atexit_func_t)(void );
int atexit(atexit_func_t func);

#ifdef __cplusplus
}
#endif


#endif //minicrt.h