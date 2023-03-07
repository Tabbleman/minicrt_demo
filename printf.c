#include "minicrt.h"
/**
 * @brief printf(), fputc(), fputs()
 * 
 */

long fputc(int c, FILE* stream){
  if(fwrite(&c, 1, 1, stream) != 1)
    return EOF;
  else 
    return c;
}

long fputs(const char* str, FILE* stream){
  int len = strlen(str);
  if(fwrite(str, 1, len, stream) != len)
    return EOF;
  else 
    return len;
}
#define va_list char*
#define va_start(ap,arg) (ap=(va_list)&arg+sizeof(arg))
#define va_arg(ap,t) (*(t*)(ap+=sizeof(t)-sizeof(t)))
#define va_end(ap) (ap=(va_list)0)

long vfprintf(FILE* stream, const char* format, va_list arglist){
  int translating = 0;
  int ret = 0;
  const char* p = 0;
  for(p = format; *p != 0; p++){
    switch (*p)
    {
    case '%':
      if(translating){
        //do something
        if(fputc('%', stream) < 0)
          return EOF;
        ++ret;
        translating = 0;
      }

      else 
        translating = 1;
      break;
    case 'd': // %d
      if(translating){
        char buffer[20];
        translating = 0;
        itoa(va_arg(arglist, int), buffer, 10);
        if(fputs(buffer, stream) < 0)
          return EOF;
        ret += strlen(buffer);
      }
      else if (fputc('d', stream) < 0)
        return EOF;
      else 
        ++ret;
      break;

    case 's':
      if(translating){
        const char* str = va_arg(arglist, const char*);
        translating = !translating;
        if(fputs(str, stream) < 0)  
          return EOF;
        ret += strlen(str);
      }
      else if(fputc('s', stream))
        return EOF;
      else 
        ++ret;
      break;

    // case 'c':

    //   break;
    default:
      if(translating)
        translating = 0;
      if (fputc(*p, stream) < 0)
        return EOF;
      else 
        ++ret;
      break;
    } 
  }
  return ret;
}

long printf(const char* format, ...){
  va_list(arglist);
  va_start(arglist,format);
  return vfprintf(stdout, format,arglist);
}

long fprintf(FILE* stream, const char* format, ...){
  va_list(arglist);
  va_start(arglist, format);
  return vfprintf(stream, format, arglist);
}
